# Logger System Integration Tests

Comprehensive integration testing suite for the logger_system, validating end-to-end functionality, performance characteristics, and error handling across multiple scenarios.

## Overview

This test suite provides extensive integration testing coverage for the logger_system, focusing on real-world usage scenarios, performance validation, and robustness under various conditions.

### Test Coverage

- **Total Tests**: 46 integration tests
- **Test Suites**: 4 comprehensive test suites
- **Test Categories**: Scenarios, Performance, Failures

## Test Suites

### 1. Logger Lifecycle Tests (15 tests)

**File**: `scenarios/logger_lifecycle_test.cpp`

Validates core logger lifecycle operations:

- Logger initialization and destruction
- Start/stop operations
- Multiple start/stop cycles
- Log level changes at runtime
- Writer management (add, remove, clear)
- Concurrent logging from multiple threads
- Synchronous vs asynchronous modes
- Flush behavior
- Resource cleanup
- Log filtering with `is_enabled()`
- Source location logging

**Key Scenarios**:
- Creating and destroying logger instances
- Logging before and after start
- Dynamic writer management while running
- Concurrent logging from 8+ threads
- Multiple logger instances operating independently

### 2. Log Formatting Integration Tests (12 tests)

**File**: `scenarios/log_formatting_integration_test.cpp`

Validates log message formatting and handling:

- Basic message formatting
- Multiple log levels formatting
- Timestamp presence and format
- Thread ID inclusion
- Source location formatting
- Long message handling (5KB+ messages)
- Multi-line message handling
- Special characters and escape sequences
- Empty messages
- Message ordering under load
- Unicode/internationalization support

**Key Scenarios**:
- Processing 1000+ consecutive messages
- Unicode text handling (Korean, Japanese, Chinese, Russian)
- Special character preservation
- Order preservation verification

### 3. Logger Performance Tests (9 tests)

**File**: `performance/logger_performance_test.cpp`

Validates performance characteristics and baselines:

**Performance Targets**:
- **Throughput**: > 100,000 messages/second (async mode)
- **Latency P50**: < 10 microseconds
- **Latency P95**: < 100 microseconds
- **Memory**: Stable under 100K+ message load

**Test Scenarios**:
- Async mode throughput measurement
- Sync mode throughput measurement
- Async vs sync performance comparison
- Multi-threaded throughput (8 threads)
- Scalability with thread count (1, 2, 4, 8 threads)
- Large message performance (1KB messages)
- Burst logging patterns
- Memory usage under sustained load

**Metrics Collected**:
- Messages per second
- P50, P95, P99 latency percentiles
- Mean latency
- Data rate (MB/sec for large messages)

### 4. Error Handling Tests (12 tests)

**File**: `failures/error_handling_test.cpp`

Validates robustness and error recovery:

- Invalid file paths handling
- Non-existent directory handling
- Multiple start/stop cycles
- Double start/stop operations
- Logging without start
- Logging after stop
- Excessive flush calls
- Writer management while running
- Very long messages (10MB+)
- Rapid log level changes under concurrent load
- Multiple logger instances isolation

**Key Scenarios**:
- Graceful degradation under error conditions
- No crashes or resource leaks
- Proper error signaling through Result<T>
- Thread safety under stress

## Test Infrastructure

### Framework Components

**`framework/system_fixture.h`**:
- `LoggerSystemFixture`: Base fixture for single-logger tests
- `MultiLoggerFixture`: Fixture for multi-logger scenarios
- Temporary file management
- Log verification utilities
- Wait helpers with timeout support

**`framework/test_helpers.h`**:
- `ScopedTimer`: RAII timing measurement
- `PerformanceMetrics`: Statistical analysis (P50/P95/P99)
- `WorkSimulator`: CPU work simulation
- `BarrierSync`: Thread synchronization
- `RateLimiter`: Operation rate control
- `TempLogFile`: RAII temporary file management
- `MockLogWriter`: Mock writer for testing

### Helper Functions

- `WaitForCondition()`: Generic condition waiting with timeout
- `WaitForFile()`: Wait for file existence
- `WaitForLogLines()`: Wait for expected log line count
- `ReadLogFile()`: Read entire log file contents
- `LogContains()`: Check for substring in log
- `CountLogLines()`: Count non-empty lines
- `CountOccurrences()`: Count substring occurrences
- `GetLogLines()`: Get all lines as vector
- `CalculateThroughput()`: Compute ops/second
- `FormatDuration()`: Human-readable duration
- `GenerateRandomString()`: Random string generation

