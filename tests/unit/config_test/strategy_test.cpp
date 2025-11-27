/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>
#include <kcenon/logger/core/strategies/config_strategy_interface.h>
#include <kcenon/logger/core/strategies/environment_strategy.h>
#include <kcenon/logger/core/strategies/performance_strategy.h>
#include <kcenon/logger/core/strategies/deployment_strategy.h>
#include <kcenon/logger/core/strategies/composite_strategy.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/factories/writer_factory.h>
#include <kcenon/logger/factories/formatter_factory.h>
#include <kcenon/logger/factories/filter_factory.h>
#include <chrono>
#include <cstdlib>

using namespace kcenon::logger;

class ConfigStrategyTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset any environment variables
        unsetenv("LOG_ENV");
        unsetenv("LOG_LEVEL");
        unsetenv("LOG_ASYNC");
        unsetenv("LOG_BUFFER_SIZE");
        unsetenv("LOG_BATCH_SIZE");
        unsetenv("LOG_COLOR");
        unsetenv("LOG_METRICS");
    }

    void TearDown() override {
        // Clean up
        unsetenv("LOG_ENV");
        unsetenv("LOG_LEVEL");
        unsetenv("LOG_ASYNC");
        unsetenv("LOG_BUFFER_SIZE");
        unsetenv("LOG_BATCH_SIZE");
        unsetenv("LOG_COLOR");
        unsetenv("LOG_METRICS");
    }
};

//============================================================================
// Performance Strategy Tests
//============================================================================

TEST_F(ConfigStrategyTest, PerformanceStrategy_LowLatency) {
    performance_strategy strategy(performance_level::low_latency);

    EXPECT_EQ(strategy.get_name(), "performance:low_latency");
    EXPECT_EQ(strategy.priority(), 50);

    logger_config config;
    strategy.apply(config);

    // Verify low latency settings
    EXPECT_TRUE(config.async);
    EXPECT_EQ(config.buffer_size, 4096);
    EXPECT_EQ(config.batch_size, 10);
    EXPECT_EQ(config.flush_interval, std::chrono::milliseconds(10));
    EXPECT_TRUE(config.use_lock_free);
    EXPECT_FALSE(config.enable_batch_writing);
}

TEST_F(ConfigStrategyTest, PerformanceStrategy_Balanced) {
    performance_strategy strategy(performance_level::balanced);

    EXPECT_EQ(strategy.get_name(), "performance:balanced");

    logger_config config;
    strategy.apply(config);

    // Verify balanced settings
    EXPECT_TRUE(config.async);
    EXPECT_EQ(config.buffer_size, 8192);
    EXPECT_EQ(config.batch_size, 100);
    EXPECT_EQ(config.flush_interval, std::chrono::milliseconds(1000));
    EXPECT_TRUE(config.enable_batch_writing);
}

TEST_F(ConfigStrategyTest, PerformanceStrategy_HighThroughput) {
    performance_strategy strategy(performance_level::high_throughput);

    EXPECT_EQ(strategy.get_name(), "performance:high_throughput");

    logger_config config;
    strategy.apply(config);

    // Verify high throughput settings
    EXPECT_TRUE(config.async);
    EXPECT_EQ(config.buffer_size, 65536);
    EXPECT_EQ(config.batch_size, 500);
    EXPECT_EQ(config.flush_interval, std::chrono::milliseconds(5000));
    EXPECT_TRUE(config.use_lock_free);
    EXPECT_EQ(config.max_queue_size, 100000);
    EXPECT_EQ(config.writer_thread_count, 2);
    EXPECT_TRUE(config.enable_compression);
    EXPECT_TRUE(config.enable_batch_writing);
}

TEST_F(ConfigStrategyTest, PerformanceStrategy_MinimalOverhead) {
    performance_strategy strategy(performance_level::minimal_overhead);

    EXPECT_EQ(strategy.get_name(), "performance:minimal_overhead");

    logger_config config;
    strategy.apply(config);

    // Verify minimal overhead settings
    EXPECT_FALSE(config.enable_metrics);
    EXPECT_FALSE(config.enable_structured_logging);
    EXPECT_FALSE(config.enable_source_location);
    EXPECT_FALSE(config.enable_color_output);
}

//============================================================================
// Deployment Strategy Tests
//============================================================================

TEST_F(ConfigStrategyTest, DeploymentStrategy_Development) {
    deployment_strategy strategy(deployment_env::development);

    EXPECT_EQ(strategy.get_name(), "deployment:development");
    EXPECT_EQ(strategy.priority(), 75);

    logger_config config;
    strategy.apply(config);

    // Verify development settings
    EXPECT_FALSE(config.async);
    EXPECT_EQ(config.min_level, logger_system::log_level::trace);
    EXPECT_TRUE(config.enable_color_output);
    EXPECT_TRUE(config.enable_source_location);
    EXPECT_EQ(config.batch_size, 1);
}

