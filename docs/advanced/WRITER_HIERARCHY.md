> **Language:** **English** | [한국어](WRITER_HIERARCHY.kr.md)

# Writer Hierarchy and Categories

This document describes the logger system's writer hierarchy and category classification system introduced in v1.4.0.

## Overview

Writers are categorized based on their operational characteristics using a type-safe tag system. This helps users understand writer behavior and make informed decisions about which writer to use.

## Category System

### Writer Categories

| Category | Description | Examples |
|----------|-------------|----------|
| **Synchronous** | Blocking I/O operations, immediate output | `console_writer`, `file_writer` |
| **Asynchronous** | Non-blocking, queue-based background processing | `async_writer`, `batch_writer`, `network_writer` |
| **Decorator** | Wraps another writer to add functionality | `async_writer`, `critical_writer`, `encrypted_writer` |
| **Composite** | Combines multiple outputs or pipeline stages | `composite_writer`, `hybrid_writer` |

### Category Tags

Each writer inherits from one or more category tags to indicate its behavior:

```cpp
// Category tags defined in writer_category.h
struct sync_writer_tag { ... };
struct async_writer_tag { ... };
struct decorator_writer_tag { ... };
struct composite_writer_tag { ... };
```

## Complete Writer Hierarchy

```
log_writer_interface (interface)
  │
  ├── base_writer (abstract)
  │     │
  │     ├── thread_safe_writer (abstract, Template Method Pattern)
  │     │     ├── console_writer [sync_writer_tag]
  │     │     ├── file_writer [sync_writer_tag]
  │     │     │     └── rotating_file_writer (inherits sync from file_writer)
  │     │
  │     ├── async_writer [async_writer_tag, decorator_writer_tag]
  │     ├── batch_writer [async_writer_tag, decorator_writer_tag]
  │     ├── critical_writer [decorator_writer_tag]
  │     ├── encrypted_writer [decorator_writer_tag]
  │     ├── network_writer [async_writer_tag]
  │     ├── otlp_writer [async_writer_tag]
  │     └── hybrid_writer [composite_writer_tag, decorator_writer_tag]
  │
  └── composite_writer [composite_writer_tag]
        (Pipeline Pattern: formatter + sink)
```

## Category Characteristics

### Synchronous Writers (sync_writer_tag)

**Characteristics:**
- Immediate, guaranteed output when `write()` returns
- Blocking operation (caller waits for I/O completion)
- Thread-safe via mutex (through `thread_safe_writer`)

**Best for:**
- Low-volume, critical logging where immediate output is required
- Debugging and development
- Applications where latency is not critical

**Writers:** `console_writer`, `file_writer`, `rotating_file_writer`

### Asynchronous Writers (async_writer_tag)

**Characteristics:**
- Non-blocking operation (returns immediately)
- Higher throughput potential
- Requires explicit `flush()` for guaranteed delivery
- May lose messages on crash (depends on configuration)

**Best for:**
- High-volume logging scenarios (>10K msg/sec)
- Applications where logging latency matters
- Network destinations with variable latency

**Writers:** `async_writer`, `batch_writer`, `network_writer`, `otlp_writer`

### Decorator Writers (decorator_writer_tag)

**Characteristics:**
- Wraps another writer to add functionality
- Transparent to the logging system
- Can be stacked (decorator wrapping decorator)
- Follows the Decorator Pattern

**Best for:**
- Adding encryption to any writer
- Converting sync writers to async
- Adding critical message handling
- Batching writes for performance

**Writers:** `async_writer`, `batch_writer`, `critical_writer`, `encrypted_writer`, `hybrid_writer`

### Composite Writers (composite_writer_tag)

**Characteristics:**
- Combines multiple components
- May use Pipeline or Composite pattern
- Single entry point, multiple outputs possible

**Best for:**
- Custom formatting + output combinations
- Multiple simultaneous outputs
- Complex logging pipelines

**Writers:** `composite_writer`, `hybrid_writer`

## Using Category Information

### Compile-Time Type Traits

```cpp
#include <kcenon/logger/interfaces/writer_category.h>

// Check if a writer is synchronous
static_assert(is_sync_writer_v<console_writer>);
static_assert(!is_sync_writer_v<async_writer>);

// Check if a writer is a decorator
static_assert(is_decorator_writer_v<async_writer>);
static_assert(is_decorator_writer_v<encrypted_writer>);

// Check if a writer is composite
static_assert(is_composite_writer_v<composite_writer>);
```

### C++20 Concepts

```cpp
// Function that only accepts sync writers
template<SyncWriter W>
void add_sync_writer(std::unique_ptr<W> writer);

// Function that only accepts async writers
template<AsyncWriter W>
void add_async_writer(std::unique_ptr<W> writer);
```

### Runtime Category Query

```cpp
// Get the primary category of a writer type
constexpr writer_category cat = get_writer_category<async_writer>();
// cat == writer_category::decorator (decorator takes priority over async)

// Convert category to string
const char* name = to_string(writer_category::synchronous);
// name == "synchronous"
```

## Design Patterns Used

### Template Method Pattern (thread_safe_writer)

`thread_safe_writer` uses the Template Method pattern to provide automatic thread-safety:

```cpp
// Public method handles locking
common::VoidResult write(...) final {
    std::lock_guard<std::mutex> lock(mutex_);
    return write_impl(...);  // Call protected implementation
}

// Derived classes implement this
virtual common::VoidResult write_impl(...) = 0;
```

### Decorator Pattern

Decorator writers wrap other writers to add functionality:

```cpp
auto file = std::make_unique<file_writer>("app.log");
auto async = std::make_unique<async_writer>(std::move(file));
auto critical = std::make_unique<critical_writer>(std::move(async));
// Result: critical -> async -> file
```

### Pipeline Pattern (composite_writer)

`composite_writer` composes a formatter and sink:

```cpp
auto writer = make_composite_writer(
    std::make_unique<json_formatter>(),
    std::make_unique<file_sink>("app.json")
);
// entry -> formatter.format() -> sink.write_raw()
```

## Related Documentation

- [Writer Selection Guide](WRITER_SELECTION_GUIDE.md) - How to choose the right writer
- [Custom Writers Guide](CUSTOM_WRITERS.md) - Creating custom writers
- [Async Writers Guide](ASYNC_WRITERS.md) - Async writer patterns and performance

---

*Last Updated: 2025-01-11*
*Version: 1.4.0* (Phase 2.0 - Writer Hierarchy Clarification)
