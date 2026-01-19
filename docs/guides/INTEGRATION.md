# Logger System Integration Guide

**English | [한국어](INTEGRATION.kr.md)**

---

## Table of Contents

- [Overview](#overview)
- [System Dependencies](#system-dependencies)
- [Integration with common_system](#integration-with-common_system)
- [Integration with thread_system](#integration-with-thread_system)
- [Integration with monitoring_system](#integration-with-monitoring_system)
- [Build Configuration](#build-configuration)
- [Code Examples](#code-examples)
- [Result Handling Cheatsheet](#result-handling-cheatsheet)
- [Error Code Registry](#error-code-registry)
- [Troubleshooting](#troubleshooting)

## Overview

The Logger System is designed to integrate seamlessly with the ecosystem while maintaining the ability to operate independently. This document provides comprehensive guidance on integrating logger_system with other system modules.

**Integration Architecture**:
```
common_system (interfaces) ← logger_system implements ILogger
                          ↖ optional: inject IMonitor at runtime
                          ↖ optional: use IExecutor for async ops
```

**Key Integration Points**:
- **common_system**: Provides `ILogger`, `Result<T>`, error code registry
- **thread_system**: Provides async execution via adapters
- **monitoring_system**: Provides metrics collection via `IMonitor` interface

## System Dependencies

### Required Dependencies

#### common_system (Header-only)
- **Version**: 1.0.0+
- **Purpose**: Core interfaces and error handling patterns
- **Components Used**:
  - `ILogger` interface implementation
  - `Result<T>` pattern for error handling
  - Centralized error code registry
  - Smart adapter patterns

**CMake Configuration**:
```cmake
find_package(common_system CONFIG REQUIRED)
target_link_libraries(LoggerSystem PUBLIC kcenon::common_system)
```

### Optional Dependencies

#### thread_system
- **Version**: 1.0.0+
- **Purpose**: Asynchronous logging operations
- **Components Used**:
  - `IExecutor` for async task submission
  - Thread pool for background processing
  - DI adapter for service integration

#### monitoring_system
- **Version**: 1.0.0+
- **Purpose**: Performance metrics and health monitoring
- **Components Used**:
  - `IMonitor` interface for metrics injection
  - Performance counters
  - Health check integration

## Integration with common_system

### ILogger Interface Implementation

The logger_system implements the `ILogger` interface defined in common_system:

```cpp
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/adapters/common_logger_adapter.h>

namespace kcenon::logger {

// Logger implements ILogger interface
class logger : public common::interfaces::ILogger {
public:
    // ILogger interface methods
    auto log(log_level level, const std::string& message) -> void override;
    auto set_level(log_level min_level) -> void override;
    auto is_enabled(log_level level) const -> bool override;
};

} // namespace kcenon::logger
```

**Usage Example**:
```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/common/interfaces/logger_interface.h>

// Create logger as ILogger
std::shared_ptr<common::interfaces::ILogger> logger =
    kcenon::logger::logger_builder()
        .use_template("production")
        .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
        .build()
        .value();

// Use through common interface
logger->log(common::log_level::info, "Application started");
```

### Result&lt;T&gt; Pattern Usage

Logger system uses `Result<T>` for comprehensive error handling:

```cpp
#include <kcenon/common/patterns/result.h>
#include <kcenon/logger/core/logger_builder.h>

// Builder returns Result<unique_ptr<logger>>
auto result = kcenon::logger::logger_builder()
    .use_template("production")
    .with_buffer_size(0)  // Invalid configuration
    .build();

if (!result) {
    // Handle error
    auto error = result.error();
    std::cerr << "Failed to create logger: " << error.message()
              << " (code: " << static_cast<int>(error.code()) << ")\n";
    return -1;
}

auto logger = std::move(result.value());
```

**Core Operations with Result&lt;T&gt;**:
```cpp
// Configuration operations return common::VoidResult
auto add_result = logger->add_writer("file",
    std::make_unique<kcenon::logger::file_writer>("app.log"));

if (add_result.is_err()) {
    std::cerr << "Failed to add writer: "
              << add_result.error().message << "\n";
}

// Hot path logging uses void for zero overhead
logger->log(kcenon::logger::log_level::info, "Performance-critical message");
```

### Adapter Pattern

Use adapters to bridge between logger_system and common_system interfaces:

```cpp
#include <kcenon/logger/adapters/common_logger_adapter.h>

// Adapt logger_system logger to common::interfaces::ILogger
auto native_logger = kcenon::logger::logger_builder()
    .use_template("production")
    .build()
    .value();

auto common_logger = kcenon::logger::adapters::make_common_logger_adapter(
    std::move(native_logger)
);

// Use as common::interfaces::ILogger
common_logger->log(common::log_level::info, "Using adapter pattern");
```

## Integration with thread_system

### Asynchronous Logging with IExecutor

Logger system can use `IExecutor` from common_system for async operations:

```cpp
#include <kcenon/thread/core/thread_pool.h>
#include <kcenon/thread/adapters/common_executor_adapter.h>
#include <kcenon/logger/core/logger.h>

// Create thread pool and adapt to IExecutor
auto thread_pool = std::make_shared<kcenon::thread::thread_pool>(4);
auto executor = kcenon::thread::adapters::make_common_executor(thread_pool);

// Logger can use executor for async operations (future enhancement)
auto logger = kcenon::logger::logger_builder()
    .use_template("production")
    .build()
    .value();

// Current implementation uses internal async queue
// Future: logger->set_executor(executor);
```

### Cross-System Integration

Complete example of DI integration across systems:

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/thread/core/thread_pool.h>
#include <kcenon/thread/interfaces/service_container.h>

int main() {
    // 1. Create logger
    auto logger = kcenon::logger::logger_builder()
        .use_template("production")
        .add_writer("file", std::make_unique<kcenon::logger::file_writer>("app.log"))
        .build()
        .value();

    // 2. Register logger in thread system's service container
    kcenon::thread::service_container::global()
        .register_singleton<kcenon::thread::logger_interface>(logger);

    // 3. Create thread pool with logging context
    auto context = kcenon::thread::thread_context();  // Auto-resolves logger
    auto pool = std::make_shared<kcenon::thread::thread_pool>("WorkerPool", context);

    // 4. Thread pool operations are now automatically logged
    pool->start();
    pool->submit_task([]() {
        // Worker tasks can access logger via context
    });

    return 0;
}
```

## Integration with monitoring_system

### IMonitor Interface Injection

Logger system supports runtime injection of `IMonitor` for metrics collection:

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/monitoring/core/performance_monitor.h>
#include <kcenon/common/interfaces/monitoring_interface.h>

// Create monitoring system
auto monitor = kcenon::monitoring::create_performance_monitor();

// Create logger
auto logger = kcenon::logger::logger_builder()
    .use_template("production")
    .build()
    .value();

// Inject monitor at runtime
logger->set_monitor(monitor.get());

// Logging automatically records metrics
logger->log(kcenon::logger::log_level::info, "Application started");
logger->log(kcenon::logger::log_level::error, "Connection failed");

// Check metrics
auto metrics = monitor->collect_metrics();
std::cout << "Total logs: " << metrics["log_count"].value << std::endl;
std::cout << "Error logs: " << metrics["error_count"].value << std::endl;
```

### Performance Metrics Collection

Logger system exposes built-in metrics:

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/metrics/logger_metrics.h>

// Enable metrics collection
logger->enable_metrics_collection(true);

// Log some messages
for (int i = 0; i < 1000; ++i) {
    logger->log(log_level::info, "Test message");
}

// Get current metrics
auto metrics = logger->get_current_metrics();
std::cout << "Messages per second: " << metrics.get_messages_per_second() << "\n";
std::cout << "Average enqueue time: " << metrics.get_avg_enqueue_time_ns() << " ns\n";
std::cout << "Queue utilization: " << metrics.get_queue_utilization_percent() << "%\n";
```

### Health Monitoring Integration

```cpp
#include <kcenon/common/interfaces/monitoring_interface.h>

// Create logger with health monitoring
auto logger = kcenon::logger::logger_builder()
    .use_template("production")
    .with_monitoring(true)  // Enable monitoring support
    .build()
    .value();

// Health check via monitoring interface
if (logger->is_healthy()) {
    std::cout << "Logger is healthy\n";
} else {
    std::cerr << "Logger health check failed\n";
}
```

## Build Configuration

### CMake Integration Examples

#### Minimal Configuration (common_system only)
```cmake
cmake_minimum_required(VERSION 3.16)
project(MyApp)

# Find logger_system
find_package(LoggerSystem CONFIG REQUIRED)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE LoggerSystem::logger)
```

#### Full Ecosystem Integration
```cmake
cmake_minimum_required(VERSION 3.16)
project(MyApp)

# Find all systems
find_package(common_system CONFIG REQUIRED)
find_package(thread_system CONFIG REQUIRED)
find_package(LoggerSystem CONFIG REQUIRED)
find_package(monitoring_system CONFIG QUIET)

add_executable(myapp main.cpp)

# Link in dependency order
target_link_libraries(myapp PRIVATE
    kcenon::common_system      # Foundation
    kcenon::thread_system      # Core systems
    LoggerSystem::logger       # Service systems
)

# Optional monitoring integration
if(monitoring_system_FOUND)
    target_link_libraries(myapp PRIVATE kcenon::monitoring_system)
    target_compile_definitions(myapp PRIVATE USE_MONITORING)
endif()
```

### Build Configuration Flags

Configure logger_system features via CMake options:

```bash
# Core Features
cmake -DLOGGER_USE_DI=ON                  # Enable dependency injection (default: ON)
cmake -DLOGGER_USE_MONITORING=ON          # Enable monitoring support (default: ON)
cmake -DLOGGER_ENABLE_ASYNC=ON            # Enable async logging (default: ON)

# Integration Options
cmake -DBUILD_WITH_COMMON_SYSTEM=ON       # Enable common_system integration (default: ON)
cmake -DUSE_THREAD_SYSTEM=ON              # Use thread_system if available (default: ON)
cmake -DLOGGER_STANDALONE_MODE=OFF        # Standalone mode without integrations (default: OFF)

# Performance Tuning
cmake -DLOGGER_DEFAULT_BUFFER_SIZE=16384  # Buffer size in bytes
cmake -DLOGGER_DEFAULT_BATCH_SIZE=200     # Batch processing size
cmake -DLOGGER_DEFAULT_QUEUE_SIZE=20000   # Maximum queue size

# Quality Assurance
cmake -DLOGGER_ENABLE_SANITIZERS=ON       # Enable sanitizers
cmake -DLOGGER_ENABLE_COVERAGE=ON         # Enable code coverage
cmake -DLOGGER_WARNINGS_AS_ERRORS=ON      # Treat warnings as errors
```

### vcpkg Integration

Use vcpkg for dependency management:

```json
{
  "name": "myapp",
  "version": "1.0.0",
  "dependencies": [
    "common-system",
    "thread-system",
    "logger-system"
  ]
}
```

## Code Examples

### Example 1: Basic Integration with common_system

```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/common/patterns/result.h>

int main() {
    // Create logger with Result<T> error handling
    auto result = kcenon::logger::logger_builder()
        .use_template("production")
        .with_min_level(kcenon::logger::log_level::info)
        .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
        .build();

    if (!result) {
        std::cerr << "Logger creation failed: "
                  << result.error().message() << "\n";
        return -1;
    }

    auto logger = std::move(result.value());
    logger->log(kcenon::logger::log_level::info, "Application started");

    return 0;
}
```

### Example 2: Full Stack Integration

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/thread/core/thread_pool.h>
#include <kcenon/monitoring/core/performance_monitor.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/common/interfaces/monitoring_interface.h>

int main() {
    // Create monitoring system
    auto monitor = kcenon::monitoring::create_performance_monitor();

    // Create logger with monitoring
    auto logger = kcenon::logger::logger_builder()
        .use_template("production")
        .add_writer("file", std::make_unique<kcenon::logger::file_writer>("app.log"))
        .build()
        .value();

    logger->set_monitor(monitor.get());

    // Create thread pool
    auto pool = kcenon::thread::create_thread_pool(4);

    // Submit logging tasks
    pool->submit([&logger]() {
        logger->log(kcenon::logger::log_level::info, "Task executed");
    });

    // Check metrics
    auto metrics = monitor->collect_metrics();
    std::cout << "Total logs: " << metrics["log_count"].value << "\n";

    return 0;
}
```

### Example 3: Error Handling with Result&lt;T&gt;

```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/common/patterns/result.h>

common::Result<std::unique_ptr<kcenon::logger::logger>>
create_configured_logger(const std::string& config_file) {
    auto result = kcenon::logger::logger_builder()
        .use_template("production")
        .with_buffer_size(32768)
        .add_writer("file", std::make_unique<kcenon::logger::file_writer>("app.log"))
        .build();

    if (!result) {
        return common::error<std::unique_ptr<kcenon::logger::logger>>(
            result.error().code(),
            std::format("Failed to create logger from config '{}': {}",
                        config_file, result.error().message()),
            "logger_factory"
        );
    }

    return common::ok(std::move(result.value()));
}

int main() {
    auto logger_result = create_configured_logger("config.json");

    if (!logger_result) {
        std::cerr << "Error: " << logger_result.error().message() << "\n";
        return -1;
    }

    auto logger = std::move(logger_result.value());
    logger->log(kcenon::logger::log_level::info, "Logger configured successfully");

    return 0;
}
```

## Result Handling Cheatsheet

- Prefer `result.is_err()` / `result.error()` for every public API: legacy `.get_error()` is maintained only for backward compatibility.
- Bubble up failures with `return Result<void>::err(result.error());` to avoid losing diagnostic context.
- When orchestrating several operations, capture both `err.code` and `err.module` for log filtering.

```cpp
auto add_writer_result = logger->add_writer("network", std::move(writer));
if (add_writer_result.is_err()) {
    const auto& err = add_writer_result.error();
    log_error("writer_add_failed",
              fmt::format("[{}] code={} message={}",
                          err.module, err.code, err.message));
    return Result<void>::err(err);
}
```

## Error Code Registry

### Allocated Error Code Range

Logger system uses error codes **-200 to -299** in the centralized error code registry:

```cpp
namespace kcenon::logger {

enum class error_code : int {
    // System lifecycle (-200 to -209)
    logger_not_initialized = -200,
    logger_already_started = -201,
    logger_not_started = -202,
    logger_shutdown = -203,

    // Writer management (-210 to -219)
    writer_not_found = -210,
    writer_already_exists = -211,
    writer_add_failed = -212,
    writer_remove_failed = -213,
    invalid_writer = -214,

    // Configuration (-220 to -229)
    invalid_config = -220,
    invalid_buffer_size = -221,
    invalid_batch_size = -222,
    invalid_queue_size = -223,
    config_validation_failed = -224,

    // I/O operations (-230 to -239)
    write_failed = -230,
    flush_failed = -231,
    file_open_failed = -232,
    network_send_failed = -233,
    queue_full = -234,
    queue_stopped = -235,
};

} // namespace kcenon::logger
```

### Error Message Mapping

```cpp
#include <kcenon/logger/core/error_codes.h>

// Get human-readable error message
std::string get_error_message(kcenon::logger::error_code code) {
    switch (code) {
        case kcenon::logger::error_code::logger_not_initialized:
            return "Logger not initialized";
        case kcenon::logger::error_code::invalid_buffer_size:
            return "Invalid buffer size (must be > 0)";
        case kcenon::logger::error_code::queue_full:
            return "Logger queue is full";
        // ... more mappings
        default:
            return "Unknown error";
    }
}
```

## Troubleshooting

### Build Fails with "common_system not found"

**Problem**: CMake cannot find common_system

**Solution 1**: Ensure common_system is in a sibling directory
```bash
/path/to/systems/
├── common_system/
├── logger_system/
└── your_project/
```

**Solution 2**: Use FetchContent (automatic in logger_system)
```cmake
# logger_system CMakeLists.txt automatically uses FetchContent
# if common_system is not found locally
```

**Solution 3**: Install common_system system-wide
```bash
cd common_system
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build
sudo cmake --install build
```

### Linker Errors: Undefined References

**Problem**: Linking fails with undefined symbol errors

**Solution**: Ensure proper link order
```cmake
target_link_libraries(MyApp PRIVATE
    kcenon::common_system      # Foundation (first)
    kcenon::thread_system      # Core systems
    LoggerSystem::logger       # Service systems
    kcenon::monitoring_system  # Optional systems (last)
)
```

### Runtime: Logger Not Outputting

**Problem**: Logger created but no output visible

**Solution**: Check configuration and flush
```cpp
auto logger = kcenon::logger::logger_builder()
    .use_template("production")
    .with_min_level(log_level::debug)  // Set appropriate level
    .add_writer("console", std::make_unique<console_writer>())
    .build()
    .value();

// Ensure flush before exit
logger->flush();
```

### Integration: IMonitor Not Injecting

**Problem**: Monitoring metrics not being recorded

**Solution**: Verify monitoring is enabled and injected
```cpp
// Enable monitoring at build time
#define LOGGER_USE_MONITORING

// Inject at runtime
logger->set_monitor(monitor.get());

// Verify injection
if (logger->has_monitor()) {
    std::cout << "Monitoring active\n";
}
```

### Performance: High Latency

**Problem**: Logging operations taking too long

**Solution**: Tune performance parameters
```cmake
# Increase batch size for throughput
cmake -DLOGGER_DEFAULT_BATCH_SIZE=500

# Increase buffer size for large messages
cmake -DLOGGER_DEFAULT_BUFFER_SIZE=65536

# Increase queue size to reduce drops
cmake -DLOGGER_DEFAULT_QUEUE_SIZE=50000
```

## Best Practices

### 1. Use Result&lt;T&gt; for Error Handling

Always check `Result<T>` return values:
```cpp
// Good: Check result
auto result = logger_builder().build();
if (!result) {
    handle_error(result.error());
    return;
}

// Bad: Ignore result (may throw)
auto logger = logger_builder().build().value();  // May throw if error
```

### 2. Prefer Interface-Based Design

Use common interfaces for flexibility:
```cpp
// Good: Use interface
std::shared_ptr<common::interfaces::ILogger> logger = create_logger();

// Less flexible: Use concrete type
std::shared_ptr<kcenon::logger::logger> logger = create_logger();
```

### 3. Leverage DI for Testing

Use dependency injection for testability:
```cpp
class MyService {
    std::shared_ptr<common::interfaces::ILogger> logger_;
public:
    MyService(std::shared_ptr<common::interfaces::ILogger> logger)
        : logger_(std::move(logger)) {}
};

// Easy to test with mock logger
auto mock_logger = std::make_shared<MockLogger>();
MyService service(mock_logger);
```

### 4. Monitor Performance in Production

Enable metrics in production builds:
```cpp
logger->enable_metrics_collection(true);

// Periodically check metrics
auto metrics = logger->get_current_metrics();
if (metrics.get_queue_utilization_percent() > 80) {
    // Queue is filling up, increase capacity or reduce log volume
}
```

## References

- [logger_system README.md](README.md) - Project overview
- [STRUCTURE.md](STRUCTURE.md) - Directory structure and organization
- [common_system ARCHITECTURE.md](../common_system/ARCHITECTURE.md) - Common system architecture
- [common_system INTEGRATION.md](../common_system/INTEGRATION.md) - Integration patterns
