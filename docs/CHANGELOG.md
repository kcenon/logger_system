# Changelog - Logger System

> **Language:** **English** | [한국어](CHANGELOG_KO.md)

All notable changes to the Logger System project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

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
