#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file logger_builder.h
 * @brief Builder pattern implementation for flexible logger configuration
 * @author 🍀☀🌕🌥 🌊
 * @since 1.0.0
 * 
 * @details This file provides a fluent interface for constructing logger instances
 * with validated configurations. The builder pattern allows for step-by-step
 * configuration with sensible defaults and automatic validation.
 * 
 * @note The builder ensures configuration consistency and applies optimizations
 * based on the selected settings (e.g., disabling batching in sync mode).
 * 
 * @example Basic logger creation:
 * @code
 * auto result = logger_builder()
 *     .with_async(true)
 *     .with_min_level(log_level::info)
 *     .add_writer("console", std::make_unique<console_writer>())
 *     .build();
 * 
 * if (result) {
 *     auto logger = std::move(result.value());
 *     // Use logger...
 * }
 * @endcode
 * 
 * @example Using configuration templates:
 * @code
 * // High-performance configuration
 * auto logger = logger_builder()
 *     .use_template("high_performance")
 *     .with_file_output("/var/log/app", "myapp")
 *     .build();
 * 
 * // Debug configuration with environment detection
 * auto logger = logger_builder()
 *     .detect_environment()
 *     .use_template("debug")
 *     .with_metrics(true)
 *     .build();
 * @endcode
 * 
 * @example Advanced configuration with strategies:
 * @code
 * auto logger = logger_builder()
 *     .for_environment("production")
 *     .with_performance_tuning("aggressive")
 *     .with_batch_writing(true)
 *     .with_overflow_policy(logger_config::overflow_policy::drop_oldest)
 *     .add_writer("file", std::make_unique<rotating_file_writer>())
 *     .add_writer("network", std::make_unique<network_writer>())
 *     .with_monitoring(std::make_shared<prometheus_monitor>())
 *     .build();
 * @endcode
 */

#include "thread_integration_detector.h"
#include "logger_config.h"
#include "strategies/config_strategy_interface.h"
#include "strategies/environment_strategy.h"
#include "strategies/performance_strategy.h"
#include "strategies/deployment_strategy.h"
#include "strategies/composite_strategy.h"
#include "logger.h"
#include "../backends/integration_backend.h"
#include "../backends/standalone_backend.h"
#include "../writers/base_writer.h"
#include "../writers/batch_writer.h"
#include "../filters/log_filter.h"
#include "../interfaces/log_formatter_interface.h"
#include "../routing/log_router.h"
#include "../analysis/realtime_log_analyzer.h"
#include "../sampling/log_sampler.h"

// Use common_system interfaces (Phase 2.2.4)
#include <kcenon/common/interfaces/monitoring_interface.h>

#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>

namespace kcenon::logger {

/**
 * @class logger_builder
 * @brief Builder pattern for logger construction with validation
 * 
 * @details The logger_builder class provides a fluent interface for constructing
 * logger instances with complex configurations. It ensures configuration validity,
 * applies optimization strategies, and handles the complexity of logger initialization.
 * 
 * Key features:
 * - Fluent interface for intuitive configuration
 * - Automatic validation of configuration parameters
 * - Support for configuration templates and strategies
 * - Environment-based auto-configuration
 * - Integration with dependency injection containers
 * - Performance tuning presets
 * 
 * @note All builder methods return a reference to the builder for method chaining.
 * 
 * @warning The build() method consumes writers and filters. After calling build(),
 * the builder should not be reused without adding new writers.
 * 
 * @since 1.0.0
 */
class logger_builder {
public:
    logger_builder() : config_(logger_config::default_config()) {}
    
    /**
     * @brief Start with a predefined configuration
     * @param config Base configuration to use
     * @return Reference to builder for chaining
     * 
     * @details Replaces the current configuration with the provided one.
     * Useful for starting with a known good configuration and making adjustments.
     * 
     * @example
     * @code
     * logger_config custom_config = logger_config::high_performance();
     * auto logger = logger_builder()
     *     .with_config(custom_config)
     *     .with_min_level(log_level::debug)  // Override specific setting
     *     .build();
     * @endcode
     * 
     * @since 1.0.0
     */
    logger_builder& with_config(const logger_config& config) {
        config_ = config;
        return *this;
    }
    
