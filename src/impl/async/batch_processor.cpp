/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file batch_processor.cpp
 * @brief Batch processor implementation with jthread compatibility
 * @since 1.3.0 - Refactored to use jthread compatibility layer
 *
 * This implementation uses C++20 std::jthread with std::stop_token for
 * cooperative cancellation where available, with fallback to std::thread
 * for environments without jthread support (e.g., libc++).
 */

#include "batch_processor.h"
#include "../memory/object_pool.h"
#include "jthread_compat.h"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <thread>

namespace kcenon::logger::async {

/**
 * @brief Worker thread for batch processing with jthread compatibility
 *
 * Uses std::jthread with std::stop_token for cooperative cancellation
 * where available, falls back to std::thread with manual stop mechanism
 * for environments without jthread support (e.g., libc++).
 */
class batch_processing_jthread_worker {
public:
    using process_callback = std::function<void()>;

    explicit batch_processing_jthread_worker(process_callback callback)
        : callback_(std::move(callback))
#if !LOGGER_HAS_JTHREAD
        , stop_source_(std::make_shared<simple_stop_source>())
#endif
    {}

    ~batch_processing_jthread_worker() {
        stop();
    }

    void start() {
        if (running_.exchange(true, std::memory_order_acq_rel)) {
            return;  // Already started
        }

#if LOGGER_HAS_JTHREAD
        auto callback = callback_;
        thread_ = compat_jthread([callback](std::stop_token stop_token) {
            while (!stop_token.stop_requested()) {
                if (callback) {
                    callback();
                }
                // Brief sleep to control loop frequency
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
#else
        // Reset stop source for new start
        stop_source_->reset();

        auto callback = callback_;
        auto stop = stop_source_;
        thread_ = compat_jthread([callback, stop](simple_stop_source& /*unused*/) {
            while (!stop->stop_requested()) {
                if (callback) {
                    callback();
                }
                // Brief sleep to control loop frequency
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
#endif
    }

    void stop() {
        if (!running_.exchange(false, std::memory_order_acq_rel)) {
            return;  // Already stopped
        }

        // Request stop and join thread
        thread_.request_stop();
        thread_.join();
    }

    [[nodiscard]] bool is_running() const noexcept {
        return running_.load(std::memory_order_acquire);
    }

private:
    process_callback callback_;
    compat_jthread thread_;
    std::atomic<bool> running_{false};
#if !LOGGER_HAS_JTHREAD
    std::shared_ptr<simple_stop_source> stop_source_;
#endif
};

batch_processor::batch_processor(std::unique_ptr<base_writer> writer, const config& cfg)
    : config_(cfg)
    , writer_(std::move(writer))
    , queue_(std::make_unique<lockfree_spsc_queue<batch_entry, queue_size>>())
    , current_batch_size_(cfg.initial_batch_size)
    , current_wait_time_(cfg.max_wait_time)
    , last_adjustment_time_(std::chrono::steady_clock::now()) {

    if (!writer_) {
        throw std::invalid_argument("Writer cannot be null");
    }

    // Validate configuration
    if (cfg.min_batch_size > cfg.max_batch_size) {
        throw std::invalid_argument("min_batch_size cannot be greater than max_batch_size");
    }

    if (cfg.initial_batch_size < cfg.min_batch_size || cfg.initial_batch_size > cfg.max_batch_size) {
        throw std::invalid_argument("initial_batch_size must be within min/max range");
    }
}

batch_processor::~batch_processor() {
    stop(true);
}

bool batch_processor::start() {
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true)) {
        return false; // Already running
    }

    should_stop_ = false;
    processing_worker_ = std::make_unique<batch_processing_jthread_worker>(
        [this] { process_loop_iteration(); });
    processing_worker_->start();
    return true;
}

void batch_processor::stop(bool flush_remaining) {
    if (!running_.exchange(false)) {
        return; // Already stopped
    }

    should_stop_ = true;

    if (processing_worker_) {
        processing_worker_->stop();
        processing_worker_.reset();
    }

    if (flush_remaining) {
        // Process any remaining entries
        batch_entry entry;
        std::vector<batch_entry> final_batch;
        final_batch.reserve(queue_size);

        while (queue_->dequeue(entry)) {
            final_batch.push_back(std::move(entry));
        }

        if (!final_batch.empty()) {
            process_batch(final_batch);
        }

        if (writer_) {
            writer_->flush();
        }
    }
}

bool batch_processor::add_entry(batch_entry&& entry) {
    if (!running_.load(std::memory_order_relaxed)) {
        return false;
    }

    if (!queue_->enqueue(std::move(entry))) {
        stats_.dropped_entries.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    return true;
}

bool batch_processor::add_entry(const batch_entry& entry) {
    return add_entry(batch_entry(entry));
}

void batch_processor::flush() {
    // This is handled by the processing loop
    // We could add a flush signal mechanism here if needed
}

bool batch_processor::is_healthy() const {
    return running_.load(std::memory_order_relaxed) &&
           writer_ && writer_->is_healthy();
}

size_t batch_processor::get_queue_size() const {
    return queue_->size();
}

void batch_processor::process_loop_iteration() {
    static thread_local std::chrono::steady_clock::time_point last_flush_time = std::chrono::steady_clock::now();
    static thread_local std::chrono::steady_clock::time_point last_adjustment_time = std::chrono::steady_clock::now();

    const auto batch_size = current_batch_size_.load(std::memory_order_relaxed);
    const auto wait_time = current_wait_time_.load(std::memory_order_relaxed);

    std::vector<batch_entry> current_batch;
    current_batch.reserve(batch_size);

    const auto deadline = std::chrono::steady_clock::now() + wait_time;
    const auto entries_collected = collect_entries(current_batch, batch_size, deadline);

    if (entries_collected > 0) {
        const auto process_start = std::chrono::steady_clock::now();
        const auto processed = process_batch(current_batch);
        const auto process_end = std::chrono::steady_clock::now();

        const auto processing_time = process_end - process_start;
        const bool flushed_by_size = (entries_collected >= batch_size);
        const bool flushed_by_time = should_flush_by_time(last_flush_time);

        std::string flush_reason;
        if (flushed_by_size) {
            flush_reason = "size";
            stats_.flush_by_size.fetch_add(1, std::memory_order_relaxed);
        } else if (flushed_by_time) {
            flush_reason = "time";
            stats_.flush_by_time.fetch_add(1, std::memory_order_relaxed);
        } else {
            flush_reason = "partial";
        }

        update_stats(processed, processing_time, flush_reason);
        last_flush_time = process_end;

        // Handle back-pressure
        if (config_.enable_back_pressure) {
            handle_back_pressure();
        }

        // Dynamic batch size adjustment
        if (config_.enable_dynamic_sizing) {
            const auto now = std::chrono::steady_clock::now();
            if (now - last_adjustment_time > std::chrono::seconds(5)) {
                adjust_batch_size();
                last_adjustment_time = now;
            }
        }
    }
}

size_t batch_processor::collect_entries(std::vector<batch_entry>& batch,
                                       size_t max_entries,
                                       std::chrono::steady_clock::time_point deadline) {
    size_t collected = 0;
    batch_entry entry;

    while (collected < max_entries && std::chrono::steady_clock::now() < deadline) {
        if (queue_->dequeue(entry)) {
            batch.push_back(std::move(entry));
            ++collected;
        } else {
            // Queue is empty, short wait
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }

    // Try to collect more entries if we have time left and space
    while (collected < max_entries && queue_->dequeue(entry)) {
        batch.push_back(std::move(entry));
        ++collected;
    }

    return collected;
}

size_t batch_processor::process_batch(std::vector<batch_entry>& batch) {
    if (batch.empty() || !writer_) {
        return 0;
    }

    size_t processed = 0;
    for (const auto& entry : batch) {
        if (auto result = writer_->write(entry.level, entry.message, entry.file,
                                        entry.line, entry.function, entry.timestamp)) {
            ++processed;
        }
    }

    // Flush after batch processing
    writer_->flush();

    stats_.total_batches.fetch_add(1, std::memory_order_relaxed);
    stats_.total_entries.fetch_add(processed, std::memory_order_relaxed);

    return processed;
}

void batch_processor::adjust_batch_size() {
    const auto current_size = current_batch_size_.load(std::memory_order_relaxed);
    const auto queue_size = get_queue_size();
    const auto recent_time = recent_processing_time_ms_.load(std::memory_order_relaxed);

    size_t new_size = current_size;

    // Increase batch size if queue is building up and processing is fast
    if (queue_size > current_size * 2 && recent_time < 10.0) {
        new_size = std::min(config_.max_batch_size,
                           static_cast<size_t>(current_size * config_.size_increase_factor));
    }
    // Decrease batch size if processing is slow or queue is small
    else if (recent_time > 100.0 || queue_size < current_size / 4) {
        new_size = std::max(config_.min_batch_size,
                           static_cast<size_t>(current_size * config_.size_decrease_factor));
    }

    if (new_size != current_size) {
        current_batch_size_.store(new_size, std::memory_order_relaxed);
        stats_.dynamic_size_adjustments.fetch_add(1, std::memory_order_relaxed);
    }
}

bool batch_processor::handle_back_pressure() {
    const auto queue_size = get_queue_size();

    if (queue_size > config_.back_pressure_threshold) {
        stats_.back_pressure_events.fetch_add(1, std::memory_order_relaxed);

        // Apply back-pressure delay
        std::this_thread::sleep_for(config_.back_pressure_delay);

        return queue_size < queue_size * 1.5; // Continue if queue isn't growing too fast
    }

    return true;
}

bool batch_processor::should_flush_by_time(std::chrono::steady_clock::time_point last_flush_time) const {
    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = now - last_flush_time;
    const auto current_wait = current_wait_time_.load(std::memory_order_relaxed);

    return elapsed >= current_wait;
}

void batch_processor::update_stats(size_t batch_size,
                                  std::chrono::nanoseconds processing_time,
                                  const std::string& flush_reason) {
    (void)batch_size;  // Suppress unused parameter warning
    (void)flush_reason;  // Suppress unused parameter warning
    const double processing_time_ms =
        std::chrono::duration_cast<std::chrono::microseconds>(processing_time).count() / 1000.0;

    // Update recent processing time (exponential moving average)
    const double alpha = 0.1;
    const double current_time = recent_processing_time_ms_.load(std::memory_order_relaxed);
    const double new_time = alpha * processing_time_ms + (1.0 - alpha) * current_time;
    recent_processing_time_ms_.store(new_time, std::memory_order_relaxed);

    // Update average batch size
    const auto total_batches = stats_.total_batches.load(std::memory_order_relaxed);
    if (total_batches > 0) {
        const auto total_entries = stats_.total_entries.load(std::memory_order_relaxed);
        const double avg_size = static_cast<double>(total_entries) / total_batches;
        stats_.average_batch_size.store(avg_size, std::memory_order_relaxed);
    }

    // Update average processing time
    stats_.average_processing_time_ms.store(
        recent_processing_time_ms_.load(std::memory_order_relaxed),
        std::memory_order_relaxed);

    // Update recent queue size for adjustment algorithm
    recent_queue_size_.store(get_queue_size(), std::memory_order_relaxed);
}

std::unique_ptr<batch_processor> make_batch_processor(
    std::unique_ptr<base_writer> writer,
    const batch_processor::config& cfg) {
    return std::make_unique<batch_processor>(std::move(writer), cfg);
}

} // namespace kcenon::logger::async
