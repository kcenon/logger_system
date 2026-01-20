// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file structured_logger.h
 * @brief Structured logging functionality
 */

#pragma once

#include <kcenon/common/interfaces/logger_interface.h>
#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include <functional>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

namespace kcenon::logger::structured {

// Type alias for log_level
using log_level = common::interfaces::log_level;

/**
 * @brief Value type for structured logging
 */
using log_value = std::variant<std::string, int, double, bool>;

/**
 * @brief Structured log entry
 */
struct structured_log_entry {
    log_level level;
    std::string message;
    std::unordered_map<std::string, log_value> fields;
    std::chrono::system_clock::time_point timestamp;

    structured_log_entry() : timestamp(std::chrono::system_clock::now()) {}
};

/**
 * @brief Structured logger interface
 */
class structured_logger_interface {
public:
    virtual ~structured_logger_interface() = default;

    /**
     * @brief Log a structured message
     */
    virtual void log_structured(const structured_log_entry& entry) = 0;

    /**
     * @brief Start building a structured log entry
     */
    virtual class log_builder start_log(log_level level) = 0;
};

/**
 * @brief Builder for structured log entries
 */
class log_builder {
private:
    structured_log_entry entry_;
    structured_logger_interface* logger_;

public:
    log_builder(log_level level, structured_logger_interface* logger)
        : logger_(logger) {
        entry_.level = level;
    }

    log_builder& message(const std::string& msg) {
        entry_.message = msg;
        return *this;
    }

    log_builder& field(const std::string& key, const log_value& value) {
        entry_.fields[key] = value;
        return *this;
    }

    log_builder& field(const std::string& key, const std::string& value) {
        entry_.fields[key] = value;
        return *this;
    }

    log_builder& field(const std::string& key, int value) {
        entry_.fields[key] = value;
        return *this;
    }

    log_builder& field(const std::string& key, double value) {
        entry_.fields[key] = value;
        return *this;
    }

    log_builder& field(const std::string& key, bool value) {
        entry_.fields[key] = value;
        return *this;
    }

    void log() {
        if (logger_) {
            logger_->log_structured(entry_);
        }
    }
};

/**
 * @brief Output format for structured logs
 */
enum class structured_format {
    json,       // JSON format
    logfmt      // key=value format (Heroku/logfmt style)
};

/**
 * @brief Output callback type for structured logger
 */
using structured_output_callback = std::function<void(log_level, const std::string&)>;

/**
 * @brief Basic structured logger implementation
 *
 * @details Supports multiple output formats (JSON, logfmt) and can output
 * to stdout/stderr or a custom callback function.
 *
 * @example
 * @code
 * basic_structured_logger logger;
 * logger.set_format(structured_format::json);
 *
 * logger.start_log(log_level::info)
 *     .message("User logged in")
 *     .field("user_id", "12345")
 *     .field("ip_address", "192.168.1.1")
 *     .log();
 * @endcode
 */
class basic_structured_logger : public structured_logger_interface {
private:
    structured_format format_ = structured_format::json;
    structured_output_callback output_callback_;
    bool output_to_stderr_ = false;

public:
    /**
     * @brief Set the output format
     * @param format The format to use (json or logfmt)
     */
    void set_format(structured_format format) {
        format_ = format;
    }

    /**
     * @brief Set a custom output callback
     * @param callback Function to call with formatted log output
     */
    void set_output_callback(structured_output_callback callback) {
        output_callback_ = std::move(callback);
    }

    /**
     * @brief Set whether to output to stderr (default: stdout)
     * @param use_stderr If true, output to stderr; otherwise stdout
     */
    void set_output_to_stderr(bool use_stderr) {
        output_to_stderr_ = use_stderr;
    }

    void log_structured(const structured_log_entry& entry) override {
        std::string formatted;

        switch (format_) {
            case structured_format::json:
                formatted = json_formatter::format(entry);
                break;
            case structured_format::logfmt:
                formatted = format_logfmt(entry);
                break;
        }

        // Output the formatted log
        if (output_callback_) {
            output_callback_(entry.level, formatted);
        } else {
            auto& out = output_to_stderr_ ? std::cerr : std::cout;
            out << formatted << std::endl;
        }
    }

