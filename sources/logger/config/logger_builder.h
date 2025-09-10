#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "logger_config.h"
#include "config_strategy_interface.h"
#include "configuration_templates.h"
#include "../logger.h"
#include "../writers/base_writer.h"
#include "../writers/batch_writer.h"
#include "../filters/log_filter.h"
#include "../interfaces/log_formatter_interface.h"
#include "../di/di_container_interface.h"
#include "../monitoring/monitoring_interface.h"
#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>

namespace logger_module {

/**
 * @class logger_builder
 * @brief Builder pattern for logger construction with validation
 * 
 * This class provides a fluent interface for constructing loggers
 * with validated configuration and proper initialization.
 */
class logger_builder {
public:
    logger_builder() : config_(logger_config::default_config()) {}
    
    /**
     * @brief Start with a predefined configuration
     * @param config Base configuration to use
     * @return Reference to builder for chaining
     */
    logger_builder& with_config(const logger_config& config) {
        config_ = config;
        return *this;
    }
    
    /**
     * @brief Set async mode
     * @param async Enable/disable async logging
     * @return Reference to builder for chaining
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
     */
    logger_builder& with_buffer_size(std::size_t size) {
        config_.buffer_size = size;
        return *this;
    }
    
    /**
     * @brief Set minimum log level
     * @param level Minimum level to log
     * @return Reference to builder for chaining
     */
    logger_builder& with_min_level(thread_module::log_level level) {
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
     */
    logger_builder& add_filter(std::unique_ptr<log_filter> filter) {
        filters_.push_back(std::move(filter));
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
     */
    logger_builder& use_template(const std::string& name) {
        auto strategy = config_strategy_factory::create_template(name);
        if (strategy) {
            apply_strategy(std::move(strategy));
        } else {
            // Fallback to old behavior for backward compatibility
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
        }
        return *this;
    }
    
    /**
     * @brief Apply a configuration strategy
     * @param strategy Strategy to apply
     * @return Reference to builder for chaining
     */
    logger_builder& apply_strategy(std::unique_ptr<config_strategy_interface> strategy) {
        if (strategy) {
            strategies_.push_back(std::move(strategy));
        }
        return *this;
    }
    
    /**
     * @brief Apply environment-based configuration
     * @param env Environment name ("development", "testing", "staging", "production")
     * @return Reference to builder for chaining
     */
    logger_builder& for_environment(const std::string& env) {
        auto strategy = config_strategy_factory::create_environment(env);
        if (strategy) {
            apply_strategy(std::move(strategy));
        }
        return *this;
    }
    
    /**
     * @brief Apply performance tuning strategy
     * @param level Tuning level ("conservative", "balanced", "aggressive")
     * @return Reference to builder for chaining
     */
    logger_builder& with_performance_tuning(const std::string& level = "balanced") {
        auto strategy = config_strategy_factory::create_tuning(level);
        if (strategy) {
            apply_strategy(std::move(strategy));
        }
        return *this;
    }
    
    /**
     * @brief Auto-detect and apply environment configuration
     * @return Reference to builder for chaining
     */
    logger_builder& auto_configure() {
        auto strategy = config_strategy_factory::from_environment();
        if (strategy) {
            apply_strategy(std::move(strategy));
        }
        return *this;
    }
    
    /**
     * @brief Apply a configuration template
     * @param template_type Template to apply
     * @return Reference to builder for chaining
     */
    logger_builder& apply_template(logger_system::configuration_template template_type) {
        auto template_config = logger_system::get_template_config(template_type);
        config_.min_level = template_config.min_level;
        config_.buffer_size = template_config.buffer_size;
        config_.async = template_config.async_mode;
        config_.enable_batch_writing = template_config.enable_batch_writing;
        config_.batch_size = template_config.batch_size;
        config_.flush_interval = template_config.flush_interval;
        config_.overflow_policy = template_config.overflow_policy_type;
        return *this;
    }
    
    /**
     * @brief Apply a performance strategy
     * @param strategy Performance strategy to apply
     * @return Reference to builder for chaining
     */
    logger_builder& apply_performance_strategy(logger_system::performance_strategy strategy) {
        auto perf_config = logger_system::get_performance_config(strategy);
        config_.min_level = perf_config.min_level;
        config_.buffer_size = perf_config.buffer_size;
        config_.async = perf_config.async_mode;
        config_.enable_batch_writing = perf_config.enable_batch_writing;
        config_.batch_size = perf_config.batch_size;
        config_.flush_interval = perf_config.flush_interval;
        config_.overflow_policy = perf_config.overflow_policy_type;
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
                apply_template(configuration_template::production);
            } else if (env_str == "debug" || env_str == "development") {
                apply_template(configuration_template::debug);
            }
        }
        
        if (level) {
            std::string level_str(level);
            if (level_str == "trace") config_.min_level = thread_module::log_level::trace;
            else if (level_str == "debug") config_.min_level = thread_module::log_level::debug;
            else if (level_str == "info") config_.min_level = thread_module::log_level::info;
            else if (level_str == "warn") config_.min_level = thread_module::log_level::warning;
            else if (level_str == "error") config_.min_level = thread_module::log_level::error;
            else if (level_str == "fatal") config_.min_level = thread_module::log_level::fatal;
        }
        
        return *this;
    }
    