TEST_F(ConfigStrategyTest, DeploymentStrategy_Staging) {
    deployment_strategy strategy(deployment_env::staging);

    EXPECT_EQ(strategy.get_name(), "deployment:staging");

    logger_config config;
    strategy.apply(config);

    // Verify staging settings
    EXPECT_TRUE(config.async);
    EXPECT_EQ(config.min_level, logger_system::log_level::info);
    EXPECT_TRUE(config.enable_structured_logging);
    EXPECT_TRUE(config.enable_batch_writing);
}

TEST_F(ConfigStrategyTest, DeploymentStrategy_Production) {
    deployment_strategy strategy(deployment_env::production);

    EXPECT_EQ(strategy.get_name(), "deployment:production");

    logger_config config;
    strategy.apply(config);

    // Verify production settings
    EXPECT_TRUE(config.async);
    EXPECT_EQ(config.min_level, logger_system::log_level::warn);
    EXPECT_TRUE(config.enable_crash_handler);
    EXPECT_FALSE(config.enable_color_output);
    EXPECT_TRUE(config.enable_structured_logging);
    EXPECT_TRUE(config.enable_compression);
}

TEST_F(ConfigStrategyTest, DeploymentStrategy_Testing) {
    deployment_strategy strategy(deployment_env::testing);

    EXPECT_EQ(strategy.get_name(), "deployment:testing");

    logger_config config;
    strategy.apply(config);

    // Verify testing settings
    EXPECT_FALSE(config.async);
    EXPECT_EQ(config.min_level, logger_system::log_level::trace);
    EXPECT_FALSE(config.enable_crash_handler);
    EXPECT_TRUE(config.enable_source_location);
}

//============================================================================
// Environment Strategy Tests
//============================================================================

TEST_F(ConfigStrategyTest, EnvironmentStrategy_LogLevel) {
    setenv("LOG_LEVEL", "error", 1);

    environment_strategy strategy;
    EXPECT_TRUE(strategy.is_applicable());
    EXPECT_EQ(strategy.priority(), 100);

    logger_config config;
    strategy.apply(config);

    EXPECT_EQ(config.min_level, logger_system::log_level::error);
}

TEST_F(ConfigStrategyTest, EnvironmentStrategy_MultipleVars) {
    setenv("LOG_LEVEL", "debug", 1);
    setenv("LOG_ASYNC", "false", 1);
    setenv("LOG_BUFFER_SIZE", "16384", 1);
    setenv("LOG_COLOR", "true", 1);
    setenv("LOG_METRICS", "true", 1);

    environment_strategy strategy;
    EXPECT_TRUE(strategy.is_applicable());

    logger_config config;
    strategy.apply(config);

    EXPECT_EQ(config.min_level, logger_system::log_level::debug);
    EXPECT_FALSE(config.async);
    EXPECT_EQ(config.buffer_size, 16384);
    EXPECT_TRUE(config.enable_color_output);
    EXPECT_TRUE(config.enable_metrics);
}

TEST_F(ConfigStrategyTest, EnvironmentStrategy_NotApplicable) {
    // No LOG_* environment variables set
    environment_strategy strategy;
    EXPECT_FALSE(strategy.is_applicable());
}

//============================================================================
// Composite Strategy Tests
//============================================================================

TEST_F(ConfigStrategyTest, CompositeStrategy_AppliesInOrder) {
    composite_strategy composite;

    // Add strategies with different priorities
    composite.add<deployment_strategy>(deployment_env::development);  // priority 75
    composite.add<performance_strategy>(performance_level::high_throughput);  // priority 50

    EXPECT_EQ(composite.size(), 2);

    logger_config config;
    composite.apply(config);

    // Deployment (higher priority) applied first, then performance
    // Performance strategy overrides async to true
    EXPECT_TRUE(config.async);  // High throughput sets this
    EXPECT_TRUE(config.enable_color_output);  // Development sets this
}

TEST_F(ConfigStrategyTest, CompositeStrategy_Empty) {
    composite_strategy composite;

    EXPECT_TRUE(composite.empty());

    logger_config config;
    logger_config original = config;

    composite.apply(config);

    // Config should be unchanged
    EXPECT_EQ(config.buffer_size, original.buffer_size);
}

TEST_F(ConfigStrategyTest, CompositeStrategy_Clear) {
    composite_strategy composite;
    composite.add<deployment_strategy>(deployment_env::production);

    EXPECT_EQ(composite.size(), 1);

    composite.clear();

    EXPECT_TRUE(composite.empty());
}

//============================================================================
// Writer Factory Tests
//============================================================================

TEST_F(ConfigStrategyTest, WriterFactory_CreateConsole) {
    auto writer = writer_factory::create_console();
    ASSERT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "console");
}

TEST_F(ConfigStrategyTest, WriterFactory_CreateFile) {
    auto writer = writer_factory::create_file("/tmp/test.log");
    ASSERT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "file");
}

TEST_F(ConfigStrategyTest, WriterFactory_CreateRotatingFile) {
    auto writer = writer_factory::create_rotating_file(
        "/tmp/test.log", 10 * 1024 * 1024, 5);
    ASSERT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "rotating_file");
}

