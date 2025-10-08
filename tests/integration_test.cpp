/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/writers/rotating_file_writer.h>
#include <kcenon/logger/writers/encrypted_writer.h>

#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>

using namespace kcenon::logger;
using namespace std::chrono_literals;

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        cleanup_test_files();
        // Reset global metrics before each test
        metrics::g_logger_stats.reset();
    }

    void TearDown() override {
        cleanup_test_files();
    }

    void cleanup_test_files() {
        std::vector<std::string> test_files = {
            "test_integration.log",
            "test_rotating.log",
            "test_encrypted.log",
            "test.key"
        };

        for (const auto& file : test_files) {
            std::filesystem::remove(file);
        }

        // Remove rotating file backups
        for (int i = 1; i <= 5; ++i) {
            std::filesystem::remove("test_rotating.log." + std::to_string(i));
        }
    }
};

// Test 1: Multi-writer integration
TEST_F(IntegrationTest, MultiWriterLogging) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();

    // Add multiple writers
    test_logger->add_writer(std::make_unique<console_writer>());
    test_logger->add_writer(std::make_unique<file_writer>("test_integration.log"));
    test_logger->add_writer(std::make_unique<rotating_file_writer>(
        "test_rotating.log", 1024, 3));

    // Log messages at various levels
    for (int i = 0; i < 50; ++i) {
        if (i % 10 == 0) {
            test_logger->log(log_level::error, "Error message " + std::to_string(i));
        } else if (i % 5 == 0) {
            test_logger->log(log_level::warning, "Warning message " + std::to_string(i));
        } else {
            test_logger->log(log_level::info, "Info message " + std::to_string(i));
        }
    }

    test_logger->flush();
    test_logger->stop();

    // Verify files created
    EXPECT_TRUE(std::filesystem::exists("test_integration.log"));
    EXPECT_TRUE(std::filesystem::exists("test_rotating.log"));

    // Verify file has content
    std::ifstream file("test_integration.log");
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    EXPECT_FALSE(content.empty());
    EXPECT_NE(content.find("Error message"), std::string::npos);
    EXPECT_NE(content.find("Warning message"), std::string::npos);
    EXPECT_NE(content.find("Info message"), std::string::npos);
}

// Test 2: Metrics collection
TEST_F(IntegrationTest, MetricsCollection) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();
    test_logger->add_writer(std::make_unique<file_writer>("test_integration.log"));
    test_logger->enable_metrics_collection(true);

    const int message_count = 100;
    for (int i = 0; i < message_count; ++i) {
        test_logger->log(log_level::info, "Test message " + std::to_string(i));
    }

    test_logger->flush();

    // Check metrics
    auto metrics_result = test_logger->get_current_metrics();
    ASSERT_TRUE(metrics_result.has_value());
    const auto& metrics = metrics_result.value();

    EXPECT_EQ(metrics.messages_enqueued.load(), message_count);
    EXPECT_GT(metrics.get_messages_per_second(), 0);

    test_logger->stop();
}

// Test 3: Encrypted writer
TEST_F(IntegrationTest, EncryptedWriting) {
    // Generate encryption key
    auto key = encrypted_writer::generate_key();
    ASSERT_EQ(key.size(), 32);  // AES-256 key size

    // Save and load key
    ASSERT_TRUE(encrypted_writer::save_key(key, "test.key"));
    auto loaded_key = encrypted_writer::load_key("test.key");
    ASSERT_EQ(key, loaded_key);

    // Create logger with encrypted writer
    auto test_logger = std::make_shared<logger>();
    test_logger->start();

    auto file = std::make_unique<file_writer>("test_encrypted.log");
    auto encrypted = std::make_unique<encrypted_writer>(std::move(file), key);
    test_logger->add_writer(std::move(encrypted));

    test_logger->log(log_level::info, "Encrypted test message");
    test_logger->log(log_level::warning, "Another encrypted message");

    test_logger->flush();
    test_logger->stop();

    // Verify encrypted file exists
    EXPECT_TRUE(std::filesystem::exists("test_encrypted.log"));

    // Encrypted file should contain non-plaintext data
    std::ifstream file_stream("test_encrypted.log", std::ios::binary);
    std::string encrypted_content((std::istreambuf_iterator<char>(file_stream)),
                                  std::istreambuf_iterator<char>());
    EXPECT_FALSE(encrypted_content.empty());
    // Should NOT contain plaintext message
    EXPECT_EQ(encrypted_content.find("Encrypted test message"), std::string::npos);
}

// Test 4: Rotating file writer
TEST_F(IntegrationTest, FileRotation) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();

    // Small file size to trigger rotation
    test_logger->add_writer(std::make_unique<rotating_file_writer>(
        "test_rotating.log", 512, 3));  // 512 bytes, 3 backups

    // Write enough messages to trigger rotation
    for (int i = 0; i < 100; ++i) {
        test_logger->log(log_level::info,
            "Long message to trigger file rotation - message number " + std::to_string(i) +
            " with additional padding to increase file size quickly");
    }

    test_logger->flush();
    test_logger->stop();

    // Main file should exist
    EXPECT_TRUE(std::filesystem::exists("test_rotating.log"));

    // At least one backup should exist due to rotation
    bool backup_exists = false;
    for (int i = 1; i <= 3; ++i) {
        if (std::filesystem::exists("test_rotating.log." + std::to_string(i))) {
            backup_exists = true;
            break;
        }
    }
    EXPECT_TRUE(backup_exists);
}

// Test 5: Concurrent multi-threaded logging
TEST_F(IntegrationTest, ConcurrentLogging) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();
    test_logger->add_writer(std::make_unique<file_writer>("test_integration.log"));
    test_logger->enable_metrics_collection(true);

    const int num_threads = 4;
    const int messages_per_thread = 100;
    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&test_logger, t, messages_per_thread]() {
            for (int i = 0; i < messages_per_thread; ++i) {
                std::string msg = "Thread " + std::to_string(t) +
                                 " message " + std::to_string(i);
                test_logger->log(log_level::info, msg);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    test_logger->flush();

    // Check performance
    auto metrics_result = test_logger->get_current_metrics();
    ASSERT_TRUE(metrics_result.has_value());
    const auto& metrics = metrics_result.value();

    EXPECT_EQ(metrics.messages_enqueued.load(), num_threads * messages_per_thread);
    EXPECT_GT(metrics.get_messages_per_second(), 0);

    test_logger->stop();

    // Verify all messages logged
    std::ifstream file("test_integration.log");
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());

    // Should contain messages from all threads
    for (int t = 0; t < num_threads; ++t) {
        std::string thread_marker = "Thread " + std::to_string(t);
        EXPECT_NE(content.find(thread_marker), std::string::npos);
    }
}

// Test 6: Start/stop lifecycle
TEST_F(IntegrationTest, StartStopLifecycle) {
    auto test_logger = std::make_shared<logger>();
    test_logger->add_writer(std::make_unique<file_writer>("test_integration.log"));

    // Multiple start/stop cycles
    for (int cycle = 0; cycle < 5; ++cycle) {
        test_logger->start();

        test_logger->log(log_level::info, "Cycle " + std::to_string(cycle) + " message");

        test_logger->flush();
        test_logger->stop();
    }

    // Verify file has content from all cycles
    std::ifstream file("test_integration.log");
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());

    EXPECT_FALSE(content.empty());
    for (int cycle = 0; cycle < 5; ++cycle) {
        std::string cycle_marker = "Cycle " + std::to_string(cycle);
        EXPECT_NE(content.find(cycle_marker), std::string::npos);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
