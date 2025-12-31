# Asynchronous Writer Implementations Guide

> **Language:** **English** | [한국어](ASYNC_WRITERS_KO.md)

## Overview

The Logger System provides multiple asynchronous writer implementations, each optimized for different use cases. This guide helps you understand the differences and choose the right implementation for your needs.

**Version:** 1.0.0
**Last Updated:** 2025-12-31

---

## Table of Contents

- [Quick Decision Guide](#quick-decision-guide)
- [Implementation Comparison](#implementation-comparison)
- [Detailed Descriptions](#detailed-descriptions)
  - [async_writer](#1-async_writer-public-api)
  - [async_worker](#2-async_worker-internal)
  - [high_performance_async_writer](#3-high_performance_async_writer-internal)
- [Performance Characteristics](#performance-characteristics)
- [Usage Examples](#usage-examples)
- [Best Practices](#best-practices)
- [Migration Guide](#migration-guide)

---

## Quick Decision Guide

| Use Case | Recommended Implementation |
|----------|---------------------------|
| **General async logging** | `async_writer` |
| **Simple async wrapper for any writer** | `async_writer` |
| **Maximum throughput (>1M msg/sec)** | `high_performance_async_writer` (internal) |
| **Custom async task processing** | `async_worker` (internal) |
| **Production applications** | `async_writer` with appropriate queue size |

### Decision Flowchart

```
Start
  │
  ├─ Need simple async wrapper? ──────────────► async_writer (Public API)
  │
  ├─ Need maximum throughput?
  │   │
  │   └─ Yes ──────────────────────────────────► high_performance_async_writer
  │                                               (Internal, requires custom build)
  │
  └─ Building internal components? ────────────► async_worker (Internal API)
```

---

## Implementation Comparison

| Aspect | `async_writer` | `async_worker` | `high_performance_async_writer` |
|--------|----------------|----------------|--------------------------------|
| **Location** | Public API | Internal | Internal |
| **Header** | `writers/async_writer.h` | `src/impl/async/` | `src/impl/async/` |
| **Purpose** | Wrap any writer | Generic task worker | Maximum throughput |
| **Queue Type** | `std::queue` + mutex | `std::queue` + mutex | Lock-free queue |
| **Memory Pooling** | No | No | Yes (configurable) |
| **Batch Processing** | No | No | Yes (dynamic sizing) |
| **jthread Support** | No | Yes (with fallback) | No |
| **Performance Stats** | No | Dropped count only | Full statistics |
| **Recommended For** | Most use cases | Internal components | High-volume logging |

---

## Detailed Descriptions

### 1. async_writer (Public API)

**Location:** `include/kcenon/logger/writers/async_writer.h`

The `async_writer` is the **recommended choice for most applications**. It wraps any `base_writer` implementation and provides asynchronous writing capabilities using a background thread.

#### Key Features

- **Universal wrapper**: Works with any `base_writer` (console, file, network, etc.)
- **Configurable queue size**: Default 10,000 messages
- **Configurable flush timeout**: Default 5 seconds
- **Graceful shutdown**: Processes remaining messages on stop
- **Thread-safe**: Safe for concurrent writes from multiple threads

#### Architecture

```
┌─────────────┐     ┌──────────────┐     ┌────────────────┐
│ Application │────►│ async_writer │────►│ wrapped_writer │
│   Threads   │     │   (queue)    │     │  (any writer)  │
└─────────────┘     └──────┬───────┘     └────────────────┘
                           │
                    ┌──────┴───────┐
                    │ Worker Thread│
                    └──────────────┘
```

#### API Reference

```cpp
class async_writer : public base_writer {
public:
    // Constructor
    explicit async_writer(
        std::unique_ptr<base_writer> wrapped_writer,
        std::size_t queue_size = 10000,
        std::chrono::seconds flush_timeout = std::chrono::seconds(5));

    // Control methods
    void start();                        // Start worker thread
    void stop(bool force_flush = true);  // Stop with optional flush

    // base_writer interface
    common::VoidResult write(...) override;
    common::VoidResult flush() override;
    bool is_healthy() const override;
    std::string get_name() const override;
};
```

#### When to Use

- Standard async logging requirements
- Wrapping slow writers (file, network)
- Applications with moderate logging volume (<100K msg/sec)
- When simplicity is preferred over maximum performance

---

### 2. async_worker (Internal)

**Location:** `src/impl/async/async_worker.h`

The `async_worker` is a **low-level component** used internally for task-based asynchronous processing. It's designed for internal use and provides a foundation for building async components.

#### Key Features

- **Task-based processing**: Uses `std::function<void()>` for flexibility
- **jthread compatibility**: Uses `std::jthread` where available, with fallback
- **Cooperative cancellation**: Graceful shutdown via stop tokens
- **Dropped task tracking**: Monitors queue overflow

#### Architecture

```
┌──────────────┐     ┌──────────────┐
│ Task Sources │────►│ async_worker │────► Execute tasks
│  (internal)  │     │   (queue)    │      sequentially
└──────────────┘     └──────────────┘
```

#### When to Use

- Building internal async components
- Custom task processing requirements
- Integration with other async systems

> **Note:** This is an internal API and may change without notice.

---

### 3. high_performance_async_writer (Internal)

**Location:** `src/impl/async/high_performance_async_writer.h`

The `high_performance_async_writer` is optimized for **maximum throughput** scenarios. It combines lock-free queues, memory pooling, and batch processing for high-volume logging.

#### Key Features

- **Lock-free queue**: Minimizes contention in high-concurrency scenarios
- **Memory pooling**: Reduces allocation overhead
- **Dynamic batch processing**: Adapts batch size based on load
- **Back-pressure handling**: Configurable queue full behavior
- **Performance statistics**: Detailed metrics (latency, throughput, success ratio)

#### Architecture

```
┌─────────────┐     ┌─────────────────────────────────────┐
│ Application │     │  high_performance_async_writer      │
│   Threads   │────►│  ┌─────────────┐  ┌──────────────┐ │
└─────────────┘     │  │ Lock-free   │  │ Batch        │ │
                    │  │ Queue       │─►│ Processor    │─┼──► Writer
                    │  └─────────────┘  └──────────────┘ │
                    │  ┌─────────────┐                   │
                    │  │ Memory Pool │                   │
                    │  └─────────────┘                   │
                    └─────────────────────────────────────┘
```

#### Configuration

```cpp
struct config {
    // Queue configuration
    size_t queue_size{8192};              // Must be power of 2

    // Batch processing
    batch_processor::config batch_config;
    // - initial_batch_size: 50
    // - max_batch_size: 500
    // - min_batch_size: 10
    // - max_wait_time: 100ms
    // - enable_dynamic_sizing: true
    // - enable_back_pressure: true

    // Memory pool
    memory::object_pool<...>::config pool_config;
    // - initial_size: 200
    // - max_size: 2000
    // - allow_growth: true

    // Features
    bool enable_memory_pooling{true};
    bool enable_batch_processing{true};
    std::chrono::microseconds flush_timeout{1000};
};
```

#### When to Use

- Logging >100K messages per second
- Latency-sensitive applications
- Systems with high thread contention
- When detailed performance metrics are needed

> **Note:** This is currently an internal API. Contact maintainers for production use guidance.

---

## Performance Characteristics

### Benchmark Comparison

**Test Environment:** Apple M1 (8-core), 16GB RAM, macOS

| Metric | async_writer | high_performance_async_writer |
|--------|--------------|------------------------------|
| **Single-thread throughput** | ~500K msg/s | ~2M msg/s |
| **8-thread throughput** | ~200K msg/s | ~800K msg/s |
| **Average enqueue latency** | ~500 ns | ~150 ns |
| **Memory overhead** | Low | Medium (pool) |
| **Queue full behavior** | Returns error | Back-pressure |

### Latency Distribution

```
async_writer:
  P50: 400 ns
  P99: 2 μs
  P99.9: 10 μs

high_performance_async_writer:
  P50: 100 ns
  P99: 500 ns
  P99.9: 2 μs
```

---

## Usage Examples

### Example 1: Basic async_writer Usage

```cpp
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/file_writer.h>

// Create async file writer
auto file_writer = std::make_unique<kcenon::logger::file_writer>("app.log");
auto async = std::make_unique<kcenon::logger::async_writer>(
    std::move(file_writer),
    10000,  // queue size
    std::chrono::seconds(5)  // flush timeout
);

// Start the async writer
async->start();

// Use the writer
async->write(log_level::info, "Hello async!", __FILE__, __LINE__, __func__,
             std::chrono::system_clock::now());

// Flush and stop
async->flush();
async->stop();
```

### Example 2: async_writer with Console Output

```cpp
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/console_writer.h>

// Wrap console writer for async operation
auto console = std::make_unique<kcenon::logger::console_writer>();
auto async_console = std::make_unique<kcenon::logger::async_writer>(
    std::move(console),
    5000  // smaller queue for console
);

async_console->start();
// ... use async_console ...
async_console->stop(true);  // flush remaining messages
```

### Example 3: Queue Full Handling

```cpp
auto async = std::make_unique<kcenon::logger::async_writer>(
    std::make_unique<kcenon::logger::file_writer>("app.log"),
    1000  // small queue for demonstration
);

async->start();

// Write with error handling
auto result = async->write(log_level::info, "Message", "", 0, "",
                           std::chrono::system_clock::now());

if (!result.is_ok()) {
    if (result.error().code() == logger_error_code::queue_full) {
        // Handle queue full - maybe write synchronously or drop
        std::cerr << "Async queue full, message dropped\n";
    }
}
```

### Example 4: Graceful Shutdown Pattern

```cpp
class application {
    std::unique_ptr<async_writer> logger_;

public:
    application() {
        auto writer = std::make_unique<file_writer>("app.log");
        logger_ = std::make_unique<async_writer>(std::move(writer));
        logger_->start();
    }

    ~application() {
        // Ensure all logs are flushed on shutdown
        if (logger_) {
            logger_->flush();  // Wait for queue to empty
            logger_->stop(true);  // Stop with flush
        }
    }

    void log(const std::string& message) {
        logger_->write(log_level::info, message, "", 0, "",
                       std::chrono::system_clock::now());
    }
};
```

---

## Best Practices

### 1. Queue Size Selection

```cpp
// Low volume (<1K msg/sec): Small queue
async_writer(writer, 1000);

// Medium volume (1K-10K msg/sec): Default
async_writer(writer, 10000);

// High volume (>10K msg/sec): Large queue
async_writer(writer, 100000);
```

### 2. Flush Timeout Configuration

```cpp
// Fast flush requirement (interactive apps)
async_writer(writer, 10000, std::chrono::seconds(1));

// Standard applications
async_writer(writer, 10000, std::chrono::seconds(5));

// Batch-oriented (background services)
async_writer(writer, 10000, std::chrono::seconds(30));
```

### 3. Error Handling

Always check the result of `write()` operations:

```cpp
auto result = async->write(...);
if (!result.is_ok()) {
    // Fallback: write synchronously or drop
}
```

### 4. Lifecycle Management

- Call `start()` before any writes
- Call `flush()` before `stop()` if you need all messages written
- Use RAII patterns for automatic cleanup

### 5. Thread Safety Considerations

- `async_writer` is thread-safe for writes
- Avoid calling `start()`/`stop()` concurrently
- Use external synchronization for configuration changes

---

## Migration Guide

### From Synchronous to async_writer

**Before:**
```cpp
auto writer = std::make_unique<file_writer>("app.log");
writer->write(level, message, file, line, func, timestamp);
```

**After:**
```cpp
auto writer = std::make_unique<file_writer>("app.log");
auto async = std::make_unique<async_writer>(std::move(writer));
async->start();
async->write(level, message, file, line, func, timestamp);
// Don't forget: async->stop() on shutdown
```

### Queue Full Migration

If experiencing queue full errors:

1. **Increase queue size:**
   ```cpp
   async_writer(writer, 50000);  // 5x default
   ```

2. **Add back-pressure handling:**
   ```cpp
   while (!async->write(...).is_ok()) {
       std::this_thread::sleep_for(std::chrono::milliseconds(1));
   }
   ```

3. **Consider high_performance_async_writer** for extreme cases

---

## Related Documentation

- [Custom Writers Guide](CUSTOM_WRITERS.md) - Creating custom writer implementations
- [Architecture Overview](ARCHITECTURE.md) - System architecture and design
- [Performance Benchmarks](../BENCHMARKS.md) - Detailed performance analysis
- [Thread System Integration](../integration/THREAD_SYSTEM.md) - Thread system integration

---

**Maintainer:** kcenon@naver.com
**Issue Tracker:** [GitHub Issues](https://github.com/kcenon/logger_system/issues)