    /**
     * @brief Set async mode
     * @param async Enable (true) or disable (false) async logging
     * @return Reference to builder for chaining
     * 
     * @details Configures whether logging operations are performed asynchronously.
     * Async mode provides better performance but may lose messages on crash.
     * Sync mode ensures all messages are written immediately but may impact performance.
     * 
     * @note When disabling async mode, batch_size is automatically set to 1.
     * 
     * @example
     * @code
     * // High-throughput application
     * builder.with_async(true);
     * 
     * // Critical logging that must not lose messages
     * builder.with_async(false);
     * @endcode
     * 
     * @since 1.0.0
     */
    logger_builder& with_async(bool async = true) {
        config_.async = async;
        // When disabling async, ensure batch_size is 1
        if (!async && config_.batch_size > 1) {
            config_.batch_size = 1;
        }
        return *this;
    }
    
    /**
     * @brief Set buffer size
     * @param size Buffer size in bytes
     * @return Reference to builder for chaining
     * 
     * @details Configures the internal buffer size for log message queuing.
     * Larger buffers can improve throughput but increase memory usage.
     * 
     * @note Recommended sizes:
     * - Small applications: 4096-8192 bytes
     * - Medium applications: 16384-32768 bytes
     * - High-throughput applications: 65536+ bytes
     * 
     * @warning Very large buffers may cause memory pressure in constrained environments.
     * 
     * @since 1.0.0
     */
    logger_builder& with_buffer_size(std::size_t size) {
        config_.buffer_size = size;
        return *this;
    }
    
    /**
     * @brief Set minimum log level
     * @param level Minimum level to log
     * @return Reference to builder for chaining
     *
     * @details Sets the threshold for message logging. Messages below this level
     * are discarded at the earliest opportunity for maximum performance.
     *
     * @example
     * @code
     * // Production: only important messages
     * builder.with_min_level(log_level::warning);
     *
     * // Development: all messages
     * builder.with_min_level(log_level::trace);
     * @endcode
     *
     * @since 1.0.0
     */
    logger_builder& with_min_level(log_level level) {
        config_.min_level = level;
        return *this;
    }

    /**
     * @brief Set batch size for processing
     * @param size Batch size
     * @return Reference to builder for chaining
     */
    logger_builder& with_batch_size(std::size_t size) {
        config_.batch_size = size;
        return *this;
    }
    
    /**
     * @brief Enable batch writing for writers
     * @param enable Enable/disable batch writing
     * @return Reference to builder for chaining
     */
    logger_builder& with_batch_writing(bool enable = true) {
        config_.enable_batch_writing = enable;
        return *this;
    }
    
    /**
     * @brief Set flush interval
     * @param interval Flush interval
     * @return Reference to builder for chaining
     */
    logger_builder& with_flush_interval(std::chrono::milliseconds interval) {
        config_.flush_interval = interval;
        return *this;
    }
    
    /**
     * @brief Enable lock-free queue
     * @param enable Enable/disable lock-free mode
     * @return Reference to builder for chaining
     */
    logger_builder& with_lock_free(bool enable = true) {
        config_.use_lock_free = enable;
        return *this;
    }
    
    /**
     * @brief Enable metrics collection
     * @param enable Enable/disable metrics
     * @return Reference to builder for chaining
     */
    logger_builder& with_metrics(bool enable = true) {
        config_.enable_metrics = enable;
        return *this;
    }
    
    /**
     * @brief Enable crash handler
     * @param enable Enable/disable crash handler
     * @return Reference to builder for chaining
     */
    logger_builder& with_crash_handler(bool enable = true) {
        config_.enable_crash_handler = enable;
        return *this;
    }
    
    /**
     * @brief Enable structured logging
     * @param enable Enable/disable structured logging
     * @return Reference to builder for chaining
     */
    logger_builder& with_structured_logging(bool enable = true) {
        config_.enable_structured_logging = enable;
        return *this;
    }
    
    /**
     * @brief Set queue overflow policy
     * @param policy Overflow policy to use
     * @return Reference to builder for chaining
     */
    logger_builder& with_overflow_policy(logger_config::overflow_policy policy) {
        config_.queue_overflow_policy = policy;
        return *this;
    }
    
    /**
     * @brief Set max queue size
     * @param size Maximum queue size
     * @return Reference to builder for chaining
     */
    logger_builder& with_max_queue_size(std::size_t size) {
        config_.max_queue_size = size;
        return *this;
    }
    
    /**
     * @brief Configure file output
     * @param directory Log directory
     * @param prefix File prefix
     * @param max_size Maximum file size
     * @param max_count Maximum file count for rotation
     * @return Reference to builder for chaining
     */
    logger_builder& with_file_output(const std::string& directory,
                                    const std::string& prefix = "app",
                                    std::size_t max_size = 100 * 1024 * 1024,
                                    std::size_t max_count = 5) {
        config_.log_directory = directory;
        config_.log_file_prefix = prefix;
        config_.max_file_size = max_size;
        config_.max_file_count = max_count;
        return *this;
    }
    
