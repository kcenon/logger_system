/**
 * @file logger_async_bench.cpp
 * @brief Benchmark for async writer
 * @details Measures asynchronous logging performance and queue latency
 *
 * Target Metrics:
 * - Async queue latency: < 1ms (p99)
 * - Throughput improvement vs synchronous
 * - Queue saturation behavior
 *
 * Phase 0, Task 0.2: Baseline Performance Benchmarking
 */

#include <benchmark/benchmark.h>
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/null_writer.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/core/logger.h>
#include <filesystem>
#include <memory>
#include <atomic>

namespace fs = std::filesystem;

//-----------------------------------------------------------------------------
// Async vs Sync Comparison
//-----------------------------------------------------------------------------

static void BM_AsyncWriter_Throughput(benchmark::State& state) {
    std::string test_file = "async_bench.log";
    auto file_writer = std::make_unique<kcenon::logger::file_writer>(test_file);
    auto async_writer = std::make_unique<kcenon::logger::async_writer>(
        std::move(file_writer), 10000); // 10k queue size
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(async_writer));

    size_t messages_logged = 0;

    for (auto _ : state) {
        logger->info("Async benchmark message {}", messages_logged++);
    }

    // Wait for async queue to flush
    logger.reset();

    state.SetItemsProcessed(messages_logged);
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged, benchmark::Counter::kIsRate);
    state.SetLabel("async_writer");

    // Cleanup
    if (fs::exists(test_file)) {
        fs::remove(test_file);
    }
}
BENCHMARK(BM_AsyncWriter_Throughput);

static void BM_SyncWriter_Throughput(benchmark::State& state) {
    std::string test_file = "sync_bench.log";
    auto file_writer = std::make_unique<kcenon::logger::file_writer>(test_file);
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(file_writer));

    size_t messages_logged = 0;

    for (auto _ : state) {
        logger->info("Sync benchmark message {}", messages_logged++);
    }

    state.SetItemsProcessed(messages_logged);
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged, benchmark::Counter::kIsRate);
    state.SetLabel("sync_writer");

    // Cleanup
    logger.reset();
    if (fs::exists(test_file)) {
        fs::remove(test_file);
    }
}
BENCHMARK(BM_SyncWriter_Throughput);

//-----------------------------------------------------------------------------
// Queue Latency Measurement
//-----------------------------------------------------------------------------

static void BM_AsyncWriter_QueueLatency(benchmark::State& state) {
    auto null_writer = std::make_unique<kcenon::logger::null_writer>();
    auto async_writer = std::make_unique<kcenon::logger::async_writer>(
        std::move(null_writer), 10000);
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(async_writer));

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        logger->info("Queue latency test");
        auto end = std::chrono::high_resolution_clock::now();

        auto latency = std::chrono::duration_cast<std::chrono::nanoseconds>(
            end - start).count();
        state.counters["latency_ns"] = latency;
    }

    logger.reset(); // Flush queue
}
BENCHMARK(BM_AsyncWriter_QueueLatency);

//-----------------------------------------------------------------------------
// Queue Size Impact
//-----------------------------------------------------------------------------

static void BM_AsyncWriter_QueueSizeImpact(benchmark::State& state) {
    const size_t queue_size = state.range(0);

    auto null_writer = std::make_unique<kcenon::logger::null_writer>();
    auto async_writer = std::make_unique<kcenon::logger::async_writer>(
        std::move(null_writer), queue_size);
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(async_writer));

    size_t messages_logged = 0;

    for (auto _ : state) {
        logger->info("Queue size test message {}", messages_logged++);
    }

    logger.reset(); // Flush queue

    state.SetItemsProcessed(messages_logged);
    state.counters["queue_size"] = queue_size;
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged, benchmark::Counter::kIsRate);
}
BENCHMARK(BM_AsyncWriter_QueueSizeImpact)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

//-----------------------------------------------------------------------------
// Queue Saturation Test
//-----------------------------------------------------------------------------

