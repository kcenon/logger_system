/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file realtime_analyzer_test.cpp
 * @brief Tests for real-time log analysis with anomaly detection (Issue #281)
 */

#include <gtest/gtest.h>

#include <kcenon/logger/analysis/realtime_log_analyzer.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/base_writer.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

using namespace kcenon::logger;
using namespace kcenon::logger::analysis;

// =============================================================================
// Test Fixture
// =============================================================================

class RealtimeAnalyzerTest : public ::testing::Test {
protected:
    void SetUp() override {
        analyzer_ = std::make_unique<realtime_log_analyzer>();
    }

    void TearDown() override {
        analyzer_.reset();
    }

    analyzed_log_entry make_entry(log_level level,
                                   const std::string& message) {
        analyzed_log_entry entry;
        // Convert common::interfaces::log_level to log_level for analyzed_log_entry
        entry.level = static_cast<log_level>(static_cast<int>(level));
        entry.message = message;
        entry.timestamp = std::chrono::system_clock::now();
        entry.source_file = "test.cpp";
        entry.source_line = 1;
        entry.function_name = "test_function";
        return entry;
    }

    std::unique_ptr<realtime_log_analyzer> analyzer_;
};

// =============================================================================
// Basic Functionality Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, DefaultConstruction) {
    EXPECT_EQ(analyzer_->get_statistics().total_analyzed, 0);
    EXPECT_EQ(analyzer_->get_statistics().total_errors, 0);
    EXPECT_EQ(analyzer_->get_statistics().anomalies_detected, 0);
}

TEST_F(RealtimeAnalyzerTest, AnalyzeInfoLog) {
    auto entry = make_entry(log_level::info, "Test message");
    analyzer_->analyze(entry);

    auto stats = analyzer_->get_statistics();
    EXPECT_EQ(stats.total_analyzed, 1);
    EXPECT_EQ(stats.total_errors, 0);
}

TEST_F(RealtimeAnalyzerTest, AnalyzeErrorLog) {
    auto entry = make_entry(log_level::error, "Error message");
    analyzer_->analyze(entry);

    auto stats = analyzer_->get_statistics();
    EXPECT_EQ(stats.total_analyzed, 1);
    EXPECT_EQ(stats.total_errors, 1);
}

TEST_F(RealtimeAnalyzerTest, AnalyzeFatalLog) {
    auto entry = make_entry(log_level::fatal, "Fatal message");
    analyzer_->analyze(entry);

    auto stats = analyzer_->get_statistics();
    EXPECT_EQ(stats.total_analyzed, 1);
    EXPECT_EQ(stats.total_errors, 1);
}

// =============================================================================
// Pattern Alert Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, PatternAlertDetection) {
    std::atomic<int> callback_count{0};
    std::string detected_pattern;
    std::mutex mtx;

    analyzer_->add_pattern_alert("OutOfMemory", log_level::error);
    analyzer_->set_anomaly_callback([&](const anomaly_event& event) {
        callback_count++;
        std::lock_guard<std::mutex> lock(mtx);
        if (event.anomaly_type == anomaly_event::type::pattern_match) {
            detected_pattern = event.pattern;
        }
    });

    // Should not trigger (info level)
    auto info_entry = make_entry(log_level::info, "OutOfMemory error");
    analyzer_->analyze(info_entry);

    // Should trigger (error level with matching pattern)
    auto error_entry = make_entry(log_level::error, "OutOfMemory exception occurred");
    analyzer_->analyze(error_entry);

    // Wait for callback
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_GE(callback_count.load(), 1);
    {
        std::lock_guard<std::mutex> lock(mtx);
        EXPECT_EQ(detected_pattern, "OutOfMemory");
    }
}

TEST_F(RealtimeAnalyzerTest, MultiplePatternAlerts) {
    analyzer_->add_pattern_alert("Connection refused", log_level::error);
    analyzer_->add_pattern_alert("Timeout", log_level::warn);

    std::atomic<int> match_count{0};
    analyzer_->set_anomaly_callback([&](const anomaly_event& event) {
        if (event.anomaly_type == anomaly_event::type::pattern_match) {
            match_count++;
        }
    });

    analyzer_->analyze(make_entry(log_level::error, "Connection refused"));
    analyzer_->analyze(make_entry(log_level::warn, "Timeout occurred"));

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_EQ(match_count.load(), 2);
}

