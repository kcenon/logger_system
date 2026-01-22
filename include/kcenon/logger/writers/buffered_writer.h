#pragma once

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
 * @file buffered_writer.h
 * @brief Decorator that provides buffering for wrapped log writers
 * @author kcenon
 * @since 4.0.0
 *
 * @details This file defines the buffered_writer class, a Decorator pattern
 * implementation that wraps any log_writer_interface and buffers log entries
 * before delegating writes. This reduces I/O operations by batching writes
 * and provides configurable buffer size and flush intervals.
 *
 * Part of the Decorator pattern refactoring (#356).
 *
 * @example Basic usage:
 * @code
 * // Create a buffered file writer (100 entries, 5s flush interval)
 * auto writer = std::make_unique<buffered_writer>(
 *     std::make_unique<file_writer>("app.log"),
 *     100,
 *     std::chrono::milliseconds{5000}
 * );
 *
 * // Compose with other decorators
 * auto filtered_buffered = std::make_unique<filtered_writer>(
 *     std::make_unique<buffered_writer>(
 *         std::make_unique<file_writer>("errors.log"),
 *         50
 *     ),
 *     std::make_unique<level_filter>(log_level::error)
 * );
 * @endcode
 */

#include "decorator_writer_base.h"
#include "../interfaces/log_entry.h"

#include <chrono>
#include <mutex>
#include <vector>

namespace kcenon::logger {

/**
 * @class buffered_writer
 * @brief Decorator that buffers log entries before writing to wrapped writer
 *
 * @details This class implements the Decorator pattern for log writers.
 * It wraps any log_writer_interface implementation and buffers log entries
 * up to a configurable limit before flushing them to the wrapped writer.
 *
 * Key features:
 * - Configurable buffer size (number of entries)
 * - Configurable flush interval for time-based flushing
 * - Auto-flushes when buffer is full
 * - Thread-safe implementation with mutex protection
 * - Can be nested with other decorators
 *
 * Flush triggers:
 * 1. Buffer reaches max_entries limit
 * 2. Flush interval elapsed since last flush
 * 3. Explicit flush() call
 * 4. Destructor (ensures no data loss)
 *
 * Category: Synchronous (immediate return), Decorator
 *
 * @note Thread-safe: All operations are protected by mutex.
 * @note Destructor flushes remaining entries to prevent data loss.
 *
 * @since 4.0.0
 */
class buffered_writer : public decorator_writer_base {
public:
    /**
     * @brief Default buffer size (number of entries)
     */
    static constexpr size_t DEFAULT_BUFFER_SIZE = 100;

    /**
     * @brief Default flush interval
     */
    static constexpr std::chrono::milliseconds DEFAULT_FLUSH_INTERVAL{5000};

    /**
     * @brief Construct a buffered writer
     *
     * @param wrapped The writer to wrap with buffering
     * @param max_entries Maximum number of entries to buffer before auto-flush
     * @param flush_interval Maximum time between flushes (0 disables time-based flush)
     *
     * @throws std::invalid_argument if wrapped is nullptr
     * @throws std::invalid_argument if max_entries is 0
     *
     * @since 4.0.0
     */
    explicit buffered_writer(
        std::unique_ptr<log_writer_interface> wrapped,
        size_t max_entries = DEFAULT_BUFFER_SIZE,
        std::chrono::milliseconds flush_interval = DEFAULT_FLUSH_INTERVAL);

    /**
     * @brief Destructor - flushes any remaining buffered entries
     *
     * @details Ensures no log entries are lost by flushing the buffer
     * before destruction. Any errors during final flush are silently
     * ignored as there's no way to report them.
     *
     * @since 4.0.0
     */
    ~buffered_writer() override;

    // Non-copyable
    buffered_writer(const buffered_writer&) = delete;
    buffered_writer& operator=(const buffered_writer&) = delete;

    // Non-movable (mutex is not movable)
    buffered_writer(buffered_writer&&) = delete;
    buffered_writer& operator=(buffered_writer&&) = delete;

    /**
     * @brief Write a log entry to the buffer
     *
     * @param entry The log entry to write
     * @return common::VoidResult Success on buffering, error if flush fails
     *
     * @details Adds the entry to the internal buffer. If the buffer becomes
     * full or the flush interval has elapsed, triggers an automatic flush.
     *
     * @note Thread-safe operation.
     *
     * @since 4.0.0
     */
    common::VoidResult write(const log_entry& entry) override;

    /**
     * @brief Flush all buffered entries to the wrapped writer
     *
     * @return common::VoidResult Success if all entries written, error otherwise
     *
     * @details Writes all buffered entries to the wrapped writer and clears
     * the buffer. Also calls flush() on the wrapped writer.
     *
     * @note Thread-safe operation.
     *
     * @since 4.0.0
     */
    common::VoidResult flush() override;

    /**
     * @brief Get the current number of buffered entries
     *
     * @return Number of entries currently in the buffer
     *
     * @since 4.0.0
     */
    size_t get_buffer_count() const;

    /**
     * @brief Get the maximum buffer size
     *
     * @return Maximum number of entries the buffer can hold
     *
     * @since 4.0.0
     */
    size_t get_max_entries() const noexcept;

    /**
     * @brief Get the configured flush interval
     *
     * @return Flush interval in milliseconds
     *
     * @since 4.0.0
     */
    std::chrono::milliseconds get_flush_interval() const noexcept;

    /**
     * @brief Statistics for the buffered writer
     */
    struct stats {
        std::atomic<uint64_t> total_entries_written{0};
        std::atomic<uint64_t> total_flushes{0};
        std::atomic<uint64_t> flush_on_full{0};
        std::atomic<uint64_t> flush_on_interval{0};
        std::atomic<uint64_t> manual_flushes{0};
    };

    /**
     * @brief Get buffered writer statistics
     *
     * @return Reference to current statistics
     *
     * @since 4.0.0
     */
    const stats& get_stats() const noexcept;

    /**
     * @brief Reset statistics counters
     *
     * @since 4.0.0
     */
    void reset_stats();

private:
    /**
     * @brief Flush buffer without acquiring mutex (caller must hold lock)
     *
     * @return common::VoidResult indicating success or failure
     */
    common::VoidResult flush_buffer_unsafe();

    /**
     * @brief Check if time-based flush is needed
     *
     * @return true if flush interval has elapsed since last flush
     */
    bool should_flush_by_time() const;

    /**
     * @brief Copy a log entry for buffer storage
     *
     * @param entry Source entry to copy
     * @return Copied log entry
     */
    static log_entry copy_entry(const log_entry& entry);

    size_t max_entries_;
    std::chrono::milliseconds flush_interval_;

    mutable std::mutex mutex_;
    std::vector<log_entry> buffer_;
    std::chrono::steady_clock::time_point last_flush_time_;

    mutable stats stats_;
};

/**
 * @brief Factory function to create a buffered writer
 *
 * @param writer The writer to wrap
 * @param max_entries Maximum buffer size
 * @param flush_interval Time between automatic flushes
 * @return Unique pointer to the buffered writer
 *
 * @throws std::invalid_argument if writer is nullptr or max_entries is 0
 *
 * @since 4.0.0
 */
std::unique_ptr<buffered_writer> make_buffered_writer(
    std::unique_ptr<log_writer_interface> writer,
    size_t max_entries = buffered_writer::DEFAULT_BUFFER_SIZE,
    std::chrono::milliseconds flush_interval = buffered_writer::DEFAULT_FLUSH_INTERVAL);

}  // namespace kcenon::logger
