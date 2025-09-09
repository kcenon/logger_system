# Changelog

All notable changes to the Logger System will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.1.0] - Phase 3 Crash Safety Implementation (2025-09-10)

### Added - Phase 3 Task A2 Complete

- **Comprehensive Crash Safety System**
  - Full implementation of `logger_crash_safety` singleton manager
  - Signal handlers for SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS
  - Emergency log flushing on crash with signal-safe operations
  - Automatic backup creation with configurable intervals
  - Crash recovery detection and handling
  - Three safety levels: minimal, standard, paranoid
  - Emergency log writing with file descriptor-based I/O

- **Log File Recovery Mechanisms**
  - Corruption detection for log files
  - Automatic recovery of corrupted log files
  - Backup creation with checksum verification
  - File integrity checking using simple hash algorithm

- **Asynchronous Logger Safety Extensions**
  - Framework for async logger crash handling
  - Overflow handler configuration
  - Emergency sync mode switching capability

- **Comprehensive Test Suite**
  - 15 unit tests covering all crash safety features
  - Tests for singleton pattern, logger registration, emergency logging
  - File recovery and corruption detection tests
  - Scoped crash protection RAII helper tests
  - All tests passing with 100% success rate

### Technical Details
- Implementation: `sources/logger/safety/crash_safety.cpp` (546 lines)
- Header updates: Added necessary system headers (signal, unistd, fcntl)
- Build integration: Updated CMakeLists.txt for new component
- Test coverage: `unittest/safety_test/` with comprehensive test cases

## [2.0.0] - Phase 2 Core Systems Complete (2025-09-09)

### Added - Phase 2 Complete (C1, C2, C3 & C4)

- **Adaptive Dependency Injection System** [C1]
  - Abstract `di_container_interface` with no external dependencies
  - `lightweight_di_container` implementation with factory and singleton support
  - Optional `thread_system_di_adapter` for advanced integration
  - `di_container_factory` for automatic container selection
  - DI support integrated into logger class with backward compatibility
  - 16 unit tests with 100% pass rate
  - Thread-safe concurrent component registration and resolution

- **Pluggable Monitoring System** [C2]
  - Abstract `monitoring_interface` for backend independence
  - `basic_monitor` implementation with zero external dependencies
  - Optional `thread_system_monitor_adapter` for advanced features
  - `monitoring_factory` for automatic backend selection
  - Comprehensive metrics collection:
    - Counters, gauges, histograms, and summaries
    - Performance metrics (processing time min/max/avg)
    - Resource metrics (buffer usage, queue size)
    - Error tracking and uptime monitoring
  - Three-level health check system (healthy/degraded/unhealthy)
  - 21 unit tests with 100% pass rate
  - Thread-safe metric collection with minimal overhead

- **Configuration Strategy Pattern** [C3]
  - Abstract `config_strategy_interface` for flexible configuration
  - Template strategies (production, debug, high_performance, low_latency)
  - Environment-based strategies (development, testing, staging, production)
  - Performance tuning strategies (conservative, balanced, aggressive)
  - Composite strategy with priority-based ordering
  - Factory pattern for strategy creation
  - Automatic environment detection from LOG_ENV and LOG_LEVEL
  - Enhanced `logger_builder` with strategy support:
    - `use_template()` for predefined configurations
    - `for_environment()` for environment-based setup
    - `with_performance_tuning()` for performance optimization
    - `auto_configure()` for automatic environment detection
    - `apply_strategy()` for custom strategies
  - Backward compatibility maintained
  - 25 unit tests with 100% pass rate

- **CMake Modularization with Feature Flags** [C4]
  - `LoggerFeatures.cmake` module with 15+ configurable options:
    - Core features (DI, monitoring, async, crash handler)
    - Advanced features (lock-free, structured logging, network writer)
    - Performance tuning (buffer/batch/queue sizes, max writers)
    - Build options (sanitizers, coverage, compression, encryption)
  - `LoggerDependencies.cmake` for automatic dependency detection
  - Enhanced `LoggerSystemConfig.cmake.in` for find_package() support
  - Feature validation and conflict resolution
  - Per-target feature configuration with `logger_configure_target()`
  - Complete build system modularization

## [1.0.0] - Phase 1 Foundation Complete (2025-09-09)