TEST_F(RealtimeAnalyzerTest, RemovePatternAlert) {
    analyzer_->add_pattern_alert("Test", log_level::info);

    EXPECT_TRUE(analyzer_->remove_pattern_alert("Test"));
    EXPECT_FALSE(analyzer_->remove_pattern_alert("NonExistent"));
}

TEST_F(RealtimeAnalyzerTest, ClearPatternAlerts) {
    analyzer_->add_pattern_alert("Pattern1", log_level::info);
    analyzer_->add_pattern_alert("Pattern2", log_level::info);

    std::atomic<int> match_count{0};
    analyzer_->set_anomaly_callback([&](const anomaly_event& event) {
        if (event.anomaly_type == anomaly_event::type::pattern_match) {
            match_count++;
        }
    });

    analyzer_->clear_pattern_alerts();

    analyzer_->analyze(make_entry(log_level::info, "Pattern1 test"));
    analyzer_->analyze(make_entry(log_level::info, "Pattern2 test"));

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_EQ(match_count.load(), 0);
}

// =============================================================================
// Error Spike Detection Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, ErrorSpikeDetection) {
    realtime_analysis_config config;
    config.error_spike_threshold = 5;  // Low threshold for testing
    config.window_duration = std::chrono::seconds(60);

    auto spike_analyzer = std::make_unique<realtime_log_analyzer>(config);

    std::atomic<bool> spike_detected{false};
    spike_analyzer->set_anomaly_callback([&](const anomaly_event& event) {
        if (event.anomaly_type == anomaly_event::type::error_spike) {
            spike_detected = true;
        }
    });

    // Generate error spike
    for (int i = 0; i < 10; ++i) {
        spike_analyzer->analyze(make_entry(log_level::error, "Error " + std::to_string(i)));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_TRUE(spike_detected.load());
}

TEST_F(RealtimeAnalyzerTest, NoSpikeBelowThreshold) {
    realtime_analysis_config config;
    config.error_spike_threshold = 100;  // High threshold
    config.window_duration = std::chrono::seconds(60);

    auto spike_analyzer = std::make_unique<realtime_log_analyzer>(config);

    std::atomic<bool> spike_detected{false};
    spike_analyzer->set_anomaly_callback([&](const anomaly_event& event) {
        if (event.anomaly_type == anomaly_event::type::error_spike) {
            spike_detected = true;
        }
    });

    // Generate few errors
    for (int i = 0; i < 5; ++i) {
        spike_analyzer->analyze(make_entry(log_level::error, "Error " + std::to_string(i)));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_FALSE(spike_detected.load());
}

// =============================================================================
// New Error Type Detection Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, NewErrorTypeDetection) {
    realtime_analysis_config config;
    config.track_new_errors = true;

    auto tracker_analyzer = std::make_unique<realtime_log_analyzer>(config);

    std::atomic<int> new_error_count{0};
    tracker_analyzer->set_anomaly_callback([&](const anomaly_event& event) {
        if (event.anomaly_type == anomaly_event::type::new_error_type) {
            new_error_count++;
        }
    });

    // First occurrence
    tracker_analyzer->analyze(make_entry(log_level::error, "Database connection failed"));

    // Second occurrence of same type
    tracker_analyzer->analyze(make_entry(log_level::error, "Database connection failed"));

    // Different error type
    tracker_analyzer->analyze(make_entry(log_level::error, "Network timeout"));

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Should detect 2 unique error types
    EXPECT_EQ(new_error_count.load(), 2);
}

