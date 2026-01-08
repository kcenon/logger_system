// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <gtest/gtest.h>
#include <kcenon/logger/sampling/log_sampler.h>
#include <kcenon/logger/sampling/sampling_config.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

using log_level = logger_system::log_level;
using namespace kcenon::logger;
using namespace kcenon::logger::sampling;

namespace {

class memory_writer final : public base_writer {
public:
    struct entry_record {
        logger_system::log_level level = logger_system::log_level::info;
        std::string message;
    };

    kcenon::common::VoidResult write(logger_system::log_level level,
                                     const std::string& message,
                                     [[maybe_unused]] const std::string& file,
                                     [[maybe_unused]] int line,
                                     [[maybe_unused]] const std::string& function,
                                     [[maybe_unused]] const std::chrono::system_clock::time_point& timestamp) override {
        entry_record rec;
        rec.level = level;
        rec.message = message;
        records_.push_back(std::move(rec));
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult flush() override {
        return kcenon::common::ok();
    }

    std::string get_name() const override {
        return "memory_writer";
    }

    const std::vector<entry_record>& records() const {
        return records_;
    }

    void clear() {
        records_.clear();
    }

    size_t count() const {
        return records_.size();
    }

private:
    std::vector<entry_record> records_;
};

} // namespace

// =============================================================================
// sampling_config tests
// =============================================================================

TEST(SamplingConfigTest, DefaultConfigIsDisabled) {
    sampling_config config;
    EXPECT_FALSE(config.enabled);
    EXPECT_DOUBLE_EQ(config.rate, 1.0);
    EXPECT_EQ(config.strategy, sampling_strategy::random);
}

TEST(SamplingConfigTest, DisabledFactoryMethod) {
    auto config = sampling_config::disabled();
    EXPECT_FALSE(config.enabled);
}

TEST(SamplingConfigTest, RandomSamplingFactoryMethod) {
    auto config = sampling_config::random_sampling(0.5);
    EXPECT_TRUE(config.enabled);
    EXPECT_DOUBLE_EQ(config.rate, 0.5);
    EXPECT_EQ(config.strategy, sampling_strategy::random);
}

TEST(SamplingConfigTest, RateLimitedFactoryMethod) {
    auto config = sampling_config::rate_limited(500);
    EXPECT_TRUE(config.enabled);
    EXPECT_EQ(config.strategy, sampling_strategy::rate_limiting);
    EXPECT_EQ(config.rate_limit_per_second, 500U);
}

TEST(SamplingConfigTest, AdaptiveFactoryMethod) {
    auto config = sampling_config::adaptive(5000, 0.05);
    EXPECT_TRUE(config.enabled);
    EXPECT_EQ(config.strategy, sampling_strategy::adaptive);
    EXPECT_EQ(config.adaptive_threshold, 5000U);
    EXPECT_DOUBLE_EQ(config.adaptive_min_rate, 0.05);
}

TEST(SamplingConfigTest, HashBasedFactoryMethod) {
    auto config = sampling_config::hash_based(0.3, 12345);
    EXPECT_TRUE(config.enabled);
    EXPECT_EQ(config.strategy, sampling_strategy::hash_based);
    EXPECT_DOUBLE_EQ(config.rate, 0.3);
    EXPECT_EQ(config.hash_seed, 12345U);
}

TEST(SamplingConfigTest, AlwaysLogLevelsDefault) {
    sampling_config config;
    EXPECT_EQ(config.always_log_levels.size(), 2U);
    EXPECT_NE(std::find(config.always_log_levels.begin(),
                        config.always_log_levels.end(),
                        log_level::error),
              config.always_log_levels.end());
    EXPECT_NE(std::find(config.always_log_levels.begin(),
                        config.always_log_levels.end(),
                        log_level::fatal),
              config.always_log_levels.end());
}

// =============================================================================
// sampling_stats tests
// =============================================================================

TEST(SamplingStatsTest, InitialValuesAreZero) {
    sampling_stats stats;
    EXPECT_EQ(stats.total_count, 0U);
    EXPECT_EQ(stats.sampled_count, 0U);
    EXPECT_EQ(stats.dropped_count, 0U);
    EXPECT_EQ(stats.bypassed_count, 0U);
    EXPECT_DOUBLE_EQ(stats.effective_rate, 1.0);
    EXPECT_FALSE(stats.is_throttling);
}

TEST(SamplingStatsTest, ActualRatioWithZeroTotal) {
    sampling_stats stats;
    EXPECT_DOUBLE_EQ(stats.actual_ratio(), 1.0);
}

TEST(SamplingStatsTest, ActualRatioCalculation) {
    sampling_stats stats;
    stats.total_count = 100;
    stats.sampled_count = 45;
    stats.bypassed_count = 5;
    // actual_ratio = (sampled + bypassed) / total = 50/100 = 0.5
    EXPECT_DOUBLE_EQ(stats.actual_ratio(), 0.5);
}

TEST(SamplingStatsTest, Reset) {
    sampling_stats stats;
    stats.total_count = 100;
    stats.sampled_count = 50;
    stats.dropped_count = 50;
    stats.is_throttling = true;

    stats.reset();

    EXPECT_EQ(stats.total_count, 0U);
    EXPECT_EQ(stats.sampled_count, 0U);
    EXPECT_EQ(stats.dropped_count, 0U);
    EXPECT_FALSE(stats.is_throttling);
}

// =============================================================================
// log_sampler tests - Basic functionality
// =============================================================================

TEST(LogSamplerTest, DisabledSamplerPassesAll) {
    log_sampler sampler(sampling_config::disabled());

    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(sampler.should_sample(log_level::info, "test message"));
    }

