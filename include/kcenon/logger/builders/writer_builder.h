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
 * @brief Fluent API builder for composing log writer decorators
 * @author kcenon
 * @since 4.0.0
 *
 * @details This file provides a builder pattern implementation for creating
 * complex log writer configurations with a fluent, chainable API. It simplifies
 * the composition of decorator patterns by replacing verbose manual nesting
 * with readable method chaining.
 *
 * Example usage:
 * @code
 * // Simple file writer
 * auto writer = writer_builder()
 *     .file("app.log")
 *     .build();
 *
 * // Production setup with multiple decorators
 * auto writer = writer_builder()
 *     .file("app.log")
 *     .buffered(500)
 *     .encrypted(key)
 *     .async(10000)
 *     .build();
 * @endcode
 *
 * Part of EPIC #391 - Decorator pattern refactoring
 */

#include "../interfaces/log_writer_interface.h"
#include "../security/secure_key_storage.h"
#include "../writers/network_writer.h"
#include "../writers/rotating_file_writer.h"
#include "../writers/otlp_writer.h"

#include <memory>
#include <string>
#include <cstddef>
#include <cstdint>
#include <chrono>

namespace kcenon::logger {

// Forward declarations
class log_filter_interface;
struct encryption_config;

/**
 * @class writer_builder
 * @brief Fluent API builder for composing log writer decorators
 *
 * @details This class implements the Builder pattern to create complex log
 * writer configurations. It provides a fluent interface for chaining core
 * writers with decorators in a readable, self-documenting way.
 *
 * Decorator composition order (from core to outermost):
 * 1. Core writer (file/console/network/otlp)
 * 2. Filtering decorators
 * 3. Buffering decorators
 * 4. Encryption decorators
 * 5. Thread-safety decorators
 * 6. Async decorators
 *
 * All methods return *this to enable method chaining.
 * Call build() to finalize and obtain the configured writer.
 *
 * @note This class is not thread-safe. Each builder instance should be
 * used from a single thread.
 *
 * @since 4.0.0
 */
class writer_builder {
public:
    /**
     * @brief Construct a new writer builder
     */
    writer_builder() = default;

    /**
     * @brief Destructor
     */
    ~writer_builder() = default;

    // Non-copyable, movable
    writer_builder(const writer_builder&) = delete;
    writer_builder& operator=(const writer_builder&) = delete;
    writer_builder(writer_builder&&) noexcept = default;
    writer_builder& operator=(writer_builder&&) noexcept = default;

    //========================================================================
    // Core Writers (Terminal Nodes)
    //========================================================================

    /**
     * @brief Configure a file writer
     * @param path Path to the log file
     * @param append Append to existing file (default: true)
     * @return Reference to this builder for chaining
     *
     * @note This replaces any previously configured core writer
     */
    writer_builder& file(const std::string& path,
                         bool append = true);

    /**
     * @brief Configure a rotating file writer (size-based)
     * @param path Path to the log file
     * @param max_size Maximum file size before rotation
     * @param max_files Maximum number of backup files
     * @param check_interval Writes between rotation checks (default: 100)
     * @return Reference to this builder for chaining
     *
     * @note This replaces any previously configured core writer
     */
    writer_builder& rotating_file(const std::string& path,
                                  std::size_t max_size,
                                  std::size_t max_files,
                                  std::size_t check_interval = 100);

    /**
     * @brief Configure a rotating file writer (time-based)
     * @param path Path to the log file
     * @param type Rotation type (daily/hourly)
     * @param max_files Maximum number of backup files
     * @param check_interval Writes between rotation checks (default: 100)
     * @return Reference to this builder for chaining
     *
     * @note This replaces any previously configured core writer
     */
    writer_builder& rotating_file(const std::string& path,
                                  rotation_type type,
                                  std::size_t max_files,
                                  std::size_t check_interval = 100);

    /**
     * @brief Configure a console writer
     * @param use_stderr Send output to stderr instead of stdout (default: false)
     * @param auto_detect_color Auto-detect terminal color support (default: true)
     * @return Reference to this builder for chaining
     *
     * @note This replaces any previously configured core writer
     */
    writer_builder& console(bool use_stderr = false,
                           bool auto_detect_color = true);

    /**
     * @brief Configure a network writer
     * @param host Remote host address
     * @param port Remote port number
     * @param protocol Network protocol (TCP/UDP, default: TCP)
     * @param buffer_size Internal buffer size (default: 8192)
     * @param reconnect_interval Reconnection interval (default: 5s)
     * @return Reference to this builder for chaining
     *
     * @note This replaces any previously configured core writer
     */
    writer_builder& network(const std::string& host,
                           uint16_t port,
                           network_writer::protocol_type protocol = network_writer::protocol_type::tcp,
                           std::size_t buffer_size = 8192,
                           std::chrono::seconds reconnect_interval = std::chrono::seconds(5));

    /**
     * @brief Configure an OTLP writer
     * @param config OTLP configuration
     * @return Reference to this builder for chaining
     *
     * @note This replaces any previously configured core writer
     */
    writer_builder& otlp(const otlp_writer::config& config);

    //========================================================================
    // Decorators (Middleware)
    //========================================================================

    /**
     * @brief Add async decorator
     * @param queue_size Maximum queue size for pending messages (default: 10000)
     * @param flush_timeout Maximum time to wait for flush in seconds (default: 5s)
     * @return Reference to this builder for chaining
     *
     * @note Wraps the current writer with async functionality
     */
    writer_builder& async(std::size_t queue_size = 10000,
                         std::chrono::seconds flush_timeout = std::chrono::seconds(5));

    /**
     * @brief Add buffered decorator
     * @param max_entries Maximum number of entries to buffer (default: 100)
     * @param flush_interval Auto-flush interval (default: 5s)
     * @return Reference to this builder for chaining
     *
     * @note Wraps the current writer with buffering functionality
     */
    writer_builder& buffered(std::size_t max_entries = 100,
                            std::chrono::milliseconds flush_interval = std::chrono::milliseconds(5000));

    /**
     * @brief Add encrypted decorator
     * @param config Encryption configuration with key and algorithm
     * @return Reference to this builder for chaining
     *
     * @note Wraps the current writer with encryption functionality
     * @note Requires OpenSSL support
     */
    writer_builder& encrypted(encryption_config config);

    /**
     * @brief Add thread-safe decorator
     * @return Reference to this builder for chaining
     *
     * @note Wraps the current writer with mutex-based synchronization
     */
    writer_builder& thread_safe();

    /**
     * @brief Add filtered decorator
     * @param filter Filter interface to apply
     * @return Reference to this builder for chaining
     *
     * @note Wraps the current writer with filtering functionality
     */
    writer_builder& filtered(std::unique_ptr<log_filter_interface> filter);

    //========================================================================
    // Build Method
    //========================================================================

    /**
     * @brief Build the configured writer
     * @return Unique pointer to the configured writer
     *
     * @throws std::logic_error if no core writer has been configured
     *
     * @note After calling build(), the builder is left in a valid but
     * unspecified state (ownership transferred)
     */
    log_writer_ptr build();

private:
    /**
     * @brief Check if a core writer has been configured
     * @throws std::logic_error if no core writer exists
     */
    void ensure_core_writer() const;

    /**
     * @brief The current writer being built (core + decorators)
     */
    log_writer_ptr writer_;
};

} // namespace kcenon::logger
