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
#include <ctime>

#ifdef USE_THREAD_SYSTEM_INTEGRATION
    #include <kcenon/thread/interfaces/logger_interface.h>
#else
    #include <kcenon/logger/interfaces/logger_interface.h>
#endif

/**
 * @file timestamp_formatter.h
 * @brief Default human-readable formatter with timestamps
 * @author 🍀☀🌕🌥 🌊
 * @since 1.2.0
 * @version Phase 3 - Code Quality Improvement
 *
 * @details Implements the default log formatting style used by the logger system.
 * Produces human-readable output with timestamp, level, message, and optional
 * source location information.
 *
 * Output format:
 * [YYYY-MM-DD HH:MM:SS.mmm] [LEVEL] [thread:TID] message [file:line in function()]
 *
 * @example Output:
 * [2025-11-03 14:30:15.123] [INFO] [thread:12345] Application started
 * [2025-11-03 14:30:16.456] [ERROR] [thread:12345] Connection failed [network.cpp:42 in connect()]
 */

namespace kcenon::logger {

/**
 * @class timestamp_formatter
 * @brief Default formatter with human-readable timestamp format
 *
 * @details Provides the traditional log format with timestamps, levels,
 * and optional source location. This is the default formatter used by
 * the logger system and is optimized for human readability.
 *
 * Features:
 * - Millisecond-precision timestamps
 * - Color-coded log levels (if enabled)
 * - Thread ID tracking
 * - Source location information (file, line, function)
 * - Automatic filename extraction from paths
 *
 * Thread-safety: This formatter is stateless and thread-safe.
 *
 * @since 1.2.0
 */
class timestamp_formatter : public log_formatter_interface {
public:
    /**
     * @brief Constructor with optional format options
     * @param opts Initial format options
     *
     * @since 1.2.0
     */
    explicit timestamp_formatter(const format_options& opts = format_options{}) {
        options_ = opts;
    }

    /**
     * @brief Format a log entry to human-readable string
     * @param entry The log entry to format
     * @return Formatted string with timestamp, level, message, and location
     *
     * @details Produces output in the format:
     * [YYYY-MM-DD HH:MM:SS.mmm] [LEVEL] [thread:TID] message [file:line in function()]
     *
     * @note Thread-safe. Can be called concurrently.
     *
     * @since 1.2.0
     */
    std::string format(const log_entry& entry) const override {
        std::ostringstream oss;

        // Timestamp
        if (options_.include_timestamp) {
            oss << "[" << format_timestamp(entry.timestamp) << "] ";
        }

        // Level (with color)
        if (options_.include_level) {
            if (options_.use_colors) {
                oss << level_to_color(entry.level);
            }
            oss << "[" << level_to_string(entry.level) << "] ";
            if (options_.use_colors) {
                oss << "\033[0m";  // Reset color
            }
        }

        // Thread ID
        if (options_.include_thread_id && entry.thread_id) {
            oss << "[thread:" << entry.thread_id->to_string() << "] ";
        }

        // Message
        oss << entry.message.to_string();

        // Source location
        if (options_.include_source_location && entry.location) {
            oss << " [";

            // Extract filename from path
            std::string file_path = entry.location->file.to_string();
            if (!file_path.empty()) {
                size_t pos = file_path.find_last_of("/\\");
                std::string filename = (pos != std::string::npos)
                    ? file_path.substr(pos + 1)
                    : file_path;
                oss << filename << ":" << entry.location->line;
            }

            // Function name
            std::string func = entry.location->function.to_string();
            if (!func.empty()) {
                oss << " in " << func << "()";
            }

            oss << "]";
        }

        return oss.str();
    }

    /**
     * @brief Get formatter name
     * @return "timestamp_formatter"
     *
     * @since 1.2.0
     */
    std::string get_name() const override {
        return "timestamp_formatter";
    }

private:
    /**
     * @brief Format timestamp to YYYY-MM-DD HH:MM:SS.mmm
     * @param tp Time point to format
     * @return Formatted timestamp string
     *
     * @note Uses thread-safe time conversion functions
     *
     * @since 1.2.0
     */
    static std::string format_timestamp(
        const std::chrono::system_clock::time_point& tp
    ) {
        auto time_t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_buf{};

#ifdef _WIN32
        localtime_s(&tm_buf, &time_t);  // Windows thread-safe version
#else
        localtime_r(&time_t, &tm_buf);  // POSIX thread-safe version
#endif

        // Format base timestamp
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_buf);

        // Add milliseconds
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()
        ) % 1000;

        std::ostringstream oss;
        oss << buffer << "."
            << std::setfill('0') << std::setw(3) << ms.count();

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

    /**
     * @brief Get ANSI color code for log level
     * @param level Log level to get color for
     * @return ANSI escape sequence for color
     *
     * @note Returns empty string if colors are disabled
     *
     * @since 1.2.0
     */
    std::string level_to_color(logger_system::log_level level) const {
        if (!options_.use_colors) {
            return "";
        }

        switch (level) {
            case logger_system::log_level::fatal:   return "\033[1;35m"; // Bright Magenta
            case logger_system::log_level::error:   return "\033[1;31m"; // Bright Red
            case logger_system::log_level::warn:    return "\033[1;33m"; // Bright Yellow
            case logger_system::log_level::info:    return "\033[1;32m"; // Bright Green
            case logger_system::log_level::debug:   return "\033[1;36m"; // Bright Cyan
            case logger_system::log_level::trace:   return "\033[1;37m"; // Bright White
            case logger_system::log_level::off:     return "";           // No color for off
        }
        return "";
    }
};

} // namespace kcenon::logger
