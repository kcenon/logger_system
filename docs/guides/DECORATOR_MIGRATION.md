# Decorator Pattern Writer Migration Guide

**Version**: 4.0.0
**Last Updated**: 2026-01-22

## Table of Contents
1. [Overview](#overview)
2. [New Architecture](#new-architecture)
3. [Built-in Decorators](#built-in-decorators)
4. [Migrating Custom Writers](#migrating-custom-writers)
5. [Composing Decorators](#composing-decorators)
6. [Deprecation Timeline and Legacy Patterns](#deprecation-timeline-and-legacy-patterns)
7. [Best Practices](#best-practices)
8. [Troubleshooting](#troubleshooting)

---

## Overview

Version 4.0.0 introduces a decorator pattern-based writer architecture that separates cross-cutting concerns (filtering, buffering, formatting) from destination-specific logic (console, file, network output).

### Why Decorator Pattern?

| Problem (v3.x) | Solution (v4.0) |
|----------------|-----------------|
| ~70% code duplication across writers | Each concern implemented once in decorators |
| Writers had multiple responsibilities | Single Responsibility Principle enforced |
| Hard to test individual features | Each decorator independently testable |
| Tight coupling between concerns | Loose coupling via composition |

### Key Components

```
┌─────────────────────────────────────────────────────────────┐
│                    Decorator Writers                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐          │
│  │ formatted   │→ │ buffered    │→ │ filtered    │→ [Leaf]  │
│  └─────────────┘  └─────────────┘  └─────────────┘          │
└─────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────┐
│                      Leaf Writers                            │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐          │
│  │ console     │  │ file        │  │ network     │          │
│  └─────────────┘  └─────────────┘  └─────────────┘          │
└─────────────────────────────────────────────────────────────┘
```

---

## New Architecture

### Base Classes

#### `log_writer_interface`

The core interface that all writers must implement:

```cpp
class log_writer_interface {
public:
    virtual common::VoidResult write(const log_entry& entry) = 0;
    virtual common::VoidResult flush() = 0;
    virtual std::string get_name() const = 0;
    virtual bool is_healthy() const = 0;
    virtual ~log_writer_interface() = default;
};
```

#### `decorator_writer_base`

Abstract base class for decorators that eliminates boilerplate:

```cpp
class decorator_writer_base : public log_writer_interface, public decorator_writer_tag {
public:
    explicit decorator_writer_base(std::unique_ptr<log_writer_interface> wrapped,
                                   std::string_view decorator_name);

    // Must be implemented by derived classes
    common::VoidResult write(const log_entry& entry) override = 0;

    // Default implementations that delegate to wrapped writer
    common::VoidResult flush() override;
    std::string get_name() const override;
    bool is_healthy() const override;

protected:
    log_writer_interface& wrapped() noexcept;
    const log_writer_interface& wrapped() const noexcept;
};
```

---

## Built-in Decorators

### `filtered_writer`

Filters log entries before passing to the wrapped writer:

```cpp
#include <kcenon/logger/writers/filtered_writer.h>
#include <kcenon/logger/filters/log_filter.h>

// Only write errors and above
auto writer = std::make_unique<filtered_writer>(
    std::make_unique<file_writer>("errors.log"),
    std::make_unique<filters::level_filter>(log_level::error)
);

// Use composite filter for complex logic
auto composite = std::make_unique<filters::composite_filter>(
    filters::composite_filter::logic_type::AND
);
composite->add_filter(std::make_unique<filters::level_filter>(log_level::info));
composite->add_filter(std::make_unique<filters::regex_filter>("security", true));

auto security_writer = std::make_unique<filtered_writer>(
    std::make_unique<file_writer>("security.log"),
    std::move(composite)
);
```

### `buffered_writer`

Buffers log entries before writing to improve performance:

```cpp
#include <kcenon/logger/writers/buffered_writer.h>

buffered_writer::config config;
config.max_buffer_size = 100;  // Flush after 100 entries
config.flush_interval = std::chrono::seconds(5);  // Or flush every 5 seconds

auto writer = std::make_unique<buffered_writer>(
    std::make_unique<file_writer>("app.log"),
    config
);
```

### `formatted_writer`

Applies formatting before writing:

```cpp
#include <kcenon/logger/writers/formatted_writer.h>
#include <kcenon/logger/formatters/json_formatter.h>

auto writer = std::make_unique<formatted_writer>(
    std::make_unique<file_writer>("app.json"),
    std::make_unique<json_formatter>()
);
```

---

## Migrating Custom Writers

### Before (v3.x): Monolithic Writer

```cpp
// Old approach: everything in one class
class my_custom_writer : public base_writer {
public:
    common::VoidResult write(const log_entry& entry) override {
        // Check filter (duplicated logic)
        if (!should_log(entry)) return {};

        // Format message (duplicated logic)
        auto formatted = format_entry(entry);

        // Buffer if needed (duplicated logic)
        buffer_.push_back(formatted);
        if (buffer_.size() >= buffer_size_) {
            flush_buffer();
        }

        // Actual custom output
        send_to_my_destination(formatted);
        return {};
    }

private:
    std::vector<std::string> buffer_;
    size_t buffer_size_;
};
```

### After (v4.0): Focused Leaf Writer

```cpp
// New approach: only destination-specific logic
class my_custom_writer : public thread_safe_writer, public sync_writer_tag {
public:
    explicit my_custom_writer(const std::string& destination)
        : destination_(destination) {}

    std::string get_name() const override { return "my_custom"; }

protected:
    common::VoidResult write_entry_impl(const log_entry& entry) override {
        // Only the custom output logic
        return send_to_my_destination(entry);
    }

    common::VoidResult flush_impl() override {
        return flush_my_destination();
    }

private:
    std::string destination_;
};

// Compose with decorators for additional functionality
auto writer = std::make_unique<formatted_writer>(
    std::make_unique<buffered_writer>(
        std::make_unique<filtered_writer>(
            std::make_unique<my_custom_writer>("my-endpoint"),
            std::make_unique<level_filter>(log_level::info)
        ),
        buffered_writer::config{.max_buffer_size = 50}
    ),
    std::make_unique<json_formatter>()
);
```

### Creating a Custom Decorator

If you need custom cross-cutting behavior, extend `decorator_writer_base`:

```cpp
class rate_limiting_writer : public decorator_writer_base {
public:
    rate_limiting_writer(std::unique_ptr<log_writer_interface> wrapped,
                         size_t max_per_second)
        : decorator_writer_base(std::move(wrapped), "rate_limited"),
          max_per_second_(max_per_second) {}

    common::VoidResult write(const log_entry& entry) override {
        auto now = std::chrono::steady_clock::now();

        // Reset counter every second
        if (now - window_start_ >= std::chrono::seconds(1)) {
            window_start_ = now;
            count_ = 0;
        }

        // Drop if rate exceeded
        if (count_ >= max_per_second_) {
            return {};  // Silently drop
        }

        ++count_;
        return wrapped().write(entry);
    }

private:
    size_t max_per_second_;
    size_t count_ = 0;
    std::chrono::steady_clock::time_point window_start_;
};
```

---

## Composing Decorators

### Order Matters

Decorators are applied in reverse order of construction (innermost first):

```cpp
// Execution order: filter → buffer → format → file
auto writer = std::make_unique<formatted_writer>(      // 4. Format output
    std::make_unique<buffered_writer>(                  // 3. Buffer entries
        std::make_unique<filtered_writer>(              // 2. Filter entries
            std::make_unique<file_writer>("app.log"),   // 1. Write to file
            std::make_unique<level_filter>(log_level::info)
        ),
        buffered_writer::config{.max_buffer_size = 100}
    ),
    std::make_unique<json_formatter>()
);
```

### Recommended Order

1. **Filtering** (earliest) - Drop unwanted entries before any processing
2. **Buffering** - Batch entries for efficiency
3. **Formatting** - Transform entries just before output
4. **Output** (leaf writer) - Write to destination

### Using `writer_builder` (Recommended)

**Since v4.1.0**, the `writer_builder` provides a fluent API for composing writers with decorators:

```cpp
#include <kcenon/logger/builders/writer_builder.h>

// Simple file writer
auto writer = writer_builder()
    .file("app.log")
    .build();

// Production setup: async + buffered
auto production_writer = writer_builder()
    .file("app.log")
    .buffered(500)       // Buffer 500 entries
    .async(20000)        // Queue size 20000
    .build();

// Filtered console (warnings and above)
auto console_writer = writer_builder()
    .console()
    .filtered(std::make_unique<level_filter>(log_level::warning))
    .build();

// Encrypted logging with buffering
#ifdef LOGGER_WITH_ENCRYPTION
auto secure_writer = writer_builder()
    .file("secure.log.enc")
    .encrypted(key)
    .buffered(100)
    .async()
    .build();
#endif
```

**Benefits of `writer_builder`:**
- **Readable**: Fluent API is self-documenting
- **Type-safe**: Compile-time validation
- **Consistent order**: Decorators applied in optimal order
- **Error-proof**: No manual nesting required

See [writer_builder_example.cpp](../../examples/writer_builder_example.cpp) for complete examples.

### Using `logger_builder`

The logger builder accepts writers created by `writer_builder`:

```cpp
auto logger = logger_builder()
    .add_writer("file", writer_builder().file("app.log").build())
    .add_level_filter(log_level::info)   // Adds filtered_writer
    .with_batch_writing(true)             // Wraps with batch_writer
    .with_formatter(std::make_unique<json_formatter>())  // Adds formatted_writer
    .build();
```

---

## Deprecation Timeline and Legacy Patterns

### Deprecated Writer Patterns

The following legacy writer patterns are **deprecated** as of v4.1.0:

| Legacy Pattern | Status | Replacement |
|----------------|--------|-------------|
| `rotating_file_writer` | Deprecated (v4.1) | `writer_builder().file().buffered()` |
| `async_file_writer` | Deprecated (v4.1) | `writer_builder().file().async()` |
| Combined writer classes | Deprecated (v4.1) | `writer_builder()` with decorator chain |

### Timeline

| Version | Status | Action Required |
|---------|--------|-----------------|
| **4.0.0** | Decorator pattern introduced | Optional: Begin planning migration |
| **4.1.0** | `writer_builder` available | **Recommended**: Migrate to builder API |
| **4.1.0** | Deprecation warnings added | Legacy classes emit compile warnings |
| **5.0.0** | Legacy classes removed | **Required**: Must use decorator pattern |

### Migration Paths

#### Scenario 1: Simple File Writer

**Before (Deprecated)**:
```cpp
auto writer = std::make_unique<file_writer>("app.log");
```

**After (Recommended)**:
```cpp
auto writer = writer_builder().file("app.log").build();
```

#### Scenario 2: Rotating File Writer

**Before (Deprecated)**:
```cpp
auto writer = std::make_unique<rotating_file_writer>(
    "app.log",
    10 * 1024 * 1024,  // 10MB max size
    5                   // 5 backup files
);
```

**After (Recommended)**:
```cpp
// Note: rotating_file_writer is specialized, not yet fully replaced by decorators
// For now, continue using rotating_file_writer but expect future decorator-based replacement
auto writer = std::make_unique<rotating_file_writer>("app.log", 10 * 1024 * 1024, 5);

// Alternative: Use external log rotation tools (logrotate, etc.)
auto writer = writer_builder().file("app.log").build();
```

#### Scenario 3: Async + Encrypted Writer

**Before (Manual Nesting)**:
```cpp
auto writer = std::make_unique<async_writer>(
    std::make_unique<encrypted_writer>(
        std::make_unique<file_writer>("secure.log.enc"),
        key
    ),
    10000  // queue size
);
```

**After (Builder API)**:
```cpp
auto writer = writer_builder()
    .file("secure.log.enc")
    .encrypted(key)
    .async(10000)
    .build();
```

#### Scenario 4: Buffered + Filtered Writer

**Before (Manual Nesting)**:
```cpp
auto writer = std::make_unique<buffered_writer>(
    std::make_unique<filtered_writer>(
        std::make_unique<console_writer>(),
        std::make_unique<level_filter>(log_level::warning)
    ),
    buffered_writer::config{.max_buffer_size = 100}
);
```

**After (Builder API)**:
```cpp
auto writer = writer_builder()
    .console()
    .filtered(std::make_unique<level_filter>(log_level::warning))
    .buffered(100)
    .build();
```

#### Scenario 5: Multiple Decorators (Production Setup)

**Before (Complex Nesting)**:
```cpp
auto writer = std::make_unique<formatted_writer>(
    std::make_unique<async_writer>(
        std::make_unique<buffered_writer>(
            std::make_unique<filtered_writer>(
                std::make_unique<file_writer>("production.log"),
                std::make_unique<level_filter>(log_level::info)
            ),
            buffered_writer::config{.max_buffer_size = 500}
        ),
        20000
    ),
    std::make_unique<json_formatter>()
);
```

**After (Fluent Builder)**:
```cpp
auto writer = writer_builder()
    .file("production.log")
    .filtered(std::make_unique<level_filter>(log_level::info))
    .buffered(500)
    .formatted(std::make_unique<json_formatter>())
    .async(20000)
    .build();
```

#### Scenario 6: Custom Writer Integration

**Before (Inheritance)**:
```cpp
class my_combined_writer : public base_writer {
    // 100+ lines of filtering, buffering, formatting logic
    // Mixed with custom destination logic
};
```

**After (Composition)**:
```cpp
// 1. Create simple leaf writer (20-30 lines)
class my_destination_writer : public thread_safe_writer {
protected:
    common::VoidResult write_entry_impl(const log_entry& entry) override {
        return send_to_my_destination(entry);
    }
};

// 2. Compose with decorators
auto writer = writer_builder()
    .custom(std::make_unique<my_destination_writer>())
    .filtered(filter)
    .buffered(100)
    .async()
    .build();
```

### Support Policy

- **v4.x series**: Full backward compatibility, deprecation warnings only
- **v5.0.0**: Legacy classes removed, builder API required
- **Migration support**: See [examples/writer_builder_example.cpp](../../examples/writer_builder_example.cpp)

---

## Best Practices

### 1. Keep Leaf Writers Simple

Leaf writers should only handle destination-specific I/O:

```cpp
// Good: Focused on one thing
class kafka_writer : public thread_safe_writer {
protected:
    common::VoidResult write_entry_impl(const log_entry& entry) override {
        return kafka_client_.send(topic_, entry.formatted_message());
    }
};

// Bad: Too many responsibilities
class kafka_writer : public thread_safe_writer {
protected:
    common::VoidResult write_entry_impl(const log_entry& entry) override {
        if (entry.level() < min_level_) return {};  // Should be filtered_writer
        auto json = to_json(entry);                  // Should be formatted_writer
        buffer_.push(json);                          // Should be buffered_writer
        // ...
    }
};
```

### 2. Use Category Tags

Mark your writers with appropriate category tags:

```cpp
class my_sync_writer : public thread_safe_writer, public sync_writer_tag { ... };
class my_async_writer : public queued_writer_base, public async_writer_tag { ... };
class my_decorator : public decorator_writer_base, public decorator_writer_tag { ... };
```

### 3. Handle Errors Gracefully

Decorators should propagate errors from wrapped writers:

```cpp
common::VoidResult write(const log_entry& entry) override {
    // Do pre-processing
    auto result = wrapped().write(entry);
    if (result.is_err()) {
        return result;  // Propagate error
    }
    // Do post-processing
    return {};
}
```

### 4. Ensure Thread Safety

If your decorator maintains state, protect it:

```cpp
class stateful_decorator : public decorator_writer_base {
    common::VoidResult write(const log_entry& entry) override {
        std::lock_guard<std::mutex> lock(mutex_);
        // Access mutable state safely
        return wrapped().write(entry);
    }
private:
    std::mutex mutex_;
    // mutable state...
};
```

---

## Troubleshooting

### Common Issues

#### "Writer not receiving entries"

Check decorator composition order. Filters should be innermost:

```cpp
// Wrong: Filter after buffer means filtered entries still buffered
auto wrong = std::make_unique<filtered_writer>(
    std::make_unique<buffered_writer>(...),
    filter
);

// Right: Filter before buffer
auto right = std::make_unique<buffered_writer>(
    std::make_unique<filtered_writer>(..., filter),
    config
);
```

#### "Entries not being formatted"

Ensure `formatted_writer` wraps the leaf writer:

```cpp
// Wrong: Formatting lost because it's wrapped by buffer
auto wrong = std::make_unique<buffered_writer>(
    std::make_unique<formatted_writer>(...),
    config
);

// Right: Format just before output
auto right = std::make_unique<formatted_writer>(
    std::make_unique<buffered_writer>(..., config),
    formatter
);
```

#### "Flush not working"

Call `flush()` on the outermost decorator:

```cpp
auto outermost = std::make_unique<formatted_writer>(
    std::make_unique<buffered_writer>(inner, config),
    formatter
);

// This flushes all layers
outermost->flush();
```

### Migration Checklist

- [ ] Identify cross-cutting concerns in existing writers
- [ ] Separate destination logic from filtering/buffering/formatting
- [ ] Create simple leaf writers inheriting from `thread_safe_writer`
- [ ] Use built-in decorators for common functionality
- [ ] Create custom decorators only for unique cross-cutting needs
- [ ] Update tests to verify decorator composition
- [ ] Benchmark to ensure no performance regression

---

## Related Documentation

- [writer_builder Example](../../examples/writer_builder_example.cpp) - Complete usage examples
- [API Reference - Writers](../API_REFERENCE.md#writers)
- [Architecture Overview](../ARCHITECTURE.md)
- [Changelog](../CHANGELOG.md) - Version history and breaking changes
- [Best Practices](BEST_PRACTICES.md)

---

*Part of Logger System v4.1+ documentation*
*Last updated: 2026-02-01 for v4.1.0 deprecation timeline*
