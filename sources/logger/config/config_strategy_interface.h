#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "logger_config.h"
#include "../error_codes.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <unordered_map>

namespace logger_module {

/**
 * @interface config_strategy_interface
 * @brief Abstract interface for configuration strategies
 * 
 * This interface defines the contract for configuration strategies
 * that can be used with the logger_builder to apply different
 * configuration approaches (templates, environments, custom rules).
 */
class config_strategy_interface {
public:
    virtual ~config_strategy_interface() = default;
    
    /**
     * @brief Apply the strategy to a configuration
     * @param config Configuration to modify
     * @return result_void indicating success or error
     */
    virtual result_void apply(logger_config& config) = 0;
    
    /**
     * @brief Get strategy name for identification
     * @return Strategy name
     */
    virtual std::string get_name() const = 0;
    
    /**
     * @brief Get strategy description
     * @return Human-readable description
     */
    virtual std::string get_description() const = 0;
    
    /**
     * @brief Validate if this strategy can be applied
     * @param config Current configuration
     * @return result_void indicating if strategy is applicable
     */
    virtual result_void can_apply(const logger_config& config) const = 0;
    
    /**
     * @brief Get the priority of this strategy (higher = applied first)
     * @return Priority value (0-100)
     */
    virtual int get_priority() const { return 50; }
    
    /**
     * @brief Check if this strategy should override existing settings
     * @return true if strategy should override, false to merge
     */
    virtual bool should_override() const { return false; }
};

/**
 * @class template_strategy
 * @brief Strategy for applying predefined configuration templates
 */
class template_strategy : public config_strategy_interface {
public:
    enum class template_type {
        default_config,
        high_performance,
        low_latency,
        debug,
        production
    };
    
    explicit template_strategy(template_type type) : type_(type) {}
    
    result_void apply(logger_config& config) override {
        switch (type_) {
            case template_type::high_performance:
                config = logger_config::high_performance();
                break;
            case template_type::low_latency:
                config = logger_config::low_latency();
                break;
            case template_type::debug:
                config = logger_config::debug_config();
                break;
            case template_type::production:
                config = logger_config::production();
                break;
            default:
                config = logger_config::default_config();
                break;
        }
        return result_void{};
    }
    
    std::string get_name() const override {
        switch (type_) {
            case template_type::high_performance:
                return "high_performance";
            case template_type::low_latency:
                return "low_latency";
            case template_type::debug:
                return "debug";
            case template_type::production:
                return "production";
            default:
                return "default";
        }
    }
    
    std::string get_description() const override {
        switch (type_) {
            case template_type::high_performance:
                return "Optimized for high throughput with large buffers";
            case template_type::low_latency:
                return "Minimized latency with small batches and frequent flushes";
            case template_type::debug:
                return "Synchronous logging with immediate output for debugging";
            case template_type::production:
                return "Production-ready with metrics and crash handling";
            default:
                return "Default balanced configuration";
        }
    }
    
    result_void can_apply(const logger_config&) const override {
        return result_void{}; // Templates can always be applied
    }
    
    bool should_override() const override { return true; }
    
private:
    template_type type_;
};

/**
 * @class environment_strategy
 * @brief Strategy for applying environment-based configuration
 */
class environment_strategy : public config_strategy_interface {
public:
    enum class environment {
        development,
        testing,
        staging,
        production
    };
    
    explicit environment_strategy(environment env) : env_(env) {}
    
    result_void apply(logger_config& config) override {
        switch (env_) {
            case environment::development:
                config.async = false;  // Immediate output for development
                config.min_level = thread_module::log_level::trace;
                config.enable_color_output = true;
                config.enable_metrics = false;
                break;
                
            case environment::testing:
                config.async = true;
                config.min_level = thread_module::log_level::debug;
                config.enable_metrics = true;
                config.enable_crash_handler = false;
                break;
                
            case environment::staging:
                config.async = true;
                config.min_level = thread_module::log_level::info;
                config.enable_metrics = true;
                config.enable_crash_handler = true;
                config.enable_structured_logging = true;
                break;
                
            case environment::production:
                config = logger_config::production();
                break;
        }
        return result_void{};
    }
    
