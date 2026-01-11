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
 * @file log_context_scope.h
 * @brief RAII-based context scope management for structured logging
 * @author 🍀☀🌕🌥 🌊
 * @since 3.2.0
 *
 * @details This file provides thread-local context storage and RAII scope
 * guards for structured logging context fields. Context fields set within
 * a scope are automatically included in all structured log entries and
 * are automatically cleaned up when the scope exits.
 *
 * @example
 * @code
 * // Set thread-local context for a request
 * void handle_request(const Request& req) {
 *     log_context_scope scope(logger, {
 *         {"request_id", req.id()},
 *         {"user_id", req.user_id()}
 *     });
 *
 *     // All structured logs in this scope include request_id and user_id
 *     logger->info_structured()
 *         .message("Processing request")
 *         .emit();
 * } // Context automatically cleared here
 * @endcode
 */

#include <kcenon/logger/interfaces/log_entry.h>

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

namespace kcenon::logger {

// Forward declaration
class logger;

/**
 * @class log_context_storage
 * @brief Thread-local storage for structured logging context fields
 *
 * @details Provides thread-safe storage and retrieval of context fields
 * using thread-local storage. This allows automatic context propagation
 * within a thread without affecting other threads.
 *
 * Thread-local context takes precedence over logger-level context when
 * both are set. Fields are merged with thread-local fields overriding
 * logger-level fields with the same key.
 *
 * @example
 * @code
 * // Set context for current thread
 * log_context_storage::set("request_id", "req-123");
 * log_context_storage::set("trace_id", "trace-456");
 *
 * // All logs on this thread now include these fields
 * logger->info_structured().message("Processing").emit();
 *
 * // Clear when done
 * log_context_storage::clear();
 * @endcode
 *
 * @since 3.2.0
 */
class log_context_storage {
public:
    /**
     * @brief Set a string context field for the current thread
     * @param key Field name
     * @param value Field value
     */
    static void set(const std::string& key, const std::string& value) {
        get_storage()[key] = value;
        get_has_context() = true;
    }

    /**
     * @brief Set a C-string context field for the current thread
     * @param key Field name
     * @param value Field value (will be converted to std::string)
     */
    static void set(const std::string& key, const char* value) {
        get_storage()[key] = std::string(value);
        get_has_context() = true;
    }

    /**
     * @brief Set an integer context field for the current thread
     * @param key Field name
     * @param value Field value
     */
    static void set(const std::string& key, int64_t value) {
        get_storage()[key] = value;
        get_has_context() = true;
    }

    /**
     * @brief Set a double context field for the current thread
     * @param key Field name
     * @param value Field value
     */
    static void set(const std::string& key, double value) {
        get_storage()[key] = value;
        get_has_context() = true;
    }

    /**
     * @brief Set a boolean context field for the current thread
     * @param key Field name
     * @param value Field value
     */
    static void set(const std::string& key, bool value) {
        get_storage()[key] = value;
        get_has_context() = true;
    }

    /**
     * @brief Set a log_value context field for the current thread
     * @param key Field name
     * @param value Field value
     */
    static void set(const std::string& key, const log_value& value) {
        get_storage()[key] = value;
        get_has_context() = true;
    }

    /**
     * @brief Set multiple context fields at once
     * @param fields Fields to set
     */
    static void set_all(const log_fields& fields) {
        for (const auto& [key, value] : fields) {
            get_storage()[key] = value;
        }
        if (!fields.empty()) {
            get_has_context() = true;
        }
    }

    /**
     * @brief Remove a context field for the current thread
     * @param key Field name to remove
     */
    static void remove(const std::string& key) {
        get_storage().erase(key);
        if (get_storage().empty()) {
            get_has_context() = false;
        }
    }

    /**
     * @brief Get all context fields for the current thread
     * @return Copy of context fields, empty if not set
     */
    [[nodiscard]] static log_fields get() {
        if (get_has_context()) {
            return get_storage();
        }
        return {};
    }

    /**
     * @brief Clear all context fields for the current thread
     */
    static void clear() {
        get_storage().clear();
        get_has_context() = false;
    }

    /**
     * @brief Check if any context fields are set for the current thread
     * @return true if context is set
     */
    [[nodiscard]] static bool has_context() {
        return get_has_context();
    }

    /**
     * @brief Get a specific field value if it exists
     * @param key Field name
     * @return Optional field value
     */
    [[nodiscard]] static std::optional<log_value> get_field(const std::string& key) {
        if (!get_has_context()) {
            return std::nullopt;
        }
        auto& storage = get_storage();
        auto it = storage.find(key);
        if (it != storage.end()) {
            return it->second;
        }
        return std::nullopt;
    }

private:
    static log_fields& get_storage() {
        thread_local log_fields storage;
        return storage;
    }

