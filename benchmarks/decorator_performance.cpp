/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

/**
 * @file decorator_performance.cpp
 * @brief Performance benchmarks for decorator pattern implementation
 *
 * This file benchmarks the decorator pattern implementation against manual
 * nesting to verify zero-overhead abstraction and identify performance
 * characteristics of various decorator combinations.
 *
 * Part of EPIC #391 - Decorator pattern refactoring
 * Implements issue #423 - Benchmark decorator pattern performance
 */

#include <benchmark/benchmark.h>
#include <kcenon/logger/builders/writer_builder.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/buffered_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <memory>
#include <string>
#include <filesystem>

using namespace kcenon::logger;

namespace {
    // Test log file path
    constexpr const char* BENCH_LOG_FILE = "bench_decorator.log";

    // Helper to create a sample log entry
    log_entry create_test_entry() {
        return log_entry{log_level::info, "benchmark message"};
    }

    // Helper to cleanup test files
    void cleanup_test_file() {
        std::filesystem::remove(BENCH_LOG_FILE);
    }
}

//==============================================================================
// Baseline: Direct file writer (no decorators)
//==============================================================================

static void BM_DirectFileWriter(benchmark::State& state) {
    cleanup_test_file();
    auto writer = std::make_unique<file_writer>(BENCH_LOG_FILE);
    auto entry = create_test_entry();

    for (auto _ : state) {
        writer->write(entry);
    }

    state.SetItemsProcessed(state.iterations());
    cleanup_test_file();
}
BENCHMARK(BM_DirectFileWriter);

//==============================================================================
// Single decorator: Async only
//==============================================================================

static void BM_AsyncDecorator(benchmark::State& state) {
    cleanup_test_file();
    auto writer = writer_builder()
        .file(BENCH_LOG_FILE)
        .async(10000)
        .build();

    // Start async writer
    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->start();
    }

    auto entry = create_test_entry();

    for (auto _ : state) {
        writer->write(entry);
    }

    // Stop async writer and flush
    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->stop(true);
    }

    state.SetItemsProcessed(state.iterations());
    cleanup_test_file();
}
BENCHMARK(BM_AsyncDecorator);

//==============================================================================
// Single decorator: Buffered only
//==============================================================================

static void BM_BufferedDecorator(benchmark::State& state) {
    cleanup_test_file();
    auto writer = writer_builder()
        .file(BENCH_LOG_FILE)
        .buffered(100)
        .build();

    auto entry = create_test_entry();

    for (auto _ : state) {
        writer->write(entry);
    }

    writer->flush();

    state.SetItemsProcessed(state.iterations());
    cleanup_test_file();
}
BENCHMARK(BM_BufferedDecorator);

//==============================================================================
// Double decorators: Buffered + Async (common pattern)
//==============================================================================

static void BM_BufferedAsyncDecorator(benchmark::State& state) {
    cleanup_test_file();
    auto writer = writer_builder()
        .file(BENCH_LOG_FILE)
        .buffered(100)
        .async(10000)
        .build();

    // Start async writer (outermost decorator)
    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->start();
    }

    auto entry = create_test_entry();

    for (auto _ : state) {
        writer->write(entry);
    }

    // Stop async writer and flush
    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->stop(true);
    }

    state.SetItemsProcessed(state.iterations());
    cleanup_test_file();
}
BENCHMARK(BM_BufferedAsyncDecorator);

//==============================================================================
// Manual nesting (old pattern) - Baseline for comparison
//==============================================================================

static void BM_ManualNesting_Async(benchmark::State& state) {
    cleanup_test_file();
    auto writer = std::make_unique<async_writer>(
        std::make_unique<file_writer>(BENCH_LOG_FILE),
        10000
    );

    writer->start();
    auto entry = create_test_entry();

    for (auto _ : state) {
        writer->write(entry);
    }

    writer->stop(true);

    state.SetItemsProcessed(state.iterations());
    cleanup_test_file();
}
BENCHMARK(BM_ManualNesting_Async);

static void BM_ManualNesting_BufferedAsync(benchmark::State& state) {
    cleanup_test_file();
    auto writer = std::make_unique<async_writer>(
        std::make_unique<buffered_writer>(
            std::make_unique<file_writer>(BENCH_LOG_FILE),
            100
        ),
        10000
    );

    writer->start();
    auto entry = create_test_entry();

    for (auto _ : state) {
        writer->write(entry);
    }

    writer->stop(true);

    state.SetItemsProcessed(state.iterations());
    cleanup_test_file();
}
BENCHMARK(BM_ManualNesting_BufferedAsync);

//==============================================================================
// Console writer benchmarks (different I/O target)
//==============================================================================

static void BM_DirectConsoleWriter(benchmark::State& state) {
    auto writer = std::make_unique<console_writer>(false, false);
    auto entry = create_test_entry();

    for (auto _ : state) {
        writer->write(entry);
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_DirectConsoleWriter);

static void BM_ConsoleAsyncDecorator(benchmark::State& state) {
    auto writer = writer_builder()
        .console(false, false)
        .async(10000)
        .build();

    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->start();
    }

    auto entry = create_test_entry();

    for (auto _ : state) {
        writer->write(entry);
    }

    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->stop(true);
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_ConsoleAsyncDecorator);

//==============================================================================
// Throughput benchmarks with varying message sizes
//==============================================================================

static void BM_Throughput_SmallMessages(benchmark::State& state) {
    cleanup_test_file();
    auto writer = writer_builder()
        .file(BENCH_LOG_FILE)
        .buffered(100)
        .async(10000)
        .build();

    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->start();
    }

    // Small message (typical log line)
    log_entry entry{log_level::info, "short message"};

    for (auto _ : state) {
        writer->write(entry);
    }

    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->stop(true);
    }

    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * entry.message.size());
    cleanup_test_file();
}
BENCHMARK(BM_Throughput_SmallMessages);

static void BM_Throughput_LargeMessages(benchmark::State& state) {
    cleanup_test_file();
    auto writer = writer_builder()
        .file(BENCH_LOG_FILE)
        .buffered(100)
        .async(10000)
        .build();

    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->start();
    }

    // Large message (stack trace, JSON payload, etc.)
    std::string large_msg(1024, 'X');
    log_entry entry{log_level::info, large_msg};

    for (auto _ : state) {
        writer->write(entry);
    }

    if (auto* async = dynamic_cast<async_writer*>(writer.get())) {
        async->stop(true);
    }

    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * entry.message.size());
    cleanup_test_file();
}
BENCHMARK(BM_Throughput_LargeMessages);

// Run all benchmarks
BENCHMARK_MAIN();
