// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#include <benchmark/benchmark.h>
#include <logger/config/logger_builder.h>
#include <logger/writers/console_writer.h>
#include <memory>
#include <thread>
#include <vector>

using namespace logger_module;

/**
 * @brief Placeholder benchmark for batch processing
 * 
 * This benchmark simulates batch processing behavior
 * until the actual batch_writer implementation is available.
 */

// Null writer for benchmarking
class benchmark_writer : public base_writer {
private:
    std::atomic<size_t> write_count_{0};
    
public:
    common::VoidResult write(thread_module::log_level level,
                     const std::string& message,
                     const std::string& file,
                     int line,
                     const std::string& function,
                     const std::chrono::system_clock::time_point& timestamp) override {
        write_count_++;
        benchmark::DoNotOptimize(message.size());
        return common::ok();
    }

    common::VoidResult flush() override {
        return common::ok();
    }
    
    std::string get_name() const override {
        return "benchmark_writer";
    }
    
    size_t get_write_count() const { return write_count_; }
};

/**
 * @brief Benchmark simulating batch vs direct writing
 */
static void BM_SimulatedBatchWriting(benchmark::State& state) {
    bool simulate_batch = state.range(0);
    size_t batch_size = state.range(1);
    
    auto writer = std::make_unique<benchmark_writer>();
    auto* writer_ptr = writer.get();
    
    const std::string message = "Simulated batch processing test message";
    const auto timestamp = std::chrono::system_clock::now();
    
    std::vector<std::pair<thread_module::log_level, std::string>> batch;
    
    for (auto _ : state) {
        if (simulate_batch) {
            // Simulate batching behavior
            batch.push_back({thread_module::log_level::info, message});
            
            if (batch.size() >= batch_size) {
                // Simulate batch flush
                for (const auto& [level, msg] : batch) {
                    writer->write(level, msg, __FILE__, __LINE__, __FUNCTION__, timestamp);
                }
                batch.clear();
            }
        } else {
            // Direct write
            writer->write(thread_module::log_level::info, 
                         message, __FILE__, __LINE__, __FUNCTION__, timestamp);
        }
    }
    
    // Flush remaining batch
    if (!batch.empty()) {
        for (const auto& [level, msg] : batch) {
            writer->write(level, msg, __FILE__, __LINE__, __FUNCTION__, timestamp);
        }
    }
    
    state.SetItemsProcessed(state.iterations());
    state.SetLabel(simulate_batch ? 
                   "batch_size: " + std::to_string(batch_size) : 
                   "direct");
}
BENCHMARK(BM_SimulatedBatchWriting)
    ->Args({0, 0})    // Direct writing
    ->Args({1, 10})   // Batch size 10
    ->Args({1, 50})   // Batch size 50
    ->Args({1, 100})  // Batch size 100
    ->Args({1, 500}); // Batch size 500

/**
 * @brief Benchmark logger with batch configuration
 */
static void BM_LoggerWithBatchConfig(benchmark::State& state) {
    size_t batch_size = state.range(0);
    
    logger_builder builder;
    auto logger = builder
        .with_async(true)
        .with_batch_size(batch_size)
        .add_writer("benchmark", std::make_unique<benchmark_writer>())
        .build();
    
    if (!logger) {
        state.SkipWithError("Failed to build logger");
        return;
    }
    
    auto& log = *logger.value();
    log->start();
    
    const std::string message = "Logger batch configuration test";
    
    for (auto _ : state) {
        log->log(thread_module::log_level::info, message);
    }
    
    log->stop();
    
    state.SetItemsProcessed(state.iterations());
    state.SetLabel("batch_size: " + std::to_string(batch_size));
}
BENCHMARK(BM_LoggerWithBatchConfig)
    ->Arg(1)
    ->Arg(10)
    ->Arg(50)
    ->Arg(100)
    ->Arg(500);

/**
 * @brief Benchmark multi-threaded logging with batching
 */
static void BM_MultithreadedBatchSimulation(benchmark::State& state) {
    int thread_count = state.range(0);
    size_t batch_size = state.range(1);
    
    logger_builder builder;
    auto logger = builder
        .with_async(true)
        .with_batch_size(batch_size)
        .add_writer("benchmark", std::make_unique<benchmark_writer>())
        .build();
    
    if (!logger) {
        state.SkipWithError("Failed to build logger");
        return;
    }
    
    auto& log = *logger.value();
    log->start();
    
    const std::string message = "Multi-threaded batch test";
    
    for (auto _ : state) {
        std::vector<std::thread> threads;
        threads.reserve(thread_count);
        
        auto work = [&log, &message]() {
            for (int i = 0; i < 100; ++i) {
                log->log(thread_module::log_level::info, message);
            }
        };
        
        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back(work);
        }
        
        for (auto& t : threads) {
            t.join();
        }
    }
    
    log->stop();
    
    state.SetItemsProcessed(state.iterations() * thread_count * 100);
    state.SetLabel("threads: " + std::to_string(thread_count) + 
                   ", batch: " + std::to_string(batch_size));
}
BENCHMARK(BM_MultithreadedBatchSimulation)
    ->Args({1, 10})
    ->Args({2, 10})
    ->Args({4, 10})
    ->Args({1, 100})
    ->Args({2, 100})
    ->Args({4, 100});

BENCHMARK_MAIN();