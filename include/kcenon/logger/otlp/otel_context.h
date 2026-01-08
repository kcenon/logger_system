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
 * @file otel_context.h
 * @brief OpenTelemetry context structure for trace correlation
 * @author 🍀☀🌕🌥 🌊
 * @since 3.0.0
 *
 * @details This file defines the OpenTelemetry context structure used for
 * correlating logs with distributed traces. The context follows the W3C
 * Trace Context specification for interoperability.
 *
 * @see https://www.w3.org/TR/trace-context/
 */

#include <string>
#include <optional>
#include <unordered_map>

namespace kcenon::logger::otlp {

/**
 * @struct otel_context
 * @brief OpenTelemetry context for trace correlation
 *
 * @details Contains trace identification and propagation information
 * following the W3C Trace Context specification.
 *
 * @example
 * @code
 * otel_context ctx{
 *     .trace_id = "0af7651916cd43dd8448eb211c80319c",
 *     .span_id = "b7ad6b7169203331",
 *     .trace_flags = "01",  // Sampled
 *     .trace_state = "congo=t61rcWkgMzE"
 * };
 * @endcode
 *
 * @since 3.0.0
 */
struct otel_context {
    /**
     * @brief Unique trace identifier (32 hex characters)
     *
     * @details The trace_id identifies the entire trace (all spans).
     * Format: 32 lowercase hexadecimal characters (128 bits).
     *
     * @example "0af7651916cd43dd8448eb211c80319c"
     */
    std::string trace_id;

    /**
     * @brief Unique span identifier (16 hex characters)
     *
     * @details The span_id identifies a specific span within a trace.
     * Format: 16 lowercase hexadecimal characters (64 bits).
     *
     * @example "b7ad6b7169203331"
     */
    std::string span_id;

    /**
     * @brief Trace flags (2 hex characters)
     *
     * @details Contains trace flags as defined in W3C Trace Context.
     * Currently only the sampled flag (bit 0) is defined:
     * - "00": Not sampled
     * - "01": Sampled
     *
     * @example "01"
     */
    std::string trace_flags;

    /**
     * @brief Vendor-specific trace state
     *
     * @details Optional vendor-specific data for tracing systems.
     * Format: key=value pairs separated by commas.
     *
     * @example "congo=t61rcWkgMzE,rojo=00f067aa0ba902b7"
     */
    std::string trace_state;

    /**
     * @brief Check if context is valid
     *
     * @return true if trace_id and span_id are present and properly formatted
     */
    [[nodiscard]] bool is_valid() const noexcept {
        return trace_id.length() == 32 && span_id.length() == 16 &&
               is_valid_hex(trace_id) && is_valid_hex(span_id);
    }

    /**
     * @brief Check if trace is sampled
     *
     * @return true if the sampled flag (bit 0) is set
     */
    [[nodiscard]] bool is_sampled() const noexcept {
        if (trace_flags.empty()) {
            return false;
        }
        // Check if the last character is '1', '3', '5', '7', '9', 'b', 'd', 'f'
        // (odd numbers in hex, meaning bit 0 is set)
        char last = trace_flags.back();
        return (last == '1' || last == '3' || last == '5' || last == '7' ||
                last == '9' || last == 'b' || last == 'd' || last == 'f' ||
                last == 'B' || last == 'D' || last == 'F');
    }

    /**
     * @brief Create an empty/invalid context
     *
     * @return Empty otel_context
     */
    [[nodiscard]] static otel_context empty() noexcept {
        return otel_context{};
    }

private:
    [[nodiscard]] static bool is_valid_hex(const std::string& str) noexcept {
        for (char c : str) {
            if (!((c >= '0' && c <= '9') ||
                  (c >= 'a' && c <= 'f') ||
                  (c >= 'A' && c <= 'F'))) {
                return false;
            }
        }
        return true;
    }
};

/**
 * @struct otel_resource
 * @brief OpenTelemetry resource attributes
 *
 * @details Contains resource attributes that describe the entity producing
 * telemetry. These attributes are attached to all exported data.
 *
 * @since 3.0.0
 */
struct otel_resource {
    /**
     * @brief Service name (required)
     *
     * @details Logical name of the service. Should be unique within
     * the deployment environment.
     *
     * @example "api-gateway", "user-service"
     */
    std::string service_name;

