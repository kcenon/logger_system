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
 * @file scoped_context_guard.h
 * @brief RAII guard for automatic context restoration
 * @author kcenon
 * @since 3.3.0
 *
 * @details This file provides the scoped_context_guard class that enables
 * RAII-based context management with automatic restoration on scope exit.
 * It works with the unified_log_context API to provide exception-safe
 * context handling.
 *
 * @example
 * @code
 * void handle_request(logger& log, const Request& req) {
 *     scoped_context_guard guard(log);
 *     guard.set_request(req.id())
 *          .set("user_id", req.user_id());
 *
 *     log.info_structured()
 *         .message("Processing request")
 *         .emit();
 * } // Context automatically restored here
 * @endcode
 */

#include "unified_log_context.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace kcenon::logger {

// Forward declaration
class logger;

/**
 * @class scoped_context_guard
 * @brief RAII guard for automatic context management
 *
 * @details Provides exception-safe context management by saving the current
 * context state on construction and restoring it on destruction. Supports
 * chainable setters for convenient context configuration.
 *
 * The guard tracks which keys were added during its lifetime and removes
 * only those keys on destruction, while restoring any values that were
 * overridden.
 *
 * Thread Safety: Each instance is tied to a specific logger instance and
 * should only be used from a single thread. However, multiple threads can
 * use separate guards with the same logger safely.
 *
 * @example
 * @code
 * // Example 1: Request context
 * void handle_request(logger& log, const Request& req) {
 *     scoped_context_guard guard(log);
 *     guard.set_request(req.id(), req.correlation_id())
 *          .set("user_id", req.user_id())
 *          .set("endpoint", req.path());
 *
 *     log.info("Processing request");
 * } // Context automatically restored
 *
 * // Example 2: Nested scopes
 * void process_order(logger& log, int order_id) {
 *     scoped_context_guard order_ctx(log);
 *     order_ctx.set("order_id", int64_t{order_id});
 *
 *     log.info("Processing order");
 *
 *     {
 *         scoped_context_guard step_ctx(log);
 *         step_ctx.set("step", "payment");
 *         log.info("Processing payment");
 *     } // "step" removed
 *
 *     log.info("Order complete");
 * } // "order_id" removed
 *
 * // Example 3: Exception safety
 * void risky_operation(logger& log) {
 *     scoped_context_guard guard(log);
 *     guard.set("operation", "risky");
 *
 *     might_throw(); // Context restored even if exception thrown
 * }
 * @endcode
 *
 * @since 3.3.0
 */
class scoped_context_guard {
public:
    /**
     * @brief Construct guard and save current context
     * @param log Logger whose context to manage
     *
     * @details Saves the current state of the logger's context.
     * On destruction, the context will be restored to this state.
     */
    explicit scoped_context_guard(logger& log);

    /**
     * @brief Construct guard and set a single context field
     * @param log Logger whose context to manage
     * @param key Field name to set
     * @param value Field value to set
     * @param category Context category (default: custom)
     *
     * @details Convenience constructor for setting a single field.
     * Equivalent to constructing with logger and then calling set().
     */
    scoped_context_guard(logger& log,
                        std::string_view key,
                        context_value value,
                        context_category category = context_category::custom);

    /**
     * @brief Destructor - restores previous context
     *
     * @details Removes all keys that were added by this guard and restores
     * any values that were overridden. This ensures the logger's context
     * is returned to its state before the guard was constructed.
     */
    ~scoped_context_guard();

    // Non-copyable
    scoped_context_guard(const scoped_context_guard&) = delete;
    scoped_context_guard& operator=(const scoped_context_guard&) = delete;

    // Movable
    scoped_context_guard(scoped_context_guard&& other) noexcept;
    scoped_context_guard& operator=(scoped_context_guard&& other) noexcept;

    // =========================================================================
    // Setters (chainable)
    // =========================================================================

    /**
     * @brief Set a context value
     * @param key Field name
     * @param value Field value
     * @param category Context category (default: custom)
     * @return Reference to this guard for chaining
     *
     * @details Sets a key-value pair in the logger's context. If the key
     * already exists, its value is overridden and the previous value is
     * saved for restoration.
     */
    scoped_context_guard& set(std::string_view key,
                              context_value value,
                              context_category category = context_category::custom);

    /**
     * @brief Set trace context
     * @param trace_id Trace identifier
     * @param span_id Span identifier
     * @param parent_span_id Optional parent span identifier
     * @return Reference to this guard for chaining
     *
     * @details Convenience method for setting distributed tracing context.
     * Sets trace_id, span_id, and optionally parent_span_id with
     * context_category::trace.
     */
    scoped_context_guard& set_trace(std::string_view trace_id,
                                    std::string_view span_id,
                                    std::optional<std::string_view> parent_span_id = std::nullopt);

    /**
     * @brief Set request context
     * @param request_id Request identifier
     * @param correlation_id Optional correlation identifier
     * @return Reference to this guard for chaining
     *
     * @details Convenience method for setting request tracking context.
     * Sets request_id and optionally correlation_id with
     * context_category::request.
     */
    scoped_context_guard& set_request(std::string_view request_id,
                                      std::optional<std::string_view> correlation_id = std::nullopt);

    /**
     * @brief Set OpenTelemetry context
     * @param ctx OpenTelemetry context structure
     * @return Reference to this guard for chaining
     *
     * @details Imports all fields from an otel_context structure with
     * context_category::otel.
     */
    scoped_context_guard& set_otel(const otlp::otel_context& ctx);

private:
    /**
     * @brief Track a key as added or overridden
     * @param key Key that was set
     */
    void track_key(std::string_view key);

    logger* logger_;                          // Logger whose context we're managing
    unified_log_context saved_context_;       // Saved context state
    std::vector<std::string> tracked_keys_;   // Keys added/modified by this guard
};

} // namespace kcenon::logger