    /**
     * @brief Configure network output
     * @param host Remote host
     * @param port Remote port
     * @param timeout Connection timeout
     * @return Reference to builder for chaining
     */
    logger_builder& with_network_output(const std::string& host,
                                       uint16_t port,
                                       std::chrono::milliseconds timeout = std::chrono::milliseconds(5000)) {
        config_.remote_host = host;
        config_.remote_port = port;
        config_.network_timeout = timeout;
        return *this;
    }
    
    /**
     * @brief Add a writer to the logger
     * @param name Writer name
     * @param writer Writer instance
     * @return Reference to builder for chaining
     */
    logger_builder& add_writer(const std::string& name, std::unique_ptr<base_writer> writer) {
        writers_.push_back({name, std::move(writer)});
        return *this;
    }
    
    /**
     * @brief Add a filter to the logger
     * @param filter Filter instance
     * @return Reference to builder for chaining
     *
     * @details Adds a filter that determines whether log entries should be processed.
     * Multiple filters can be added; they are combined using AND logic (all must pass).
     *
     * @example
     * @code
     * auto logger = logger_builder()
     *     .add_filter(std::make_unique<filters::level_filter>(log_level::info))
     *     .add_filter(std::make_unique<filters::regex_filter>("error|warning"))
     *     .build();
     * @endcode
     *
     * @since 2.0.0
     */
    logger_builder& add_filter(std::unique_ptr<log_filter_interface> filter) {
        if (filter) {
            filters_.push_back(std::move(filter));
        }
        return *this;
    }

    /**
     * @brief Add a level filter (convenience method)
     * @param min_level Minimum log level to pass through
     * @return Reference to builder for chaining
     *
     * @since 2.0.0
     */
    logger_builder& add_level_filter(log_level min_level) {
        filters_.push_back(std::make_unique<filters::level_filter>(min_level));
        return *this;
    }

    /**
     * @brief Add a regex filter (convenience method)
     * @param pattern Regex pattern to match against log messages
     * @param include_matches If true, only matching messages pass; if false, non-matching pass
     * @return Reference to builder for chaining
     *
     * @since 2.0.0
     */
    logger_builder& add_regex_filter(const std::string& pattern, bool include_matches = true) {
        filters_.push_back(std::make_unique<filters::regex_filter>(pattern, include_matches));
        return *this;
    }

    /**
     * @brief Add a function-based filter (convenience method)
     * @param predicate Function that returns true for entries that should pass
     * @return Reference to builder for chaining
     *
     * @since 2.0.0
     */
    logger_builder& add_function_filter(std::function<bool(const log_entry&)> predicate) {
        filters_.push_back(std::make_unique<filters::function_filter>(std::move(predicate)));
        return *this;
    }

    // =========================================================================
    // Routing configuration
    // =========================================================================

    /**
     * @brief Add a routing rule to the logger
     * @param config Route configuration
     * @return Reference to builder for chaining
     *
     * @details Routes determine which writers receive specific log messages
     * based on configurable rules (level, pattern matching, etc.).
     *
     * @example
     * @code
     * auto logger = logger_builder()
     *     .add_writer("console", std::make_unique<console_writer>())
     *     .add_writer("errors", std::make_unique<file_writer>("errors.log"))
     *     .with_route(routing::route_config{
     *         .writer_names = {"errors"},
     *         .filter = std::make_unique<filters::level_filter>(log_level::error),
     *         .stop_propagation = false
     *     })
     *     .build();
     * @endcode
     *
     * @since 2.0.0
     */
    logger_builder& with_route(routing::route_config config) {
        routes_.push_back(std::move(config));
        return *this;
    }

    /**
     * @brief Set exclusive routing mode
     * @param exclusive If true, only matched routes receive logs; otherwise all writers receive
     * @return Reference to builder for chaining
     *
     * @details In exclusive mode, only writers matched by routing rules receive log messages.
     * In non-exclusive mode (default), all writers receive messages regardless of routing.
     *
     * @since 2.0.0
     */
    logger_builder& with_exclusive_routing(bool exclusive = true) {
        exclusive_routing_ = exclusive;
        return *this;
    }

