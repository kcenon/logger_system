#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file jthread_compat.h
 * @brief Compatibility header for std::jthread and std::stop_token
 * @author 🍀☀🌕🌥 🌊
 * @since 1.3.0
 *
 * @details This header provides compatibility macros and fallback implementations
 * for environments where std::jthread is not available (e.g., libc++ on macOS/Clang).
 *
 * Detection Strategy:
 * - Check for __cpp_lib_jthread feature test macro (P0660R10)
 * - Check for libc++ which doesn't fully support jthread yet
 * - Provide manual stop mechanism fallback when jthread is unavailable
 *
 * @note This is an internal header, not part of the public API
 */

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <version>

namespace kcenon::logger::async {

/**
 * @brief Check if std::jthread is available
 *
 * std::jthread requires:
 * - C++20 support
 * - Implementation of P0660R10 (stop_token and jthread)
 *
 * Known limitations:
 * - libc++ (Apple Clang, LLVM Clang with -stdlib=libc++) doesn't support jthread
 * - Only libstdc++ (GCC) fully supports jthread
 * - MSVC STL supports jthread from VS 2019 16.8+
 */
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    // Full jthread support available
    #define LOGGER_HAS_JTHREAD 1
#elif defined(_LIBCPP_VERSION)
    // libc++ doesn't support jthread yet
    #define LOGGER_HAS_JTHREAD 0
#elif defined(__GLIBCXX__) && defined(__cpp_lib_jthread)
    // libstdc++ with jthread support
    #define LOGGER_HAS_JTHREAD 1
#elif defined(_MSC_VER) && _MSC_VER >= 1928
    // MSVC 16.8+ has jthread support
    #define LOGGER_HAS_JTHREAD 1
#else
    // Unknown or unsupported - fall back to manual implementation
    #define LOGGER_HAS_JTHREAD 0
#endif

#if LOGGER_HAS_JTHREAD
    #include <stop_token>
#endif

/**
 * @brief Simple stop source for environments without std::stop_token
 *
 * Provides a minimal implementation of stop signaling mechanism
 * using atomic flags and condition variables.
 */
class simple_stop_source {
public:
    simple_stop_source() : stopped_(false) {}

    /**
     * @brief Request stop
     */
    void request_stop() {
        stopped_.store(true, std::memory_order_release);
    }

    /**
     * @brief Check if stop was requested
     */
    [[nodiscard]] bool stop_requested() const noexcept {
        return stopped_.load(std::memory_order_acquire);
    }

    /**
     * @brief Reset stop state (for reuse)
     */
    void reset() {
        stopped_.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> stopped_;
};

/**
 * @brief Wrapper for std::jthread or std::thread with manual stop mechanism
 *
 * When LOGGER_HAS_JTHREAD is true, this is a thin wrapper around std::jthread.
 * When LOGGER_HAS_JTHREAD is false, this provides equivalent functionality
 * using std::thread with a simple_stop_source.
 */
class compat_jthread {
public:
    /**
     * @brief Default constructor - no thread
     */
    compat_jthread() noexcept = default;

    /**
     * @brief Construct and start thread with stop token support
     * @tparam F Callable type
     * @param func Function to run in thread
     *
     * The function signature should be:
     * - For jthread: void(std::stop_token)
     * - For fallback: void(simple_stop_source&)
     */
#if LOGGER_HAS_JTHREAD
    template<typename F>
    explicit compat_jthread(F&& func)
        : thread_(std::forward<F>(func)) {}
#else
    template<typename F>
    explicit compat_jthread(F&& func)
        : stop_source_(std::make_shared<simple_stop_source>())
        , thread_([func = std::forward<F>(func), stop = stop_source_]() {
            func(*stop);
        }) {}
#endif

    /**
     * @brief Destructor - requests stop and joins
     */
    ~compat_jthread() {
        if (joinable()) {
            request_stop();
            join();
        }
    }

    // Non-copyable
    compat_jthread(const compat_jthread&) = delete;
    compat_jthread& operator=(const compat_jthread&) = delete;

    // Movable
    compat_jthread(compat_jthread&& other) noexcept
#if LOGGER_HAS_JTHREAD
        : thread_(std::move(other.thread_)) {}
#else
        : stop_source_(std::move(other.stop_source_))
        , thread_(std::move(other.thread_)) {}
#endif

    compat_jthread& operator=(compat_jthread&& other) noexcept {
        if (this != &other) {
            if (joinable()) {
                request_stop();
                join();
            }
#if LOGGER_HAS_JTHREAD
            thread_ = std::move(other.thread_);
#else
            stop_source_ = std::move(other.stop_source_);
            thread_ = std::move(other.thread_);
#endif
        }
        return *this;
    }

    /**
     * @brief Check if thread is joinable
     */
    [[nodiscard]] bool joinable() const noexcept {
#if LOGGER_HAS_JTHREAD
        return thread_.joinable();
#else
        return thread_.joinable();
#endif
    }

    /**
     * @brief Request the thread to stop
     */
    void request_stop() {
#if LOGGER_HAS_JTHREAD
        thread_.request_stop();
#else
        if (stop_source_) {
            stop_source_->request_stop();
        }
#endif
    }

    /**
     * @brief Wait for thread to complete
     */
    void join() {
#if LOGGER_HAS_JTHREAD
        if (thread_.joinable()) {
            thread_.join();
        }
#else
        if (thread_.joinable()) {
            thread_.join();
        }
#endif
    }

private:
#if LOGGER_HAS_JTHREAD
    std::jthread thread_;
#else
    std::shared_ptr<simple_stop_source> stop_source_;
    std::thread thread_;
#endif
};

/**
 * @brief Condition variable wait helper with stop support
 *
 * Provides a unified interface for waiting on condition variables
 * with stop token support, regardless of jthread availability.
 */
class compat_cv_wait {
public:
    /**
     * @brief Wait on condition variable with stop support
     * @tparam Lock Lock type
     * @tparam Predicate Predicate type
     * @param cv Condition variable
     * @param lock Lock to use
     * @param stop Stop source to check
     * @param pred Predicate to check
     * @return true if predicate is satisfied, false if stop was requested
     */
#if LOGGER_HAS_JTHREAD
    template<typename Lock, typename Predicate>
    static bool wait(std::condition_variable_any& cv,
                     Lock& lock,
                     std::stop_token stop,
                     Predicate pred) {
        return cv.wait(lock, stop, pred);
    }
#else
    template<typename Lock, typename Predicate>
    static bool wait(std::condition_variable& cv,
                     Lock& lock,
                     simple_stop_source& stop,
                     Predicate pred) {
        cv.wait(lock, [&]() {
            return stop.stop_requested() || pred();
        });
        return !stop.stop_requested() && pred();
    }
#endif
};

} // namespace kcenon::logger::async
