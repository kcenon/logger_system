#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

/**
 * @file environment_strategy.h
 * @brief Environment variable-based configuration strategy
 * @since 2.0.0
 *
 * @details Configures the logger based on environment variables:
 * - LOG_LEVEL: Log level (trace, debug, info, warn, error, fatal)
 * - LOG_ASYNC: Async mode (true/false, 1/0)
 * - LOG_BUFFER_SIZE: Buffer size in bytes
 * - LOG_BATCH_SIZE: Batch size for processing
 * - LOG_FLUSH_INTERVAL: Flush interval in milliseconds
 * - LOG_COLOR: Enable color output (true/false, 1/0)
 * - LOG_METRICS: Enable metrics collection (true/false, 1/0)
 */

#include "config_strategy_interface.h"
#include "../logger_config.h"
#include <cstdlib>
#include <string>
#include <algorithm>

namespace kcenon::logger {

/**
 * @class environment_strategy
 * @brief Configuration strategy based on environment variables
 *
 * @details Reads environment variables and applies corresponding settings
 * to the logger configuration. Useful for containerized deployments
 * where configuration is passed via environment.
 *
 * @since 2.0.0
 */
class environment_strategy : public config_strategy_interface {
public:
    std::string get_name() const override {
        return "environment";
    }

    void apply(logger_config& config) const override {
        // LOG_LEVEL
        if (const char* level = std::getenv("LOG_LEVEL")) {
            config.min_level = parse_log_level(level);
        }

        // LOG_ASYNC
        if (const char* async_str = std::getenv("LOG_ASYNC")) {
            config.async = parse_bool(async_str);
            if (!config.async && config.batch_size > 1) {
                config.batch_size = 1;
            }
        }

        // LOG_BUFFER_SIZE
        if (const char* buffer = std::getenv("LOG_BUFFER_SIZE")) {
            config.buffer_size = parse_size(buffer, config.buffer_size);
        }

        // LOG_BATCH_SIZE
        if (const char* batch = std::getenv("LOG_BATCH_SIZE")) {
            config.batch_size = parse_size(batch, config.batch_size);
        }

        // LOG_FLUSH_INTERVAL
        if (const char* interval = std::getenv("LOG_FLUSH_INTERVAL")) {
            config.flush_interval = std::chrono::milliseconds(
                parse_size(interval, static_cast<std::size_t>(config.flush_interval.count()))
            );
        }

        // LOG_COLOR
        if (const char* color = std::getenv("LOG_COLOR")) {
            config.enable_color_output = parse_bool(color);
        }

        // LOG_METRICS
        if (const char* metrics = std::getenv("LOG_METRICS")) {
            config.enable_metrics = parse_bool(metrics);
        }

        // LOG_STRUCTURED
        if (const char* structured = std::getenv("LOG_STRUCTURED")) {
            config.enable_structured_logging = parse_bool(structured);
        }

        // LOG_CRASH_HANDLER
        if (const char* crash = std::getenv("LOG_CRASH_HANDLER")) {
            config.enable_crash_handler = parse_bool(crash);
        }

        // LOG_MAX_QUEUE_SIZE
        if (const char* queue = std::getenv("LOG_MAX_QUEUE_SIZE")) {
            config.max_queue_size = parse_size(queue, config.max_queue_size);
        }

        // LOG_BATCH_WRITING
        if (const char* batch_write = std::getenv("LOG_BATCH_WRITING")) {
            config.enable_batch_writing = parse_bool(batch_write);
        }
    }

    bool is_applicable() const override {
        // Applicable if any relevant environment variable is set
        return std::getenv("LOG_LEVEL") != nullptr ||
               std::getenv("LOG_ASYNC") != nullptr ||
               std::getenv("LOG_BUFFER_SIZE") != nullptr ||
               std::getenv("LOG_BATCH_SIZE") != nullptr ||
               std::getenv("LOG_FLUSH_INTERVAL") != nullptr ||
               std::getenv("LOG_COLOR") != nullptr ||
               std::getenv("LOG_METRICS") != nullptr;
    }

    int priority() const override {
        // High priority - environment overrides other strategies
        return 100;
    }

private:
    static logger_system::log_level parse_log_level(const char* str) {
        std::string level(str);
        std::transform(level.begin(), level.end(), level.begin(), ::tolower);

        if (level == "trace") return logger_system::log_level::trace;
        if (level == "debug") return logger_system::log_level::debug;
        if (level == "info") return logger_system::log_level::info;
        if (level == "warn" || level == "warning") return logger_system::log_level::warn;
        if (level == "error") return logger_system::log_level::error;
        if (level == "fatal" || level == "critical") return logger_system::log_level::fatal;

        return logger_system::log_level::info;  // Default
    }

    static bool parse_bool(const char* str) {
        std::string value(str);
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        return value == "true" || value == "1" || value == "yes" || value == "on";
    }

    static std::size_t parse_size(const char* str, std::size_t default_value) {
        try {
            return std::stoull(str);
        } catch (...) {
            return default_value;
        }
    }
};

} // namespace kcenon::logger