TEST_F(RealtimeAnalyzerTest, ErrorNormalization) {
    realtime_analysis_config config;
    config.track_new_errors = true;

    auto tracker_analyzer = std::make_unique<realtime_log_analyzer>(config);

    std::atomic<int> new_error_count{0};
    tracker_analyzer->set_anomaly_callback([&](const anomaly_event& event) {
        if (event.anomaly_type == anomaly_event::type::new_error_type) {
            new_error_count++;
        }
    });

    // Same error type with different IDs
    tracker_analyzer->analyze(make_entry(log_level::error, "Failed to process request 12345"));
    tracker_analyzer->analyze(make_entry(log_level::error, "Failed to process request 67890"));

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Should detect only 1 unique error type (IDs normalized)
    EXPECT_EQ(new_error_count.load(), 1);
}

// =============================================================================
// Rate Calculation Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, LogRateCalculation) {
    for (int i = 0; i < 100; ++i) {
        analyzer_->analyze(make_entry(log_level::info, "Log " + std::to_string(i)));
    }

    double rate = analyzer_->get_log_rate();
    EXPECT_GT(rate, 0.0);
}

TEST_F(RealtimeAnalyzerTest, ErrorRateCalculation) {
    for (int i = 0; i < 50; ++i) {
        analyzer_->analyze(make_entry(log_level::error, "Error " + std::to_string(i)));
    }

    double error_rate = analyzer_->get_error_rate();
    EXPECT_GT(error_rate, 0.0);
}

// =============================================================================
// Statistics Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, StatisticsAccuracy) {
    analyzer_->analyze(make_entry(log_level::info, "Info"));
    analyzer_->analyze(make_entry(log_level::warn, "Warn"));
    analyzer_->analyze(make_entry(log_level::error, "Error"));
    analyzer_->analyze(make_entry(log_level::fatal, "Fatal"));

    auto stats = analyzer_->get_statistics();
    EXPECT_EQ(stats.total_analyzed, 4);
    EXPECT_EQ(stats.total_errors, 2);  // error + fatal
}

TEST_F(RealtimeAnalyzerTest, ResetStatistics) {
    analyzer_->analyze(make_entry(log_level::info, "Test"));
    analyzer_->analyze(make_entry(log_level::error, "Error"));

    analyzer_->reset();

    auto stats = analyzer_->get_statistics();
    EXPECT_EQ(stats.total_analyzed, 0);
    EXPECT_EQ(stats.total_errors, 0);
    EXPECT_EQ(stats.anomalies_detected, 0);
}

// =============================================================================
// Configuration Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, ConfigurationUpdate) {
    realtime_analysis_config config;
    config.error_spike_threshold = 50;
    config.track_new_errors = false;

    analyzer_->set_config(config);

    const auto& current = analyzer_->get_config();
    EXPECT_EQ(current.error_spike_threshold, 50);
    EXPECT_FALSE(current.track_new_errors);
}

TEST_F(RealtimeAnalyzerTest, ThresholdSetters) {
    analyzer_->set_error_spike_threshold(75);
    analyzer_->set_rate_thresholds(2000, 5);
    analyzer_->set_track_new_errors(false);

    const auto& config = analyzer_->get_config();
    EXPECT_EQ(config.error_spike_threshold, 75);
    EXPECT_EQ(config.rate_anomaly_high_threshold, 2000);
    EXPECT_EQ(config.rate_anomaly_low_threshold, 5);
    EXPECT_FALSE(config.track_new_errors);
}

// =============================================================================
// Factory Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, FactoryCreateBasic) {
    auto basic = realtime_analyzer_factory::create_basic();
    EXPECT_NE(basic, nullptr);
}

TEST_F(RealtimeAnalyzerTest, FactoryCreateWithConfig) {
    realtime_analysis_config config;
    config.error_spike_threshold = 100;

    auto configured = realtime_analyzer_factory::create(config);
    EXPECT_NE(configured, nullptr);
    EXPECT_EQ(configured->get_config().error_spike_threshold, 100);
}

TEST_F(RealtimeAnalyzerTest, FactoryCreateProduction) {
    std::atomic<bool> callback_invoked{false};
    auto prod = realtime_analyzer_factory::create_production(25, [&](const anomaly_event&) {
        callback_invoked = true;
    });

    EXPECT_NE(prod, nullptr);
    EXPECT_EQ(prod->get_config().error_spike_threshold, 25);
}

