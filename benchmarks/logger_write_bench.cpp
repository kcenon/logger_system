/**
 * @file logger_write_bench.cpp
 * @brief Benchmark for logger write latency
 * @details Measures single log write performance across different log levels
 *
 * Target Metrics:
 * - Write latency (p50, p95, p99): < 100μs
 * - Memory allocation per write: minimal
 *
 * Phase 0, Task 0.2: Baseline Performance Benchmarking
 */

#include <benchmark/benchmark.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/writers/null_writer.h>
#include <filesystem>
#include <memory>
#include <string>

namespace fs = std::filesystem;

// Fixture for logger benchmarks
class LoggerWriteFixture : public benchmark::Fixture {
protected:
    std::unique_ptr<kcenon::logger::logger> file_logger_;
    std::unique_ptr<kcenon::logger::logger> null_logger_;
    std::string test_log_file_;

    void SetUp(const ::benchmark::State& state) override {
        // Create temporary log file
        test_log_file_ = "benchmark_test.log";

        // Create logger with null writer (measures logger overhead only)
        auto null_writer = std::make_unique<kcenon::logger::null_writer>();
        null_logger_ = std::make_unique<kcenon::logger::logger>();
        null_logger_->add_writer(std::move(null_writer));

        // Create logger with file writer (measures full file I/O)
        auto file_writer = std::make_unique<kcenon::logger::file_writer>(test_log_file_);
        file_logger_ = std::make_unique<kcenon::logger::logger>();
        file_logger_->add_writer(std::move(file_writer));
    }

    void TearDown(const ::benchmark::State& state) override {
        null_logger_.reset();
        file_logger_.reset();

        // Clean up test file
        if (fs::exists(test_log_file_)) {
            fs::remove(test_log_file_);
        }
    }
};

//-----------------------------------------------------------------------------
// Single Write Benchmarks
//-----------------------------------------------------------------------------

BENCHMARK_F(LoggerWriteFixture, WriteToNullLogger_Info)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->info("Benchmark message with {} placeholder", 42);
    }

    state.SetItemsProcessed(state.iterations());
    state.SetLabel("null_writer");
}

BENCHMARK_F(LoggerWriteFixture, WriteToFileLogger_Info)(benchmark::State& state) {
    for (auto _ : state) {
        file_logger_->info("Benchmark message with {} placeholder", 42);
    }

    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * 50); // Approximate message size
    state.SetLabel("file_writer");
}

//-----------------------------------------------------------------------------
// Write Latency by Log Level
//-----------------------------------------------------------------------------

BENCHMARK_F(LoggerWriteFixture, WriteLatency_Trace)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->trace("Trace level message");
    }
    state.SetLabel("trace_level");
}

BENCHMARK_F(LoggerWriteFixture, WriteLatency_Debug)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->debug("Debug level message");
    }
    state.SetLabel("debug_level");
}

BENCHMARK_F(LoggerWriteFixture, WriteLatency_Info)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->info("Info level message");
    }
    state.SetLabel("info_level");
}

BENCHMARK_F(LoggerWriteFixture, WriteLatency_Warning)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->warning("Warning level message");
    }
    state.SetLabel("warning_level");
}

BENCHMARK_F(LoggerWriteFixture, WriteLatency_Error)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->error("Error level message");
    }
    state.SetLabel("error_level");
}

//-----------------------------------------------------------------------------
// Message Formatting Overhead
//-----------------------------------------------------------------------------

BENCHMARK_F(LoggerWriteFixture, FormattingOverhead_NoArgs)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->info("Simple message without formatting");
    }
    state.SetLabel("no_args");
}

BENCHMARK_F(LoggerWriteFixture, FormattingOverhead_OneArg)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->info("Message with one arg: {}", 42);
    }
    state.SetLabel("one_arg");
}

BENCHMARK_F(LoggerWriteFixture, FormattingOverhead_ThreeArgs)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->info("Message with three args: {} {} {}", 42, "test", 3.14);
    }
    state.SetLabel("three_args");
}

BENCHMARK_F(LoggerWriteFixture, FormattingOverhead_TenArgs)(benchmark::State& state) {
    for (auto _ : state) {
        null_logger_->info("Ten args: {} {} {} {} {} {} {} {} {} {}",
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    }
    state.SetLabel("ten_args");
}

//-----------------------------------------------------------------------------
// Message Size Impact
//-----------------------------------------------------------------------------

BENCHMARK_F(LoggerWriteFixture, MessageSize_Small)(benchmark::State& state) {
    std::string small_msg(50, 'x');
    for (auto _ : state) {
        file_logger_->info("{}", small_msg);
    }
    state.SetBytesProcessed(state.iterations() * small_msg.size());
    state.SetLabel("50_bytes");
}

BENCHMARK_F(LoggerWriteFixture, MessageSize_Medium)(benchmark::State& state) {
    std::string medium_msg(500, 'x');
    for (auto _ : state) {
        file_logger_->info("{}", medium_msg);
    }
    state.SetBytesProcessed(state.iterations() * medium_msg.size());
    state.SetLabel("500_bytes");
}

BENCHMARK_F(LoggerWriteFixture, MessageSize_Large)(benchmark::State& state) {
    std::string large_msg(5000, 'x');
    for (auto _ : state) {
        file_logger_->info("{}", large_msg);
    }
    state.SetBytesProcessed(state.iterations() * large_msg.size());
    state.SetLabel("5000_bytes");
}

//-----------------------------------------------------------------------------
// Memory Allocation Benchmarks
//-----------------------------------------------------------------------------

BENCHMARK_F(LoggerWriteFixture, MemoryAllocation_Overhead)(benchmark::State& state) {
    // Measures memory allocation overhead by logging strings of increasing size
    size_t allocation_count = 0;

    for (auto _ : state) {
        state.PauseTiming();
        std::string msg(state.range(0), 'x');
        state.ResumeTiming();

        null_logger_->info("{}", msg);
        allocation_count++;
    }

    state.counters["allocations"] = allocation_count;
}
BENCHMARK_REGISTER_F(LoggerWriteFixture, MemoryAllocation_Overhead)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000);

//-----------------------------------------------------------------------------
// Register benchmarks with custom arguments
//-----------------------------------------------------------------------------

// Run write benchmarks with different thread counts
BENCHMARK_F(LoggerWriteFixture, WriteToFileLogger_Info)->Threads(1);
BENCHMARK_F(LoggerWriteFixture, WriteToFileLogger_Info)->Threads(2);
BENCHMARK_F(LoggerWriteFixture, WriteToFileLogger_Info)->Threads(4);
BENCHMARK_F(LoggerWriteFixture, WriteToFileLogger_Info)->Threads(8);
