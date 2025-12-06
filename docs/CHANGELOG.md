# Changelog - Logger System

> **Language:** **English** | [한국어](CHANGELOG_KO.md)

All notable changes to the Logger System project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

### Fix broken example files after #226 API changes (Issue #228) - 2025-12-06

#### Removed
- **migration_example.cpp**: Removed example file demonstrating non-existent v1 API
  - Referenced `logger_module::v1_compat` namespace that was never implemented
  - Used undefined functions: `create_logger()`, `create_file_logger()`, `add_file_writer()`
  - Used undefined macros: `LOG_INFO`, `LOG_DEBUG`, `LOG_ERROR`, `LOG_WARNING`
  - Used undefined types: `logger_config_v1`

#### Changed
- **advanced_features_demo.cpp**: Comprehensive rewrite to work with current API
  - Removed `thread_module` namespace (not available in standalone mode)
  - Added `filters::` namespace for filter classes
  - Removed routing code (get_router() not implemented)
  - Added `<filesystem>` for directory creation
  - Improved output messages for clarity

#### Disabled (awaiting feature implementation)
- **distributed_logging_demo.cpp**: Commented out in CMakeLists.txt
  - Requires unimplemented features: `log_server`, `log_aggregator`, `log_analyzer::time_window_stats`
- **security_demo.cpp**: Commented out in CMakeLists.txt
  - Requires unimplemented features: `log_sanitizer`, `structured_logger`, `get_router()`

#### Related Issues
- Fixes #228: fix(examples): broken example files after #226 API changes

---

### Standalone async implementation (Issue #222) - 2025-12-06

#### Changed
- **thread_system is now OPTIONAL**: Logger system can build and run standalone
  - CMake option `LOGGER_USE_THREAD_SYSTEM` (default: OFF) controls integration
  - When disabled, uses C++20 std::jthread for all async operations
  - Full thread_system integration still available when enabled

- **C++20 std::jthread migration**: All internal threading uses std::jthread
  - `log_collector.cpp`: Uses `log_collector_jthread_worker` with stop_token
  - `network_writer.cpp`: Uses `network_send_jthread_worker` and `network_reconnect_jthread_worker`
  - `batch_processor.cpp`: Uses `batch_processing_jthread_worker`
  - `async_worker.h/cpp`: New standalone async worker implementation

#### Added
- **async_worker class**: New standalone async worker using std::jthread
  - Located in `src/impl/async/async_worker.h` and `async_worker.cpp`
  - Features cooperative cancellation via std::stop_token
  - Lock-free enqueue path with overflow detection
  - Configurable queue size and dropped task counting

#### Benefits
- **Resolves circular dependency**: thread_system can now use logger_system for logging
- **Standalone operation**: logger_system works without external threading dependencies
- **Better shutdown**: std::jthread provides cleaner RAII-based thread management
- **Cooperative cancellation**: No more polling/busy-waiting in shutdown path

#### Migration Guide
- **Default behavior**: logger_system now builds standalone (no thread_system)
- **To enable thread_system integration**:
  ```cmake
  cmake -DLOGGER_USE_THREAD_SYSTEM=ON ...
  ```
- Existing functionality preserved when integration is enabled

#### Related Issues
- Closes #222: Implement async_worker class using std::jthread
- Prerequisite for #223, #224, #225

---

### CMake fmt Fallback Removal - 2025-12-03

#### Changed
- **CMake configuration now requires std::format**: Removed all fmt library fallback logic
  - `cmake/CompilerChecks.cmake`: Now fails with FATAL_ERROR if std::format is unavailable
  - `cmake/LoggerCompatibility.cmake`: Removed fmt detection, C++17 mode, and vcpkg fallback

#### Removed
- **fmt library fallback in CMake**:
  - `check_std_format()`: Removed find_package(fmt) and vcpkg fallback logic
  - `configure_logger_compatibility()`: Removed C++17 compatibility mode entirely
  - `setup_formatting_library()`: Removed fmt::format and basic formatting branches
  - `setup_vcpkg_fmt_fallback()`: Function completely removed

#### Benefits
- **Cleaner CMake configuration**: Simplified feature detection without fallback complexity
- **Consistent build behavior**: Same behavior across all platforms and CI environments
- **Reduced dependencies**: No conditional fmt library linking
- **Cross-project alignment**: Matches thread_system, container_system, network_system, database_system

#### Related Issues
- Part of cross-project standardization effort
- See: thread_system#219, container_system#168, network_system#257, database_system#203

---