    static bool& get_has_context() {
        thread_local bool has_ctx = false;
        return has_ctx;
    }
};

/**
 * @class log_context_scope
 * @brief RAII guard for structured logging context
 *
 * @details Automatically sets context fields on construction and restores
 * the previous context on destruction. Supports nested scopes where inner
 * scopes can add or override fields from outer scopes.
 *
 * @example
 * @code
 * void handle_request(const Request& req) {
 *     // Set request-level context
 *     log_context_scope request_scope({
 *         {"request_id", req.id()},
 *         {"method", req.method()}
 *     });
 *
 *     // Nested scope for specific operation
 *     {
 *         log_context_scope operation_scope({
 *             {"operation", "database_query"}
 *         });
 *
 *         // Logs here include request_id, method, and operation
 *         logger->info_structured().message("Executing query").emit();
 *     } // operation scope ends, "operation" removed
 *
 *     // Logs here include only request_id and method
 *     logger->info_structured().message("Request completed").emit();
 * } // request scope ends, all context cleared
 * @endcode
 *
 * @since 3.2.0
 */
class log_context_scope {
public:
    /**
     * @brief Construct scope with initial fields
     * @param fields Context fields to set for this scope
     *
     * @details Saves the current thread-local context and sets the new fields.
     * Fields are merged with existing context (new fields override existing
     * ones with the same key).
     */
    explicit log_context_scope(const log_fields& fields)
        : previous_context_(log_context_storage::get()),
          had_previous_(log_context_storage::has_context()) {
        for (const auto& [key, value] : fields) {
            // Track which keys we're adding (to remove them on destruction)
            if (previous_context_.find(key) == previous_context_.end()) {
                added_keys_.push_back(key);
            }
            log_context_storage::set(key, value);
        }
    }

    /**
     * @brief Construct scope with a logger reference (for logger-level context)
     * @param log Logger to use for context
     * @param fields Context fields to set for this scope
     *
     * @details Sets context on both the logger (for non-thread-local use)
     * and thread-local storage.
     */
    log_context_scope(logger& log, const log_fields& fields);

    /**
     * @brief Destructor - restores previous context
     *
     * @details Removes fields that were added by this scope and restores
     * fields that were overridden.
     */
    ~log_context_scope() {
        // Remove keys that were added by this scope
        for (const auto& key : added_keys_) {
            log_context_storage::remove(key);
        }

        // Restore overridden values from previous context
        if (had_previous_) {
            for (const auto& [key, value] : previous_context_) {
                // Only restore if the key was overridden (not newly added)
                if (std::find(added_keys_.begin(), added_keys_.end(), key) == added_keys_.end()) {
                    log_context_storage::set(key, value);
                }
            }
        }

        // Clear logger context if we set it
        if (logger_ != nullptr) {
            for (const auto& key : added_keys_) {
                remove_logger_context(key);
            }
        }
    }

    // Non-copyable, non-movable
    log_context_scope(const log_context_scope&) = delete;
    log_context_scope& operator=(const log_context_scope&) = delete;
    log_context_scope(log_context_scope&&) = delete;
    log_context_scope& operator=(log_context_scope&&) = delete;

private:
    void remove_logger_context(const std::string& key);

    log_fields previous_context_;
    bool had_previous_;
    std::vector<std::string> added_keys_;
    logger* logger_ = nullptr;
};

/**
 * @class scoped_context
 * @brief Convenience class for setting a single context field with RAII
 *
 * @details Provides a simpler interface for setting a single context field
 * that will be automatically removed when the scope exits.
 *
 * @example
 * @code
 * void process_order(int order_id) {
 *     scoped_context ctx("order_id", order_id);
 *
 *     // All logs in this scope include order_id
 *     logger->info_structured().message("Processing order").emit();
 * } // order_id automatically removed
 * @endcode
 *
 * @since 3.2.0
 */
class scoped_context {
public:
    /**
     * @brief Construct with string value
     */
    scoped_context(const std::string& key, const std::string& value)
        : key_(key), had_previous_(false) {
        auto prev = log_context_storage::get_field(key);
        if (prev) {
            had_previous_ = true;
            previous_value_ = *prev;
        }
        log_context_storage::set(key, value);
    }

    /**
     * @brief Construct with integer value
     */
    scoped_context(const std::string& key, int64_t value)
        : key_(key), had_previous_(false) {
        auto prev = log_context_storage::get_field(key);
        if (prev) {
            had_previous_ = true;
            previous_value_ = *prev;
        }
        log_context_storage::set(key, value);
    }

    /**
     * @brief Construct with int value (converts to int64_t)
     */
    scoped_context(const std::string& key, int value)
        : scoped_context(key, static_cast<int64_t>(value)) {}

    /**
     * @brief Construct with double value
     */
    scoped_context(const std::string& key, double value)
        : key_(key), had_previous_(false) {
        auto prev = log_context_storage::get_field(key);
        if (prev) {
            had_previous_ = true;
            previous_value_ = *prev;
        }
        log_context_storage::set(key, value);
    }

    /**
     * @brief Construct with boolean value
     */
    scoped_context(const std::string& key, bool value)
        : key_(key), had_previous_(false) {
        auto prev = log_context_storage::get_field(key);
        if (prev) {
            had_previous_ = true;
            previous_value_ = *prev;
        }
        log_context_storage::set(key, value);
    }

    /**
     * @brief Destructor - restores or removes the context field
     */
    ~scoped_context() {
        if (had_previous_) {
            log_context_storage::set(key_, previous_value_);
        } else {
            log_context_storage::remove(key_);
        }
    }

    // Non-copyable, non-movable
    scoped_context(const scoped_context&) = delete;
    scoped_context& operator=(const scoped_context&) = delete;
    scoped_context(scoped_context&&) = delete;
    scoped_context& operator=(scoped_context&&) = delete;

private:
    std::string key_;
    log_value previous_value_;
    bool had_previous_;
};

} // namespace kcenon::logger
