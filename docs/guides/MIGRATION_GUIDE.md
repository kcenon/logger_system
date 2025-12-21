> **Language:** **English** | [한국어](MIGRATION_GUIDE_KO.md)

# Logger System Migration Guide

**Version**: 3.0.0
**Last Updated**: 2025-12-14

## Table of Contents
1. [Overview](#overview)
2. [CMake Configuration Changes](#cmake-configuration-changes)
3. [Version Migration](#version-migration)
   - [From v2.x to v3.0](#from-v2x-to-v30)
   - [From v1.x to v2.x](#from-v1x-to-v2x)
4. [API Changes](#api-changes)
5. [Configuration Migration](#configuration-migration)
6. [Migration from Other Libraries](#migration-from-other-libraries)
7. [Compatibility Wrappers](#compatibility-wrappers)
8. [Step-by-Step Migration](#step-by-step-migration)
9. [Common Issues and Solutions](#common-issues-and-solutions)

## Overview

This guide helps you migrate to Logger System from:
- Earlier versions of Logger System (v2.x to v3.0, v1.x to v2.x)
- Other popular logging libraries (spdlog, Boost.Log, glog, log4cpp)
- Custom logging solutions

### Breaking Changes Summary

| Version | Component | Change | Impact |
|---------|-----------|--------|--------|
| **v3.0** | Namespace | `logger_module` → `kcenon::logger` | **High** |
| **v3.0** | Interface | Implements `common::interfaces::ILogger` | **High** |
| **v3.0** | Dependencies | thread_system now optional | Medium |
| **v3.0** | C++ Standard | Requires C++20 | Medium |
| **v3.0** | Result Types | Uses `common::VoidResult` | Medium |
| v2.0 | Error Handling | Exceptions → Result types | High |
| v2.0 | Memory Management | Raw pointers → Smart pointers | High |
| v2.0 | Configuration | Direct setters → Builder pattern | Medium |

## CMake Configuration Changes

### v2.x (Previous)

```cmake
# CMakeLists.txt for v2.x
find_package(thread_system REQUIRED)  # Was required
find_package(logger_system REQUIRED)

target_link_libraries(your_app PRIVATE
    thread_system::thread_system
    logger_system::logger_system
)
```

```bash
# Build flags for v2.x
cmake -DUSE_THREAD_SYSTEM=ON ..  # Old flag name
```

### v3.0 (Current)

```cmake
# CMakeLists.txt for v3.0
find_package(common_system REQUIRED)  # New required dependency
find_package(logger_system REQUIRED)
# find_package(thread_system)  # Optional now

target_link_libraries(your_app PRIVATE
    kcenon::common
    kcenon::logger
    # kcenon::thread  # Optional
)
```

```bash
# Build flags for v3.0
cmake -DLOGGER_USE_THREAD_SYSTEM=ON ..   # New flag name (optional)
cmake -DLOGGER_STANDALONE_MODE=ON ..     # Standalone mode (default)
```

### CMake Flag Changes Summary

| v2.x Flag | v3.0 Flag | Default | Description |
|-----------|-----------|---------|-------------|
| `USE_THREAD_SYSTEM=ON` | `LOGGER_USE_THREAD_SYSTEM=ON` | `OFF` | Enable thread_system integration |
| N/A | `LOGGER_STANDALONE_MODE=ON` | `ON` | Use standalone std::jthread worker |
| `BUILD_TESTS=ON` | `BUILD_TESTS=ON` | `ON` | Build test suite |
| `BUILD_EXAMPLES=ON` | `BUILD_EXAMPLES=ON` | `ON` | Build examples |

### Target Name Changes

| v2.x Target | v3.0 Target |
|-------------|-------------|
| `logger_system::logger_system` | `kcenon::logger` |
| `thread_system::thread_system` | `kcenon::thread` (optional) |
| N/A | `kcenon::common` (required) |

### Dependency Changes

**v2.x Dependency Tree:**
```
logger_system
└── thread_system (required)
```

**v3.0 Dependency Tree:**
```
logger_system
├── common_system (required)
└── thread_system (optional, for async logging with thread pool)
    └── common_system (required)
```

## Version Migration

### From v2.x to v3.0

#### Summary of Changes

| Aspect | v2.x | v3.0 |
|--------|------|------|
| Namespace | `logger_module` | `kcenon::logger` |
| Interface | `thread_module::logger_interface` | `common::interfaces::ILogger` |
| Header Path | `<logger_system/...>` | `<kcenon/logger/...>` |
| Result Type | `result_void` | `common::VoidResult` |
| thread_system | Required | Optional |
| C++ Standard | C++17 | C++20 |

#### 1. Namespace Migration

**Old (v2.x):**
```cpp
#include <logger_system/logger.h>
#include <logger_system/logger_builder.h>
#include <logger_system/writers/console_writer.h>

using namespace logger_module;

auto logger = logger_builder()
    .use_template("production")
    .build();
```

**New (v3.0):**
```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>

using namespace kcenon::logger;

auto logger = logger_builder()
    .use_template("production")
    .build();
```

#### 2. Interface Migration

**Old (v2.x):**
```cpp
class logger : public thread_module::logger_interface {
    result_void log(thread_module::log_level level,
                    const std::string& message) override;
};

// Usage
logger->log(thread_module::log_level::info, "Message");
```

**New (v3.0):**
```cpp
class logger : public common::interfaces::ILogger {
    common::VoidResult log(common::interfaces::log_level level,
                           const std::string& message) override;
};

// Usage with ILogger interface (recommended)
logger->log(common::interfaces::log_level::info, "Message");

// Or with native API (backward compatible)
logger->log(log_level::info, "Message");
```

#### 3. Dual API Support

v3.0 provides both ILogger interface and native API:

```cpp
using namespace kcenon::logger;

auto logger = logger_builder()
    .use_template("production")
    .add_writer("console", std::make_unique<console_writer>())
    .build()
    .value();

// ILogger interface (standardized, recommended for new code)
logger->log(common::interfaces::log_level::info, "Using ILogger interface");

// With C++20 source_location (automatic)
logger->log(common::interfaces::log_level::debug, "Source location captured automatically");

// Native API (backward compatible)
logger->log(log_level::info, "Using native API");

// Note: Manual source location (__FILE__, __LINE__, __func__) is deprecated.
// Source location is now auto-captured. This API will be removed in v3.0.0.
logger->log(log_level::error, "Error message");
```

#### 4. Result Type Migration

**Old (v2.x):**
```cpp
result_void result = logger->log(level, message);
if (!result) {
    std::cerr << result.error().message() << "\n";
}
```

**New (v3.0):**
```cpp
common::VoidResult result = logger->log(level, message);
if (!result) {
    std::cerr << result.error().message() << "\n";
}

// Or using auto (simpler)
auto result = logger->log(level, message);
if (!result) {
    std::cerr << result.error().message() << "\n";
}
```

#### 5. thread_system Dependency Changes

**Old (v2.x):**
```cpp
// thread_system was required
// CMakeLists.txt
find_package(thread_system REQUIRED)
target_link_libraries(your_app PRIVATE thread_system::thread_system)
```

**New (v3.0):**
```cpp
// thread_system is now optional
// CMakeLists.txt
find_package(common_system REQUIRED)  # Required
find_package(logger_system REQUIRED)
# find_package(thread_system OPTIONAL)  # Only if needed

target_link_libraries(your_app PRIVATE
    kcenon::common
    kcenon::logger
)
```

#### 6. Backend Selection (New in v3.0)

```cpp
using namespace kcenon::logger;

// Standalone mode (default, no thread_system needed)
auto logger = logger_builder()
    .with_standalone_backend()  // Uses std::jthread
    .build();

// With thread_system integration (requires LOGGER_USE_THREAD_SYSTEM=ON)
auto logger = logger_builder()
    .with_thread_system_backend()  // Deprecated, falls back to standalone
    .build();

// Custom backend
auto logger = logger_builder()
    .with_backend(std::make_unique<my_custom_backend>())
    .build();
```

#### 7. C++20 Source Location

**New in v3.0:**
```cpp
// Source location is automatically captured
logger->log(common::interfaces::log_level::info, "Debug message");
// Output: [INFO] [main.cpp:42] [main()] Debug message

// Explicit source location
logger->log(common::interfaces::log_level::info, "Message",
            common::source_location::current());
```

#### 8. Configuration Strategies (Enhanced in v3.0)

```cpp
using namespace kcenon::logger;

// Deployment strategy
auto logger = logger_builder()
    .for_environment(deployment_env::production)  // New in v3.0
    .build();

// Performance strategy
auto logger = logger_builder()
    .with_performance_tuning(performance_level::high_throughput)  // New in v3.0
    .build();

// Environment variable configuration
auto logger = logger_builder()
    .auto_configure()  // Reads LOG_* environment variables
    .build();
```

#### 9. Monitoring Integration (New in v3.0)

```cpp
#include <kcenon/monitoring/monitoring.h>

auto monitor = std::make_shared<kcenon::monitoring::monitoring>();

auto logger = logger_builder()
    .with_monitoring(monitor)  // New in v3.0
    .with_health_check_interval(std::chrono::seconds(30))
    .build();
```

#### Migration Script

For automated migration, use this script pattern:

```bash
#!/bin/bash
# migrate_v2_to_v3.sh

# Update includes
find . -name "*.cpp" -o -name "*.h" | xargs sed -i '' \
    -e 's|#include <logger_system/|#include <kcenon/logger/|g' \
    -e 's|#include "logger_system/|#include "kcenon/logger/|g'

# Update namespaces
find . -name "*.cpp" -o -name "*.h" | xargs sed -i '' \
    -e 's|logger_module::|kcenon::logger::|g' \
    -e 's|namespace logger_module|namespace kcenon::logger|g' \
    -e 's|using namespace logger_module|using namespace kcenon::logger|g'

# Update interface references
find . -name "*.cpp" -o -name "*.h" | xargs sed -i '' \
    -e 's|thread_module::logger_interface|common::interfaces::ILogger|g' \
    -e 's|thread_module::log_level|common::interfaces::log_level|g'

# Update result types
find . -name "*.cpp" -o -name "*.h" | xargs sed -i '' \
    -e 's|result_void|common::VoidResult|g'
```

---

### From v1.x to v2.x

#### 1. Error Handling Migration

**Old (v1.x):**
```cpp
try {
    logger->add_writer(new file_writer("app.log"));
} catch (const std::exception& e) {
    std::cerr << "Failed: " << e.what() << std::endl;
}
```

**New (v2.x):**
```cpp
auto result = logger->add_writer(std::make_unique<file_writer>("app.log"));
if (!result) {
    std::cerr << "Failed: " << result.error().message() << std::endl;
}
```

#### 2. Memory Management Migration

**Old (v1.x):**
```cpp
// Manual memory management
base_writer* writer = new file_writer("app.log");
logger->add_writer(writer);  // Logger takes ownership
// Don't delete writer - logger owns it
```

**New (v2.x):**
```cpp
// RAII with unique_ptr
auto writer = std::make_unique<file_writer>("app.log");
logger->add_writer(std::move(writer));  // Explicit ownership transfer
```

#### 3. Configuration Migration

**Old (v1.x):**
```cpp
logger* log = new logger();
log->set_min_level(log_level::info);
log->set_async(true);
log->set_buffer_size(10000);
log->add_console_writer();
log->add_file_writer("app.log");
```

**New (v2.x):**
```cpp
auto log = logger_builder()
    .with_min_level(log_level::info)
    .with_async_mode(true)
    .with_buffer_size(10000)
    .with_console_writer()
    .with_file_writer("app.log")
    .build();
```

#### 4. Logging API Migration

**Old (v1.x):**
```cpp
LOG_INFO(logger, "Message with %s", param);
LOG_DEBUG_F(logger, "Formatted: {}", value);
```

**New (v2.x):**
```cpp
logger->info("Message with param", {{"param", param}});
logger->debug("Message", {{"value", value}});
```

## API Changes

### v3.0 API Changes Summary

| v2.x | v3.0 | Notes |
|------|------|-------|
| `logger_module::logger` | `kcenon::logger::logger` | Namespace change |
| `thread_module::logger_interface` | `common::interfaces::ILogger` | Interface change |
| `thread_module::log_level` | `common::interfaces::log_level` | Level type change |
| `result_void` | `common::VoidResult` | Result type change |
| `set_min_level(level)` | `set_level(level)` | Method rename (deprecated) |
| `get_min_level()` | `get_level()` | Method rename (deprecated) |
| N/A | `log(level, msg, source_location)` | New C++20 overload |
| N/A | `with_standalone_backend()` | New backend selection |
| N/A | `for_environment(env)` | New configuration strategy |
| N/A | `with_performance_tuning(level)` | New configuration strategy |
| N/A | `with_monitoring(monitor)` | New monitoring integration |

### Core Logger API

| v1.x Method | v2.x Equivalent | v3.0 Equivalent |
|-------------|-----------------|-----------------|
| `log(level, msg)` | `log(level, msg, fields)` | `log(level, msg)` via ILogger |
| `set_min_level(level)` | Builder: `with_min_level(level)` | `set_level(level)` + Builder |
| `add_writer(writer*)` | `add_writer(unique_ptr<writer>)` | Same as v2.x |
| `flush()` | `flush()` → result<void> | `flush()` → VoidResult |

## Configuration Migration

### From INI/XML Configuration

**Old (config.ini):**
```ini
[logger]
level = INFO
async = true
buffer_size = 10000

[console]
enabled = true
colored = true

[file]
enabled = true
path = app.log
rotation = 10485760
max_files = 5
```

**New (code-based, v3.0):**
```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/rotating_file_writer.h>

using namespace kcenon::logger;

auto create_logger_from_config() {
    return logger_builder()
        .with_min_level(log_level::info)
        .with_async(true)
        .with_buffer_size(10000)
        .add_writer("console", std::make_unique<console_writer>(true))
        .add_writer("file", std::make_unique<rotating_file_writer>(
            "app.log", 10 * 1024 * 1024, 5))
        .build();
}
```

### Environment-based Configuration (v3.0)

```cpp
using namespace kcenon::logger;

auto create_logger() {
    return logger_builder()
        .auto_configure()  // Reads LOG_* environment variables
        .build();
}

// Supported environment variables:
// LOG_LEVEL: trace, debug, info, warn, error, fatal
// LOG_ASYNC: true/false
// LOG_BUFFER_SIZE: buffer size in bytes
// LOG_BATCH_SIZE: batch size
// LOG_FLUSH_INTERVAL: flush interval in ms
// LOG_COLOR: true/false
// LOG_METRICS: true/false
// LOG_ENV: production/staging/development/testing
```

## Migration from Other Libraries

### From spdlog

```cpp
// spdlog
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

auto logger = spdlog::basic_logger_mt("my_logger", "logs/my_log.txt");
logger->set_level(spdlog::level::info);
logger->info("Hello {}", "World");
logger->error("Error code: {}", 404);

// Logger System v3.0 equivalent
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/file_writer.h>

using namespace kcenon::logger;

auto logger = logger_builder()
    .with_min_level(log_level::info)
    .add_writer("file", std::make_unique<file_writer>("logs/my_log.txt"))
    .build()
    .value();

// Using ILogger interface
logger->log(common::interfaces::log_level::info, "Hello World");
logger->log(common::interfaces::log_level::error, "Error code: 404");

// Or using native API
logger->log(log_level::info, "Hello World");
logger->log(log_level::error, "Error code: 404");
```

### From Boost.Log

```cpp
// Boost.Log
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace logging = boost::log;

logging::add_file_log("sample.log");
BOOST_LOG_TRIVIAL(info) << "An informational message";
BOOST_LOG_TRIVIAL(error) << "An error message";

// Logger System v3.0 equivalent
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/file_writer.h>

using namespace kcenon::logger;

auto logger = logger_builder()
    .add_writer("file", std::make_unique<file_writer>("sample.log"))
    .build()
    .value();

logger->log(common::interfaces::log_level::info, "An informational message");
logger->log(common::interfaces::log_level::error, "An error message");
```

### From Google glog

```cpp
// glog
#include <glog/logging.h>

google::InitGoogleLogging(argv[0]);
LOG(INFO) << "Found " << num_cookies << " cookies";
LOG(ERROR) << "Error code: " << error_code;
CHECK(condition) << "Condition failed";

// Logger System v3.0 equivalent
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>

using namespace kcenon::logger;

auto logger = logger_builder()
    .use_template("production")
    .build()
    .value();

logger->log(common::interfaces::log_level::info,
            "Found " + std::to_string(num_cookies) + " cookies");
logger->log(common::interfaces::log_level::error,
            "Error code: " + std::to_string(error_code));
if (!condition) {
    logger->log(common::interfaces::log_level::fatal, "Condition failed");
    std::abort();
}
```

## Compatibility Wrappers

### v2.x to v3.0 Compatibility Header

```cpp
// compatibility/logger_v2_compat.h
#pragma once

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>

// Namespace alias for gradual migration
namespace logger_module = kcenon::logger;

// Type aliases
using result_void = common::VoidResult;

template<typename T>
using result = common::Result<T>;

// Level mapping
namespace thread_module {
    using log_level = common::interfaces::log_level;
}

// Usage:
// 1. Include this header
// 2. Existing v2.x code will compile with minimal changes
// 3. Gradually update to new namespaces
// 4. Remove this header when migration is complete
```

### Legacy API Wrapper (v1.x compatibility)

```cpp
// compatibility/logger_v1_compat.h
#pragma once
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>

// Legacy macros for backward compatibility
#define LOG_INFO(logger, msg, ...) \
    logger->log(common::interfaces::log_level::info, format_string(msg, ##__VA_ARGS__))

#define LOG_ERROR(logger, msg, ...) \
    logger->log(common::interfaces::log_level::error, format_string(msg, ##__VA_ARGS__))

#define LOG_DEBUG(logger, msg, ...) \
    logger->log(common::interfaces::log_level::debug, format_string(msg, ##__VA_ARGS__))

// Legacy function signatures
namespace logger_v1_compat {

    [[deprecated("Use kcenon::logger::logger_builder instead")]]
    inline auto create_logger() {
        return kcenon::logger::logger_builder().build();
    }

}
```

## Step-by-Step Migration

### Phase 1: Preparation

1. **Check C++ Standard**
```cmake
# Ensure C++20 is enabled
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

2. **Update Dependencies**
```cmake
# v3.0 requires common_system
find_package(common_system REQUIRED)
find_package(logger_system REQUIRED)

# thread_system is now optional
# find_package(thread_system OPTIONAL)
```

3. **Create Migration Branch**
```bash
git checkout -b migration/logger-v3.0
```

### Phase 2: Namespace Migration

1. **Update Include Paths**
```cpp
// OLD
#include <logger_system/logger.h>

// NEW
#include <kcenon/logger/core/logger.h>
```

2. **Update Namespace Usage**
```cpp
// OLD
using namespace logger_module;

// NEW
using namespace kcenon::logger;
```

### Phase 3: Interface Migration

1. **Update Interface References**
```cpp
// OLD
class MyLogger : public thread_module::logger_interface { ... };

// NEW
class MyLogger : public common::interfaces::ILogger { ... };
```

2. **Update Log Level Types**
```cpp
// OLD
thread_module::log_level::info

// NEW
common::interfaces::log_level::info
// or (native API)
kcenon::logger::log_level::info
```

### Phase 4: Validation

1. **Run Tests**
```bash
mkdir build && cd build
cmake .. -DCMAKE_CXX_STANDARD=20
make -j$(nproc)
ctest --output-on-failure
```

2. **Check Sanitizers**
```bash
cmake .. -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined"
make && ctest
```

## Common Issues and Solutions

### Issue 1: Namespace Not Found

**Error:**
```
error: 'logger_module' is not a namespace-name
```

**Solution:**
```cpp
// Change from
using namespace logger_module;

// To
using namespace kcenon::logger;
```

### Issue 2: Interface Type Mismatch

**Error:**
```
error: 'thread_module::logger_interface' is not a base of 'kcenon::logger::logger'
```

**Solution:**
```cpp
// Change from
thread_module::logger_interface* logger = ...;

// To
common::interfaces::ILogger* logger = ...;
```

### Issue 3: Result Type Mismatch

**Error:**
```
error: cannot convert 'common::VoidResult' to 'result_void'
```

**Solution:**
```cpp
// Change from
result_void result = logger->log(...);

// To
common::VoidResult result = logger->log(...);
// Or simply
auto result = logger->log(...);
```

### Issue 4: Missing thread_system

**Error:**
```
error: 'thread_module' has not been declared
```

**Solution:**
```cpp
// v3.0 doesn't require thread_system
// Change from
#include <thread_system/logger_interface.h>

// To
#include <common/interfaces/logger_interface.h>
```

### Issue 5: C++20 Not Enabled

**Error:**
```
error: 'source_location' is not a member of 'std'
```

**Solution:**
```cmake
# Ensure C++20 is enabled
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

## Migration Checklist

### v2.x to v3.0 Migration

- [ ] Enable C++20 in build system
- [ ] Update common_system dependency
- [ ] Update include paths (`logger_system/` → `kcenon/logger/`)
- [ ] Update namespace (`logger_module` → `kcenon::logger`)
- [ ] Update interface references (`thread_module::logger_interface` → `common::interfaces::ILogger`)
- [ ] Update log level types (`thread_module::log_level` → `common::interfaces::log_level`)
- [ ] Update result types (`result_void` → `common::VoidResult`)
- [ ] Remove thread_system dependency if not needed
- [ ] Update deprecated method calls (`set_min_level` → `set_level`)
- [ ] Test with sanitizers
- [ ] Performance benchmarks
- [ ] Update documentation

### v1.x to v2.x Migration

- [ ] Backup current codebase
- [ ] Create migration branch
- [ ] Add compatibility headers
- [ ] Update build system (CMake/Make)
- [ ] Migrate logger creation to builder pattern
- [ ] Update writer management to smart pointers
- [ ] Convert logging calls
- [ ] Update configuration
- [ ] Migrate tests
- [ ] Remove deprecated code
- [ ] Run full test suite
- [ ] Performance benchmarks
- [ ] Update documentation
- [ ] Code review
- [ ] Merge to main branch

## Support and Resources

- [Quick Start Guide](QUICK_START.md)
- [API Reference](../API_REFERENCE.md)
- [Architecture Documentation](../advanced/LOGGER_SYSTEM_ARCHITECTURE.md)
- [Best Practices Guide](BEST_PRACTICES.md)
- [thread_system Integration](../integration/THREAD_SYSTEM.md)
- [Example Code](../../examples/)
- [Issue Tracker](https://github.com/kcenon/logger_system/issues)

For migration assistance, please file an issue with the `migration` label.

---

*Last Updated: 2025-12-14*
