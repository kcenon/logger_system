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
#endif

#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <barrier>
#include <random>

using namespace kcenon::logger;
using namespace std::chrono_literals;

// Use the correct log_level type based on build configuration
#ifdef USE_THREAD_SYSTEM_INTEGRATION
using log_level = kcenon::thread::log_level;
#else
using log_level = logger_system::log_level;
#endif

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
            std::filesystem::remove(file);
        }

        // Remove rotating file backups
        for (int i = 1; i <= 5; ++i) {
            std::filesystem::remove("test_rotation.log." + std::to_string(i));
        }
    }
};

// Test 1: Concurrent logging from multiple threads
TEST_F(ThreadSafetyTest, ConcurrentLogging) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();
    test_logger->add_writer(std::make_unique<file_writer>("test_concurrent.log"));

    const int num_threads = 10;
    const int messages_per_thread = 500;

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    std::string msg = "Thread " + std::to_string(thread_id) +
                                    " message " + std::to_string(j);
                    test_logger->log(log_level::info, msg);
                } catch (...) {
                    ++errors;
                }

                if (j % 100 == 0) {
                    std::this_thread::sleep_for(1ms);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    test_logger->flush();
    test_logger->stop();

    EXPECT_EQ(errors.load(), 0);
    EXPECT_TRUE(std::filesystem::exists("test_concurrent.log"));
}

// Test 2: High throughput stress test
TEST_F(ThreadSafetyTest, HighThroughputStress) {
    auto test_logger = std::make_shared<logger>(true, 16384); // Larger buffer
    test_logger->start();
    test_logger->add_writer(std::make_unique<file_writer>("test_async.log"));

    const int num_threads = 20;
    const int messages_per_thread = 1000;

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};
    std::barrier sync_point(num_threads);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            sync_point.arrive_and_wait(); // All start together

            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    test_logger->log(log_level::info,
                                   "High throughput test: thread " + std::to_string(thread_id) +
                                   " msg " + std::to_string(j));
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    test_logger->flush();
    test_logger->stop();

    EXPECT_EQ(errors.load(), 0);

    // Calculate throughput
    double total_messages = num_threads * messages_per_thread;
    double messages_per_second = (total_messages / duration.count()) * 1000.0;

    std::cout << "Throughput: " << messages_per_second << " messages/second" << std::endl;
}

// Test 3: Rotating file writer concurrency
TEST_F(ThreadSafetyTest, RotatingFileWriterConcurrency) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();
    test_logger->add_writer(std::make_unique<rotating_file_writer>(
        "test_rotation.log", 1024 * 10, 3)); // 10KB max, 3 backups

    const int num_threads = 8;
    const int messages_per_thread = 500;

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    // Create longer messages to trigger rotation
                    std::string msg = "Rotation test thread " + std::to_string(thread_id) +
                                    " message " + std::to_string(j) +
                                    " - padding data to increase file size quickly";
                    test_logger->log(log_level::info, msg);
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    test_logger->flush();
    test_logger->stop();

    EXPECT_EQ(errors.load(), 0);
    EXPECT_TRUE(std::filesystem::exists("test_rotation.log"));
}

