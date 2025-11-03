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

#include "../interfaces/log_formatter_interface.h"
#include <sstream>
#include <iomanip>

#ifdef USE_THREAD_SYSTEM_INTEGRATION
    #include <kcenon/thread/interfaces/logger_interface.h>
#else
    #include <kcenon/logger/interfaces/logger_interface.h>
#endif

/**
 * @file json_formatter.h
 * @brief JSON formatter for structured logging
 * @author 🍀☀🌕🌥 🌊
 * @since 1.2.0
 * @version Phase 3 - Code Quality Improvement
 *
 * @details Formats log messages as JSON objects for machine parsing and
 * integration with log aggregation systems (ELK, Splunk, CloudWatch, etc.).
 *
 * Output format:
 * {"timestamp":"2025-11-03T14:30:15.123Z","level":"INFO","thread_id":12345,"message":"App started"}
 *
 * With source location:
 * {"timestamp":"2025-11-03T14:30:15.123Z","level":"ERROR","thread_id":12345,"message":"Failed",
 *  "file":"network.cpp","line":42,"function":"connect"}
 *
 * @example Usage:
 * @code
 * auto formatter = std::make_unique<json_formatter>();
 * formatter->set_options({.pretty_print = true, .include_source_location = true});
 * auto writer = std::make_unique<file_writer>("app.json", std::move(formatter));
 * @endcode
 */

namespace kcenon::logger {

/**
 * @class json_formatter
 * @brief Formatter that outputs JSON-structured log messages
 *
 * @details Produces machine-readable JSON output suitable for log aggregation,
 * analysis tools, and automated processing. Supports both compact and
 * pretty-printed output formats.
 *
 * Features:
 * - ISO 8601 timestamp format (RFC 3339)
 * - Proper JSON escaping of special characters
 * - Optional pretty-printing for readability
 * - Structured source location information
 * - Compatible with JSON parsers and log aggregation systems
 *
 * Thread-safety: This formatter is stateless and thread-safe.
 *
 * @note This implementation provides basic JSON formatting without external
 * dependencies. For more complex JSON structures, consider integrating
 * nlohmann/json or similar libraries.
 *
 * @since 1.2.0
 */
class json_formatter : public log_formatter_interface {
public:
    /**
     * @brief Constructor with optional format options
     * @param opts Initial format options
     *
     * @note For JSON output, use_colors option is ignored
     *
     * @since 1.2.0
     */
    explicit json_formatter(const format_options& opts = format_options{}) {
        options_ = opts;
        // JSON doesn't support colors
        options_.use_colors = false;
    }

    /**
     * @brief Format a log entry to JSON string
     * @param entry The log entry to format
     * @return JSON-formatted string
     *
     * @details Produces a JSON object with fields:
     * - timestamp: ISO 8601 format (UTC)
     * - level: Log level as string
     * - thread_id: Thread ID as number
     * - message: Log message (escaped)
     * - file, line, function: Source location (if enabled)
     *
     * @note Thread-safe. Can be called concurrently.
     *
     * @since 1.2.0
     */
    std::string format(const log_entry& entry) const override {
        std::ostringstream oss;
        const char* indent = options_.pretty_print ? "  " : "";
        const char* newline = options_.pretty_print ? "\n" : "";

        oss << "{" << newline;

        bool first = true;

        // Timestamp (ISO 8601)
        if (options_.include_timestamp) {
            if (!first) oss << "," << newline;
            oss << indent << "\"timestamp\":\"" << format_iso8601(entry.timestamp) << "\"";
            first = false;
        }

        // Level
        if (options_.include_level) {
            if (!first) oss << "," << newline;
            oss << indent << "\"level\":\"" << level_to_string(entry.level) << "\"";
            first = false;
        }

        // Thread ID
        if (options_.include_thread_id && entry.thread_id) {
            if (!first) oss << "," << newline;
            oss << indent << "\"thread_id\":\"" << escape_json(entry.thread_id->to_string()) << "\"";
            first = false;
        }

        // Message (always include)
        if (!first) oss << "," << newline;
        oss << indent << "\"message\":\"" << escape_json(entry.message.to_string()) << "\"";
        first = false;

        // Source location
        if (options_.include_source_location && entry.location) {
            std::string file_path = entry.location->file.to_string();
            if (!file_path.empty()) {
                oss << "," << newline << indent << "\"file\":\"" << escape_json(file_path) << "\"";
            }

            if (entry.location->line > 0) {
                oss << "," << newline << indent << "\"line\":" << entry.location->line;
            }

            std::string func = entry.location->function.to_string();
            if (!func.empty()) {
                oss << "," << newline << indent << "\"function\":\"" << escape_json(func) << "\"";
            }
        }

        oss << newline << "}";

        return oss.str();
    }

    /**
     * @brief Get formatter name
     * @return "json_formatter"
     *
     * @since 1.2.0
     */
    std::string get_name() const override {
        return "json_formatter";
    }

private:
    /**
     * @brief Format timestamp to ISO 8601 / RFC 3339
     * @param tp Time point to format
     * @return ISO 8601 formatted timestamp string
     *
     * @details Produces format: YYYY-MM-DDTHH:MM:SS.sssZ
     * Example: 2025-11-03T14:30:15.123Z
     *
     * @note Uses UTC timezone (Z suffix)
     *
     * @since 1.2.0
     */
    static std::string format_iso8601(
        const std::chrono::system_clock::time_point& tp
    ) {
        auto time_t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_buf{};

#ifdef _WIN32
        gmtime_s(&tm_buf, &time_t);  // Windows thread-safe UTC version
#else
        gmtime_r(&time_t, &tm_buf);  // POSIX thread-safe UTC version
#endif

        // Format base timestamp
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &tm_buf);

        // Add milliseconds
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()
        ) % 1000;

        std::ostringstream oss;
        oss << buffer << "."
            << std::setfill('0') << std::setw(3) << ms.count()
            << "Z";  // UTC timezone

        return oss.str();
    }

    /**
     * @brief Escape special characters for JSON
     * @param str String to escape
     * @return JSON-escaped string
     *
     * @details Escapes: " \ / \b \f \n \r \t
     *
     * @since 1.2.0
     */
    static std::string escape_json(const std::string& str) {
        std::ostringstream oss;
        for (char c : str) {
            switch (c) {
                case '"':  oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '/':  oss << "\\/"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;
                default:
                    // Handle control characters
                    if (c >= 0 && c < 0x20) {
                        oss << "\\u"
                            << std::hex << std::setw(4) << std::setfill('0')
                            << static_cast<int>(c);
                    } else {
                        oss << c;
                    }
                    break;
            }
        }
        return oss.str();
    }

    /**
     * @brief Convert log level to string
     * @param level Log level to convert
     * @return String representation of level
     *
     * @since 1.2.0
     */
    static std::string level_to_string(logger_system::log_level level) {
        switch (level) {
            case logger_system::log_level::fatal:   return "CRITICAL";
            case logger_system::log_level::error:   return "ERROR";
            case logger_system::log_level::warn:    return "WARNING";
            case logger_system::log_level::info:    return "INFO";
            case logger_system::log_level::debug:   return "DEBUG";
            case logger_system::log_level::trace:   return "TRACE";
            case logger_system::log_level::off:     return "OFF";
        }
        return "UNKNOWN";
    }
};

} // namespace kcenon::logger
