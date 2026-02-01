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

> **Note**: All scenarios below show complete working examples. If you encounter issues during migration, see [Troubleshooting](#troubleshooting).

#### Scenario 1: Simple File Writer

**Before (Deprecated)**:
```cpp
auto writer = std::make_unique<file_writer>("app.log");
```

**After (Recommended)**:
```cpp
auto writer = writer_builder().file("app.log").build();
```

**Migration Steps**:
1. Replace `std::make_unique<file_writer>()` with `writer_builder().file()`
2. Add `.build()` at the end
3. Test that logs are written correctly
4. **Common Issue**: If entries aren't being written, ensure writer is added to logger with `add_writer()`

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

**Migration Steps**:
1. If file rotation is critical, continue using `rotating_file_writer` (not deprecated yet)
2. Consider external rotation tools (logrotate on Linux, log management services)
3. Plan for future migration when decorator-based rotation becomes available
4. **Common Issue**: See [Flush not working](#flush-not-working) if rotation timing is incorrect

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

**Migration Steps**:
1. Replace innermost nesting (file_writer) with `.file()`
2. Add `.encrypted()` for encryption decorator
3. Add `.async()` for async decorator (outermost)
4. Call `.build()` to construct the writer chain
5. **Important**: Must call `start()` on async writer before use:
   ```cpp
   if (auto* async_w = dynamic_cast<async_writer*>(writer.get())) {
       async_w->start();
   }
   ```
6. **Common Issue**: If logs don't appear, check async writer is started. See [Troubleshooting](#common-issues)

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

**Migration Steps**:
1. Start with core writer: `.console()`
2. Add filter decorator: `.filtered(filter)` (applied first)
3. Add buffer decorator: `.buffered(size)` (applied after filter)
4. Build the chain: `.build()`
5. **Order Matters**: Filter → Buffer → Output (see [Composing Decorators](#composing-decorators))
6. **Common Issue**: Wrong decorator order. See [Writer not receiving entries](#writer-not-receiving-entries)

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

**Migration Steps**:
1. Identify all nested decorators (4 layers in this example)
2. Reverse the nesting order when using builder (innermost → outermost)
3. Core writer first: `.file()`
4. Add decorators in order: filter → buffer → format → async
5. Start async writer after building (if using async)
6. Test thoroughly with actual log messages
7. **Performance Tip**: This pattern provides ~4M msg/s throughput. See [Performance Guide](PERFORMANCE.md)
8. **Common Issue**: If formatting fails, ensure formatter is compatible. See [Entries not being formatted](#entries-not-being-formatted)

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
public:
    std::string get_name() const override { return "my_destination"; }

protected:
    common::VoidResult write_entry_impl(const log_entry& entry) override {
        return send_to_my_destination(entry);
    }

    common::VoidResult flush_impl() override {
        return flush_my_destination();
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

**Migration Steps**:
1. **Separate concerns**: Extract destination-specific I/O from cross-cutting logic
2. **Create leaf writer**: Inherit from `thread_safe_writer` or `sync_writer_tag`
3. **Implement only I/O methods**: `write_entry_impl()`, `flush_impl()`, `get_name()`
4. **Remove duplicated logic**: Delete filtering, buffering, formatting from custom class
5. **Use decorators**: Compose with `.filtered()`, `.buffered()`, `.async()` as needed
6. **Benefits**: 70% less code, independently testable, reusable
7. **Common Issue**: If custom writer doesn't compile, ensure it implements `log_writer_interface`. See [Best Practices](#1-keep-leaf-writers-simple)

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

**Symptom**: Log entries not appearing in output file or console.

**Possible Causes**:

1. **Decorator order is wrong** - Filters should be innermost:

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

// Using builder (automatic correct order):
auto correct = writer_builder()
    .file("app.log")
    .filtered(filter)  // Applied first
    .buffered(100)     // Applied second
    .build();
```

2. **Writer not added to logger**:

```cpp
auto writer = writer_builder().file("app.log").build();
// Must add to logger!
logger.add_writer("main", std::move(writer));
```

3. **Async writer not started**:

```cpp
auto writer = writer_builder().file("app.log").async().build();
// Must start async writer!
if (auto* async_w = dynamic_cast<async_writer*>(writer.get())) {
    async_w->start();
}
logger.add_writer("main", std::move(writer));
```

4. **Filter is too restrictive**:

```cpp
// This filter blocks everything below ERROR level
auto filter = std::make_unique<level_filter>(log_level::error);
auto writer = writer_builder()
    .console()
    .filtered(std::move(filter))
    .build();

// INFO and DEBUG messages won't appear!
logger.log(log_level::info, "This won't show");  // Filtered out
logger.log(log_level::error, "This will show");  // Passes filter
```

#### "Entries not being formatted"

**Symptom**: Log entries appear in plain text instead of JSON/custom format.

**Possible Causes**:

1. **Formatter applied in wrong order**:

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

// Using builder (handles order automatically):
auto correct = writer_builder()
    .file("app.json")
    .buffered(100)
    .formatted(std::make_unique<json_formatter>())
    .build();
```

2. **Formatter not provided**:

```cpp
// Missing formatter - will use default plain text
auto writer = writer_builder().file("app.json").build();

// Add formatter for JSON output
auto writer = writer_builder()
    .file("app.json")
    .formatted(std::make_unique<json_formatter>())
    .build();
```

3. **Incompatible formatter for writer type**:

```cpp
// Some formatters may not support all entry types
// Check formatter documentation for compatibility
```

#### "Flush not working"

**Symptom**: Logs not appearing immediately, delayed until program exit.

**Possible Causes**:

1. **Flushing wrong writer**:

```cpp
auto inner = std::make_unique<file_writer>("app.log");
auto buffered = std::make_unique<buffered_writer>(std::move(inner), config);
auto outermost = std::make_unique<formatted_writer>(std::move(buffered), formatter);

// Wrong: Flushing reference to moved writer (undefined behavior)
// inner->flush();  // Don't do this!

// Right: Flush the outermost decorator
outermost->flush();
```

2. **Not calling flush() at all**:

```cpp
auto writer = writer_builder().file("app.log").buffered(100).build();
logger.add_writer("main", std::move(writer));

// Logs buffered but not flushed
logger.log(log_level::info, "Message");

// Must flush before exit!
logger.flush();  // This flushes all writers
```

3. **Async writer not stopped properly**:

```cpp
auto writer = writer_builder().file("app.log").async().build();
if (auto* async_w = dynamic_cast<async_writer*>(writer.get())) {
    async_w->start();
}
logger.add_writer("main", std::move(writer));

// Before program exit:
logger.flush();  // Flush buffered entries

// Get async writer reference and stop it
auto& writer_ref = logger.get_writer("main");
if (auto* async_w = dynamic_cast<async_writer*>(&writer_ref)) {
    async_w->stop();  // Processes remaining queue
}
```

4. **Buffer size too large**:

```cpp
// Very large buffer - may never fill up in short-running programs
auto writer = writer_builder()
    .file("app.log")
    .buffered(10000)  // Too large for a few log messages
    .build();

// Solution: Use smaller buffer or explicit flush
auto writer = writer_builder()
    .file("app.log")
    .buffered(100)  // More reasonable
    .build();
```

#### "Compilation errors with custom writers"

**Symptom**: Custom writer doesn't compile after migration to decorator pattern.

**Possible Causes**:

1. **Missing interface methods**:

```cpp
// Error: abstract class cannot be instantiated
class my_writer : public thread_safe_writer {
    // Missing required methods!
};

// Solution: Implement all required methods
class my_writer : public thread_safe_writer {
public:
    std::string get_name() const override { return "my_writer"; }

protected:
    common::VoidResult write_entry_impl(const log_entry& entry) override {
        // Implementation
        return {};
    }

    common::VoidResult flush_impl() override {
        // Implementation
        return {};
    }
};
```

2. **Wrong base class**:

```cpp
// Wrong: Using old base_writer class
class my_writer : public base_writer { ... };

// Right: Use thread_safe_writer or implement log_writer_interface
class my_writer : public thread_safe_writer { ... };
```

3. **Incorrect method signatures**:

```cpp
// Wrong: Missing const qualifier
std::string get_name() { return "my_writer"; }

// Right: Match interface exactly
std::string get_name() const override { return "my_writer"; }
```

#### "Performance regression after migration"

**Symptom**: Logging is slower after switching to decorator pattern.

**Possible Causes**:

1. **Too many decorator layers**:

```cpp
// Avoid excessive nesting
auto writer = writer_builder()
    .file("app.log")
    .filtered(filter1)
    .filtered(filter2)  // Each filter adds overhead
    .filtered(filter3)
    .buffered(100)
    .buffered(50)       // Multiple buffers inefficient
    .async()
    .build();

// Better: Use composite filter and single buffer
auto composite_filter = std::make_unique<composite_filter>(logic_type::AND);
composite_filter->add_filter(std::move(filter1));
composite_filter->add_filter(std::move(filter2));
composite_filter->add_filter(std::move(filter3));

auto writer = writer_builder()
    .file("app.log")
    .filtered(std::move(composite_filter))
    .buffered(100)
    .async()
    .build();
```

2. **Wrong decorator order**:

```cpp
// Inefficient: Buffering before filtering
auto slow = writer_builder()
    .file("app.log")
    .buffered(100)      // Buffers everything
    .filtered(filter)   // Then filters (wasted buffer space)
    .build();

// Efficient: Filtering before buffering
auto fast = writer_builder()
    .file("app.log")
    .filtered(filter)   // Filter first
    .buffered(100)      // Only buffer passing entries
    .build();
```

3. **Async not used for high-throughput scenarios**:

```cpp
// Synchronous writing blocks caller
auto slow = writer_builder()
    .file("app.log")
    .buffered(100)
    .build();

// Async writing is non-blocking
auto fast = writer_builder()
    .file("app.log")
    .buffered(100)
    .async(20000)
    .build();
```

### Migration Checklist

Before starting migration:
- [ ] Read this entire migration guide
- [ ] Review [decorator_usage.cpp](../../examples/decorator_usage.cpp) examples
- [ ] Identify all existing writer usage in codebase

During migration:
- [ ] Identify cross-cutting concerns in existing writers
- [ ] Separate destination logic from filtering/buffering/formatting
- [ ] Create simple leaf writers inheriting from `thread_safe_writer`
- [ ] Replace manual nesting with `writer_builder()`
- [ ] Use built-in decorators for common functionality
- [ ] Create custom decorators only for unique cross-cutting needs
- [ ] Start async writers with `.start()` where applicable

After migration:
- [ ] Update tests to verify decorator composition
- [ ] Test with actual log messages (info, warning, error levels)
- [ ] Verify flush() works correctly before program exit
- [ ] Benchmark to ensure no performance regression
- [ ] Update documentation for your project
- [ ] Remove deprecated code once validated

### Getting Help

If you encounter issues not covered in this guide:

1. Check [examples/decorator_usage.cpp](../../examples/decorator_usage.cpp) for comprehensive examples
2. Review [writer_builder_example.cpp](../../examples/writer_builder_example.cpp) for builder API patterns
3. See [Best Practices](BEST_PRACTICES.md) for general guidance
4. Check [FAQ](FAQ.md) for common questions
5. File an issue on [GitHub](https://github.com/kcenon/logger_system/issues) with:
   - Current code (manual nesting)
   - Expected behavior
   - Error messages or compilation errors
   - Platform and compiler version

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
