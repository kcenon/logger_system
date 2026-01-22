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
 * @file formatted_writer.h
 * @brief Decorator that applies formatting to wrapped log writers
 * @author kcenon
 * @since 4.0.0
 *
 * @details This file defines the formatted_writer class, a Decorator pattern
 * implementation that wraps any log_writer_interface and applies a formatter
 * before delegating writes. This enables composable formatting at the writer
 * level, allowing different formats for different output destinations.
 *
 * Part of the Decorator pattern refactoring (#356).
 *
 * @example Basic usage:
 * @code
 * // Create a file writer with JSON formatting
 * auto writer = std::make_unique<formatted_writer>(
 *     std::make_unique<file_writer>("app.log"),
 *     std::make_unique<json_formatter>()
 * );
 *
 * // Compose with other decorators
 * auto filtered_formatted = std::make_unique<filtered_writer>(
 *     std::make_unique<formatted_writer>(
 *         std::make_unique<file_writer>("errors.json"),
 *         std::make_unique<json_formatter>()
 *     ),
 *     std::make_unique<level_filter>(log_level::error)
 * );
 * @endcode
 */

#include "decorator_writer_base.h"
#include "../interfaces/log_formatter_interface.h"

#include <memory>

namespace kcenon::logger {

/**
 * @class formatted_writer
 * @brief Decorator that applies a formatter to a wrapped writer
 *
 * @details This class implements the Decorator pattern for log writers.
 * It wraps any log_writer_interface implementation and applies formatting
 * logic before delegating write operations. The formatted message is stored
 * in the log entry before passing it to the wrapped writer.
 *
 * Key features:
 * - Composable with any log_writer_interface implementation
 * - Works with all formatter types (json_formatter, timestamp_formatter, etc.)
 * - Can be nested with other decorators (filtered_writer, buffered_writer, etc.)
 * - Thread-safe if the wrapped writer and formatter are thread-safe
 *
 * Category: Synchronous (delegates to wrapped writer), Decorator
 *
 * @note The formatter is evaluated before each write operation.
 * @note If no formatter is provided, entries pass through unchanged.
 *
 * @since 4.0.0
 * @since 4.1.0 Migrated to use decorator_writer_base for code reuse
 */
class formatted_writer : public decorator_writer_base {
public:
    /**
     * @brief Construct a formatted writer
     *
     * @param wrapped The writer to wrap with formatting
     * @param formatter The formatter to apply before writing
     *
     * @throws std::invalid_argument if wrapped is nullptr
     *
     * @note formatter can be nullptr, in which case all entries pass through unchanged
     *
     * @since 4.0.0
     */
    explicit formatted_writer(std::unique_ptr<log_writer_interface> wrapped,
                              std::unique_ptr<log_formatter_interface> formatter);

    /**
     * @brief Destructor
     */
    ~formatted_writer() override = default;

    // Non-copyable
    formatted_writer(const formatted_writer&) = delete;
    formatted_writer& operator=(const formatted_writer&) = delete;

    // Movable
    formatted_writer(formatted_writer&&) noexcept = default;
    formatted_writer& operator=(formatted_writer&&) noexcept = default;

    /**
     * @brief Write a log entry after applying the formatter
     *
     * @param entry The log entry to write
     * @return common::VoidResult Success if written successfully,
     *         error if write operation fails
     *
     * @details If the formatter is nullptr, the entry is delegated directly
     * to the wrapped writer. Otherwise, the formatter is applied to create
     * a formatted message, which is then stored in a new log entry with
     * the same metadata.
     *
     * @since 4.0.0
     */
    common::VoidResult write(const log_entry& entry) override;

    /**
     * @brief Get the name of this writer
     *
     * @return String in format "formatted_<wrapped_name>" or
     *         "formatted(<formatter_name>)_<wrapped_name>" if formatter has a name
     *
     * @details Overrides base class to include formatter name when available.
     *
     * @since 4.0.0
     */
    std::string get_name() const override;

    /**
     * @brief Get the current formatter
     *
     * @return Pointer to the formatter (may be nullptr)
     *
     * @since 4.0.0
     */
    const log_formatter_interface* get_formatter() const;

private:
    std::unique_ptr<log_formatter_interface> formatter_;
};

/**
 * @brief Factory function to create a formatted writer
 *
 * @param writer The writer to wrap
 * @param formatter The formatter to apply
 * @return Unique pointer to the formatted writer
 *
 * @throws std::invalid_argument if writer is nullptr
 *
 * @since 4.0.0
 */
std::unique_ptr<formatted_writer> make_formatted_writer(
    std::unique_ptr<log_writer_interface> writer,
    std::unique_ptr<log_formatter_interface> formatter);

}  // namespace kcenon::logger
