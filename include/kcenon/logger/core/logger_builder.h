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
// TODO: Re-enable when strategy pattern is implemented
// #include "config_strategy_interface.h"
// #include "configuration_templates.h"
#include "logger.h"
#include "../backends/integration_backend.h"
#include "../backends/standalone_backend.h"
#ifdef USE_THREAD_SYSTEM_INTEGRATION
    #include "../backends/thread_system_backend.h"
#endif
#include "../writers/base_writer.h"
#include "../writers/batch_writer.h"
// TODO: Implement filtering system
// #include "../filters/log_filter.h"
#include "../interfaces/log_formatter_interface.h"
#include "di/di_container_interface.h"

// Use common_system interfaces (Phase 2.2.4)
#ifdef BUILD_WITH_COMMON_SYSTEM
    #include <kcenon/common/interfaces/monitoring_interface.h>
#endif

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
#ifdef USE_THREAD_SYSTEM_INTEGRATION
    logger_builder& with_min_level(kcenon::thread::log_level level) {
#else
    logger_builder& with_min_level(logger_system::log_level level) {
#endif
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
    
    // TODO: Implement filtering system
    /**
     * @brief Add a filter to the logger
     * @param filter Filter instance
     * @return Reference to builder for chaining
     */
    /*
    logger_builder& add_filter(std::unique_ptr<log_filter> filter) {
        filters_.push_back(std::move(filter));
        return *this;
    }
    */
    
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
    
    // TODO: Re-enable when strategy pattern is implemented
    /*
    logger_builder& apply_strategy(std::unique_ptr<config_strategy_interface> strategy) {
        if (strategy) {
            strategies_.push_back(std::move(strategy));
        }
        return *this;
    }

    logger_builder& for_environment(const std::string& env) {
        auto strategy = config_strategy_factory::create_environment(env);
        if (strategy) {
            apply_strategy(std::move(strategy));
        }
        return *this;
    }

    logger_builder& with_performance_tuning(const std::string& level = "balanced") {
        auto strategy = config_strategy_factory::create_tuning(level);
        if (strategy) {
            apply_strategy(std::move(strategy));
        }
        return *this;
    }

    logger_builder& auto_configure() {
        auto strategy = config_strategy_factory::from_environment();
        if (strategy) {
            apply_strategy(std::move(strategy));
        }
        return *this;
    }
    */
    
    // TODO: Re-enable when configuration_template enum is implemented
    /*
    logger_builder& apply_template(kcenon::logger::configuration_template template_type) {
        auto template_config = kcenon::logger::get_template_config(template_type);
        config_.min_level = template_config.min_level;
        config_.buffer_size = template_config.buffer_size;
        config_.async = template_config.async_mode;
        config_.enable_batch_writing = template_config.enable_batch_writing;
        config_.batch_size = template_config.batch_size;
        config_.flush_interval = template_config.flush_interval;
        // Convert kcenon::logger::overflow_policy to logger_config::overflow_policy
        switch (template_config.overflow_policy_type) {
            case kcenon::logger::overflow_policy::block:
                config_.queue_overflow_policy = logger_config::overflow_policy::block;
                break;
            case kcenon::logger::overflow_policy::drop_oldest:
                config_.queue_overflow_policy = logger_config::overflow_policy::drop_oldest;
                break;
            case kcenon::logger::overflow_policy::drop_newest:
                config_.queue_overflow_policy = logger_config::overflow_policy::drop_newest;
                break;
            case kcenon::logger::overflow_policy::grow:
                config_.queue_overflow_policy = logger_config::overflow_policy::grow;
                break;
        }
        return *this;
    }

    logger_builder& apply_performance_strategy(kcenon::logger::performance_strategy strategy) {
        auto perf_config = kcenon::logger::get_performance_config(strategy);
        config_.min_level = perf_config.min_level;
        config_.buffer_size = perf_config.buffer_size;
        config_.async = perf_config.async_mode;
        config_.enable_batch_writing = perf_config.enable_batch_writing;
        config_.batch_size = perf_config.batch_size;
        config_.flush_interval = perf_config.flush_interval;
        // Convert kcenon::logger::overflow_policy to logger_config::overflow_policy
        switch (perf_config.overflow_policy_type) {
            case kcenon::logger::overflow_policy::block:
                config_.queue_overflow_policy = logger_config::overflow_policy::block;
                break;
            case kcenon::logger::overflow_policy::drop_oldest:
                config_.queue_overflow_policy = logger_config::overflow_policy::drop_oldest;
                break;
            case kcenon::logger::overflow_policy::drop_newest:
                config_.queue_overflow_policy = logger_config::overflow_policy::drop_newest;
                break;
            case kcenon::logger::overflow_policy::grow:
                config_.queue_overflow_policy = logger_config::overflow_policy::grow;
                break;
        }
        return *this;
    }
    */
    
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
#ifdef USE_THREAD_SYSTEM_INTEGRATION
            if (level_str == "trace") config_.min_level = kcenon::thread::log_level::trace;
            else if (level_str == "debug") config_.min_level = kcenon::thread::log_level::debug;
            else if (level_str == "info") config_.min_level = kcenon::thread::log_level::info;
            else if (level_str == "warn") config_.min_level = kcenon::thread::log_level::warning;
            else if (level_str == "error") config_.min_level = kcenon::thread::log_level::error;
            else if (level_str == "fatal") config_.min_level = kcenon::thread::log_level::error; // Note: fatal mapped to error
#else
            if (level_str == "trace") config_.min_level = logger_system::log_level::trace;
            else if (level_str == "debug") config_.min_level = logger_system::log_level::debug;
            else if (level_str == "info") config_.min_level = logger_system::log_level::info;
            else if (level_str == "warn") config_.min_level = logger_system::log_level::warn;
            else if (level_str == "error") config_.min_level = logger_system::log_level::error;
            else if (level_str == "fatal") config_.min_level = logger_system::log_level::fatal;
#endif
        }

        return *this;
    }

    /**
     * @brief Set monitoring interface (Phase 2.2.4)
     * @param monitor IMonitor implementation from common_system
     * @return Reference to builder for chaining
     */
