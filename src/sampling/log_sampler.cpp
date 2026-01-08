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

#include <kcenon/logger/sampling/log_sampler.h>

#include <algorithm>
#include <cstring>

namespace kcenon::logger::sampling {

// =============================================================================
// log_sampler implementation
// =============================================================================

log_sampler::log_sampler(const sampling_config& config)
    : config_(config),
      rng_state_(static_cast<std::uint64_t>(
          std::chrono::steady_clock::now().time_since_epoch().count()) | 1) {
    // Initialize rate limiting window
    auto now = std::chrono::steady_clock::now();
    rate_limit_window_start_.store(now.time_since_epoch().count());
    adaptive_window_start_.store(now.time_since_epoch().count());
    effective_rate_.store(config_.rate);
}

log_sampler::log_sampler(log_sampler&& other) noexcept
    : config_(std::move(other.config_)),
      total_count_(other.total_count_.load()),
      sampled_count_(other.sampled_count_.load()),
      dropped_count_(other.dropped_count_.load()),
      bypassed_count_(other.bypassed_count_.load()),
      rng_state_(other.rng_state_.load()),
      rate_limit_count_(other.rate_limit_count_.load()),
      rate_limit_window_start_(other.rate_limit_window_start_.load()),
      effective_rate_(other.effective_rate_.load()),
      adaptive_window_count_(other.adaptive_window_count_.load()),
      adaptive_window_start_(other.adaptive_window_start_.load()),
      is_throttling_(other.is_throttling_.load()) {
}

log_sampler& log_sampler::operator=(log_sampler&& other) noexcept {
    if (this != &other) {
        std::unique_lock<std::shared_mutex> lock(config_mutex_);
        config_ = std::move(other.config_);
        total_count_.store(other.total_count_.load());
        sampled_count_.store(other.sampled_count_.load());
        dropped_count_.store(other.dropped_count_.load());
        bypassed_count_.store(other.bypassed_count_.load());
        rng_state_.store(other.rng_state_.load());
        rate_limit_count_.store(other.rate_limit_count_.load());
        rate_limit_window_start_.store(other.rate_limit_window_start_.load());
        effective_rate_.store(other.effective_rate_.load());
        adaptive_window_count_.store(other.adaptive_window_count_.load());
        adaptive_window_start_.store(other.adaptive_window_start_.load());
        is_throttling_.store(other.is_throttling_.load());
    }
    return *this;
}

bool log_sampler::should_sample(const log_entry& entry) {
    std::optional<std::string> category;
    if (entry.category.has_value()) {
        category = entry.category->to_string();
    }
    return should_sample(entry.level, entry.message.to_string(), category);
}

bool log_sampler::should_sample(logger_system::log_level level,
                                const std::string& message) {
    return should_sample(level, message, std::nullopt);
}

bool log_sampler::should_sample(logger_system::log_level level,
                                const std::string& message,
                                const std::optional<std::string>& category) {
    // Increment total count
    total_count_.fetch_add(1, std::memory_order_relaxed);

    // Check if sampling is enabled
    bool enabled = false;
    sampling_strategy strategy = sampling_strategy::random;
    double rate = 1.0;
    {
        std::shared_lock<std::shared_mutex> lock(config_mutex_);
        enabled = config_.enabled;
        strategy = config_.strategy;
        rate = config_.rate;
    }

    if (!enabled) {
        sampled_count_.fetch_add(1, std::memory_order_relaxed);
        return true;  // Pass through when disabled
    }

    // Check if level bypasses sampling
    if (should_bypass_level(level)) {
        bypassed_count_.fetch_add(1, std::memory_order_relaxed);
        return true;
    }

    // Get category-specific rate if applicable
    double effective = rate;
    if (category.has_value()) {
        effective = get_category_rate(category.value());
    }

    // Apply the configured strategy
    bool sampled = false;
    switch (strategy) {
        case sampling_strategy::random:
            sampled = random_sample(effective);
            break;
        case sampling_strategy::rate_limiting:
            sampled = rate_limit_sample();
            break;
        case sampling_strategy::adaptive:
            sampled = adaptive_sample();
            break;
        case sampling_strategy::hash_based:
            sampled = hash_sample(message, effective);
            break;
    }

    if (sampled) {
        sampled_count_.fetch_add(1, std::memory_order_relaxed);
    } else {
        dropped_count_.fetch_add(1, std::memory_order_relaxed);
    }

    return sampled;
}

void log_sampler::set_config(const sampling_config& config) {
    std::unique_lock<std::shared_mutex> lock(config_mutex_);
    config_ = config;
    effective_rate_.store(config.rate);
}

sampling_config log_sampler::get_config() const {
    std::shared_lock<std::shared_mutex> lock(config_mutex_);
    return config_;
}

sampling_stats log_sampler::get_stats() const {
    sampling_stats stats;
    stats.total_count = total_count_.load(std::memory_order_relaxed);
    stats.sampled_count = sampled_count_.load(std::memory_order_relaxed);
    stats.dropped_count = dropped_count_.load(std::memory_order_relaxed);
    stats.bypassed_count = bypassed_count_.load(std::memory_order_relaxed);
    stats.effective_rate = effective_rate_.load(std::memory_order_relaxed);
    stats.is_throttling = is_throttling_.load(std::memory_order_relaxed);
    return stats;
}

void log_sampler::reset_stats() {
    total_count_.store(0, std::memory_order_relaxed);
    sampled_count_.store(0, std::memory_order_relaxed);
    dropped_count_.store(0, std::memory_order_relaxed);
    bypassed_count_.store(0, std::memory_order_relaxed);
}

bool log_sampler::is_enabled() const {
    std::shared_lock<std::shared_mutex> lock(config_mutex_);
    return config_.enabled;
}

void log_sampler::set_enabled(bool enabled) {
    std::unique_lock<std::shared_mutex> lock(config_mutex_);
    config_.enabled = enabled;
}

double log_sampler::get_effective_rate() const {
    return effective_rate_.load(std::memory_order_relaxed);
}

bool log_sampler::should_bypass_level(logger_system::log_level level) const {
    std::shared_lock<std::shared_mutex> lock(config_mutex_);
    const auto& levels = config_.always_log_levels;
    return std::find(levels.begin(), levels.end(), level) != levels.end();
}

double log_sampler::get_category_rate(const std::string& category) const {
    std::shared_lock<std::shared_mutex> lock(config_mutex_);
    auto it = config_.category_rates.find(category);
    if (it != config_.category_rates.end()) {
        return it->second;
    }
    return config_.rate;
}

bool log_sampler::random_sample(double rate) {
    if (rate >= 1.0) {
        return true;
    }
    if (rate <= 0.0) {
        return false;
    }

    // Generate random number and compare with threshold
    std::uint64_t random = xorshift64();
    double normalized = static_cast<double>(random) /
                        static_cast<double>(std::numeric_limits<std::uint64_t>::max());
    return normalized < rate;
}

bool log_sampler::rate_limit_sample() {
    auto now = std::chrono::steady_clock::now();
    auto now_count = now.time_since_epoch().count();

    std::size_t limit_per_second = 0;
    std::size_t window_ms = 0;
    {
        std::shared_lock<std::shared_mutex> lock(config_mutex_);
        limit_per_second = config_.rate_limit_per_second;
        window_ms = config_.rate_limit_window_ms;
    }

    // Calculate max messages per window
    std::size_t max_per_window = (limit_per_second * window_ms) / 1000;
    if (max_per_window == 0) {
        max_per_window = 1;
    }

    // Check if we need to reset the window
    auto window_start = std::chrono::steady_clock::time_point(
        std::chrono::steady_clock::duration(rate_limit_window_start_.load()));
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - window_start).count();

