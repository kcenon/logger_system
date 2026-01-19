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
 * @file structured_log_builder.h
 * @brief Fluent builder for structured log entries
 * @author 🍀☀🌕🌥 🌊
 * @since 3.1.0
 *
 * @details Provides a fluent interface for building structured log entries
 * with arbitrary key-value fields. Integrates with the main logger class.
 *
 * @example
 * @code
 * logger->info_structured()
 *     .message("User login")
 *     .field("user_id", 12345)
 *     .field("ip_address", "192.168.1.1")
 *     .field("session_id", "abc-123")
 *     .emit();
 * @endcode
 */

#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/common/interfaces/logger_interface.h>

#include <string>
#include <functional>

namespace kcenon::logger {

// Type alias for log_level
using log_level = common::interfaces::log_level;

// Forward declaration
class logger;

/**
 * @class structured_log_builder
 * @brief Fluent builder for creating structured log entries
 *
 * @details Provides a builder pattern for constructing log entries with
 * arbitrary structured fields. The builder collects message and field
 * information, then emits the log entry when emit() is called.
 *
 * Thread-safety: Each builder instance should be used by a single thread.
 * The emit() operation is thread-safe.
 *
 * @note The builder holds a reference to the logger. Do not use the builder
 * after the logger has been destroyed.
 *
 * @since 3.1.0
 */
class structured_log_builder {
public:
    /**
     * @brief Callback type for emitting structured log entries
     */
    using emit_callback = std::function<void(log_entry&&)>;

    /**
     * @brief Constructor
     * @param level Log level for the entry
     * @param callback Callback to invoke when emit() is called
     * @param context_fields Context fields to include automatically
     */
    structured_log_builder(log_level level,
                           emit_callback callback,
                           const log_fields* context_fields = nullptr)
        : level_(level),
          callback_(std::move(callback)) {
        if (context_fields && !context_fields->empty()) {
            fields_ = *context_fields;
        }
    }

    /**
     * @brief Set the log message
     * @param msg The message to log
     * @return Reference to this builder for chaining
     */
    structured_log_builder& message(const std::string& msg) {
        message_ = msg;
        return *this;
    }

    /**
     * @brief Add a string field
     * @param key Field name
     * @param value Field value
     * @return Reference to this builder for chaining
     */
    structured_log_builder& field(const std::string& key, const std::string& value) {
        fields_[key] = value;
        return *this;
    }

    /**
     * @brief Add a C-string field
     * @param key Field name
     * @param value Field value
     * @return Reference to this builder for chaining
     */
    structured_log_builder& field(const std::string& key, const char* value) {
        fields_[key] = std::string(value);
        return *this;
    }

    /**
     * @brief Add an integer field
     * @param key Field name
     * @param value Field value
     * @return Reference to this builder for chaining
     */
    structured_log_builder& field(const std::string& key, int value) {
        fields_[key] = static_cast<int64_t>(value);
        return *this;
    }

    /**
     * @brief Add a long integer field
     * @param key Field name
     * @param value Field value
     * @return Reference to this builder for chaining
     */
    structured_log_builder& field(const std::string& key, int64_t value) {
        fields_[key] = value;
        return *this;
    }

    /**
     * @brief Add a double field
     * @param key Field name
     * @param value Field value
     * @return Reference to this builder for chaining
     */
    structured_log_builder& field(const std::string& key, double value) {
        fields_[key] = value;
        return *this;
    }

    /**
     * @brief Add a boolean field
     * @param key Field name
     * @param value Field value
     * @return Reference to this builder for chaining
     */
    structured_log_builder& field(const std::string& key, bool value) {
        fields_[key] = value;
        return *this;
    }

    /**
     * @brief Add a log_value field
     * @param key Field name
     * @param value Field value
     * @return Reference to this builder for chaining
     */
    structured_log_builder& field(const std::string& key, const log_value& value) {
        fields_[key] = value;
        return *this;
    }

    /**
     * @brief Set the category for the log entry
     * @param cat Category string
     * @return Reference to this builder for chaining
     */
    structured_log_builder& category(const std::string& cat) {
        category_ = cat;
        return *this;
    }

    /**
     * @brief Emit the log entry
     *
     * @details Constructs the log_entry with all accumulated fields and
     * invokes the callback to send it to the logger. After calling emit(),
     * the builder should not be reused.
     */
    void emit() {
        if (!callback_) {
            return;
        }

        log_entry entry(level_, message_);

        if (!fields_.empty()) {
            entry.fields = std::move(fields_);
        }

        if (!category_.empty()) {
            entry.category = small_string_128(category_);
        }

        callback_(std::move(entry));
    }

    /**
     * @brief Destructor - does NOT auto-emit
     *
     * @details If emit() was not called, the log entry is discarded.
     * This is intentional to avoid unexpected behavior.
     */
    ~structured_log_builder() = default;

    // Disable copy
    structured_log_builder(const structured_log_builder&) = delete;
    structured_log_builder& operator=(const structured_log_builder&) = delete;

    // Enable move
    structured_log_builder(structured_log_builder&&) noexcept = default;
    structured_log_builder& operator=(structured_log_builder&&) noexcept = default;

private:
    log_level level_;
    emit_callback callback_;
    std::string message_;
    std::string category_;
    log_fields fields_;
};

} // namespace kcenon::logger
