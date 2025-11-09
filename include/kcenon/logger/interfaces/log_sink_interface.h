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

/**
 * @file log_sink_interface.h
 * @brief Interface for log output destinations (Single Responsibility Principle)
 * @author 🍀☀🌕🌥 🌊
 * @since 1.3.0
 * @version Sprint 5-6 - Writer Refactoring
 *
 * @details This interface represents the output destination for log messages.
 * It follows the Single Responsibility Principle by handling ONLY the I/O
 * operations, completely separated from formatting concerns.
 *
 * Key design principles:
 * - Accepts pre-formatted strings only
 * - No formatting logic whatsoever
 * - Manages destination-specific I/O (file, console, network, etc.)
 * - Handles buffering and flushing strategies
 * - Reports health status of the output destination
 *
 * This separation enables:
 * - Independent testing of I/O and formatting logic
 * - Flexible composition of formatters and sinks
 * - Easy addition of new output destinations
 * - Better adherence to SOLID principles
 */

#include <string>
#include <string_view>
#include <memory>
#include <functional>
#include <kcenon/logger/core/error_codes.h>

namespace kcenon::logger {

/**
 * @class log_sink_interface
 * @brief Abstract interface for log output destinations
 *
 * @details Defines the contract for outputting pre-formatted log messages
 * to various destinations. Sinks are responsible ONLY for I/O operations,
 * not formatting.
 *
 * Thread-safety: Implementations should be thread-safe as they may be called
 * concurrently from multiple threads when used with async logging.
 *
 * Responsibilities:
 * - Write pre-formatted strings to destination
 * - Manage buffering (if applicable)
 * - Handle flushing operations
 * - Report destination health status
 * - Handle destination-specific errors (disk full, network unavailable, etc.)
 *
 * Non-responsibilities (delegated to formatters):
 * - Log message formatting
 * - Timestamp generation
 * - Level-to-string conversion
 * - Color code insertion
 *
 * @note Implementations should handle partial writes and retries gracefully.
 *
 * @example Implementing a custom sink:
 * @code
 * class database_sink : public log_sink_interface {
 * public:
 *     database_sink(std::string connection_string)
 *         : connection_(connect(connection_string)) {}
 *
 *     result_void write_raw(std::string_view message) override {
 *         if (!connection_.is_connected()) {
 *             return logger_error_code::write_failed;
 *         }
 *         return connection_.execute_insert(message);
 *     }
 *
 *     result_void flush() override {
 *         return connection_.commit();
 *     }
 *
 *     bool is_healthy() const override {
 *         return connection_.is_connected();
 *     }
 *
 *     std::string get_name() const override {
 *         return "database_sink";
 *     }
 * };
 * @endcode
 *
 * @since 1.3.0
 */
class log_sink_interface {
public:
    virtual ~log_sink_interface() = default;

    /**
     * @brief Write a pre-formatted message to the output destination
     * @param message Pre-formatted log message (ready for output)
     * @return result_void Success or error code
     *
     * @details Writes the message to the destination without any modification.
     * The message should already be formatted by a formatter and is written
     * as-is to the output.
     *
     * Error handling:
     * - Returns success if write completes
     * - Returns error code if write fails (disk full, network error, etc.)
     * - May buffer the message instead of writing immediately
     *
     * @note This method must be thread-safe.
     *
     * @warning Do not modify or format the message in any way.
     * Accept it as-is and write it to the destination.
     *
     * @since 1.3.0
     */
    virtual result_void write_raw(std::string_view message) = 0;

    /**
     * @brief Flush any buffered data to the destination
     * @return result_void Success or error code
     *
     * @details Forces any buffered log messages to be written immediately.
     * This is critical for:
     * - Ensuring data persistence before shutdown
     * - Guaranteeing immediate output for critical messages
     * - Meeting real-time logging requirements
     *
     * Behavior:
     * - For unbuffered sinks: Can be a no-op returning success
     * - For buffered sinks: Must write all buffered data
     * - For network sinks: Should ensure transmission complete
     *
     * @note This method must be thread-safe.
     *
     * @example
     * @code
     * sink->write_raw("Critical error occurred");
     * sink->flush(); // Ensure message is persisted immediately
     * @endcode
     *
     * @since 1.3.0
     */
    virtual result_void flush() = 0;

    /**
     * @brief Check if the sink is healthy and operational
     * @return true if the sink can write messages, false otherwise
     *
     * @details Reports the health status of the output destination.
     * Used for:
     * - Health monitoring and alerting
     * - Automatic failover to backup sinks
     * - Preventing writes to failed destinations
     *
     * A sink might be unhealthy if:
     * - Disk is full or unmounted
     * - Network is disconnected
     * - File permissions are insufficient
     * - Database connection is lost
     * - Output device is unavailable
     *
     * @note This should be a lightweight check, not a full connectivity test.
     *
     * @since 1.3.0
     */
    virtual bool is_healthy() const = 0;

    /**
     * @brief Get the unique name of this sink
     * @return String identifier for this sink instance
     *
     * @details Returns a name identifying this sink for:
     * - Configuration management
     * - Debugging and diagnostics
     * - Logging and error messages
     * - Sink identification in multi-sink setups
     *
     * @example Common names: "console", "file", "syslog", "network", "database"
     *
     * @since 1.3.0
     */
    virtual std::string get_name() const = 0;

    /**
     * @brief Get sink-specific configuration or state information
     * @return Human-readable string describing sink state
     *
     * @details Provides diagnostic information about the sink's configuration
     * and current state. Useful for debugging and monitoring.
     *
     * @example
     * - file_sink might return: "file: /var/log/app.log, size: 1.2MB"
     * - network_sink might return: "tcp://192.168.1.100:514, connected: true"
     * - console_sink might return: "stdout, color: enabled"
     *
     * @note Default implementation returns empty string.
     *
     * @since 1.3.0
     */
    virtual std::string get_info() const {
        return "";
    }
};

/**
 * @brief Factory function type for creating sinks
 *
 * @details Can be used with dependency injection systems to create
 * sinks dynamically based on configuration.
 *
 * @example
 * @code
 * std::map<std::string, sink_factory> factories = {
 *     {"console", []() { return std::make_unique<console_sink>(); }},
 *     {"file", []() { return std::make_unique<file_sink>("app.log"); }}
 * };
 *
 * auto sink = factories["console"]();
 * @endcode
 *
 * @since 1.3.0
 */
using sink_factory = std::function<std::unique_ptr<log_sink_interface>()>;

} // namespace kcenon::logger