    if (static_cast<std::size_t>(elapsed_ms) >= window_ms) {
        // Reset window
        std::lock_guard<std::mutex> lock(rate_limit_mutex_);
        // Double-check after acquiring lock
        window_start = std::chrono::steady_clock::time_point(
            std::chrono::steady_clock::duration(rate_limit_window_start_.load()));
        elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - window_start).count();

        if (static_cast<std::size_t>(elapsed_ms) >= window_ms) {
            rate_limit_window_start_.store(now_count);
            rate_limit_count_.store(0);
        }
    }

    // Check if within limit
    std::uint64_t current = rate_limit_count_.fetch_add(1, std::memory_order_relaxed);
    return current < max_per_window;
}

bool log_sampler::adaptive_sample() {
    // Update adaptive rate if needed
    update_adaptive_rate();

    // Use effective rate for sampling
    double rate = effective_rate_.load(std::memory_order_relaxed);
    return random_sample(rate);
}

void log_sampler::update_adaptive_rate() {
    auto now = std::chrono::steady_clock::now();
    auto now_count = now.time_since_epoch().count();

    std::size_t threshold = 0;
    double min_rate = 0.01;
    double base_rate = 1.0;
    {
        std::shared_lock<std::shared_mutex> lock(config_mutex_);
        threshold = config_.adaptive_threshold;
        min_rate = config_.adaptive_min_rate;
        base_rate = config_.rate;
    }

    // Increment window count
    adaptive_window_count_.fetch_add(1, std::memory_order_relaxed);

    // Check if we need to recalculate rate (every second)
    auto window_start = std::chrono::steady_clock::time_point(
        std::chrono::steady_clock::duration(adaptive_window_start_.load()));
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - window_start).count();

    if (elapsed_ms >= 1000) {
        // Calculate messages per second
        std::uint64_t count = adaptive_window_count_.exchange(0);
        adaptive_window_start_.store(now_count);

        if (count > threshold) {
            // Calculate new rate based on how much we're over threshold
            double ratio = static_cast<double>(threshold) / static_cast<double>(count);
            double new_rate = base_rate * ratio;
            new_rate = std::max(new_rate, min_rate);
            effective_rate_.store(new_rate);
            is_throttling_.store(true);
        } else {
            // Reset to base rate
            effective_rate_.store(base_rate);
            is_throttling_.store(false);
        }
    }
}

