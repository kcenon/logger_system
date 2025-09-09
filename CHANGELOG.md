# Changelog

All notable changes to the Logger System will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased] - Phase 1 Foundation Complete (2025-09-09)

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