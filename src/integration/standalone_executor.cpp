/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
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
 * @file standalone_executor.cpp
 * @brief Implementation of standalone IExecutor
 * @author kcenon
 * @since 1.5.0
 */

#include <kcenon/logger/integration/standalone_executor.h>

#if LOGGER_HAS_IEXECUTOR

namespace kcenon::logger::integration {

standalone_executor::standalone_executor(std::size_t queue_size, std::string name)
    : name_(std::move(name))
    , queue_size_(queue_size) {}

standalone_executor::~standalone_executor() {
    shutdown(true);
}

void standalone_executor::start() {
    if (running_.exchange(true)) {
        return; // Already running
    }

    stop_requested_.store(false);
    worker_thread_ = std::thread([this]() { worker_loop(); });
}

common::Result<std::future<void>> standalone_executor::execute(
    std::unique_ptr<common::interfaces::IJob>&& job) {

    if (!running_.load()) {
        return common::Result<std::future<void>>(common::error_info{
            -1, "Executor is not running", "standalone_executor"
        });
    }

    auto promise = std::make_shared<std::promise<void>>();
    auto future = promise->get_future();

    pending_task task{
        std::move(job),
        promise,
        std::chrono::steady_clock::time_point{} // No delay
    };

    if (!enqueue_task(std::move(task))) {
        return common::Result<std::future<void>>(common::error_info{
            -2, "Queue is full", "standalone_executor"
        });
    }

    return common::Result<std::future<void>>::ok(std::move(future));
}

common::Result<std::future<void>> standalone_executor::execute_delayed(
    std::unique_ptr<common::interfaces::IJob>&& job,
    std::chrono::milliseconds delay) {

    if (!running_.load()) {
        return common::Result<std::future<void>>(common::error_info{
            -1, "Executor is not running", "standalone_executor"
        });
    }

    auto promise = std::make_shared<std::promise<void>>();
    auto future = promise->get_future();

    pending_task task{
        std::move(job),
        promise,
        std::chrono::steady_clock::now() + delay
    };

    if (!enqueue_task(std::move(task))) {
        return common::Result<std::future<void>>(common::error_info{
            -2, "Queue is full", "standalone_executor"
        });
    }

    return common::Result<std::future<void>>::ok(std::move(future));
}

size_t standalone_executor::worker_count() const {
    return running_.load() ? 1 : 0;
}

bool standalone_executor::is_running() const {
    return running_.load();
}

size_t standalone_executor::pending_tasks() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return queue_.size();
}

void standalone_executor::shutdown(bool wait_for_completion) {
    if (!running_.exchange(false)) {
        return; // Already stopped
    }

    stop_requested_.store(true);
    queue_cv_.notify_all();

    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }

    if (wait_for_completion) {
        drain_queue();
    }
}

std::uint64_t standalone_executor::dropped_count() const noexcept {
    return dropped_count_.load();
}

bool standalone_executor::enqueue_task(pending_task&& task) {
    std::lock_guard<std::mutex> lock(queue_mutex_);

    if (queue_.size() >= queue_size_) {
        dropped_count_.fetch_add(1);
        // Set exception on the promise to signal failure
        task.completion_promise->set_exception(
            std::make_exception_ptr(
                std::runtime_error("Queue is full - task dropped")));
        return false;
    }

    queue_.push(std::move(task));
    queue_cv_.notify_one();
    return true;
}

void standalone_executor::worker_loop() {
    while (!stop_requested_.load()) {
        pending_task task;

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            queue_cv_.wait(lock, [this]() {
                return stop_requested_.load() || !queue_.empty();
            });

            if (stop_requested_.load() && queue_.empty()) {
                break;
            }

            if (queue_.empty()) {
                continue;
            }

            task = std::move(queue_.front());
            queue_.pop();
        }

        // Handle delayed execution
        auto now = std::chrono::steady_clock::now();
        if (task.execute_after > now) {
            std::this_thread::sleep_until(task.execute_after);
        }

        // Execute the job
        try {
            auto result = task.job->execute();
            if (result.is_ok()) {
                task.completion_promise->set_value();
            } else {
                task.completion_promise->set_exception(
                    std::make_exception_ptr(
                        std::runtime_error(result.error().message)));
            }
        } catch (...) {
            task.completion_promise->set_exception(std::current_exception());
        }
    }
}

void standalone_executor::drain_queue() {
    std::queue<pending_task> remaining;

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        std::swap(remaining, queue_);
    }

    while (!remaining.empty()) {
        auto task = std::move(remaining.front());
        remaining.pop();

        try {
            auto result = task.job->execute();
            if (result.is_ok()) {
                task.completion_promise->set_value();
            } else {
                task.completion_promise->set_exception(
                    std::make_exception_ptr(
                        std::runtime_error(result.error().message)));
            }
        } catch (...) {
            task.completion_promise->set_exception(std::current_exception());
        }
    }
}

// Factory implementation
std::shared_ptr<common::interfaces::IExecutor> standalone_executor_factory::create(
    std::size_t queue_size,
    const std::string& name) {

    auto executor = std::make_shared<standalone_executor>(queue_size, name);
    executor->start();
    return executor;
}

} // namespace kcenon::logger::integration

#endif // LOGGER_HAS_IEXECUTOR