## Building and Running

### Build Configuration

```bash
# Configure with integration tests enabled
cmake -B build \
  -DLOGGER_BUILD_INTEGRATION_TESTS=ON \
  -DBUILD_TESTS=ON \
  -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build -j$(nproc)
```

### Run All Integration Tests

```bash
cd build
ctest -L integration --output-on-failure
```

### Run Specific Test Suite

```bash
# Run only lifecycle tests
./integration_tests --gtest_filter="LoggerLifecycleTest.*"

# Run only performance tests
./integration_tests --gtest_filter="LoggerPerformanceTest.*"

# Run only error handling tests
./integration_tests --gtest_filter="ErrorHandlingTest.*"

# Run only formatting tests
./integration_tests --gtest_filter="LogFormattingIntegrationTest.*"
```

### Run with Verbose Output

```bash
./integration_tests --gtest_filter="*" --gtest_print_time=1
```

## Performance Baselines

### Throughput Targets

| Mode | Threads | Target (msgs/sec) | Notes |
|------|---------|-------------------|-------|
| Async | 1 | > 100,000 | Single producer |
| Async | 8 | > 100,000 | Multi-producer |
| Sync | 1 | > 1,000 | Baseline comparison |

### Latency Targets

| Percentile | Target | Notes |
|------------|--------|-------|
| P50 | < 10 µs | Median latency |
| P95 | < 100 µs | 95th percentile |
| P99 | < 1 ms | 99th percentile (async queue delay acceptable) |

### Memory Targets

| Scenario | Target | Notes |
|----------|--------|-------|
| 100K msgs | < 50 MB | Total memory overhead |
| 1M msgs | < 100 MB | Sustained logging |

## CI/CD Integration

Integration tests run automatically in CI/CD pipelines:

- **Platforms**: Ubuntu, macOS
- **Configurations**: Debug, Release
- **Coverage**: Generated for Debug builds (lcov)
- **Artifacts**: Test results, coverage reports, performance metrics

### Workflow File

`.github/workflows/integration-tests.yml`

Features:
- Matrix builds (Ubuntu/macOS × Debug/Release)
- Coverage reporting with lcov
- Performance baseline validation
- Artifact upload (reports, metrics)

## Coverage Goals

- **Line Coverage**: > 80% of integration-testable code
- **Branch Coverage**: > 70% of decision points
- **Function Coverage**: > 85% of public APIs

## Test Maintenance

### Adding New Tests

1. Choose appropriate test suite file
2. Add test using `TEST_F` with proper fixture
3. Follow existing naming conventions
4. Include clear test documentation
5. Update test count in this README

### Performance Regression Detection

Performance tests validate against established baselines:
- Throughput must exceed minimum thresholds
- Latency percentiles must stay within bounds
- CI fails on performance regressions

### Debugging Failed Tests

```bash
# Run with GDB
gdb --args ./integration_tests --gtest_filter="FailingTest"

# Run with sanitizers (if built with sanitizers)
ASAN_OPTIONS=detect_leaks=1 ./integration_tests

# Increase verbosity
./integration_tests --gtest_filter="FailingTest" --gtest_repeat=10
```

## Dependencies

- **Google Test**: Unit testing framework
- **C++20**: Modern C++ features
- **Filesystem**: Temporary file management
- **Threads**: Multi-threading support

## Known Limitations

1. **Platform-specific behavior**: File permissions and paths vary by OS
2. **Performance variability**: CI environments may show different performance
3. **Timing-dependent tests**: Use generous timeouts to avoid flakiness
4. **Temporary files**: Cleaned up in teardown, but may persist on crash

## Future Enhancements

- [ ] Add network writer integration tests
- [ ] Add encryption writer tests
- [ ] Add log rotation integration tests
- [ ] Add structured logging tests
- [ ] Add metrics collection validation
- [ ] Add health check integration tests

## Related Documentation

- [Logger System README](../README.md)
- [Architecture Documentation](../docs/architecture.md)
- [API Reference](../docs/api.md)
- [Performance Tuning Guide](../docs/performance.md)
