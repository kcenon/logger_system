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

#include <string>
#include <chrono>

// Always use logger_system interface (Phase 3-4)
#include <kcenon/logger/interfaces/logger_interface.h>
#include <kcenon/common/patterns/result.h>
#include <kcenon/logger/core/error_codes.h>
#include "../interfaces/log_writer_interface.h"
#include "../interfaces/log_entry.h"
#include "../interfaces/log_formatter_interface.h"

/**
 * @file base_writer.h
 * @brief Abstract base class for all log output writers
 * @author 🍀☀🌕🌥 🌊
 * @since 1.0.0
 *
 * @details This file defines the abstract base class for all log writers in the system.
 * Writers are responsible for outputting formatted log messages to various destinations
 * such as console, files, network endpoints, databases, or custom targets.
 *
 * The base_writer class provides:
 * - A common interface for all writer implementations
 * - Compatibility layer between legacy and modern APIs
 * - Built-in formatting utilities for consistent output
 * - Color support for terminals that support ANSI codes
 * - IMonitorable implementation for observability (Phase 2.2.5)
 *
 * @note Writers must be thread-safe if used in async logging mode.
 * 
 * @example Implementing a custom writer:
 * @code
 * class custom_writer : public base_writer {
 * public:
 *     common::VoidResult write(logger_system::log_level level,
 *                              const std::string& message,
 *                              const std::string& file,
 *                              int line,
 *                              const std::string& function,
 *                              const std::chrono::system_clock::time_point& timestamp) override {
 *         // Format the message
 *         std::string formatted = format_log_entry(level, message, file, line, function, timestamp);
 *
 *         // Write to your custom destination
 *         if (!write_to_destination(formatted)) {
 *             return make_logger_void_result(logger_error_code::file_write_failed);
 *         }
 *
 *         return common::ok(); // Success
 *     }
 *
 *     common::VoidResult flush() override {
 *         // Flush any buffered data
 *         return flush_destination();
 *     }
 *
 *     std::string get_name() const override {
 *         return "custom_writer";
 *     }
 * };
 * @endcode
 */

namespace kcenon::logger {

/**
 * @class base_writer
 * @brief Abstract base class for all log output writers
 *
 * @details Writers are responsible for outputting log messages to various destinations.
 * This class provides a compatibility layer between the old API and the new
 * interface-based approach, implementing log_writer_interface and IMonitorable
 * for observability (Phase 2.2.5).
 *
 * Key responsibilities:
 * - Format log messages for output
 * - Handle destination-specific output logic
 * - Manage buffering and flushing
 * - Provide health status information
 *
 * @note All derived classes must implement the pure virtual methods.
 * @note For monitoring integration, use performance_monitor_adapter (Phase 3.3)
 *
 * @warning Writers used in async mode must be thread-safe.
 *
 * @since 1.0.0
 */
class base_writer : public log_writer_interface {
public:
    /**
     * @brief Constructor with optional formatter
     * @param formatter Custom formatter (default: timestamp_formatter)
     *
     * @details If no formatter is provided, a default timestamp_formatter is created.
     * This allows writers to use custom formatting strategies while maintaining
     * backward compatibility.
     *
     * @note The formatter is immutable after construction for thread safety.
     * To use a different formatter, create a new writer instance.
     *
     * @since 1.2.0 (Phase 3)
     */
    explicit base_writer(std::unique_ptr<log_formatter_interface> formatter = nullptr);

    virtual ~base_writer() = default;
    
    /**
     * @brief Write a log entry using the new interface
     * @param entry The log entry to write
     * @return common::VoidResult Success or error code
     *
     * @details This method provides compatibility with the modern log_entry structure.
     * The default implementation converts the entry to the legacy API format for
     * backward compatibility. Derived classes can override this for optimized handling.
     *
     * @note This method extracts source location information if present in the entry.
     *
     * @since 1.0.0
     */
    common::VoidResult write(const log_entry& entry) override {
        // Convert to old API for backward compatibility
        std::string file = entry.location ? entry.location->file.to_string() : "";
        int line = entry.location ? entry.location->line : 0;
        std::string function = entry.location ? entry.location->function.to_string() : "";

        return write(entry.level, entry.message.to_string(), file, line, function, entry.timestamp);
    }

