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

/**
 * @file logger_throughput_bench.cpp
 * @brief Benchmark for logger throughput
 * @details Measures sustained logging performance and messages/sec capacity
 *
 * Target Metrics:
 * - Throughput: > 100k messages/sec
 * - Sustained performance under load
 * - Multi-threaded throughput scaling
 *
 * Phase 0, Task 0.2: Baseline Performance Benchmarking
 */

#include <benchmark/benchmark.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/null_writer.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <filesystem>
#include <memory>
#include <atomic>
#include <thread>
#include <chrono>

namespace ci = kcenon::common::interfaces;

namespace fs = std::filesystem;

//-----------------------------------------------------------------------------
// Throughput Benchmarks (Single-threaded)
//-----------------------------------------------------------------------------

static void BM_Throughput_NullWriter(benchmark::State& state) {
    auto null_writer = std::make_unique<kcenon::logger::null_writer>();
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(null_writer));

    size_t messages_logged = 0;

    for (auto _ : state) {
        logger->info("Throughput test message {}", messages_logged++);
    }

    state.SetItemsProcessed(messages_logged);
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged, benchmark::Counter::kIsRate);
}
BENCHMARK(BM_Throughput_NullWriter);

static void BM_Throughput_FileWriter(benchmark::State& state) {
    std::string test_file = "throughput_bench.log";
    auto file_writer = std::make_unique<kcenon::logger::file_writer>(test_file);
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(file_writer));

    size_t messages_logged = 0;

    for (auto _ : state) {
        logger->info("Throughput test message {}", messages_logged++);
    }

    state.SetItemsProcessed(messages_logged);
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged, benchmark::Counter::kIsRate);

    // Cleanup
    logger.reset();
    if (fs::exists(test_file)) {
        fs::remove(test_file);
    }
}
BENCHMARK(BM_Throughput_FileWriter);

//-----------------------------------------------------------------------------
// Burst Throughput (Simulates log storms)
//-----------------------------------------------------------------------------

static void BM_BurstThroughput(benchmark::State& state) {
    auto null_writer = std::make_unique<kcenon::logger::null_writer>();
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(null_writer));

    const size_t burst_size = state.range(0);
    size_t total_messages = 0;

    for (auto _ : state) {
        // Simulate burst logging
        for (size_t i = 0; i < burst_size; ++i) {
            logger->info("Burst message {}", i);
            total_messages++;
        }
    }

    state.SetItemsProcessed(total_messages);
    state.counters["burst_size"] = burst_size;
    state.counters["messages/sec"] = benchmark::Counter(
        total_messages, benchmark::Counter::kIsRate);
}
BENCHMARK(BM_BurstThroughput)
    ->Arg(10)      // Small burst
    ->Arg(100)     // Medium burst
    ->Arg(1000)    // Large burst
    ->Arg(10000);  // Very large burst

//-----------------------------------------------------------------------------
// Multi-threaded Throughput
//-----------------------------------------------------------------------------

static void BM_MultithreadedThroughput(benchmark::State& state) {
    static std::shared_ptr<kcenon::logger::logger> shared_logger;

    // Initialize logger once for all threads
    if (state.thread_index() == 0) {
        auto null_writer = std::make_unique<kcenon::logger::null_writer>();
        shared_logger = std::make_shared<kcenon::logger::logger>();
        shared_logger->add_writer(std::move(null_writer));
    }

    std::atomic<size_t> messages_logged{0};

    for (auto _ : state) {
        shared_logger->info("Thread {} message", state.thread_index());
        messages_logged++;
    }

    state.SetItemsProcessed(messages_logged.load());
    state.counters["thread_count"] = state.threads();
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged.load(), benchmark::Counter::kIsRate);

    // Cleanup after last thread
    if (state.thread_index() == 0) {
        shared_logger.reset();
    }
}
BENCHMARK(BM_MultithreadedThroughput)
    ->Threads(1)
    ->Threads(2)
    ->Threads(4)
    ->Threads(8)
    ->Threads(16)
    ->UseRealTime();

