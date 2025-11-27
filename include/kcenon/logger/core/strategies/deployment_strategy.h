#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

/**
 * @file deployment_strategy.h
 * @brief Deployment environment configuration strategies
 * @since 2.0.0
 *
 * @details Provides pre-configured settings optimized for different
 * deployment environments:
 * - development: Verbose logging, colors, immediate output
 * - staging: JSON format, file + console, rotation enabled
 * - production: Optimized, warning+, crash protection
 * - testing: Memory buffer, synchronous, full capture
 */

#include "config_strategy_interface.h"
#include "../logger_config.h"

namespace kcenon::logger {

/**
 * @enum deployment_env
 * @brief Deployment environment types
 */
enum class deployment_env {
    development,  ///< Development environment (verbose, colorful)
    staging,      ///< Staging environment (JSON, file + console)
    production,   ///< Production environment (optimized, safe)
    testing       ///< Testing environment (memory buffer, sync)
};

/**
 * @class deployment_strategy
 * @brief Configuration strategy based on deployment environment
 *
 * @details Applies environment-appropriate settings for the logger.
 * Each environment type is optimized for its specific use case.
 *
 * @since 2.0.0
 */
class deployment_strategy : public config_strategy_interface {
public:
    /**
     * @brief Constructor with deployment environment
     * @param env The deployment environment to configure for
     */
    explicit deployment_strategy(deployment_env env)
        : env_(env) {}

    std::string get_name() const override {
        return "deployment:" + env_to_string(env_);
    }

    void apply(logger_config& config) const override {
        switch (env_) {
        case deployment_env::development:
            apply_development(config);
            break;
        case deployment_env::staging:
            apply_staging(config);
            break;
        case deployment_env::production:
            apply_production(config);
            break;
        case deployment_env::testing:
            apply_testing(config);
            break;
        }
    }

    int priority() const override {
        return 75;  // Higher than performance, lower than environment
    }

private:
    deployment_env env_;

    static void apply_development(logger_config& config) {
        config.async = false;
        config.min_level = logger_system::log_level::trace;
        config.enable_metrics = true;
        config.enable_crash_handler = true;
        config.enable_color_output = true;
        config.enable_source_location = true;
        config.enable_structured_logging = false;
        config.batch_size = 1;
        config.flush_interval = std::chrono::milliseconds(0);
    }

    static void apply_staging(logger_config& config) {
        config.async = true;
        config.min_level = logger_system::log_level::info;
        config.enable_metrics = true;
        config.enable_crash_handler = true;
        config.enable_color_output = false;
        config.enable_structured_logging = true;
        config.buffer_size = 16384;
        config.batch_size = 100;
        config.flush_interval = std::chrono::milliseconds(1000);
        config.max_file_size = 50 * 1024 * 1024;  // 50MB
        config.max_file_count = 5;
        config.enable_batch_writing = true;
    }

    static void apply_production(logger_config& config) {
        config.async = true;
        config.min_level = logger_system::log_level::warn;
        config.enable_metrics = true;
        config.enable_crash_handler = true;
        config.enable_color_output = false;
        config.enable_source_location = false;
        config.enable_structured_logging = true;
        config.buffer_size = 32768;
        config.batch_size = 200;
        config.flush_interval = std::chrono::milliseconds(2000);
        config.max_file_size = 100 * 1024 * 1024;  // 100MB
        config.max_file_count = 10;
        config.enable_compression = true;
        config.enable_batch_writing = true;
        config.queue_overflow_policy = logger_config::overflow_policy::drop_oldest;
    }

    static void apply_testing(logger_config& config) {
        config.async = false;
        config.min_level = logger_system::log_level::trace;
        config.enable_metrics = false;
        config.enable_crash_handler = false;
        config.enable_color_output = false;
        config.enable_source_location = true;
        config.batch_size = 1;
        config.flush_interval = std::chrono::milliseconds(0);
    }

    static std::string env_to_string(deployment_env env) {
        switch (env) {
        case deployment_env::development: return "development";
        case deployment_env::staging: return "staging";
        case deployment_env::production: return "production";
        case deployment_env::testing: return "testing";
        default: return "unknown";
        }
    }
};

} // namespace kcenon::logger
