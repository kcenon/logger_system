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
 * @file executor_integration.h
 * @brief IExecutor-based async integration for logger_system
 * @author kcenon
 * @since 1.5.0
 *
 * @details This file provides the IExecutor-based integration layer for
 * logger_system's async processing. It replaces the direct thread_system
 * dependency with a neutral abstraction layer from common_system.
 *
 * Benefits of IExecutor integration:
 * - No compile-time dependency on thread_system
 * - Runtime executor injection for flexibility
 * - Testability with mock executors
 * - Future-proof architecture
 *
 * This is the recommended approach for async integration as of v1.5.0.
 *
 * @note Part of Issue #253: Migrate to IExecutor interface
 *
 * @see kcenon::common::interfaces::IExecutor
 * @see standalone_executor For the default executor implementation
 * @see thread_system_integration For legacy direct integration
 */

#pragma once

#include <kcenon/logger/core/thread_integration_detector.h>
#include <kcenon/logger/integration/standalone_executor.h>

#if LOGGER_HAS_IEXECUTOR

#if __has_include(<kcenon/common/interfaces/executor_interface.h>)
#include <kcenon/common/interfaces/executor_interface.h>
#elif __has_include(<common/interfaces/executor_interface.h>)
#include <common/interfaces/executor_interface.h>
#endif

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>

namespace kcenon::logger::integration {

/**
 * @brief Executor backend type enumeration
 *
 * Defines the available executor implementations for async log processing.
 */
enum class executor_type {
    /**
     * @brief No executor configured (synchronous mode)
     */
    none,

    /**
     * @brief Standalone executor using std::jthread
     *
     * Built-in executor that requires no external dependencies.
     */
    standalone,

    /**
     * @brief External executor (e.g., from thread_system)
     *
     * User-provided IExecutor implementation for advanced use cases.
     */
    external
};

/**
 * @brief IExecutor-based integration manager for logger_system
 *
 * This class manages async processing using the IExecutor interface from
 * common_system. It provides runtime executor injection and automatic
 * fallback to standalone mode.
 *
 * Thread Safety:
 * - All methods are thread-safe
 * - Executor switching is protected by mutex
 * - Task submission is lock-free once executor is configured
 *
 * Usage:
 * @code
 * // Option 1: Use default standalone executor
 * executor_integration::enable();
 *
 * // Option 2: Provide external executor (e.g., from thread_system)
 * auto thread_pool_executor = thread_system::create_executor();
 * executor_integration::set_executor(thread_pool_executor);
 *
 * // Submit tasks
 * executor_integration::submit_task([]() {
 *     // async work
 * });
 *
 * // Check status
 * if (executor_integration::is_enabled()) {
 *     // Using async mode
 * }
 *
 * // Disable and cleanup
 * executor_integration::disable();
 * @endcode
 *
 * @since 1.5.0
 */
class executor_integration {
public:
    /**
     * @brief Enable async processing with optional executor
     * @param executor Custom executor to use (optional)
     *
     * If no executor is provided, a default standalone executor will be
     * created automatically.
     *
     * Thread-safe: Can be called from any thread.
     *
     * @code
     * // Enable with default standalone executor
     * executor_integration::enable();
     *
     * // Enable with custom executor
     * auto executor = create_my_executor();
     * executor_integration::enable(executor);
     * @endcode
     */
    static void enable(std::shared_ptr<common::interfaces::IExecutor> executor = nullptr);

    /**
     * @brief Disable async processing
     *
     * Switches to synchronous mode. The executor reference is released
     * but not explicitly shut down - the executor will be cleaned up
     * when all references are released.
     *
     * Thread-safe: Can be called from any thread.
     */
    static void disable();

    /**
     * @brief Check if async processing is enabled
     * @return true if an executor is configured and running
     *
     * Thread-safe: Can be called from any thread.
     */
    [[nodiscard]] static bool is_enabled() noexcept;

    /**
     * @brief Get the current executor type
     * @return Current executor type
     *
     * Thread-safe: Can be called from any thread.
     */
    [[nodiscard]] static executor_type get_executor_type() noexcept;