    /**
     * @brief Add a level-based route (convenience method)
     * @param level Target log level
     * @param writer_names Writers to route to
     * @param stop_propagation Stop further rule evaluation after match
     * @return Reference to builder for chaining
     *
     * @example
     * @code
     * auto logger = logger_builder()
     *     .add_writer("console", std::make_unique<console_writer>())
     *     .add_writer("errors", std::make_unique<file_writer>("errors.log"))
     *     .route_level(log_level::error, {"errors"})
     *     .build();
     * @endcode
     *
     * @since 2.0.0
     */
    logger_builder& route_level(log_level level,
                                const std::vector<std::string>& writer_names,
                                bool stop_propagation = false) {
        routing::route_config config;
        config.writer_names = writer_names;
        config.filter = std::make_unique<filters::level_filter>(level);
        config.stop_propagation = stop_propagation;
        routes_.push_back(std::move(config));
        return *this;
    }

    /**
     * @brief Add a pattern-based route (convenience method)
     * @param pattern Regex pattern to match against log messages
     * @param writer_names Writers to route to
     * @param stop_propagation Stop further rule evaluation after match
     * @return Reference to builder for chaining
     *
     * @example
     * @code
     * auto logger = logger_builder()
     *     .add_writer("console", std::make_unique<console_writer>())
     *     .add_writer("security", std::make_unique<file_writer>("security.log"))
     *     .route_pattern("security|auth", {"security"})
     *     .build();
     * @endcode
     *
     * @since 2.0.0
     */
    logger_builder& route_pattern(const std::string& pattern,
                                  const std::vector<std::string>& writer_names,
                                  bool stop_propagation = false) {
        routing::route_config config;
        config.writer_names = writer_names;
        config.filter = std::make_unique<filters::regex_filter>(pattern, true);
        config.stop_propagation = stop_propagation;
        routes_.push_back(std::move(config));
        return *this;
    }

    /**
     * @brief Set formatter for the logger
     * @param formatter Formatter instance
     * @return Reference to builder for chaining
     */
    logger_builder& with_formatter(std::unique_ptr<log_formatter_interface> formatter) {
        formatter_ = std::move(formatter);
        return *this;
    }
    
    /**
     * @brief Use a predefined configuration template
     * @param name Template name ("default", "high_performance", "low_latency", "debug", "production")
     * @return Reference to builder for chaining
     * 
     * @details Applies a pre-configured template optimized for specific use cases:
     * - "default": Balanced configuration for general use
     * - "high_performance": Optimized for maximum throughput
     * - "low_latency": Minimized processing delay
     * - "debug": Verbose logging for development
     * - "production": Optimized for production environments
     * 
     * @note Templates can be further customized with additional builder methods.
     * 
     * @example
     * @code
     * // Start with high-performance template and customize
     * auto logger = logger_builder()
     *     .use_template("high_performance")
     *     .with_min_level(log_level::info)  // Override template's level
     *     .add_writer("custom", std::make_unique<custom_writer>())
     *     .build();
     * @endcode
     * 
     * @since 1.0.0
     */
    logger_builder& use_template(const std::string& name) {
        // Use built-in logger_config templates
        if (name == "high_performance") {
            config_ = logger_config::high_performance();
        } else if (name == "low_latency") {
            config_ = logger_config::low_latency();
        } else if (name == "debug") {
            config_ = logger_config::debug_config();
        } else if (name == "production") {
            config_ = logger_config::production();
        } else {
            config_ = logger_config::default_config();
        }
        return *this;
    }
    
    /**
     * @brief Apply a configuration strategy
     * @param strategy Strategy to apply during build
     * @return Reference to builder for chaining
     *
     * @details Strategies are applied in priority order during build().
     * Higher priority strategies are applied first.
     *
     * @since 2.0.0
     */
    logger_builder& apply_strategy(std::unique_ptr<config_strategy_interface> strategy) {
        if (strategy) {
            strategies_.push_back(std::move(strategy));
        }
        return *this;
    }

    /**
     * @brief Configure for a specific deployment environment
     * @param env Deployment environment type
     * @return Reference to builder for chaining
     *
     * @details Applies pre-configured settings optimized for the environment:
     * - development: Verbose, colored, synchronous
     * - staging: JSON, file + console, rotation
     * - production: Optimized, warning+, crash protection
     * - testing: Memory buffer, synchronous
     *
     * @since 2.0.0
     */
    logger_builder& for_environment(deployment_env env) {
        return apply_strategy(std::make_unique<deployment_strategy>(env));
    }