    auto stats = sampler.get_stats();
    EXPECT_EQ(stats.total_count, 100U);
    EXPECT_EQ(stats.sampled_count, 100U);
    EXPECT_EQ(stats.dropped_count, 0U);
}

TEST(LogSamplerTest, FullRatePassesAll) {
    auto config = sampling_config::random_sampling(1.0);
    log_sampler sampler(config);

    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(sampler.should_sample(log_level::info, "test message"));
    }

    auto stats = sampler.get_stats();
    EXPECT_EQ(stats.sampled_count, 100U);
    EXPECT_EQ(stats.dropped_count, 0U);
}

TEST(LogSamplerTest, ZeroRateDropsAll) {
    auto config = sampling_config::random_sampling(0.0);
    config.always_log_levels.clear();  // Remove bypass levels
    log_sampler sampler(config);

    for (int i = 0; i < 100; ++i) {
        EXPECT_FALSE(sampler.should_sample(log_level::info, "test message"));
    }

    auto stats = sampler.get_stats();
    EXPECT_EQ(stats.sampled_count, 0U);
    EXPECT_EQ(stats.dropped_count, 100U);
}

TEST(LogSamplerTest, AlwaysLogLevelBypassesSampling) {
    auto config = sampling_config::random_sampling(0.0);  // Drop all
    config.always_log_levels = {log_level::error, log_level::fatal};
    log_sampler sampler(config);

    // Info should be dropped
    EXPECT_FALSE(sampler.should_sample(log_level::info, "info message"));

    // Error should bypass
    EXPECT_TRUE(sampler.should_sample(log_level::error, "error message"));

    // Fatal should bypass
    EXPECT_TRUE(sampler.should_sample(log_level::fatal, "fatal message"));

    auto stats = sampler.get_stats();
    EXPECT_EQ(stats.bypassed_count, 2U);
    EXPECT_EQ(stats.dropped_count, 1U);
}

// =============================================================================
// log_sampler tests - Random sampling
// =============================================================================

