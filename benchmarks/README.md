# logger_system Performance Benchmarks

Phase 0, Task 0.2: Baseline Performance Benchmarking

## Overview

This directory contains comprehensive performance benchmarks for the logger_system, measuring:

- **Write Latency**: Single log write performance (p50, p95, p99)
- **Throughput**: Sustained logging performance (messages/sec)
- **File Rotation**: Rotation overhead and performance impact
- **Async Writer**: Asynchronous logging performance and queue behavior

## Building

### Prerequisites

```bash
# macOS (Homebrew)
brew install google-benchmark

# Ubuntu/Debian
sudo apt-get install libbenchmark-dev

# From source
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -E make_directory build
cmake -E chdir build cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../
cmake --build build --config Release
sudo cmake --build build --config Release --target install
```

### Build Benchmarks

```bash
cd logger_system
cmake -B build -S . -DLOGGER_BUILD_BENCHMARKS=ON
cmake --build build

# Or use the build target
cd build
make logger_benchmarks
```

## Running Benchmarks

### Run All Benchmarks

```bash
./build/benchmarks/logger_benchmarks
```

### Run Specific Benchmark Categories

```bash
# Write latency benchmarks only
./build/benchmarks/logger_benchmarks --benchmark_filter=WriteLatency

# Throughput benchmarks only
./build/benchmarks/logger_benchmarks --benchmark_filter=Throughput

# Rotation benchmarks only
./build/benchmarks/logger_benchmarks --benchmark_filter=Rotation

# Async writer benchmarks only
./build/benchmarks/logger_benchmarks --benchmark_filter=Async
```

### Output Formats

```bash
# Console output (default)
./build/benchmarks/logger_benchmarks

# JSON output
./build/benchmarks/logger_benchmarks --benchmark_format=json

# CSV output
./build/benchmarks/logger_benchmarks --benchmark_format=csv

# Save to file
./build/benchmarks/logger_benchmarks --benchmark_format=json --benchmark_out=results.json
```

### Advanced Options

```bash
# Run for minimum time (for stable results)
./build/benchmarks/logger_benchmarks --benchmark_min_time=5.0

# Specify number of iterations
./build/benchmarks/logger_benchmarks --benchmark_repetitions=10

# Show all statistics
./build/benchmarks/logger_benchmarks --benchmark_report_aggregates_only=false
```

## Benchmark Categories

### 1. Write Latency Benchmarks

**File**: `logger_write_bench.cpp`

Measures single log write performance:

- Null writer (measures logger overhead only)
- File writer (measures full file I/O)
- Different log levels (trace, debug, info, warning, error)
- Message formatting overhead (0, 1, 3, 10 arguments)
- Message size impact (50, 500, 5000 bytes)
- Memory allocation overhead

**Target Metrics**:
- Write latency: < 100μs (p99)
- File write latency: < 1ms (p99)

### 2. Throughput Benchmarks

**File**: `logger_throughput_bench.cpp`

Measures sustained logging performance:

- Single-threaded throughput
- Multi-threaded throughput
- Burst throughput (log storms)
- Sustained throughput (long duration)
- Throughput with variable message sizes
- Throughput with log level filtering
- Contention under heavy multi-threaded load

**Target Metrics**:
- Throughput: > 100k messages/sec (single-threaded)
- Multi-threaded scaling: near-linear up to 4 threads

### 3. Rotation Benchmarks

**File**: `logger_rotation_bench.cpp`

Measures file rotation performance:

- Rotation overhead time
- Write performance during rotation
- File size threshold accuracy
- Maximum files rotation behavior
- Concurrent rotation (multi-threaded)

**Target Metrics**:
- Rotation overhead: < 10ms
- Minimal performance degradation during rotation

### 4. Async Writer Benchmarks

**File**: `logger_async_bench.cpp`

Measures asynchronous logging performance:

- Async vs sync throughput comparison
- Queue latency measurement
- Queue size impact
- Queue saturation behavior
- Multi-threaded async performance
- Flush overhead
- Variable message size handling

**Target Metrics**:
- Async queue latency: < 1ms (p99)
- Throughput improvement: > 2x vs synchronous
- Queue saturation: graceful degradation

## Baseline Results

**To be measured**: Run benchmarks and record results in `docs/BASELINE.md`

Expected baseline ranges (to be confirmed):

| Metric | Target | Acceptable |
|--------|--------|------------|
| Write Latency (null) | < 1μs | < 10μs |
| Write Latency (file) | < 100μs | < 1ms |
| Throughput (single) | > 100k msg/s | > 50k msg/s |
| Throughput (4 threads) | > 300k msg/s | > 150k msg/s |
| Rotation Overhead | < 10ms | < 50ms |
| Async Queue Latency | < 100μs | < 1ms |

## Interpreting Results

### Understanding Benchmark Output

```
---------------------------------------------------------------
Benchmark                         Time           CPU Iterations
---------------------------------------------------------------
BM_WriteLatency_Info          2456 ns       2455 ns     284891
```

- **Time**: Wall clock time per iteration
- **CPU**: CPU time per iteration
- **Iterations**: Number of times the benchmark was run

### Percentiles

For latency-sensitive benchmarks, focus on p95 and p99:

```bash
# Run with statistics
./build/benchmarks/logger_benchmarks --benchmark_enable_random_interleaving=true
```

### Comparison

To compare before/after performance:

```bash
# Baseline
./build/benchmarks/logger_benchmarks --benchmark_out=baseline.json --benchmark_out_format=json

# After changes
./build/benchmarks/logger_benchmarks --benchmark_out=after.json --benchmark_out_format=json

# Compare (requires benchmark tools)
compare.py baseline.json after.json
```

## CI Integration

Benchmarks are run in CI on every PR to detect performance regressions.

See `.github/workflows/benchmarks.yml` for configuration.

## Troubleshooting

### Google Benchmark Not Found

```bash
# Check installation
find /usr -name "*benchmark*" 2>/dev/null

# Try pkg-config
pkg-config --modversion benchmark

# Reinstall
brew reinstall google-benchmark  # macOS
```

### Build Errors

```bash
# Clean build
rm -rf build
cmake -B build -S . -DLOGGER_BUILD_BENCHMARKS=ON
cmake --build build -j8
```

### Unstable Results

```bash
# Increase minimum time
./build/benchmarks/logger_benchmarks --benchmark_min_time=10.0

# Disable CPU frequency scaling (Linux)
sudo cpupower frequency-set --governor performance
```

## Contributing

When adding new benchmarks:

1. Follow existing naming conventions (`BM_Category_SpecificTest`)
2. Use appropriate benchmark types (Fixture, Threaded, etc.)
3. Set meaningful labels and counters
4. Document target metrics in file header
5. Clean up resources in TearDown/after benchmark
6. Update this README with new benchmark description

## References

- [Google Benchmark Documentation](https://github.com/google/benchmark)
- [Benchmark Best Practices](https://github.com/google/benchmark/blob/main/docs/user_guide.md)
- [logger_system Architecture](../docs/LOGGER_SYSTEM_ARCHITECTURE.md)

---

**Last Updated**: 2025-10-07
**Phase**: 0 - Foundation and Tooling
**Status**: Baseline measurement in progress