    /**
     * @brief Apply performance tuning
     * @param level Performance tuning level
     * @return Reference to builder for chaining
     *
     * @details Applies performance-optimized settings:
     * - low_latency: Small buffers, immediate flush
     * - balanced: Default balanced settings
     * - high_throughput: Large buffers, batch processing
     * - minimal_overhead: Reduced features, lower CPU usage
     *
     * @since 2.0.0
     */
    logger_builder& with_performance_tuning(performance_level level) {
        return apply_strategy(std::make_unique<performance_strategy>(level));
    }

    /**
     * @brief Auto-configure from environment variables
     * @return Reference to builder for chaining
     *
     * @details Reads LOG_* environment variables and applies them:
     * - LOG_LEVEL, LOG_ASYNC, LOG_BUFFER_SIZE, LOG_BATCH_SIZE
     * - LOG_FLUSH_INTERVAL, LOG_COLOR, LOG_METRICS
     *
     * @since 2.0.0
     */
    logger_builder& auto_configure() {
        auto strategy = std::make_unique<environment_strategy>();
        if (strategy->is_applicable()) {
            apply_strategy(std::move(strategy));
        }
        return *this;
    }

    /**
     * @brief Clear all registered strategies
     * @return Reference to builder for chaining
     *
     * @since 2.0.0
     */
    logger_builder& clear_strategies() {
        strategies_.clear();
        return *this;
    }
    
    
    /**
     * @brief Detect environment from environment variables
     * @return Reference to builder for chaining
     */
    logger_builder& detect_environment() {
        const char* env = std::getenv("LOG_ENV");
        const char* level = std::getenv("LOG_LEVEL");

        if (env) {
            std::string env_str(env);
            if (env_str == "production") {
                use_template("production");
            } else if (env_str == "debug" || env_str == "development") {
                use_template("debug");
            }
        }

        if (level) {
            std::string level_str(level);
            if (level_str == "trace") config_.min_level = log_level::trace;
            else if (level_str == "debug") config_.min_level = log_level::debug;
            else if (level_str == "info") config_.min_level = log_level::info;
            else if (level_str == "warn") config_.min_level = log_level::warning;
            else if (level_str == "error") config_.min_level = log_level::error;
            else if (level_str == "fatal") config_.min_level = log_level::critical;
        }

        return *this;
    }

    /**
     * @brief Set monitoring interface (Phase 2.2.4)
     * @param monitor IMonitor implementation from common_system
     * @return Reference to builder for chaining
     */
    logger_builder& with_monitoring(std::shared_ptr<common::interfaces::IMonitor> monitor) {
        monitor_ = monitor;
        config_.enable_metrics = true;
        return *this;
    }

    /**
     * @brief Set health check interval
     * @param interval Health check interval
     * @return Reference to builder for chaining
     */
    logger_builder& with_health_check_interval(std::chrono::milliseconds interval) {
        health_check_interval_ = interval;
        return *this;
    }
    
    
    /**
     * @brief Set integration backend explicitly
     * @param backend Custom integration backend
     * @return Reference to builder for chaining
     *
     * @details Allows setting a custom integration backend instead of using
     * the auto-detected one. Useful for testing or custom integration scenarios.
     *
     * @since 1.2.0
     */
    logger_builder& with_backend(std::unique_ptr<backends::integration_backend> backend) {
        backend_ = std::move(backend);
        return *this;
    }

    /**
     * @brief Use standalone backend explicitly
     * @return Reference to builder for chaining
     *
     * @details Explicitly selects the standalone integration backend.
     * This is the default backend when no external integration is required.
     *
     * @since 1.2.0
     */
    logger_builder& with_standalone_backend() {
        backend_ = std::make_unique<backends::standalone_backend>();
        return *this;
    }

    /**
     * @brief Set error handler
     * @param handler Error handler function
     * @return Reference to builder for chaining
     */
    logger_builder& with_error_handler(std::function<void(const logger_error_code&)> handler) {
        error_handler_ = handler;
        return *this;
    }

