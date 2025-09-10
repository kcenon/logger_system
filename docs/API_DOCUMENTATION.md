# Logger System API Documentation

## Table of Contents
1. [Introduction](#introduction)
2. [Quick Start](#quick-start)
3. [Core Components](#core-components)
4. [Configuration](#configuration)
5. [Advanced Features](#advanced-features)
6. [Best Practices](#best-practices)
7. [Performance Optimization](#performance-optimization)
8. [Migration Guide](#migration-guide)
9. [API Reference](#api-reference)

## Introduction

The Logger System is a high-performance, thread-safe logging library designed for modern C++20 applications. It provides flexible configuration, multiple output targets, and advanced features like asynchronous logging and batch processing.

### Key Features
- **Thread-safe** logging with lock-free options
- **Multiple writers** (console, file, async, batch)
- **Flexible formatting** with customizable patterns
- **Performance optimized** with SSO and batch processing
- **Modular design** with optional dependencies
- **Comprehensive filtering** by level, category, and custom rules

## Quick Start

### Basic Usage

```cpp
#include "logger_system/logger.h"

int main() {
    // Create a simple logger
    auto logger = logger_module::logger_builder()
        .with_console_writer()
        .with_min_level(logger_module::log_level::info)
        .build();
    
    // Log messages
    logger->info("Application started");
    logger->debug("Debug information", {{"user", "john"}});
    logger->error("An error occurred", {{"code", 404}});
    
    return 0;
}
```

### File Logging

```cpp
#include "logger_system/logger.h"
#include "logger_system/writers/file_writer.h"

int main() {
    auto logger = logger_module::logger_builder()
        .with_file_writer("app.log")
        .with_rotation(10 * 1024 * 1024)  // 10MB rotation
        .with_max_files(5)                 // Keep 5 backup files
        .build();
    
    logger->info("Logging to file");
    return 0;
}
```

## Core Components

### Logger

The main interface for logging messages. Thread-safe and supports multiple severity levels.

```cpp
class logger {
public:
    // Log methods for different severity levels
    void trace(const std::string& message, const fields_type& fields = {});
    void debug(const std::string& message, const fields_type& fields = {});
    void info(const std::string& message, const fields_type& fields = {});
    void warning(const std::string& message, const fields_type& fields = {});
    void error(const std::string& message, const fields_type& fields = {});
    void critical(const std::string& message, const fields_type& fields = {});
    
    // Generic log method
    void log(log_level level, const std::string& message, 
             const fields_type& fields = {});
    
    // Structured logging
    void log_structured(const log_entry& entry);
};
```

### Writers

Writers determine where log messages are output.

#### Console Writer
```cpp
auto writer = std::make_unique<console_writer>();
writer->set_colored_output(true);  // Enable colored output
logger->add_writer(std::move(writer));
```

#### File Writer
```cpp
auto writer = std::make_unique<file_writer>("app.log");
writer->set_rotation_size(10 * 1024 * 1024);  // 10MB
writer->set_max_files(5);
logger->add_writer(std::move(writer));
```

#### Async Writer
```cpp
auto file_writer = std::make_unique<file_writer>("async.log");
auto async_writer = std::make_unique<async_writer>(std::move(file_writer));
async_writer->set_queue_size(10000);
async_writer->set_flush_interval(std::chrono::seconds(1));
logger->add_writer(std::move(async_writer));
```

#### Batch Writer
```cpp
auto file_writer = std::make_unique<file_writer>("batch.log");
auto batch_writer = std::make_unique<batch_writer>(std::move(file_writer));
batch_writer->set_batch_size(100);
batch_writer->set_flush_interval(std::chrono::milliseconds(500));
logger->add_writer(std::move(batch_writer));
```

### Formatters

Formatters control how log messages are displayed.

```cpp
// Default formatter
auto formatter = std::make_unique<default_formatter>();
formatter->set_pattern("[%time%] [%level%] %message%");

// JSON formatter
auto json_formatter = std::make_unique<json_formatter>();
json_formatter->set_pretty_print(true);

// Custom formatter
class custom_formatter : public base_formatter {
public:
    std::string format(const log_entry& entry) override {
        // Custom formatting logic
        return formatted_string;
    }
};
```

### Filters

Filters control which messages are logged.

```cpp
// Level filter
auto level_filter = std::make_unique<level_filter>(log_level::warning);

// Category filter
auto category_filter = std::make_unique<category_filter>();
category_filter->add_allowed_category("network");
category_filter->add_allowed_category("database");

// Custom filter
class custom_filter : public base_filter {
public:
    bool should_log(const log_entry& entry) override {
        // Custom filtering logic
        return true;
    }
};
```

## Configuration

### Logger Builder

The recommended way to configure a logger:

```cpp
auto logger = logger_module::logger_builder()
    // Writers
    .with_console_writer()
    .with_file_writer("app.log")
    
    // Formatting
    .with_pattern("[%time%] [%level%] [%thread%] %message%")
    .with_timestamp_format("%Y-%m-%d %H:%M:%S")
    
    // Filtering
    .with_min_level(log_level::info)
    .with_category_filter({"network", "database"})
    
    // Performance
    .with_async_mode(true)
    .with_batch_writing(true)
    .with_queue_size(10000)
    
    // Build
    .build();
```

### Configuration Templates

Pre-configured templates for common use cases:

```cpp
// Production configuration
auto logger = logger_builder()
    .with_configuration_template(configuration_template::production)
    .build();

// Debug configuration
auto logger = logger_builder()
    .with_configuration_template(configuration_template::debug)
    .build();

// High-performance configuration
auto logger = logger_builder()
    .with_configuration_template(configuration_template::high_performance)
    .build();
```

### Environment-based Configuration

```cpp
auto logger = logger_builder()
    .with_environment_strategy(environment_strategy::auto_detect)
    .build();
// Automatically configures based on LOG_ENV environment variable
```

## Advanced Features

### Structured Logging

```cpp
// Using structured fields
logger->info("User login", {
    {"user_id", 12345},
    {"ip_address", "192.168.1.1"},
    {"timestamp", std::chrono::system_clock::now()},
    {"success", true}
});

// Using log_entry directly
log_entry entry;
entry.level = log_level::info;
entry.message = "Database query";
entry.fields["query"] = "SELECT * FROM users";
entry.fields["duration_ms"] = 45;
entry.fields["rows_returned"] = 100;
logger->log_structured(entry);
```

### Contextual Logging

```cpp
// Set thread-local context
logger->set_context("request_id", "abc-123");
logger->set_context("user_id", "user-456");

// All subsequent logs include context
logger->info("Processing request");  // Includes request_id and user_id

// Clear context
logger->clear_context();
```

### Performance Monitoring

```cpp
// Enable performance metrics
auto monitor = logger->get_monitor();
monitor->enable_metrics(true);

// Log some messages
for (int i = 0; i < 1000; ++i) {
    logger->info("Test message");
}

// Get metrics
auto metrics = monitor->get_metrics();
std::cout << "Messages logged: " << metrics.total_messages << "\n";
std::cout << "Average latency: " << metrics.average_latency_us << "μs\n";
std::cout << "Dropped messages: " << metrics.dropped_messages << "\n";
```

### Crash Safety

```cpp
// Install crash handler
logger->install_crash_handler();

// Configure emergency buffer
logger->set_emergency_buffer_size(4096);

// On crash, emergency logs are written to stderr
```

## Best Practices

### 1. Use Appropriate Log Levels

```cpp
// TRACE: Detailed diagnostic information
logger->trace("Entering function process_data()");

// DEBUG: Diagnostic information
logger->debug("Processing item", {{"id", item_id}});

// INFO: General informational messages
logger->info("Server started on port 8080");

// WARNING: Warning messages
logger->warning("Connection pool near capacity", {{"usage", 0.9}});

// ERROR: Error events that might still allow continued operation
logger->error("Failed to connect to database", {{"retry_count", 3}});

// CRITICAL: Critical events that require immediate attention
logger->critical("Out of memory", {{"available", 0}});
```

### 2. Structured Logging for Analytics

```cpp
// Good: Structured data for easy parsing
logger->info("HTTP Request", {
    {"method", "GET"},
    {"path", "/api/users"},
    {"status", 200},
    {"duration_ms", 45},
    {"client_ip", "192.168.1.1"}
});

// Avoid: Unstructured strings
logger->info("GET /api/users 200 45ms from 192.168.1.1");
```

### 3. Performance Considerations

```cpp
// Use lazy evaluation for expensive operations
if (logger->should_log(log_level::debug)) {
    auto expensive_data = compute_expensive_debug_info();
    logger->debug("Debug info", {{"data", expensive_data}});
}

// Use batch writing for high-throughput scenarios
auto logger = logger_builder()
    .with_batch_writing(true)
    .with_batch_size(1000)
    .build();

// Use async logging for I/O-bound applications
auto logger = logger_builder()
    .with_async_mode(true)
    .with_queue_size(100000)
    .build();
```

### 4. Error Handling

```cpp
// Check writer status
auto result = logger->add_writer(std::move(writer));
if (!result) {
    std::cerr << "Failed to add writer: " << result.error().message() << "\n";
}

// Handle logging failures gracefully
try {
    logger->info("Message");
} catch (const std::exception& e) {
    // Fallback to stderr
    std::cerr << "Logging failed: " << e.what() << "\n";
}
```

### 5. Resource Management

```cpp
// Properly shutdown logger
logger->flush();  // Ensure all messages are written
logger->shutdown();  // Clean shutdown

// Use RAII for automatic cleanup
{
    auto logger = create_logger();
    // Logger automatically cleaned up when out of scope
}
```

## Performance Optimization

### Memory Optimization

```cpp
// Enable Small String Optimization (SSO)
auto logger = logger_builder()
    .with_sso_threshold(256)  // Strings < 256 bytes use stack allocation
    .build();

// Pre-allocate buffers
logger->reserve_buffer(1024 * 1024);  // 1MB buffer
```

### Throughput Optimization

```cpp
// Use lock-free queue for high concurrency
auto logger = logger_builder()
    .with_lock_free_queue(true)
    .build();

// Batch writes to reduce syscalls
auto logger = logger_builder()
    .with_batch_writing(true)
    .with_batch_size(500)
    .with_flush_interval(std::chrono::milliseconds(100))
    .build();
```

### Latency Optimization

```cpp
// Use overflow policy to prevent blocking
auto logger = logger_builder()
    .with_overflow_policy(overflow_policy::drop_oldest)
    .with_queue_size(10000)
    .build();

// Disable features for minimal latency
auto logger = logger_builder()
    .with_configuration_template(configuration_template::low_latency)
    .build();
```

## Migration Guide

### From Version 1.x to 2.x

```cpp
// Old API (1.x)
logger->add_writer(new file_writer("app.log"));

// New API (2.x)
auto result = logger->add_writer(std::make_unique<file_writer>("app.log"));
if (!result) {
    // Handle error
}
```

### From Other Logging Libraries

#### From spdlog
```cpp
// spdlog
auto logger = spdlog::basic_logger_mt("my_logger", "logs/my_log.txt");
logger->info("Hello {}", "World");

// Logger System
auto logger = logger_builder()
    .with_file_writer("logs/my_log.txt")
    .build();
logger->info("Hello World");
```

#### From Boost.Log
```cpp
// Boost.Log
BOOST_LOG_TRIVIAL(info) << "Hello World";

// Logger System
logger->info("Hello World");
```

## API Reference

### Enumerations

```cpp
enum class log_level {
    trace = 0,
    debug = 1,
    info = 2,
    warning = 3,
    error = 4,
    critical = 5
};

enum class overflow_policy {
    block,          // Block until space available
    drop_oldest,    // Drop oldest messages
    drop_newest     // Drop new messages
};

enum class rotation_policy {
    size_based,     // Rotate based on file size
    time_based,     // Rotate based on time
    daily           // Rotate daily
};
```

### Type Definitions

```cpp
using fields_type = std::unordered_map<std::string, std::any>;
using timestamp_type = std::chrono::system_clock::time_point;
using thread_id_type = std::thread::id;
```

### Error Handling

```cpp
template<typename T>
class result {
public:
    bool has_value() const;
    T& value();
    const error& error() const;
    operator bool() const;
};

class error {
public:
    error_code code() const;
    std::string message() const;
};
```

---

*For complete API reference, see the [Doxygen documentation](https://kcenon.github.io/logger_system/).*