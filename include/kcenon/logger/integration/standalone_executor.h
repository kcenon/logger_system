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
 * @file standalone_executor.h
 * @brief Standalone IExecutor implementation using std::jthread
 * @author kcenon
 * @since 1.5.0
 *
 * @details This file provides a standalone IExecutor implementation that
 * uses std::jthread (or fallback) for async task execution. It serves as
 * the default executor when thread_system is not available.
 *
 * This implementation wraps the existing async_worker functionality to
 * provide the IExecutor interface, enabling runtime executor injection
 * without compile-time dependencies on thread_system.
 *
 * Key features:
 * - Implements common::interfaces::IExecutor
 * - Uses std::jthread with cooperative cancellation
 * - Thread-safe task submission
 * - Graceful shutdown support
 *
 * @note Part of Issue #253: Migrate to IExecutor interface
 *
 * @see kcenon::common::interfaces::IExecutor
 * @see kcenon::logger::async::async_worker
 */

#pragma once

#include <kcenon/logger/core/thread_integration_detector.h>

#if LOGGER_HAS_IEXECUTOR

#if __has_include(<kcenon/common/interfaces/executor_interface.h>)
#include <kcenon/common/interfaces/executor_interface.h>
#include <kcenon/common/patterns/result.h>
#elif __has_include(<common/interfaces/executor_interface.h>)
#include <common/interfaces/executor_interface.h>
#include <common/patterns/result.h>
#endif

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace kcenon::logger::integration {

/**
 * @brief Simple job wrapper for function execution
 *
 * Wraps a std::function<void()> to implement IJob interface.
 */
class function_job : public common::interfaces::IJob {
public:
    explicit function_job(std::function<void()> func, std::string name = "logger_task")
        : func_(std::move(func)), name_(std::move(name)) {}

    common::VoidResult execute() override {
        try {
            func_();
            return common::ok();
        } catch (const std::exception& ex) {
            return common::VoidResult(common::error_info{
                -1, ex.what(), "standalone_executor"
            });
        } catch (...) {
            return common::VoidResult(common::error_info{
                -1, "Unknown exception during job execution", "standalone_executor"
            });
        }
    }

    std::string get_name() const override { return name_; }

private:
    std::function<void()> func_;
    std::string name_;
};

/**
 * @brief Standalone IExecutor implementation
 *
 * This executor uses a single worker thread with a task queue to process
 * jobs asynchronously. It provides a simple, dependency-free implementation
 * of the IExecutor interface.
 *
 * Thread Safety:
 * - All public methods are thread-safe
 * - Multiple threads can submit jobs concurrently
 * - Single worker thread processes jobs sequentially
 *
 * Usage:
 * @code
 * auto executor = std::make_shared<standalone_executor>();
 * executor->start();
 *
 * auto job = std::make_unique<function_job>([]() {
 *     // do work
 * });
 * auto result = executor->execute(std::move(job));
 *
 * executor->shutdown();
 * @endcode
 *
 * @since 1.5.0
 */
class standalone_executor : public common::interfaces::IExecutor {
public:
    /**
     * @brief Constructor with configurable queue size
     * @param queue_size Maximum number of pending tasks (default: 8192)
     * @param name Executor name for identification (default: "standalone_executor")
     */
    explicit standalone_executor(std::size_t queue_size = 8192,
                                  std::string name = "standalone_executor");

    /**
     * @brief Destructor - ensures graceful shutdown
     */
    ~standalone_executor() override;

    // Non-copyable, non-movable
    standalone_executor(const standalone_executor&) = delete;
    standalone_executor& operator=(const standalone_executor&) = delete;
    standalone_executor(standalone_executor&&) = delete;
    standalone_executor& operator=(standalone_executor&&) = delete;

    /**
     * @brief Start the executor worker thread
     *
     * Must be called before submitting jobs.
     */
    void start();

    // ===== IExecutor Interface Implementation =====

    /**
     * @brief Execute a job asynchronously
     * @param job The job to execute
     * @return Result containing future or error
     */
    common::Result<std::future<void>> execute(
        std::unique_ptr<common::interfaces::IJob>&& job) override;

    /**
     * @brief Execute a job with delay
     * @param job The job to execute
     * @param delay The delay before execution
     * @return Result containing future or error
     */
    common::Result<std::future<void>> execute_delayed(
        std::unique_ptr<common::interfaces::IJob>&& job,
        std::chrono::milliseconds delay) override;

    /**
     * @brief Get the number of worker threads
     * @return Always returns 1 for standalone executor
     */
    size_t worker_count() const override;

    /**
     * @brief Check if the executor is running
     * @return true if running, false otherwise
     */
    bool is_running() const override;

    /**
     * @brief Get the number of pending tasks
     * @return Number of tasks waiting to be executed
     */
    size_t pending_tasks() const override;

    /**
     * @brief Shutdown the executor gracefully
     * @param wait_for_completion Wait for all pending tasks to complete
     */
    void shutdown(bool wait_for_completion = true) override;

    // ===== Additional Methods =====

    /**
     * @brief Get the executor name
     * @return Executor name string
     */
    [[nodiscard]] std::string get_name() const noexcept { return name_; }

    /**
     * @brief Get the number of dropped tasks due to queue overflow
     * @return Total number of tasks that were not enqueued
     */
    [[nodiscard]] std::uint64_t dropped_count() const noexcept;

private:
    /**
     * @brief Internal task structure with promise for completion signaling
     */
    struct pending_task {
        std::unique_ptr<common::interfaces::IJob> job;
        std::shared_ptr<std::promise<void>> completion_promise;
        std::chrono::steady_clock::time_point execute_after;
    };

    /**
     * @brief Worker thread main loop
     */
    void worker_loop();

    /**
     * @brief Process all remaining tasks in the queue
     */
    void drain_queue();

    /**
     * @brief Enqueue a task with optional delay
     * @param task The task to enqueue
     * @return true if enqueued successfully
     */
    bool enqueue_task(pending_task&& task);

private:
    const std::string name_;
    const std::size_t queue_size_;

    std::queue<pending_task> queue_;
    mutable std::mutex queue_mutex_;
    std::condition_variable queue_cv_;

    std::thread worker_thread_;
    std::atomic<bool> running_{false};
    std::atomic<bool> stop_requested_{false};
    std::atomic<std::uint64_t> dropped_count_{0};
};

/**
 * @brief Factory for creating standalone executor instances
 */
class standalone_executor_factory {
public:
    /**
     * @brief Create a new standalone executor
     * @param queue_size Maximum queue size
     * @param name Executor name
     * @return Shared pointer to the executor (already started)
     */
    static std::shared_ptr<common::interfaces::IExecutor> create(
        std::size_t queue_size = 8192,
        const std::string& name = "standalone_executor");
};

} // namespace kcenon::logger::integration

#endif // LOGGER_HAS_IEXECUTOR