    log_builder start_log(log_level level) override {
        return log_builder(level, this);
    }

private:
    /**
     * @brief Format entry in logfmt style (key=value pairs)
     */
    static std::string format_logfmt(const structured_log_entry& entry) {
        std::ostringstream oss;

        // Standard fields first
        oss << "level=" << level_to_string(entry.level);
        oss << " ts=" << format_timestamp_logfmt(entry.timestamp);

        if (!entry.message.empty()) {
            oss << " msg=" << escape_logfmt_value(entry.message);
        }

        // Custom fields
        for (const auto& [key, value] : entry.fields) {
            oss << " " << key << "=";
            std::visit([&oss](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    oss << escape_logfmt_value(v);
                } else if constexpr (std::is_same_v<T, bool>) {
                    oss << (v ? "true" : "false");
                } else {
                    oss << v;
                }
            }, value);
        }

        return oss.str();
    }

    static std::string level_to_string(log_level level) {
        switch (level) {
            case log_level::trace: return "trace";
            case log_level::debug: return "debug";
            case log_level::info: return "info";
            case log_level::warning: return "warn";
            case log_level::error: return "error";
            case log_level::critical: return "fatal";
            case log_level::off: return "off";
            default: return "unknown";
        }
    }

    static std::string format_timestamp_logfmt(std::chrono::system_clock::time_point tp) {
        auto time_t_val = std::chrono::system_clock::to_time_t(tp);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::gmtime(&time_t_val), "%Y-%m-%dT%H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count() << "Z";
        return oss.str();
    }

    static std::string escape_logfmt_value(const std::string& value) {
        // If value contains spaces, quotes, or special chars, quote it
        bool needs_quoting = false;
        for (char c : value) {
            if (c == ' ' || c == '"' || c == '=' || c == '\n' || c == '\t') {
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
                case '\t': oss << "\\t"; break;
                default: oss << c;
            }
        }
        oss << '"';
        return oss.str();
    }
};

/**
 * @brief JSON formatter for structured logs
 *
 * @details Produces valid JSON output with proper escaping and ISO 8601 timestamps.
 */
class json_formatter {
public:
    static std::string format(const structured_log_entry& entry) {
        std::ostringstream json;
        json << "{";
        json << "\"timestamp\":\"" << format_timestamp_iso8601(entry.timestamp) << "\",";
        json << "\"level\":\"" << level_to_string(entry.level) << "\",";
        json << "\"message\":" << escape_json_string(entry.message);

        // Add custom fields
        for (const auto& [key, value] : entry.fields) {
            json << ",\"" << escape_json_key(key) << "\":";

            std::visit([&json](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    json << escape_json_string(v);
                } else if constexpr (std::is_same_v<T, bool>) {
                    json << (v ? "true" : "false");
                } else if constexpr (std::is_same_v<T, int>) {
                    json << v;
                } else if constexpr (std::is_same_v<T, double>) {
                    json << std::fixed << std::setprecision(6) << v;
                } else {
                    json << v;
                }
            }, value);
        }

        json << "}";
        return json.str();
    }

private:
    static std::string level_to_string(log_level level) {
        switch (level) {
            case log_level::trace: return "TRACE";
            case log_level::debug: return "DEBUG";
            case log_level::info: return "INFO";
            case log_level::warning: return "WARN";
            case log_level::error: return "ERROR";
            case log_level::critical: return "FATAL";
            case log_level::off: return "OFF";
            default: return "UNKNOWN";
        }
    }

    static std::string format_timestamp_iso8601(std::chrono::system_clock::time_point tp) {
        auto time_t_val = std::chrono::system_clock::to_time_t(tp);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::gmtime(&time_t_val), "%Y-%m-%dT%H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count() << "Z";
        return oss.str();
    }

    static std::string escape_json_string(const std::string& s) {
        std::ostringstream oss;
        oss << '"';
        for (char c : s) {
            switch (c) {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        oss << "\\u" << std::hex << std::setfill('0') << std::setw(4) << static_cast<int>(c);
                    } else {
                        oss << c;
                    }
            }
        }
        oss << '"';
        return oss.str();
    }

    static std::string escape_json_key(const std::string& key) {
        // Keys should be simple identifiers, but escape just in case
        std::string result;
        for (char c : key) {
            if (c == '"' || c == '\\') {
                result += '\\';
            }
            result += c;
        }
        return result;
    }
};

} // namespace kcenon::logger::structured