    /**
     * @brief Set the executor for async processing
     * @param executor Shared pointer to the executor
     *
     * This method allows setting a custom IExecutor implementation.
     * The executor should be started before calling this method.
     * If the executor is null, async processing is disabled.
     *
     * Thread-safe: Can be called from any thread.
     *
     * @note This implicitly enables async processing if executor is not null
     */
    static void set_executor(std::shared_ptr<common::interfaces::IExecutor> executor);

    /**
     * @brief Get the current executor
     * @return Shared pointer to the current executor, or nullptr if disabled
     *
     * Thread-safe: Can be called from any thread.
     */
    [[nodiscard]] static std::shared_ptr<common::interfaces::IExecutor> get_executor() noexcept;

    /**
     * @brief Submit a task for async execution
     * @param task The task to execute asynchronously
     * @return true if task was submitted successfully, false otherwise
     *
     * If async processing is disabled or the executor is not available,
     * returns false. The caller should handle this case (e.g., execute
     * synchronously).
     *
     * Thread-safe: Can be called from any thread.
     *
     * @code
     * auto submitted = executor_integration::submit_task([&logger]() {
     *     logger.write("async message");
     * });
     * if (!submitted) {
     *     // Fall back to synchronous execution
     *     logger.write("sync message");
     * }
     * @endcode
     */
    [[nodiscard]] static bool submit_task(std::function<void()> task);

    /**
     * @brief Submit a task with delay
     * @param task The task to execute
     * @param delay_ms Delay in milliseconds before execution
     * @return true if task was submitted successfully
     */
    [[nodiscard]] static bool submit_task_delayed(
        std::function<void()> task,
        std::chrono::milliseconds delay_ms);

    /**
     * @brief Get the name of the current executor
     * @return String representation of the current executor type
     */
    [[nodiscard]] static std::string get_executor_name() noexcept;

    /**
     * @brief Get the number of pending tasks
     * @return Number of tasks waiting to be executed
     */
    [[nodiscard]] static size_t pending_tasks() noexcept;

    /**
     * @brief Get the number of worker threads
     * @return Number of worker threads in the executor
     */
    [[nodiscard]] static size_t worker_count() noexcept;

private:
    // Prevent instantiation - all methods are static
    executor_integration() = delete;
    ~executor_integration() = delete;
    executor_integration(const executor_integration&) = delete;
    executor_integration& operator=(const executor_integration&) = delete;

    // Internal state
    static std::atomic<executor_type> current_type_;
    static std::shared_ptr<common::interfaces::IExecutor> executor_;
    static std::mutex executor_mutex_;

    /**
     * @brief Create a default standalone executor
     * @return Shared pointer to the newly created executor
     */
    static std::shared_ptr<common::interfaces::IExecutor> create_default_executor();
};

/**
 * @brief Helper function to check if IExecutor integration is available
 * @return true if LOGGER_HAS_IEXECUTOR is defined, false otherwise
 *
 * This compile-time constant can be used to conditionally execute code
 * that requires IExecutor integration.
 *
 * @code
 * if constexpr (has_executor_support()) {
 *     executor_integration::enable();
 * } else {
 *     // Use synchronous logging
 * }
 * @endcode
 */
[[nodiscard]] constexpr bool has_executor_support() noexcept {
    return true;
}

} // namespace kcenon::logger::integration

#else // !LOGGER_HAS_IEXECUTOR

namespace kcenon::logger::integration {

/**
 * @brief Stub implementation when IExecutor is not available
 *
 * Provides no-op implementations for all methods.
 */
class executor_integration {
public:
    static void enable() noexcept {}
    static void disable() noexcept {}
    [[nodiscard]] static constexpr bool is_enabled() noexcept { return false; }
    [[nodiscard]] static bool submit_task(std::function<void()>) noexcept { return false; }
    [[nodiscard]] static std::string get_executor_name() noexcept { return "none"; }

private:
    executor_integration() = delete;
};

[[nodiscard]] constexpr bool has_executor_support() noexcept {
    return false;
}

} // namespace kcenon::logger::integration

#endif // LOGGER_HAS_IEXECUTOR