TEST(LogSamplerTest, RandomSamplingApproximatesRate) {
    auto config = sampling_config::random_sampling(0.5);
    config.always_log_levels.clear();  // Remove bypass levels
    log_sampler sampler(config);

    const int iterations = 10000;
    int sampled = 0;

    for (int i = 0; i < iterations; ++i) {
        if (sampler.should_sample(log_level::info, "test message " + std::to_string(i))) {
            ++sampled;
        }
    }

    // With 50% sampling, expect roughly 5000 samples
    // Allow 10% tolerance for randomness
    EXPECT_GT(sampled, iterations * 0.4);
    EXPECT_LT(sampled, iterations * 0.6);
}

TEST(LogSamplerTest, RandomSamplingLowRate) {
    auto config = sampling_config::random_sampling(0.1);
    config.always_log_levels.clear();
    log_sampler sampler(config);

    const int iterations = 10000;
    int sampled = 0;

    for (int i = 0; i < iterations; ++i) {
        if (sampler.should_sample(log_level::info, "test message " + std::to_string(i))) {
            ++sampled;
        }
    }

    // With 10% sampling, expect roughly 1000 samples
    EXPECT_GT(sampled, iterations * 0.05);
    EXPECT_LT(sampled, iterations * 0.15);
}

// =============================================================================
// log_sampler tests - Rate limiting
// =============================================================================

TEST(LogSamplerTest, RateLimitingBasic) {
    auto config = sampling_config::rate_limited(100);  // 100 per second
    config.always_log_levels.clear();
    log_sampler sampler(config);

    int sampled = 0;
    for (int i = 0; i < 200; ++i) {
        if (sampler.should_sample(log_level::info, "test message")) {
            ++sampled;
        }
    }

    // Should allow at most ~100 messages (within the window)
    EXPECT_LE(sampled, 110);  // Allow some tolerance
}

// =============================================================================
// log_sampler tests - Hash-based sampling
// =============================================================================

TEST(LogSamplerTest, HashBasedSamplingIsDeterministic) {
    auto config = sampling_config::hash_based(0.5, 12345);
    config.always_log_levels.clear();
    log_sampler sampler(config);

    // Same message should always get same result
    bool result1 = sampler.should_sample(log_level::info, "specific message");
    sampler.reset_stats();
    bool result2 = sampler.should_sample(log_level::info, "specific message");

    EXPECT_EQ(result1, result2);
}

TEST(LogSamplerTest, HashBasedSamplingDifferentMessages) {
    auto config = sampling_config::hash_based(0.5, 12345);
    config.always_log_levels.clear();
    log_sampler sampler(config);

    // Use more varied message patterns for better hash distribution
    const int iterations = 10000;
    int sampled = 0;

    for (int i = 0; i < iterations; ++i) {
        std::string msg = "log_event_" + std::to_string(i) + "_action_" +
                          std::to_string(i * 7 % 100);
        if (sampler.should_sample(log_level::info, msg)) {
            ++sampled;
        }
    }

    // Hash-based sampling should approximate the configured rate
    // Allow wider tolerance due to hash distribution properties
    double actual_rate = static_cast<double>(sampled) / static_cast<double>(iterations);
    EXPECT_GT(actual_rate, 0.2);  // At least 20%
    EXPECT_LT(actual_rate, 0.8);  // At most 80%
}

// =============================================================================
// log_sampler tests - Category-based sampling
// =============================================================================

TEST(LogSamplerTest, CategorySpecificRates) {
    sampling_config config;
    config.enabled = true;
    config.rate = 0.5;
    config.always_log_levels.clear();
    config.category_rates["database"] = 0.1;  // Only 10% of database logs
    config.category_rates["security"] = 1.0;   // All security logs

    log_sampler sampler(config);

    // Test security category - should all pass
    int security_passed = 0;
    for (int i = 0; i < 100; ++i) {
        if (sampler.should_sample(log_level::info, "security event", std::string("security"))) {
            ++security_passed;
        }
    }
    EXPECT_EQ(security_passed, 100);

    sampler.reset_stats();

    // Test database category - should have low pass rate
    int database_passed = 0;
    for (int i = 0; i < 1000; ++i) {
        if (sampler.should_sample(log_level::info, "db query " + std::to_string(i), std::string("database"))) {
            ++database_passed;
        }
    }
    EXPECT_GT(database_passed, 50);   // At least 5%
    EXPECT_LT(database_passed, 200);  // At most 20%
}