    // =========================================================================
    // Real-time analysis configuration (requires LOGGER_WITH_ANALYSIS)
    // =========================================================================

#ifdef LOGGER_WITH_ANALYSIS
    /**
     * @brief Set real-time log analyzer
     * @param analyzer Pre-configured analyzer instance
     * @return Reference to builder for chaining
     *
     * @details Sets a real-time analyzer that will be attached to the logger
     * for anomaly detection during log processing.
     *
     * @note This API is only available when LOGGER_WITH_ANALYSIS is defined.
     *
     * @example
     * @code
     * auto analyzer = std::make_unique<realtime_log_analyzer>();
     * analyzer->set_error_spike_threshold(50);
     * analyzer->add_pattern_alert("OutOfMemory", log_level::error);
     *
     * auto logger = logger_builder()
     *     .with_realtime_analyzer(std::move(analyzer))
     *     .build();
     * @endcode
     *
     * @since 3.2.0
     */
    logger_builder& with_realtime_analyzer(
        std::unique_ptr<analysis::realtime_log_analyzer> analyzer) {
        realtime_analyzer_ = std::move(analyzer);
        return *this;
    }

    /**
     * @brief Configure real-time analysis with settings
     * @param config Analysis configuration
     * @param callback Optional anomaly callback
     * @return Reference to builder for chaining
     *
     * @details Creates and configures a real-time analyzer with the provided
     * settings. This is a convenience method for common use cases.
     *
     * @note This API is only available when LOGGER_WITH_ANALYSIS is defined.
     *
     * @example
     * @code
     * auto logger = logger_builder()
     *     .with_realtime_analysis(
     *         analysis::realtime_analysis_config{
     *             .error_spike_threshold = 100,
     *             .track_new_errors = true
     *         },
     *         [](const analysis::anomaly_event& e) {
     *             alert_team(e);
     *         })
     *     .build();
     * @endcode
     *
     * @since 3.2.0
     */
    logger_builder& with_realtime_analysis(
        const analysis::realtime_analysis_config& config,
        analysis::realtime_log_analyzer::anomaly_callback callback = nullptr) {
        realtime_analyzer_ = std::make_unique<analysis::realtime_log_analyzer>(config);
        if (callback) {
            realtime_analyzer_->set_anomaly_callback(std::move(callback));
        }
        return *this;
    }

    /**
     * @brief Enable basic real-time analysis with default settings
     * @param error_threshold Error spike threshold (errors per minute)
     * @param callback Optional anomaly callback
     * @return Reference to builder for chaining
     *
     * @details Creates a real-time analyzer with sensible defaults.
     *
     * @note This API is only available when LOGGER_WITH_ANALYSIS is defined.
     *
     * @since 3.2.0
     */
    logger_builder& with_realtime_analysis(
        size_t error_threshold = 50,
        analysis::realtime_log_analyzer::anomaly_callback callback = nullptr) {
        realtime_analyzer_ = analysis::realtime_analyzer_factory::create_production(
            error_threshold, std::move(callback));
        return *this;
    }
#endif  // LOGGER_WITH_ANALYSIS

    /**
     * @brief Use default pattern for logging
     * @return Reference to builder for chaining
     */
    logger_builder& with_default_pattern() {
        // Set a default format pattern
        config_.enable_timestamp = true;
        config_.enable_source_location = true;
        return *this;
    }

    // =========================================================================
    // Sampling configuration
    // =========================================================================

    /**
     * @brief Set log sampler for volume reduction
     * @param sampler Pre-configured sampler instance
     * @return Reference to builder for chaining
     *
     * @details Sets a sampler that determines which logs are passed through
     * based on configured sampling strategies.
     *
     * @example
     * @code
     * auto sampler = std::make_unique<sampling::log_sampler>(
     *     sampling::sampling_config::random_sampling(0.1)
     * );
     * auto logger = logger_builder()
     *     .with_sampler(std::move(sampler))
     *     .build();
     * @endcode
     *
     * @since 3.3.0
     */
    logger_builder& with_sampler(std::unique_ptr<sampling::log_sampler> sampler) {
        sampler_ = std::move(sampler);
        return *this;
    }

    /**
     * @brief Configure sampling with a config struct
     * @param config Sampling configuration
     * @return Reference to builder for chaining
     *
     * @example
     * @code
     * auto logger = logger_builder()
     *     .with_sampling(sampling::sampling_config{
     *         .enabled = true,
     *         .rate = 0.1,
     *         .always_log_levels = {log_level::error, log_level::fatal}
     *     })
     *     .build();
     * @endcode
     *
     * @since 3.3.0
     */
    logger_builder& with_sampling(const sampling::sampling_config& config) {
        sampler_ = std::make_unique<sampling::log_sampler>(config);
        return *this;
    }

