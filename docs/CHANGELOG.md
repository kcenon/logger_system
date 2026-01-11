# Changelog - Logger System

> **Language:** **English** | [한국어](CHANGELOG_KO.md)

All notable changes to the Logger System project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [3.0.0] - 2025-12-31

### Removed - Deprecated API Cleanup (Issue #268)

This release removes deprecated API compatibility layers following the "Fewest Elements" Simple Design principle.

#### Breaking Changes

- **Removed `logger_interface.h` header** (`include/kcenon/logger/interfaces/logger_interface.h`)
  - `logger_system::logger_interface` class removed - use `common::interfaces::ILogger` instead
  - `logger_system::logger_registry` class removed
  - `THREAD_LOG_*` macros removed - use `LOG_*` macros from common_system instead

- **Removed `with_thread_system_backend()` method** from `logger_builder`
  - Use `with_standalone_backend()` or `with_backend()` instead

- **Removed deprecated `result_void` class** from `error_codes.h`
  - Use `common::VoidResult` directly instead

- **Removed deprecated `make_logger_error` functions**
  - Use `result<T>{error_code, message}` construction directly instead

#### Migration Guide

```cpp
// Before (deprecated):
#include <kcenon/logger/interfaces/logger_interface.h>
auto logger = logger_builder().with_thread_system_backend().build();
result_void res = make_logger_error(code, "message");

// After:
#include <kcenon/common/interfaces/logger_interface.h>
auto logger = logger_builder().with_standalone_backend().build();
common::VoidResult res = make_logger_void_result(code, "message");
```

---

## [Unreleased]

### Real-time Log Analysis Integration (Issue #281)

#### Added
- **New `realtime_log_analyzer` class** for real-time anomaly detection during logging
  - Error spike detection with configurable threshold
  - Pattern-based alerting with regex support
  - Rate anomaly detection (high/low rate alerts)
  - New error type tracking and classification
  - Thread-safe sliding window for rate calculations
  - Anomaly callback for immediate notifications

- **Logger integration**
  - `logger::set_realtime_analyzer()` - Set analyzer for real-time monitoring
  - `logger::get_realtime_analyzer()` - Access the configured analyzer
  - `logger::has_realtime_analysis()` - Check if analysis is enabled

- **Builder methods**
  - `with_realtime_analyzer()` - Set pre-configured analyzer
  - `with_realtime_analysis(config, callback)` - Configure with settings
  - `with_realtime_analysis(threshold, callback)` - Enable with defaults

- **Factory methods**
  - `realtime_analyzer_factory::create_basic()` - Default analyzer
  - `realtime_analyzer_factory::create(config)` - Custom configuration
  - `realtime_analyzer_factory::create_production()` - Production-ready defaults

#### Example
```cpp
auto logger = logger_builder()
    .with_realtime_analysis(50, [](const anomaly_event& event) {
        if (event.anomaly_type == anomaly_event::type::error_spike) {
            alert_ops_team(event);
        }
    })
    .build();
```

---

### Log Sampling for High-Volume Scenarios (Issue #282)

#### Added
- **New `log_sampler` class** for reducing log volume in high-throughput scenarios
  - Random sampling with configurable rate (0.0 to 1.0)
  - Rate limiting (max N logs per second)
  - Adaptive sampling that auto-adjusts based on load
  - Hash-based deterministic sampling for debugging
  - Level-based bypass for critical messages (error, fatal always logged)
  - Per-category sampling rates

- **Sampling configuration**
  - `sampling_config` struct with factory methods for common use cases
  - `sampling_stats` for monitoring sampled/dropped message counts
  - Thread-safe counters for accurate metrics

- **Logger integration**
  - `logger::set_sampler()` - Set sampler for volume reduction
  - `logger::get_sampler()` - Access the configured sampler
  - `logger::has_sampling()` - Check if sampling is enabled
  - `logger::get_sampling_stats()` - Get sampling metrics
  - `logger::reset_sampling_stats()` - Reset metrics

