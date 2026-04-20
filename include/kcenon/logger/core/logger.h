// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#pragma once

/**
 * @file logger.h
 * @brief High-performance, thread-safe logging system with asynchronous capabilities
 * @author kcenon
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
 * logger.log(log_level::error, "An error occurred");
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
#include <kcenon/logger/interfaces/logger_types.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/log_writer_interface.h>
#include <kcenon/logger/logger_export.h>
#include <kcenon/logger/otlp/otel_context.h>
#include <kcenon/logger/sampling/sampling_config.h>
#include <kcenon/logger/security/signal_manager.h>
#include "structured_log_builder.h"
#include "unified_log_context.h"

namespace kcenon::logger {

// Type aliases - use common::interfaces types for standardization
using log_level = common::interfaces::log_level;
using health_status = logger_system::health_status;
using overflow_policy = logger_system::overflow_policy;

// Type aliases for convenience and compatibility
using logger_metrics = metrics::logger_performance_stats;
using performance_metrics = metrics::logger_performance_stats; // Alias for examples

/**
 * @enum metric_type
 * @brief Types of metrics that can be recorded by the logger.
 */
enum class metric_type {
    gauge,      ///< A metric that can go up or down (e.g., queue depth).
    counter,    ///< A monotonically increasing metric (e.g., total messages).
    histogram   ///< A distribution of values (e.g., processing latency).
};

// Forward declarations
class log_collector;
class base_writer;
class logger_metrics_collector;
class log_filter_interface;  // Forward declaration for filtering system

#ifdef LOGGER_WITH_ANALYSIS
namespace analysis {
class realtime_log_analyzer;
}  // namespace analysis
#endif  // LOGGER_WITH_ANALYSIS

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
class LOGGER_SYSTEM_API logger : public security::critical_logger_interface,
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

    // Legacy log(level, message, file, line, function) removed.
    // Use log(level, message, source_location::current()) instead.

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
    common::VoidResult add_writer(log_writer_ptr writer);

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
    common::VoidResult add_writer(const std::string& name, log_writer_ptr writer);
    
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
    log_writer_interface* get_writer(const std::string& name);
    
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
    // Real-time analysis (optional, requires LOGGER_WITH_ANALYSIS)
    // =========================================================================

#ifdef LOGGER_WITH_ANALYSIS
    /**
     * @brief Set real-time log analyzer for anomaly detection
     * @param analyzer The analyzer instance
     *
     * @details Sets a real-time analyzer that processes each log entry
     * for anomaly detection. The analyzer is invoked synchronously during
     * log processing.
     *
     * @note This API is only available when LOGGER_WITH_ANALYSIS is defined.
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
     * @note This API is only available when LOGGER_WITH_ANALYSIS is defined.
     *
     * @since 3.2.0
     */
    [[nodiscard]] analysis::realtime_log_analyzer* get_realtime_analyzer();

    /**
     * @brief Get the real-time analyzer (const version)
     * @return Pointer to analyzer or nullptr if not set
     *
     * @note This API is only available when LOGGER_WITH_ANALYSIS is defined.
     *
     * @since 3.2.0
     */
    [[nodiscard]] const analysis::realtime_log_analyzer* get_realtime_analyzer() const;

    /**
     * @brief Check if real-time analysis is enabled
     * @return true if a realtime analyzer is set
     *
     * @note This API is only available when LOGGER_WITH_ANALYSIS is defined.
     *
     * @since 3.2.0
     */
    [[nodiscard]] bool has_realtime_analysis() const;
#endif  // LOGGER_WITH_ANALYSIS

    // OpenTelemetry context: Use context().set_otel(), context().get_string("otel_trace_id"),
    // context().clear(context_category::otel), context().has("otel_trace_id") instead.

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
     * when emit() is called on the builder. Context fields (set via context().set())
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

    // =========================================================================
    // Unified Context API (NEW)
    // =========================================================================

    /**
     * @brief Get the unified context for this logger
     * @return Reference to the unified log context
     *
     * @details Provides access to the unified context management system that
     * consolidates all context types (custom fields, trace IDs, request IDs,
     * and OpenTelemetry context) into a single interface.
     *
     * @example
     * @code
     * // Set various context types
     * logger->context()
     *     .set("user_id", int64_t{12345})
     *     .set_trace("trace-abc", "span-123")
     *     .set_request("req-456");
     *
     * // All structured logs include the context
     * logger->log_structured(log_level::info)
     *     .message("Processing request")
     *     .emit();
     *
     * // Clear specific category
     * logger->context().clear(context_category::trace);
     * @endcode
     *
     * @since 3.5.0
     */
    [[nodiscard]] unified_log_context& context();

    /**
     * @brief Get the unified context for this logger (const version)
     * @return Const reference to the unified log context
     *
     * @since 3.5.0
     */
    [[nodiscard]] const unified_log_context& context() const;

    // Legacy context fields: Use context().set(key, value) and context().remove(key) instead.

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
