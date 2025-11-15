# Logger System Integration Guide

## Overview

This directory contains integration guides for using logger_system with other KCENON systems.

## Integration Guides

- [With Common System](with-common-system.md) - ILogger interface implementation
- [With Thread System](with-thread-system.md) - Async logging and thread-safe operations
- [With Monitoring System](with-monitoring.md) - Log metrics and observability
- [With Network System](with-network-system.md) - Network event logging
- [With Database System](with-database-system.md) - Database query logging

## Quick Start

### Basic Logging

```cpp
#include "logger_system/Logger.h"

int main() {
    auto logger = logger_system::createLogger("app.log");

    logger->info("Application started");
    logger->debug("Debug information");
    logger->warn("Warning message");
    logger->error("Error occurred: {}", error_msg);
}
```

### Structured Logging

```cpp
#include "logger_system/StructuredLogger.h"

auto logger = logger_system::createStructuredLogger("app.json");

logger->log({
    {"event", "user_login"},
    {"user_id", 12345},
    {"ip_address", "192.168.1.100"},
    {"timestamp", std::time(nullptr)}
});
```

### Async Logging for High Performance

```cpp
auto logger = logger_system::createLogger({
    .filename = "app.log",
    .async = true,
    .buffer_size = 8192,
    .flush_interval = std::chrono::milliseconds(100)
});
```

## Integration Patterns

### Dependency Injection

```cpp
class MyService {
public:
    MyService(std::shared_ptr<ILogger> logger)
        : logger_(std::move(logger)) {}

    void doWork() {
        logger_->info("Starting work");
        // ... work ...
        logger_->info("Work completed");
    }

private:
    std::shared_ptr<ILogger> logger_;
};
```

### Multiple Loggers

```cpp
// Application logger
auto app_logger = logger_system::createLogger("app.log");

// Database query logger
auto db_logger = logger_system::createLogger({
    .filename = "database.log",
    .level = LogLevel::DEBUG
});

// Error logger
auto error_logger = logger_system::createLogger({
    .filename = "errors.log",
    .level = LogLevel::ERROR
});
```

## Common Use Cases

### 1. Request/Response Logging

```cpp
server->on_request([logger](const auto& req) {
    logger->info("Request: {} {}", req.method(), req.path());
});

server->on_response([logger](const auto& resp) {
    logger->info("Response: {} ({}ms)", resp.status(), resp.duration());
});
```

### 2. Log Rotation

```cpp
auto logger = logger_system::createLogger({
    .filename = "app.log",
    .rotation = logger_system::Rotation::Daily,
    .max_files = 7  // Keep 7 days of logs
});
```

### 3. Contextual Logging

```cpp
auto base_logger = logger_system::createLogger("app.log");

// Add context for specific request
auto request_logger = base_logger->with_context({
    {"request_id", generate_uuid()},
    {"user_id", user_id}
});

request_logger->info("Processing request");  // Includes context
```

## Best Practices

- Use INFO level for production deployments
- Enable async logging for high-throughput applications
- Separate logs by component or subsystem
- Filter sensitive data from logs (passwords, API keys)
- Use structured logging for machine-readable logs

## Additional Resources

- [Logger System API Reference](../API_REFERENCE.md)
- [Ecosystem Integration Guide](../../../ECOSYSTEM_INTEGRATION.md)
