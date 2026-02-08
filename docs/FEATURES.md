# Logger System Features

**Last Updated**: 2026-02-08
**Version**: 0.4.0.0

This document provides comprehensive details about all features available in the logger system.

---

## Table of Contents

- [Core Features](#core-features)
- [Writer Types](#writer-types)
- [Formatting Options](#formatting-options)
- [Filtering Capabilities](#filtering-capabilities)
- [Rotation Policies](#rotation-policies)
- [Security Features](#security-features)
- [Advanced Capabilities](#advanced-capabilities)
- [OTLP Integration](#otlp-integration)
- [Log Sampling](#log-sampling)

---

## Core Features

### 🎯 High-Performance Logging

#### Asynchronous Processing
- **Non-blocking log operations**: Background thread processes log entries without blocking application threads
- **Batched queue processing**: Processes multiple log entries efficiently to maximize throughput
- **Configurable batching**: Tunable batch sizes and queue depths for optimal throughput
- **Zero-copy design**: Efficient message passing with minimal allocations
- **Adaptive queuing**: Intelligent backoff and batching strategies for optimal resource utilization

**Performance Metrics**:
- Peak throughput: 4.34M messages/second (single thread, async mode)
- Average enqueue time: 148 nanoseconds
- Memory efficiency: <2MB baseline with adaptive buffer management

#### Thread-Safe Operations
- **Concurrent logging**: Safe logging from multiple threads without locks on hot path
- **Writer thread safety**: All writer implementations verified for concurrent access
- **ThreadSanitizer compliance**: Zero data races detected in CI/CD pipeline
- **Perfect RAII compliance**: 100% smart pointer usage, zero memory leaks

#### Configuration Validation
- **Comprehensive validation**: Configuration validation with meaningful error messages
- **Builder pattern**: Fluent API for logger construction with validation
- **Template-based configurations**: Predefined configurations for common scenarios
- **Error handling**: Result pattern ensures no silent failures

### 🔧 Advanced Capabilities

#### Result-Based Error Handling
- **Result<T> pattern**: Type-safe error handling without exceptions
- **Comprehensive error codes**: Allocated range -200 to -299 in centralized error code registry
- **Meaningful error messages**: Comprehensive error context with validation details
- **Dual API design**: Balance between comprehensive error reporting and high-performance logging

#### Builder Pattern API
- **Fluent interface**: Type-safe logger construction with method chaining
- **Automatic validation**: Configuration validation during build process
- **Template support**: Use predefined templates or customize all parameters
- **Dependency injection**: Optional runtime component injection with fallback mechanisms

#### Configuration Strategies
- **Production template**: Optimized for production environments with balanced performance
- **Debug template**: Immediate output for development with detailed logging
- **High-performance template**: Maximized throughput for high-load scenarios
- **Low-latency template**: Minimized latency for real-time systems

#### Monitoring Integration
- **Pluggable monitoring**: Optional IMonitor interface integration
- **Health checks**: Built-in health check endpoints
- **Performance metrics**: Real-time metrics collection and reporting
- **Queue utilization**: Automatic optimization maintains high throughput

---

## Writer Types

### Console Writer

**Features**:
- ANSI colored output for different log levels
- Cross-platform support (Windows, Linux, macOS)
- Configurable color schemes
- Thread-safe concurrent output

**Usage**:
```cpp
#include <kcenon/logger/writers/console_writer.h>

auto logger = kcenon::logger::logger_builder()
    .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
    .build()
    .value();
```

**Configuration Options**:
- Color enabled/disabled
- Custom color schemes for different log levels
- Stdout/stderr routing based on log level

### File Writer

**Features**:
- Buffered file output for performance
- Automatic file creation and directory creation
- Configurable buffer sizes
- Thread-safe file access

**Usage**:
```cpp
#include <kcenon/logger/writers/file_writer.h>

auto logger = kcenon::logger::logger_builder()
    .add_writer("file", std::make_unique<kcenon::logger::file_writer>("app.log"))
    .build()
    .value();
```

**Configuration Options**:
- Buffer size (default: 8KB)
- Flush interval
- File permissions (default: 0600 - owner read/write only)
- Append/overwrite mode

### Rotating File Writer

**Features**:
- **Size-based rotation**: Automatically rotate when file reaches configured size
- **Time-based rotation**: Rotate daily, hourly, or at custom intervals
- **Backup file management**: Configurable number of backup files to keep
- **Automatic compression**: Optional gzip compression of rotated files
- **Thread-safe rotation**: Safe rotation during concurrent logging

**Size-Based Rotation**:
```cpp
#include <kcenon/logger/writers/rotating_file_writer.h>

// Rotate when file reaches 10MB, keep 5 backup files
auto logger = kcenon::logger::logger_builder()
    .add_writer("rotating", std::make_unique<kcenon::logger::rotating_file_writer>(
        "logs/app.log",
        10 * 1024 * 1024,  // 10MB per file
        5                   // Keep 5 backup files
    ))
    .build()
    .value();
```

**Time-Based Rotation**:
```cpp
// Rotate daily, keep 30 days of logs
auto logger = kcenon::logger::logger_builder()
    .add_writer("daily", std::make_unique<kcenon::logger::rotating_file_writer>(
        "logs/daily.log",
        kcenon::logger::rotating_file_writer::rotation_type::daily,
        30  // Keep 30 days of logs
    ))
    .build()
    .value();
```

**Configuration Options**:
- Rotation policy: size-based, daily, hourly, weekly
- Maximum file size (for size-based rotation)
- Number of backup files to keep
- Compression: none, gzip, bzip2
- Naming pattern for rotated files

### Network Writer

**Features**:
- TCP/UDP protocol support
- Automatic reconnection on connection failure
- Configurable retry strategies
- Message batching for network efficiency

**TCP Network Writer**:
```cpp
#include <kcenon/logger/writers/network_writer.h>

auto logger = kcenon::logger::logger_builder()
    .add_writer("remote", std::make_unique<kcenon::logger::network_writer>(
        "log-server.example.com",
        9999,
        kcenon::logger::network_writer::protocol_type::tcp
    ))
    .build()
    .value();
```

**UDP Network Writer**:
```cpp
// UDP for fire-and-forget logging (higher performance, no guarantees)
auto logger = kcenon::logger::logger_builder()
    .add_writer("udp_remote", std::make_unique<kcenon::logger::network_writer>(
        "log-collector.example.com",
        514,  // Syslog port
        kcenon::logger::network_writer::protocol_type::udp
    ))
    .build()
    .value();
```

**Configuration Options**:
- Protocol: TCP, UDP
- Connection timeout
- Retry attempts and backoff strategy
- Message batching size
- Compression

### Critical Writer

**Features**:
- **Synchronous logging**: Bypasses async queue for immediate writes
- **Guaranteed persistence**: File is updated before function returns
- **Crash-safe**: Ensures critical messages survive abnormal termination
- **Wrapper pattern**: Wraps any other writer for synchronous behavior

**Usage**:
```cpp
#include <kcenon/logger/writers/critical_writer.h>
#include <kcenon/logger/writers/file_writer.h>

// Create a critical writer wrapping a file writer
auto critical = std::make_unique<kcenon::logger::critical_writer>(
    std::make_unique<kcenon::logger::file_writer>("critical.log")
);

auto logger = kcenon::logger::logger_builder()
    .add_writer("critical", std::move(critical))
    .build()
    .value();

// This log is written immediately, not queued
logger->log(kcenon::logger::log_level::error, "Critical error occurred");
// ⚠️ File is guaranteed to be updated before this line executes
```

**Use Cases**:
- Fatal error logging before application termination
- Security audit trails requiring immediate persistence
- Transactional logging where data loss is unacceptable
- Pre-crash diagnostics

**⚠️ Performance Warning**: Critical writer blocks the calling thread until write completes. Use sparingly for truly critical messages only.

### Hybrid Writer

**Features**:
- **Adaptive behavior**: Automatically switches between async and sync based on log level
- **Best of both worlds**: Performance for info/debug, reliability for errors
- **Configurable threshold**: Set which log levels use sync vs async
- **Queue management**: Separate queue for async operations

**Usage**:
```cpp
#include <kcenon/logger/writers/hybrid_writer.h>
#include <kcenon/logger/writers/rotating_file_writer.h>

// Configure hybrid behavior
kcenon::logger::hybrid_writer_config config;
config.sync_level = kcenon::logger::log_level::error;  // Errors and above sync
config.async_queue_size = 10000;

auto hybrid = std::make_unique<kcenon::logger::hybrid_writer>(
    std::make_unique<kcenon::logger::rotating_file_writer>(
        "hybrid.log",
        10 * 1024 * 1024,  // 10MB per file
        5                   // Keep 5 files
    ),
    config
);

auto logger = kcenon::logger::logger_builder()
    .add_writer("hybrid", std::move(hybrid))
    .build()
    .value();

// Debug/Info: queued (async)
logger->log(kcenon::logger::log_level::debug, "Debugging info");

// Error: written immediately (sync)
logger->log(kcenon::logger::log_level::error, "Critical error!");
```

**Benefits**:
- Performance: Info/debug logs are asynchronous (low overhead)
- Reliability: Error/fatal logs are synchronous (guaranteed persistence)
- Simplicity: Single writer instead of multiple configurations

**Configuration Options**:
- Sync threshold level (default: ERROR)
- Async queue size
- Flush interval for async queue
- Underlying writer configuration

---

## Formatting Options

### Plain Text Formatter

**Features**:
- Simple, human-readable format
- Customizable timestamp format
- Configurable field order
- Color support (when used with console writer)

**Format**:
```
[2025-11-15 08:30:00.123] [INFO] [main.cpp:42] [main] Application started
```

**Usage**:
```cpp
#include <kcenon/logger/formatters/plain_formatter.h>

auto formatter = std::make_unique<kcenon::logger::plain_formatter>();
// Configure formatter...
```

### JSON Formatter

**Features**:
- Machine-parseable JSON output
- Structured field support
- ISO 8601 timestamp format
- Nested object support

**Format**:
```json
{
  "@timestamp": "2025-11-15T08:30:00.123Z",
  "level": "INFO",
  "message": "Application started",
  "thread_id": "12345",
  "file": "main.cpp",
  "line": 42,
  "function": "main"
}
```

**Usage**:
```cpp
#include <kcenon/logger/formatters/json_formatter.h>

auto formatter = std::make_unique<kcenon::logger::json_formatter>();
```

### Logfmt Formatter

**Features**:
- Key-value pair format
- Easy parsing with standard tools
- Compact representation
- Compatible with Logstash and other log processors

**Format**:
```
timestamp="2025-11-15T08:30:00.123Z" level=INFO message="Application started" thread_id=12345 file=main.cpp line=42 function=main
```

### Custom Formatter

**Features**:
- Implement your own formatting logic
- Full control over output format
- Access to all log entry fields
- Integration with custom log processors

**Usage**:
```cpp
class custom_formatter : public kcenon::logger::log_formatter_interface {
public:
    std::string format(const kcenon::logger::log_entry& entry) const override {
        // Custom formatting logic
        return fmt::format("[{}] {}", entry.level, entry.message);
    }
};
```

---

## Filtering Capabilities

### Level-Based Filtering

**Features**:
- Filter by minimum log level
- Dynamic level adjustment at runtime
- Per-writer level configuration
- Global and local level settings

**Usage**:
```cpp
#include <kcenon/logger/filters/level_filter.h>

// Only log warnings and above
logger->set_filter(std::make_unique<kcenon::logger::level_filter>(
    kcenon::logger::log_level::warning
));
```

### Regex-Based Filtering

**Features**:
- Pattern matching on log messages
- Include or exclude patterns
- Multiple pattern support
- Case-sensitive or case-insensitive matching

**Usage**:
```cpp
#include <kcenon/logger/filters/regex_filter.h>

// Filter out sensitive information
logger->set_filter(std::make_unique<kcenon::logger::regex_filter>(
    "password|secret|token",
    false  // Exclude matching messages
));
```

### Function-Based Filtering

**Features**:
- Custom filtering logic
- Access to all log entry fields
- Lambda or function pointer support
- Composable filters

**Usage**:
```cpp
#include <kcenon/logger/filters/function_filter.h>

auto custom_filter = std::make_unique<kcenon::logger::function_filter>(
    [](const kcenon::logger::log_entry& entry) {
        // Only log messages from specific files
        return entry.file.find("critical_module") != std::string::npos;
    }
);
logger->set_filter(std::move(custom_filter));
```

### Composite Filtering

**Features**:
- Combine multiple filters
- AND/OR logic support
- Filter chains
- Priority-based filtering

**Usage**:
```cpp
// Combine level and regex filters
auto composite = std::make_unique<kcenon::logger::composite_filter>(
    kcenon::logger::filter_logic::AND
);
composite->add_filter(std::make_unique<kcenon::logger::level_filter>(
    kcenon::logger::log_level::info
));
composite->add_filter(std::make_unique<kcenon::logger::regex_filter>(
    "critical|important",
    true  // Include matching messages
));
logger->set_filter(std::move(composite));
```

---

## Rotation Policies

### Size-Based Rotation

**Features**:
- Rotate when file reaches configured size
- Configurable maximum file size
- Backup file naming with sequence numbers
- Automatic cleanup of old backups

**Configuration**:
```cpp
kcenon::logger::rotation_config config;
config.policy = kcenon::logger::rotation_policy::size;
config.max_size_bytes = 10 * 1024 * 1024;  // 10MB
config.max_files = 5;  // Keep 5 backup files
```

**Rotation Behavior**:
- `app.log` → Current log file
- `app.log.1` → Most recent backup
- `app.log.2` → Second most recent backup
- ...
- `app.log.5` → Oldest backup (deleted when new rotation occurs)

### Time-Based Rotation

**Features**:
- Rotate at specific time intervals
- Support for daily, hourly, weekly rotation
- Custom rotation schedules
- Timestamp-based backup naming

**Daily Rotation**:
```cpp
kcenon::logger::rotation_config config;
config.policy = kcenon::logger::rotation_policy::daily;
config.rotation_hour = 0;  // Rotate at midnight
config.max_files = 30;  // Keep 30 days
```

**Hourly Rotation**:
```cpp
config.policy = kcenon::logger::rotation_policy::hourly;
config.max_files = 24;  // Keep 24 hours
```

**Rotation Behavior**:
- `app.log` → Current log file
- `app-2025-11-15.log` → Previous day's log
- `app-2025-11-14.log` → Two days ago
- ...

### Hybrid Rotation

**Features**:
- Combine size and time-based policies
- Rotate on whichever condition is met first
- Flexible configuration
- Optimal for variable-load applications

**Configuration**:
```cpp
kcenon::logger::rotation_config config;
config.policy = kcenon::logger::rotation_policy::hybrid;
config.max_size_bytes = 100 * 1024 * 1024;  // 100MB OR
config.rotation_interval = std::chrono::hours(24);  // 24 hours
config.max_files = 10;
```

### Compression Support

**Features**:
- Automatic compression of rotated files
- Support for gzip, bzip2
- Configurable compression level
- Background compression (non-blocking)

**Configuration**:
```cpp
kcenon::logger::rotation_config config;
config.compression = kcenon::logger::compression_type::gzip;
config.compression_level = 6;  // 1-9, higher = better compression
```

---

## Security Features

### Secure Key Storage

**Features**:
- RAII-based encryption key management
- Automatic memory cleanup using OpenSSL `OPENSSL_cleanse()`
- File permission enforcement (0600 - owner read/write only)
- Cryptographically secure random key generation
- **OpenSSL 3.x compatible** with EVP_MAC API for HMAC operations
- Backward compatible with OpenSSL 1.1.x (deprecated, EOL September 2023)

**Usage**:
```cpp
#include <kcenon/logger/security/secure_key_storage.h>

auto key_storage = kcenon::logger::secure_key_storage::create();
// Key is automatically cleaned from memory when object is destroyed
```

### Path Validation

**Features**:
- Path traversal attack prevention
- Symbolic link validation
- Base directory enforcement
- Filename character restrictions

**Protections**:
- Blocks `../` path traversal attempts
- Validates symlinks don't escape base directory
- Restricts dangerous characters in filenames
- Enforces whitelisted file extensions

**Usage**:
```cpp
#include <kcenon/logger/security/path_validator.h>

kcenon::logger::path_validator validator("/var/log/app");
auto result = validator.validate("/var/log/app/user-input.log");
if (!result) {
    std::cerr << "Invalid path: " << result.error().message() << "\n";
}
```

### Signal Handler Safety

**Features**:
- Centralized signal handler management
- Thread-safe logger registration
- Emergency flush for crash scenarios
- POSIX async-signal-safe functions only

**Protected Signals**:
- `SIGSEGV`: Segmentation fault
- `SIGABRT`: Abort signal
- `SIGFPE`: Floating point exception
- `SIGILL`: Illegal instruction
- `SIGTERM`: Termination request
- `SIGINT`: Interrupt (Ctrl+C)

**Usage**:
```cpp
#include <kcenon/logger/safety/crash_safe_logger.h>

// Install signal handlers for crash detection
kcenon::logger::crash_safe_logger::install_signal_handlers();
```

### Security Audit Logging

**Features**:
- Tamper-evident audit trail
- JSON-formatted audit entries
- HMAC-SHA256 signatures for integrity verification
- Tracks lifecycle, encryption, and security events

**Audit Entry Format**:
```json
{
  "@timestamp": "2025-11-15T08:30:00.123Z",
  "event_type": "logger_start",
  "user": "application",
  "action": "Logger initialized",
  "hmac": "a3d5c8e2f1b4..."
}
```

### Sensitive Data Sanitization

**Features**:
- Automatic detection and masking of sensitive data
- Configurable sanitization rules
- Support for emails, credit cards, tokens, passwords
- Custom pattern support

**Usage**:
```cpp
#include <kcenon/logger/security/log_sanitizer.h>

// Create sanitizer with default rules
auto sanitizer = std::make_shared<kcenon::logger::log_sanitizer>();

// Sanitize logs before writing
std::string message = "User login: john.doe@example.com, Card: 4532-1234-5678-9012";
std::string sanitized = sanitizer->sanitize(message);
// Result: "User login: j******e@example.com, Card: 4532********9012"

// Add custom sanitization rules
sanitizer->add_rule({
    "jwt_token",
    std::regex("Bearer\\s+[A-Za-z0-9\\-_]+\\.[A-Za-z0-9\\-_]+\\.[A-Za-z0-9\\-_]+"),
    [](const std::smatch& match) { return "Bearer [REDACTED]"; }
});
```

### Access Control

**Features**:
- Role-based access control for logging
- File-specific permissions
- User context tracking
- Permission level enforcement

**Usage**:
```cpp
#include <kcenon/logger/security/access_control_filter.h>

// Create access control filter
auto access_filter = std::make_unique<kcenon::logger::access_control_filter>(
    kcenon::logger::permission_level::write_info
);

// Set file-specific permissions
access_filter->set_file_permission(".*secure.*",
    kcenon::logger::permission_level::admin);

// Set user context
access_filter->set_user_context("current_user",
    kcenon::logger::permission_level::write_info);

logger->set_filter(std::move(access_filter));
```

### Compliance Support

**Standards**:
- **GDPR**: Data minimization, right to erasure, audit trails
- **PCI DSS**: Secure logging of payment data, access controls
- **ISO 27001**: Information security management
- **SOC 2**: Security, availability, processing integrity

**OWASP Top 10 Mitigations**:
- **A01 (Broken Access Control)**: Role-based access control
- **A02 (Cryptographic Failures)**: Secure key storage, encryption support
- **A03 (Injection)**: Input validation, sanitization
- **A09 (Security Logging Failures)**: Comprehensive audit logging

---

## Advanced Capabilities

### Structured Logging

**Features**:
- Key-value pair logging
- Nested object support
- Type-safe field addition
- Multiple output formats (JSON, logfmt)

**Usage**:
```cpp
#include <kcenon/logger/structured/structured_logger.h>

// Create structured logger wrapper
auto structured = std::make_shared<kcenon::logger::structured_logger>(
    logger,
    kcenon::logger::structured_logger::output_format::json
);

// Log with structured fields
structured->info("User logged in")
    .field("user_id", 12345)
    .field("ip_address", "192.168.1.1")
    .field("session_duration", 3600)
    .commit();

// Output (JSON format):
// {"@timestamp":"2025-11-15T08:30:00Z","level":"INFO","message":"User logged in",
//  "thread_id":"12345","user_id":12345,"ip_address":"192.168.1.1","session_duration":3600}
```

### Log Routing

**Features**:
- Conditional routing based on log level, message content
- Multiple target support
- Route to specific writers
- Stop propagation option
- Exclusive routing mode (messages only go to matched routes)
- Direct logger integration via `logger_builder`

**Builder API (Recommended)**:
```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/file_writer.h>

// Configure routing during logger construction
auto logger = kcenon::logger::logger_builder()
    .add_writer("all", std::make_unique<file_writer>("app.log"))
    .add_writer("errors", std::make_unique<file_writer>("errors.log"))
    .add_writer("security", std::make_unique<file_writer>("security.log"))
    // Route errors to dedicated file
    .route_level(kcenon::logger::log_level::error, {"errors"})
    // Route security-related messages by pattern
    .route_pattern("[Ss]ecurity|[Aa]uth", {"security"})
    // Enable exclusive routing (non-matching messages are dropped)
    .with_exclusive_routing(true)
    .build()
    .value();
```

**Direct Router Access**:
```cpp
// Access router after logger creation
auto& router = logger->get_router();

// Configure routing dynamically
router.set_exclusive_routes(true);

// Add custom route with filter
kcenon::logger::routing::route_config config;
config.writer_names = {"errors"};
config.filter = std::make_unique<kcenon::logger::filters::exact_level_filter>(
    kcenon::logger::log_level::error
);
config.stop_propagation = false;
router.add_route(std::move(config));
```

**Available Filters**:
- `level_filter`: Passes messages at or above minimum level
- `exact_level_filter`: Passes only messages at exactly the specified level
- `regex_filter`: Pattern matching on message content
- `function_filter`: Custom predicate-based filtering
- `composite_filter`: Combine multiple filters with AND/OR logic

**Routing Modes**:
- **Non-exclusive (default)**: All messages go to all writers
- **Exclusive**: Only messages matching a route go to matched writers; unmatched messages are dropped

### Performance Metrics Collection

**Features**:
- Built-in metrics collection
- Real-time performance monitoring
- Queue utilization tracking
- Message throughput calculation

**Usage**:
```cpp
// Enable metrics collection
logger->enable_metrics_collection(true);

// Log some messages
for (int i = 0; i < 1000; ++i) {
    logger->log(kcenon::logger::log_level::info, "Test message");
}

// Get current metrics
auto metrics = logger->get_current_metrics();
std::cout << "Messages per second: " << metrics.get_messages_per_second() << "\n";
std::cout << "Average enqueue time: " << metrics.get_avg_enqueue_time_ns() << " ns\n";
std::cout << "Queue utilization: " << metrics.get_queue_utilization_percent() << "%\n";
```

### Real-time Log Analysis

**Features**:
- Pattern matching and detection
- Statistical analysis
- Alert rules
- Report generation
- Real-time anomaly detection (error spikes, rate anomalies, new error types)

#### Post-hoc Analysis (log_analyzer)

```cpp
#include <kcenon/logger/analysis/log_analyzer.h>

// Create analyzer
auto analyzer = kcenon::logger::analysis::analyzer_factory::create_basic();

// Add entries and analyze
analyzer->add_entry(entry);
auto stats = analyzer->get_stats();
auto errors = analyzer->filter_by_level(log_level::error);
double error_rate = analyzer->get_error_rate(std::chrono::minutes(5));
std::string report = analyzer->generate_summary_report();
```

#### Real-time Anomaly Detection (realtime_log_analyzer)

```cpp
#include <kcenon/logger/analysis/realtime_log_analyzer.h>

// Create with production defaults
auto rt_analyzer = kcenon::logger::analysis::realtime_analyzer_factory::create_production(
    50,  // Error spike: 50 errors/minute
    [](const kcenon::logger::analysis::anomaly_event& event) {
        // Handle anomaly: error_spike, pattern_match, rate_anomaly, new_error_type
        std::cerr << "Anomaly detected: " << event.description << std::endl;
    }
);

// Add pattern alerts
rt_analyzer->add_pattern_alert("OutOfMemory|OOM", log_level::error);
rt_analyzer->add_pattern_alert("deadlock|timeout", log_level::warning);

// Integrate with logger
logger->set_realtime_analyzer(std::move(rt_analyzer));

// Query metrics
auto stats = logger->get_realtime_analyzer()->get_statistics();
// stats.total_analyzed, anomalies_detected, current_error_rate, current_log_rate
```

**Anomaly Types**:

| Type | Description | Trigger |
|------|-------------|---------|
| `error_spike` | Sudden increase in errors | Error count exceeds threshold per window |
| `pattern_match` | Regex pattern detected | Log message matches configured pattern |
| `rate_anomaly` | Abnormal log rate | Rate deviates significantly from baseline |
| `new_error_type` | Previously unseen error | Error message not seen before |

### Distributed Logging

**Features**:
- Network-based log transmission
- Log server for centralized collection
- Multiple protocol support (TCP, UDP)
- Compression and encryption support

**Client (Send Logs)**:
```cpp
#include <kcenon/logger/writers/network_writer.h>

// Send logs to remote server
logger->add_writer("remote", std::make_unique<kcenon::logger::network_writer>(
    "log-server.example.com",
    9999,
    kcenon::logger::network_writer::protocol_type::tcp
));
```

**Server (Receive Logs)**:
```cpp
#include <kcenon/logger/server/log_server.h>

// Create log server to receive logs
auto server = std::make_unique<kcenon::logger::log_server>(9999, true);
server->add_handler([](const kcenon::logger::log_server::network_log_entry& entry) {
    std::cout << "Received log from " << entry.source_address
              << ": " << entry.raw_data << std::endl;
});
server->start();
```

---

## OTLP Integration

OpenTelemetry Protocol (OTLP) support for exporting logs to observability backends.

### Overview

The OTLP writer exports log entries to OpenTelemetry-compatible collectors (e.g., Jaeger, Zipkin, Grafana Tempo) with full W3C Trace Context support for distributed tracing correlation.

```cpp
#include <kcenon/logger/writers/otlp_writer.h>
#include <kcenon/logger/otlp/otel_context.h>
```

### OTLP Writer Configuration

```cpp
kcenon::logger::writers::otlp_writer::config cfg;
cfg.endpoint = "http://localhost:4318/v1/logs";
cfg.protocol = kcenon::logger::writers::otlp_writer::protocol_type::http;
cfg.service_name = "my-service";
cfg.service_version = "1.0.0";
cfg.timeout = std::chrono::milliseconds{5000};
cfg.use_tls = false;
cfg.max_batch_size = 512;
cfg.flush_interval = std::chrono::milliseconds{5000};
cfg.max_queue_size = 10000;
cfg.max_retries = 3;
cfg.retry_delay = std::chrono::milliseconds{100};  // Exponential backoff

auto otlp = std::make_unique<kcenon::logger::writers::otlp_writer>(cfg);
logger->add_writer("otlp", std::move(otlp));
```

### Trace Context Propagation

```cpp
using namespace kcenon::logger::otlp;

// Set trace context for current thread
otel_context ctx;
ctx.trace_id = "0af7651916cd43dd8448eb211c80319c";
ctx.span_id = "b7ad6b7169203331";
ctx.trace_flags = "01";  // Sampled

// RAII scope - automatically restores previous context on destruction
{
    otel_context_scope scope(ctx);
    logger->log(log_level::info, "Processing request");
    // Log entry automatically includes trace_id and span_id
}

// Thread-local storage API
otel_context_storage::set(ctx);
auto current = otel_context_storage::get();  // std::optional<otel_context>
otel_context_storage::clear();
```

### Protocol Support

| Protocol | Default Port | Use Case |
|----------|-------------|----------|
| `http` | 4318 | Standard OTLP/HTTP, firewall-friendly |
| `grpc` | 4317 | High-throughput, streaming |

### Export Statistics

```cpp
auto stats = otlp_writer->get_stats();
// stats.logs_exported, logs_dropped, export_success, export_failures, retries
```

### Features

- **W3C Trace Context**: Full trace_id, span_id, trace_flags propagation
- **Batched Export**: Configurable batch size and flush interval
- **Retry with Backoff**: Exponential backoff on export failures
- **Resource Attributes**: Service name, version, namespace, custom attributes
- **Health Monitoring**: `is_healthy()` check for writer status
- **Dual Protocol**: HTTP and gRPC support
- **TLS Support**: Secure transport for production deployments

---

## Log Sampling

Adaptive log sampling to control volume while preserving important messages.

### Overview

The sampling system reduces log volume in high-throughput scenarios while ensuring critical messages are never dropped. Multiple strategies are available for different use cases.

```cpp
#include <kcenon/logger/sampling/log_sampler.h>
#include <kcenon/logger/sampling/sampling_config.h>
```

### Sampling Strategies

| Strategy | Description | Best For |
|----------|-------------|----------|
| `random` | Probabilistic sampling based on rate | General volume reduction |
| `rate_limiting` | N logs per time window | Strict rate control |
| `adaptive` | Adjusts rate based on volume | Variable traffic patterns |
| `hash_based` | Deterministic per message content | Reproducible sampling |

### Quick Setup

```cpp
using namespace kcenon::logger::sampling;

// Factory methods for common configurations
auto sampler = sampler_factory::create_random(0.1);          // 10% sampling
auto sampler = sampler_factory::create_rate_limited(1000);   // 1000/sec max
auto sampler = sampler_factory::create_adaptive(10000, 0.01); // Adapt at 10K/s
auto sampler = sampler_factory::create_production(0.1,
    {log_level::error, log_level::critical});                // Production preset

// Integrate with logger
logger->set_sampler(std::move(sampler));
```

### Configuration

```cpp
sampling_config config;
config.enabled = true;
config.rate = 0.1;  // 10% base rate
config.strategy = sampling_strategy::adaptive;

// Critical messages always logged
config.always_log_levels = {log_level::error, log_level::critical};

// Per-category rate overrides
config.category_rates = {
    {"security", 1.0},    // Always log security events
    {"debug", 0.01},      // 1% of debug messages
};

// Adaptive settings
config.adaptive_enabled = true;
config.adaptive_threshold = 10000;  // Trigger at 10K msgs/sec
config.adaptive_min_rate = 0.01;    // Floor at 1%

auto sampler = std::make_unique<log_sampler>(config);
logger->set_sampler(std::move(sampler));
```

### Runtime Control

```cpp
auto* sampler = logger->get_sampler();
sampler->set_enabled(true);
double effective_rate = sampler->get_effective_rate();

// Statistics
auto stats = logger->get_sampling_stats();
// stats.total_count, sampled_count, dropped_count, bypassed_count
// stats.effective_rate, is_throttling, actual_ratio()

logger->reset_sampling_stats();
```

### Features

- **Always-Log Levels**: Critical messages bypass sampling entirely
- **Per-Category Rates**: Different sampling rates for different log categories
- **Per-Field Rates**: Sampling based on structured field values
- **Adaptive Throttling**: Automatic rate reduction under high load
- **Runtime Reconfiguration**: Change sampling config without restart
- **Detailed Statistics**: Track sampling effectiveness with atomic counters
- **High Performance**: Fast xorshift64 PRNG, FNV-1a hashing

---

## See Also

- [Performance Benchmarks](BENCHMARKS.md) - Detailed performance analysis
- [Architecture Overview](01-architecture.md) - System architecture and design
- [API Reference](02-API_REFERENCE.md) - Complete API documentation
- [Security Guide](../SECURITY.md) - Security best practices
- [Getting Started](guides/GETTING_STARTED.md) - Quick start guide
