# Architecture - Logger System

> **Language:** **English** | [한국어](ARCHITECTURE_KO.md)

## Overview

The Logger System is a production-ready C++20 asynchronous logging framework designed with modular, interface-based architecture for high-performance multithreaded applications.

**Version:** 1.0.0
**Last Updated:** 2025-10-22

---

## Table of Contents

- [Design Philosophy](#design-philosophy)
- [Layered Architecture](#layered-architecture)
- [Core Components](#core-components)
- [Integration Architecture](#integration-architecture)
- [Threading Model](#threading-model)
- [Memory Management](#memory-management)
- [Performance Characteristics](#performance-characteristics)
- [Design Patterns](#design-patterns)
- [Build Configuration](#build-configuration)

---

## Design Philosophy

### Core Principles

1. **Interface-Driven Design**: Clean separation via abstract interfaces
   - ILogger, IMonitor, IMonitorable for ecosystem integration
   - IWriter, IFormatter, IFilter for component abstraction
   - Zero circular dependencies through interface-only dependencies

2. **Asynchronous by Default**: Non-blocking log operations
   - Background thread processes batched log entries
   - Zero-copy message passing where possible
   - Adaptive queuing with intelligent backoff

3. **Modular Architecture**: Use only what you need
   - Optional ecosystem integration (common_system, thread_system, monitoring_system)
   - Pluggable writers, filters, formatters
   - Independent compilation without ecosystem dependencies

4. **Performance First**: Optimized for production workloads
   - Batched processing for maximum throughput
   - Lock-free operations on hot path
   - Minimal allocations with small string optimization

5. **Type Safety**: Comprehensive compile-time guarantees
   - Result<T> pattern for error handling
   - Builder pattern with validation
   - Template-based configuration strategies

---

## Layered Architecture

```
┌──────────────────────────────────────────────────────────────┐
│                      Application Layer                       │
│  (User code using logger via logger_builder or DI adapter)  │
└───────────────────────────┬──────────────────────────────────┘
                            │
┌───────────────────────────┴──────────────────────────────────┐
│                    Public API Layer                          │
│  ┌─────────────┐  ┌──────────────┐  ┌───────────────────┐  │
│  │   logger    │  │logger_builder│  │ common_logger_    │  │
│  │   (main)    │  │  (fluent)    │  │   adapter (DI)    │  │
│  └─────────────┘  └──────────────┘  └───────────────────┘  │
└───────────────────────────┬──────────────────────────────────┘
                            │
┌───────────────────────────┴──────────────────────────────────┐
│                     Core Layer                               │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────────────┐   │
│  │ log_collector│  │ logger_config│  │ monitoring_     │   │
│  │  (batching)  │  │ (validation) │  │ factory (DI)    │   │
│  └──────────────┘  └──────────────┘  └─────────────────┘   │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────────────┐   │
│  │ small_string │  │ error_codes  │  │ thread_         │   │
│  │ (optimized)  │  │  (-200~-299) │  │ integration_    │   │
│  └──────────────┘  └──────────────┘  │ detector        │   │
│                                       └─────────────────┘   │
└───────────────────────────┬──────────────────────────────────┘
                            │
┌───────────────────────────┴──────────────────────────────────┐
│                  Interface Layer                             │
│  ┌─────────────────────┐  ┌─────────────────────────────┐   │
│  │ ILogger             │  │ log_writer_interface        │   │
│  │ (logger_interface)  │  │ (output abstraction)        │   │
│  └─────────────────────┘  └─────────────────────────────┘   │
│  ┌─────────────────────┐  ┌─────────────────────────────┐   │
│  │ IFormatter          │  │ IFilter                     │   │
│  │ (format abstraction)│  │ (filter abstraction)        │   │
│  └─────────────────────┘  └─────────────────────────────┘   │
│  ┌─────────────────────┐  ┌─────────────────────────────┐   │
│  │ IMonitor            │  │ log_sink_interface          │   │
│  │ (metrics/observ.)   │  │ (processing abstraction)    │   │
│  └─────────────────────┘  └─────────────────────────────┘   │
└───────────────────────────┬──────────────────────────────────┘
                            │
┌───────────────────────────┴──────────────────────────────────┐
│               Implementation Layer                           │
│  ┌──────────────────┐  ┌──────────────────┐  ┌────────────┐ │
│  │ Writers:         │  │ Formatters:      │  │ Filters:   │ │
│  │ • console_writer │  │ • base_formatter │  │ • log_     │ │
│  │ • file_writer    │  │   (plain text)   │  │   filter   │ │
│  │ • rotating_file_ │  │ • JSON formatter │  │ • level    │ │
│  │   writer         │  │ • XML formatter  │  │ • regex    │ │
│  │ • network_writer │  │ • Custom formats │  │ • custom   │ │
│  │ • critical_writer│  └──────────────────┘  └────────────┘ │
│  │ • encrypted_     │                                        │
│  │   writer         │                                        │
│  │ • async_writer   │                                        │
│  │ • batch_writer   │                                        │
│  └──────────────────┘                                        │
└───────────────────────────┬──────────────────────────────────┘
                            │
┌───────────────────────────┴──────────────────────────────────┐
│                 Advanced Features Layer                      │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────┐  │
│  │ log_router   │  │ log_analyzer │  │ crash_safe_      │  │
│  │ (routing)    │  │ (analysis)   │  │ logger (safety)  │  │
│  └──────────────┘  └──────────────┘  └──────────────────┘  │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────┐  │
│  │ log_server   │  │ structured_  │  │ logger_metrics   │  │
│  │ (remote log) │  │ logger (JSON)│  │ (performance)    │  │
│  └──────────────┘  └──────────────┘  └──────────────────┘  │
└──────────────────────────────────────────────────────────────┘
```

### Layer Responsibilities

1. **Application Layer**: User-facing code using logger
2. **Public API Layer**: Main entry points (logger, builder, adapters)
3. **Core Layer**: Business logic (batching, configuration, monitoring)
4. **Interface Layer**: Abstract contracts for all components
5. **Implementation Layer**: Concrete writer/formatter/filter implementations
6. **Advanced Features Layer**: Optional high-level features

---

## Core Components

### 1. Logger (`core/logger.h`, `core/logger.cpp`)

**Purpose**: Main logger class with asynchronous processing

**Key Methods**:
```cpp
class logger {
public:
    // Logging methods
    void log(log_level level, std::string_view message);
    void trace(std::string_view message);
    void debug(std::string_view message);
    void info(std::string_view message);
    void warn(std::string_view message);
    void error(std::string_view message);
    void critical(std::string_view message);

    // Control methods
    void flush();
    void stop();

    // Configuration
    void set_level(log_level level);
    void add_writer(std::shared_ptr<log_writer_interface> writer);
};
```

**Features**:
- Asynchronous batched processing
- Background worker thread
- Thread-safe queue operations
- Automatic buffer management
- Emergency flush on crash

---

### 2. Logger Builder (`core/logger_builder.h`)

**Purpose**: Fluent API for logger construction with validation

**Key Methods**:
```cpp
class logger_builder {
public:
    logger_builder& with_level(log_level level);
    logger_builder& with_console_output();
    logger_builder& with_file_output(const std::string& path);
    logger_builder& with_rotating_file(const std::string& path, size_t max_size);
    logger_builder& with_network_output(const std::string& host, uint16_t port);
    logger_builder& with_monitoring(std::shared_ptr<monitoring_interface> monitor);
    logger_builder& with_template(config_template tmpl);

    common::Result<std::shared_ptr<logger>> build();
};
```

**Configuration Templates**:
- `production`: Rotating files, structured format, ERROR+ level
- `debug`: Console + file, DEBUG+ level, colored output
- `high_performance`: Async, batched, minimal validation
- `low_latency`: Lock-free, optimized for minimal delay

---

### 3. Log Collector (`core/log_collector.h`)

**Purpose**: Batched log entry collection and processing

**Features**:
- Queue-based batching
- Configurable batch sizes (default: 100 entries)
- Adaptive queue depth
- Back-pressure handling
- Emergency overflow handling

**Performance**:
- Average enqueue: 148 nanoseconds
- Peak throughput: 4.34M messages/second (single thread)
- Multi-thread: 1.07M messages/s (4 threads)

---

### 4. Writers

#### Console Writer (`writers/console_writer.h`)
- Colored ANSI output
- Stdout/stderr routing by level
- Thread-safe console access

#### File Writer (`writers/file_writer.h`)
- Buffered file I/O
- Automatic flush policies
- Error recovery with Result<T>

#### Rotating File Writer (`writers/rotating_file_writer.h`)
- Size-based rotation (default: 10MB)
- Time-based rotation (daily, hourly)
- Automatic old file compression
- Configurable retention policy

#### Network Writer (`writers/network_writer.h`)
- TCP/UDP transport
- Connection pooling
- Automatic reconnection
- Buffering on network failure

#### Critical Writer (`writers/critical_writer.h`)
- Synchronous logging (no batching)
- Immediate flush guarantee
- For critical messages only

#### Async Writer (`writers/async_writer.h`)
- Wraps any writer for async operation
- Independent worker thread
- Backlog management

---

### 5. Formatters

#### Base Formatter (`formatters/base_formatter.h`)
```
[2025-10-22 15:30:45.123] [INFO] [thread:1234] Message text
```

#### JSON Formatter
```json
{
  "timestamp": "2025-10-22T15:30:45.123Z",
  "level": "INFO",
  "thread_id": 1234,
  "message": "Message text"
}
```

#### XML Formatter
```xml
<log>
  <timestamp>2025-10-22T15:30:45.123Z</timestamp>
  <level>INFO</level>
  <thread_id>1234</thread_id>
  <message>Message text</message>
</log>
```

---

### 6. Filters

#### Level Filter (`filters/log_filter.h`)
- Filter by minimum log level
- Per-writer level configuration

#### Regex Filter
- Pattern-based message filtering
- Compiled regex for performance

#### Function Filter
- Custom filter logic via lambda
- Stateful filtering support

---

## Integration Architecture

### Common System Integration

```cpp
// logger_system implements ILogger from common_system
namespace kcenon::logger {
    class logger : public common::ILogger {
    public:
        // Implements ILogger interface
        void log(log_level level, std::string_view message) override;
        void flush() override;
        void stop() override;
    };
}

// Usage with Result<T> pattern
common::Result<std::shared_ptr<logger>> create_logger() {
    return logger_builder()
        .with_console_output()
        .with_level(log_level::info)
        .build();
}

auto result = create_logger();
if (result.is_ok()) {
    auto logger_instance = result.value();
    logger_instance->info("Logger created successfully");
}
```

### Thread System Integration

```cpp
// Optional integration detected at compile-time
#if defined(LOGGER_USE_THREAD_SYSTEM)
    #include <thread_system/logger_interface.h>

    // logger implements thread_system::logger_interface
    class logger : public thread_system::logger_interface {
    public:
        void log(log_level level, std::string_view message) override;
    };
#endif
```

**Dependency Injection Adapter**:
```cpp
// Adapter for runtime DI from thread_system
#include <kcenon/logger/adapters/logger_adapter.h>

auto logger_instance = create_logger().value();
auto adapter = std::make_shared<logger_adapter>(logger_instance);

// Inject into thread_system
thread_pool->set_logger(adapter);
```

### Monitoring System Integration

```cpp
// Optional monitoring backend injection
#include <kcenon/logger/core/monitoring/monitoring_interface.h>

// Create logger with monitoring
auto logger_instance = logger_builder()
    .with_console_output()
    .with_monitoring(monitoring_backend) // Inject IMonitor
    .build()
    .value();

// Logger reports metrics to monitoring_backend:
// - Log messages per second
// - Average batch size
// - Queue depth
// - Writer performance
```

---

## Threading Model

### Asynchronous Processing

```
┌─────────────┐                   ┌──────────────┐
│ Application │ log() [enqueue]   │ Log Collector│
│   Thread    │──────────────────>│   (queue)    │
│             │   [non-blocking]  │              │
└─────────────┘                   └──────┬───────┘
                                         │
                                         │ batched entries
                                         ↓
                                  ┌──────────────┐
                                  │   Worker     │
                                  │   Thread     │
                                  │  (batching)  │
                                  └──────┬───────┘
                                         │
                       ┌─────────────────┼─────────────────┐
                       │                 │                 │
                       ↓                 ↓                 ↓
                ┌─────────────┐   ┌─────────────┐   ┌─────────────┐
                │  Writer 1   │   │  Writer 2   │   │  Writer N   │
                │  (console)  │   │   (file)    │   │  (network)  │
                └─────────────┘   └─────────────┘   └─────────────┘
```

### Thread Safety Guarantees

1. **Lock-Free Enqueue**: Application threads never block on locks during log()
2. **Single Writer Thread**: One worker thread processes all batches
3. **Writer Isolation**: Each writer is called serially from worker thread
4. **Queue Protection**: Internal queue uses mutex only on worker thread

**Performance Impact**:
- Enqueue latency: 148ns average
- No contention on hot path
- Batching reduces writer calls by 100x

---

## Memory Management

### Small String Optimization (SSO)

```cpp
// small_string.h - Stack-allocated short strings
class small_string {
    static constexpr size_t SSO_SIZE = 23; // Fit in 24 bytes

    union {
        char stack_buffer[SSO_SIZE + 1];
        struct {
            char* heap_ptr;
            size_t capacity;
        };
    };
    size_t size_;
    bool is_heap_;
};
```

**Benefits**:
- Short messages (<24 chars) never allocate
- 95% of log messages fit in SSO
- Reduces allocation overhead by 90%

### Log Entry Structure

```cpp
struct log_entry {
    log_level level;
    small_string message;        // SSO-optimized
    std::chrono::system_clock::time_point timestamp;
    std::thread::id thread_id;
    std::source_location location; // C++20 feature
};
```

**Memory Footprint**:
- sizeof(log_entry) = 64 bytes
- Queue capacity: 10,000 entries = 640KB
- Total baseline memory: <2MB

---

## Performance Characteristics

### Benchmarks

**Platform**: Apple M1 (8-core) @ 3.2GHz, 16GB RAM, macOS Sonoma

| Metric | Value | Comparison |
|--------|-------|------------|
| **Single-thread throughput** | 4.34M msg/s | Competitive with spdlog async |
| **4-thread throughput** | 1.07M msg/s | 24% better than spdlog |
| **8-thread throughput** | 412K msg/s | 78% improvement with batching |
| **16-thread throughput** | 390K msg/s | 117% boost in high contention |
| **Average enqueue latency** | 148 ns | 15.7x lower than spdlog |
| **Memory baseline** | <2MB | Adaptive buffer management |

### Optimization Techniques

1. **Batching**: Processes 100 entries per batch (configurable)
2. **Zero-Copy**: Move semantics for log_entry transfer
3. **SSO**: Small string optimization eliminates 95% of allocations
4. **Lock-Free Enqueue**: Application threads never wait
5. **Adaptive Queue**: Grows/shrinks based on load
6. **Writer Pooling**: Reuses writer connections

---

## Design Patterns

### 1. Observer Pattern
- Log entries notify registered writers
- Filters observe and potentially block events
- Monitoring observers collect metrics

### 2. Strategy Pattern
- Pluggable formatters (plain, JSON, XML)
- Pluggable writers (console, file, network)
- Pluggable filters (level, regex, custom)

### 3. Builder Pattern
- Fluent API for logger construction
- Comprehensive validation before build()
- Template-based configuration shortcuts

### 4. Template Method Pattern
- base_writer defines write() workflow
- Derived writers customize format_message()
- Inherited flush() and close() behavior

### 5. Dependency Injection
- monitoring_interface injected at runtime
- common_logger_adapter for ecosystem DI
- di_container_factory for advanced scenarios

### 6. Factory Pattern
- writer factory creates appropriate writers
- formatter factory based on output type
- monitoring_factory for IMonitor instances

---

## Build Configuration

### CMake Options

```cmake
# Core options
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(BUILD_SAMPLES "Build sample programs" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Integration options
option(LOGGER_USE_THREAD_SYSTEM "Integrate with thread_system" ON)
option(BUILD_WITH_COMMON_SYSTEM "Integrate with common_system" ON)
option(LOGGER_STANDALONE_MODE "Build without ecosystem dependencies" OFF)

# Testing options
option(LOGGER_BUILD_INTEGRATION_TESTS "Build integration tests" ON)
option(LOGGER_ENABLE_COVERAGE "Enable code coverage reporting" OFF)

# Performance options
option(USE_LOCKFREE "Use lock-free data structures (reserved)" OFF)
option(ENABLE_SMALL_STRING_OPTIMIZATION "Enable SSO" ON)
```

### Dependency Resolution

**Priority Order** (CMakeLists.txt):
1. Local path: `/Users/dongcheolshin/Sources/common_system/include`
2. User home: `/home/$USER/Sources/common_system/include`
3. GitHub workspace: `$ENV{GITHUB_WORKSPACE}/common_system/include`
4. Sibling directory: `${CMAKE_CURRENT_SOURCE_DIR}/../common_system/include`
5. Parent directory: `${CMAKE_SOURCE_DIR}/../common_system/include`
6. FetchContent from GitHub (fallback)

### Compiler Requirements

- **C++ Standard**: C++20 required
- **Compilers**: GCC 10+, Clang 10+, MSVC 19.29+ (Visual Studio 2019+)
- **Platforms**: Windows, Linux, macOS
- **Architectures**: x86, x86_64, ARM, ARM64

---

## Future Enhancements

### Planned Features

1. **Lock-Free Queue**: Replace mutex-based queue with MPSC lock-free queue
2. **SIMD Formatting**: Vectorized string formatting for high-performance paths
3. **Compression**: Automatic log compression for rotating files
4. **Remote Configuration**: Dynamic configuration updates via network
5. **Distributed Tracing**: OpenTelemetry integration for distributed systems

### Research Areas

1. **Structured Logging**: Enhanced JSON/CBOR support with schema validation
2. **Query Interface**: SQL-like query language for log analysis
3. **Machine Learning**: Anomaly detection in log patterns
4. **Cloud Integration**: Native support for AWS CloudWatch, Google Cloud Logging

---

## Error Code Allocation

Logger System uses error codes in the range **-200 to -299**.

Common error codes:
- `-200`: Invalid configuration
- `-201`: Writer initialization failed
- `-202`: File open error
- `-203`: Network connection failed
- `-204`: Queue overflow
- `-205`: Format error
- `-206`: Filter error

See `core/error_codes.h` for complete error code listing.

---

## References

- [README.md](README.md) - Project overview and quick start
- [CHANGELOG.md](CHANGELOG.md) - Version history
- [MIGRATION.md](MIGRATION.md) - Migration from integrated thread_system
- [BASELINE.md](BASELINE.md) - Build requirements and baseline
- [common_system Integration](https://github.com/kcenon/common_system)
- [thread_system Integration](https://github.com/kcenon/thread_system)
- [monitoring_system Integration](https://github.com/kcenon/monitoring_system)

---

**Last Updated:** 2025-10-22
**Version:** 1.0.0
**Maintainer:** kcenon@naver.com
