#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
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
 * @file unified_log_context.h
 * @brief Unified interface for managing all types of logging context
 * @author kcenon
 * @since 3.3.0
 *
 * @details This file provides the unified_log_context class that consolidates
 * all logging context management into a single, thread-safe interface.
 * It replaces the previous scattered approach of managing context through
 * separate mechanisms (log_context_storage, otel_context_storage, etc.).
 *
 * Key Features:
 * - Single unified storage for all context types
 * - Thread-safe operations with shared mutex
 * - Category-based context organization
 * - Type-safe value storage using std::variant
 *
 * @example
 * @code
 * unified_log_context ctx;
 * ctx.set("user_id", int64_t{12345});
 * ctx.set_trace("trace123", "span456");
 * ctx.set_request("req-789");
 *
 * auto fields = ctx.to_fields();
 * // fields contains all context entries
 * @endcode
 */

#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/otlp/otel_context.h>

#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace kcenon::logger {

/**
 * @brief Value type for unified context storage
 * @details Supports common types used in logging context:
 * - std::monostate for null/unset values
 * - bool for boolean values
 * - int64_t for integer values
 * - double for floating-point values
 * - std::string for text values
 * @since 3.3.0
 */
using context_value = std::variant<std::monostate, bool, int64_t, double, std::string>;

/**
 * @enum context_category
 * @brief Categories for organizing context entries
 *
 * @details Context entries are categorized to enable selective clearing
 * and querying. Each category represents a different source or purpose
 * for the context data.
 *
 * @since 3.3.0
 */
enum class context_category : uint8_t {
    custom = 0,   ///< User-defined custom fields
    trace = 1,    ///< Distributed tracing (trace_id, span_id, parent_span_id)
    request = 2,  ///< Request metadata (request_id, correlation_id)
    otel = 3      ///< OpenTelemetry specific fields
};

/**
 * @class unified_log_context
 * @brief Unified interface for managing all types of logging context
 *
 * @details This class provides a single point of access for all logging
 * context operations. It consolidates what was previously spread across
 * multiple storage mechanisms into one coherent interface.
 *
 * Thread Safety: All methods are thread-safe. Read operations use shared
 * locks, write operations use exclusive locks.
 *
 * @example
 * @code
 * unified_log_context ctx;
 *
 * // Set custom fields
 * ctx.set("user_id", int64_t{12345});
 * ctx.set("session_active", true);
 *
 * // Set trace context
 * ctx.set_trace("0af7651916cd43dd8448eb211c80319c", "b7ad6b7169203331");
 *
 * // Set request context
 * ctx.set_request("req-123", "corr-456");
 *
 * // Query context
 * auto user_id = ctx.get_as<int64_t>("user_id");
 * auto trace_id = ctx.get_string("trace_id");
 *
 * // Export to log_fields
 * auto fields = ctx.to_fields();
 *
 * // Clear specific category
 * ctx.clear(context_category::trace);
 * @endcode
 *
 * @since 3.3.0
 */
class unified_log_context {
public:
    /**
     * @brief Default constructor
     */
    unified_log_context() = default;

    /**
     * @brief Copy constructor
     * @param other Context to copy from
     */
    unified_log_context(const unified_log_context& other);

    /**
     * @brief Move constructor
     * @param other Context to move from
     */
    unified_log_context(unified_log_context&& other) noexcept;

    /**
     * @brief Copy assignment operator
     * @param other Context to copy from
     * @return Reference to this context
     */
    unified_log_context& operator=(const unified_log_context& other);

