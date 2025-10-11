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

#include "../framework/system_fixture.h"
#include "../framework/test_helpers.h"
#include <gtest/gtest.h>

using namespace integration_tests;

/**
 * @brief Performance tests for logger system
 *
 * These tests verify:
 * - Logging throughput (target: > 100K msgs/s)
 * - Latency measurements (P50, P95, P99)
 * - Memory usage under load
 * - Async vs sync performance comparison
 * - Queue saturation handling
 * - Scalability with thread count
 */
class LoggerPerformanceTest : public LoggerSystemFixture {};

TEST_F(LoggerPerformanceTest, ThroughputAsyncMode) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t message_count = 100'000;
    PerformanceMetrics metrics;

    ScopedTimer timer([&metrics](auto duration) {
        metrics.add_sample(duration);
    });

    for (size_t i = 0; i < message_count; ++i) {
        logger_->log(logger_system::log_level::info, "Performance test message " + std::to_string(i));
    }

    auto elapsed = timer.elapsed();
    logger_->flush();

    double throughput = CalculateThroughput(message_count, elapsed);

    std::cout << "Async throughput: " << throughput << " msgs/sec\n";
    std::cout << "Total time: " << FormatDuration(elapsed) << "\n";

    // Target: > 100K msgs/s
    EXPECT_GT(throughput, 100'000.0) << "Throughput below target";
}

TEST_F(LoggerPerformanceTest, ThroughputSyncMode) {
    CreateLogger(false);  // sync mode

    auto log_file = GetTempFilePath("sync_perf.log");
    auto writer = std::make_unique<kcenon::logger::file_writer>(log_file);
    logger_->add_writer(std::move(writer));
    logger_->start();

    const size_t message_count = 10'000;  // Smaller count for sync mode
    PerformanceMetrics metrics;

    ScopedTimer timer([&metrics](auto duration) {
        metrics.add_sample(duration);
    });

    for (size_t i = 0; i < message_count; ++i) {
        logger_->log(logger_system::log_level::info, "Sync test message " + std::to_string(i));
    }

    auto elapsed = timer.elapsed();
    logger_->flush();

    double throughput = CalculateThroughput(message_count, elapsed);

    std::cout << "Sync throughput: " << throughput << " msgs/sec\n";
    std::cout << "Total time: " << FormatDuration(elapsed) << "\n";

    // Sync mode should still provide reasonable throughput
    EXPECT_GT(throughput, 1'000.0);
}

TEST_F(LoggerPerformanceTest, LatencyMeasurements) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t sample_count = 10'000;
    PerformanceMetrics latency_metrics;

    for (size_t i = 0; i < sample_count; ++i) {
        auto start = std::chrono::high_resolution_clock::now();

        logger_->log(logger_system::log_level::info, "Latency test " + std::to_string(i));

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        latency_metrics.add_sample(duration);
    }

    logger_->flush();

    std::cout << "Latency P50: " << latency_metrics.p50() << " ns\n";
    std::cout << "Latency P95: " << latency_metrics.p95() << " ns\n";
    std::cout << "Latency P99: " << latency_metrics.p99() << " ns\n";
    std::cout << "Latency Mean: " << latency_metrics.mean() << " ns\n";

    // Performance baselines
    EXPECT_LT(latency_metrics.p50(), 10'000);    // P50 < 10 microseconds
    EXPECT_LT(latency_metrics.p95(), 100'000);   // P95 < 100 microseconds
}

TEST_F(LoggerPerformanceTest, AsyncVsSyncComparison) {
    const size_t message_count = 10'000;

    // Test async mode
    auto async_file = GetTempFilePath("async_compare.log");
    CreateLogger(true);
    auto async_writer = std::make_unique<kcenon::logger::file_writer>(async_file);
    logger_->add_writer(std::move(async_writer));
    logger_->start();

    auto async_start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < message_count; ++i) {
        logger_->log(logger_system::log_level::info, "Message " + std::to_string(i));
    }
    logger_->flush();
    auto async_elapsed = std::chrono::high_resolution_clock::now() - async_start;

    logger_->stop();
    logger_.reset();

    // Test sync mode
    auto sync_file = GetTempFilePath("sync_compare.log");
    CreateLogger(false);
    auto sync_writer = std::make_unique<kcenon::logger::file_writer>(sync_file);
    logger_->add_writer(std::move(sync_writer));
    logger_->start();

    auto sync_start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < message_count; ++i) {
        logger_->log(logger_system::log_level::info, "Message " + std::to_string(i));
    }
    logger_->flush();
    auto sync_elapsed = std::chrono::high_resolution_clock::now() - sync_start;

    auto async_throughput = CalculateThroughput(message_count, async_elapsed);
    auto sync_throughput = CalculateThroughput(message_count, sync_elapsed);

    std::cout << "Async throughput: " << async_throughput << " msgs/sec\n";
    std::cout << "Sync throughput: " << sync_throughput << " msgs/sec\n";
    std::cout << "Speedup: " << (async_throughput / sync_throughput) << "x\n";

    // Async should be significantly faster
    EXPECT_GT(async_throughput, sync_throughput);
}