// Test 4: Start/stop stress test
TEST_F(ThreadSafetyTest, StartStopStress) {
    auto test_logger = std::make_shared<logger>();
    test_logger->add_writer(std::make_unique<file_writer>("test_shutdown.log"));

    const int num_cycles = 20;
    const int messages_per_cycle = 50;

    std::atomic<int> errors{0};

    for (int cycle = 0; cycle < num_cycles; ++cycle) {
        test_logger->start();

        std::vector<std::thread> threads;
        for (int i = 0; i < 5; ++i) {
            threads.emplace_back([&, thread_id = i]() {
                for (int j = 0; j < messages_per_cycle; ++j) {
                    try {
                        test_logger->log(log_level::info,
                                       "Cycle " + std::to_string(cycle) +
                                       " thread " + std::to_string(thread_id) +
                                       " msg " + std::to_string(j));
                    } catch (...) {
                        ++errors;
                    }
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        test_logger->flush();
        test_logger->stop();
    }

    EXPECT_EQ(errors.load(), 0);
}

// Test 5: Multiple writers concurrent access
TEST_F(ThreadSafetyTest, MultipleWritersConcurrent) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();

    test_logger->add_writer(std::make_unique<file_writer>("test_multiple.log"));
    test_logger->add_writer(std::make_unique<file_writer>("test_multiple2.log"));
    test_logger->add_writer(std::make_unique<console_writer>());

    const int num_threads = 12;
    const int messages_per_thread = 300;

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    // Cycle through different log levels
                    log_level level;
                    switch (j % 4) {
                        case 0: level = log_level::debug; break;
                        case 1: level = log_level::info; break;
                        case 2: level = log_level::warn; break;
                        case 3: level = log_level::error; break;
                        default: level = log_level::info;
                    }

                    test_logger->log(level,
                                   "Multiple writers test: " + std::to_string(thread_id) +
                                   ":" + std::to_string(j));
                } catch (...) {
                    ++errors;
                }

                if (j % 50 == 0) {
                    std::this_thread::sleep_for(1ms);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    test_logger->flush();
    test_logger->stop();

    EXPECT_EQ(errors.load(), 0);
    EXPECT_TRUE(std::filesystem::exists("test_multiple.log"));
    EXPECT_TRUE(std::filesystem::exists("test_multiple2.log"));
}

// Test 6: Flush during concurrent logging
TEST_F(ThreadSafetyTest, FlushDuringLogging) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();
    test_logger->add_writer(std::make_unique<file_writer>("test_concurrent.log"));

    const int num_logger_threads = 10;
    const int num_flush_threads = 3;
    const int messages_per_thread = 400;

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};
    std::atomic<bool> running{true};

    // Logger threads
    for (int i = 0; i < num_logger_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread && running.load(); ++j) {
                try {
                    test_logger->log(log_level::info,
                                   "Concurrent flush test " + std::to_string(thread_id) +
                                   ":" + std::to_string(j));
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    // Flush threads
    for (int i = 0; i < num_flush_threads; ++i) {
        threads.emplace_back([&]() {
            while (running.load()) {
                try {
                    test_logger->flush();
                    std::this_thread::sleep_for(50ms);
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    std::this_thread::sleep_for(500ms);
    running.store(false);

    for (auto& t : threads) {
        t.join();
    }

    test_logger->stop();

    EXPECT_EQ(errors.load(), 0);
}

// Test 7: Source location logging concurrency
TEST_F(ThreadSafetyTest, SourceLocationConcurrency) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();
    test_logger->add_writer(std::make_unique<file_writer>("test_concurrent.log"));

    const int num_threads = 8;
    const int messages_per_thread = 500;

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                try {
                    test_logger->log(log_level::info,
                                   "Source location test " + std::to_string(j),
                                   __FILE__, __LINE__, __FUNCTION__);
                } catch (...) {
                    ++errors;
                }

                if (j % 100 == 0) {
                    std::this_thread::sleep_for(1ms);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    test_logger->flush();
    test_logger->stop();

    EXPECT_EQ(errors.load(), 0);
}

// Test 8: Mixed log levels stress
TEST_F(ThreadSafetyTest, MixedLogLevelsStress) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();
    test_logger->add_writer(std::make_unique<file_writer>("test_concurrent.log"));

    const int num_threads = 15;
    const int operations_per_thread = 400;

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            std::mt19937 rng(thread_id);
            std::uniform_int_distribution<int> level_dist(0, 5);

            for (int j = 0; j < operations_per_thread; ++j) {
                try {
                    log_level level;
                    switch (level_dist(rng)) {
                        case 0: level = log_level::trace; break;
                        case 1: level = log_level::debug; break;
                        case 2: level = log_level::info; break;
                        case 3: level = log_level::warn; break;
                        case 4: level = log_level::error; break;
                        default: level = log_level::fatal; break;
                    }

                    test_logger->log(level,
                                   "Mixed level test " + std::to_string(thread_id) +
                                   ":" + std::to_string(j));
                } catch (...) {
                    ++errors;
                }

                if (j % 50 == 0) {
                    std::this_thread::sleep_for(1ms);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    test_logger->flush();
    test_logger->stop();

    EXPECT_EQ(errors.load(), 0);
}

// Test 9: Add writer during logging
TEST_F(ThreadSafetyTest, DynamicWriterAddition) {
    auto test_logger = std::make_shared<logger>();
    test_logger->start();
    test_logger->add_writer(std::make_unique<file_writer>("test_concurrent.log"));

    const int num_logger_threads = 8;
    const int messages_per_thread = 500;

    std::vector<std::thread> threads;
    std::atomic<int> errors{0};
    std::atomic<bool> running{true};

    // Logger threads
    for (int i = 0; i < num_logger_threads; ++i) {
        threads.emplace_back([&, thread_id = i]() {
            for (int j = 0; j < messages_per_thread && running.load(); ++j) {
                try {
                    test_logger->log(log_level::info,
                                   "Dynamic writer test " + std::to_string(thread_id) +
                                   ":" + std::to_string(j));
                } catch (...) {
                    ++errors;
                }
            }
        });
    }

    // Writer addition thread
    threads.emplace_back([&]() {
        std::this_thread::sleep_for(100ms);
        try {
            test_logger->add_writer(std::make_unique<file_writer>("test_multiple.log"));
        } catch (...) {
            ++errors;
        }
    });

    std::this_thread::sleep_for(600ms);
    running.store(false);

    for (auto& t : threads) {
        t.join();
    }

    test_logger->flush();
    test_logger->stop();

    EXPECT_EQ(errors.load(), 0);
}

// Test 10: Memory safety - no leaks during concurrent logging
TEST_F(ThreadSafetyTest, MemorySafetyTest) {
    const int num_iterations = 30;
    const int threads_per_iteration = 8;
    const int messages_per_thread = 100;

    std::atomic<int> total_errors{0};

    for (int iteration = 0; iteration < num_iterations; ++iteration) {
        auto test_logger = std::make_shared<logger>();
        test_logger->start();
        test_logger->add_writer(std::make_unique<file_writer>("test_concurrent.log"));

        std::vector<std::thread> threads;

        for (int i = 0; i < threads_per_iteration; ++i) {
            threads.emplace_back([&, thread_id = i]() {
                for (int j = 0; j < messages_per_thread; ++j) {
                    try {
                        test_logger->log(log_level::info,
                                       "Memory safety iter " + std::to_string(iteration) +
                                       " thread " + std::to_string(thread_id) +
                                       " msg " + std::to_string(j));
                    } catch (...) {
                        ++total_errors;
                    }
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        test_logger->flush();
        test_logger->stop();

        // Logger destructor called here
    }

    EXPECT_EQ(total_errors.load(), 0);
}