// =============================================================================
// log_sampler tests - Configuration updates
// =============================================================================

TEST(LogSamplerTest, ConfigCanBeUpdated) {
    log_sampler sampler(sampling_config::disabled());

    // Initially passes all
    EXPECT_TRUE(sampler.should_sample(log_level::info, "test"));

    // Update to drop all
    auto new_config = sampling_config::random_sampling(0.0);
    new_config.always_log_levels.clear();
    sampler.set_config(new_config);

    // Now should drop
    EXPECT_FALSE(sampler.should_sample(log_level::info, "test"));
}

TEST(LogSamplerTest, EnableDisableToggle) {
    auto config = sampling_config::random_sampling(0.0);
    config.always_log_levels.clear();
    log_sampler sampler(config);

    // Enabled by default via config
    EXPECT_TRUE(sampler.is_enabled());
    EXPECT_FALSE(sampler.should_sample(log_level::info, "test"));

    // Disable
    sampler.set_enabled(false);
    EXPECT_FALSE(sampler.is_enabled());
    EXPECT_TRUE(sampler.should_sample(log_level::info, "test"));  // Now passes

    // Re-enable
    sampler.set_enabled(true);
    EXPECT_TRUE(sampler.is_enabled());
    EXPECT_FALSE(sampler.should_sample(log_level::info, "test"));  // Back to dropping
}

// =============================================================================
// log_sampler tests - Statistics
// =============================================================================

TEST(LogSamplerTest, StatsAreAccurate) {
    auto config = sampling_config::random_sampling(0.5);
    config.always_log_levels = {log_level::error};
    log_sampler sampler(config);

    const int iterations = 1000;
    for (int i = 0; i < iterations; ++i) {
        (void)sampler.should_sample(log_level::info, "info " + std::to_string(i));
        (void)sampler.should_sample(log_level::error, "error " + std::to_string(i));  // Bypassed
    }

    auto stats = sampler.get_stats();

    // Total should be 2000
    EXPECT_EQ(stats.total_count, 2000U);

    // Bypassed should be 1000 (all error messages)
    EXPECT_EQ(stats.bypassed_count, 1000U);

    // Sampled + dropped should equal iterations (1000 info messages)
    EXPECT_EQ(stats.sampled_count + stats.dropped_count, 1000U);
}

TEST(LogSamplerTest, ResetStats) {
    auto config = sampling_config::random_sampling(0.5);
    log_sampler sampler(config);

    for (int i = 0; i < 100; ++i) {
        (void)sampler.should_sample(log_level::info, "test");
    }

    auto stats = sampler.get_stats();
    EXPECT_GT(stats.total_count, 0U);

    sampler.reset_stats();

    stats = sampler.get_stats();
    EXPECT_EQ(stats.total_count, 0U);
    EXPECT_EQ(stats.sampled_count, 0U);
    EXPECT_EQ(stats.dropped_count, 0U);
}

// =============================================================================
// sampler_factory tests
// =============================================================================

TEST(SamplerFactoryTest, CreateDisabled) {
    auto sampler = sampler_factory::create_disabled();
    EXPECT_NE(sampler, nullptr);
    EXPECT_FALSE(sampler->is_enabled());
}

TEST(SamplerFactoryTest, CreateRandom) {
    auto sampler = sampler_factory::create_random(0.3);
    EXPECT_NE(sampler, nullptr);
    EXPECT_TRUE(sampler->is_enabled());
}

TEST(SamplerFactoryTest, CreateRateLimited) {
    auto sampler = sampler_factory::create_rate_limited(500);
    EXPECT_NE(sampler, nullptr);
    EXPECT_TRUE(sampler->is_enabled());
}