    /**
     * @brief Set monitoring interface
     * @param monitor Monitoring interface implementation
     * @return Reference to builder for chaining
     */
    logger_builder& with_monitoring(std::shared_ptr<monitoring_interface> monitor) {
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
     * @brief Set DI container
     * @param container DI container implementation
     * @return Reference to builder for chaining
     */
    template<typename T>
    logger_builder& with_di_container(std::shared_ptr<di_container_interface<T>> container) {
        // Store for later use
        // Note: Implementation would need to handle type erasure or specific container type
        return *this;
    }
    
    /**
     * @brief Add writer from DI container
     * @param name Writer name to resolve from DI
     * @return Reference to builder for chaining
     */
    logger_builder& with_writer_from_di(const std::string& name) {
        // Implementation would resolve from DI container
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
     * @brief Set overflow policy
     * @param policy Overflow policy to use
     * @return Reference to builder for chaining
     */
    logger_builder& with_overflow_policy(overflow_policy policy) {
        config_.overflow_policy = policy;
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
    
    /**
     * @brief Clear all applied strategies
     * @return Reference to builder for chaining
     */
    logger_builder& clear_strategies() {
        strategies_.clear();
        return *this;
    }
    
    /**
     * @brief Build the logger with validation
     * @return Result containing the logger or error
     */
    result<std::unique_ptr<logger>> build() {
        // Apply all strategies first
        for (const auto& strategy : strategies_) {
            if (auto can_apply = strategy->can_apply(config_); !can_apply) {
                // Log warning but continue with other strategies
                continue;
            }
            
            if (auto result = strategy->apply(config_); !result) {
#ifdef USE_THREAD_SYSTEM
                auto error_code = static_cast<int>(result.get_error().code());
                auto logger_code = static_cast<logger_error_code>(error_code - 10000);
                return make_error<std::unique_ptr<logger>>(
                    logger_code,
                    "Strategy application failed: " + result.get_error().message()
                );
#else
                return make_logger_error<std::unique_ptr<logger>>(
                    result.error_code(),
                    "Strategy application failed: " + result.error_message()
                );
#endif
            }
        }
        
        // Validate configuration
        if (auto validation = config_.validate(); !validation) {
#ifdef USE_THREAD_SYSTEM
            // Extract error code - we need to reverse the offset we applied
            auto error_code = static_cast<int>(validation.get_error().code());
            auto logger_code = static_cast<logger_error_code>(error_code - 10000);
            return make_error<std::unique_ptr<logger>>(
                logger_code,
                "Configuration validation failed: " + validation.get_error().message()
            );
#else
            return make_logger_error<std::unique_ptr<logger>>(
                validation.error_code(),
                "Configuration validation failed: " + validation.error_message()
            );
#endif
        }
        
        // Validate writer count
        if (!writers_.empty() && writers_.size() > config_.max_writers) {
            return make_logger_error<std::unique_ptr<logger>>(
                logger_error_code::invalid_configuration,
                "Number of writers exceeds max_writers configuration"
            );
        }
        
        // Create logger with validated configuration
        auto logger_instance = std::make_unique<logger>(config_.async, config_.buffer_size);
        
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
        
        // Add filters
        if (!filters_.empty()) {
            // Create composite filter if multiple filters
            if (filters_.size() == 1) {
                logger_instance->set_filter(std::move(filters_[0]));
            } else {
                auto composite = std::make_unique<composite_filter>(composite_filter::logic_type::AND);
                for (auto& filter : filters_) {
                    composite->add_filter(std::move(filter));
                }
                logger_instance->set_filter(std::move(composite));
            }
        }
        
        // Start logger if async
        if (config_.async) {
            logger_instance->start();
        }
        
        // Store configuration in logger (if we add a config getter)
        built_config_ = config_;
        
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
    std::vector<std::unique_ptr<log_filter>> filters_;
    std::unique_ptr<log_formatter_interface> formatter_;
    std::vector<std::unique_ptr<config_strategy_interface>> strategies_;
    mutable logger_config built_config_;  // Store last built configuration
    std::shared_ptr<monitoring_interface> monitor_;
    std::chrono::milliseconds health_check_interval_{1000};
    std::function<void(const logger_error_code&)> error_handler_;
};

} // namespace logger_module