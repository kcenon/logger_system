// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#pragma once

/**
 * @file filtered_writer.h
 * @brief Decorator that applies filtering to wrapped log writers
 * @author kcenon
 * @since 4.0.0
 *
 * @details This file defines the filtered_writer class, a Decorator pattern
 * implementation that wraps any log_writer_interface and applies a filter
 * before delegating writes. This enables composable filtering at the writer
 * level, allowing different filters for different output destinations.
 *
 * Part of the Decorator pattern refactoring (#356).
 *
 * @example Basic usage:
 * @code
 * // Create a file writer that only logs errors
 * auto writer = std::make_unique<filtered_writer>(
 *     std::make_unique<file_writer>("errors.log"),
 *     std::make_unique<level_filter>(log_level::error)
 * );
 *
 * // Compose with other decorators
 * auto async_filtered = std::make_unique<async_writer>(
 *     std::make_unique<filtered_writer>(
 *         std::make_unique<console_writer>(),
 *         std::make_unique<level_filter>(log_level::warning)
 *     )
 * );
 * @endcode
 */

#include "decorator_writer_base.h"
#include "../interfaces/log_filter_interface.h"

#include <kcenon/logger/logger_export.h>

#include <memory>

namespace kcenon::logger {

/**
 * @class filtered_writer
 * @brief Decorator that applies a filter to a wrapped writer
 *
 * @details This class implements the Decorator pattern for log writers.
 * It wraps any log_writer_interface implementation and applies filtering
 * logic before delegating write operations. Log entries that do not pass
 * the filter are silently dropped (returning success).
 *
 * Key features:
 * - Composable with any log_writer_interface implementation
 * - Works with all filter types from log_filter.h
 * - Can be nested with other decorators (async_writer, batch_writer, etc.)
 * - Thread-safe if the wrapped writer and filter are thread-safe
 *
 * Category: Synchronous (delegates to wrapped writer), Decorator
 *
 * @note The filter is evaluated before each write operation.
 * @note Filtered entries return success (not an error).
 *
 * @since 4.0.0
 * @since 4.1.0 Migrated to use decorator_writer_base for code reuse
 */
class LOGGER_SYSTEM_API filtered_writer : public decorator_writer_base {
public:
    /**
     * @brief Construct a filtered writer
     *
     * @param wrapped The writer to wrap with filtering
     * @param filter The filter to apply before writing
     *
     * @throws std::invalid_argument if wrapped is nullptr
     *
     * @note filter can be nullptr, in which case all entries pass through
     *
     * @since 4.0.0
     */
    explicit filtered_writer(std::unique_ptr<log_writer_interface> wrapped,
                             std::unique_ptr<log_filter_interface> filter);

    /**
     * @brief Destructor
     */
    ~filtered_writer() override = default;

    // Non-copyable
    filtered_writer(const filtered_writer&) = delete;
    filtered_writer& operator=(const filtered_writer&) = delete;

    // Movable
    filtered_writer(filtered_writer&&) noexcept = default;
    filtered_writer& operator=(filtered_writer&&) noexcept = default;

    /**
     * @brief Write a log entry if it passes the filter
     *
     * @param entry The log entry to write
     * @return common::VoidResult Success if filtered out or written successfully,
     *         error if write operation fails
     *
     * @details If the filter is nullptr or the entry passes the filter,
     * the entry is delegated to the wrapped writer. If the entry does not
     * pass the filter, this method returns success without writing.
     *
     * @since 4.0.0
     */
    common::VoidResult write(const log_entry& entry) override;

    /**
     * @brief Get the name of this writer
     *
     * @return String in format "filtered_<wrapped_name>" or
     *         "filtered(<filter_name>)_<wrapped_name>" if filter has a name
     *
     * @details Overrides base class to include filter name when available.
     *
     * @since 4.0.0
     */
    std::string get_name() const override;

    /**
     * @brief Get the current filter
     *
     * @return Pointer to the filter (may be nullptr)
     *
     * @since 4.0.0
     */
    const log_filter_interface* get_filter() const;

private:
    std::unique_ptr<log_filter_interface> filter_;
};

/**
 * @brief Factory function to create a filtered writer
 *
 * @param writer The writer to wrap
 * @param filter The filter to apply
 * @return Unique pointer to the filtered writer
 *
 * @throws std::invalid_argument if writer is nullptr
 *
 * @since 4.0.0
 */
std::unique_ptr<filtered_writer> make_filtered_writer(
    std::unique_ptr<log_writer_interface> writer,
    std::unique_ptr<log_filter_interface> filter);

}  // namespace kcenon::logger
