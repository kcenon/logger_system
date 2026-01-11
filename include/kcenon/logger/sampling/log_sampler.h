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
 * @file log_sampler.h
 * @brief Log sampling implementation for high-volume scenarios
 * @author 🍀☀🌕🌥 🌊
 * @since 3.3.0
 *
 * @details Provides log sampling functionality to reduce log volume while
 * preserving critical messages. Supports multiple sampling strategies:
 * - Random: Simple probabilistic sampling
 * - Rate Limiting: Maximum N logs per time window
 * - Adaptive: Automatically adjusts based on volume
 * - Hash-based: Deterministic sampling for debugging
 *
 * @example Basic usage:
 * @code
 * auto config = sampling_config::random_sampling(0.1);  // 10% sampling
 * config.always_log_levels = {log_level::error, log_level::fatal};
 *
 * log_sampler sampler(config);
 *
 * log_entry entry(log_level::info, "Message");
 * if (sampler.should_sample(entry)) {
 *     // Log the message
 * }
 * @endcode
 */

#include "sampling_config.h"
#include <kcenon/logger/interfaces/log_entry.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <optional>

namespace kcenon::logger::sampling {

/**
 * @class log_sampler
 * @brief Thread-safe log sampler with multiple strategy support
 *
 * @details The log_sampler class determines whether a log entry should be
 * logged based on the configured sampling strategy. It maintains thread-safe
 * statistics and supports runtime configuration updates.
 *
 * Key features:
 * - Thread-safe operation for high-concurrency scenarios
 * - Multiple sampling strategies (random, rate limiting, adaptive, hash-based)
 * - Level-based bypass for critical messages
 * - Per-category sampling rates
 * - Real-time statistics tracking
 *
 * @note The sampler uses fast PRNG (xorshift64) for random sampling to minimize
 * overhead in the hot path.
 *
 * @since 3.3.0
 */
class log_sampler {
public:
    /**
     * @brief Construct a sampler with the given configuration
     * @param config Sampling configuration
     */
    explicit log_sampler(const sampling_config& config = sampling_config{});

    /**
     * @brief Destructor
     */
    ~log_sampler() = default;

    /**
     * @brief Copy constructor (deleted)
     * @details Samplers contain atomic state and should not be copied
     */
    log_sampler(const log_sampler&) = delete;

    /**
     * @brief Copy assignment operator (deleted)
     */
    log_sampler& operator=(const log_sampler&) = delete;

    /**
     * @brief Move constructor
     */
    log_sampler(log_sampler&& other) noexcept;

    /**
     * @brief Move assignment operator
     */
    log_sampler& operator=(log_sampler&& other) noexcept;

    /**
     * @brief Check if a log entry should be sampled (logged)
     * @param entry The log entry to check
     * @return true if the entry should be logged, false if it should be dropped
     *
     * @details This is the main entry point for sampling decisions. It considers:
     * 1. Whether sampling is enabled
     * 2. Whether the log level bypasses sampling
     * 3. Category-specific sampling rates
     * 4. The configured sampling strategy
     *
     * @note Thread-safe. Can be called from multiple threads concurrently.
     */
    [[nodiscard]] bool should_sample(const log_entry& entry);

    /**
     * @brief Check if a log entry should be sampled (without category)
     * @param level Log level
     * @param message Log message (used for hash-based sampling)
     * @return true if the entry should be logged
     */
    [[nodiscard]] bool should_sample(logger_system::log_level level,
                                     const std::string& message);

    /**
     * @brief Check if a log entry should be sampled with category
     * @param level Log level
     * @param message Log message
     * @param category Optional category for category-specific rates
     * @return true if the entry should be logged
     */
    [[nodiscard]] bool should_sample(logger_system::log_level level,
                                     const std::string& message,
                                     const std::optional<std::string>& category);

    /**
     * @brief Update the sampling configuration
     * @param config New configuration to apply
     *
     * @note Thread-safe. Takes effect for subsequent sampling decisions.
     */
    void set_config(const sampling_config& config);

    /**
     * @brief Get the current configuration
     * @return Copy of the current configuration
     */
    [[nodiscard]] sampling_config get_config() const;

    /**
     * @brief Get sampling statistics
     * @return Copy of current statistics
     *
     * @note Thread-safe. Returns a snapshot of current statistics.
     */
    [[nodiscard]] sampling_stats get_stats() const;

    /**
     * @brief Reset sampling statistics
     *
     * @note Thread-safe. Does not affect the configuration.
     */
    void reset_stats();

    /**
     * @brief Check if sampling is enabled
     * @return true if sampling is currently enabled
     */
    [[nodiscard]] bool is_enabled() const;

    /**
     * @brief Enable or disable sampling
     * @param enabled true to enable, false to disable
     */
    void set_enabled(bool enabled);