    /**
     * @brief Enable random sampling with specified rate
     * @param rate Sampling rate (0.0 to 1.0)
     * @param always_log_levels Levels that bypass sampling
     * @return Reference to builder for chaining
     *
     * @deprecated Use with_sampling(sampling_config::random_sampling(rate).with_always_log(levels)) instead
     *
     * @example
     * @code
     * auto logger = logger_builder()
     *     .with_sampling(sampling::sampling_config::random_sampling(0.1))
     *     .build();
     * @endcode
     *
     * @since 3.3.0
     */
    [[deprecated("Use with_sampling(sampling_config::random_sampling(rate).with_always_log(levels)) instead")]]
    logger_builder& with_random_sampling(
        double rate,
        std::vector<log_level> always_log_levels = {
            log_level::error,
            log_level::critical
        }) {
        return with_sampling(
            sampling::sampling_config::random_sampling(rate)
                .with_always_log(std::move(always_log_levels)));
    }

    /**
     * @brief Enable rate limiting sampling
     * @param max_per_second Maximum logs per second
     * @param always_log_levels Levels that bypass sampling
     * @return Reference to builder for chaining
     *
     * @deprecated Use with_sampling(sampling_config::rate_limited(max_per_second).with_always_log(levels)) instead
     *
     * @example
     * @code
     * auto logger = logger_builder()
     *     .with_sampling(sampling::sampling_config::rate_limited(1000))
     *     .build();
     * @endcode
     *
     * @since 3.3.0
     */
    [[deprecated("Use with_sampling(sampling_config::rate_limited(max_per_second).with_always_log(levels)) instead")]]
    logger_builder& with_rate_limiting(
        std::size_t max_per_second,
        std::vector<log_level> always_log_levels = {
            log_level::error,
            log_level::critical
        }) {
        return with_sampling(
            sampling::sampling_config::rate_limited(max_per_second)
                .with_always_log(std::move(always_log_levels)));
    }

    /**
     * @brief Enable adaptive sampling
     * @param threshold Messages/second threshold to trigger adaptation
     * @param min_rate Minimum sampling rate under load
     * @param always_log_levels Levels that bypass sampling
     * @return Reference to builder for chaining
     *
     * @deprecated Use with_sampling(sampling_config::adaptive(threshold, min_rate).with_always_log(levels)) instead
     *
     * @example
     * @code
     * auto logger = logger_builder()
     *     .with_sampling(sampling::sampling_config::adaptive(50000, 0.01))
     *     .build();
     * @endcode
     *
     * @since 3.3.0
     */
    [[deprecated("Use with_sampling(sampling_config::adaptive(threshold, min_rate).with_always_log(levels)) instead")]]
    logger_builder& with_adaptive_sampling(
        std::size_t threshold = 10000,
        double min_rate = 0.01,
        std::vector<log_level> always_log_levels = {
            log_level::warning,
            log_level::error,
            log_level::critical
        }) {
        return with_sampling(
            sampling::sampling_config::adaptive(threshold, min_rate)
                .with_always_log(std::move(always_log_levels)));
    }