    /**
     * @brief Service version
     *
     * @example "1.2.3", "2025.01.08"
     */
    std::string service_version;

    /**
     * @brief Service namespace
     *
     * @details Namespace for the service, useful for grouping related services.
     *
     * @example "production", "staging"
     */
    std::string service_namespace;

    /**
     * @brief Service instance ID
     *
     * @details Unique identifier for the service instance. Useful in
     * replicated deployments.
     *
     * @example "pod-abc123", "instance-1"
     */
    std::string service_instance_id;

    /**
     * @brief Additional custom attributes
     *
     * @details Key-value pairs for custom resource attributes.
     *
     * @example {"environment": "production", "region": "us-east-1"}
     */
    std::unordered_map<std::string, std::string> attributes;
};

/**
 * @class otel_context_storage
 * @brief Thread-local storage for OpenTelemetry context
 *
 * @details Provides thread-safe storage and retrieval of OpenTelemetry
 * context using thread-local storage. This allows automatic context
 * propagation within a thread.
 *
 * @example
 * @code
 * // Set context for current thread
 * otel_context_storage::set(ctx);
 *
 * // All logs on this thread now include trace context
 * logger->info("Processing request");
 *
 * // Clear when done
 * otel_context_storage::clear();
 * @endcode
 *
 * @since 3.0.0
 */
class otel_context_storage {
public:
    /**
     * @brief Set the OpenTelemetry context for the current thread
     *
     * @param ctx Context to set
     */
    static void set(const otel_context& ctx) {
        get_storage() = ctx;
        get_has_context() = true;
    }

    /**
     * @brief Get the OpenTelemetry context for the current thread
     *
     * @return Optional context, empty if not set
     */
    [[nodiscard]] static std::optional<otel_context> get() {
        if (get_has_context()) {
            return get_storage();
        }
        return std::nullopt;
    }

    /**
     * @brief Clear the OpenTelemetry context for the current thread
     */
    static void clear() {
        get_storage() = otel_context{};
        get_has_context() = false;
    }

    /**
     * @brief Check if a context is set for the current thread
     *
     * @return true if context is set
     */
    [[nodiscard]] static bool has_context() {
        return get_has_context();
    }

private:
    static otel_context& get_storage() {
        thread_local otel_context storage;
        return storage;
    }

    static bool& get_has_context() {
        thread_local bool has_ctx = false;
        return has_ctx;
    }
};

/**
 * @class otel_context_scope
 * @brief RAII guard for OpenTelemetry context
 *
 * @details Automatically sets context on construction and clears it
 * on destruction. Useful for request processing.
 *
 * @example
 * @code
 * void handle_request(const Request& req) {
 *     otel_context_scope scope(extract_context(req));
 *     // All logs in this scope include trace context
 *     process(req);
 * } // Context automatically cleared here
 * @endcode
 *
 * @since 3.0.0
 */
class otel_context_scope {
public:
    /**
     * @brief Construct and set context
     *
     * @param ctx Context to set for this scope
     */
    explicit otel_context_scope(const otel_context& ctx)
        : previous_context_(otel_context_storage::get()),
          had_previous_(otel_context_storage::has_context()) {
        otel_context_storage::set(ctx);
    }

    /**
     * @brief Destructor - restores previous context
     */
    ~otel_context_scope() {
        if (had_previous_) {
            otel_context_storage::set(*previous_context_);
        } else {
            otel_context_storage::clear();
        }
    }

    // Non-copyable, non-movable
    otel_context_scope(const otel_context_scope&) = delete;
    otel_context_scope& operator=(const otel_context_scope&) = delete;
    otel_context_scope(otel_context_scope&&) = delete;
    otel_context_scope& operator=(otel_context_scope&&) = delete;

private:
    std::optional<otel_context> previous_context_;
    bool had_previous_;
};

} // namespace kcenon::logger::otlp
