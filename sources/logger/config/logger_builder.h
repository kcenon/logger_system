#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "logger_config.h"
#include "../logger.h"
#include "../writers/base_writer.h"
#include "../filters/log_filter.h"
#include "../interfaces/log_formatter_interface.h"
#include <memory>
#include <vector>

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
     * @brief Build the logger with validation
     * @return Result containing the logger or error
     */
    result<std::unique_ptr<logger>> build() {
        // Validate configuration
        if (auto validation = config_.validate(); !validation) {
            // Extract error code - we need to reverse the offset we applied
            auto error_code = static_cast<int>(validation.get_error().code());
            auto logger_code = static_cast<logger_error_code>(error_code - 10000);
            return make_error<std::unique_ptr<logger>>(
                logger_code,
                "Configuration validation failed: " + validation.get_error().message()
            );
        }
        
        // Validate writer count
        if (!writers_.empty() && writers_.size() > config_.max_writers) {
            return make_error<std::unique_ptr<logger>>(
                logger_error_code::invalid_configuration,
                "Number of writers exceeds max_writers configuration"
            );
        }
        
        // Create logger with validated configuration
        auto logger_instance = std::make_unique<logger>(config_.buffer_size);
        
        // Apply configuration settings
        logger_instance->set_min_level(config_.min_level);
        
        if (config_.enable_metrics) {
            logger_instance->enable_metrics_collection(true);
        }
        
        // Add writers
        for (auto& [name, writer] : writers_) {
            if (writer) {
                writer->set_use_color(config_.enable_color_output);
                logger_instance->add_writer(name, std::move(writer));
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
    mutable logger_config built_config_;  // Store last built configuration
};

} // namespace logger_module