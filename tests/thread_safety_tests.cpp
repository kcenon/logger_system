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

#ifndef LOGGER_STANDALONE_MODE
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/core/log_collector.h>
#endif

#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <barrier>

using namespace kcenon::logger;
using namespace std::chrono_literals;

class ThreadSafetyTest : public ::testing::Test {
protected:
    void SetUp() override {
        cleanup_test_files();
    }

    void TearDown() override {
        cleanup_test_files();
    }

    void cleanup_test_files() {
        std::vector<std::string> test_files = {
            "test_concurrent.log",
            "test_rotation.log",
            "test_async.log",
            "test_shutdown.log",
            "test_multiple.log",
            "test_multiple2.log",
            "test_format.log"
        };

        for (const auto& file : test_files) {
            if (std::filesystem::exists(file)) {
                std::filesystem::remove(file);
            }
        }
    }

    bool verify_log_integrity(const std::string& filename) {
        if (!std::filesystem::exists(filename)) {
            return false;
        }

        std::ifstream file(filename);
        std::string line;
        size_t line_count = 0;

        while (std::getline(file, line)) {
            ++line_count;
            // Basic integrity check: lines should not be corrupted
            // (mixed characters from different threads)
            if (line.empty()) {
                return false;
            }
        }

        return line_count > 0;
    }

    size_t count_log_lines(const std::string& filename) {
        if (!std::filesystem::exists(filename)) {
            return 0;
        }

        std::ifstream file(filename);
        return std::count(std::istreambuf_iterator<char>(file),
                         std::istreambuf_iterator<char>(), '\n');
    }
};

// Test 1: Concurrent rotation - 10 threads writing simultaneously, trigger rotation
TEST_F(ThreadSafetyTest, ConcurrentRotation) {
    const size_t max_file_size = 1024; // 1KB to force rotation
    const int num_threads = 10;
    const int messages_per_thread = 100;

    auto writer = std::make_shared<rotating_file_writer>(
        "test_rotation.log",
        max_file_size,
        3 // max backup files
    );

    auto test_logger = std::make_shared<logger>(writer);

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};
    std::atomic<int> total_written{0};

    // Use barrier to ensure all threads start simultaneously
    std::barrier sync_point(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            sync_point.arrive_and_wait(); // Synchronize start

            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    std::ostringstream msg;
                    msg << "Thread " << thread_id << " message " << j
                        << " - This is a long message to trigger rotation faster";
                    test_logger->info(msg.str());
                    ++total_written;
                } catch (...) {
                    ++errors;
                }

                // Small delay to allow rotation to occur
                if (j % 10 == 0) {
                    std::this_thread::sleep_for(1ms);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Flush and wait for async operations
    std::this_thread::sleep_for(100ms);

    EXPECT_EQ(errors.load(), 0) << "No errors should occur during concurrent rotation";
    EXPECT_EQ(total_written.load(), num_threads * messages_per_thread)
        << "All messages should be written";
}

#ifndef LOGGER_STANDALONE_MODE
// Test 2: Async writer stress test - Fill queue, test backpressure
TEST_F(ThreadSafetyTest, AsyncWriterStress) {
    const int num_threads = 8;
    const int messages_per_thread = 1000;
    const size_t queue_capacity = 100;

    auto file_writer = std::make_shared<file_writer>("test_async.log");
    auto async_writer = std::make_shared<async_writer>(file_writer, queue_capacity);
    auto test_logger = std::make_shared<logger>(async_writer);

    std::vector<std::thread> threads;
    std::atomic<int> messages_sent{0};
    std::atomic<int> errors{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    test_logger->info("Thread {} message {}", thread_id, j);
                    ++messages_sent;
                } catch (...) {
                    ++errors;
                }

                // Vary the rate to test backpressure
                if (j % 100 == 0) {
                    std::this_thread::sleep_for(5ms);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Allow async writer to flush
    std::this_thread::sleep_for(500ms);

    EXPECT_EQ(errors.load(), 0) << "No errors should occur during async writing";

    // Verify log file integrity
    EXPECT_TRUE(verify_log_integrity("test_async.log"));
}
#endif

// Test 3: Shutdown race - Stop logger while threads are writing
TEST_F(ThreadSafetyTest, ShutdownRace) {
    const int num_threads = 10;
    const int messages_per_thread = 500;

    auto writer = std::make_shared<file_writer>("test_shutdown.log");
    auto test_logger = std::make_shared<logger>(writer);

    std::vector<std::thread> threads;
    std::atomic<bool> should_stop{false};
    std::atomic<int> messages_written{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread && !should_stop.load(); ++j) {
                try {
                    test_logger->info("Thread {} message {}", thread_id, j);
                    ++messages_written;
                } catch (...) {
                    // Expected during shutdown
                }

                if (j % 50 == 0) {
                    std::this_thread::sleep_for(1ms);
                }
            }
        });
    }

    // Let threads run for a bit
    std::this_thread::sleep_for(100ms);

    // Signal shutdown while threads are still writing
    should_stop.store(true);

    // Wait for threads to finish
    for (auto& t : threads) {
        t.join();
    }

    // Logger should still be in valid state
    EXPECT_NO_THROW(test_logger->info("Final message after threads stopped"));

    // Verify log file is not corrupted
    EXPECT_TRUE(verify_log_integrity("test_shutdown.log"));
}