static void BM_AsyncWriter_QueueSaturation(benchmark::State& state) {
    const size_t small_queue_size = 100;

    auto null_writer = std::make_unique<kcenon::logger::null_writer>();
    auto async_writer = std::make_unique<kcenon::logger::async_writer>(
        std::move(null_writer), small_queue_size);
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(async_writer));

    size_t messages_logged = 0;
    size_t blocked_writes = 0;

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        logger->info("Saturation test message {}", messages_logged++);
        auto end = std::chrono::high_resolution_clock::now();

        auto write_time = std::chrono::duration_cast<std::chrono::microseconds>(
            end - start).count();

        // If write took > 100us, likely queue was full and blocked
        if (write_time > 100) {
            blocked_writes++;
        }
    }

    logger.reset(); // Flush queue

    state.SetItemsProcessed(messages_logged);
    state.counters["blocked_writes"] = blocked_writes;
    state.counters["block_rate"] = static_cast<double>(blocked_writes) / messages_logged;
}
BENCHMARK(BM_AsyncWriter_QueueSaturation)->MinTime(1.0);

//-----------------------------------------------------------------------------
// Multi-threaded Async Performance
//-----------------------------------------------------------------------------

static void BM_AsyncWriter_Multithreaded(benchmark::State& state) {
    static std::shared_ptr<kcenon::logger::logger> shared_logger;

    if (state.thread_index() == 0) {
        auto null_writer = std::make_unique<kcenon::logger::null_writer>();
        auto async_writer = std::make_unique<kcenon::logger::async_writer>(
            std::move(null_writer), 100000); // Large queue
        shared_logger = std::make_shared<kcenon::logger::logger>();
        shared_logger->add_writer(std::move(async_writer));
    }

    std::atomic<size_t> messages_logged{0};

    for (auto _ : state) {
        shared_logger->info("Thread {} async message", state.thread_index());
        messages_logged++;
    }

    state.SetItemsProcessed(messages_logged.load());
    state.counters["thread_count"] = state.threads();
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged.load(), benchmark::Counter::kIsRate);

    if (state.thread_index() == 0) {
        shared_logger.reset(); // Flush queue
    }
}
BENCHMARK(BM_AsyncWriter_Multithreaded)
    ->Threads(2)
    ->Threads(4)
    ->Threads(8)
    ->Threads(16)
    ->UseRealTime();

//-----------------------------------------------------------------------------
// Flush Overhead
//-----------------------------------------------------------------------------

static void BM_AsyncWriter_FlushOverhead(benchmark::State& state) {
    std::string test_file = "async_flush_bench.log";

    for (auto _ : state) {
        state.PauseTiming();

        auto file_writer = std::make_unique<kcenon::logger::file_writer>(test_file);
        auto async_writer = std::make_unique<kcenon::logger::async_writer>(
            std::move(file_writer), 1000);
        auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(async_writer));

        // Write some messages
        for (int i = 0; i < 100; ++i) {
            logger->info("Message before flush {}", i);
        }

        state.ResumeTiming();

        // Measure flush time
        auto start = std::chrono::high_resolution_clock::now();
        logger.reset(); // Triggers flush
        auto end = std::chrono::high_resolution_clock::now();

        auto flush_time = std::chrono::duration_cast<std::chrono::microseconds>(
            end - start).count();

        state.counters["flush_time_us"] = flush_time;
    }

    // Cleanup
    if (fs::exists(test_file)) {
        fs::remove(test_file);
    }
}
BENCHMARK(BM_AsyncWriter_FlushOverhead)->Iterations(10);

//-----------------------------------------------------------------------------
// Async Writer with Variable Message Size
//-----------------------------------------------------------------------------

static void BM_AsyncWriter_VariableMessageSize(benchmark::State& state) {
    auto null_writer = std::make_unique<kcenon::logger::null_writer>();
    auto async_writer = std::make_unique<kcenon::logger::async_writer>(
        std::move(null_writer), 10000);
    auto logger = std::make_unique<kcenon::logger::logger>();
    logger->add_writer(std::move(async_writer));

    std::vector<std::string> messages = {
        "Short",
        std::string(50, 'x'),
        std::string(200, 'y'),
        std::string(1000, 'z'),
    };

    size_t messages_logged = 0;
    size_t msg_index = 0;

    for (auto _ : state) {
        logger->info("{}", messages[msg_index++ % messages.size()]);
        messages_logged++;
    }

    logger.reset(); // Flush queue

    state.SetItemsProcessed(messages_logged);
    state.counters["messages/sec"] = benchmark::Counter(
        messages_logged, benchmark::Counter::kIsRate);
}
BENCHMARK(BM_AsyncWriter_VariableMessageSize);
