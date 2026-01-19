#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/**
 * @file sampling_config.h
 * @brief Configuration structures and enums for log sampling
 * @author 🍀☀🌕🌥 🌊
 * @since 3.3.0
 *
 * @details This file defines the configuration structures and sampling strategies
 * for the log sampling feature. Sampling helps reduce log volume in high-throughput
 * scenarios while preserving critical log entries.
 *
 * @example Basic usage:
 * @code
 * sampling_config config;
 * config.rate = 0.1;  // Sample 10% of logs
 * config.always_log_levels = {log_level::error, log_level::fatal};
 * @endcode
 */

#include <vector>
#include <string>
#include <unordered_map>
#include <cstddef>
#include <cstdint>

#include <kcenon/common/interfaces/logger_interface.h>

namespace kcenon::logger::sampling {

// Type alias for log_level
using log_level = common::interfaces::log_level;

/**
 * @enum sampling_strategy
 * @brief Defines the sampling algorithm to use
 *
 * @details Different strategies are suitable for different scenarios:
 * - random: Simple probabilistic sampling, good for general use
 * - rate_limiting: Ensures consistent log rate, good for burst protection
 * - adaptive: Automatically adjusts based on load, good for variable workloads
 * - hash_based: Deterministic sampling based on message hash, good for debugging
 *
 * @since 3.3.0
 */
enum class sampling_strategy : std::uint8_t {
    random,         ///< Simple random sampling based on probability
    rate_limiting,  ///< Rate-based sampling (N logs per time window)
    adaptive,       ///< Adaptive sampling that adjusts based on volume
    hash_based      ///< Deterministic sampling based on message hash
};

/**
 * @struct sampling_config
 * @brief Configuration for log sampling behavior
 *
 * @details Controls how logs are sampled to reduce volume in high-throughput
 * scenarios. Critical levels can be configured to bypass sampling entirely.
 *
 * @example
 * @code
 * sampling_config config;
 * config.enabled = true;
 * config.rate = 0.1;  // Sample 10%
 * config.strategy = sampling_strategy::random;
 * config.always_log_levels = {log_level::warn, log_level::error, log_level::fatal};
 * config.category_rates["database"] = 0.01;  // Only 1% of database logs
 * @endcode
 *
 * @since 3.3.0
 */
struct sampling_config {
    /**
     * @brief Enable or disable sampling
     * @details When false, all logs are passed through without sampling
     */
    bool enabled = false;

    /**
     * @brief Base sampling rate (0.0 to 1.0)
     * @details Probability that a log message will be sampled (kept).
     * - 1.0 = keep all logs (no sampling)
     * - 0.1 = keep 10% of logs
     * - 0.0 = drop all logs (not recommended)
     */
    double rate = 1.0;

    /**
     * @brief Sampling strategy to use
     * @details Determines the algorithm for selecting which logs to keep
     */
    sampling_strategy strategy = sampling_strategy::random;

    /**
     * @brief Log levels that are never sampled (always logged)
     * @details Critical levels should bypass sampling to ensure important
     * messages are never dropped. Default includes error and critical.
     */
    std::vector<log_level> always_log_levels = {
        log_level::error,
        log_level::critical
    };

    /**
     * @brief Per-category sampling rates
     * @details Allows different sampling rates for different log categories.
     * Categories not in this map use the base rate.
     *
     * @example
     * @code
     * config.category_rates["database"] = 0.01;   // 1% of database logs
     * config.category_rates["security"] = 1.0;    // All security logs
     * config.category_rates["http"] = 0.05;       // 5% of HTTP logs
     * @endcode
     */
    std::unordered_map<std::string, double> category_rates;

    /**
     * @brief Per-field sampling rates for structured fields
     * @details Allows different sampling rates based on field values.
     * Each entry maps a field name to a map of value->rate.
     * Fields/values not in this map use the base rate.
     *
     * @example
     * @code
     * // Sample 100% of high severity, 10% of low severity
     * config.field_rates["severity"]["high"] = 1.0;
     * config.field_rates["severity"]["low"] = 0.1;
     *
     * // Sample 1% of health checks
     * config.field_rates["endpoint"]["/health"] = 0.01;
     * @endcode
     *
     * @since 3.4.0
     */
    std::unordered_map<std::string, std::unordered_map<std::string, double>> field_rates;

    /**
     * @brief Fields whose presence bypasses sampling (always logged)
     * @details Entries with any of these fields will bypass sampling entirely.
     * Useful for ensuring logs with specific fields are never dropped.
     *
     * @example
     * @code
     * config.always_log_fields = {"error_id", "transaction_id"};
     * @endcode
     *
     * @since 3.4.0
     */
    std::vector<std::string> always_log_fields;

    // Rate limiting specific settings

