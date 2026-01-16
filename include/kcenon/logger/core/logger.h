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

#include <memory>
#include <vector>
#include <atomic>
#include <thread>
#include <string_view>

#include "thread_integration_detector.h"

// Use common_system's ILogger interface for standardized logging
#include <kcenon/common/interfaces/logger_interface.h>

#include "error_codes.h"
#include "metrics/logger_metrics.h"
#include "../backends/integration_backend.h"
#include "log_context.h"
#include <kcenon/logger/interfaces/logger_types.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/security/signal_manager.h>
#include <kcenon/logger/otlp/otel_context.h>
#include <kcenon/logger/sampling/sampling_config.h>
#include "structured_log_builder.h"

/**
 * @file logger.h
 * @brief High-performance, thread-safe logging system with asynchronous capabilities
 * @author 🍀☀🌕🌥 🌊
 * @since 1.0.0
 * 
 * @details This file defines the main logger class that provides a comprehensive
 * logging solution with support for multiple output destinations, asynchronous
 * processing, metrics collection, and dependency injection. The logger is designed
 * to be thread-safe and supports both synchronous and asynchronous operation modes.
 * 
 * @note The logger integrates with the thread_system when USE_THREAD_SYSTEM is defined,
 * providing seamless compatibility with the broader thread management infrastructure.
 * 
 * @example Basic usage:
 * @code
 * // Create a logger with default settings
 * kcenon::logger::logger logger;
 * 
 * // Add a console writer
 * logger.add_writer(std::make_unique<console_writer>());
 * 
 * // Start the logger in async mode
 * logger.start();
 * 
 * // Log messages
 * logger.log(log_level::info, "Application started");
 * logger.log(log_level::error, "An error occurred", __FILE__, __LINE__, __FUNCTION__);
 * 
 * // Flush and stop
 * logger.flush();
 * logger.stop();
 * @endcode
 * 
 * @example Advanced configuration with builder:
 * @code
 * auto result = logger_builder()
 *     .with_async(true)
 *     .with_buffer_size(16384)
 *     .with_min_level(log_level::debug)
 *     .with_metrics(true)
 *     .add_writer("console", std::make_unique<console_writer>())
 *     .add_writer("file", std::make_unique<file_writer>("logs/app.log"))
 *     .build();
 * 
 * if (result) {
 *     auto logger = std::move(result.value());
 *     // Use logger...
 * }
 * @endcode
 */

namespace kcenon::logger {

// Type aliases - always use logger_system types
using log_level = logger_system::log_level;
using health_status = logger_system::health_status;
using overflow_policy = logger_system::overflow_policy;

// Type aliases for convenience and compatibility
using logger_metrics = metrics::logger_performance_stats;
using performance_metrics = metrics::logger_performance_stats; // Alias for examples

// Metric type enum
enum class metric_type {
    gauge,
    counter,
    histogram
};

// Forward declarations
class log_collector;
class base_writer;
class logger_metrics_collector;
class log_filter_interface;  // Forward declaration for filtering system

namespace analysis {
class realtime_log_analyzer;
}  // namespace analysis

namespace sampling {
class log_sampler;
}  // namespace sampling

} // namespace kcenon::logger

// Include routing header for log_router
#include "../routing/log_router.h"