//-----------------------------------------------------------------------------
// Sustained Throughput (Longer duration test)
//-----------------------------------------------------------------------------

static void BM_SustainedThroughput(benchmark::State& state) {
    auto null_writer = std::make_unique<kcenon::logger::null_writer>();
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(null_writer));

    size_t messages_logged = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    for (auto _ : state) {
        logger->info("Sustained throughput message {}", messages_logged++);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time).count();

    state.SetItemsProcessed(messages_logged);
    state.counters["duration_us"] = duration;
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged, benchmark::Counter::kIsRate);
}
BENCHMARK(BM_SustainedThroughput)
    ->MinTime(1.0)   // Run for at least 1 second
    ->MinTime(5.0)   // Run for at least 5 seconds
    ->MinTime(10.0); // Run for at least 10 seconds

//-----------------------------------------------------------------------------
// Throughput with Different Message Patterns
//-----------------------------------------------------------------------------

static void BM_ThroughputWithVariableMessageSize(benchmark::State& state) {
    auto null_writer = std::make_unique<kcenon::logger::null_writer>();
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(null_writer));

    size_t messages_logged = 0;
    std::vector<std::string> messages = {
        "Short",
        "Medium length message with some content",
        std::string(100, 'x'),  // Long message
        std::string(500, 'y'),  // Very long message
    };

    size_t msg_index = 0;

    for (auto _ : state) {
        logger->info("{}", messages[msg_index++ % messages.size()]);
        messages_logged++;
    }

    state.SetItemsProcessed(messages_logged);
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged, benchmark::Counter::kIsRate);
}
BENCHMARK(BM_ThroughputWithVariableMessageSize);

//-----------------------------------------------------------------------------
// Throughput with Log Level Filtering
//-----------------------------------------------------------------------------

static void BM_ThroughputWithFiltering(benchmark::State& state) {
    auto null_writer = std::make_unique<kcenon::logger::null_writer>();
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(null_writer));

    // Set minimum log level to WARNING (filters out DEBUG and INFO)
    logger->set_level(ci::log_level::warning);

    size_t messages_logged = 0;

    for (auto _ : state) {
        // These should be filtered out
        logger->debug("Debug message {}", messages_logged);
        logger->info("Info message {}", messages_logged);

        // These should be logged
        logger->warning("Warning message {}", messages_logged);
        logger->error("Error message {}", messages_logged);

        messages_logged += 4;
    }

    state.SetItemsProcessed(messages_logged);
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged, benchmark::Counter::kIsRate);
    state.SetLabel("filtered_2/4_messages");
}
BENCHMARK(BM_ThroughputWithFiltering);

//-----------------------------------------------------------------------------
// Contention Benchmark (Heavy multi-threaded writes)
//-----------------------------------------------------------------------------

static void BM_ContentionThroughput(benchmark::State& state) {
    static std::shared_ptr<kcenon::logger::logger> shared_logger;
    static std::atomic<bool> ready{false};

    if (state.thread_index() == 0) {
        auto null_writer = std::make_unique<kcenon::logger::null_writer>();
        shared_logger = std::make_shared<kcenon::logger::logger>();
        shared_logger->add_writer(std::move(null_writer));
        ready = true;
    }

    // Wait for logger initialization
    while (!ready.load()) {
        std::this_thread::yield();
    }

    std::atomic<size_t> messages_logged{0};

    for (auto _ : state) {
        // All threads hammering the logger simultaneously
        shared_logger->info("Contention test - thread {}", state.thread_index());
        messages_logged++;
    }

    state.SetItemsProcessed(messages_logged.load());
    state.counters["thread_count"] = state.threads();
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged.load(), benchmark::Counter::kIsRate);

    if (state.thread_index() == 0) {
        shared_logger.reset();
        ready = false;
    }
}
BENCHMARK(BM_ContentionThroughput)
    ->Threads(2)
    ->Threads(4)
    ->Threads(8)
    ->Threads(16)
    ->Threads(32)
    ->UseRealTime();
