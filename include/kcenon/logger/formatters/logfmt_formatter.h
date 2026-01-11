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
 * @file logfmt_formatter.h
 * @brief Logfmt formatter for structured logging
 * @author 🍀☀🌕🌥 🌊
 * @since 3.1.0
 *
 * @details Formats log messages in logfmt format (key=value pairs) for
 * integration with log aggregation systems that support this format
 * (Prometheus, Grafana Loki, Heroku, etc.).
 *
 * Output format:
 * level=info ts=2025-01-11T10:30:15.123Z msg="Application started" thread_id=12345
 *
 * With structured fields:
 * level=error ts=2025-01-11T10:30:15.123Z msg="Connection failed" user_id=123 retry_count=3
 *
 * @see https://brandur.org/logfmt
 *
 * @example Usage:
 * @code
 * auto formatter = std::make_unique<logfmt_formatter>();
 * auto writer = std::make_unique<file_writer>("app.log", std::move(formatter));
 * @endcode
 */

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

namespace kcenon::logger {

/**
 * @class logfmt_formatter
 * @brief Formatter that outputs logfmt-structured log messages
 *
 * @details Produces machine-readable logfmt output suitable for log aggregation,
 * analysis tools, and automated processing. The format consists of key=value
 * pairs separated by spaces.
 *
 * Features:
 * - ISO 8601 timestamp format (RFC 3339)
 * - Proper escaping of special characters
 * - Structured source location information
 * - OpenTelemetry context support (trace_id, span_id)
 * - Compatible with logfmt parsers and log aggregation systems
 *
 * Thread-safety: This formatter is stateless and thread-safe.
 *
 * @since 3.1.0
 */
class logfmt_formatter : public log_formatter_interface {
public:
    /**
     * @brief Constructor with optional format options
     * @param opts Initial format options
     *
     * @note For logfmt output, use_colors and pretty_print options are ignored
     *
     * @since 3.1.0
     */
    explicit logfmt_formatter(const format_options& opts = format_options{}) {
        options_ = opts;
        // Logfmt doesn't support colors or pretty print
        options_.use_colors = false;
        options_.pretty_print = false;
    }

    /**
     * @brief Format a log entry to logfmt string
     * @param entry The log entry to format
     * @return Logfmt-formatted string
     *
     * @details Produces key=value pairs with fields:
     * - level: Log level as lowercase string
     * - ts: ISO 8601 format timestamp (UTC)
     * - msg: Log message (escaped)
     * - thread_id: Thread ID
     * - file, line, function: Source location (if enabled)
     * - trace_id, span_id: OpenTelemetry context (if present)
     * - Additional structured fields
     *
     * @note Thread-safe. Can be called concurrently.
     *
     * @since 3.1.0
     */
    [[nodiscard]] std::string format(const log_entry& entry) const override {
        std::ostringstream oss;
        bool first = true;

        // Level
        if (options_.include_level) {
            oss << "level=" << level_to_lowercase(entry.level);
            first = false;
        }

        // Timestamp (ISO 8601)
        if (options_.include_timestamp) {
            if (!first) {
                oss << " ";
            }
            oss << "ts=" << utils::time_utils::format_iso8601(entry.timestamp);
            first = false;
        }

        // Message (always include)
        if (!first) {
            oss << " ";
        }
        oss << "msg=" << escape_logfmt_value(entry.message.to_string());
        first = false;

        // Thread ID
        if (options_.include_thread_id && entry.thread_id) {
            oss << " thread_id=" << escape_logfmt_value(entry.thread_id->to_string());
        }

        // Source location
        if (options_.include_source_location && entry.location) {
            std::string file_path = entry.location->file.to_string();
            if (!file_path.empty()) {
                oss << " file=" << escape_logfmt_value(file_path);
            }

            if (entry.location->line > 0) {
                oss << " line=" << entry.location->line;
            }

            std::string func = entry.location->function.to_string();
            if (!func.empty()) {
                oss << " function=" << escape_logfmt_value(func);
            }
        }

        // Category (if present)
        if (entry.category) {
            std::string cat = entry.category->to_string();
            if (!cat.empty()) {
                oss << " category=" << escape_logfmt_value(cat);
            }
        }

        // OpenTelemetry context (if present)
        if (entry.otel_ctx && entry.otel_ctx->is_valid()) {
            if (!entry.otel_ctx->trace_id.empty()) {
                oss << " trace_id=" << entry.otel_ctx->trace_id;
            }
            if (!entry.otel_ctx->span_id.empty()) {
                oss << " span_id=" << entry.otel_ctx->span_id;
            }
            if (!entry.otel_ctx->trace_flags.empty()) {
                oss << " trace_flags=" << entry.otel_ctx->trace_flags;
            }
        }

        // Structured fields (if present)
        if (entry.fields && !entry.fields->empty()) {
            for (const auto& [key, value] : *entry.fields) {
                oss << " " << escape_logfmt_key(key) << "=";
                format_value(oss, value);
            }
        }

        return oss.str();
    }

    /**
     * @brief Get formatter name
     * @return "logfmt_formatter"
     *
     * @since 3.1.0
     */
    [[nodiscard]] std::string get_name() const override {
        return "logfmt_formatter";
    }

private:
    /**
     * @brief Convert log level to lowercase string
     * @param level Log level
     * @return Lowercase string representation
     */
    static std::string level_to_lowercase(logger_system::log_level level) {
        switch (level) {
            case logger_system::log_level::trace: return "trace";
            case logger_system::log_level::debug: return "debug";
            case logger_system::log_level::info: return "info";
            case logger_system::log_level::warn: return "warn";
            case logger_system::log_level::error: return "error";
            case logger_system::log_level::fatal: return "fatal";
            case logger_system::log_level::off: return "off";
            default: return "unknown";
        }
    }

    /**
     * @brief Escape a logfmt key (remove spaces and special characters)
     * @param key Key to escape
     * @return Escaped key
     */
    static std::string escape_logfmt_key(const std::string& key) {
        std::string result;
        result.reserve(key.size());
        for (char c : key) {
            if (c == ' ' || c == '=' || c == '"' || c == '\n' || c == '\t') {
                result += '_';
            } else {
                result += c;
            }
        }
        return result;
    }

    /**
     * @brief Escape a logfmt value
     * @param value Value to escape
     * @return Escaped value (quoted if necessary)
     *
     * @details Values containing spaces, quotes, or special characters
     * are wrapped in double quotes with proper escaping.
     */
    static std::string escape_logfmt_value(const std::string& value) {
        // Check if quoting is needed
        bool needs_quoting = false;
        for (char c : value) {
            if (c == ' ' || c == '"' || c == '=' || c == '\n' || c == '\t' || c == '\r') {
                needs_quoting = true;
                break;
            }
        }

        if (!needs_quoting && !value.empty()) {
            return value;
        }

        // Quote and escape
        std::ostringstream oss;
        oss << '"';
        for (char c : value) {
            switch (c) {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;
                default: oss << c;
            }
        }
        oss << '"';
        return oss.str();
    }

    /**
     * @brief Format a log_value to logfmt
     * @param oss Output stream
     * @param value Value to format
     */
    static void format_value(std::ostringstream& oss, const log_value& value) {
        std::visit([&oss](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::string>) {
                oss << escape_logfmt_value(v);
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
};

} // namespace kcenon::logger
