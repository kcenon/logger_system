#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

/**
 * @file performance_strategy.h
 * @brief Performance tuning configuration strategies
 * @since 2.0.0
 *
 * @details Provides pre-configured performance profiles for different use cases:
 * - low_latency: Minimizes processing delay
 * - balanced: Default balanced configuration
 * - high_throughput: Maximizes logging throughput
 * - minimal_overhead: Reduces CPU/memory overhead
 */

#include "config_strategy_interface.h"
#include "../logger_config.h"

namespace kcenon::logger {

/**
 * @enum performance_level
 * @brief Performance tuning presets
 */
enum class performance_level {
    low_latency,      ///< Minimize latency (small buffers, immediate flush)
    balanced,         ///< Balanced configuration (default)
    high_throughput,  ///< Maximize throughput (large buffers, batch processing)
    minimal_overhead  ///< Minimize resource usage (simple format, less features)
};

/**
 * @class performance_strategy
 * @brief Configuration strategy for performance tuning
 *
 * @details Applies performance-optimized settings based on the selected level.
 * Each level is optimized for specific use cases.
 *
 * @since 2.0.0
 */
class performance_strategy : public config_strategy_interface {
public:
    /**
     * @brief Constructor with performance level
     * @param level The performance tuning level to apply
     */
    explicit performance_strategy(performance_level level)
        : level_(level) {}

    std::string get_name() const override {
        return "performance:" + level_to_string(level_);
    }

    void apply(logger_config& config) const override {
        switch (level_) {
        case performance_level::low_latency:
            apply_low_latency(config);
            break;
        case performance_level::balanced:
            apply_balanced(config);
            break;
        case performance_level::high_throughput:
            apply_high_throughput(config);
            break;
        case performance_level::minimal_overhead:
            apply_minimal_overhead(config);
            break;
        }
    }

    int priority() const override {
        return 50;  // Medium priority
    }

private:
    performance_level level_;

    static void apply_low_latency(logger_config& config) {
        config.async = true;
        config.buffer_size = 4096;
        config.batch_size = 10;
        config.flush_interval = std::chrono::milliseconds(10);
        config.use_lock_free = true;
        config.max_queue_size = 10000;
        config.queue_overflow_policy = logger_config::overflow_policy::drop_oldest;
        config.enable_batch_writing = false;
    }

    static void apply_balanced(logger_config& config) {
        config.async = true;
        config.buffer_size = 8192;
        config.batch_size = 100;
        config.flush_interval = std::chrono::milliseconds(1000);
        config.use_lock_free = false;
        config.max_queue_size = 10000;
        config.queue_overflow_policy = logger_config::overflow_policy::drop_newest;
        config.enable_batch_writing = true;
    }

    static void apply_high_throughput(logger_config& config) {
        config.async = true;
        config.buffer_size = 65536;
        config.batch_size = 500;
        config.flush_interval = std::chrono::milliseconds(5000);
        config.use_lock_free = true;
        config.max_queue_size = 100000;
        config.queue_overflow_policy = logger_config::overflow_policy::drop_oldest;
        config.writer_thread_count = 2;
        config.enable_compression = true;
        config.enable_batch_writing = true;
    }

    static void apply_minimal_overhead(logger_config& config) {
        config.async = true;
        config.buffer_size = 4096;
        config.batch_size = 50;
        config.flush_interval = std::chrono::milliseconds(2000);
        config.enable_metrics = false;
        config.enable_structured_logging = false;
        config.enable_source_location = false;
        config.enable_color_output = false;
        config.enable_batch_writing = true;
    }

    static std::string level_to_string(performance_level level) {
        switch (level) {
        case performance_level::low_latency: return "low_latency";
        case performance_level::balanced: return "balanced";
        case performance_level::high_throughput: return "high_throughput";
        case performance_level::minimal_overhead: return "minimal_overhead";
        default: return "unknown";
        }
    }
};

} // namespace kcenon::logger
