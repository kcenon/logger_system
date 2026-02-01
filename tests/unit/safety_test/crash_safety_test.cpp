/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file crash_safety_test.cpp
 * @brief Unit tests for crash_safe_logger implementation
 */

#include <gtest/gtest.h>
#include <kcenon/logger/safety/crash_safe_logger.h>
#include <kcenon/logger/writers/console_writer.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>

using namespace kcenon::logger;
using namespace kcenon::logger::safety;
namespace ci = kcenon::common::interfaces;
using log_level = ci::log_level;

class CrashSafetyTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_log_path_ = "./test_crash_safe.log";
        cleanup_test_files();
    }

    void TearDown() override {
        cleanup_test_files();
    }

    void cleanup_test_files() {
        if (std::filesystem::exists(test_log_path_)) {
            std::filesystem::remove(test_log_path_);
        }
    }

    std::string test_log_path_;
};

TEST_F(CrashSafetyTest, CreateWithDefaultLogger) {
    auto safe_logger = crash_safe_logger::create();
    ASSERT_NE(safe_logger, nullptr);
    EXPECT_NE(safe_logger->get_underlying_logger(), nullptr);
}

TEST_F(CrashSafetyTest, CreateWithCustomLogger) {
    auto underlying = std::make_shared<logger>(true, 8192);
    auto safe_logger = crash_safe_logger::create(underlying);

    ASSERT_NE(safe_logger, nullptr);
    EXPECT_EQ(safe_logger->get_underlying_logger(), underlying);
}

TEST_F(CrashSafetyTest, InstallAndUninstallCrashHandlers) {
    auto safe_logger = crash_safe_logger::create();

    // Install handlers
    bool installed = safe_logger->install_crash_handlers();
    EXPECT_TRUE(installed);

    // Install again should succeed (idempotent)
    installed = safe_logger->install_crash_handlers();
    EXPECT_TRUE(installed);

    // Uninstall
    safe_logger->uninstall_crash_handlers();
}

TEST_F(CrashSafetyTest, SetAndGetMinLevel) {
    auto safe_logger = crash_safe_logger::create();

    // Default level
    safe_logger->set_min_level(log_level::debug);
    EXPECT_EQ(safe_logger->get_min_level(), log_level::debug);

    safe_logger->set_min_level(log_level::error);
    EXPECT_EQ(safe_logger->get_min_level(), log_level::error);

    safe_logger->set_min_level(log_level::info);
    EXPECT_EQ(safe_logger->get_min_level(), log_level::info);
}

TEST_F(CrashSafetyTest, FlushWithTimeout) {
    auto safe_logger = crash_safe_logger::create();

    // Start the logger first
    auto result = safe_logger->start();
    ASSERT_TRUE(result.is_ok());

    // Flush with timeout should succeed (nothing to flush)
    bool flushed = safe_logger->flush_with_timeout(std::chrono::milliseconds(100));
    EXPECT_TRUE(flushed);

    // Stop the logger
    safe_logger->stop();
}

TEST_F(CrashSafetyTest, LogMessage) {
    auto safe_logger = crash_safe_logger::create();

    auto result = safe_logger->start();
    ASSERT_TRUE(result.is_ok());

    // Log at various levels
    safe_logger->set_min_level(log_level::trace);
    safe_logger->log(log_level::trace, "Trace message");
    safe_logger->log(log_level::debug, "Debug message");
    safe_logger->log(log_level::info, "Info message");
    safe_logger->log(log_level::warning, "Warning message");
    safe_logger->log(log_level::error, "Error message");

    // Flush and stop
    safe_logger->flush_with_timeout(std::chrono::milliseconds(100));
    safe_logger->stop();
}

TEST_F(CrashSafetyTest, LogMessageWithSourceLocation) {
    auto safe_logger = crash_safe_logger::create();

    auto result = safe_logger->start();
    ASSERT_TRUE(result.is_ok());

    safe_logger->set_min_level(log_level::debug);
    safe_logger->log(log_level::info, "Test message", __FILE__, __LINE__, __func__);

    safe_logger->flush_with_timeout(std::chrono::milliseconds(100));
    safe_logger->stop();
}

TEST_F(CrashSafetyTest, MinLevelFiltering) {
    auto safe_logger = crash_safe_logger::create();

    auto result = safe_logger->start();
    ASSERT_TRUE(result.is_ok());

    // Set min level to warning - debug and info should be filtered
    safe_logger->set_min_level(log_level::warning);

    // These should be filtered (not logged)
    safe_logger->log(log_level::debug, "Should be filtered");
    safe_logger->log(log_level::info, "Should also be filtered");

    // These should be logged
    safe_logger->log(log_level::warning, "Should be logged");
    safe_logger->log(log_level::error, "Should also be logged");

    safe_logger->stop();
}

TEST_F(CrashSafetyTest, EmergencyFlush) {
    auto safe_logger = crash_safe_logger::create();

    auto result = safe_logger->start();
    ASSERT_TRUE(result.is_ok());

    // Log some messages
    safe_logger->log(log_level::info, "Pre-emergency message");

    // Emergency flush should not throw or crash
    safe_logger->emergency_flush();

    safe_logger->stop();
}

TEST_F(CrashSafetyTest, AutoFlushInterval) {
    auto safe_logger = crash_safe_logger::create();

    auto result = safe_logger->start();
    ASSERT_TRUE(result.is_ok());

    // Enable auto-flush
    safe_logger->set_auto_flush_interval(std::chrono::milliseconds(50));

    // Log some messages
    safe_logger->log(log_level::info, "Message before auto-flush");

    // Wait for auto-flush to occur
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Disable auto-flush
    safe_logger->set_auto_flush_interval(std::chrono::milliseconds(0));

    safe_logger->stop();
}

TEST_F(CrashSafetyTest, ThreadSafetyBasic) {
    auto safe_logger = crash_safe_logger::create();

    auto result = safe_logger->start();
    ASSERT_TRUE(result.is_ok());

    safe_logger->set_min_level(log_level::trace);

    const int num_threads = 4;
    const int messages_per_thread = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([safe_logger, i, messages_per_thread]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                safe_logger->log(log_level::info,
                    "Thread " + std::to_string(i) + " message " + std::to_string(j));

                // Occasionally change log level (thread-safe)
                if (j % 20 == 0) {
                    safe_logger->set_min_level(log_level::debug);
                    safe_logger->set_min_level(log_level::info);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    safe_logger->flush_with_timeout(std::chrono::milliseconds(500));
    safe_logger->stop();
}

TEST_F(CrashSafetyTest, StartStopCycle) {
    auto safe_logger = crash_safe_logger::create();

    // Multiple start/stop cycles
    for (int cycle = 0; cycle < 3; ++cycle) {
        auto result = safe_logger->start();
        EXPECT_TRUE(result.is_ok()) << "Cycle " << cycle << " start failed";

        safe_logger->log(log_level::info, "Cycle " + std::to_string(cycle));

        result = safe_logger->stop();
        EXPECT_TRUE(result.is_ok()) << "Cycle " << cycle << " stop failed";
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
