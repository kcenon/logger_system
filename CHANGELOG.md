# Changelog - Logger System

> **Language:** **English** | [한국어](CHANGELOG_KO.md)

All notable changes to the Logger System project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## Phase 2: Performance Optimization - 2025-11-03

### Added
- **thread_local_object_pool**: New thread-local cached object pool implementation
  - Per-thread local cache to minimize lock contention
  - Batch transfers between local cache and global pool
  - Configurable cache size (default: 16 objects per thread)
  - Detailed statistics tracking (cache hits, global pool hits, allocations)
  - **Performance**: 2.4x faster multi-threaded performance (8 threads)
  - **Performance**: 2.6x faster single-threaded performance
  - See `src/impl/memory/object_pool.h` for implementation

- **object_pool_bench**: Comprehensive object pool benchmarks
  - Single-threaded and multi-threaded performance tests
  - High contention scenario testing
  - Cache efficiency measurements
  - Located in `benchmarks/object_pool_bench.cpp`

### Changed
- **rotating_file_writer**: Periodic rotation checks for improved performance
  - Added `check_interval` parameter (default: 100 writes)
  - Reduces filesystem system calls from every write to every 100 writes
  - **Expected performance**: 10-20% improvement in throughput
  - Maintains rotation accuracy with configurable interval
  - All constructors now accept optional `check_interval` parameter
  - Backward compatible with default value

### Performance
- **object_pool improvements** (8 threads):
  - Original: 949 ns/op (1.05M ops/s)
  - Optimized: 396 ns/op (2.53M ops/s)
  - **Improvement: 2.4x faster**

- **object_pool improvements** (single thread):
  - Original: 18.2 ns/op (54.9M ops/s)
  - Optimized: 7.03 ns/op (142.4M ops/s)
  - **Improvement: 2.6x faster**

- **High contention scenario** (8 threads, 10 objects):
  - Original: 6336 ns/op
  - Optimized: 3534 ns/op
  - **Improvement: 1.8x faster**

- All Phase 1 + Phase 2 tests passing: 7/7

---

## Phase 1: Critical Fixes - 2025-11-03

### Fixed
- **async_writer**: Removed timeout-based detach mechanism that caused memory leaks and message loss
  - Replaced 5-second timeout with infinite wait for safer shutdown
  - Worker thread now always joins properly without detaching
  - Added `force_flush` parameter to control remaining message processing
  - Eliminated potential use-after-free scenarios during shutdown

- **file_writer**: Changed `std::endl` to `'\n'` for 50-100x performance improvement
  - Eliminated forced disk synchronization (`fsync`) on every write
  - Leverages OS buffering for better I/O performance
  - Explicit `flush()` method still available when needed
  - **Expected improvement**: ~10K msg/s → ~1M msg/s on SSD

- **rotating_file_writer**: Changed `std::endl` to `'\n'` for better performance
  - Consistent with file_writer improvements
  - Maintains all rotation functionality while improving throughput

- **console_writer**: Changed `std::endl` to `'\n'` for better performance
  - Reduces console output overhead
  - Maintains proper newline behavior

### Performance
- File writing performance improved by **50-100x** (from ~10K msg/s to ~1M msg/s on SSD)
- Async writer shutdown now prioritizes safety over speed (no message loss)
- All existing tests pass without regressions (7/7 tests passing)

---

## Initial Release - 2025-10-22

### Added
- **Core Logger System**: Production-ready C++20 asynchronous logging framework
  - Asynchronous processing with non-blocking log operations
  - Multiple output targets (console, file, rotating file, network, encrypted, hybrid)
  - Thread-safe operations with concurrent logging support
  - Zero-copy design for efficient message passing

- **Critical Writer**: Synchronous logging for critical messages requiring immediate write
  - Emergency flush mechanism to preserve logs during abnormal termination
  - Crash-safe logging with automatic buffer flushing

- **Advanced Architecture**:
  - Interface-driven design with ILogger, IMonitor, IMonitorable abstractions
  - Modular components with pluggable writers, filters, formatters
  - Zero circular dependencies through interface-only dependencies
  - Independent compilation without ecosystem dependencies

- **Builder Pattern API**: Fluent, type-safe logger construction
  - Configuration strategies (template-based, environment-aware, performance tuning)
  - Comprehensive validation with Result<T> error handling
  - Dependency injection for optional runtime component injection

- **CMake Build System**: Comprehensive build configuration
  - 15+ feature flags for flexible builds
  - Automatic dependency detection
  - Cross-platform support (Windows, Linux, macOS)
  - Compiler support (GCC, Clang, MSVC)

- **Writers**:
  - `console_writer`: Standard output/error stream logging
  - `file_writer`: Basic file output
  - `rotating_file_writer`: Automatic log rotation by size/time
  - `network_writer`: Remote logging over network
  - `critical_writer`: Synchronous critical message logging
  - `hybrid_writer`: Combined multiple writer support

- **Formatters**:
  - `text_formatter`: Plain text format
  - `json_formatter`: Structured JSON logging
  - `xml_formatter`: XML format for legacy systems
  - Custom formatter support via IFormatter interface

- **Filters**:
  - Log level filtering
  - Pattern-based message filtering
  - Custom filter support via IFilter interface

- **Integration Support**:
  - common_system integration (ILogger, IMonitor, Result<T> pattern)
  - thread_system integration (threading primitives, logger_interface)
  - monitoring_system integration (metrics and health monitoring via IMonitor)

- **Performance Features**:
  - Configurable batching with tunable batch sizes and queue depths
  - Lock-free operations on hot path
  - Minimal allocations with efficient memory management
  - ~15 seconds build time for independent compilation

### Changed
- N/A (Initial release)

### Deprecated
- N/A (Initial release)

### Removed
- N/A (Initial release)

### Fixed
- N/A (Initial release)

### Security
- Thread-safe operations ensure concurrent logging without data races
- Encrypted writer support for sensitive log data

---

## Project Information

**Repository:** https://github.com/kcenon/logger_system
**Documentation:** [docs/](docs/)
**License:** See LICENSE file
**Maintainer:** kcenon@naver.com

---

## Version Support Matrix

| Version | Release Date | C++ Standard | CMake Minimum | Status |
|---------|--------------|--------------|---------------|---------|
| 1.0.0   | 2025-10-22   | C++20        | 3.16          | Current |

---

## Migration Guide

For migration from integrated thread_system or standalone logger implementations, see [MIGRATION.md](MIGRATION.md).

---

**Last Updated:** 2025-10-22