TEST_F(ConfigStrategyTest, WriterFactory_CreateDevelopment) {
    auto writer = writer_factory::create_development();
    ASSERT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "console");
}

TEST_F(ConfigStrategyTest, WriterFactory_Registry) {
    // Register custom type
    writer_factory::register_type("custom", []() {
        return writer_factory::create_console();
    });

    EXPECT_TRUE(writer_factory::has_type("custom"));

    auto writer = writer_factory::create("custom");
    ASSERT_NE(writer, nullptr);
}

//============================================================================
// Formatter Factory Tests
//============================================================================

TEST_F(ConfigStrategyTest, FormatterFactory_CreatePlain) {
    auto formatter = formatter_factory::create_plain();
    ASSERT_NE(formatter, nullptr);
    EXPECT_EQ(formatter->get_name(), "timestamp_formatter");
}

TEST_F(ConfigStrategyTest, FormatterFactory_CreateJson) {
    auto formatter = formatter_factory::create_json();
    ASSERT_NE(formatter, nullptr);
    EXPECT_EQ(formatter->get_name(), "json_formatter");
}

TEST_F(ConfigStrategyTest, FormatterFactory_CreateByType) {
    auto plain = formatter_factory::create(format_type::plain);
    auto json = formatter_factory::create(format_type::json);
    auto compact = formatter_factory::create(format_type::compact);

    ASSERT_NE(plain, nullptr);
    ASSERT_NE(json, nullptr);
    ASSERT_NE(compact, nullptr);
}

TEST_F(ConfigStrategyTest, FormatterFactory_Presets) {
    auto dev = formatter_factory::create_development();
    auto prod = formatter_factory::create_production();

    ASSERT_NE(dev, nullptr);
    ASSERT_NE(prod, nullptr);

    // Development should have colors enabled
    EXPECT_TRUE(dev->get_options().use_colors);

    // Production should use JSON
    EXPECT_EQ(prod->get_name(), "json_formatter");
}

//============================================================================
// Filter Factory Tests
//============================================================================

TEST_F(ConfigStrategyTest, FilterFactory_CreateLevel) {
    auto filter = filter_factory::create_level(logger_system::log_level::warn);
    ASSERT_NE(filter, nullptr);
    EXPECT_EQ(filter->get_name(), "level_filter");
}

TEST_F(ConfigStrategyTest, FilterFactory_CreateRegex) {
    auto filter = filter_factory::create_regex("error|warning");
    ASSERT_NE(filter, nullptr);
    EXPECT_EQ(filter->get_name(), "regex_filter");
}

TEST_F(ConfigStrategyTest, FilterFactory_Builder) {
    auto filter = filter_factory::create_builder()
        .with_min_level(logger_system::log_level::info)
        .exclude_pattern("password|secret")
        .build();

    ASSERT_NE(filter, nullptr);
}

TEST_F(ConfigStrategyTest, FilterFactory_Production) {
    auto filter = filter_factory::create_production();
    ASSERT_NE(filter, nullptr);
}

//============================================================================
// Builder Integration Tests
//============================================================================

TEST_F(ConfigStrategyTest, Builder_ForEnvironment) {
    logger_builder builder;
    builder.for_environment(deployment_env::development);

    auto result = builder.build();
    EXPECT_TRUE(result.has_value());
}

TEST_F(ConfigStrategyTest, Builder_WithPerformanceTuning) {
    logger_builder builder;
    builder.with_performance_tuning(performance_level::balanced);

    auto result = builder.build();
    EXPECT_TRUE(result.has_value());
}

TEST_F(ConfigStrategyTest, Builder_AutoConfigure) {
    setenv("LOG_LEVEL", "warn", 1);

    logger_builder builder;
    builder.auto_configure();

    auto& config = builder.get_config();
    // After auto_configure and before build, strategies are stored but not applied
    // After build, they are applied

    auto result = builder.build();
    EXPECT_TRUE(result.has_value());
}

TEST_F(ConfigStrategyTest, Builder_ClearStrategies) {
    logger_builder builder;
    builder.for_environment(deployment_env::production)
           .with_performance_tuning(performance_level::high_throughput)
           .clear_strategies();

    // After clearing, no strategies should be applied
    auto result = builder.build();
    EXPECT_TRUE(result.has_value());
}

TEST_F(ConfigStrategyTest, Builder_ChainedStrategies) {
    logger_builder builder;

    builder.for_environment(deployment_env::staging)
           .with_performance_tuning(performance_level::balanced)
           .add_writer("console", writer_factory::create_console());

    auto result = builder.build();
    EXPECT_TRUE(result.has_value());
}

TEST_F(ConfigStrategyTest, Builder_ApplyStrategy) {
    logger_builder builder;

    auto strategy = std::make_unique<deployment_strategy>(deployment_env::testing);
    builder.apply_strategy(std::move(strategy));

    auto result = builder.build();
    EXPECT_TRUE(result.has_value());
}