- **Builder methods**
  - `with_sampler()` - Set pre-configured sampler
  - `with_sampling(config)` - Configure with settings
  - `with_random_sampling(rate)` - Random sampling shortcut
  - `with_rate_limiting(max_per_second)` - Rate limiting shortcut
  - `with_adaptive_sampling(threshold, min_rate)` - Adaptive sampling

- **Factory methods**
  - `sampler_factory::create_disabled()` - Pass-through sampler
  - `sampler_factory::create_random(rate)` - Random sampler
  - `sampler_factory::create_rate_limited(max)` - Rate limiter
  - `sampler_factory::create_adaptive(threshold, min)` - Adaptive sampler
  - `sampler_factory::create_production(rate, critical_levels)` - Production defaults

#### Example
```cpp
// Random 10% sampling with error/fatal bypass
auto logger = logger_builder()
    .with_random_sampling(0.1)
    .build();

// Rate limiting to 1000 logs/sec
auto logger = logger_builder()
    .with_rate_limiting(1000)
    .build();

// Adaptive sampling under high load
auto logger = logger_builder()
    .with_adaptive_sampling(50000, 0.01)  // Adapt when >50k/sec
    .build();

// Custom per-category rates
sampling_config config;
config.enabled = true;
config.rate = 0.1;
config.category_rates["database"] = 0.01;  // Only 1% of DB logs
config.category_rates["security"] = 1.0;   // All security logs
auto logger = logger_builder()
    .with_sampling(config)
    .build();
```

---

### Phase 3.1: Structured Logging API Design (Issue #308) - 2026-01-11

#### Added
- **Logfmt formatter** (`logfmt_formatter.h`) for structured log output
  - Key-value pair output format (Heroku/12-Factor style)
  - ISO 8601 timestamps
  - Full structured fields support
  - OpenTelemetry context (trace_id, span_id) output
  - Compatible with Prometheus, Grafana Loki, and logfmt parsers

- **OpenTelemetry context output in JSON formatter**
  - `trace_id`, `span_id`, `trace_flags` fields now included in JSON output
  - Enables correlation with distributed tracing systems

- **Correlation ID convenience API**
  - `logger::set_correlation_id()` - Set correlation ID for request tracking
  - `logger::get_correlation_id()` - Get current correlation ID
  - `logger::clear_correlation_id()` - Clear correlation ID
  - `logger::has_correlation_id()` - Check if correlation ID is set
  - `logger::set_request_id()` / `get_request_id()` / `clear_request_id()` / `has_request_id()` - Alias for correlation ID

#### Example
```cpp
// Using correlation ID for request tracking
logger->set_correlation_id("req-abc-123");

logger->info_structured()
    .message("Processing request")
    .field("user_id", 12345)
    .emit();
// Output includes: "correlation_id":"req-abc-123"

logger->clear_correlation_id();

// Using logfmt formatter
auto formatter = std::make_unique<logfmt_formatter>();
auto writer = std::make_unique<file_writer>("app.log", std::move(formatter));
// Output: level=info ts=2026-01-11T10:30:15.123Z msg="Processing request" user_id=12345
```

---

### Coverage Build Fix (PR #291) - 2026-01-08

#### Fixed
- **Coverage linker errors**: Fixed gcov undefined reference errors in CI coverage builds
  - Added explicit `gcov` library link for GCC when using static libraries with coverage instrumentation
  - Added `logger_structured_logging_test` to coverage target registration list
  - Fixes `undefined reference to '__gcov_init'`, `__gcov_exit'`, `__gcov_merge_add'` errors

- **Compiler warnings**: Fixed unused variable warnings in `executor_integration.cpp`
  - Replaced unused `auto* standalone` variable with direct `dynamic_cast` null check
  - Eliminates `-Wunused-variable` warnings in `enable()` and `set_executor()` methods