    /**
     * @brief Move assignment operator
     * @param other Context to move from
     * @return Reference to this context
     */
    unified_log_context& operator=(unified_log_context&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~unified_log_context() = default;

    // =========================================================================
    // Setters
    // =========================================================================

    /**
     * @brief Set a context value
     * @param key Field name
     * @param value Field value
     * @param category Context category (default: custom)
     * @return Reference to this context for chaining
     *
     * @details Sets a key-value pair in the context. If the key already exists,
     * its value and category are updated.
     */
    unified_log_context& set(std::string_view key,
                             context_value value,
                             context_category category = context_category::custom);

    /**
     * @brief Set trace context
     * @param trace_id Trace identifier (32 hex chars)
     * @param span_id Span identifier (16 hex chars)
     * @param parent_span_id Optional parent span identifier
     * @return Reference to this context for chaining
     *
     * @details Sets trace_id, span_id, and optionally parent_span_id with
     * context_category::trace. These are used for distributed tracing.
     */
    unified_log_context& set_trace(std::string_view trace_id,
                                   std::string_view span_id,
                                   std::optional<std::string_view> parent_span_id = std::nullopt);

    /**
     * @brief Set request context
     * @param request_id Request identifier
     * @param correlation_id Optional correlation identifier
     * @return Reference to this context for chaining
     *
     * @details Sets request_id and optionally correlation_id with
     * context_category::request. These are used for request tracking.
     */
    unified_log_context& set_request(std::string_view request_id,
                                     std::optional<std::string_view> correlation_id = std::nullopt);

    /**
     * @brief Set OpenTelemetry context
     * @param ctx OpenTelemetry context structure
     * @return Reference to this context for chaining
     *
     * @details Imports all fields from an otel_context structure with
     * context_category::otel.
     */
    unified_log_context& set_otel(const otlp::otel_context& ctx);

    // =========================================================================
    // Getters
    // =========================================================================

    /**
     * @brief Get a context value by key
     * @param key Field name
     * @return Optional context value, empty if not found
     */
    [[nodiscard]] std::optional<context_value> get(std::string_view key) const;

    /**
     * @brief Get a context value as a specific type
     * @tparam T Expected value type
     * @param key Field name
     * @return Optional typed value, empty if not found or wrong type
     *
     * @example
     * @code
     * auto user_id = ctx.get_as<int64_t>("user_id");
     * if (user_id) {
     *     std::cout << "User ID: " << *user_id << std::endl;
     * }
     * @endcode
     */
    template <typename T>
    [[nodiscard]] std::optional<T> get_as(std::string_view key) const {
        auto value = get(key);
        if (!value) {
            return std::nullopt;
        }
        if (auto* ptr = std::get_if<T>(&*value)) {
            return *ptr;
        }
        return std::nullopt;
    }

    /**
     * @brief Get a context value as string
     * @param key Field name
     * @param default_value Value to return if not found
     * @return String value or default
     *
     * @details Returns the value if it's a string, or the default value
     * if not found or if the value is not a string type.
     */
    [[nodiscard]] std::string get_string(std::string_view key,
                                         std::string_view default_value = "") const;

    /**
     * @brief Get the category of a context entry
     * @param key Field name
     * @return Optional category, empty if key not found
     */
    [[nodiscard]] std::optional<context_category> get_category(std::string_view key) const;

    // =========================================================================
    // Query methods
    // =========================================================================

    /**
     * @brief Check if a key exists in the context
     * @param key Field name
     * @return true if key exists
     */
    [[nodiscard]] bool has(std::string_view key) const;

    /**
     * @brief Check if the context is empty
     * @return true if no entries exist
     */
    [[nodiscard]] bool empty() const;

    /**
     * @brief Get the number of entries
     * @return Number of context entries
     */
    [[nodiscard]] size_t size() const;

    /**
     * @brief Get all keys in the context
     * @return Vector of key names
     */
    [[nodiscard]] std::vector<std::string> keys() const;

    /**
     * @brief Get keys for a specific category
     * @param category Category to filter by
     * @return Vector of key names in the specified category
     */
    [[nodiscard]] std::vector<std::string> keys(context_category category) const;

    // =========================================================================
    // Removal
    // =========================================================================

    /**
     * @brief Remove a specific key from the context
     * @param key Field name to remove
     */
    void remove(std::string_view key);

    /**
     * @brief Clear all entries from the context
     */
    void clear();

    /**
     * @brief Clear entries of a specific category
     * @param category Category to clear
     */
    void clear(context_category category);

    // =========================================================================
    // Export
    // =========================================================================

    /**
     * @brief Export context to log_fields format
     * @return Map of string keys to log_value variants
     *
     * @details Converts all context entries to the log_fields format
     * used by the structured logging system. Values are converted as follows:
     * - std::monostate -> (key is skipped)
     * - bool -> bool
     * - int64_t -> int64_t
     * - double -> double
     * - std::string -> std::string
     */
    [[nodiscard]] log_fields to_fields() const;

    /**
     * @brief Merge another context into this one
     * @param other Context to merge from
     * @param overwrite If true, other's values overwrite existing keys
     * @return Reference to this context for chaining
     *
     * @details Copies all entries from other into this context.
     * If overwrite is true (default), existing keys are replaced.
     * If overwrite is false, existing keys are preserved.
     */
    unified_log_context& merge(const unified_log_context& other, bool overwrite = true);

private:
    /**
     * @brief Internal entry structure
     */
    struct entry {
        context_value value;
        context_category category;
    };

    mutable std::shared_mutex mutex_;
    std::unordered_map<std::string, entry> data_;
};

}  // namespace kcenon::logger
