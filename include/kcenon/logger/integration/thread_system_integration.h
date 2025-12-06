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
 * @file thread_system_integration.h
 * @brief Optional thread_system integration for advanced async processing
 * @author kcenon
 * @since 1.4.0
 *
 * @details This file provides optional integration with the thread_system
 * library for users who want to leverage its advanced thread pool features
 * instead of the standalone std::jthread implementation.
 *
 * The integration is compile-time optional and controlled by:
 * - CMake option: LOGGER_USE_THREAD_SYSTEM
 * - Compile definition: LOGGER_HAS_THREAD_SYSTEM
 *
 * When thread_system is not available, all integration classes and functions
 * are excluded via preprocessor conditionals, ensuring zero overhead for
 * standalone builds.
 *
 * Key features when enabled:
 * - Use thread_system's thread_pool for async log processing
 * - Benefit from advanced thread pool features (priority queuing, metrics)
 * - Runtime switching between standalone and thread_pool backends
 * - Automatic fallback to standalone mode if thread_pool is not configured
 *
 * @note Part of Issue #224: Add optional thread_system integration support
 *
 * @see async_worker For the standalone implementation
 * @see kcenon::thread::thread_pool For the thread_system thread pool
 */

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <string>

// Conditional include for thread_system
// LOGGER_HAS_THREAD_SYSTEM is defined by CMake when thread_system is available
#ifdef LOGGER_HAS_THREAD_SYSTEM
#include <kcenon/thread/core/thread_pool.h>
#endif

namespace kcenon::logger::integration {

/**
 * @brief Backend type enumeration for async processing
 *
 * Defines the available backend implementations for async log processing.
 * The backend can be selected at compile time (via CMake options) or
 * runtime (via thread_system_integration::set_backend()).
 */
enum class async_backend_type {
    /**
     * @brief Standalone backend using std::jthread
     *
     * Default backend that requires no external dependencies.
     * Uses the async_worker class with std::jthread for cooperative
     * cancellation support.
     */
    standalone,

    /**
     * @brief Thread pool backend using thread_system
     *
     * Advanced backend that uses thread_system's thread_pool for
     * async processing. Only available when LOGGER_HAS_THREAD_SYSTEM
     * is defined.
     *
     * Benefits:
     * - Shared thread pool for multiple loggers
     * - Priority-based job scheduling
     * - Built-in metrics and monitoring
     * - Work stealing for load balancing
     */
    thread_pool
};

#ifdef LOGGER_HAS_THREAD_SYSTEM

/**
 * @brief Integration module for thread_system
 *
 * Provides advanced async processing using thread_system's thread pool
 * instead of the standalone std::jthread implementation.
 *
 * This class manages a shared thread_pool instance that can be used by
 * multiple logger instances. The pool is lazily initialized on first use
 * or can be explicitly configured via set_thread_pool().
 *
 * Thread Safety:
 * - All methods are thread-safe
 * - Backend switching is atomic
 * - Thread pool operations are internally synchronized
 *
 * Usage:
 * @code
 * // Option 1: Use default thread pool (auto-created)
 * thread_system_integration::enable();
 *
 * // Option 2: Provide custom thread pool
 * auto pool = std::make_shared<kcenon::thread::thread_pool>("logger_pool");
 * pool->start();
 * thread_system_integration::set_thread_pool(pool);
 *
 * // Check status
 * if (thread_system_integration::is_enabled()) {
 *     // Using thread_pool backend
 * }
 *
 * // Disable and fall back to standalone
 * thread_system_integration::disable();
 * @endcode
 *
 * @note Only available when LOGGER_HAS_THREAD_SYSTEM is defined
 * @since 1.4.0
 */
class thread_system_integration {
public:
    /**
     * @brief Enable thread_pool backend with optional custom pool
     * @param pool Custom thread pool to use (optional)
     *
     * If no pool is provided, a default pool will be created with
     * optimal thread count based on hardware concurrency.
     *
     * Thread-safe: Can be called from any thread.
     *
     * @code
     * // Enable with default pool
     * thread_system_integration::enable();
     *
     * // Enable with custom pool
     * auto pool = std::make_shared<kcenon::thread::thread_pool>("custom", 4);
     * pool->start();
     * thread_system_integration::enable(pool);
     * @endcode
     */
    static void enable(std::shared_ptr<kcenon::thread::thread_pool> pool = nullptr);

    /**
     * @brief Disable thread_pool backend
     *
     * Switches back to standalone std::jthread backend.
     * The thread pool reference is released but not stopped - the pool
     * will be stopped when all references are released.
     *
     * Thread-safe: Can be called from any thread.
     */
    static void disable();

    /**
     * @brief Check if thread_pool backend is enabled
     * @return true if using thread_pool backend, false for standalone
     *
     * Thread-safe: Can be called from any thread.
     */
    [[nodiscard]] static bool is_enabled() noexcept;

    /**
     * @brief Get the current async backend type
     * @return Current backend type (standalone or thread_pool)
     *
     * Thread-safe: Can be called from any thread.
     */
    [[nodiscard]] static async_backend_type get_backend() noexcept;

