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
 * @file template_formatter.h
 * @brief Template-based customizable formatter for structured logging
 * @author 🍀☀🌕🌥 🌊
 * @since 3.1.0
 *
 * @details Formats log messages using user-defined templates with placeholders.
 * Placeholders are replaced with actual log entry values at runtime.
 *
 * Supported placeholders:
 * - {timestamp} - ISO 8601 formatted timestamp
 * - {timestamp_local} - Local time formatted timestamp
 * - {level} - Log level (e.g., "INFO", "ERROR")
 * - {level_lower} - Lowercase log level (e.g., "info", "error")
 * - {message} - Log message content
 * - {thread_id} - Thread identifier
 * - {file} - Source file path
 * - {filename} - Source file name only (without path)
 * - {line} - Source line number
 * - {function} - Function name
 * - {category} - Log category
 * - {trace_id} - OpenTelemetry trace ID
 * - {span_id} - OpenTelemetry span ID
 *
 * @example Usage:
 * @code
 * // Simple template
 * auto formatter = std::make_unique<template_formatter>(
 *     "[{timestamp}] [{level}] {message}"
 * );
 *
 * // Detailed template with source location
 * auto detailed = std::make_unique<template_formatter>(
 *     "{timestamp} | {level:8} | {thread_id} | {message} ({filename}:{line})"
 * );
 *
 * // Apache-style format
 * auto apache = std::make_unique<template_formatter>(
 *     "[{timestamp_local}] [{level_lower}] [{category}] {message}"
 * );
 * @endcode
 *
 * @see https://docs.python.org/3/library/logging.html#logrecord-attributes
 */

#include "../interfaces/log_entry.h"
#include "../interfaces/log_formatter_interface.h"
#include "../utils/time_utils.h"
#include "../utils/string_utils.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <type_traits>
#include <variant>

// Use common_system's standard interface
#include <kcenon/common/interfaces/logger_interface.h>

namespace kcenon::logger {

/**
 * @class template_formatter
 * @brief Customizable formatter using template strings with placeholders
 *
 * @details Provides flexible log formatting by allowing users to define
 * their own output format using a template string with placeholders.
 * This enables adaptation to various logging requirements without
 * creating new formatter classes.
 *
 * Features:
 * - User-defined template strings
 * - Multiple placeholder types for all log entry fields
 * - Optional color support for terminal output
 * - Field width formatting (e.g., {level:8} for 8-char width)
 * - Fallback values for missing fields
 *
 * Thread-safety: This formatter is thread-safe once constructed.
 *
 * @since 3.1.0
 */
class template_formatter : public log_formatter_interface {
public:
    /**
     * @brief Default template pattern
     */
    static constexpr const char* DEFAULT_TEMPLATE =
        "[{timestamp}] [{level}] [{thread_id}] {message}";

    /**
     * @brief Constructor with custom template
     * @param template_pattern Template string with placeholders
     * @param opts Initial format options
     *
     * @details Creates a formatter with the specified template pattern.
     * Placeholders are enclosed in curly braces: {placeholder_name}
     *
     * @example
     * @code
     * template_formatter fmt("{timestamp} [{level}] {message}");
     * @endcode
     *
     * @since 3.1.0
     */
    explicit template_formatter(
        const std::string& template_pattern = DEFAULT_TEMPLATE,
        const format_options& opts = format_options{}
    ) : template_(template_pattern) {
        options_ = opts;
        parse_template();
    }

    /**
     * @brief Format a log entry using the template
     * @param entry The log entry to format
     * @return Formatted string according to template
     *
     * @details Replaces all placeholders in the template with actual
     * values from the log entry. Missing optional fields are replaced
     * with empty strings or default values.
     *
     * @note Thread-safe. Can be called concurrently.
     *
     * @since 3.1.0
     */
    [[nodiscard]] std::string format(const log_entry& entry) const override {
        std::ostringstream oss;

        for (const auto& segment : segments_) {
            if (segment.is_placeholder) {
                oss << resolve_placeholder(segment.content, segment.width, entry);
            } else {
                oss << segment.content;
            }
        }

        return oss.str();
    }

    /**
     * @brief Get formatter name
     * @return "template_formatter"
     *
     * @since 3.1.0
     */
    [[nodiscard]] std::string get_name() const override {
        return "template_formatter";
    }

    /**
     * @brief Get current template pattern
     * @return The template string used for formatting
     *
     * @since 3.1.0
     */
    [[nodiscard]] const std::string& get_template() const {
        return template_;
    }

    /**
     * @brief Set a new template pattern
     * @param template_pattern New template string
     *
     * @note This method re-parses the template.
     *
     * @since 3.1.0
     */
    void set_template(const std::string& template_pattern) {
        template_ = template_pattern;
        parse_template();
    }

private:
    /**
     * @brief Segment of parsed template
     */
    struct template_segment {
        std::string content;       ///< Text content or placeholder name
        bool is_placeholder;       ///< True if this is a placeholder
        int width;                 ///< Optional field width (0 = no padding)

        template_segment(const std::string& c, bool p, int w = 0)
            : content(c), is_placeholder(p), width(w) {}
    };

