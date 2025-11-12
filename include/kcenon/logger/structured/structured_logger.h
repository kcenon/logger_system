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

#include <kcenon/logger/interfaces/logger_types.h>
#include <string>
#include <unordered_map>
#include <variant>
#include <memory>

namespace kcenon::logger::structured {

/**
 * @brief Value type for structured logging
 */
using log_value = std::variant<std::string, int, double, bool>;

/**
 * @brief Structured log entry
 */
struct structured_log_entry {
    logger_system::log_level level;
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
    virtual class log_builder start_log(logger_system::log_level level) = 0;
};

/**
 * @brief Builder for structured log entries
 */
class log_builder {
private:
    structured_log_entry entry_;
    structured_logger_interface* logger_;

public:
    log_builder(logger_system::log_level level, structured_logger_interface* logger)
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
 * @brief Basic structured logger implementation
 */
class basic_structured_logger : public structured_logger_interface {
public:
    void log_structured(const structured_log_entry& entry) override {
        // Implementation would format and output the structured log
        // For now, this is a placeholder
        (void)entry;
    }

    log_builder start_log(logger_system::log_level level) override {
        return log_builder(level, this);
    }
};

/**
 * @brief JSON formatter for structured logs
 */
class json_formatter {
public:
    static std::string format(const structured_log_entry& entry) {
        // Simple JSON formatting - in real implementation would use a proper JSON library
        std::string json = "{";
        json += "\"level\":\"" + level_to_string(entry.level) + "\",";
        json += "\"message\":\"" + entry.message + "\",";
        json += "\"timestamp\":\"" + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
            entry.timestamp.time_since_epoch()).count()) + "\",";
        json += "\"fields\":{";

        bool first = true;
        for (const auto& [key, value] : entry.fields) {
            if (!first) json += ",";
            json += "\"" + key + "\":";

            std::visit([&json](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    json += "\"" + v + "\"";
                } else if constexpr (std::is_same_v<T, bool>) {
                    json += v ? "true" : "false";
                } else {
                    json += std::to_string(v);
                }
            }, value);

            first = false;
        }

        json += "}}";
        return json;
    }

private:
    static std::string level_to_string(logger_system::log_level level) {
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
};

} // namespace kcenon::logger::structured