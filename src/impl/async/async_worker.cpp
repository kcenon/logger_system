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

#include "async_worker.h"

#include <cstdio>
#include <utility>

namespace kcenon::logger::async {

async_worker::async_worker(std::size_t queue_size)
    : queue_size_(queue_size)
#if !LOGGER_HAS_JTHREAD
    , stop_source_(std::make_shared<simple_stop_source>())
#endif
{
}

async_worker::~async_worker() {
    stop();
}

void async_worker::start() {
    // Check if already running (idempotent)
    if (running_.exchange(true, std::memory_order_acq_rel)) {
        return;
    }

#if LOGGER_HAS_JTHREAD
    // Create worker thread with std::jthread
    // The stop_token is automatically provided by jthread
    worker_thread_ = compat_jthread([this](std::stop_token stop_token) {
        worker_loop(stop_token);
    });
#else
    // Reset stop source for new start
    stop_source_->reset();

    // Create worker thread with manual stop source
    auto stop = stop_source_;
    worker_thread_ = compat_jthread([this, stop](simple_stop_source& /*unused*/) {
        worker_loop(*stop);
    });
#endif
}

void async_worker::stop() {
    // Check if not running (idempotent)
    if (!running_.exchange(false, std::memory_order_acq_rel)) {
        return;
    }

    // Request stop
    worker_thread_.request_stop();

    // Wake up the worker thread
    queue_cv_.notify_all();

    // Wait for thread to finish
    worker_thread_.join();

    // Process any remaining tasks
    drain_queue();
}

bool async_worker::enqueue(task_type task) {
    if (!task || !running_.load(std::memory_order_acquire)) {
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);

        // Check queue capacity
        if (queue_.size() >= queue_size_) {
            // Track dropped task
            std::uint64_t dropped = dropped_count_.fetch_add(1, std::memory_order_relaxed) + 1;

            // Log warning periodically (every 100 drops) to avoid log spam
            if (dropped % 100 == 1) {
                std::fprintf(stderr,
                    "[WARNING] Async worker queue full: %llu tasks dropped total\n",
                    static_cast<unsigned long long>(dropped));
            }
            return false;
        }

        queue_.push(std::move(task));
    }

    // Notify worker thread
    queue_cv_.notify_one();
    return true;
}

void async_worker::flush() {
    if (!running_.load(std::memory_order_acquire)) {
        // Not running - just drain the queue directly
        drain_queue();
        return;
    }

    // Wait until queue is empty
    // Use a polling approach with short waits to avoid deadlock
    while (true) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (queue_.empty()) {
                break;
            }
        }
        // Brief yield to allow worker to process
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

bool async_worker::is_running() const noexcept {
    return running_.load(std::memory_order_acquire);
}

std::size_t async_worker::pending_count() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return queue_.size();
}

std::size_t async_worker::capacity() const noexcept {
    return queue_size_;
}

std::uint64_t async_worker::dropped_count() const noexcept {
    return dropped_count_.load(std::memory_order_relaxed);
}

#if LOGGER_HAS_JTHREAD
void async_worker::worker_loop(std::stop_token stop_token) {
    while (!stop_token.stop_requested()) {
        task_type task;

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // Wait for task or stop signal
            // condition_variable_any works with stop_token
            bool has_work = queue_cv_.wait(lock, stop_token, [this]() {
                return !queue_.empty();
            });

            // Check if we should stop
            if (stop_token.stop_requested()) {
                break;
            }

            // Check if we have work (not spurious wakeup)
            if (!has_work || queue_.empty()) {
                continue;
            }

            // Get task from queue
            task = std::move(queue_.front());
            queue_.pop();
        }

        // Execute task outside the lock
        if (task) {
            try {
                task();
            } catch (...) {
                // Swallow exceptions to prevent thread termination
            }
        }
    }
}
#else
void async_worker::worker_loop(simple_stop_source& stop) {
    while (!stop.stop_requested()) {
        task_type task;

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // Wait for task or stop signal
            queue_cv_.wait(lock, [this, &stop]() {
                return stop.stop_requested() || !queue_.empty();
            });

            // Check if we should stop
            if (stop.stop_requested()) {
                break;
            }

            // Check if we have work
            if (queue_.empty()) {
                continue;
            }

            // Get task from queue
            task = std::move(queue_.front());
            queue_.pop();
        }

        // Execute task outside the lock
        if (task) {
            try {
                task();
            } catch (...) {
                // Swallow exceptions to prevent thread termination
            }
        }
    }
}
#endif

void async_worker::drain_queue() {
    std::queue<task_type> remaining;

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        std::swap(remaining, queue_);
    }

    // Process remaining tasks
    while (!remaining.empty()) {
        auto task = std::move(remaining.front());
        remaining.pop();

        if (task) {
            try {
                task();
            } catch (...) {
                // Swallow exceptions during drain
            }
        }
    }
}

} // namespace kcenon::logger::async