    std::string template_;                      ///< Original template string
    std::vector<template_segment> segments_;    ///< Parsed template segments

    /**
     * @brief Parse template string into segments
     *
     * @details Breaks down the template into literal text segments
     * and placeholder segments for efficient formatting.
     */
    void parse_template() {
        segments_.clear();

        size_t pos = 0;
        size_t len = template_.length();

        while (pos < len) {
            size_t start = template_.find('{', pos);

            if (start == std::string::npos) {
                // No more placeholders, add remaining text
                if (pos < len) {
                    segments_.emplace_back(template_.substr(pos), false);
                }
                break;
            }

            // Add text before placeholder
            if (start > pos) {
                segments_.emplace_back(template_.substr(pos, start - pos), false);
            }

            // Find closing brace
            size_t end = template_.find('}', start);
            if (end == std::string::npos) {
                // Unclosed brace, treat as literal
                segments_.emplace_back(template_.substr(start), false);
                break;
            }

            // Extract placeholder name and optional width
            std::string placeholder = template_.substr(start + 1, end - start - 1);
            int width = 0;

            size_t colon_pos = placeholder.find(':');
            if (colon_pos != std::string::npos) {
                std::string width_str = placeholder.substr(colon_pos + 1);
                placeholder = placeholder.substr(0, colon_pos);
                try {
                    width = std::stoi(width_str);
                } catch (...) {
                    width = 0;
                }
            }

            segments_.emplace_back(placeholder, true, width);
            pos = end + 1;
        }
    }

    /**
     * @brief Resolve a placeholder to its actual value
     * @param name Placeholder name
     * @param width Optional field width
     * @param entry Log entry to extract value from
     * @return Resolved value string
     */
    [[nodiscard]] std::string resolve_placeholder(
        const std::string& name,
        int width,
        const log_entry& entry
    ) const {
        std::string value;

        if (name == "timestamp") {
            value = utils::time_utils::format_iso8601(entry.timestamp);
        } else if (name == "timestamp_local") {
            value = utils::time_utils::format_timestamp(entry.timestamp);
        } else if (name == "level") {
            std::string level_str = utils::string_utils::level_to_string(entry.level);
            if (options_.use_colors) {
                value = utils::string_utils::level_to_color(entry.level) +
                        level_str +
                        utils::string_utils::color_reset();
            } else {
                value = level_str;
            }
        } else if (name == "level_lower") {
            value = utils::string_utils::to_lower(
                utils::string_utils::level_to_string(entry.level)
            );
        } else if (name == "message") {
            value = entry.message.to_string();
        } else if (name == "thread_id") {
            if (entry.thread_id) {
                value = entry.thread_id->to_string();
            }
        } else if (name == "file") {
            if (entry.location) {
                value = entry.location->file.to_string();
            }
        } else if (name == "filename") {
            if (entry.location) {
                value = utils::string_utils::extract_filename(
                    entry.location->file.to_string()
                );
            }
        } else if (name == "line") {
            if (entry.location && entry.location->line > 0) {
                value = std::to_string(entry.location->line);
            }
        } else if (name == "function") {
            if (entry.location) {
                value = entry.location->function.to_string();
            }
        } else if (name == "category") {
            if (entry.category) {
                value = entry.category->to_string();
            }
        } else if (name == "trace_id") {
            if (entry.otel_ctx && !entry.otel_ctx->trace_id.empty()) {
                value = entry.otel_ctx->trace_id;
            }
        } else if (name == "span_id") {
            if (entry.otel_ctx && !entry.otel_ctx->span_id.empty()) {
                value = entry.otel_ctx->span_id;
            }
        } else {
            // Check structured fields
            if (entry.fields) {
                auto it = entry.fields->find(name);
                if (it != entry.fields->end()) {
                    value = format_field_value(it->second);
                }
            }
        }

        // Apply width formatting if specified
        if (width > 0 && !value.empty()) {
            // Calculate actual display width (excluding ANSI codes)
            size_t display_len = calculate_display_width(value);
            if (display_len < static_cast<size_t>(width)) {
                value += std::string(width - display_len, ' ');
            }
        }

        return value;
    }

    /**
     * @brief Format a structured field value to string
     * @param value The log_value to format
     * @return String representation
     */
    [[nodiscard]] static std::string format_field_value(const log_value& value) {
        return std::visit([](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return v;
            } else if constexpr (std::is_same_v<T, bool>) {
                return v ? "true" : "false";
            } else if constexpr (std::is_same_v<T, int64_t>) {
                return std::to_string(v);
            } else if constexpr (std::is_same_v<T, double>) {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(6) << v;
                return oss.str();
            } else {
                return std::to_string(v);
            }
        }, value);
    }

    /**
     * @brief Calculate display width excluding ANSI escape codes
     * @param str String that may contain ANSI codes
     * @return Display width in characters
     */
    [[nodiscard]] static size_t calculate_display_width(const std::string& str) {
        size_t width = 0;
        bool in_escape = false;

        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '\033') {
                in_escape = true;
            } else if (in_escape) {
                if (str[i] == 'm') {
                    in_escape = false;
                }
            } else {
                ++width;
            }
        }

        return width;
    }
};

} // namespace kcenon::logger
