// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#pragma once

#include "../interfaces/log_formatter_interface.h"
#include "../utils/time_utils.h"
#include "../utils/string_utils.h"
#include <sstream>
#include <iomanip>
#include <ctime>

// Use common_system's standard interface
#include <kcenon/common/interfaces/logger_interface.h>

/**
 * @file timestamp_formatter.h
 * @brief Default human-readable formatter with timestamps
 *  kcenon
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
            oss << "[" << utils::time_utils::format_timestamp(entry.timestamp) << "] ";
        }

        // Level (with color)
        if (options_.include_level) {
            if (options_.use_colors) {
                oss << utils::string_utils::level_to_color(entry.level, true);
            }
            oss << "[" << utils::string_utils::level_to_string(entry.level) << "] ";
            if (options_.use_colors) {
                oss << utils::string_utils::color_reset();
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
                std::string filename = utils::string_utils::extract_filename(file_path);
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
    // Note: Formatting functions moved to utils::time_utils and utils::string_utils (Phase 3.4)
    // This reduces code duplication and improves maintainability.
};

} // namespace kcenon::logger
