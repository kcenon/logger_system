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

#include "../interfaces/log_entry.h"
#include "../interfaces/log_formatter_interface.h"
#include "../utils/time_utils.h"
#include "../utils/string_utils.h"
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <variant>

// Use common_system's standard interface
#include <kcenon/common/interfaces/logger_interface.h>

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
            oss << indent << "\"timestamp\":\"" << utils::time_utils::format_iso8601(entry.timestamp) << "\"";
            first = false;
        }

        // Level
        if (options_.include_level) {
            if (!first) oss << "," << newline;
            oss << indent << "\"level\":\"" << utils::string_utils::level_to_string(entry.level) << "\"";
            first = false;
        }

        // Thread ID
        if (options_.include_thread_id && entry.thread_id) {
            if (!first) oss << "," << newline;
            oss << indent << "\"thread_id\":\"" << utils::string_utils::escape_json(entry.thread_id->to_string()) << "\"";
            first = false;
        }

        // Message (always include)
        if (!first) oss << "," << newline;
        oss << indent << "\"message\":\"" << utils::string_utils::escape_json(entry.message.to_string()) << "\"";
        first = false;

        // Source location
        if (options_.include_source_location && entry.location) {
            std::string file_path = entry.location->file.to_string();
            if (!file_path.empty()) {
                oss << "," << newline << indent << "\"file\":\"" << utils::string_utils::escape_json(file_path) << "\"";
            }

            if (entry.location->line > 0) {
                oss << "," << newline << indent << "\"line\":" << entry.location->line;
            }

            std::string func = entry.location->function.to_string();
            if (!func.empty()) {
                oss << "," << newline << indent << "\"function\":\"" << utils::string_utils::escape_json(func) << "\"";
            }
        }

        // Category (if present)
        if (entry.category) {
            std::string cat = entry.category->to_string();
            if (!cat.empty()) {
                oss << "," << newline << indent << "\"category\":\"" << utils::string_utils::escape_json(cat) << "\"";
            }
        }

        // OpenTelemetry context (if present)
        if (entry.otel_ctx && entry.otel_ctx->is_valid()) {
            if (!entry.otel_ctx->trace_id.empty()) {
                oss << "," << newline << indent << "\"trace_id\":\"" << utils::string_utils::escape_json(entry.otel_ctx->trace_id) << "\"";
            }
            if (!entry.otel_ctx->span_id.empty()) {
                oss << "," << newline << indent << "\"span_id\":\"" << utils::string_utils::escape_json(entry.otel_ctx->span_id) << "\"";
            }
            if (!entry.otel_ctx->trace_flags.empty()) {
                oss << "," << newline << indent << "\"trace_flags\":\"" << utils::string_utils::escape_json(entry.otel_ctx->trace_flags) << "\"";
            }
        }

        // Structured fields (if present)
        if (entry.fields && !entry.fields->empty()) {
            for (const auto& [key, value] : *entry.fields) {
                oss << "," << newline << indent << "\"" << utils::string_utils::escape_json(key) << "\":";
                format_value(oss, value);
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
     * @brief Format a log_value to JSON
     * @param oss Output stream
     * @param value Value to format
     */
    static void format_value(std::ostringstream& oss, const log_value& value) {
        std::visit([&oss](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::string>) {
                oss << "\"" << utils::string_utils::escape_json(v) << "\"";
            } else if constexpr (std::is_same_v<T, bool>) {
                oss << (v ? "true" : "false");
            } else if constexpr (std::is_same_v<T, int64_t>) {
                oss << v;
            } else if constexpr (std::is_same_v<T, double>) {
                oss << std::fixed << std::setprecision(6) << v;
            } else {
                oss << v;
            }
        }, value);
    }

    // Note: Formatting functions moved to utils::time_utils and utils::string_utils (Phase 3.4)
    // This reduces code duplication and improves maintainability.
};

} // namespace kcenon::logger