bool log_sampler::hash_sample(const std::string& message, double rate) {
    if (rate >= 1.0) {
        return true;
    }
    if (rate <= 0.0) {
        return false;
    }

    std::uint64_t seed = 0;
    {
        std::shared_lock<std::shared_mutex> lock(config_mutex_);
        seed = config_.hash_seed;
    }

    // Compute hash of message
    std::uint64_t hash = fnv1a_hash(message);
    hash ^= seed;

    // Normalize hash to [0, 1) and compare with rate
    double normalized = static_cast<double>(hash) /
                        static_cast<double>(std::numeric_limits<std::uint64_t>::max());
    return normalized < rate;
}

std::uint64_t log_sampler::xorshift64() {
    std::uint64_t x = rng_state_.load(std::memory_order_relaxed);
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    rng_state_.store(x, std::memory_order_relaxed);
    return x;
}

std::uint64_t log_sampler::fnv1a_hash(const std::string& str) {
    constexpr std::uint64_t kFnvOffsetBasis = 14695981039346656037ULL;
    constexpr std::uint64_t kFnvPrime = 1099511628211ULL;

    std::uint64_t hash = kFnvOffsetBasis;
    for (char c : str) {
        hash ^= static_cast<std::uint64_t>(static_cast<unsigned char>(c));
        hash *= kFnvPrime;
    }
    return hash;
}

// =============================================================================
// sampler_factory implementation
// =============================================================================

std::unique_ptr<log_sampler> sampler_factory::create_disabled() {
    return std::make_unique<log_sampler>(sampling_config::disabled());
}

std::unique_ptr<log_sampler> sampler_factory::create_random(double rate) {
    return std::make_unique<log_sampler>(sampling_config::random_sampling(rate));
}

std::unique_ptr<log_sampler> sampler_factory::create_rate_limited(std::size_t max_per_second) {
    return std::make_unique<log_sampler>(sampling_config::rate_limited(max_per_second));
}

std::unique_ptr<log_sampler> sampler_factory::create_adaptive(
    std::size_t threshold,
    double min_rate) {
    return std::make_unique<log_sampler>(sampling_config::adaptive(threshold, min_rate));
}

std::unique_ptr<log_sampler> sampler_factory::create_production(
    double base_rate,
    std::vector<logger_system::log_level> critical_levels) {
    sampling_config config;
    config.enabled = true;
    config.rate = base_rate;
    config.strategy = sampling_strategy::adaptive;
    config.adaptive_enabled = true;
    config.adaptive_threshold = 50000;
    config.adaptive_min_rate = 0.01;
    config.always_log_levels = std::move(critical_levels);
    return std::make_unique<log_sampler>(config);
}

} // namespace kcenon::logger::sampling