---

### CI Workflow Fix (PR #290) - 2026-01-08

#### Fixed
- **CI linker errors**: Fixed undefined reference errors for `EnhancedThreadPoolMetrics`
  - Changed `thread_system` checkout from `feature/adopt-kcenon-feature-flags` to `main` branch
  - The feature branch lacked `EnhancedThreadPoolMetrics` implementation that was added to main
  - Removed `LOGGER_USE_THREAD_SYSTEM=ON` from coverage job until thread_system fixes its CMake
  - Affected workflows: `integration-tests.yml`, `ci.yml`, `benchmarks.yml`, `sanitizers.yml`

- **macOS Debug test timeout**: Fixed `MultipleStartStopCycles` test timing out on macOS Debug builds
  - Replaced `CreateLoggerWithFileWriter()` with `CreateLogger()` to avoid redundant `start()` call
  - Used simple `flush()` instead of `WaitForFlush()` to eliminate unnecessary stop/start cycles
  - Test now completes in milliseconds instead of 20+ minutes timeout

---

### OpenTelemetry Integration Fix (Issue #283) - 2026-01-08

#### Fixed
- **macOS compatibility**: Replaced `std::jthread` with `std::thread` in `otlp_writer`
  - `std::jthread` and `std::stop_token` are not supported in Apple Clang's libc++
  - Uses existing `running_` atomic flag for stop mechanism
  - Maintains same behavior and thread-safety guarantees

- **Windows build fix**: Removed `write`/`fsync` macro pollution from `signal_manager.h`
  - The `#define write _write` macro was leaking to other headers
  - Caused `batch_writer.h` and `otlp_writer.h` `write()` method to be renamed to `_write()`
  - Fixed by replacing macros with `detail::safe_write()` and `detail::safe_fsync()` inline functions
  - These wrapper functions handle Windows POSIX compatibility internally

---

### C++20 Module Files (Issue #275) - 2026-01-03

#### Added
- **C++20 module files**: Created module interface for `kcenon.logger`
  - `src/modules/logger.cppm`: Primary module interface
  - `src/modules/core.cppm`: Core partition (Logger, LogContext, LogCollector)
  - `src/modules/backends.cppm`: Backends partition (integration_backend, standalone_backend)
  - `src/modules/analysis.cppm`: Analysis partition (log_analyzer, analysis_stats)

- **CMake module support**: Added optional C++20 modules support
  - Requires CMake 3.28+ for module scanning
  - New option `LOGGER_USE_MODULES` to enable module build
  - `LOGGER_HAS_MODULES` compile definition when modules are active
  - Header-based build still works during transition period

#### Module Structure
```cpp
export module kcenon.logger;

import kcenon.common;           // Required dependency

export import :core;            // Logger, LogContext, LogCollector
export import :backends;        // Integration backends
export import :analysis;        // Log analysis utilities
```

#### CMake Usage
```cmake
cmake -DLOGGER_USE_MODULES=ON ..  # Requires CMake 3.28+
```

---

### Fix Duplicate Writer Add Logic (Issue #270) - 2025-12-31

#### Fixed
- **Named writer storage**: `add_writer(name, writer)` now properly stores writers by name
  - Previously the `name` parameter was ignored
  - Writers can now be retrieved via `get_writer(name)` and removed via `remove_writer(name)`

#### Changed
- **Unified return types**: `add_writer(name, writer)` now returns `common::VoidResult` instead of `void`
  - Consistent with unnamed `add_writer(writer)` overload
  - Proper error handling for null writers and uninitialized logger

#### Added
- **Named writer implementation**: Full implementation of named writer storage
  - `add_writer(name, writer)` stores writer in both general list and named map
  - `remove_writer(name)` removes writer from both storage locations
  - `get_writer(name)` retrieves writer pointer by name
  - `clear_writers()` now also clears named writer storage

---

### Use KCENON Feature Detection (Issue #250) - 2025-12-22

