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
 * @file logger_rotation_bench.cpp
 * @brief Benchmark for rotating file writer
 * @details Measures file rotation overhead and performance degradation
 *
 * Target Metrics:
 * - Rotation overhead: < 10ms
 * - Write performance during rotation: minimal degradation
 * - File size threshold accuracy
 *
 * Phase 0, Task 0.2: Baseline Performance Benchmarking
 */

#include <benchmark/benchmark.h>
#include <kcenon/logger/writers/rotating_file_writer.h>
#include <kcenon/logger/core/logger.h>
#include <filesystem>
#include <memory>
#include <chrono>

namespace fs = std::filesystem;

//-----------------------------------------------------------------------------
// Rotation Overhead Benchmark
//-----------------------------------------------------------------------------

static void BM_RotationOverhead(benchmark::State& state) {
    std::string base_filename = "rotation_bench";
    const size_t max_file_size = 1024 * 10; // 10 KB (small for quick rotation)
    const size_t max_files = 3;

    auto rotating_writer = std::make_unique<kcenon::logger::rotating_file_writer>(
        base_filename, max_file_size, max_files);
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(rotating_writer));

    // Write enough to trigger rotation
    std::string large_message(1024, 'x'); // 1 KB message
    size_t rotation_count = 0;

    for (auto _ : state) {
        state.PauseTiming();
        // Write messages until rotation is about to happen
        for (size_t i = 0; i < 9; ++i) {
            logger->info("{}", large_message);
        }
        state.ResumeTiming();

        // This write should trigger rotation
        auto start = std::chrono::high_resolution_clock::now();
        logger->info("{}", large_message);
        auto end = std::chrono::high_resolution_clock::now();

        auto rotation_time = std::chrono::duration_cast<std::chrono::microseconds>(
            end - start).count();

        state.counters["rotation_time_us"] = rotation_time;
        rotation_count++;
    }

    state.counters["rotations"] = rotation_count;

    // Cleanup
    logger.reset();
    for (size_t i = 0; i < max_files; ++i) {
        std::string filename = base_filename + "." + std::to_string(i) + ".log";
        if (fs::exists(filename)) {
            fs::remove(filename);
        }
    }
}
BENCHMARK(BM_RotationOverhead)->Iterations(10);

//-----------------------------------------------------------------------------
// Write Performance During Rotation
//-----------------------------------------------------------------------------

static void BM_WritePerformanceDuringRotation(benchmark::State& state) {
    std::string base_filename = "rotation_perf_bench";
    const size_t max_file_size = 1024 * 50; // 50 KB
    const size_t max_files = 5;

    auto rotating_writer = std::make_unique<kcenon::logger::rotating_file_writer>(
        base_filename, max_file_size, max_files);
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(rotating_writer));

    std::string message(100, 'x');
    size_t messages_written = 0;

    for (auto _ : state) {
        logger->info("{} - message {}", message, messages_written++);
    }

    state.SetItemsProcessed(messages_written);
    state.counters["messages/sec"] = benchmark::Counter(
        messages_written, benchmark::Counter::kIsRate);

    // Cleanup
    logger.reset();
    for (size_t i = 0; i < max_files; ++i) {
        std::string filename = base_filename + "." + std::to_string(i) + ".log";
        if (fs::exists(filename)) {
            fs::remove(filename);
        }
    }
}
BENCHMARK(BM_WritePerformanceDuringRotation)->MinTime(2.0);

//-----------------------------------------------------------------------------
// File Size Threshold Accuracy
//-----------------------------------------------------------------------------