    /**
     * @brief Set the thread pool to use for async processing
     * @param pool Shared pointer to the thread pool
     *
     * This method allows setting a custom thread pool. The pool should
     * be started before calling this method. If the pool is null,
     * the backend is switched to standalone mode.
     *
     * Thread-safe: Can be called from any thread.
     *
     * @note This implicitly enables the thread_pool backend if pool is not null
     */
    static void set_thread_pool(std::shared_ptr<kcenon::thread::thread_pool> pool);

    /**
     * @brief Get the current thread pool
     * @return Shared pointer to the current thread pool, or nullptr if disabled
     *
     * Thread-safe: Can be called from any thread.
     */
    [[nodiscard]] static std::shared_ptr<kcenon::thread::thread_pool> get_thread_pool() noexcept;

    /**
     * @brief Submit a task to the configured backend
     * @param task The task to execute asynchronously
     * @return true if task was submitted successfully, false otherwise
     *
     * This method submits a task to either the thread_pool backend
     * (if enabled) or falls back to indicating that standalone mode
     * should be used.
     *
     * Thread-safe: Can be called from any thread.
     *
     * @code
     * auto submitted = thread_system_integration::submit_task([&logger]() {
     *     logger.write("async message");
     * });
     * if (!submitted) {
     *     // Task was not submitted - handle synchronously or use standalone
     * }
     * @endcode
     */
    [[nodiscard]] static bool submit_task(std::function<void()> task);

    /**
     * @brief Get the name of the current backend
     * @return String representation of the current backend type
     *
     * Returns "thread_pool" when thread_pool backend is enabled,
     * "standalone" otherwise.
     */
    [[nodiscard]] static std::string get_backend_name() noexcept;

private:
    // Prevent instantiation - all methods are static
    thread_system_integration() = delete;
    ~thread_system_integration() = delete;
    thread_system_integration(const thread_system_integration&) = delete;
    thread_system_integration& operator=(const thread_system_integration&) = delete;

    // Internal state
    static std::atomic<async_backend_type> current_backend_;
    static std::shared_ptr<kcenon::thread::thread_pool> thread_pool_;
    static std::mutex pool_mutex_;

    /**
     * @brief Create a default thread pool with optimal settings
     * @return Shared pointer to the newly created and started pool
     */
    static std::shared_ptr<kcenon::thread::thread_pool> create_default_pool();
};

#else // !LOGGER_HAS_THREAD_SYSTEM

/**
 * @brief Stub implementation when thread_system is not available
 *
 * This stub provides the same interface as the full implementation
 * but all thread_pool-related operations return false or no-op.
 * This allows code to be written once and compile regardless of
 * whether thread_system is available.
 *
 * @note Available even when LOGGER_HAS_THREAD_SYSTEM is not defined
 * @since 1.4.0
 */
class thread_system_integration {
public:
    /**
     * @brief No-op when thread_system is not available
     */
    static void enable() noexcept {
        // No-op: thread_system not available
    }

    /**
     * @brief No-op when thread_system is not available
     */
    static void disable() noexcept {
        // No-op: thread_system not available
    }

    /**
     * @brief Always returns false when thread_system is not available
     * @return false (thread_pool backend not available)
     */
    [[nodiscard]] static constexpr bool is_enabled() noexcept {
        return false;
    }

    /**
     * @brief Always returns standalone when thread_system is not available
     * @return async_backend_type::standalone
     */
    [[nodiscard]] static constexpr async_backend_type get_backend() noexcept {
        return async_backend_type::standalone;
    }

    /**
     * @brief Always returns false when thread_system is not available
     * @param task The task (ignored)
     * @return false (task not submitted to thread_pool)
     */
    [[nodiscard]] static bool submit_task(std::function<void()> /*task*/) noexcept {
        return false;
    }

    /**
     * @brief Always returns "standalone" when thread_system is not available
     * @return "standalone"
     */
    [[nodiscard]] static std::string get_backend_name() noexcept {
        return "standalone";
    }

private:
    thread_system_integration() = delete;
    ~thread_system_integration() = delete;
    thread_system_integration(const thread_system_integration&) = delete;
    thread_system_integration& operator=(const thread_system_integration&) = delete;
};

#endif // LOGGER_HAS_THREAD_SYSTEM

/**
 * @brief Helper function to check if thread_system integration is available
 * @return true if LOGGER_HAS_THREAD_SYSTEM is defined, false otherwise
 *
 * This compile-time constant can be used to conditionally execute code
 * that requires thread_system integration.
 *
 * @code
 * if constexpr (has_thread_system_support()) {
 *     // Use thread_system features
 * } else {
 *     // Use standalone features
 * }
 * @endcode
 */
[[nodiscard]] constexpr bool has_thread_system_support() noexcept {
#ifdef LOGGER_HAS_THREAD_SYSTEM
    return true;
#else
    return false;
#endif
}

} // namespace kcenon::logger::integration
