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
#include <kcenon/logger/security/signal_manager.h>
#include <kcenon/logger/otlp/otel_context.h>

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
