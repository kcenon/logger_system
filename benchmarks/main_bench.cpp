/**
 * @file main_bench.cpp
 * @brief Main entry point for logger_system benchmarks
 * @details Initializes Google Benchmark and runs all registered benchmarks
 *
 * Usage:
 *   ./logger_benchmarks                                    # Run all benchmarks
 *   ./logger_benchmarks --benchmark_filter=Throughput      # Run specific benchmarks
 *   ./logger_benchmarks --benchmark_format=json            # Output JSON
 *   ./logger_benchmarks --benchmark_out=results.json       # Save to file
 *
 * Phase 0, Task 0.2: Baseline Performance Benchmarking
 */

#include <benchmark/benchmark.h>
#include <iostream>

int main(int argc, char** argv) {
    // Print banner
    std::cout << "========================================\n";
    std::cout << "logger_system Performance Benchmarks\n";
    std::cout << "Phase 0: Baseline Measurement\n";
    std::cout << "========================================\n\n";

    // Initialize and run benchmarks
    benchmark::Initialize(&argc, argv);

    if (benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    }

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    std::cout << "\n========================================\n";
    std::cout << "Benchmarks Complete\n";
    std::cout << "========================================\n";

    return 0;
}
