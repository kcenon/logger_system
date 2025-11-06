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

// Conditional include based on build mode
#include <kcenon/logger/core/thread_integration_detector.h>

#ifdef USE_THREAD_SYSTEM_INTEGRATION
    #include <kcenon/thread/interfaces/logger_interface.h>
#else
    #include <kcenon/logger/interfaces/logger_interface.h>
#endif

#include <kcenon/logger/core/error_codes.h>
#include "../interfaces/log_writer_interface.h"
#include "../interfaces/log_entry.h"
#include "../interfaces/log_formatter_interface.h"

// Conditional monitoring interface (Phase 2.2.5)
#ifdef BUILD_WITH_COMMON_SYSTEM
    #include <kcenon/common/interfaces/monitoring_interface.h>
#endif

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
 *     result_void write(logger_system::log_level level,
 *                      const std::string& message,
 *                      const std::string& file,
 *                      int line,
 *                      const std::string& function,
 *                      const std::chrono::system_clock::time_point& timestamp) override {
 *         // Format the message
 *         std::string formatted = format_log_entry(level, message, file, line, function, timestamp);
 *         
 *         // Write to your custom destination
 *         if (!write_to_destination(formatted)) {
 *             return logger_error_code::write_failed;
 *         }
 *         
 *         return {}; // Success
 *     }
 *     
 *     result_void flush() override {
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
 * - Expose monitoring metrics and health checks
 *
 * @note All derived classes must implement the pure virtual methods.
 *
 * @warning Writers used in async mode must be thread-safe.
 *
 * @since 1.0.0
 */
class base_writer : public log_writer_interface
#ifdef BUILD_WITH_COMMON_SYSTEM
                  , public common::interfaces::IMonitorable
#endif
{
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
     * @return result_void Success or error code
     * 
     * @details This method provides compatibility with the modern log_entry structure.
     * The default implementation converts the entry to the legacy API format for
     * backward compatibility. Derived classes can override this for optimized handling.
     * 
     * @note This method extracts source location information if present in the entry.
     * 
     * @since 1.0.0
     */
    virtual result_void write(const log_entry& entry) override {
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
     * @return result_void Success or error code
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
    virtual result_void write(logger_system::log_level level,
                              const std::string& message,
                              const std::string& file,
                              int line,
                              const std::string& function,
                              const std::chrono::system_clock::time_point& timestamp) = 0;
    
    /**
     * @brief Flush any buffered data to the destination
     * @return result_void Success or error code
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
    virtual result_void flush() override = 0;
    
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
     *
     * @since 1.0.0
     */
    virtual bool is_healthy() const override { return true; }

#ifdef BUILD_WITH_COMMON_SYSTEM
    // IMonitorable interface implementation (Phase 2.2.5)

    /**
     * @brief Get monitoring data for this writer
     * @return Result containing metrics snapshot
     *
     * @details Provides basic writer metrics. Derived classes can override
     * to add writer-specific metrics.
     */
    virtual common::Result<common::interfaces::metrics_snapshot> get_monitoring_data() override {
        common::interfaces::metrics_snapshot snapshot;
        snapshot.source_id = "logger_writer::" + get_name();
        snapshot.capture_time = std::chrono::system_clock::now();
        snapshot.add_metric("is_healthy", is_healthy() ? 1.0 : 0.0);
        return snapshot;
    }

    /**
     * @brief Perform health check on this writer
     * @return Result containing health check result
     */
    virtual common::Result<common::interfaces::health_check_result> health_check() override {
        common::interfaces::health_check_result result;
        result.timestamp = std::chrono::system_clock::now();
        result.status = is_healthy() ?
            common::interfaces::health_status::healthy :
            common::interfaces::health_status::unhealthy;
        result.message = is_healthy() ?
            "Writer operational" :
            "Writer unhealthy";
        result.metadata["writer_name"] = get_name();
        return result;
    }

    /**
     * @brief Get component name for monitoring
     * @return Component identifier
     */
    virtual std::string get_component_name() const override {
        return "logger_writer::" + get_name();
    }
#endif // BUILD_WITH_COMMON_SYSTEM

    /**
     * @brief Get the current formatter
     * @return Pointer to current formatter (non-owning)
     *
     * @note The formatter is read-only after construction for thread safety.
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

    /**
     * @brief Format a log entry to a human-readable string (legacy API)
     * @param level Log severity level
     * @param message Log message text
     * @param file Source file path
     * @param line Source line number
     * @param function Function name
     * @param timestamp Time of log entry
     * @return Formatted string ready for output
     *
     * @details Provides a default formatting implementation that derived classes
     * can use. The format includes timestamp, level, message, and optional source
     * location. Color codes are included if color output is enabled.
     *
     * @deprecated Use format_log_entry(const log_entry&) instead.
     * This method is provided for backward compatibility and will be removed
     * in a future version. It internally converts parameters to log_entry and
     * uses the formatter.
     *
     * @note Format: "[YYYY-MM-DD HH:MM:SS.mmm] [LEVEL] message [file:line in function()]"
     *
     * @since 1.0.0
     */
    [[deprecated("Use format_log_entry(const log_entry&) instead")]]
    std::string format_log_entry(logger_system::log_level level,
                                const std::string& message,
                                const std::string& file,
                                int line,
                                const std::string& function,
                                const std::chrono::system_clock::time_point& timestamp);

    /**
     * @brief Convert log level to string representation
     * @param level Log level to convert
     * @return String representation of the level
     *
     * @details Maps log levels to human-readable strings:
     * - trace -> "TRACE"
     * - debug -> "DEBUG"
     * - info -> "INFO"
     * - warning -> "WARNING"
     * - error -> "ERROR"
     *
     * @deprecated This method is deprecated. Formatting is now handled by formatters.
     *
     * @since 1.0.0
     */
    [[deprecated("Formatting is now handled by formatters")]]
    std::string level_to_string(logger_system::log_level level) const;

    /**
     * @brief Get ANSI color code for the specified log level
     * @param level Log level to get color for
     * @return ANSI escape sequence for color, or empty string if color is disabled
     *
     * @details Returns appropriate ANSI color codes for terminal output:
     * - trace -> Gray/Dim
     * - debug -> Cyan
     * - info -> Green
     * - warning -> Yellow
     * - error -> Red
     *
     * @note Returns empty string if use_color() is false.
     *
     * @warning Only use for terminals that support ANSI escape codes.
     *
     * @deprecated This method is deprecated. Formatting is now handled by formatters.
     *
     * @since 1.0.0
     */
    [[deprecated("Formatting is now handled by formatters")]]
    std::string level_to_color(logger_system::log_level level) const;

private:
    /** Current formatter (owns the formatter) */
    std::unique_ptr<log_formatter_interface> formatter_;

    /** Legacy color setting */
    bool use_color_ = true;
};

} // namespace kcenon::logger