    /**
     * @brief Build the logger with validation
     * @return Result containing the logger or error
     * 
     * @details Validates the configuration and constructs the logger instance.
     * This method performs the following steps:
     * 1. Applies all registered configuration strategies
     * 2. Validates the final configuration
     * 3. Creates the logger instance
     * 4. Configures writers with appropriate wrappers (batching, etc.)
     * 5. Applies filters and formatters
     * 6. Starts the logger if in async mode
     * 
     * @note This method consumes writers and filters. The builder should not
     * be reused after calling build() without adding new writers.
     * 
     * @warning If validation fails, an error result is returned with details
     * about the configuration problem.
     * 
     * @example
     * @code
     * auto result = builder.build();
     * if (!result) {
     *     std::cerr << "Failed to build logger: " 
     *               << result.error_message() << std::endl;
     *     return;
     * }
     * auto logger = std::move(result.value());
     * @endcode
     * 
     * @since 1.0.0
     */
    result<std::unique_ptr<logger>> build() {
        // Apply all strategies in priority order (highest first)
        if (!strategies_.empty()) {
            // Sort strategies by priority (descending)
            std::sort(strategies_.begin(), strategies_.end(),
                [](const auto& a, const auto& b) {
                    return a->priority() > b->priority();
                });

            // Apply each applicable strategy
            for (const auto& strategy : strategies_) {
                if (strategy->is_applicable()) {
                    strategy->apply(config_);
                }
            }
        }

        // Validate configuration
        if (auto validation = config_.validate(); validation.is_err()) {
            return result<std::unique_ptr<logger>>{
                logger_error_code::invalid_configuration,
                "Configuration validation failed"};
        }

        // Validate writer count
        if (!writers_.empty() && writers_.size() > config_.max_writers) {
            return result<std::unique_ptr<logger>>{
                logger_error_code::invalid_configuration,
                "Number of writers exceeds max_writers configuration"};
        }

        // Auto-detect backend if not explicitly set
        // Users can provide thread_system_backend or other backends via with_backend()
        if (!backend_) {
            backend_ = std::make_unique<backends::standalone_backend>();
        }

        // Create logger with validated configuration
        auto logger_instance = std::make_unique<logger>(config_.async, config_.buffer_size, std::move(backend_));

        // Apply configuration settings
        logger_instance->set_level(config_.min_level);
        
        if (config_.enable_metrics) {
            logger_instance->enable_metrics_collection(true);
        }
        
        // Add writers (apply batch writing if enabled)
        for (auto& [name, writer] : writers_) {
            if (writer) {
                writer->set_use_color(config_.enable_color_output);
                
                // Wrap with batch writer if enabled and async mode
                if (config_.enable_batch_writing && config_.async) {
                    batch_writer::config batch_cfg;
                    batch_cfg.max_batch_size = config_.batch_size;
                    batch_cfg.flush_interval = config_.flush_interval;
                    
                    auto batch_wrapped = std::make_unique<batch_writer>(
                        std::move(writer), batch_cfg
                    );
                    logger_instance->add_writer(name, std::move(batch_wrapped));
                } else {
                    logger_instance->add_writer(name, std::move(writer));
                }
            }
        }

        // Apply filters if any were added
        if (!filters_.empty()) {
            // Create composite filter if multiple filters
            if (filters_.size() == 1) {
                logger_instance->set_filter(std::move(filters_[0]));
            } else {
                auto composite = std::make_unique<filters::composite_filter>(filters::composite_filter::logic_type::AND);
                for (auto& filter : filters_) {
                    composite->add_filter(std::move(filter));
                }
                logger_instance->set_filter(std::move(composite));
            }
        }
        filters_.clear();  // Clear after moving

        // Apply routing configuration
        if (!routes_.empty()) {
            auto& router = logger_instance->get_router();
            router.set_exclusive_routes(exclusive_routing_);
            for (auto& route : routes_) {
                router.add_route(std::move(route));
            }
        }
        routes_.clear();  // Clear after moving

        // Apply real-time analyzer if configured
#ifdef LOGGER_WITH_ANALYSIS
        if (realtime_analyzer_) {
            logger_instance->set_realtime_analyzer(std::move(realtime_analyzer_));
        }
#endif  // LOGGER_WITH_ANALYSIS

        // Apply sampler if configured
        if (sampler_) {
            logger_instance->set_sampler(std::move(sampler_));
        }

        // Start logger if async
        if (config_.async) {
            logger_instance->start();
        }

        // Store configuration in logger (if we add a config getter)
        built_config_ = config_;

        // Return logger instance (NRVO will be applied automatically)
        return logger_instance;
    }
    
    /**
     * @brief Get the current configuration (for inspection)
     * @return Current configuration
     */
    const logger_config& get_config() const {
        return config_;
    }
    
    /**
     * @brief Validate current configuration without building
     * @return Validation result
     */
    common::VoidResult validate() const {
        return config_.validate();
    }
    
private:
    logger_config config_;
    std::vector<std::pair<std::string, std::unique_ptr<base_writer>>> writers_;
    std::vector<std::unique_ptr<log_filter_interface>> filters_;
    std::vector<routing::route_config> routes_;  // Routing configurations
    bool exclusive_routing_ = false;  // Exclusive routing mode flag
    std::unique_ptr<log_formatter_interface> formatter_;
    std::unique_ptr<backends::integration_backend> backend_;  // Integration backend (Phase 3.2)
    std::vector<std::unique_ptr<config_strategy_interface>> strategies_;  // Configuration strategies
    mutable logger_config built_config_;  // Store last built configuration
    std::shared_ptr<common::interfaces::IMonitor> monitor_;  // Phase 2.2.4
    std::chrono::milliseconds health_check_interval_{1000};
    std::function<void(const logger_error_code&)> error_handler_;
#ifdef LOGGER_WITH_ANALYSIS
    std::unique_ptr<analysis::realtime_log_analyzer> realtime_analyzer_;  // Real-time analyzer
#endif  // LOGGER_WITH_ANALYSIS
    std::unique_ptr<sampling::log_sampler> sampler_;  // Log sampler for volume reduction
};

} // namespace kcenon::logger