### Added
- **Result Pattern Error Handling** [F2]
  - Comprehensive error handling using `result<T>` and `result_void` from thread_system
  - New `error_codes.h` with detailed `logger_error_code` enum covering 20+ error conditions
  - All APIs now return result types for proper error propagation
  - Helper functions (`make_error`) for creating contextual error results
  
- **Configuration Validation Framework** [F4]
  - `logger_config.h` with comprehensive validation logic
  - Validates 15+ configuration parameters (buffer size, batch size, queue settings, etc.)
  - Predefined configuration templates:
    - `production`: Metrics enabled, crash handler, no color output
    - `debug`: Synchronous mode, immediate output, trace level
    - `high_performance`: 64KB buffer, lock-free queue, compression
    - `low_latency`: 10ms flush interval, small batches
  - All configurations validated before logger instantiation
  
- **Builder Pattern Implementation** [F4]
  - `logger_builder.h` with fluent interface for logger construction
  - Automatic configuration validation before build
  - Template support via `use_template()` method
  - Method chaining for intuitive API usage
  - Validates writer count against configuration limits
  
- **Interface Segregation (SOLID)** [F3]
  - Clean separation following Interface Segregation Principle:
    - `log_writer_interface`: Core writer contract with write/flush operations
    - `log_filter_interface`: Filter abstraction for log filtering
    - `log_formatter_interface`: Formatting contract for output formatting
    - `log_sink_interface`: Output sink abstraction
  - Unified `log_entry` structure for consistent data passing
  - `base_formatter` with three implementations (plain, json, compact)
  - Backward compatibility maintained with existing APIs

- **Comprehensive Test Coverage**
  - 18 new configuration validation tests
  - Builder pattern tests with valid/invalid scenarios
  - Template configuration tests
  - Interface implementation tests
  - All tests passing (100% success rate)

### Changed
- **Thread System Integration** [F1]
  - Replaced local logger_interface with thread_system headers
  - Added conditional compilation (`USE_THREAD_SYSTEM` / `LOGGER_STANDALONE`)
  - CMake automatically detects sibling thread_system project
  - Seamless integration when thread_system is available
  
- **API Improvements**
  - All log operations return `result_void` for error handling
  - Writer operations return results for error propagation
  - Filter/formatter interfaces use const-correct signatures
  - Updated all 10+ implementation files for new signatures
  - Maintained backward compatibility with wrapper methods

### Fixed
- Compilation errors with thread_system result type access
- Error code mapping between logger and thread domains (10000 offset)
- CMake dependency resolution for sibling projects
- Result type method access (`error()` → `get_error()`)

### Security
- Added sanitization interface groundwork for future security features

## [1.0.0] - 2025-01-12

### Added
- **Core Features**
  - High-performance logger implementation
  - Lock-free asynchronous logging mode
  - Synchronous logging mode for immediate output
  - Six log levels: trace, debug, info, warning, error, critical
  - Log level filtering
  - Source location tracking (file, line, function)

- **Writers**
  - Console writer with automatic color detection
  - Base writer class for custom implementations
  - Thread-safe writer management
  - Multiple writer support

- **Integration**
  - Thread System logger_interface implementation
  - Service container integration
  - Direct usage as standalone library

- **Performance**
  - Configurable buffer sizes
  - Batch processing in async mode
  - Minimal allocation strategy
  - Lock-free enqueue operations (planned)

- **Build System**
  - CMake 3.16+ support
  - FetchContent integration
  - Package installation support
  - Cross-platform compatibility (Linux, macOS, Windows)

- **Documentation**
  - Getting started guide
  - Architecture overview
  - API reference
  - Performance guide
  - Custom writer tutorial
  - Contributing guidelines

- **Testing**
  - Unit test framework
  - Benchmark framework
  - CI/CD with GitHub Actions

### Known Issues
- File writer not yet implemented (example provided in docs)
- Lock-free queue uses mutex (true lock-free planned)
- No log rotation built-in

## Future Releases

### [1.1.0] - Planned
- File writer with rotation support
- True lock-free queue implementation
- Structured logging (JSON format)
- Network writer
- Syslog writer

### [1.2.0] - Planned
- Log aggregation features
- Sampling for high-frequency logs
- Runtime configuration
- Hot-reload of configuration

### [2.0.0] - Future
- C++23 features
- Compile-time format checking
- Zero-allocation mode
- Plugin system for writers

---

[Unreleased]: https://github.com/kcenon/logger_system/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/kcenon/logger_system/releases/tag/v1.0.0