    std::string get_name() const override {
        switch (env_) {
            case environment::development:
                return "development";
            case environment::testing:
                return "testing";
            case environment::staging:
                return "staging";
            case environment::production:
                return "production";
        }
    }
    
    std::string get_description() const override {
        switch (env_) {
            case environment::development:
                return "Development environment with verbose logging";
            case environment::testing:
                return "Testing environment with debug output";
            case environment::staging:
                return "Staging environment matching production setup";
            case environment::production:
                return "Production environment with optimizations";
        }
    }
    
    result_void can_apply(const logger_config&) const override {
        return result_void{};
    }
    
    int get_priority() const override { return 60; } // Higher than templates
    
private:
    environment env_;
};

/**
 * @class performance_tuning_strategy
 * @brief Strategy for automatic performance tuning based on system resources
 */
class performance_tuning_strategy : public config_strategy_interface {
public:
    enum class tuning_level {
        conservative,  // Low resource usage
        balanced,      // Default balanced approach
        aggressive     // Maximum performance
    };
    
    explicit performance_tuning_strategy(tuning_level level = tuning_level::balanced) 
        : level_(level) {}
    
    result_void apply(logger_config& config) override {
        switch (level_) {
            case tuning_level::conservative:
                config.buffer_size = 4096;
                config.batch_size = 50;
                config.max_queue_size = 1000;
                config.flush_interval = std::chrono::milliseconds(500);
                config.writer_thread_count = 1;
                break;
                
            case tuning_level::balanced:
                config.buffer_size = 8192;
                config.batch_size = 100;
                config.max_queue_size = 10000;
                config.flush_interval = std::chrono::milliseconds(1000);
                config.writer_thread_count = 2;
                break;
                
            case tuning_level::aggressive:
                config.buffer_size = 65536;
                config.batch_size = 500;
                config.max_queue_size = 100000;
                config.flush_interval = std::chrono::milliseconds(5000);
                config.writer_thread_count = 4;
                config.use_lock_free = true;
                config.enable_compression = true;
                break;
        }
        return result_void{};
    }
    
    std::string get_name() const override {
        switch (level_) {
            case tuning_level::conservative:
                return "conservative_tuning";
            case tuning_level::balanced:
                return "balanced_tuning";
            case tuning_level::aggressive:
                return "aggressive_tuning";
        }
    }
    
    std::string get_description() const override {
        switch (level_) {
            case tuning_level::conservative:
                return "Conservative resource usage for constrained environments";
            case tuning_level::balanced:
                return "Balanced performance and resource usage";
            case tuning_level::aggressive:
                return "Maximum performance with higher resource consumption";
        }
    }
    
    result_void can_apply(const logger_config& config) const override {
        // Check if async mode is enabled (required for performance tuning)
        if (!config.async) {
            return make_logger_error(logger_error_code::invalid_configuration,
                            "Performance tuning requires async mode");
        }
        return result_void{};
    }
    
    int get_priority() const override { return 40; } // Lower than environment
    
private:
    tuning_level level_;
};

/**
 * @class composite_strategy
 * @brief Combines multiple strategies in sequence
 */
class composite_strategy : public config_strategy_interface {
public:
    composite_strategy() = default;
    
    /**
     * @brief Add a strategy to the composite
     * @param strategy Strategy to add
     */
    void add_strategy(std::unique_ptr<config_strategy_interface> strategy) {
        strategies_.push_back(std::move(strategy));
        // Sort by priority (descending)
        std::sort(strategies_.begin(), strategies_.end(),
                  [](const auto& a, const auto& b) {
                      return a->get_priority() > b->get_priority();
                  });
    }
    