### thread_system Required Dependency - 2025-12-03

#### Changed
- **thread_system is now a required dependency**: Removed standalone mode
  - CMake configuration fails with FATAL_ERROR if thread_system is not found
  - All internal threading now uses thread_system's thread_base class
  - Simplified codebase by removing conditional compilation paths

- **Threading Migration**: Internal threading migrated to thread_system's thread_base
  - `log_collector.cpp`: Uses `log_collector_worker` class extending thread_base
  - `network_writer.cpp`: Uses `network_send_worker` and `network_reconnect_worker`
  - `batch_processor.cpp`: Uses `batch_processing_worker`

#### Removed
- **LOGGER_STANDALONE_MODE**: CMake option removed
- **LOGGER_STANDALONE**: Compile definition removed
- **--standalone** and **--with-thread**: Build script options removed
- **Fallback implementations**: Standalone mode fallbacks in headers removed

#### Breaking Changes
- ⚠️ **thread_system is now required**: Projects must have thread_system available
- Build will fail if thread_system is not found (no more standalone fallback)
- `--standalone` option in build.sh no longer works

#### Benefits
- **Simpler codebase**: ~90% reduction in conditional compilation
- **Consistent threading**: All threading uses thread_base for better lifecycle management
- **Easier maintenance**: Single code path instead of standalone + integrated modes
- **Better integration**: Full thread_system features always available

---

## Security Improvements

### Removed
- **encrypted_writer**: Removed insecure XOR-based encryption implementation
  - Deleted `include/kcenon/logger/writers/encrypted_writer.h`
  - Deleted `src/impl/writers/encrypted_writer.cpp`
  - Removed from integration tests
  - Updated documentation to recommend vetted crypto libraries

### Changed
- **Documentation**: Updated security guidance
  - SECURITY.md now recommends OpenSSL, libsodium, or Botan for encryption
  - README.md provides guidance for implementing custom encrypted writers
  - Added best practices for production log encryption

### Migration Guide
- If using `encrypted_writer`, implement a custom writer with a production-grade crypto library
- See SECURITY.md for implementation examples using AES-GCM or ChaCha20-Poly1305
- Use proper key management (HSM, KMS, or OS keychain)

---

## C++20 and std::format Migration - 2025-11-03

### Changed
- **C++20 now REQUIRED**: Enforced C++20 as minimum standard (no fallback to C++17)
  - Added CMake check to fail build if C++20 is not available
  - `CMAKE_CXX_STANDARD_REQUIRED ON` prevents fallback
  - Clear error message when C++20 requirement is not met

- **Removed fmt library dependency**: Migrated to std::format
  - Removed fmt from vcpkg.json dependencies
  - Removed fmt find_package from CMakeLists.txt
  - Updated common_logger_adapter.h to use std::format
  - All fmt::format calls replaced with std::format
  - Cleaner dependency tree (one less external dependency)

### Removed
- **fmt library**: No longer required or used
  - Removed from CMakeLists.txt (line 312-317)
  - Removed from vcpkg.json
  - Removed USE_FMT compile definition

### Benefits
- **Simpler dependencies**: Using only C++ standard library features
- **Better portability**: No external formatting library needed
- **Consistent with common_system**: Both now use std::format exclusively
- **Build performance**: Faster compilation without fmt dependency
- **Forward compatibility**: std::format is the C++ standard approach

### Breaking Changes
- ⚠️ **C++20 compiler required**: Projects using older compilers must upgrade
  - GCC 10+ (full std::format support in GCC 13+)
  - Clang 14+ (full std::format support in Clang 15+)
  - MSVC 19.29+ (Visual Studio 2019 16.10+)
  - Apple Clang 14.0+ (Xcode 14+)

---

## Phase 3: Code Quality - 2025-11-03

### Added
- **Common Utility Functions (Phase 3.4)**: Extracted shared utilities to reduce code duplication
  - `time_utils.h`: Timestamp formatting utilities
    - `format_timestamp()`: Human-readable format (YYYY-MM-DD HH:MM:SS.mmm)
    - `format_iso8601()`: ISO 8601 / RFC 3339 format with UTC timezone
    - `format_compact()`: Compact format for filenames (YYYYMMDDHHMMSSmmm)
    - `format_for_rotation()`: Format for rotating file writers (YYYYMMDD or YYYYMMDD_HH)
  - `string_utils.h`: String conversion and escaping utilities
    - `level_to_string()`: Convert log level to human-readable string
    - `level_to_color()`: Convert log level to ANSI color code
    - `escape_json()`: JSON string escaping
    - `escape_xml()`: XML string escaping
    - `extract_filename()`: Extract filename from full path
    - `trim()`, `to_lower()`, `to_upper()`, `replace_all()`: String manipulation helpers
  - `file_utils.h`: File path validation and sanitization utilities (security-focused)
    - `validate_log_path()`: Path traversal attack prevention
    - `sanitize_filename()`: Remove dangerous characters from filenames
    - `set_file_permissions()`: Unix/POSIX permission management
    - `is_writable()`, `get_file_size()`: File system utilities
    - `generate_temp_filename()`: Safe temporary file naming
  - Located in `include/kcenon/logger/utils/`