#### Changed
- **error_handling_utils.h**: Use unified KCENON_HAS_SOURCE_LOCATION from common_system
  - Include `<kcenon/common/config/feature_flags.h>` for feature detection
  - Replace custom source_location detection with KCENON_HAS_SOURCE_LOCATION
  - Keep LOGGER_HAS_SOURCE_LOCATION as legacy alias for backward compatibility

- **jthread_compat.h**: Use unified KCENON_HAS_JTHREAD from common_system
  - Include `<kcenon/common/config/feature_flags.h>` for feature detection
  - Replace custom jthread detection with KCENON_HAS_JTHREAD
  - Keep LOGGER_HAS_JTHREAD as legacy alias for backward compatibility

#### Fixed
- **logger.h**: Fixed build error with common_system v3.0.0 compatibility
  - Removed `override` keyword from deprecated `log(level, message, file, line, function)` method
  - This method was removed from `common::interfaces::ILogger` in v3.0.0 (Issue #217)
  - Method preserved for backward compatibility but no longer overrides the interface

- **Windows MSVC LNK2019**: Fixed unresolved external symbol for `thread_pool::is_running()`
  - Root cause: `KCENON_HAS_COMMON_EXECUTOR` was not defined when thread_system built as submodule
  - Fix: Updated thread_system `core/CMakeLists.txt` to define KCENON_HAS_COMMON_EXECUTOR=1 when executor_interface.h is found
  - Added `UNIFIED_USE_LOCAL=ON` to CI workflows to use locally checked out dependencies

- **ilogger_interface_test.cpp**: Updated test for common_system v3.0.0 API changes
  - Test now calls deprecated method directly on logger class instead of through ILogger pointer

#### Migration Guide
The LOGGER_HAS_* macros are now aliases to KCENON_HAS_* from common_system.
For new code, use KCENON_HAS_* directly:
```cpp
// Legacy (still works)
#if LOGGER_HAS_JTHREAD
    std::jthread worker(...);
#endif

// Recommended
#include <kcenon/common/config/feature_flags.h>
#if KCENON_HAS_JTHREAD
    std::jthread worker(...);
#endif
```

---

### Migrate from Deprecated common_system APIs (Issue #248) - 2025-12-22

#### Changed
- **common_system_adapter.h**: Added source_location-based logging API
  - New `log(log_level, std::string_view, const source_location&)` method
  - Deprecated `log(log_level, string, file, line, function)` now delegates to new API
  - Suppressed deprecation warnings for backward compatibility code

- **logger_interface.h**: Updated THREAD_LOG_* macro handling
  - When common_system is available, THREAD_LOG_* macros now redirect to LOG_*
  - Removed redundant macro redefinitions that caused compiler warnings
  - Standalone fallback preserved for builds without common_system

#### Migration Guide
For new code, use the common_system's LOG_* macros directly:
```cpp
// Deprecated (will be removed in v3.0.0)
THREAD_LOG_INFO("message");

// Recommended
#include <kcenon/common/logging/log_macros.h>
LOG_INFO("message");
```

For implementations of ILogger interface, override the new source_location-based method:
```cpp
// Deprecated (will be removed in v3.0.0)
VoidResult log(log_level level, const std::string& msg,
               const std::string& file, int line, const std::string& func) override;

// Recommended
VoidResult log(log_level level, std::string_view msg,
               const source_location& loc = source_location::current()) override;
```

---

### thread_system v3.0 Compatibility (Issue #244) - 2025-12-19

#### Breaking Changes
- **logger_adapter.h**: Removed `shared::ILogger` and `shared::IService` implementation
  - The `USE_THREAD_SYSTEM` conditional compilation branch has been removed
  - For thread_system integration, use `common_logger_adapter.h` which provides
    adapters for `kcenon::common::interfaces::ILogger`

#### Changed
- **thread_integration_detector.h**: Updated detection header
  - Now uses `<kcenon/thread/thread_pool.h>` instead of removed `<kcenon/thread/interfaces/logger_interface.h>`
  - thread_system v3.0 uses common_system interfaces instead of thread-specific interfaces

#### Migration Guide
For thread_system integration, replace:
```cpp
// Old (v2.x)
#include <kcenon/logger/adapters/logger_adapter.h>
// using shared::ILogger and shared::IService

// New (v3.0+)
#include <kcenon/logger/adapters/common_logger_adapter.h>
// using kcenon::common::interfaces::ILogger
```

---

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

### common::interfaces::ILogger Implementation - 2025-12-06

#### Added
- **ILogger interface implementation (Issue #223)**: Logger now implements `common::interfaces::ILogger`
  - Full compatibility with standardized logging interface from common_system
  - Seamless integration with thread_system and other kcenon ecosystem components
  - Type-safe log level conversion between `logger_system::log_level` and `common::interfaces::log_level`

- **New level_converter.h**: Utility header for converting between log level types
  - `to_logger_system_level()`: Convert from common to native log level
  - `to_common_level()`: Convert from native to common log level
  - Constexpr functions for zero-overhead conversion

- **ILogger interface methods**:
  - `log(log_level, const std::string&)` → Returns `VoidResult`
  - `log(log_level, std::string_view, const source_location&)` → Returns `VoidResult`
  - `log(log_level, message, file, line, function)` → Returns `VoidResult`
  - `log(const log_entry&)` → Structured logging support
  - `is_enabled(log_level)` → Check if level is enabled
  - `set_level(log_level)` → Set minimum log level, returns `VoidResult`
  - `get_level()` → Get current minimum level as `common::interfaces::log_level`
  - `flush()` → Flush buffers, returns `VoidResult`

- **ILogger interface compatibility tests**: New test suite in `tests/unit/ilogger_interface_test.cpp`
  - 13 comprehensive tests for interface compatibility
  - Tests for polymorphic usage, level conversion, and backward compatibility

#### Changed
- **logger class now inherits from both**:
  - `security::critical_logger_interface` (existing)
  - `common::interfaces::ILogger` (new - standardized interface)

- **Return types updated for ILogger compliance**:
  - `log()` methods now return `common::VoidResult` instead of `void`
  - `flush()` now returns `common::VoidResult` instead of `void`
  - `set_level()` now returns `common::VoidResult` instead of `void`

#### Deprecated
- **logger_types.h**: Deprecated in favor of `kcenon/common/interfaces/logger_interface.h`
  - `logger_system::log_level` marked as deprecated, use `common::interfaces::log_level`
  - Will be removed in v3.0.0

- **logger_interface.h** (logger_system namespace): Deprecated in favor of common_system's ILogger
  - `logger_system::logger_interface` marked as deprecated, use `common::interfaces::ILogger`
  - Will be removed in v3.0.0

#### Backward Compatibility
- **All existing APIs preserved**: Native `logger_system::log_level` overloads still work
  - `log(log_level, message)` - unchanged signature, unchanged behavior
  - `log(log_level, message, file, line, function)` - unchanged
  - `is_enabled(log_level)` - unchanged
  - `set_min_level(log_level)` - unchanged
  - `get_min_level()` - returns native log_level

#### Migration Guide
Existing code continues to work without changes. To use the new ILogger interface:

```cpp
// Use logger through ILogger interface
std::shared_ptr<common::interfaces::ILogger> ilogger = my_logger;
ilogger->log(common::interfaces::log_level::info, "Message");

// Or mix both styles
logger->log(log_level::info, "Native style");  // Still works
logger->log(ci::log_level::info, std::string("ILogger style"));  // New
```

#### Related Issues
- Closes #223 (implement common::interfaces::ILogger)
- Prerequisite for #224 (thread_system integration)
- Prerequisite for #225 (thread_system_backend removal)

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