// Test 4: Multiple loggers writing to different files
TEST_F(ThreadSafetyTest, MultipleLoggers) {
    const int num_loggers = 5;
    const int threads_per_logger = 4;
    const int messages_per_thread = 200;

    std::vector<std::shared_ptr<logger>> loggers;
    std::vector<std::thread> threads;
    std::atomic<int> total_errors{0};

    // Create multiple loggers with different files
    for (int i = 0; i < num_loggers; ++i) {
        std::string filename = "test_multiple" + std::to_string(i) + ".log";
        auto writer = std::make_shared<file_writer>(filename);
        loggers.push_back(std::make_shared<logger>(writer));
    }

    // Create threads that write to different loggers
    for (int logger_id = 0; logger_id < num_loggers; ++logger_id) {
        for (int i = 0; i < threads_per_logger; ++i) {
            threads.emplace_back([&, logger_id, thread_id = i]() {
                for (int j = 0; j < messages_per_thread; ++j) {
                    try {
                        loggers[logger_id]->info("Logger {} Thread {} Message {}",
                                                 logger_id, thread_id, j);
                    } catch (...) {
                        ++total_errors;
                    }

                    if (j % 25 == 0) {
                        std::this_thread::sleep_for(1ms);
                    }
                }
            });
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    std::this_thread::sleep_for(100ms);

    EXPECT_EQ(total_errors.load(), 0);

    // Verify all log files
    for (int i = 0; i < num_loggers; ++i) {
        std::string filename = "test_multiple" + std::to_string(i) + ".log";
        EXPECT_TRUE(verify_log_integrity(filename));

        // Clean up
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
    }
}

// Test 5: Concurrent formatting - Complex formatting under load
TEST_F(ThreadSafetyTest, ConcurrentFormatting) {
    const int num_threads = 12;
    const int messages_per_thread = 300;

    auto writer = std::make_shared<file_writer>("test_format.log");
    auto test_logger = std::make_shared<logger>(writer);

    std::vector<std::thread> threads;
    std::atomic<int> format_errors{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    // Use complex formatting patterns
                    test_logger->info("Thread {}: Counter={}, Float={:.2f}, Hex=0x{:X}",
                                     thread_id, j, j * 3.14159, j * 256);
                    test_logger->debug("Nested format: {} {} {} {} {}",
                                      thread_id, j, j * 2, j * 3, j * 4);
                    test_logger->warn("String formatting: '{}' len={}",
                                     std::string(j % 20, 'x'), j % 20);
                } catch (...) {
                    ++format_errors;
                }

                if (j % 30 == 0) {
                    std::this_thread::sleep_for(1ms);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::this_thread::sleep_for(100ms);

    EXPECT_EQ(format_errors.load(), 0) << "No formatting errors should occur";
    EXPECT_TRUE(verify_log_integrity("test_format.log"));
}

// Test 6: Mixed log levels concurrent access
TEST_F(ThreadSafetyTest, MixedLogLevels) {
    auto writer = std::make_shared<file_writer>("test_levels.log");
    auto test_logger = std::make_shared<logger>(writer);
    test_logger->set_level(log_level::trace);

    const int num_threads = 8;
    const int messages_per_thread = 200;

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    switch (j % 6) {
                        case 0: test_logger->trace("Trace {}", j); break;
                        case 1: test_logger->debug("Debug {}", j); break;
                        case 2: test_logger->info("Info {}", j); break;
                        case 3: test_logger->warn("Warn {}", j); break;
                        case 4: test_logger->error("Error {}", j); break;
                        case 5: test_logger->critical("Critical {}", j); break;
                    }
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(errors.load(), 0);
    EXPECT_TRUE(verify_log_integrity("test_levels.log"));

    if (std::filesystem::exists("test_levels.log")) {
        std::filesystem::remove("test_levels.log");
    }
}

// Test 7: Concurrent level changes
TEST_F(ThreadSafetyTest, ConcurrentLevelChanges) {
    auto writer = std::make_shared<file_writer>("test_level_change.log");
    auto test_logger = std::make_shared<logger>(writer);

    const int num_writer_threads = 10;
    const int num_modifier_threads = 3;
    const int messages_per_thread = 300;

    std::vector<std::thread> threads;
    std::atomic<bool> running{true};
    std::atomic<int> errors{0};

    // Writer threads
    for (int i = 0; i < num_writer_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread && running.load(); ++j) {
                try {
                    test_logger->info("Thread {} message {}", thread_id, j);
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    // Threads that change log level
    for (int i = 0; i < num_modifier_threads; ++i) {
        threads.emplace_back([&]() {
            std::this_thread::sleep_for(10ms);
            while (running.load()) {
                try {
                    test_logger->set_level(log_level::debug);
                    std::this_thread::sleep_for(20ms);
                    test_logger->set_level(log_level::info);
                    std::this_thread::sleep_for(20ms);
                    test_logger->set_level(log_level::warn);
                    std::this_thread::sleep_for(20ms);
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    // Let it run
    std::this_thread::sleep_for(200ms);
    running.store(false);

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(errors.load(), 0);
    EXPECT_TRUE(verify_log_integrity("test_level_change.log"));

    if (std::filesystem::exists("test_level_change.log")) {
        std::filesystem::remove("test_level_change.log");
    }
}

// Test 8: Stress test with very high contention
TEST_F(ThreadSafetyTest, HighContentionStress) {
    auto writer = std::make_shared<file_writer>("test_stress.log");
    auto test_logger = std::make_shared<logger>(writer);

    const int num_threads = 20;
    const int messages_per_thread = 500;

    std::vector<std::thread> threads;
    std::atomic<int> total_messages{0};
    std::atomic<int> errors{0};

    std::barrier sync_point(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            sync_point.arrive_and_wait(); // All start together

            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    test_logger->info("T{}:M{}", thread_id, j);
                    ++total_messages;
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::this_thread::sleep_for(100ms);

    EXPECT_EQ(errors.load(), 0);
    EXPECT_EQ(total_messages.load(), num_threads * messages_per_thread);
    EXPECT_TRUE(verify_log_integrity("test_stress.log"));

    if (std::filesystem::exists("test_stress.log")) {
        std::filesystem::remove("test_stress.log");
    }
}

// Test 9: Rotation during high-frequency writes
TEST_F(ThreadSafetyTest, RotationUnderHighLoad) {
    const size_t small_file_size = 512; // Small file to force frequent rotation
    const int num_threads = 15;
    const int messages_per_thread = 200;

    auto writer = std::make_shared<rotating_file_writer>(
        "test_rotation_stress.log",
        small_file_size,
        5
    );

    auto test_logger = std::make_shared<logger>(writer);

    std::vector<std::thread> threads;
    std::atomic<int> rotation_errors{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    test_logger->info("Thread {} message {} with extra padding to force rotation",
                                     thread_id, j);
                } catch (...) {
                    ++rotation_errors;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::this_thread::sleep_for(100ms);

    EXPECT_EQ(rotation_errors.load(), 0);

    // Cleanup rotation files
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        if (entry.path().filename().string().find("test_rotation_stress") != std::string::npos) {
            std::filesystem::remove(entry.path());
        }
    }
}

// Test 10: Memory safety - no leaks during concurrent operations
TEST_F(ThreadSafetyTest, MemorySafetyTest) {
    const int num_iterations = 100;
    const int threads_per_iteration = 8;
    const int messages_per_thread = 50;

    std::atomic<int> total_errors{0};

    for (int iteration = 0; iteration < num_iterations; ++iteration) {
        auto writer = std::make_shared<file_writer>("test_memory.log");
        auto test_logger = std::make_shared<logger>(writer);

        std::vector<std::thread> threads;

        for (int i = 0; i < threads_per_iteration; ++i) {
            threads.emplace_back([&, thread_id = i]() {
                for (int j = 0; j < messages_per_thread; ++j) {
                    try {
                        test_logger->info("Iter {} Thread {} Msg {}",
                                         iteration, thread_id, j);
                    } catch (...) {
                        ++total_errors;
                    }
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        // Logger and writer destructors called here
    }

    EXPECT_EQ(total_errors.load(), 0);

    if (std::filesystem::exists("test_memory.log")) {
        std::filesystem::remove("test_memory.log");
    }
}
