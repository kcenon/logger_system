> **Language:** **English** | [한국어](LOGGER_SYSTEM_ARCHITECTURE.kr.md)

# Logger System Architecture

**Version**: 3.0.0
**Last Updated**: 2025-12-10

## Table of Contents

- [Overview](#overview)
- [Architecture Diagram](#architecture-diagram)
- [Core Components](#core-components)
  - [1. ILogger Interface Implementation](#1-ilogger-interface-implementation)
  - [2. Dual API Design](#2-dual-api-design)
  - [3. Configuration Management](#3-configuration-management)
  - [4. Builder Pattern with Validation](#4-builder-pattern-with-validation)
  - [5. Backend Abstraction](#5-backend-abstraction)
  - [6. Interface Segregation](#6-interface-segregation)
  - [7. Log Entry Structure](#7-log-entry-structure)
- [Advanced Features](#advanced-features)
  - [1. Asynchronous Pipeline](#1-asynchronous-pipeline)
  - [2. Error Handling with Result Pattern](#2-error-handling-with-result-pattern)
  - [3. C++20 Source Location](#3-c20-source-location)
  - [4. Performance Monitoring](#4-performance-monitoring)
  - [5. Configuration Strategies](#5-configuration-strategies)
- [Threading Model](#threading-model)
- [Memory Management](#memory-management)
- [Performance Characteristics](#performance-characteristics)
- [Integration Patterns](#integration-patterns)
- [Extension Points](#extension-points)
- [Future Enhancements](#future-enhancements)
- [Best Practices](#best-practices)
- [Platform Notes](#platform-notes)

## Overview

The Logger System (v3.0) is a high-performance, modular logging framework that implements `common::interfaces::ILogger` from common_system. It operates in **standalone mode** by default using `std::jthread`, with optional integration with thread_system for enhanced threading capabilities.

### Key Features (v3.0)
- **ILogger Implementation**: Full implementation of `common::interfaces::ILogger`
- **Standalone Mode**: No thread_system dependency required (uses `std::jthread`)
- **Dual API**: Both `common::interfaces::log_level` and native `logger_system::log_level`
- **C++20 Support**: Leverages Concepts and `source_location`
- **Comprehensive Error Handling**: Result pattern throughout

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                      Client Application                          │
├─────────────────────────────────────────────────────────────────┤
│             common::interfaces::ILogger (API)                    │
│   ┌─────────────────────────────────────────────────────────┐   │
│   │  ILogger Methods (Phase 2.0)                             │   │
│   │  • log(level, message)                                   │   │
│   │  • log(level, message, source_location)  ← C++20        │   │
│   │  • is_enabled(level)                                     │   │
│   │  • set_level(level) / get_level()                        │   │
│   │  • flush()                                               │   │
│   └─────────────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────────────┤
│                  kcenon::logger::logger                          │
│  ┌─────────────────────────────────────────────────────────┐    │
│  │                    Sync Mode                             │    │
│  │  ┌─────────────┐  ┌─────────────┐  ┌────────────┐       │    │
│  │  │   Format    │→ │   Write     │→ │   Flush    │       │    │
│  │  └─────────────┘  └─────────────┘  └────────────┘       │    │
│  └─────────────────────────────────────────────────────────┘    │
│  ┌─────────────────────────────────────────────────────────┐    │
│  │                   Async Mode (Default)                   │    │
│  │  ┌────────────┐  ┌──────────────┐  ┌────────────┐       │    │
│  │  │   Queue    │→ │ async_worker │→ │   Batch    │       │    │
│  │  │            │  │ (std::jthread│  │  Processor │       │    │
│  │  │            │  │  standalone) │  │            │       │    │
│  │  └────────────┘  └──────────────┘  └────────────┘       │    │
│  └─────────────────────────────────────────────────────────┘    │
├─────────────────────────────────────────────────────────────────┤
│               Backend Abstraction Layer (v3.0)                   │
│  ┌──────────────────────┐  ┌─────────────────────────────────┐  │
│  │  standalone_backend  │  │  thread_system_backend          │  │
│  │  (Default)           │  │  (Optional, requires linking)   │  │
│  │  • std::jthread      │  │  • thread_pool integration      │  │
│  │  • No dependencies   │  │  • Enhanced scheduling          │  │
│  └──────────────────────┘  └─────────────────────────────────┘  │
├─────────────────────────────────────────────────────────────────┤
│            Configuration & Builder Pattern Layer                 │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐  │
│  │ logger_builder  │  │ logger_config   │  │ Config          │  │
│  │                 │  │                 │  │ Strategies      │  │
│  │ • Fluent API    │  │ • Validation    │  │ • deployment    │  │
│  │ • Result Types  │  │ • Defaults      │  │ • performance   │  │
│  │ • Backend Sel.  │  │ • C++20 opts    │  │ • environment   │  │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘  │
├─────────────────────────────────────────────────────────────────┤
│                Interface Segregation Layer                       │
│  ┌────────────────┐ ┌─────────────────┐ ┌─────────────────────┐ │
│  │ log_writer_    │ │ log_filter_     │ │ log_formatter_      │ │
│  │ interface      │ │ interface       │ │ interface           │ │
│  └────────────────┘ └─────────────────┘ └─────────────────────┘ │
├─────────────────────────────────────────────────────────────────┤
│                         Writers                                  │
│  ┌────────────┐ ┌──────────┐ ┌──────────────┐ ┌──────────────┐  │
│  │ console_   │ │ file_    │ │ rotating_    │ │ network_     │  │
│  │ writer     │ │ writer   │ │ file_writer  │ │ writer       │  │
│  └────────────┘ └──────────┘ └──────────────┘ └──────────────┘  │
│  ┌────────────┐ ┌──────────┐ ┌──────────────┐                   │
│  │ critical_  │ │ batch_   │ │ async_       │                   │
│  │ writer     │ │ writer   │ │ writer       │                   │
│  └────────────┘ └──────────┘ └──────────────┘                   │
└─────────────────────────────────────────────────────────────────┘
```

## Core Components

### 1. ILogger Interface Implementation

Since v2.0, the logger implements `common::interfaces::ILogger` for ecosystem standardization:

```cpp
namespace kcenon::logger {

class logger : public common::interfaces::ILogger,
               public security::critical_logger_interface {
public:
    // ILogger interface implementation
    common::VoidResult log(common::interfaces::log_level level,
                           const std::string& message) override;

    common::VoidResult log(common::interfaces::log_level level,
                           std::string_view message,
                           const common::source_location& loc =
                               common::source_location::current()) override;

    bool is_enabled(common::interfaces::log_level level) const override;

    common::VoidResult set_level(common::interfaces::log_level level) override;

    common::interfaces::log_level get_level() const override;

    common::VoidResult flush() override;
};

}  // namespace kcenon::logger
```

### 2. Dual API Design

The logger supports both standardized and native APIs for flexibility and backward compatibility:

```cpp
namespace kcenon::logger {

class logger {
public:
    // ========== ILogger Interface (Standardized) ==========
    // Uses common::interfaces::log_level
    common::VoidResult log(common::interfaces::log_level level,
                           const std::string& message);

    // With C++20 source_location (recommended)
    common::VoidResult log(common::interfaces::log_level level,
                           std::string_view message,
                           const common::source_location& loc =
                               common::source_location::current());

    // ========== Native API (Backward Compatible) ==========
    // Uses logger_system::log_level
    void log(log_level level, const std::string& message);

    void log(log_level level, const std::string& message,
             const std::string& file, int line, const std::string& function);

    bool is_enabled(log_level level) const;

    // Deprecated: use set_level()/get_level() instead
    void set_min_level(log_level level);
    log_level get_min_level() const;
};

}  // namespace kcenon::logger
```

### 3. Configuration Management

#### Logger Configuration with Validation

```cpp
namespace kcenon::logger {

struct logger_config {
    // Core settings
    bool async = true;
    std::size_t buffer_size = 8192;
    logger_system::log_level min_level = logger_system::log_level::info;

    // Performance tuning
    std::size_t batch_size = 100;
    std::chrono::milliseconds flush_interval{1000};
    overflow_policy queue_overflow_policy = overflow_policy::drop_newest;

    // Feature flags
    bool enable_metrics = false;
    bool enable_crash_handler = false;
    bool enable_structured_logging = false;
    bool enable_source_location = true;  // C++20 feature

    // Comprehensive validation
    common::VoidResult validate() const;
};

}  // namespace kcenon::logger
```

#### Configuration Templates

Predefined configurations for common scenarios:

| Template | Use Case | Async | Batch Size | Flush Interval |
|----------|----------|-------|------------|----------------|
| `default` | General purpose | true | 100 | 1000ms |
| `production` | Production environments | true | 200 | 500ms |
| `debug` | Development | false | 1 | 0ms |
| `high_performance` | Maximum throughput | true | 500 | 2000ms |
| `low_latency` | Real-time systems | true | 10 | 50ms |

### 4. Builder Pattern with Validation

```cpp
namespace kcenon::logger {

class logger_builder {
public:
    // Fluent interface with validation
    logger_builder& use_template(const std::string& template_name);
    logger_builder& with_async(bool async = true);
    logger_builder& with_buffer_size(std::size_t size);
    logger_builder& with_min_level(logger_system::log_level level);

    // Backend selection (v3.0)
    logger_builder& with_standalone_backend();  // Default
    logger_builder& with_backend(std::unique_ptr<backends::integration_backend> backend);

    // Writers
    logger_builder& add_writer(const std::string& name,
                               std::unique_ptr<base_writer> writer);

    // Build
    common::VoidResult validate() const;
    result<std::unique_ptr<logger>> build();
};

}  // namespace kcenon::logger
```

### 5. Backend Abstraction

The v3.0 architecture introduces a backend abstraction layer for flexible async processing:

```cpp
namespace kcenon::logger::backends {

// Abstract backend interface
class integration_backend {
public:
    virtual ~integration_backend() = default;

    // Log level conversion (dual API support)
    virtual common::interfaces::log_level to_common_level(
        logger_system::log_level level) const = 0;
    virtual logger_system::log_level from_common_level(
        common::interfaces::log_level level) const = 0;
};

// Standalone backend (default since v3.0)
class standalone_backend : public integration_backend {
    // Uses std::jthread for async processing
    // No external dependencies
};

// Thread system backend (optional, requires LOGGER_USE_THREAD_SYSTEM=ON)
class thread_system_backend : public integration_backend {
    // Uses thread_system primitives
    // Enhanced scheduling and worker pools
};

}  // namespace kcenon::logger::backends
```

### 6. Interface Segregation

Clean separation of concerns through dedicated interfaces:

#### Writer Interface
```cpp
namespace kcenon::logger {

class log_writer_interface {
public:
    virtual ~log_writer_interface() = default;
    virtual common::VoidResult write(const log_entry& entry) = 0;
    virtual common::VoidResult flush() = 0;
    virtual bool is_healthy() const { return true; }
};

}  // namespace kcenon::logger
```

#### Filter Interface
```cpp
namespace kcenon::logger {

class log_filter_interface {
public:
    virtual ~log_filter_interface() = default;
    virtual bool should_log(const log_entry& entry) const = 0;
};

}  // namespace kcenon::logger
```

#### Formatter Interface
```cpp
namespace kcenon::logger {

class log_formatter_interface {
public:
    virtual ~log_formatter_interface() = default;
    virtual std::string format(const log_entry& entry) const = 0;
};

}  // namespace kcenon::logger
```

### 7. Log Entry Structure

Unified data structure for all logging operations:

```cpp
namespace kcenon::logger {

struct log_entry {
    logger_system::log_level level;
    std::string message;
    std::string file;
    int line;
    std::string function;
    std::chrono::system_clock::time_point timestamp;
    std::thread::id thread_id;
    std::unordered_map<std::string, std::string> context;  // Structured logging
};

}  // namespace kcenon::logger
```

## Advanced Features

### 1. Asynchronous Pipeline

The async mode (default) uses a sophisticated pipeline:

```
┌──────────────┐    ┌──────────────────┐    ┌──────────────┐
│   Producer   │    │   async_worker   │    │   Writers    │
│   (Caller)   │───▶│  (std::jthread)  │───▶│              │
│              │    │                  │    │              │
│ Non-blocking │    │ • Lock-free      │    │ • Console    │
│ enqueue      │    │   dequeue        │    │ • File       │
│              │    │ • Batch process  │    │ • Network    │
└──────────────┘    └──────────────────┘    └──────────────┘
```

**Key Components:**
- **Producer Side**: Non-blocking enqueue with overflow policies
- **Consumer Side**: `async_worker` using `std::jthread` (standalone mode)
- **Queue Management**: Thread-safe queue with configurable overflow policies
- **Batch Processing**: Intelligent batching for I/O efficiency

### 2. Error Handling with Result Pattern

Uses `common::Result<T>` and `common::VoidResult` from common_system:

```cpp
// Success
return common::VoidResult{};

// Error
return common::make_error<void>(
    common::ErrorCategory::Logger,
    static_cast<int>(logger_error_code::queue_full),
    "Queue is full"
);

// Usage
auto result = logger->log(level, message);
if (!result) {
    std::cerr << "Error: " << result.error().message() << "\n";
}
```

### 3. C++20 Source Location

Automatic source location capture using `common::source_location`:

```cpp
// Source location captured automatically
logger->log(common::interfaces::log_level::info, "Debug message");
// Output: [INFO] [main.cpp:42] [main()] Debug message

// Explicit source location
logger->log(common::interfaces::log_level::info, "Message",
            common::source_location::current());
```

### 4. Performance Monitoring

Built-in metrics collection:

```cpp
namespace kcenon::logger::metrics {

class logger_performance_stats {
public:
    uint64_t get_messages_per_second() const;
    uint64_t get_avg_enqueue_time_ns() const;
    double get_queue_utilization_percent() const;
    uint64_t get_dropped_messages() const;
    uint64_t get_total_messages() const;
    uint64_t get_error_count() const;
};

}  // namespace kcenon::logger::metrics
```

### 5. Configuration Strategies

Flexible configuration management with strategy pattern:

```cpp
// Deployment strategy
logger_builder().for_environment(deployment_env::production);

// Performance strategy
logger_builder().with_performance_tuning(performance_level::high_throughput);

// Environment strategy (reads LOG_* env vars)
logger_builder().auto_configure();

// Composite strategy
logger_builder()
    .for_environment(deployment_env::production)
    .with_performance_tuning(performance_level::balanced)
    .auto_configure();  // Override with env vars
```

## Threading Model

### Synchronous Mode
- **Execution**: Direct write to all registered writers
- **Blocking**: Waits for I/O completion
- **Use Cases**: Low-frequency logging, critical errors, simple applications

### Asynchronous Mode (Default)
- **Execution**: Non-blocking enqueue, background processing via `std::jthread`
- **Throughput**: High-volume logging capability (4.34M msg/s)
- **Use Cases**: High-frequency logging, performance-critical applications

### Thread Safety Guarantees
- All public methods are thread-safe
- Writers are called sequentially (no concurrent writes to same writer)
- Internal state protected by atomic operations
- PIMPL idiom provides ABI stability

## Memory Management

### Buffer Management
- Configurable buffer sizes with validation
- Efficient memory usage with move semantics
- Pre-allocated buffers in async mode
- Smart overflow handling policies

### Object Lifetime
- RAII principles throughout
- Unique ownership via `std::unique_ptr` for writers
- Shared ownership via `std::shared_ptr` where appropriate
- Clear ownership semantics

## Performance Characteristics

### Benchmarks (v3.0)

**Platform**: Apple M1 (8-core) @ 3.2GHz, 16GB RAM, macOS Sonoma

| Mode | Operation | Latency | Throughput | Memory |
|------|-----------|---------|------------|--------|
| Async (standalone) | Enqueue | ~148ns | 4.34M msg/s | ~2MB base |
| Async (thread_sys) | Enqueue | ~140ns | 4.34M msg/s | ~2.5MB base |
| Sync | Direct write | ~100μs | I/O limited | Minimal |

### Multi-threaded Performance

| Threads | Standalone | With thread_system |
|---------|------------|-------------------|
| 1 | 4.34M msg/s | 4.34M msg/s |
| 4 | 1.07M msg/s | 1.15M msg/s |
| 8 | 412K msg/s | 450K msg/s |
| 16 | 390K msg/s | 420K msg/s |

### Optimization Strategies

1. **String Operations**: Minimized allocations, move semantics
2. **Batch Processing**: Adaptive batching for I/O efficiency
3. **Lock Contention**: Minimized through careful design
4. **Cache Performance**: Data structure layout optimization

## Integration Patterns

### ILogger Interface Integration
```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>

using namespace kcenon::logger;

// Build logger
auto logger = logger_builder()
    .use_template("production")
    .add_writer("console", std::make_unique<console_writer>())
    .build()
    .value();

// Use through ILogger interface
common::interfaces::ILogger* ilogger = logger.get();
ilogger->log(common::interfaces::log_level::info, "Application started");
```

### Dependency Injection
```cpp
class MyService {
public:
    MyService(std::shared_ptr<common::interfaces::ILogger> logger)
        : logger_(std::move(logger)) {}

    void do_work() {
        logger_->log(common::interfaces::log_level::debug, "Processing...");
    }

private:
    std::shared_ptr<common::interfaces::ILogger> logger_;
};
```

### With Monitoring (Phase 2.2.4)
```cpp
auto monitor = std::make_shared<monitoring::monitoring>();

auto logger = logger_builder()
    .use_template("production")
    .with_monitoring(monitor)
    .with_health_check_interval(std::chrono::seconds(30))
    .build()
    .value();
```

## Extension Points

### Custom Writers
```cpp
class database_writer : public kcenon::logger::log_writer_interface {
public:
    common::VoidResult write(const kcenon::logger::log_entry& entry) override {
        // Database insert logic
        return common::ok();
    }

    common::VoidResult flush() override {
        // Commit transaction
        return common::ok();
    }
};

// Usage
auto logger = logger_builder()
    .add_writer("database", std::make_unique<database_writer>(connection))
    .build();
```

### Custom Filters
```cpp
class regex_filter : public kcenon::logger::log_filter_interface {
public:
    explicit regex_filter(const std::string& pattern)
        : pattern_(pattern) {}

    bool should_log(const kcenon::logger::log_entry& entry) const override {
        return std::regex_search(entry.message, pattern_);
    }

private:
    std::regex pattern_;
};
```

### Custom Formatters
```cpp
class xml_formatter : public kcenon::logger::log_formatter_interface {
public:
    std::string format(const kcenon::logger::log_entry& entry) const override {
        return "<log level=\"" + to_string(entry.level) + "\">"
               + entry.message + "</log>";
    }
};
```

## Future Enhancements

### Performance Improvements
1. **Lock-free Queue**: True lock-free MPMC queue implementation
2. **SIMD Optimizations**: Vectorized string operations
3. **Memory Pool**: Custom allocators for frequent allocations

### Feature Additions
1. **Distributed Tracing**: Trace ID propagation
2. **Log Aggregation**: Built-in sampling and aggregation
3. **Compression**: Compressed network and file writers
4. **Encryption**: Secure audit logging

### Platform Extensions
1. **Windows Event Log**: Native Windows event log support
2. **syslog Integration**: RFC 5424 compliant syslog
3. **Cloud Logging**: AWS CloudWatch, GCP Logging adapters

## Best Practices

### For Library Users
- Use async mode (default) for production systems
- Leverage C++20 source_location for automatic location capture
- Use `is_enabled()` check before expensive message construction
- Configure appropriate buffer sizes based on load
- Monitor metrics for performance tuning
- Use ILogger interface for dependency injection

### For Contributors
- Maintain thread safety guarantees
- Follow RAII principles consistently
- Use move semantics for efficiency
- Document performance implications of changes
- Write comprehensive tests for new features
- Ensure backward compatibility with native API

## Platform Notes

- **Linux/macOS**: Full support for all features
- **Windows**: Core features supported, network components require WinSock initialization
- **Cross-platform**: CMake build system with feature detection
- **C++ Standard**: Requires C++20 (for Concepts and source_location)
- **Dependencies**: common_system (required), thread_system (optional)

---

*Last Updated: 2025-12-10*
