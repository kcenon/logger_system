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
 * @file writer_builder.h
 * @brief Fluent builder for composing log writer decorators
 * @author kcenon
 * @since 4.1.0
 *
 * @details This file defines the writer_builder class, which provides a fluent
 * API for composing writer decorators in a readable and maintainable way.
 * Instead of manually nesting decorators, users can chain method calls to
 * build complex writer configurations.
 *
 * Part of EPIC #391 - Decorator pattern refactoring.
 *
 * @example Basic usage:
 * @code
 * // Simple file writer with buffering and async
 * auto writer = writer_builder()
 *     .file("app.log")
 *     .buffered(100)
 *     .async(10000)
 *     .build();
 *
 * // Console writer with filtering
 * auto writer = writer_builder()
 *     .console()
 *     .filtered(std::make_unique<level_filter>(log_level::warning))
 *     .build();
 *
 * // Encrypted file writer
 * auto key_result = secure_key_storage::generate_key(32);
 * auto writer = writer_builder()
 *     .file("secure.log.enc")
 *     .encrypted(std::move(key_result.value()))
 *     .buffered()
 *     .async()
 *     .build();
 * @endcode
 */

#include "../interfaces/log_writer_interface.h"
#include "../interfaces/log_filter_interface.h"
#include "../interfaces/log_formatter_interface.h"

#include <chrono>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>

namespace kcenon::logger {

// Forward declarations
namespace security {
class secure_key;
}  // namespace security

/**
 * @class writer_builder
 * @brief Fluent builder for composing log writer decorators
 *
 * @details This class implements the Builder pattern to provide a fluent API
 * for creating log writers with various decorators. The builder maintains an
 * internal writer instance that gets progressively wrapped with decorators
 * as methods are called.
 *
 * Key features:
 * - Method chaining for readable configuration
 * - Type-safe composition (decorators can only wrap existing writers)
 * - Clear error messages for invalid configurations
 * - Supports all existing decorators (async, buffered, encrypted, filtered)
 *
 * Usage pattern:
 * 1. Create a builder instance
 * 2. Call a core writer method (file(), console(), etc.)
 * 3. Chain decorator methods (buffered(), async(), filtered(), etc.)
 * 4. Call build() to get the final writer
 *
 * @note The builder is move-only to ensure ownership semantics are clear.
 * @note Calling build() transfers ownership of the writer and resets the builder.
 *
 * @since 4.1.0
 */
class writer_builder {
public:
    /**
     * @brief Default constructor
     * @since 4.1.0
     */
    writer_builder() = default;

    /**
     * @brief Destructor
     * @since 4.1.0
     */
    ~writer_builder() = default;

    // Move-only semantics
    writer_builder(const writer_builder&) = delete;
    writer_builder& operator=(const writer_builder&) = delete;
    writer_builder(writer_builder&&) noexcept = default;
    writer_builder& operator=(writer_builder&&) noexcept = default;

    //========================================================================
    // Core Writers (Terminal Nodes)
    //========================================================================

    /**
     * @brief Set a file writer as the core writer
     *
     * @param filename Path to the log file
     * @param append Whether to append to existing file (default: true)
     * @return Reference to this builder for chaining
     *
     * @throws std::logic_error if a core writer is already set
     *
     * @since 4.1.0
     */
    writer_builder& file(const std::string& filename, bool append = true);

    /**
     * @brief Set a console writer as the core writer
     *
     * @param use_stderr Whether to use stderr instead of stdout (default: false)
     * @param auto_detect_color Whether to auto-detect color support (default: true)
     * @return Reference to this builder for chaining
     *
     * @throws std::logic_error if a core writer is already set
     *
     * @since 4.1.0
     */
    writer_builder& console(bool use_stderr = false, bool auto_detect_color = true);

    /**
     * @brief Use a custom writer as the core writer
     *
     * @param writer Custom writer to use as the base
     * @return Reference to this builder for chaining
     *
     * @throws std::invalid_argument if writer is nullptr
     * @throws std::logic_error if a core writer is already set
     *
     * @since 4.1.0
     */
    writer_builder& custom(std::unique_ptr<log_writer_interface> writer);

    //========================================================================
    // Decorator Writers (Middleware)
    //========================================================================

    /**
     * @brief Add asynchronous processing decorator
     *
     * @param queue_size Maximum queue size for pending messages (default: 10000)
     * @param flush_timeout Maximum time to wait for flush (default: 5 seconds)
     * @return Reference to this builder for chaining
     *
     * @note The async writer must be started with start() after build()
     * @note Async should typically be the outermost decorator
     *
     * @throws std::logic_error if no core writer is set
     *
     * @since 4.1.0
     */
    writer_builder& async(std::size_t queue_size = 10000,
                          std::chrono::seconds flush_timeout = std::chrono::seconds(5));

    /**
     * @brief Add buffering decorator
     *
     * @param max_entries Maximum number of entries to buffer (default: 100)
     * @param flush_interval Time between automatic flushes (default: 5000ms)
     * @return Reference to this builder for chaining
     *
     * @throws std::logic_error if no core writer is set
     *
     * @since 4.1.0
     */
    writer_builder& buffered(std::size_t max_entries = 100,
                             std::chrono::milliseconds flush_interval = std::chrono::milliseconds(5000));

    /**
     * @brief Add encryption decorator
     *
     * @param key Encryption key (32 bytes for AES-256)
     * @return Reference to this builder for chaining
     *
     * @throws std::logic_error if no core writer is set
     * @throws std::invalid_argument if key is invalid
     *
     * @note Requires OpenSSL to be available
     *
     * @since 4.1.0
     */
    writer_builder& encrypted(security::secure_key key);

    /**
     * @brief Add filtering decorator
     *
     * @param filter Filter to apply before writing
     * @return Reference to this builder for chaining
     *
     * @throws std::logic_error if no core writer is set
     *
     * @note filter can be nullptr for pass-through behavior
     *
     * @since 4.1.0
     */
    writer_builder& filtered(std::unique_ptr<log_filter_interface> filter);

    /**
     * @brief Add formatting decorator
     *
     * @param formatter Formatter to apply to log entries
     * @return Reference to this builder for chaining
     *
     * @throws std::logic_error if no core writer is set
     *
     * @since 4.1.0
     */
    writer_builder& formatted(std::unique_ptr<log_formatter_interface> formatter);

    //========================================================================
    // Build
    //========================================================================

    /**
     * @brief Build and return the configured writer
     *
     * @return Unique pointer to the configured writer
     *
     * @throws std::logic_error if no core writer has been set
     *
     * @note This method transfers ownership of the writer and resets the builder.
     *       After calling build(), the builder can be reused to create a new writer.
     *
     * @since 4.1.0
     */
    log_writer_ptr build();

    /**
     * @brief Check if a core writer has been set
     *
     * @return true if a core writer has been configured
     *
     * @since 4.1.0
     */
    bool has_core_writer() const noexcept;

    /**
     * @brief Reset the builder to its initial state
     *
     * @details Clears any configured writer, allowing the builder to be reused.
     *
     * @since 4.1.0
     */
    void reset() noexcept;

private:
    /**
     * @brief Ensure a core writer exists before adding decorators
     *
     * @throws std::logic_error if no core writer is set
     */
    void ensure_core_writer() const;

    /**
     * @brief Ensure no core writer exists before setting one
     *
     * @throws std::logic_error if a core writer is already set
     */
    void ensure_no_core_writer() const;

    log_writer_ptr writer_;
};

}  // namespace kcenon::logger