    /**
     * @brief Get the current effective sampling rate
     * @return Current effective rate (may differ from configured rate in adaptive mode)
     */
    [[nodiscard]] double get_effective_rate() const;

private:
    /**
     * @brief Check if a level should bypass sampling
     * @param level Log level to check
     * @return true if the level should always be logged
     */
    [[nodiscard]] bool should_bypass_level(logger_system::log_level level) const;

    /**
     * @brief Check if any field should bypass sampling
     * @param entry Log entry to check
     * @return true if any configured bypass field is present
     * @since 3.4.0
     */
    [[nodiscard]] bool should_bypass_field(const log_entry& entry) const;

    /**
     * @brief Get the sampling rate based on structured fields
     * @param entry Log entry with fields to check
     * @return Sampling rate for the matching field value, or -1 if no match
     * @since 3.4.0
     */
    [[nodiscard]] double get_field_rate(const log_entry& entry) const;

    /**
     * @brief Get the sampling rate for a category
     * @param category Category name (empty for default rate)
     * @return Sampling rate for the category
     */
    [[nodiscard]] double get_category_rate(const std::string& category) const;

    /**
     * @brief Perform random sampling decision
     * @param rate Sampling rate to use
     * @return true if the message should be sampled (logged)
     */
    [[nodiscard]] bool random_sample(double rate);

    /**
     * @brief Perform rate limiting sampling decision
     * @return true if the message is within rate limits
     */
    [[nodiscard]] bool rate_limit_sample();

    /**
     * @brief Perform adaptive sampling decision
     * @return true if the message should be sampled
     */
    [[nodiscard]] bool adaptive_sample();

    /**
     * @brief Perform hash-based sampling decision
     * @param message Message to hash
     * @param rate Sampling rate to use
     * @return true if the message should be sampled
     */
    [[nodiscard]] bool hash_sample(const std::string& message, double rate);

    /**
     * @brief Update adaptive sampling rate based on current throughput
     */
    void update_adaptive_rate();

    /**
     * @brief Fast xorshift64 PRNG for random sampling
     * @return Random number in range [0, UINT64_MAX]
     */
    [[nodiscard]] std::uint64_t xorshift64();

    /**
     * @brief FNV-1a hash for message hashing
     * @param str String to hash
     * @return 64-bit hash value
     */
    [[nodiscard]] static std::uint64_t fnv1a_hash(const std::string& str);

    // Configuration
    sampling_config config_;
    mutable std::shared_mutex config_mutex_;

    // Statistics (atomic for lock-free access)
    std::atomic<std::uint64_t> total_count_{0};
    std::atomic<std::uint64_t> sampled_count_{0};
    std::atomic<std::uint64_t> dropped_count_{0};
    std::atomic<std::uint64_t> bypassed_count_{0};

    // Random sampling state
    std::atomic<std::uint64_t> rng_state_;

    // Rate limiting state
    std::atomic<std::uint64_t> rate_limit_count_{0};
    std::atomic<std::chrono::steady_clock::time_point::rep> rate_limit_window_start_{0};
    std::mutex rate_limit_mutex_;

    // Adaptive sampling state
    std::atomic<double> effective_rate_{1.0};
    std::atomic<std::uint64_t> adaptive_window_count_{0};
    std::atomic<std::chrono::steady_clock::time_point::rep> adaptive_window_start_{0};
    std::atomic<bool> is_throttling_{false};
};

/**
 * @class sampler_factory
 * @brief Factory for creating pre-configured samplers
 *
 * @since 3.3.0
 */
class sampler_factory {
public:
    /**
     * @brief Create a disabled sampler (pass-through)
     * @return Sampler that passes all logs
     */
    static std::unique_ptr<log_sampler> create_disabled();

    /**
     * @brief Create a random sampling sampler
     * @param rate Sampling rate (0.0 to 1.0)
     * @return Configured random sampler
     */
    static std::unique_ptr<log_sampler> create_random(double rate);

    /**
     * @brief Create a rate limiting sampler
     * @param max_per_second Maximum logs per second
     * @return Configured rate limiting sampler
     */
    static std::unique_ptr<log_sampler> create_rate_limited(std::size_t max_per_second);

    /**
     * @brief Create an adaptive sampler
     * @param threshold Threshold to trigger adaptation
     * @param min_rate Minimum rate under load
     * @return Configured adaptive sampler
     */
    static std::unique_ptr<log_sampler> create_adaptive(
        std::size_t threshold = 10000,
        double min_rate = 0.01);

    /**
     * @brief Create a production-ready sampler
     * @param base_rate Base sampling rate
     * @param critical_levels Levels that bypass sampling
     * @return Production-configured sampler
     */
    static std::unique_ptr<log_sampler> create_production(
        double base_rate = 0.1,
        std::vector<logger_system::log_level> critical_levels = {
            logger_system::log_level::warn,
            logger_system::log_level::error,
            logger_system::log_level::fatal
        });
};

} // namespace kcenon::logger::sampling