namespace kcenon::logger {

// Routing type alias
using routing::log_router;
using routing::route_config;
using routing::router_builder;

/**
 * @class logger
 * @brief Main logger implementation providing high-performance logging facilities
 *
 * @details The logger class provides a high-performance, thread-safe logging system with:
 * - Asynchronous logging with configurable batching for optimal throughput
 * - Multiple writer support for outputting to different destinations simultaneously
 * - Real-time metrics collection and performance monitoring
 * - Configurable filtering and routing of log messages
 * - Integration with monitoring backends for production observability
 *
 * The logger uses the PIMPL idiom to hide implementation details and maintain ABI stability.
 * Implements common::interfaces::ILogger for standardized logging interface (Phase 2) and
 * common::interfaces::IMonitorable for observability (Phase 2.2). Can be adapted to legacy
 * thread_system interfaces through dedicated adapter classes.
 *
 * @warning When using asynchronous mode, ensure proper shutdown by calling stop() and flush()
 * before destroying the logger to prevent loss of buffered messages.
 *
 * @since 1.0.0
 * @since 2.0.0 Implements common::interfaces::ILogger
 *
 * @note For integration with common_system monitoring interfaces, use logger_monitoring_adapter
 */
class logger : public security::critical_logger_interface,
               public common::interfaces::ILogger {
public:
    /**
     * @brief Constructor with optional configuration
     * @param async Enable asynchronous logging (default: true)
     * @param buffer_size Size of the log buffer in bytes (default: 8192)
     * @param backend Integration backend for level conversion (default: auto-detect)
     *
     * @details Creates a logger instance with the specified configuration.
     * In async mode, a background thread is created to process log messages,
     * providing better performance for high-throughput applications.
     *
     * If no backend is specified, the logger will auto-detect the appropriate
     * backend based on compile-time flags (USE_THREAD_SYSTEM_INTEGRATION).
     *
     * @note The buffer_size parameter affects memory usage and batching efficiency.
     * Larger buffers can improve throughput but increase memory consumption.
     *
     * @since 1.0.0
     */
    explicit logger(bool async = true, std::size_t buffer_size = 8192,
                   std::unique_ptr<backends::integration_backend> backend = nullptr);
    
    /**
     * @brief Destructor - ensures all logs are flushed
     * 
     * @details Properly shuts down the logger, ensuring all buffered messages
     * are written to their destinations before destruction. Automatically calls
     * stop() and flush() if the logger is still running.
     * 
     * @warning Destruction may block until all pending messages are processed.
     * 
     * @since 1.0.0
     */
    ~logger();
    
    // =========================================================================
    // ILogger interface implementation (common::interfaces::ILogger)
    // =========================================================================

    /**
     * @brief Log a message with specified level (ILogger interface)
     * @param level Log level using common::interfaces::log_level
     * @param message Log message
     * @return VoidResult indicating success or error
     *
     * @note Implements common::interfaces::ILogger::log
     * @since 2.0.0
     */
    common::VoidResult log(common::interfaces::log_level level,
                           const std::string& message) override;

    /**
     * @brief Log a message with source location (ILogger interface, C++20)
     * @param level Log level
     * @param message Log message (string_view for efficiency)
     * @param loc Source location (automatically captured at call site)
     * @return VoidResult indicating success or error
     *
     * @note Implements common::interfaces::ILogger::log with source_location
     * @since 2.0.0
     */
    common::VoidResult log(common::interfaces::log_level level,
                           std::string_view message,
                           const common::source_location& loc = common::source_location::current()) override;

    /**
     * @brief Log a message with source location information (legacy convenience method)
     * @param level Log level
     * @param message Log message
     * @param file Source file name
     * @param line Source line number
     * @param function Function name
     * @return VoidResult indicating success or error
     *
     * @deprecated Use log(log_level, std::string_view, const source_location&) instead
     * @note This method was removed from common::interfaces::ILogger in v3.0.0 (Issue #217).
     *       It is preserved here for backward compatibility but no longer overrides ILogger.
     * @since 2.0.0
     */
    common::VoidResult log(common::interfaces::log_level level,
                           const std::string& message,
                           const std::string& file,
                           int line,
                           const std::string& function);

    /**
     * @brief Log a structured entry (ILogger interface)
     * @param entry Log entry containing all information
     * @return VoidResult indicating success or error
     *
     * @note Implements common::interfaces::ILogger::log(const log_entry&)
     * @since 2.0.0
     */
    common::VoidResult log(const common::interfaces::log_entry& entry) override;

    /**
     * @brief Check if logging is enabled for the specified level (ILogger interface)
     * @param level Log level to check using common::interfaces::log_level
     * @return true if logging is enabled for this level
     *
     * @note Implements common::interfaces::ILogger::is_enabled
     * @since 2.0.0
     */
    bool is_enabled(common::interfaces::log_level level) const override;

    /**
     * @brief Set the minimum log level (ILogger interface)
     * @param level Minimum level for messages to be logged
     * @return VoidResult indicating success or error
     *
     * @note Implements common::interfaces::ILogger::set_level
     * @since 2.0.0
     */
    common::VoidResult set_level(common::interfaces::log_level level) override;

    /**
     * @brief Get the current minimum log level (ILogger interface)
     * @return Current minimum log level using common::interfaces::log_level
     *
     * @note Implements common::interfaces::ILogger::get_level
     * @since 2.0.0
     */
    common::interfaces::log_level get_level() const override;

    /**
     * @brief Flush any buffered log messages (ILogger interface)
     * @return VoidResult indicating success or error
     *
     * @note Implements common::interfaces::ILogger::flush
     * @since 2.0.0
     */
    common::VoidResult flush() override;

    // =========================================================================
    // Backward-compatible API (logger_system native types)
    // These methods allow seamless use of logger_system::log_level
    // =========================================================================

    /**
     * @brief Log a simple message using native log_level
     * @param level Severity level of the message using logger_system::log_level
     * @param message The message to log
     *
     * @details Logs a message without source location information.
     * The message is queued for asynchronous processing if async mode is enabled.
     * Internally converts to common::interfaces::log_level.
     *
     * @note Messages below the minimum log level are discarded for performance.
     *
     * @since 1.0.0
     */
    void log(log_level level, const std::string& message);

    /**
     * @brief Log a message with source location using native log_level
     * @param level Severity level of the message using logger_system::log_level
     * @param message The message to log
     * @param file Source file name (typically __FILE__)
     * @param line Line number in source file (typically __LINE__)
     * @param function Function name (typically __FUNCTION__)
     *
     * @details Logs a message with complete source location information for debugging.
     * This overload is useful for tracking the exact origin of log messages.
     * Internally converts to common::interfaces::log_level.
     *
     * @since 1.0.0
     */
    void log(log_level level,
             const std::string& message,
             const std::string& file,
             int line,
             const std::string& function);

    /**
     * @brief Log using a precomputed log_context
     * @param level Severity level using logger_system::log_level
     * @param message The message to log
     * @param context Source location context
     *
     * @since 1.0.0
     */
    void log(log_level level,
             const std::string& message,
             const core::log_context& context);

    /**
     * @brief Check if a log level is enabled using native log_level
     * @param level The log level to check using logger_system::log_level
     * @return true if messages at this level will be logged, false otherwise
     *
     * @details Use this method to avoid expensive message construction
     * for log levels that won't be output.
     *
     * @since 1.0.0
     */
    bool is_enabled(log_level level) const;
    
    // Additional logger-specific methods
    
    /**
     * @brief Add a writer to output logs
     * @param writer Unique pointer to the writer to add
     * @return common::VoidResult Success or error code
     *
     * @details Adds a new output destination for log messages. Multiple writers
     * can be added to send logs to different destinations simultaneously.
     * Ownership of the writer is transferred to the logger.
     *
     * @note Writers are processed in the order they were added.
     *
     * @example
     * @code
     * auto result = logger.add_writer(std::make_unique<file_writer>("app.log"));
     * if (result.is_err()) {
     *     std::cerr << "Failed to add writer: " << result.error().message << std::endl;
     * }
     * @endcode
     *
     * @since 1.0.0
     */
    common::VoidResult add_writer(std::unique_ptr<base_writer> writer);

    /**
     * @brief Remove all writers
     * @return common::VoidResult Success or error code
     *
     * @details Removes all currently registered writers from the logger.
     * After this call, log messages will not be output anywhere until
     * new writers are added.
     *
     * @warning This operation cannot be undone. Removed writers are destroyed.
     *
     * @since 1.0.0
     */
    common::VoidResult clear_writers();
    
    /**
     * @brief Set the minimum log level (legacy API)
     * @param level Minimum level to log using logger_system::log_level
     *
     * @details Sets the threshold for message logging. Messages with a level
     * below this threshold are discarded for performance optimization.
     *
     * @deprecated Use set_level(common::interfaces::log_level) instead
     * @note This is a thread-safe operation that takes effect immediately.
     *
     * @since 1.0.0
     */
    void set_min_level(log_level level);

    /**
     * @brief Get the minimum log level (legacy API)
     * @return Current minimum log level using logger_system::log_level
     *
     * @deprecated Use get_level() which returns common::interfaces::log_level instead
     * @details Returns the current threshold level for message logging.
     *
     * @since 1.0.0
     */
    log_level get_min_level() const;
    
    /**
     * @brief Start the logger (for async mode)
     * @return common::VoidResult Success or error code
     *
     * @details Starts the background processing thread for asynchronous logging.
     * This method must be called before logging in async mode. Has no effect
     * in synchronous mode.
     *
     * @note Calling start() on an already running logger is a no-op.
     *
     * @warning Not calling start() in async mode will cause log messages to queue
     * indefinitely without being processed.
     *
     * @example
     * @code
     * kcenon::logger::logger logger(true, 16384); // async mode
     * auto result = logger.start();
     * if (result.is_err()) {
     *     std::cerr << "Failed to start logger: " << result.error().message << std::endl;
     * }
     * @endcode
     *
     * @since 1.0.0
     */
    common::VoidResult start();

    /**
     * @brief Stop the logger
     * @return common::VoidResult Success or error code
     *
     * @details Stops the background processing thread and flushes all pending messages.
     * This is a blocking operation that waits for all queued messages to be processed.
     *
     * @note After stopping, the logger can be restarted with start().
     *
     * @warning Stopping the logger may take time if there are many pending messages.
     *
     * @since 1.0.0
     */
    common::VoidResult stop();
    
    /**
     * @brief Check if logger is running
     * @return true if the logger is currently running, false otherwise
     * 
     * @details In async mode, returns true if the background processing thread
     * is active. In sync mode, always returns true.
     * 
     * @since 1.0.0
     */
    bool is_running() const;
    
    /**
     * @brief Enable or disable metrics collection
     * @param enable true to enable metrics collection
     * @return common::VoidResult indicating success or error
     */
    common::VoidResult enable_metrics_collection(bool enable = true);
    
    /**
     * @brief Check if metrics collection is enabled
     * @return true if metrics collection is enabled
     */
    bool is_metrics_collection_enabled() const;
    
    /**
     * @brief Get current performance metrics
     * @return Result containing metrics or error
     */
    result<metrics::logger_performance_stats> get_current_metrics() const;
    
    /**
     * @brief Get metrics history for a specific duration
     * @param duration How far back to retrieve metrics
     * @return Result containing metrics snapshot or error
     */
    result<std::unique_ptr<metrics::logger_performance_stats>> get_metrics_history(std::chrono::seconds duration) const;
    
    /**
     * @brief Reset performance metrics
     * @return common::VoidResult indicating success or error
     */
    common::VoidResult reset_metrics();
    
    /**
     * @brief Get metrics collector for direct access
     * @return Pointer to metrics collector (may be null if not enabled)
     */
    logger_metrics_collector* get_metrics_collector();
    
    /**
     * @brief Add a writer with a specific name
     * @param name Name for the writer (used for later retrieval/removal)
     * @param writer Unique pointer to the writer
     * @return common::VoidResult Success or error code
     *
     * @details Adds a writer with an associated name for later lookup or removal.
     * The writer is also added to the general writers list.
     *
     * @since 1.0.0
     */
    common::VoidResult add_writer(const std::string& name, std::unique_ptr<base_writer> writer);
    
    /**
     * @brief Remove a writer by name
     * @param name Name of the writer to remove
     * @return true if writer was found and removed
     */
    bool remove_writer(const std::string& name);
    
    /**
     * @brief Get a writer by name
     * @param name Name of the writer
     * @return Pointer to writer or nullptr if not found
     */
    base_writer* get_writer(const std::string& name);
    
    /**
     * @brief Set global filter for log entries
     * @param filter Filter to apply to all logs
     *
     * @details Sets a filter that determines whether log entries should be processed.
     * If a filter is set, only entries that pass the filter will be logged.
     * Pass nullptr to remove the current filter.
     *
     * @example
     * @code
     * // Only log warning and above
     * logger.set_filter(std::make_unique<filters::level_filter>(log_level::warning));
     *
     * // Remove filter
     * logger.set_filter(nullptr);
     * @endcode
     *
     * @since 2.0.0
     */
    void set_filter(std::unique_ptr<log_filter_interface> filter);

    /**
     * @brief Check if a filter is currently set
     * @return true if a filter is active, false otherwise
     *
     * @since 2.0.0
     */
    bool has_filter() const;

    // =========================================================================
    // Routing system
    // =========================================================================

    /**
     * @brief Get the log router for configuration
     * @return Reference to the log router
     *
     * @details Returns the internal router for adding/modifying routing rules.
     * The router determines which writers receive specific log messages
     * based on configurable rules (level, pattern matching, etc.).
     *
     * @note Thread-safe: Multiple threads can read the router simultaneously,
     * but modifications should be synchronized externally if done during logging.
     *
     * @example
     * @code
     * auto& router = logger.get_router();
     * router.add_route(route_config{
     *     .writer_names = {"errors"},
     *     .filter = std::make_unique<level_filter>(log_level::error),
     *     .stop_propagation = false
     * });
     * @endcode
     *
     * @since 2.0.0
     */
    log_router& get_router();

    /**
     * @brief Get the log router (const version)
     * @return Const reference to the log router
     *
     * @since 2.0.0
     */
    const log_router& get_router() const;

    /**
     * @brief Set a new log router
     * @param router Router to use
     *
     * @details Replaces the current router with the provided one.
     * Useful for setting up a pre-configured router.
     *
     * @since 2.0.0
     */
    void set_router(std::unique_ptr<log_router> router);

    /**
     * @brief Check if routing is enabled
     * @return true if router has any routes configured
     *
     * @since 2.0.0
     */
    bool has_routing() const;

    // =========================================================================
    // Real-time analysis
    // =========================================================================

    /**
     * @brief Set real-time log analyzer for anomaly detection
     * @param analyzer The analyzer instance
     *
     * @details Sets a real-time analyzer that processes each log entry
     * for anomaly detection. The analyzer is invoked synchronously during
     * log processing.
     *
     * @example
     * @code
     * auto analyzer = std::make_unique<realtime_log_analyzer>();
     * analyzer->set_error_spike_threshold(50);
     * analyzer->set_anomaly_callback([](const anomaly_event& e) {
     *     alert_ops_team(e);
     * });
     * logger->set_realtime_analyzer(std::move(analyzer));
     * @endcode
     *
     * @since 3.2.0
     */
    void set_realtime_analyzer(std::unique_ptr<analysis::realtime_log_analyzer> analyzer);

    /**
     * @brief Get the real-time analyzer (if set)
     * @return Pointer to analyzer or nullptr if not set
     *
     * @since 3.2.0
     */
    [[nodiscard]] analysis::realtime_log_analyzer* get_realtime_analyzer();

    /**
     * @brief Get the real-time analyzer (const version)
     * @return Pointer to analyzer or nullptr if not set
     *
     * @since 3.2.0
     */
    [[nodiscard]] const analysis::realtime_log_analyzer* get_realtime_analyzer() const;

    /**
     * @brief Check if real-time analysis is enabled
     * @return true if a realtime analyzer is set
     *
     * @since 3.2.0
     */
    [[nodiscard]] bool has_realtime_analysis() const;

    // =========================================================================
    // OpenTelemetry context management
    // =========================================================================

    /**
     * @brief Set OpenTelemetry context for the current thread
     * @param ctx Context to set (trace_id, span_id, etc.)
     *
     * @details Sets the OTEL context for trace correlation. All subsequent
     * log messages on this thread will include the trace_id and span_id.
     *
     * @example
     * @code
     * logger->set_otel_context(otlp::otel_context{
     *     .trace_id = extract_trace_id(request.headers),
     *     .span_id = extract_span_id(request.headers)
     * });
     * @endcode
     *
     * @since 3.0.0
     */
    void set_otel_context(const otlp::otel_context& ctx);

    /**
     * @brief Get the current OpenTelemetry context for this thread
     * @return Optional context, empty if not set
     *
     * @details Returns the OTEL context set for the current thread.
     * Useful for propagating context to downstream services.
     *
     * @since 3.0.0
     */
    [[nodiscard]] std::optional<otlp::otel_context> get_otel_context() const;

    /**
     * @brief Clear the OpenTelemetry context for this thread
     *
     * @details Clears the OTEL context. Call this at the end of request
     * processing to prevent context leakage.
     *
     * @since 3.0.0
     */
    void clear_otel_context();

    /**
     * @brief Check if OTEL context is set for this thread
     * @return true if context is set
     *
     * @since 3.0.0
     */
    [[nodiscard]] bool has_otel_context() const;

    // =========================================================================
    // Structured logging API
    // =========================================================================

    /**
     * @brief Create a structured log builder at the specified level (canonical API)
     * @param level Log level for the structured entry
     * @return Builder for constructing the structured log entry
     *
     * @details This is the canonical API for structured logging. Returns a builder
     * that allows adding arbitrary fields to the log entry. The entry is logged
     * when emit() is called on the builder. Context fields (set via set_context())
     * are automatically included.
     *
     * Level-specific convenience methods (trace_structured(), debug_structured(), etc.)
     * are deprecated and will be removed in a future version.
     *
     * @example
     * @code
     * // Recommended approach
     * logger->log_structured(log_level::info)
     *     .message("User login")
     *     .field("user_id", 12345)
     *     .field("ip_address", "192.168.1.1")
     *     .emit();
     *
     * // Different log levels
     * logger->log_structured(log_level::error)
     *     .message("Database connection failed")
     *     .field("retry_count", 3)
     *     .emit();
     * @endcode
     *
     * @since 3.1.0
     */
    [[nodiscard]] structured_log_builder log_structured(log_level level);

    /**
     * @brief Create a structured trace log builder
     * @return Builder for constructing the structured log entry
     *
     * @deprecated Use log_structured(log_level::trace) instead
     * @since 3.1.0
     */
    [[deprecated("Use log_structured(log_level::trace) instead")]]
    [[nodiscard]] structured_log_builder trace_structured();

    /**
     * @brief Create a structured debug log builder
     * @return Builder for constructing the structured log entry
     *
     * @deprecated Use log_structured(log_level::debug) instead
     * @since 3.1.0
     */
    [[deprecated("Use log_structured(log_level::debug) instead")]]
    [[nodiscard]] structured_log_builder debug_structured();

    /**
     * @brief Create a structured info log builder
     * @return Builder for constructing the structured log entry
     *
     * @deprecated Use log_structured(log_level::info) instead
     * @since 3.1.0
     */
    [[deprecated("Use log_structured(log_level::info) instead")]]
    [[nodiscard]] structured_log_builder info_structured();

    /**
     * @brief Create a structured warning log builder
     * @return Builder for constructing the structured log entry
     *
     * @deprecated Use log_structured(log_level::warning) instead
     * @since 3.1.0
     */
    [[deprecated("Use log_structured(log_level::warning) instead")]]
    [[nodiscard]] structured_log_builder warn_structured();

    /**
     * @brief Create a structured error log builder
     * @return Builder for constructing the structured log entry
     *
     * @deprecated Use log_structured(log_level::error) instead
     * @since 3.1.0
     */
    [[deprecated("Use log_structured(log_level::error) instead")]]
    [[nodiscard]] structured_log_builder error_structured();

    /**
     * @brief Create a structured fatal log builder
     * @return Builder for constructing the structured log entry
     *
     * @deprecated Use log_structured(log_level::fatal) instead
     * @since 3.1.0
     */
    [[deprecated("Use log_structured(log_level::fatal) instead")]]
    [[nodiscard]] structured_log_builder fatal_structured();

    // =========================================================================
    // Context fields management
    // =========================================================================

    /**
     * @brief Set a context field that persists across log calls
     * @param key Field name
     * @param value Field value
     *
     * @details Context fields are automatically included in all structured
     * log entries created via *_structured() methods. Useful for request IDs,
     * trace IDs, or other per-request/per-session metadata.
     *
     * @example
     * @code
     * logger->set_context("request_id", "req-123");
     * logger->set_context("trace_id", "trace-456");
     *
     * // All subsequent structured logs include request_id and trace_id
     * logger->info_structured()
     *     .message("Processing request")
     *     .emit();
     * @endcode
     *
     * @since 3.1.0
     */
    void set_context(const std::string& key, const std::string& value);

    /**
     * @brief Set a context field with integer value
     * @param key Field name
     * @param value Field value
     * @since 3.1.0
     */
    void set_context(const std::string& key, int64_t value);

    /**
     * @brief Set a context field with double value
     * @param key Field name
     * @param value Field value
     * @since 3.1.0
     */
    void set_context(const std::string& key, double value);

    /**
     * @brief Set a context field with boolean value
     * @param key Field name
     * @param value Field value
     * @since 3.1.0
     */
    void set_context(const std::string& key, bool value);

    /**
     * @brief Remove a context field
     * @param key Field name to remove
     * @since 3.1.0
     */
    void remove_context(const std::string& key);

    /**
     * @brief Clear all context fields
     * @since 3.1.0
     */
    void clear_context();

    /**
     * @brief Check if any context fields are set
     * @return true if at least one context field is set
     * @since 3.1.0
     */
    [[nodiscard]] bool has_context() const;

    /**
     * @brief Get a copy of the current context fields
     * @return Copy of context fields map
     * @note Returns a copy for thread safety
     * @since 3.1.0
     */
    [[nodiscard]] log_fields get_context() const;

    // =========================================================================
    // Generic context ID API
    // =========================================================================

    /**
     * @brief Set a context ID value by key
     * @param key The context ID key (e.g., "correlation_id", "trace_id")
     * @param value The context ID value
     *
     * @details Sets a context ID that is automatically included in all
     * structured log entries. This is the canonical API for managing
     * context IDs.
     *
     * Common keys:
     * - "correlation_id": Request/transaction tracking
     * - "request_id": HTTP request identification
     * - "trace_id": Distributed trace identification (32 hex chars)
     * - "span_id": Operation span identification (16 hex chars)
     * - "parent_span_id": Parent operation span (16 hex chars)
     *
     * @example
     * @code
     * logger->set_context_id("correlation_id", "abc-123");
     * logger->set_context_id("trace_id", "0af7651916cd43dd8448eb211c80319c");
     * @endcode
     *
     * @since 3.4.0
     */
    void set_context_id(std::string_view key, std::string_view value);

    /**
     * @brief Get a context ID value by key
     * @param key The context ID key
     * @return The context ID value, or empty string if not set
     *
     * @since 3.4.0
     */
    [[nodiscard]] std::string get_context_id(std::string_view key) const;

    /**
     * @brief Clear a context ID by key
     * @param key The context ID key to clear
     *
     * @since 3.4.0
     */
    void clear_context_id(std::string_view key);

    /**
     * @brief Check if a context ID is set
     * @param key The context ID key to check
     * @return true if the context ID is set
     *
     * @since 3.4.0
     */
    [[nodiscard]] bool has_context_id(std::string_view key) const;

    /**
     * @brief Clear all context IDs
     *
     * @details Clears all context IDs (correlation_id, request_id, trace_id,
     * span_id, parent_span_id). Use this at the end of request processing
     * to prevent context leakage.
     *
     * @note This only clears known context ID keys, not other context fields
     * set via set_context().
     *
     * @since 3.4.0
     */
    void clear_all_context_ids();

    // =========================================================================
    // Correlation ID convenience API (deprecated)
    // =========================================================================

    /**
     * @brief Set correlation ID for request tracking
     * @param correlation_id Unique identifier for the request/transaction
     *
     * @details Sets a correlation ID that is automatically included in all
     * structured log entries. This is a convenience method equivalent to
     * set_context("correlation_id", correlation_id).
     *
     * Correlation IDs are essential for:
     * - Tracing requests across microservices
     * - Grouping related log entries
     * - Debugging distributed transactions
     *
     * @example
     * @code
     * // Set correlation ID from incoming request header
     * logger->set_correlation_id(request.get_header("X-Correlation-ID"));
     *
     * // All subsequent structured logs include the correlation_id field
     * logger->info_structured()
     *     .message("Processing request")
     *     .emit();
     * @endcode
     *
     * @deprecated Use set_context_id("correlation_id", value) instead
     * @since 3.1.0
     */
    [[deprecated("Use set_context_id(\"correlation_id\", value) instead")]]
    void set_correlation_id(const std::string& correlation_id);

    /**
     * @brief Get the current correlation ID
     * @return Current correlation ID, or empty string if not set
     *
     * @deprecated Use get_context_id("correlation_id") instead
     * @since 3.1.0
     */
    [[deprecated("Use get_context_id(\"correlation_id\") instead")]]
    [[nodiscard]] std::string get_correlation_id() const;

    /**
     * @brief Clear the correlation ID
     *
     * @details Call this at the end of request processing to prevent
     * correlation ID leakage to subsequent requests.
     *
     * @deprecated Use clear_context_id("correlation_id") instead
     * @since 3.1.0
     */
    [[deprecated("Use clear_context_id(\"correlation_id\") instead")]]
    void clear_correlation_id();

    /**
     * @brief Check if a correlation ID is set
     * @return true if correlation ID is set
     *
     * @deprecated Use has_context_id("correlation_id") instead
     * @since 3.1.0
     */
    [[deprecated("Use has_context_id(\"correlation_id\") instead")]]
    [[nodiscard]] bool has_correlation_id() const;

    /**
     * @brief Set request ID for request tracking
     * @param request_id Unique identifier for the request
     *
     * @details Alias for correlation ID, commonly used in web applications.
     * Equivalent to set_context("request_id", request_id).
     *
     * @deprecated Use set_context_id("request_id", value) instead
     * @since 3.1.0
     */
    [[deprecated("Use set_context_id(\"request_id\", value) instead")]]
    void set_request_id(const std::string& request_id);

    /**
     * @brief Get the current request ID
     * @return Current request ID, or empty string if not set
     *
     * @deprecated Use get_context_id("request_id") instead
     * @since 3.1.0
     */
    [[deprecated("Use get_context_id(\"request_id\") instead")]]
    [[nodiscard]] std::string get_request_id() const;

    /**
     * @brief Clear the request ID
     *
     * @deprecated Use clear_context_id("request_id") instead
     * @since 3.1.0
     */
    [[deprecated("Use clear_context_id(\"request_id\") instead")]]
    void clear_request_id();

    /**
     * @brief Check if a request ID is set
     * @return true if request ID is set
     *
     * @deprecated Use has_context_id("request_id") instead
     * @since 3.1.0
     */
    [[deprecated("Use has_context_id(\"request_id\") instead")]]
    [[nodiscard]] bool has_request_id() const;

    // =========================================================================
    // Trace ID / Span ID convenience API (deprecated)
    // =========================================================================

    /**
     * @brief Set trace ID for distributed tracing
     * @param trace_id Unique identifier for the trace (32 hex chars)
     *
     * @details Sets a trace ID that is automatically included in all
     * structured log entries. This is stored as a context field and also
     * updates the OTEL context if set.
     *
     * Trace IDs are essential for:
     * - Correlating logs across microservices
     * - Identifying all logs from a single request flow
     * - Integration with distributed tracing systems
     *
     * @example
     * @code
     * // Set trace ID from incoming request header
     * logger->set_trace_id(request.get_header("X-Trace-ID"));
     *
     * // All subsequent structured logs include the trace_id field
     * logger->info_structured()
     *     .message("Processing request")
     *     .emit();
     * @endcode
     *
     * @deprecated Use set_context_id("trace_id", value) instead
     * @since 3.2.0
     */
    [[deprecated("Use set_context_id(\"trace_id\", value) instead")]]
    void set_trace_id(const std::string& trace_id);

    /**
     * @brief Get the current trace ID
     * @return Current trace ID, or empty string if not set
     *
     * @deprecated Use get_context_id("trace_id") instead
     * @since 3.2.0
     */
    [[deprecated("Use get_context_id(\"trace_id\") instead")]]
    [[nodiscard]] std::string get_trace_id() const;

    /**
     * @brief Clear the trace ID
     *
     * @details Call this at the end of request processing to prevent
     * trace ID leakage to subsequent requests.
     *
     * @deprecated Use clear_context_id("trace_id") instead
     * @since 3.2.0
     */
    [[deprecated("Use clear_context_id(\"trace_id\") instead")]]
    void clear_trace_id();

    /**
     * @brief Check if a trace ID is set
     * @return true if trace ID is set
     *
     * @deprecated Use has_context_id("trace_id") instead
     * @since 3.2.0
     */
    [[deprecated("Use has_context_id(\"trace_id\") instead")]]
    [[nodiscard]] bool has_trace_id() const;

    /**
     * @brief Set span ID for distributed tracing
     * @param span_id Unique identifier for the span (16 hex chars)
     *
     * @details Sets a span ID that is automatically included in all
     * structured log entries. Used to identify the current operation
     * within a trace.
     *
     * @example
     * @code
     * // Set span ID for current operation
     * logger->set_span_id(generate_span_id());
     *
     * // Logs include span_id for correlation
     * logger->info_structured()
     *     .message("Database query executed")
     *     .emit();
     * @endcode
     *
     * @deprecated Use set_context_id("span_id", value) instead
     * @since 3.2.0
     */
    [[deprecated("Use set_context_id(\"span_id\", value) instead")]]
    void set_span_id(const std::string& span_id);

    /**
     * @brief Get the current span ID
     * @return Current span ID, or empty string if not set
     *
     * @deprecated Use get_context_id("span_id") instead
     * @since 3.2.0
     */
    [[deprecated("Use get_context_id(\"span_id\") instead")]]
    [[nodiscard]] std::string get_span_id() const;

    /**
     * @brief Clear the span ID
     *
     * @deprecated Use clear_context_id("span_id") instead
     * @since 3.2.0
     */
    [[deprecated("Use clear_context_id(\"span_id\") instead")]]
    void clear_span_id();

    /**
     * @brief Check if a span ID is set
     * @return true if span ID is set
     *
     * @deprecated Use has_context_id("span_id") instead
     * @since 3.2.0
     */
    [[deprecated("Use has_context_id(\"span_id\") instead")]]
    [[nodiscard]] bool has_span_id() const;

    /**
     * @brief Set parent span ID for distributed tracing
     * @param parent_span_id Span ID of the parent operation (16 hex chars)
     *
     * @details Sets the parent span ID for establishing parent-child
     * relationships between spans in a trace.
     *
     * @deprecated Use set_context_id("parent_span_id", value) instead
     * @since 3.2.0
     */
    [[deprecated("Use set_context_id(\"parent_span_id\", value) instead")]]
    void set_parent_span_id(const std::string& parent_span_id);

    /**
     * @brief Get the current parent span ID
     * @return Current parent span ID, or empty string if not set
     *
     * @deprecated Use get_context_id("parent_span_id") instead
     * @since 3.2.0
     */
    [[deprecated("Use get_context_id(\"parent_span_id\") instead")]]
    [[nodiscard]] std::string get_parent_span_id() const;

    /**
     * @brief Clear the parent span ID
     *
     * @deprecated Use clear_context_id("parent_span_id") instead
     * @since 3.2.0
     */
    [[deprecated("Use clear_context_id(\"parent_span_id\") instead")]]
    void clear_parent_span_id();

    /**
     * @brief Check if a parent span ID is set
     * @return true if parent span ID is set
     *
     * @deprecated Use has_context_id("parent_span_id") instead
     * @since 3.2.0
     */
    [[deprecated("Use has_context_id(\"parent_span_id\") instead")]]
    [[nodiscard]] bool has_parent_span_id() const;

    // =========================================================================
    // Log sampling API
    // =========================================================================

    /**
     * @brief Set the log sampler for volume reduction
     * @param sampler The sampler instance to use
     *
     * @details Sets a sampler that determines which logs are passed through
     * based on configured sampling strategies (random, rate limiting, adaptive,
     * or hash-based). Critical levels can bypass sampling.
     *
     * @example
     * @code
     * auto sampler = std::make_unique<log_sampler>(
     *     sampling_config::random_sampling(0.1)  // 10% sampling
     * );
     * logger->set_sampler(std::move(sampler));
     * @endcode
     *
     * @since 3.3.0
     */
    void set_sampler(std::unique_ptr<sampling::log_sampler> sampler);

    /**
     * @brief Get the log sampler (if set)
     * @return Pointer to sampler or nullptr if not set
     *
     * @since 3.3.0
     */
    [[nodiscard]] sampling::log_sampler* get_sampler();

    /**
     * @brief Get the log sampler (const version)
     * @return Pointer to sampler or nullptr if not set
     *
     * @since 3.3.0
     */
    [[nodiscard]] const sampling::log_sampler* get_sampler() const;

    /**
     * @brief Check if sampling is enabled
     * @return true if a sampler is set and enabled
     *
     * @since 3.3.0
     */
    [[nodiscard]] bool has_sampling() const;

    /**
     * @brief Get sampling statistics
     * @return Sampling statistics including sampled/dropped counts
     *
     * @since 3.3.0
     */
    [[nodiscard]] sampling::sampling_stats get_sampling_stats() const;

    /**
     * @brief Reset sampling statistics
     *
     * @since 3.3.0
     */
    void reset_sampling_stats();

    // Emergency Flush Support (critical_logger_interface implementation)

    /**
     * @brief Get file descriptor for emergency writing
     * @return File descriptor or -1 if not available
     *
     * @details This method provides a file descriptor for signal-safe emergency writing.
     * It must be signal-safe (no allocations, no locks).
     *
     * @since 2.0.0
     */
    int get_emergency_fd() const override;

    /**
     * @brief Get emergency buffer pointer
     * @return Pointer to buffer or nullptr
     *
     * @details This method must be signal-safe. The buffer contains pending log messages
     * that should be flushed in case of emergency shutdown.
     *
     * @since 2.0.0
     */
    const char* get_emergency_buffer() const override;

    /**
     * @brief Get emergency buffer size
     * @return Buffer size in bytes
     *
     * @details This method must be signal-safe.
     *
     * @since 2.0.0
     */
    size_t get_emergency_buffer_size() const override;

private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};

} // namespace kcenon::logger