// =============================================================================
// Thread Safety Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, ConcurrentAnalysis) {
    constexpr int num_threads = 4;
    constexpr int logs_per_thread = 100;

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([this, t]() {
            for (int i = 0; i < logs_per_thread; ++i) {
                analyzer_->analyze(make_entry(
                    log_level::info,
                    "Thread " + std::to_string(t) + " log " + std::to_string(i)
                ));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto stats = analyzer_->get_statistics();
    EXPECT_EQ(stats.total_analyzed, num_threads * logs_per_thread);
}

TEST_F(RealtimeAnalyzerTest, ConcurrentPatternModification) {
    constexpr int iterations = 50;

    std::thread adder([this]() {
        for (int i = 0; i < iterations; ++i) {
            analyzer_->add_pattern_alert("Pattern" + std::to_string(i), log_level::info);
        }
    });

    std::thread analyzer_thread([this]() {
        for (int i = 0; i < iterations; ++i) {
            analyzer_->analyze(make_entry(log_level::info, "Test message"));
        }
    });

    adder.join();
    analyzer_thread.join();

    // No crashes or deadlocks
    SUCCEED();
}

// =============================================================================
// Logger Builder Integration Tests
// =============================================================================

TEST(RealtimeAnalyzerIntegrationTest, BuilderWithAnalyzer) {
    auto analyzer = std::make_unique<realtime_log_analyzer>();
    analyzer->set_error_spike_threshold(10);

    auto result = logger_builder()
        .with_async(false)
        .with_realtime_analyzer(std::move(analyzer))
        .build();

    ASSERT_TRUE(result.has_value());
    auto& logger = result.value();
    EXPECT_TRUE(logger->has_realtime_analysis());
}

TEST(RealtimeAnalyzerIntegrationTest, BuilderWithConfig) {
    realtime_analysis_config config;
    config.error_spike_threshold = 50;
    config.track_new_errors = true;

    std::atomic<int> anomaly_count{0};
    auto result = logger_builder()
        .with_async(false)
        .with_realtime_analysis(config, [&](const anomaly_event&) {
            anomaly_count++;
        })
        .build();

    ASSERT_TRUE(result.has_value());
    auto& logger = result.value();
    EXPECT_TRUE(logger->has_realtime_analysis());
}

TEST(RealtimeAnalyzerIntegrationTest, BuilderWithDefaultAnalysis) {
    auto result = logger_builder()
        .with_async(false)
        .with_realtime_analysis(25)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& logger = result.value();
    EXPECT_TRUE(logger->has_realtime_analysis());

    auto* analyzer = logger->get_realtime_analyzer();
    ASSERT_NE(analyzer, nullptr);
    EXPECT_EQ(analyzer->get_config().error_spike_threshold, 25);
}

// =============================================================================
// Anomaly Event Tests
// =============================================================================

TEST_F(RealtimeAnalyzerTest, AnomalyEventFields) {
    // Create analyzer with new error tracking disabled to only capture pattern match
    realtime_analysis_config config;
    config.track_new_errors = false;
    config.enable_rate_anomaly_detection = false;
    auto test_analyzer = std::make_unique<realtime_log_analyzer>(config);

    test_analyzer->add_pattern_alert("Critical", log_level::error);

    anomaly_event captured_event;
    std::mutex mtx;

    test_analyzer->set_anomaly_callback([&](const anomaly_event& event) {
        std::lock_guard<std::mutex> lock(mtx);
        captured_event = event;
    });

    test_analyzer->analyze(make_entry(log_level::error, "Critical failure"));

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::lock_guard<std::mutex> lock(mtx);
    EXPECT_EQ(captured_event.anomaly_type, anomaly_event::type::pattern_match);
    EXPECT_EQ(captured_event.pattern, "Critical");
    EXPECT_FALSE(captured_event.description.empty());
    EXPECT_FALSE(captured_event.related_entries.empty());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