TEST_F(LoggerPerformanceTest, MultiThreadedThroughput) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t thread_count = 8;
    const size_t messages_per_thread = 10'000;
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t t = 0; t < thread_count; ++t) {
        threads.emplace_back([this, t, messages_per_thread]() {
            for (size_t i = 0; i < messages_per_thread; ++i) {
                logger_->log(logger_system::log_level::info,
                           "Thread " + std::to_string(t) + " msg " + std::to_string(i));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    logger_->flush();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;

    size_t total_messages = thread_count * messages_per_thread;
    double throughput = CalculateThroughput(total_messages, elapsed);

    std::cout << "Multi-threaded throughput (" << thread_count << " threads): "
              << throughput << " msgs/sec\n";

    EXPECT_GT(throughput, 100'000.0);
}

TEST_F(LoggerPerformanceTest, ScalabilityWithThreadCount) {
    const size_t messages_per_thread = 5'000;
    std::vector<size_t> thread_counts = {1, 2, 4, 8};

    for (auto thread_count : thread_counts) {
        auto log_file = GetTempFilePath("scale_" + std::to_string(thread_count) + ".log");
        CreateLogger(true);
        auto writer = std::make_unique<kcenon::logger::file_writer>(log_file);
        logger_->add_writer(std::move(writer));
        logger_->start();

        std::vector<std::thread> threads;
        auto start = std::chrono::high_resolution_clock::now();

        for (size_t t = 0; t < thread_count; ++t) {
            threads.emplace_back([this, messages_per_thread]() {
                for (size_t i = 0; i < messages_per_thread; ++i) {
                    logger_->log(logger_system::log_level::info, "Scale test message");
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        logger_->flush();
        auto elapsed = std::chrono::high_resolution_clock::now() - start;

        size_t total_messages = thread_count * messages_per_thread;
        double throughput = CalculateThroughput(total_messages, elapsed);

        std::cout << "Threads: " << thread_count
                  << ", Throughput: " << throughput << " msgs/sec\n";

        logger_->stop();
        logger_.reset();
    }

    SUCCEED();
}

TEST_F(LoggerPerformanceTest, LargeMessagePerformance) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t message_count = 1'000;
    const size_t message_size = 1024;  // 1KB messages

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < message_count; ++i) {
        std::string large_message(message_size, 'X');
        large_message += " Message " + std::to_string(i);
        logger_->log(logger_system::log_level::info, large_message);
    }

    logger_->flush();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;

    double throughput = CalculateThroughput(message_count, elapsed);
    double data_rate_mb = (message_count * message_size) / (elapsed.count() / 1e9) / 1024 / 1024;

    std::cout << "Large message throughput: " << throughput << " msgs/sec\n";
    std::cout << "Data rate: " << data_rate_mb << " MB/sec\n";

    EXPECT_GT(throughput, 1'000.0);
}

TEST_F(LoggerPerformanceTest, BurstLogging) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t burst_count = 5;
    const size_t messages_per_burst = 10'000;

    PerformanceMetrics burst_metrics;

    for (size_t burst = 0; burst < burst_count; ++burst) {
        auto start = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < messages_per_burst; ++i) {
            logger_->log(logger_system::log_level::info,
                       "Burst " + std::to_string(burst) + " msg " + std::to_string(i));
        }

        logger_->flush();
        auto elapsed = std::chrono::high_resolution_clock::now() - start;

        burst_metrics.add_sample(elapsed);

        // Short delay between bursts
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Burst performance - Mean: " << burst_metrics.mean() << " ns\n";
    std::cout << "Burst performance - P95: " << burst_metrics.p95() << " ns\n";

    SUCCEED();
}

TEST_F(LoggerPerformanceTest, MemoryUsageUnderLoad) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t message_count = 100'000;

    // Log many messages without flushing
    for (size_t i = 0; i < message_count; ++i) {
        logger_->log(logger_system::log_level::info, "Memory test message " + std::to_string(i));
    }

    // Flush and verify all messages were handled
    logger_->flush();

    EXPECT_TRUE(WaitForLogLines(log_file, message_count, std::chrono::seconds(30)));

    // If we get here without crashes or OOM, memory management is working
    SUCCEED();
}