    /**
     * @brief Maximum logs per second for rate limiting strategy
     * @details Used when strategy is rate_limiting
     */
    std::size_t rate_limit_per_second = 1000;

    /**
     * @brief Time window for rate limiting (milliseconds)
     * @details The window over which rate limiting is applied
     */
    std::size_t rate_limit_window_ms = 1000;

    // Adaptive sampling specific settings

    /**
     * @brief Enable adaptive sampling
     * @details When true and strategy is adaptive, the sampler automatically
     * reduces the sampling rate under high load
     */
    bool adaptive_enabled = false;

    /**
     * @brief Threshold (messages/second) to trigger adaptive sampling
     * @details When log rate exceeds this threshold, adaptive sampling kicks in
     */
    std::size_t adaptive_threshold = 10000;

    /**
     * @brief Minimum sampling rate when adaptive sampling is active
     * @details Even under extreme load, at least this percentage of logs are kept
     */
    double adaptive_min_rate = 0.01;

    // Hash-based sampling specific settings

    /**
     * @brief Seed for hash-based sampling
     * @details Used to make hash-based sampling reproducible
     */
    std::uint64_t hash_seed = 0;

    /**
     * @brief Create a default sampling configuration (disabled)
     * @return Default configuration with sampling disabled
     */
    static sampling_config disabled() {
        return sampling_config{};
    }

    /**
     * @brief Create a random sampling configuration
     * @param sample_rate Sampling rate (0.0 to 1.0)
     * @return Configuration for random sampling
     */
    static sampling_config random_sampling(double sample_rate) {
        sampling_config config;
        config.enabled = true;
        config.rate = sample_rate;
        config.strategy = sampling_strategy::random;
        return config;
    }

    /**
     * @brief Create a rate limiting configuration
     * @param max_per_second Maximum logs per second
     * @return Configuration for rate limiting
     */
    static sampling_config rate_limited(std::size_t max_per_second) {
        sampling_config config;
        config.enabled = true;
        config.strategy = sampling_strategy::rate_limiting;
        config.rate_limit_per_second = max_per_second;
        return config;
    }

    /**
     * @brief Create an adaptive sampling configuration
     * @param threshold Messages/second threshold to trigger adaptation
     * @param min_rate Minimum sampling rate under load
     * @return Configuration for adaptive sampling
     */
    static sampling_config adaptive(std::size_t threshold, double min_rate = 0.01) {
        sampling_config config;
        config.enabled = true;
        config.strategy = sampling_strategy::adaptive;
        config.adaptive_enabled = true;
        config.adaptive_threshold = threshold;
        config.adaptive_min_rate = min_rate;
        return config;
    }

    /**
     * @brief Create a hash-based sampling configuration
     * @param sample_rate Sampling rate (0.0 to 1.0)
     * @param seed Optional seed for reproducibility
     * @return Configuration for hash-based sampling
     */
    static sampling_config hash_based(double sample_rate, std::uint64_t seed = 0) {
        sampling_config config;
        config.enabled = true;
        config.rate = sample_rate;
        config.strategy = sampling_strategy::hash_based;
        config.hash_seed = seed;
        return config;
    }
};

/**
 * @struct sampling_stats
 * @brief Statistics about sampling behavior
 *
 * @details Tracks sampling metrics for monitoring and debugging purposes.
 * All counters are thread-safe when accessed through the sampler API.
 *
 * @since 3.3.0
 */
struct sampling_stats {
    /**
     * @brief Total number of log messages considered for sampling
     */
    std::uint64_t total_count = 0;

    /**
     * @brief Number of messages that passed sampling (were logged)
     */
    std::uint64_t sampled_count = 0;

    /**
     * @brief Number of messages dropped due to sampling
     */
    std::uint64_t dropped_count = 0;

    /**
     * @brief Number of messages that bypassed sampling (always_log levels)
     */
    std::uint64_t bypassed_count = 0;

    /**
     * @brief Current effective sampling rate
     * @details May differ from configured rate when adaptive sampling is active
     */
    double effective_rate = 1.0;

    /**
     * @brief Whether adaptive sampling is currently reducing the rate
     */
    bool is_throttling = false;

    /**
     * @brief Calculate the actual sampling ratio
     * @return Ratio of sampled messages to total messages
     */
    [[nodiscard]] double actual_ratio() const {
        if (total_count == 0) {
            return 1.0;
        }
        return static_cast<double>(sampled_count + bypassed_count) /
               static_cast<double>(total_count);
    }

    /**
     * @brief Reset all statistics
     */
    void reset() {
        total_count = 0;
        sampled_count = 0;
        dropped_count = 0;
        bypassed_count = 0;
        effective_rate = 1.0;
        is_throttling = false;
    }
};

} // namespace kcenon::logger::sampling
