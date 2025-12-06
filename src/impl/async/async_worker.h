#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file async_worker.h
 * @brief Standalone async worker implementation with jthread compatibility
 * @author 🍀☀🌕🌥 🌊
 * @since 1.3.0
 *
 * @details This file provides a standalone async worker that uses std::jthread
 * for cooperative cancellation support where available, with a fallback to
 * std::thread + manual stop mechanism for environments without jthread support
 * (e.g., libc++ on macOS/Clang).
 *
 * Key features:
 * - std::jthread with std::stop_token for graceful shutdown (when available)
 * - Fallback to std::thread with manual stop signaling (for libc++)
 * - Lock-free queue for high-throughput message passing
 * - Configurable queue size and batch processing
 * - Cooperative cancellation without blocking operations
 *
 * @note Part of Issue #222: Remove thread_system dependency
 */

#include "jthread_compat.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace kcenon::logger::async {

/**
 * @brief Task type for async worker
 *
 * A task is a callable that performs a logging operation.
 * Using std::function allows flexible task composition.
 */
using task_type = std::function<void()>;

/**
 * @brief Standalone async worker with jthread compatibility
 *
 * This worker provides a background thread for processing logging tasks
 * asynchronously. It uses std::jthread's cooperative cancellation mechanism
 * via std::stop_token for graceful shutdown when available, or falls back
 * to manual stop mechanism for environments without jthread support.
 *
 * Thread Safety:
 * - enqueue() is thread-safe for multiple producers
 * - The worker thread is the single consumer
 * - flush() blocks until all pending tasks are processed
 *
 * Performance Characteristics:
 * - Lock-free enqueue path when queue is not full
 * - Batch processing reduces context switching overhead
 * - Configurable queue size for different workloads
 *
 * Usage:
 * @code
 * async_worker worker(8192);
 * worker.start();
 *
 * worker.enqueue([&logger, message]() {
 *     logger.write(message);
 * });
 *
 * worker.flush();  // Wait for all pending tasks
 * worker.stop();   // Graceful shutdown
 * @endcode
 *
 * @since 1.3.0
 */
class async_worker {
public:
    /**
     * @brief Constructor with configurable queue size
     * @param queue_size Maximum number of pending tasks (default: 8192)
     *
     * The queue size should be chosen based on expected logging throughput:
     * - Higher values reduce the chance of dropped messages under burst load
     * - Lower values reduce memory footprint
     *
     * @note Memory usage is approximately queue_size * sizeof(task_type)
     */
    explicit async_worker(std::size_t queue_size = 8192);

    /**
     * @brief Destructor - ensures graceful shutdown
     *
     * Automatically calls stop() if the worker is still running.
     * The destructor will block until all pending tasks are processed
     * and the worker thread has terminated.
     */
    ~async_worker();

    // Non-copyable, non-movable (owns thread resource)
    async_worker(const async_worker&) = delete;
    async_worker& operator=(const async_worker&) = delete;
    async_worker(async_worker&&) = delete;
    async_worker& operator=(async_worker&&) = delete;

    /**
     * @brief Start the worker thread
     *
     * Creates a background thread that processes tasks from the queue.
     * The thread will run until stop() is called or the worker is destroyed.
     *
     * Thread-safe: Can be called from any thread.
     * Idempotent: Multiple calls are safe (subsequent calls are no-ops).
     */
    void start();

    /**
     * @brief Stop the worker thread gracefully
     *
     * Signals the worker thread to stop and waits for it to complete
     * processing remaining tasks. This method:
     * 1. Requests stop via stop mechanism
     * 2. Wakes up the worker thread
     * 3. Waits for the thread to join
     * 4. Processes any remaining tasks in the queue
     *
     * Thread-safe: Can be called from any thread.
     * Idempotent: Multiple calls are safe (subsequent calls are no-ops).
     *
     * @note This method blocks until the worker thread terminates.
     */
    void stop();

    /**
     * @brief Enqueue a task for async processing
     * @param task The task to execute (callable with no arguments)
     * @return true if task was enqueued, false if queue is full
     *
     * If the queue is full, the task is dropped and false is returned.
     * The caller should handle this case (e.g., log synchronously or retry).
     *
     * Thread-safe: Multiple threads can call enqueue() concurrently.
     *
     * @code
     * if (!worker.enqueue(task)) {
     *     // Queue full - handle overflow
     *     task();  // Execute synchronously as fallback
     * }
     * @endcode
     */
    bool enqueue(task_type task);

    /**
     * @brief Flush all pending tasks
     *
     * Blocks until all tasks currently in the queue have been processed.
     * New tasks enqueued during flush() may or may not be included.
     *
     * Thread-safe: Can be called from any thread.
     *
     * @note This method may block for an extended period if the queue
     *       contains many tasks or tasks are slow to execute.
     */
    void flush();

    /**
     * @brief Check if the worker is currently running
     * @return true if the worker thread is active
     */
    [[nodiscard]] bool is_running() const noexcept;

    /**
     * @brief Get the current queue size
     * @return Number of pending tasks in the queue
     *
     * @note This is an approximate value due to concurrent access.
     */
    [[nodiscard]] std::size_t pending_count() const;

    /**
     * @brief Get the queue capacity
     * @return Maximum number of tasks the queue can hold
     */
    [[nodiscard]] std::size_t capacity() const noexcept;

    /**
     * @brief Get the number of dropped tasks due to queue overflow
     * @return Total number of tasks that were not enqueued
     */
    [[nodiscard]] std::uint64_t dropped_count() const noexcept;

private:
#if LOGGER_HAS_JTHREAD
    /**
     * @brief Worker thread main loop (jthread version)
     * @param stop_token Token for cooperative cancellation
     *
     * Processes tasks from the queue until stop is requested.
     * Uses condition variable for efficient waiting.
     */
    void worker_loop(std::stop_token stop_token);
#else
    /**
     * @brief Worker thread main loop (fallback version)
     * @param stop Stop source for manual cancellation
     *
     * Processes tasks from the queue until stop is requested.
     * Uses condition variable for efficient waiting.
     */
    void worker_loop(simple_stop_source& stop);
#endif

    /**
     * @brief Process all remaining tasks in the queue
     *
     * Called during shutdown to ensure no tasks are lost.
     */
    void drain_queue();

private:
    const std::size_t queue_size_;          ///< Maximum queue capacity
    std::queue<task_type> queue_;           ///< Task queue
    mutable std::mutex queue_mutex_;        ///< Protects queue_ access

#if LOGGER_HAS_JTHREAD
    std::condition_variable_any queue_cv_;  ///< Signals new tasks or stop (jthread)
#else
    std::condition_variable queue_cv_;      ///< Signals new tasks or stop (fallback)
#endif

    compat_jthread worker_thread_;          ///< Background worker thread
    std::atomic<bool> running_{false};      ///< Worker state flag
    std::atomic<std::uint64_t> dropped_count_{0};  ///< Overflow counter
};

} // namespace kcenon::logger::async
