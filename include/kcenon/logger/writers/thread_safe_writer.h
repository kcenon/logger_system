#pragma once

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

#include "base_writer.h"
#include <mutex>

/**
 * @file thread_safe_writer.h
 * @brief Thread-safe base class for writer implementations
 * @author 🍀☀🌕🌥 🌊
 * @since 1.3.0
 *
 * @details This file defines the thread_safe_writer base class that provides
 * automatic thread synchronization for writer implementations. Derived classes
 * implement the protected *_impl() methods and get thread-safety for free.
 *
 * Design pattern: Template Method Pattern
 * - Base class handles synchronization in public methods
 * - Derived classes implement logic in protected *_impl methods
 * - Eliminates boilerplate mutex code from all writer implementations
 *
 * @example Implementing a custom thread-safe writer:
 * @code
 * class my_writer : public thread_safe_writer {
 * public:
 *     my_writer() : thread_safe_writer() {}
 *     std::string get_name() const override { return "my_writer"; }
 *
 * protected:
 *     common::VoidResult write_impl(common::interfaces::log_level level,
 *                                   const std::string& message,
 *                                   const std::string& file,
 *                                   int line,
 *                                   const std::string& function,
 *                                   const std::chrono::system_clock::time_point& timestamp) override {
 *         // Your write logic here - no need for mutex
 *         return common::ok();
 *     }
 *
 *     common::VoidResult flush_impl() override {
 *         // Your flush logic here - no need for mutex
 *         return common::ok();
 *     }
 * };
 * @endcode
 */

namespace kcenon::logger {

/**
 * @class thread_safe_writer
 * @brief Base class providing automatic thread-safety for writer implementations
 *
 * @details This class uses the Template Method pattern to provide automatic
 * mutex-based synchronization. Public methods acquire a lock and delegate
 * to protected virtual *_impl methods that derived classes override.
 *
 * Benefits:
 * - Eliminates mutex boilerplate from derived classes
 * - Consistent locking strategy across all writers
 * - Derived classes focus only on output-specific logic
 * - Thread-safety cannot be accidentally omitted
 *
 * Thread-safety guarantees:
 * - All public write/flush operations are serialized
 * - Derived class *_impl methods are called while holding the lock
 * - The mutex is always released, even on exceptions (RAII)
 *
 * @note Non-copyable and non-movable (contains std::mutex)
 * @note For high-performance scenarios requiring lock-free operation,
 *       use high_performance_async_writer instead.
 *
 * @since 1.3.0
 */
class thread_safe_writer : public base_writer {
public:
    /**
     * @brief Constructor with optional formatter
     * @param formatter Custom formatter (default: timestamp_formatter)
     *
     * @since 1.3.0
     */
    explicit thread_safe_writer(std::unique_ptr<log_formatter_interface> formatter = nullptr);

    /**
     * @brief Virtual destructor
     */
    ~thread_safe_writer() override = default;

    // Non-copyable and non-movable (mutex cannot be copied or moved)
    thread_safe_writer(const thread_safe_writer&) = delete;
    thread_safe_writer& operator=(const thread_safe_writer&) = delete;
    thread_safe_writer(thread_safe_writer&&) = delete;
    thread_safe_writer& operator=(thread_safe_writer&&) = delete;

    /**
     * @brief Thread-safe write operation for structured log entries
     * @param entry The log entry to write (includes structured fields)
     * @return common::VoidResult Success or error code
     *
     * @details Acquires the mutex and delegates to write_entry_impl().
     * This method preserves all log entry data including structured fields,
     * OpenTelemetry context, and category information.
     *
     * @note This is the primary method for writing logs. It preserves
     * all structured logging information.
     *
     * @since 3.4.0
     * @since 3.5.0 This is now the only write method (legacy API removed)
     */
    common::VoidResult write(const log_entry& entry) final;

    /**
     * @brief Thread-safe flush operation
     * @return common::VoidResult Success or error code
     *
     * @details Acquires the mutex and delegates to flush_impl().
     * This method is final to ensure thread-safety is not bypassed.
     *
     * @since 1.3.0
     */
    common::VoidResult flush() final;

protected:
    /**
     * @brief Implementation of structured write operation (override in derived classes)
     * @param entry The log entry to write (includes structured fields)
     * @return common::VoidResult Success or error code
     *
     * @details Called by write(const log_entry&) while holding the mutex.
     * Derived classes must override this method to implement their output logic.
     *
     * @note The mutex is held when this method is called.
     * @note Do not call public methods on 'this' (would cause deadlock).
     *
     * @since 3.4.0
     * @since 3.5.0 Now pure virtual - implementations must override this
     */
    virtual common::VoidResult write_entry_impl(const log_entry& entry) = 0;

    /**
     * @brief Implementation of flush operation (override in derived classes)
     * @return common::VoidResult Success or error code
     *
     * @details Called by flush() while holding the mutex.
     * Derived classes implement their output-specific flush logic here.
     *
     * @note The mutex is held when this method is called.
     * @note Do not call public methods on 'this' (would cause deadlock).
     *
     * @since 1.3.0
     */
    virtual common::VoidResult flush_impl() = 0;

    /**
     * @brief Access the writer mutex for extended operations
     * @return Reference to the internal mutex
     *
     * @details Allows derived classes to use the same mutex for additional
     * thread-safe operations beyond write/flush. Use with caution to avoid
     * deadlocks.
     *
     * @warning Only use for operations that cannot be performed within
     *          write_impl() or flush_impl().
     *
     * @since 1.3.0
     */
    std::mutex& get_mutex() const { return write_mutex_; }

private:
    /** Mutex for thread-safe write/flush operations */
    mutable std::mutex write_mutex_;
};

} // namespace kcenon::logger
