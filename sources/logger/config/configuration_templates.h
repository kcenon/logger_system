/**
 * @file configuration_templates.h
 * @brief Pre-defined configuration templates for common scenarios
 * @date 2025-09-09
 *
 * BSD 3-Clause License
 * Copyright (c) 2025, kcenon
 * All rights reserved.
 */

#pragma once

#include "../logger_interface.h"
#include <string>
#include <chrono>

namespace logger_system {

// Use types from thread_module
using log_level = thread_module::log_level;

// Define overflow_policy locally
enum class overflow_policy {
    block,          // Block until space is available
    drop_oldest,    // Drop oldest messages
    drop_newest,    // Drop newest messages  
    grow           // Dynamically grow buffer
};

/**
 * @brief Pre-defined configuration templates
 */
enum class configuration_template {
    production,      // Optimized for production use
    debug,          // Verbose logging for debugging
    high_performance,  // Maximum throughput
    low_latency     // Minimum latency
};

/**
 * @brief Performance tuning strategies
 */
enum class performance_strategy {
    conservative,   // Low resource usage, stable
    balanced,      // Balance between performance and resources
    aggressive     // Maximum performance, higher resource usage
};

/**
 * @brief Template configuration settings
 */
struct template_config {
    log_level min_level;
    size_t buffer_size;
    bool async_mode;
    bool enable_batch_writing;
    size_t batch_size;
    std::chrono::milliseconds flush_interval;
    overflow_policy overflow_policy_type;
    bool enable_monitoring;
    bool enable_health_checks;
};

/**
 * @brief Get configuration for a specific template
 */
inline template_config get_template_config(configuration_template template_type) {
    switch (template_type) {
        case configuration_template::production:
            return {
                log_level::info,           // min_level
                10000,                      // buffer_size
                true,                       // async_mode
                true,                       // enable_batch_writing
                100,                        // batch_size
                std::chrono::milliseconds(1000), // flush_interval
                overflow_policy::drop_oldest, // overflow_policy_type
                true,                       // enable_monitoring
                true                        // enable_health_checks
            };
            
        case configuration_template::debug:
            return {
                log_level::trace,           // min_level
                1000,                       // buffer_size
                false,                      // async_mode
                false,                      // enable_batch_writing
                1,                          // batch_size
                std::chrono::milliseconds(0), // flush_interval
                overflow_policy::block,     // overflow_policy_type
                true,                       // enable_monitoring
                true                        // enable_health_checks
            };
            
        case configuration_template::high_performance:
            return {
                log_level::warning,            // min_level
                50000,                      // buffer_size
                true,                       // async_mode
                true,                       // enable_batch_writing
                500,                        // batch_size
                std::chrono::milliseconds(5000), // flush_interval
                overflow_policy::drop_oldest, // overflow_policy_type
                false,                      // enable_monitoring
                false                       // enable_health_checks
            };
            
        case configuration_template::low_latency:
            return {
                log_level::info,            // min_level
                5000,                       // buffer_size
                true,                       // async_mode
                false,                      // enable_batch_writing
                1,                          // batch_size
                std::chrono::milliseconds(0), // flush_interval
                overflow_policy::grow,      // overflow_policy_type
                false,                      // enable_monitoring
                false                       // enable_health_checks
            };
            
        default:
            // Default to balanced configuration
            return {
                log_level::info,            // min_level
                5000,                       // buffer_size
                true,                       // async_mode
                true,                       // enable_batch_writing
                50,                         // batch_size
                std::chrono::milliseconds(500), // flush_interval
                overflow_policy::drop_oldest, // overflow_policy_type
                true,                       // enable_monitoring
                false                       // enable_health_checks
            };
    }
}

/**
 * @brief Get configuration for a performance strategy
 */
inline template_config get_performance_config(performance_strategy strategy) {
    switch (strategy) {
        case performance_strategy::conservative:
            return {
                log_level::info,            // min_level
                1000,                       // buffer_size
                false,                      // async_mode
                false,                      // enable_batch_writing
                10,                         // batch_size
                std::chrono::milliseconds(100), // flush_interval
                overflow_policy::block,     // overflow_policy_type
                true,                       // enable_monitoring
                true                        // enable_health_checks
            };
            
        case performance_strategy::balanced:
            return {
                log_level::info,            // min_level
                5000,                       // buffer_size
                true,                       // async_mode
                true,                       // enable_batch_writing
                50,                         // batch_size
                std::chrono::milliseconds(500), // flush_interval
                overflow_policy::drop_oldest, // overflow_policy_type
                true,                       // enable_monitoring
                false                       // enable_health_checks
            };
            
        case performance_strategy::aggressive:
            return {
                log_level::warning,            // min_level
                50000,                      // buffer_size
                true,                       // async_mode
                true,                       // enable_batch_writing
                1000,                       // batch_size
                std::chrono::milliseconds(10000), // flush_interval
                overflow_policy::drop_oldest, // overflow_policy_type
                false,                      // enable_monitoring
                false                       // enable_health_checks
            };
            
        default:
            return get_performance_config(performance_strategy::balanced);
    }
}

} // namespace logger_system