- **Error Handling Utilities (Phase 3.3)**: Unified error handling across all writers
  - `error_handling_utils.h`: Comprehensive error handling helper functions
  - `try_write_operation()`: Generic error handler for write operations with specialized exception handling
  - `try_open_operation()`: Specialized handler for file open operations
  - `try_network_operation()`: Specialized handler for network operations
  - `try_encryption_operation()`: Specialized handler for encryption operations
  - `check_condition()`: Helper for precondition validation
  - `check_stream_state()`: Helper for stream state verification
  - `check_file_exists()`: Helper for file existence validation
  - `ensure_directory_exists()`: Helper for directory creation with error handling
  - Located in `include/kcenon/logger/utils/error_handling_utils.h`

- **Integration Backend Interface (Phase 3.2)**: Runtime polymorphism replaces conditional compilation
  - `integration_backend`: Abstract interface for external system integration
  - `standalone_backend`: Default backend for independent logger operation
  - `thread_system_backend`: Backend for thread_system integration
  - Automatic backend detection based on compile-time flags
  - Explicit backend selection via `logger_builder::with_*_backend()` methods
  - Located in `include/kcenon/logger/backends/`

- **Formatter Interface (Strategy Pattern)**: Implemented formatter interface to eliminate code duplication
  - `log_formatter_interface`: Abstract interface for log formatters with configurable options
  - `format_options`: Struct for controlling timestamp, thread ID, source location, colors, and pretty-printing
  - `formatter_factory`: Type alias for dependency injection support
  - Located in `include/kcenon/logger/interfaces/log_formatter_interface.h`

- **timestamp_formatter**: Default human-readable formatter
  - Millisecond-precision timestamps in YYYY-MM-DD HH:MM:SS.mmm format
  - Color-coded log levels (ANSI escape codes)
  - Thread ID tracking
  - Source location information (file, line, function)
  - Automatic filename extraction from paths
  - Located in `include/kcenon/logger/formatters/timestamp_formatter.h`

- **json_formatter**: JSON-structured formatter for log aggregation systems
  - ISO 8601 / RFC 3339 timestamp format (UTC)
  - Proper JSON escaping of special characters
  - Optional pretty-printing for readability
  - Structured source location information
  - Compatible with ELK, Splunk, CloudWatch, and other log aggregation tools
  - Located in `include/kcenon/logger/formatters/json_formatter.h`

### Changed
- **logger**: Integrated with backend interface for level conversion
  - Added `backend_` parameter to constructor (optional, auto-detects if not provided)
  - Replaced conditional `convert_log_level()` with `backend_->normalize_level()`
  - Backend initialization and shutdown handled automatically
  - Log level conversion now performed at runtime via backends
  - Located in `src/core/logger.cpp`

- **logger_builder**: Added backend selection methods
  - `with_backend()`: Set custom integration backend
  - `with_thread_system_backend()`: Explicitly use thread_system backend
  - `with_standalone_backend()`: Explicitly use standalone backend
  - Auto-detection in `build()` method if no backend specified
  - Located in `include/kcenon/logger/core/logger_builder.h`

- **base_writer**: Integrated formatter interface to eliminate code duplication
  - Added `formatter_` member for Strategy pattern implementation
  - Constructor now accepts optional `log_formatter_interface` (defaults to `timestamp_formatter`)
  - New `set_formatter()` method for runtime formatter switching
  - New `format_log_entry(const log_entry&)` method using formatters
  - Legacy formatting methods (`format_log_entry`, `level_to_string`, `level_to_color`) marked as deprecated
  - Backward compatible with existing code

- **Formatting logic moved to formatters**: Eliminated duplication across all writers
  - Removed formatting logic from individual writer implementations
  - All writers now delegate formatting to the configured formatter
  - Simplifies adding new writer types
  - Enables runtime format switching (text → JSON)