static void BM_FileSizeThresholdAccuracy(benchmark::State& state) {
    std::string base_filename = "rotation_accuracy_bench";
    const size_t max_file_size = state.range(0);
    const size_t max_files = 3;

    for (auto _ : state) {
        state.PauseTiming();
        auto rotating_writer = std::make_unique<kcenon::logger::rotating_file_writer>(
            base_filename, max_file_size, max_files);
        auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(rotating_writer));

        state.ResumeTiming();

        // Write until rotation
        std::string message(100, 'x');
        size_t total_written = 0;
        size_t messages = 0;

        while (total_written < max_file_size) {
            logger->info("{}", message);
            total_written += message.size() + 50; // Approximate overhead
            messages++;
        }

        state.PauseTiming();

        // Check file size
        std::string current_file = base_filename + ".0.log";
        if (fs::exists(current_file)) {
            size_t actual_size = fs::file_size(current_file);
            state.counters["target_size"] = max_file_size;
            state.counters["actual_size"] = actual_size;
            state.counters["size_deviation"] = std::abs(
                static_cast<double>(actual_size) - static_cast<double>(max_file_size));
        }

        // Cleanup
        logger.reset();
        for (size_t i = 0; i < max_files; ++i) {
            std::string filename = base_filename + "." + std::to_string(i) + ".log";
            if (fs::exists(filename)) {
                fs::remove(filename);
            }
        }

        state.ResumeTiming();
    }
}
BENCHMARK(BM_FileSizeThresholdAccuracy)
    ->Arg(1024 * 10)      // 10 KB
    ->Arg(1024 * 100)     // 100 KB
    ->Arg(1024 * 1024);   // 1 MB

//-----------------------------------------------------------------------------
// Maximum Files Rotation
//-----------------------------------------------------------------------------

static void BM_MaxFilesRotation(benchmark::State& state) {
    std::string base_filename = "rotation_maxfiles_bench";
    const size_t max_file_size = 1024 * 5; // 5 KB (small for quick rotation)
    const size_t max_files = state.range(0);

    auto rotating_writer = std::make_unique<kcenon::logger::rotating_file_writer>(
        base_filename, max_file_size, max_files);
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(rotating_writer));

    std::string large_message(1024, 'x'); // 1 KB message
    size_t messages_written = 0;

    for (auto _ : state) {
        logger->info("{}", large_message);
        messages_written++;
    }

    state.SetItemsProcessed(messages_written);
    state.counters["max_files"] = max_files;

    // Count actual files created
    size_t actual_files = 0;
    for (size_t i = 0; i < max_files; ++i) {
        std::string filename = base_filename + "." + std::to_string(i) + ".log";
        if (fs::exists(filename)) {
            actual_files++;
        }
    }
    state.counters["actual_files"] = actual_files;

    // Cleanup
    logger.reset();
    for (size_t i = 0; i < max_files; ++i) {
        std::string filename = base_filename + "." + std::to_string(i) + ".log";
        if (fs::exists(filename)) {
            fs::remove(filename);
        }
    }
}
BENCHMARK(BM_MaxFilesRotation)
    ->Arg(3)
    ->Arg(5)
    ->Arg(10)
    ->Arg(20)
    ->MinTime(1.0);

//-----------------------------------------------------------------------------
// Concurrent Rotation (Multi-threaded)
//-----------------------------------------------------------------------------

static void BM_ConcurrentRotation(benchmark::State& state) {
    static std::shared_ptr<kcenon::logger::logger> shared_logger;

    if (state.thread_index() == 0) {
        std::string base_filename = "rotation_concurrent_bench";
        const size_t max_file_size = 1024 * 10; // 10 KB
        const size_t max_files = 5;

        auto rotating_writer = std::make_unique<kcenon::logger::rotating_file_writer>(
            base_filename, max_file_size, max_files);
        shared_logger = std::make_shared<kcenon::logger::logger>();
        shared_logger->add_writer(std::move(rotating_writer));
    }

    std::string message(100, 'x');
    std::atomic<size_t> messages_written{0};

    for (auto _ : state) {
        shared_logger->info("Thread {} - {}", state.thread_index(), message);
        messages_written++;
    }

    state.SetItemsProcessed(messages_written.load());
    state.counters["thread_count"] = state.threads();

    if (state.thread_index() == 0) {
        // Cleanup
        shared_logger.reset();

        std::string base_filename = "rotation_concurrent_bench";
        for (size_t i = 0; i < 5; ++i) {
            std::string filename = base_filename + "." + std::to_string(i) + ".log";
            if (fs::exists(filename)) {
                fs::remove(filename);
            }
        }
    }
}
BENCHMARK(BM_ConcurrentRotation)
    ->Threads(2)
    ->Threads(4)
    ->Threads(8)
    ->MinTime(2.0)
    ->UseRealTime();
