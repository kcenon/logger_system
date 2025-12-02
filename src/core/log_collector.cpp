/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <kcenon/logger/core/log_collector.h>
#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/logger_interface.h>
#include <kcenon/thread/core/thread_base.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <functional>
#include <cstdio>

namespace kcenon::logger {

/**
 * @brief Worker thread class for log processing using thread_system's thread_base
 */
class log_collector_worker : public kcenon::thread::thread_base {
public:
    using process_callback = std::function<void()>;

    explicit log_collector_worker(process_callback callback)
        : thread_base("log_collector_worker")
        , process_callback_(std::move(callback))
        , has_work_(false) {}

    void notify_work() {
        has_work_.store(true, std::memory_order_release);
    }

protected:
    [[nodiscard]] auto should_continue_work() const -> bool override {
        return has_work_.load(std::memory_order_acquire);
    }

    auto do_work() -> kcenon::thread::result_void override {
        has_work_.store(false, std::memory_order_release);
        if (process_callback_) {
            process_callback_();
        }
        return {};
    }

private:
    process_callback process_callback_;
    std::atomic<bool> has_work_;
};

class log_collector::impl {
public:
    explicit impl(std::size_t buffer_size, std::size_t batch_size)
        : buffer_size_(buffer_size)
        , batch_size_(batch_size)
        , running_(false)
        , worker_(std::make_unique<log_collector_worker>([this] { process_batch(); })) {
    }

    ~impl() {
        stop();
    }

    bool enqueue(logger_system::log_level level,
                 const std::string& message,
                 const std::string& file,
                 int line,
                 const std::string& function,
                 const std::chrono::system_clock::time_point& timestamp) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // Check if queue is full
            if (queue_.size() >= buffer_size_) {
                // Track dropped message
                uint64_t dropped_count = dropped_messages_.fetch_add(1, std::memory_order_relaxed) + 1;

                // Log warning periodically (every 100 dropped messages) to avoid log spam
                if (dropped_count % 100 == 1) {
                    // Note: We can't use the logger here as it would create recursion
                    // In production, this metric should be exposed via monitoring
                    fprintf(stderr, "[WARNING] Log queue full: %llu messages dropped total\n",
                           static_cast<unsigned long long>(dropped_count));
                }

                return false;
            }

            // Create log_entry with optional source location
            log_entry entry(level, message, timestamp);
            if (!file.empty() || line != 0 || !function.empty()) {
                entry.location = source_location{file, line, function};
            }
            queue_.push(std::move(entry));
        }

        // Notify worker thread
        if (worker_) {
            worker_->notify_work();
        }
        return true;
    }

    void add_writer(std::shared_ptr<base_writer> writer) {
        if (!writer) {
            return;
        }
        std::lock_guard<std::mutex> lock(writers_mutex_);
        writers_.push_back(writer);
    }

    void clear_writers() {
        std::lock_guard<std::mutex> lock(writers_mutex_);
        writers_.clear();
    }

    void start() {
        if (!running_.exchange(true)) {
            if (worker_) {
                worker_->set_wake_interval(std::chrono::milliseconds(10));
                worker_->start();
            }
        }
    }

    void stop() {
        if (running_.exchange(false)) {
            if (worker_) {
                worker_->stop();
            }

            // Process any remaining entries
            drain_queue();

            // Flush all writers
            flush_writers();
        }
    }

    void flush() {
        if (!running_.load()) {
            // Just flush the writers
            flush_writers();
            return;
        }

        // Process all remaining entries
        drain_queue();

        // Flush all writers
        flush_writers();
    }

    std::pair<size_t, size_t> get_queue_metrics() const {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return {queue_.size(), buffer_size_};
    }

private:
    void process_batch() {
        std::vector<log_entry> batch;
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            batch.reserve(std::min(batch_size_, queue_.size()));
            while (!queue_.empty() && batch.size() < batch_size_) {
                batch.push_back(std::move(queue_.front()));
                queue_.pop();
            }
        }

        // Write batch to all writers
        for (const auto& entry : batch) {
            write_to_all(entry);
        }
    }

    void drain_queue() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        while (!queue_.empty()) {
            auto entry = std::move(queue_.front());
            queue_.pop();
            lock.unlock();

            write_to_all(entry);

            lock.lock();
        }
    }

    void flush_writers() {
        std::lock_guard<std::mutex> writer_lock(writers_mutex_);
        for (auto& weak_writer : writers_) {
            if (auto writer = weak_writer.lock()) {
                writer->flush();
            }
        }
    }

    void write_to_all(const log_entry& entry) {
        // Copy the writers list under lock, then release the lock before calling write()
        std::vector<std::shared_ptr<base_writer>> writers_snapshot;
        {
            std::lock_guard<std::mutex> lock(writers_mutex_);
            writers_snapshot.reserve(writers_.size());
            for (auto& weak_writer : writers_) {
                if (auto writer = weak_writer.lock()) {
                    writers_snapshot.push_back(writer);
                }
            }
        }

        // Write to all writers without holding the mutex
        std::string file = entry.location ? entry.location->file.to_string() : "";
        int line = entry.location ? entry.location->line : 0;
        std::string function = entry.location ? entry.location->function.to_string() : "";

        for (auto& writer : writers_snapshot) {
            writer->write(entry.level, entry.message.to_string(), file,
                         line, function, entry.timestamp);
        }
    }

private:
    std::size_t buffer_size_;
    std::size_t batch_size_;
    std::atomic<bool> running_;
    std::unique_ptr<log_collector_worker> worker_;

    std::queue<log_entry> queue_;
    mutable std::mutex queue_mutex_;

    std::vector<std::weak_ptr<base_writer>> writers_;
    std::mutex writers_mutex_;

    // Track dropped messages when queue is full
    std::atomic<uint64_t> dropped_messages_{0};
};

// log_collector implementation
log_collector::log_collector(std::size_t buffer_size, std::size_t batch_size)
    : pimpl_(std::make_unique<impl>(buffer_size, batch_size)) {
}

log_collector::~log_collector() = default;

bool log_collector::enqueue(logger_system::log_level level,
                           const std::string& message,
                           const std::string& file,
                           int line,
                           const std::string& function,
                           const std::chrono::system_clock::time_point& timestamp) {
    return pimpl_->enqueue(level, message, file, line, function, timestamp);
}

void log_collector::add_writer(std::shared_ptr<base_writer> writer) {
    pimpl_->add_writer(writer);
}

void log_collector::clear_writers() {
    pimpl_->clear_writers();
}

void log_collector::start() {
    pimpl_->start();
}

void log_collector::stop() {
    pimpl_->stop();
}

void log_collector::flush() {
    pimpl_->flush();
}

std::pair<size_t, size_t> log_collector::get_queue_metrics() const {
    return pimpl_->get_queue_metrics();
}

} // namespace kcenon::logger