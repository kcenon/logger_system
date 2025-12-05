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

/**
 * @file log_collector.cpp
 * @brief Asynchronous log collector implementation with jthread compatibility
 *
 * This implementation uses C++20 std::jthread with std::stop_token for
 * cooperative cancellation where available, with fallback to std::thread
 * for environments without jthread support (e.g., libc++).
 *
 * @since 1.3.0 - Refactored to use jthread compatibility layer
 */

#include <kcenon/logger/core/log_collector.h>
#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/logger_interface.h>

#include "../impl/async/jthread_compat.h"

#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace kcenon::logger {

/**
 * @brief Shared state for log processing - survives impl destruction
 *
 * This structure holds all the data that the worker needs to access.
 * By using shared_ptr, the worker can safely access this data even after
 * the impl object starts destruction, preventing use-after-free bugs.
 */
struct log_collector_shared_state {
    std::queue<log_entry> queue;
    mutable std::mutex queue_mutex;
#if LOGGER_HAS_JTHREAD
    std::condition_variable_any queue_cv;  // Works with stop_token
#else
    std::condition_variable queue_cv;       // Standard condition variable
#endif
    std::vector<std::weak_ptr<base_writer>> writers;
    std::mutex writers_mutex;
    const std::size_t batch_size;
    const std::size_t buffer_size;

    explicit log_collector_shared_state(std::size_t buffer_sz, std::size_t batch_sz)
        : batch_size(batch_sz)
        , buffer_size(buffer_sz) {}
};

/**
 * @brief Worker thread for log processing with jthread compatibility
 *
 * Uses std::jthread where available, falls back to std::thread with
 * manual stop mechanism otherwise.
 */
class log_collector_jthread_worker {
public:
    explicit log_collector_jthread_worker(std::shared_ptr<log_collector_shared_state> state)
        : state_(std::move(state))
        , running_(false)
#if !LOGGER_HAS_JTHREAD
        , stop_source_(std::make_shared<async::simple_stop_source>())
#endif
    {}

    ~log_collector_jthread_worker() {
        stop();
    }

    void start() {
        if (running_.exchange(true, std::memory_order_acq_rel)) {
            return;  // Already started
        }

#if LOGGER_HAS_JTHREAD
        // Capture state by shared_ptr - survives worker destruction
        auto state = state_;
        thread_ = async::compat_jthread([state](std::stop_token stop_token) {
            worker_loop(state, stop_token);
        });
#else
        // Reset stop source for new start
        stop_source_->reset();

        // Capture state and stop source by shared_ptr
        auto state = state_;
        auto stop = stop_source_;
        thread_ = async::compat_jthread([state, stop](async::simple_stop_source& /*unused*/) {
            worker_loop(state, *stop);
        });
#endif
    }

    void stop() {
        if (!running_.exchange(false, std::memory_order_acq_rel)) {
            return;  // Already stopped
        }

        // Request stop
        thread_.request_stop();

        // Wake up the worker in case it's waiting
        if (state_) {
            state_->queue_cv.notify_all();
        }

        // Wait for thread to finish
        thread_.join();
    }

    void notify_work() {
        if (state_) {
            state_->queue_cv.notify_one();
        }
    }

    [[nodiscard]] bool is_running() const noexcept {
        return running_.load(std::memory_order_acquire);
    }

private:
#if LOGGER_HAS_JTHREAD
    static void worker_loop(std::shared_ptr<log_collector_shared_state> state,
                           std::stop_token stop_token) {
        if (!state) {
            return;
        }

        while (!stop_token.stop_requested()) {
            std::vector<log_entry> batch;

            {
                std::unique_lock<std::mutex> lock(state->queue_mutex);

                // Wait for work or stop signal using condition_variable_any
                bool has_work = state->queue_cv.wait(lock, stop_token, [&state]() {
                    return !state->queue.empty();
                });

                // Check if stop was requested
                if (stop_token.stop_requested()) {
                    break;
                }

                // Check if we actually have work
                if (!has_work || state->queue.empty()) {
                    continue;
                }

                // Extract batch from queue
                batch.reserve(std::min(state->batch_size, state->queue.size()));
                while (!state->queue.empty() && batch.size() < state->batch_size) {
                    batch.push_back(std::move(state->queue.front()));
                    state->queue.pop();
                }
            }

            // Process batch outside the lock
            for (const auto& entry : batch) {
                write_to_all(state, entry);
            }
        }
    }
#else
    static void worker_loop(std::shared_ptr<log_collector_shared_state> state,
                           async::simple_stop_source& stop) {
        if (!state) {
            return;
        }

        while (!stop.stop_requested()) {
            std::vector<log_entry> batch;

            {
                std::unique_lock<std::mutex> lock(state->queue_mutex);

                // Wait for work or stop signal
                state->queue_cv.wait(lock, [&state, &stop]() {
                    return stop.stop_requested() || !state->queue.empty();
                });

                // Check if stop was requested
                if (stop.stop_requested()) {
                    break;
                }

                // Check if we actually have work
                if (state->queue.empty()) {
                    continue;
                }

                // Extract batch from queue
                batch.reserve(std::min(state->batch_size, state->queue.size()));
                while (!state->queue.empty() && batch.size() < state->batch_size) {
                    batch.push_back(std::move(state->queue.front()));
                    state->queue.pop();
                }
            }

            // Process batch outside the lock
            for (const auto& entry : batch) {
                write_to_all(state, entry);
            }
        }
    }
#endif