    /**
     * @brief Write a log entry (legacy API for backward compatibility)
     * @param level Log severity level
     * @param message Log message text
     * @param file Source file path (empty if not available)
     * @param line Source line number (0 if not available)
     * @param function Function name (empty if not available)
     * @param timestamp Time when the log entry was created
     * @return common::VoidResult Success or error code
     *
     * @details This is the main method that derived classes must implement.
     * It receives all log information and is responsible for outputting it
     * to the writer's specific destination.
     *
     * @note Implementations should handle empty file/function strings gracefully.
     *
     * @warning This method may be called from multiple threads in async mode.
     * Implementations must be thread-safe.
     *
     * @since 1.0.0
     */
    virtual common::VoidResult write(logger_system::log_level level,
                                     const std::string& message,
                                     const std::string& file,
                                     int line,
                                     const std::string& function,
                                     const std::chrono::system_clock::time_point& timestamp) = 0;

    /**
     * @brief Flush any buffered data to the destination
     * @return common::VoidResult Success or error code
     *
     * @details Forces any buffered log messages to be written immediately.
     * This is important for ensuring data persistence before shutdown or
     * when immediate output is required.
     *
     * @note For unbuffered writers, this can be a no-op returning success.
     *
     * @example
     * @code
     * // Ensure all logs are written before critical operation
     * writer->flush();
     * perform_critical_operation();
     * @endcode
     *
     * @since 1.0.0
     */
    common::VoidResult flush() override = 0;
    
    /**
     * @brief Set whether to use color output (if supported)
     * @param use_color true to enable color output, false to disable
     * 
     * @details Enables or disables ANSI color codes in formatted output.
     * Only affects writers that output to terminals supporting color.
     * 
     * @note Has no effect on writers that don't support color (e.g., file writers).
     * 
     * @since 1.0.0
     */
    virtual void set_use_color(bool use_color) {
        use_color_ = use_color;
    }
    
    /**
     * @brief Get current color output setting
     * @return true if color output is enabled, false otherwise
     * 
     * @since 1.0.0
     */
    bool use_color() const {
        return use_color_;
    }
    
    /**
     * @brief Get the unique name of this writer
     * @return String identifier for this writer instance
     * 
     * @details Returns a unique name identifying this writer. Used for
     * writer management, debugging, and configuration.
     * 
     * @example Common names: "console", "file", "syslog", "network"
     * 
     * @since 1.0.0
     */
    virtual std::string get_name() const override = 0;
    
    /**
     * @brief Check if the writer is healthy and operational
     * @return true if the writer is functioning correctly, false otherwise
     *
     * @details Used for health monitoring and automatic failover. A writer
     * might be unhealthy if its destination is unavailable (e.g., disk full,
     * network disconnected).
     *
     * @note Default implementation always returns true. Override for writers
     * that can detect failure conditions.
     * @note For monitoring integration, use performance_monitor_adapter (Phase 3.3)
     *
     * @since 1.0.0
     */
    virtual bool is_healthy() const override { return true; }

    /**
     * @brief Get the current formatter
     * @return Pointer to current formatter (non-owning, read-only access)
     *
     * @details The formatter is immutable after construction for thread safety.
     * To use a different formatter, create a new writer instance.
     *
     * @since 1.2.0 (Phase 3)
     */
    log_formatter_interface* get_formatter() const;

protected:
    /**
     * @brief Format a log entry using the current formatter
     * @param entry The log entry to format
     * @return Formatted string ready for output
     *
     * @details Uses the configured formatter to convert the log entry to a string.
     * This is the recommended method for formatting in derived classes.
     *
     * @note Thread-safe if the formatter is thread-safe.
     *
     * @since 1.2.0 (Phase 3)
     */
    std::string format_log_entry(const log_entry& entry) const;

private:
    /** Current formatter (owns the formatter) */
    std::unique_ptr<log_formatter_interface> formatter_;

    /** Legacy color setting */
    bool use_color_ = true;
};

} // namespace kcenon::logger