- **All writers updated with unified error handling (Phase 3.3)**: Consistent error handling across codebase
  - `file_writer.cpp`: Replaced try-catch patterns with `try_write_operation` and `try_open_operation`
  - `rotating_file_writer.cpp`: Applied error handling utilities to write, rotation, and cleanup operations
  - `encrypted_writer.cpp`: Integrated `try_encryption_operation` for encryption workflows
  - `console_writer.cpp`: Added stream state validation with `check_stream_state`
  - `critical_writer.cpp`: Enhanced WAL operations and signal handler safety with error utilities
  - All exception types properly categorized (filesystem_error, ios_base::failure, system_error, bad_alloc)
  - Consistent error code mapping across all writers

- **Formatters refactored to use common utilities (Phase 3.4)**: Eliminated code duplication in formatters
  - `timestamp_formatter.h`: Now uses `utils::time_utils` and `utils::string_utils`
  - `json_formatter.h`: Now uses `utils::time_utils` and `utils::string_utils`
  - Removed duplicate private functions from both formatters
  - Consistent behavior across all formatting operations
  - Reduced formatter code size by ~150 lines

### Deprecated
- **Legacy base_writer formatting methods**: Marked for future removal
  - `format_log_entry(level, message, file, line, function, timestamp)` - Use formatter instead
  - `level_to_string(log_level)` - Formatting now handled by formatters
  - `level_to_color(log_level)` - Formatting now handled by formatters
  - Methods remain functional for backward compatibility

### Code Quality
- **90% reduction in conditional compilation**: Runtime polymorphism replaces #ifdef directives
  - Test combinations reduced from 16 to 4 (75% reduction)
  - Improved code readability without #ifdef blocks
  - Runtime backend switching capability
  - Easier to add new integration backends

- **50% reduction in code duplication**: Formatting logic centralized in formatters
- **Unified error handling**: Eliminated try-catch duplication across all writers
  - Consistent exception handling with specialized error handlers
  - Better error message consistency across all writers
  - Reduced boilerplate code in write operations
  - Improved error code accuracy with proper exception type detection
- **Common utility functions**: Extracted shared code to reusable utilities
  - ~150 lines of duplicate code removed from formatters
  - Thread-safe, stateless utility functions
  - Security-focused file utilities ready for Phase 4
  - Consistent timestamp formatting across all components
- **Improved maintainability**: Single source of truth for formatting, error handling, and common utilities
- **Enhanced extensibility**: Easy to add new formatters (XML, YAML, custom formats) and backends
- **Better testability**: Formatters, backends, error handlers, and utilities can be tested independently

### Benefits
- **Simplified Testing**: Fewer conditional compilation paths to test
- **Runtime Flexibility**: Can switch backends at runtime for testing
- **Cleaner Codebase**: Less #ifdef clutter, more readable code
- **Easy Extension**: Adding new integration backends requires no conditional compilation

### Backward Compatibility
- All existing code continues to work without modifications
- Default formatter (timestamp_formatter) maintains original formatting behavior
- Legacy API methods remain functional with deprecation warnings
- All tests passing: 3/3 integration tests

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

- **Benchmark Infrastructure**: Complete CI/CD integration for performance regression detection
  - `compare_benchmarks.py`: Python script for automated benchmark comparison
  - Detects performance regressions with configurable threshold (default: 5%)
  - Generates markdown reports showing improvements and regressions
  - Integrated into GitHub Actions workflow
  - Baseline results stored in `benchmarks/baselines/`
  - Automatic comparison on every pull request
  - Located in `scripts/compare_benchmarks.py`

### Changed
- **rotating_file_writer**: Periodic rotation checks for improved performance
  - Added `check_interval` parameter (default: 100 writes)
  - Reduces filesystem system calls from every write to every 100 writes
  - **Expected performance**: 10-20% improvement in throughput
  - Maintains rotation accuracy with configurable interval
  - All constructors now accept optional `check_interval` parameter
  - Backward compatible with default value

- **Benchmark Workflow**: Enhanced GitHub Actions benchmarks.yml
  - Benchmarks now enabled (BUILD_BENCHMARKS=ON, previously disabled)
  - Automated benchmark execution on push and PR
  - Results uploaded as artifacts (90-day retention)
  - Baseline comparison with regression detection using compare_benchmarks.py
  - Markdown reports in GitHub Step Summary
  - Manual baseline saving via workflow_dispatch
  - Performance threshold: 5% regression detection
  - Located in `.github/workflows/benchmarks.yml`

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