TEST(SamplerFactoryTest, CreateAdaptive) {
    auto sampler = sampler_factory::create_adaptive(10000, 0.01);
    EXPECT_NE(sampler, nullptr);
    EXPECT_TRUE(sampler->is_enabled());
}

TEST(SamplerFactoryTest, CreateProduction) {
    auto sampler = sampler_factory::create_production(0.1);
    EXPECT_NE(sampler, nullptr);
    EXPECT_TRUE(sampler->is_enabled());
}

// =============================================================================
// Integration tests - Logger with sampling
// =============================================================================

TEST(LoggerSamplingTest, LoggerWithSampler) {
    logger log(false);  // Sync mode
    auto writer = std::make_unique<memory_writer>();
    auto* writer_ptr = writer.get();

    log.add_writer(std::move(writer));

    // Set a sampler that drops all (rate=0)
    auto config = sampling_config::random_sampling(0.0);
    config.always_log_levels = {log_level::error, log_level::fatal};
    log.set_sampler(std::make_unique<log_sampler>(config));

    // Info messages should be dropped
    for (int i = 0; i < 10; ++i) {
        log.log(log_level::info, "info message " + std::to_string(i));
    }
    EXPECT_EQ(writer_ptr->count(), 0U);

    // Error messages should pass (bypass level)
    for (int i = 0; i < 5; ++i) {
        log.log(log_level::error, "error message " + std::to_string(i));
    }
    EXPECT_EQ(writer_ptr->count(), 5U);
}

TEST(LoggerSamplingTest, LoggerHasSampling) {
    logger log(false);

    EXPECT_FALSE(log.has_sampling());

    log.set_sampler(std::make_unique<log_sampler>(sampling_config::random_sampling(0.5)));

    EXPECT_TRUE(log.has_sampling());
}

TEST(LoggerSamplingTest, LoggerSamplingStats) {
    logger log(false);
    auto writer = std::make_unique<memory_writer>();
    log.add_writer(std::move(writer));

    auto config = sampling_config::random_sampling(0.5);
    config.always_log_levels.clear();
    log.set_sampler(std::make_unique<log_sampler>(config));

    for (int i = 0; i < 100; ++i) {
        log.log(log_level::info, "message " + std::to_string(i));
    }

    auto stats = log.get_sampling_stats();
    EXPECT_EQ(stats.total_count, 100U);
    EXPECT_GT(stats.sampled_count, 0U);
    EXPECT_GT(stats.dropped_count, 0U);
}

// =============================================================================
// Integration tests - Builder with sampling
// =============================================================================

TEST(LoggerBuilderSamplingTest, WithSampler) {
    auto result = logger_builder()
        .with_sampler(sampler_factory::create_random(0.5))
        .with_async(false)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& log = result.value();
    EXPECT_TRUE(log->has_sampling());
}

TEST(LoggerBuilderSamplingTest, WithSamplingConfig) {
    sampling_config config;
    config.enabled = true;
    config.rate = 0.3;
    config.strategy = sampling_strategy::random;

    auto result = logger_builder()
        .with_sampling(config)
        .with_async(false)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& log = result.value();
    EXPECT_TRUE(log->has_sampling());
}

TEST(LoggerBuilderSamplingTest, WithRandomSampling) {
    auto result = logger_builder()
        .with_random_sampling(0.1)
        .with_async(false)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& log = result.value();
    EXPECT_TRUE(log->has_sampling());
}

TEST(LoggerBuilderSamplingTest, WithRateLimiting) {
    auto result = logger_builder()
        .with_rate_limiting(1000)
        .with_async(false)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& log = result.value();
    EXPECT_TRUE(log->has_sampling());
}

TEST(LoggerBuilderSamplingTest, WithAdaptiveSampling) {
    auto result = logger_builder()
        .with_adaptive_sampling(50000, 0.01)
        .with_async(false)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& log = result.value();
    EXPECT_TRUE(log->has_sampling());
}
