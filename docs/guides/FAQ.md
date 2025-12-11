# Logger System - Frequently Asked Questions

> **Version:** 0.1.0
> **Last Updated:** 2025-11-11
> **Audience:** Users, Developers

This FAQ addresses common questions about the logger_system, covering logging levels, writers, performance, and integration.

---

## Table of Contents

1. [General Questions](#general-questions)
2. [Logging Basics](#logging-basics)
3. [Log Levels](#log-levels)
4. [Writers](#writers)
5. [Performance](#performance)
6. [Integration](#integration)
7. [Advanced Topics](#advanced-topics)

---

## General Questions

### 1. What is the logger_system?

A high-performance, thread-safe logging system for C++20:
- **Multiple log levels** (trace, debug, info, warn, error, critical)
- **Multiple writers** (console, file, syslog, custom)
- **Asynchronous logging** for high throughput
- **Structured logging** with key-value pairs
- **Thread-safe** by default

```cpp
#include <logger/logger.hpp>

auto logger = logger::create("my_app");
logger->info("Application started");
logger->warn("Low memory", {{"free_mb", 128}});
logger->error("Connection failed", {{"host", "db.example.com"}});
```

---

### 2. What C++ standard is required?

**Required:** C++20

**Compiler Support:**
- GCC 11+
- Clang 14+
- MSVC 2019+

---

## Logging Basics

### 3. How do I create a logger?

```cpp
// Simple logger with default console writer
auto logger = logger::create("app");

// Logger with file writer
auto logger = logger::create("app", {
    logger_writer::file("/var/log/app.log")
});

// Logger with multiple writers
auto logger = logger::create("app", {
    logger_writer::console(),
    logger_writer::file("/var/log/app.log"),
    logger_writer::syslog("my_app")
});
```

---

### 4. How do I log messages?

```cpp
// Basic logging
logger->trace("Detailed trace information");
logger->debug("Debug information");
logger->info("Informational message");
logger->warn("Warning message");
logger->error("Error occurred");
logger->critical("Critical failure");

// Structured logging with key-value pairs
logger->info("User logged in", {
    {"user_id", user_id},
    {"ip_address", ip},
    {"timestamp", timestamp}
});

// Format strings
logger->info("Processing {} items in {} ms", count, duration);
```

---

## Log Levels

### 5. What log levels are available?

| Level | Use Case | Example |
|-------|----------|---------|
| **trace** | Very detailed debugging | Function entry/exit |
| **debug** | Debugging information | Variable values |
| **info** | Informational messages | Application events |
| **warn** | Warning messages | Deprecated API usage |
| **error** | Error messages | Operation failures |
| **critical** | Critical failures | System crashes |

---

### 6. How do I set the log level?

```cpp
// Set minimum log level
logger->set_level(log_level::info);

// Only info, warn, error, critical will be logged
logger->trace("Not logged");  // Suppressed
logger->debug("Not logged");  // Suppressed
logger->info("Logged");       // Logged
logger->error("Logged");      // Logged
```

---

### 7. Can I change log level at runtime?

```cpp
// Start with info level
auto logger = logger::create("app");
logger->set_level(log_level::info);

// Change to debug for troubleshooting
logger->set_level(log_level::debug);

// Change back to info
logger->set_level(log_level::info);

// From configuration
auto level = config.get<std::string>("log_level");
logger->set_level(parse_log_level(level));
```

---

## Writers

### 8. What writers are available?

**Console Writer:**
```cpp
auto writer = logger_writer::console();
```

**File Writer:**
```cpp
auto writer = logger_writer::file("/var/log/app.log", {
    .max_size = 10 * 1024 * 1024,  // 10 MB
    .max_files = 5,                 // Keep 5 rotated files
    .rotate_on_open = false
});
```

**Syslog Writer:**
```cpp
auto writer = logger_writer::syslog("my_app");
```

**Custom Writer:**
```cpp
class DatabaseWriter : public logger_writer_interface {
    void write(const log_entry& entry) override {
        db_->insert("logs", entry);
    }
};

auto writer = std::make_shared<DatabaseWriter>(db);
```

---

### 9. How do I use multiple writers?

```cpp
auto logger = logger::create("app", {
    logger_writer::console(),              // Dev/debugging
    logger_writer::file("/var/log/app.log"),  // Production
    logger_writer::syslog("my_app")        // System monitoring
});

// Different levels per writer
logger->add_writer(
    logger_writer::file("/var/log/errors.log"),
    log_level::error  // Only errors and critical
);
```

---

### 10. How does log rotation work?

```cpp
auto writer = logger_writer::file("/var/log/app.log", {
    .max_size = 10 * 1024 * 1024,  // Rotate at 10 MB
    .max_files = 7,                 // Keep 7 days of logs
    .compress = true                // Compress rotated files
});

// Results in:
// app.log (current)
// app.log.1.gz
// app.log.2.gz
// ...
// app.log.7.gz
```

---

## Performance

### 11. What is the performance?

**Benchmarks** (3.2 GHz Intel Core i7):

| Operation | Throughput | Latency |
|-----------|------------|---------|
| Synchronous logging | 500K logs/s | 2 μs |
| Asynchronous logging | 2M logs/s | 500 ns |
| Structured logging | 1M logs/s | 1 μs |

---

### 12. How do I enable asynchronous logging?

```cpp
// Async logger with background thread
auto logger = logger::create_async("app", {
    logger_writer::file("/var/log/app.log")
}, {
    .queue_size = 10000,
    .flush_interval = std::chrono::seconds(1)
});

// Log operations return immediately
logger->info("Fast logging");  // <500 ns
```

---

### 13. How do I optimize performance?

```cpp
// 1. Use async logging
auto logger = logger::create_async("app");

// 2. Set appropriate log level
logger->set_level(log_level::info);  // Skip debug/trace

// 3. Conditional logging
if (logger->should_log(log_level::debug)) {
    logger->debug("Expensive operation: {}", expensive_function());
}

// 4. Use structured logging (pre-formatted)
logger->info("event", {{"type", "user_login"}});
```

---

## Integration

### 14. How do I integrate with monitoring_system?

```cpp
#include <logger/adapters/monitoring_adapter.hpp>

// Send log metrics to monitoring
auto adapter = monitoring_adapter::create(monitoring);
logger->add_writer(adapter);

// Automatically tracks:
// - log_messages_total{level="error"}
// - log_messages_total{level="warn"}
```

---

### 15. How do I integrate with thread_system?

```cpp
#include <logger/adapters/thread_adapter.hpp>

// Use thread pool for async logging
auto executor = thread_system::create_pool(2);
auto logger = logger::create_async("app", writers, {
    .executor = executor
});
```

---

## Advanced Topics

### 16. Is it thread-safe?

**Yes**, all operations are thread-safe by default.

```cpp
// Multiple threads can log concurrently
std::vector<std::thread> threads;
for (int i = 0; i < 10; ++i) {
    threads.emplace_back([&logger, i]() {
        logger->info("Thread {}", i);
    });
}
```

---

### 17. How do I create custom log formats?

```cpp
class JsonFormatter : public log_formatter_interface {
    auto format(const log_entry& entry) -> std::string override {
        return json::object({
            {"timestamp", entry.timestamp},
            {"level", to_string(entry.level)},
            {"message", entry.message},
            {"fields", entry.fields}
        }).dump();
    }
};

logger->set_formatter(std::make_shared<JsonFormatter>());
```

---

### 18. How do I filter logs?

```cpp
// Filter by level
logger->set_level(log_level::warn);

// Filter by category
logger->set_filter([](const log_entry& entry) {
    return entry.category == "security";
});

// Complex filter
logger->set_filter([](const log_entry& entry) {
    return entry.level >= log_level::error ||
           entry.fields.contains("important");
});
```

---

### 19. How do I handle critical logs?

```cpp
// Critical logs trigger immediate actions
logger->on_critical([](const log_entry& entry) {
    // Send alert
    alert_system::notify("Critical log: " + entry.message);

    // Dump stack trace
    backtrace::dump();

    // Flush all logs immediately
    logger::flush_all();
});

logger->critical("System failure", {{"errno", errno}});
```

---

### 20. Where can I find more information?

**Documentation:**
- [Getting Started](GETTING_STARTED.md) - Quick start guide
- [Best Practices](BEST_PRACTICES.md) - Production patterns
- [Architecture](../architecture.md) - System design
- [API Reference](../API_REFERENCE.md) - Complete API

**Support:**
- [GitHub Issues](https://github.com/kcenon/logger_system/issues)
- [GitHub Discussions](https://github.com/kcenon/logger_system/discussions)

---

**Last Updated:** 2025-11-11
**Next Review:** 2026-02-11