    static void write_to_all(const std::shared_ptr<log_collector_shared_state>& state,
                            const log_entry& entry) {
        if (!state) {
            return;
        }

        // Snapshot writers under lock
        std::vector<std::shared_ptr<base_writer>> writers_snapshot;
        {
            std::lock_guard<std::mutex> lock(state->writers_mutex);
            writers_snapshot.reserve(state->writers.size());
            for (auto& weak_writer : state->writers) {
                if (auto writer = weak_writer.lock()) {
                    writers_snapshot.push_back(writer);
                }
            }
        }

        // Extract entry fields
        std::string file = entry.location ? entry.location->file.to_string() : "";
        int line = entry.location ? entry.location->line : 0;
        std::string function = entry.location ? entry.location->function.to_string() : "";

        // Write to all writers without holding mutex
        for (auto& writer : writers_snapshot) {
            try {
                writer->write(entry.level, entry.message.to_string(), file,
                             line, function, entry.timestamp);
            } catch (...) {
                // Swallow exceptions to prevent thread termination
            }
        }
    }

private:
    std::shared_ptr<log_collector_shared_state> state_;
    async::compat_jthread thread_;
    std::atomic<bool> running_;
#if !LOGGER_HAS_JTHREAD
    std::shared_ptr<async::simple_stop_source> stop_source_;
#endif
};

class log_collector::impl {
public:
    explicit impl(std::size_t buffer_size, std::size_t batch_size)
        : state_(std::make_shared<log_collector_shared_state>(buffer_size, batch_size))
        , worker_(std::make_unique<log_collector_jthread_worker>(state_)) {
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
            std::lock_guard<std::mutex> lock(state_->queue_mutex);

            // Check if queue is full
            if (state_->queue.size() >= state_->buffer_size) {
                // Track dropped message
                std::uint64_t dropped = dropped_messages_.fetch_add(1, std::memory_order_relaxed) + 1;

                // Log warning periodically (every 100 dropped messages) to avoid spam
                if (dropped % 100 == 1) {
                    std::fprintf(stderr, "[WARNING] Log queue full: %llu messages dropped total\n",
                               static_cast<unsigned long long>(dropped));
                }

                return false;
            }

            // Create log_entry with optional source location
            log_entry entry(level, message, timestamp);
            if (!file.empty() || line != 0 || !function.empty()) {
                entry.location = source_location{file, line, function};
            }
            state_->queue.push(std::move(entry));
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
        std::lock_guard<std::mutex> lock(state_->writers_mutex);
        state_->writers.push_back(writer);
    }

    void clear_writers() {
        std::lock_guard<std::mutex> lock(state_->writers_mutex);
        state_->writers.clear();
    }

    void start() {
        if (worker_) {
            worker_->start();
        }
    }

    void stop() {
        // Stop the worker thread first
        if (worker_) {
            worker_->stop();
        }

        // Drain remaining entries
        drain_queue();

        // Flush all writers
        flush_writers();
    }

    void flush() {
        // Wait for queue to be empty
        while (true) {
            {
                std::lock_guard<std::mutex> lock(state_->queue_mutex);
                if (state_->queue.empty()) {
                    break;
                }
            }
            // Brief yield to allow worker to process
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        // Flush all writers
        flush_writers();
    }

    [[nodiscard]] std::pair<std::size_t, std::size_t> get_queue_metrics() const {
        std::lock_guard<std::mutex> lock(state_->queue_mutex);
        return {state_->queue.size(), state_->buffer_size};
    }

private:
    void drain_queue() {
        std::queue<log_entry> remaining;
        {
            std::lock_guard<std::mutex> lock(state_->queue_mutex);
            std::swap(remaining, state_->queue);
        }

        // Process remaining entries
        while (!remaining.empty()) {
            auto entry = std::move(remaining.front());
            remaining.pop();
            write_to_all(entry);
        }
    }

    void flush_writers() {
        std::vector<std::shared_ptr<base_writer>> writers_snapshot;
        {
            std::lock_guard<std::mutex> lock(state_->writers_mutex);
            writers_snapshot.reserve(state_->writers.size());
            for (auto& weak_writer : state_->writers) {
                if (auto writer = weak_writer.lock()) {
                    writers_snapshot.push_back(writer);
                }
            }
        }

        for (auto& writer : writers_snapshot) {
            try {
                writer->flush();
            } catch (...) {
                // Swallow exceptions during flush
            }
        }
    }

    void write_to_all(const log_entry& entry) {
        // Snapshot writers under lock
        std::vector<std::shared_ptr<base_writer>> writers_snapshot;
        {
            std::lock_guard<std::mutex> lock(state_->writers_mutex);
            writers_snapshot.reserve(state_->writers.size());
            for (auto& weak_writer : state_->writers) {
                if (auto writer = weak_writer.lock()) {
                    writers_snapshot.push_back(writer);
                }
            }
        }

        // Extract entry fields
        std::string file = entry.location ? entry.location->file.to_string() : "";
        int line = entry.location ? entry.location->line : 0;
        std::string function = entry.location ? entry.location->function.to_string() : "";

        // Write to all writers without holding mutex
        for (auto& writer : writers_snapshot) {
            try {
                writer->write(entry.level, entry.message.to_string(), file,
                             line, function, entry.timestamp);
            } catch (...) {
                // Swallow exceptions to prevent issues
            }
        }
    }

private:
    std::shared_ptr<log_collector_shared_state> state_;
    std::unique_ptr<log_collector_jthread_worker> worker_;
    std::atomic<std::uint64_t> dropped_messages_{0};
};

// log_collector public interface implementation

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

std::pair<std::size_t, std::size_t> log_collector::get_queue_metrics() const {
    return pimpl_->get_queue_metrics();
}

} // namespace kcenon::logger