#ifdef BUILD_WITH_COMMON_SYSTEM
    logger_builder& with_monitoring(std::shared_ptr<common::interfaces::IMonitor> monitor) {
        monitor_ = monitor;
        config_.enable_metrics = true;
        return *this;
    }
#endif
    
    /**
     * @brief Set health check interval
     * @param interval Health check interval
     * @return Reference to builder for chaining
     */
    logger_builder& with_health_check_interval(std::chrono::milliseconds interval) {
        health_check_interval_ = interval;
        return *this;
    }
    
    // TODO: Re-enable when DI pattern is fully implemented
    /*
    template<typename T>
    logger_builder& with_di_container(std::shared_ptr<di_container_interface<T>> container) {
        // Store for later use
        // Note: Implementation would need to handle type erasure or specific container type
        return *this;
    }

    logger_builder& with_writer_from_di(const std::string& name) {
        // Implementation would resolve from DI container
        return *this;
    }
    */
    
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
     * @brief Use thread_system backend explicitly
     * @return Reference to builder for chaining
     *
     * @details Explicitly selects the thread_system integration backend.
     * This method is only available when USE_THREAD_SYSTEM_INTEGRATION is defined.
     *
     * @throws std::runtime_error if thread_system is not available
     *
     * @since 1.2.0
     */
    logger_builder& with_thread_system_backend() {
#ifdef USE_THREAD_SYSTEM_INTEGRATION
        backend_ = std::make_unique<backends::thread_system_backend>();
#else
        throw std::runtime_error("thread_system backend not available (USE_THREAD_SYSTEM_INTEGRATION not defined)");
#endif
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

    // TODO: Re-enable when strategy pattern is implemented
    /*
    logger_builder& clear_strategies() {
        strategies_.clear();
        return *this;
    }
    */

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
        // TODO: Re-enable when strategy pattern is implemented
        /*
        // Apply all strategies first
        for (const auto& strategy : strategies_) {
            if (auto can_apply = strategy->can_apply(config_); !can_apply) {
                // Log warning but continue with other strategies
                continue;
            }

            if (auto result = strategy->apply(config_); !result) {
                return make_logger_error<std::unique_ptr<logger>>(
                    logger_error_code::invalid_configuration,
                    "Strategy application failed"
                );
            }
        }
        */

        // Validate configuration
        if (auto validation = config_.validate(); !validation) {
            return make_logger_error<std::unique_ptr<logger>>(
                logger_error_code::invalid_configuration,
                "Configuration validation failed"
            );
        }
        
        // Validate writer count
        if (!writers_.empty() && writers_.size() > config_.max_writers) {
            return make_logger_error<std::unique_ptr<logger>>(
                logger_error_code::invalid_configuration,
                "Number of writers exceeds max_writers configuration"
            );
        }
        
        // Auto-detect backend if not explicitly set
        if (!backend_) {
#ifdef USE_THREAD_SYSTEM_INTEGRATION
            backend_ = std::make_unique<backends::thread_system_backend>();
#else
            backend_ = std::make_unique<backends::standalone_backend>();
#endif
        }

        // Create logger with validated configuration
        auto logger_instance = std::make_unique<logger>(config_.async, config_.buffer_size, std::move(backend_));
        
        // Apply configuration settings
        logger_instance->set_min_level(config_.min_level);
        
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

        // TODO: Implement filtering system
        // Filter application disabled until log_filter is implemented
        /*
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
        */

        // Start logger if async
        if (config_.async) {
            logger_instance->start();
        }

        // Store configuration in logger (if we add a config getter)
        built_config_ = config_;

        // Return logger instance - move into result
        return std::move(logger_instance);
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
    result_void validate() const {
        return config_.validate();
    }
    
private:
    logger_config config_;
    std::vector<std::pair<std::string, std::unique_ptr<base_writer>>> writers_;
    // TODO: Implement filtering system
    // std::vector<std::unique_ptr<log_filter>> filters_;
    std::unique_ptr<log_formatter_interface> formatter_;
    std::unique_ptr<backends::integration_backend> backend_;  // Integration backend (Phase 3.2)
    // TODO: Re-enable when strategy pattern is implemented
    // std::vector<std::unique_ptr<config_strategy_interface>> strategies_;
    mutable logger_config built_config_;  // Store last built configuration
#ifdef BUILD_WITH_COMMON_SYSTEM
    std::shared_ptr<common::interfaces::IMonitor> monitor_;  // Phase 2.2.4
#endif
    std::chrono::milliseconds health_check_interval_{1000};
    std::function<void(const logger_error_code&)> error_handler_;
};

} // namespace kcenon::logger