    result_void apply(logger_config& config) override {
        for (const auto& strategy : strategies_) {
            if (auto can_apply = strategy->can_apply(config); !can_apply) {
                continue; // Skip strategies that can't be applied
            }
            
            if (auto result = strategy->apply(config); !result) {
                return result;
            }
        }
        return result_void{};
    }
    
    std::string get_name() const override {
        return "composite";
    }
    
    std::string get_description() const override {
        std::string desc = "Composite of: ";
        for (size_t i = 0; i < strategies_.size(); ++i) {
            if (i > 0) desc += ", ";
            desc += strategies_[i]->get_name();
        }
        return desc;
    }
    
    result_void can_apply(const logger_config& config) const override {
        // Composite can apply if at least one strategy can apply
        for (const auto& strategy : strategies_) {
            if (auto can_apply = strategy->can_apply(config); can_apply) {
                return result_void{};
            }
        }
        return make_logger_error(logger_error_code::invalid_configuration,
                        "No strategies in composite can be applied");
    }
    
private:
    std::vector<std::unique_ptr<config_strategy_interface>> strategies_;
};

/**
 * @class config_strategy_factory
 * @brief Factory for creating configuration strategies
 */
class config_strategy_factory {
public:
    /**
     * @brief Create a template strategy
     * @param name Template name
     * @return Strategy instance or nullptr if name is invalid
     */
    static std::unique_ptr<config_strategy_interface> create_template(const std::string& name) {
        if (name == "high_performance") {
            return std::make_unique<template_strategy>(template_strategy::template_type::high_performance);
        } else if (name == "low_latency") {
            return std::make_unique<template_strategy>(template_strategy::template_type::low_latency);
        } else if (name == "debug") {
            return std::make_unique<template_strategy>(template_strategy::template_type::debug);
        } else if (name == "production") {
            return std::make_unique<template_strategy>(template_strategy::template_type::production);
        } else if (name == "default") {
            return std::make_unique<template_strategy>(template_strategy::template_type::default_config);
        }
        return nullptr;
    }
    
    /**
     * @brief Create an environment strategy
     * @param env Environment name
     * @return Strategy instance or nullptr if environment is invalid
     */
    static std::unique_ptr<config_strategy_interface> create_environment(const std::string& env) {
        if (env == "development" || env == "dev") {
            return std::make_unique<environment_strategy>(environment_strategy::environment::development);
        } else if (env == "testing" || env == "test") {
            return std::make_unique<environment_strategy>(environment_strategy::environment::testing);
        } else if (env == "staging" || env == "stage") {
            return std::make_unique<environment_strategy>(environment_strategy::environment::staging);
        } else if (env == "production" || env == "prod") {
            return std::make_unique<environment_strategy>(environment_strategy::environment::production);
        }
        return nullptr;
    }
    
    /**
     * @brief Create a performance tuning strategy
     * @param level Tuning level name
     * @return Strategy instance
     */
    static std::unique_ptr<config_strategy_interface> create_tuning(const std::string& level) {
        if (level == "conservative" || level == "low") {
            return std::make_unique<performance_tuning_strategy>(
                performance_tuning_strategy::tuning_level::conservative);
        } else if (level == "aggressive" || level == "high") {
            return std::make_unique<performance_tuning_strategy>(
                performance_tuning_strategy::tuning_level::aggressive);
        }
        return std::make_unique<performance_tuning_strategy>(
            performance_tuning_strategy::tuning_level::balanced);
    }
    
    /**
     * @brief Create a strategy from environment variable
     * @return Strategy based on LOG_ENV or nullptr
     */
    static std::unique_ptr<config_strategy_interface> from_environment() {
        const char* env = std::getenv("LOG_ENV");
        if (env) {
            return create_environment(env);
        }
        
        const char* level = std::getenv("LOG_LEVEL");
        if (level && std::string(level) == "DEBUG") {
            return create_template("debug");
        }
        
        return nullptr;
    }
};

} // namespace logger_module