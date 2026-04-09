---
doc_id: "LOG-API-002a"
doc_title: "API Reference - Core"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "API"
---

> **Language:** **English** | [한국어](API_REFERENCE.kr.md)

# API Reference - Core

> **Split from**: [API_REFERENCE.md](API_REFERENCE.md)

**Version**: 0.5.0.0
**Last Updated**: 2026-02-08

## Table of Contents
- [Overview](#overview)
- [Core Classes](#core-classes)
- [ILogger Interface (Phase 2.0)](#ilogger-interface-phase-20)
- [Unified Log Context (v3.3.0)](#unified-log-context-v330)
- [Scoped Context Guard (v3.3.0)](#scoped-context-guard-v330)
- [Structured Log Builder (v3.1.0)](#structured-log-builder-v310)
- [Writer Builder (v4.1.0)](#writer-builder-v410)
- [Configuration](#configuration)
- [Builder Pattern](#builder-pattern)
- [Configuration Strategies](#configuration-strategies)
- [Interfaces](#interfaces)
- [Error Handling](#error-handling)

## Overview

The Logger System (v5.0) provides a high-performance, thread-safe logging framework with:

- **Dual API Design**: Supports both `common::interfaces::ILogger` (standardized) and native `logger_system::log_level` (backward compatible)
- **Standalone Mode**: No external dependencies required (uses `std::jthread` internally)
- **Optional Integration**: Thread_system integration is optional since v3.0
- **C++20 Features**: Leverages Concepts and `source_location` for enhanced developer experience
- **Unified Log Context** (v3.3.0): Thread-safe, category-based context management with type-safe storage
- **Structured Logging** (v3.1.0): Fluent builder API for creating structured log entries with arbitrary fields
- **Writer Builder** (v4.1.0): Fluent decorator composition for building complex writer configurations
- **Log Sampling** (v3.3.0): Multiple sampling strategies (random, rate limiting, adaptive, hash-based) for high-volume scenarios
- **Real-time Analysis** (v3.2.0): Anomaly detection with error spike, pattern matching, and rate anomaly alerts

### Namespace

All classes are in the `kcenon::logger` namespace:

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>

using namespace kcenon::logger;
```

## Core Classes

### `kcenon::logger::logger`

The main logger class that handles all logging operations. Implements both `common::interfaces::ILogger` for standardized interface and `security::critical_logger_interface` for crash-safe logging.

#### Constructor

```cpp
explicit logger(bool async = true,
                std::size_t buffer_size = 8192,
                std::unique_ptr<backends::integration_backend> backend = nullptr);
```

**Parameters:**
- `async`: Enable asynchronous logging (default: true)
- `buffer_size`: Size of the log buffer in bytes (default: 8192)
- `backend`: Integration backend for async processing (default: auto-detected standalone backend)

#### Destructor

```cpp
~logger();
```

Ensures all buffered messages are flushed before destruction.

## ILogger Interface (Phase 2.0)

Since v2.0, the logger implements `common::interfaces::ILogger` for standardized logging across the unified_system ecosystem.

### Log Methods (ILogger Interface)

```cpp
// Simple message logging
common::VoidResult log(common::interfaces::log_level level,
                       const std::string& message) override;

// With C++20 source_location (recommended)
common::VoidResult log(common::interfaces::log_level level,
                       std::string_view message,
                       const common::source_location& loc = common::source_location::current()) override;

// With explicit source location (legacy)
common::VoidResult log(common::interfaces::log_level level,
                       const std::string& message,
                       const std::string& file,
                       int line,
                       const std::string& function) override;

// Structured log entry
common::VoidResult log(const common::interfaces::log_entry& entry) override;
```

### Level Management (ILogger Interface)

```cpp
// Check if level is enabled
bool is_enabled(common::interfaces::log_level level) const override;

// Set minimum log level
common::VoidResult set_level(common::interfaces::log_level level) override;

// Get current minimum log level
common::interfaces::log_level get_level() const override;

// Flush buffered messages
common::VoidResult flush() override;
```

### Native API (Backward Compatible)

For backward compatibility with logger_system native types:

```cpp
// Log with native log_level
void log(log_level level, const std::string& message);
void log(log_level level, const std::string& message,
         const std::string& file, int line, const std::string& function);
void log(log_level level, const std::string& message,
         const core::log_context& context);

// Check level with native log_level
bool is_enabled(log_level level) const;

// Set/get minimum level (legacy)
void set_min_level(log_level level);  // Deprecated: use set_level()
log_level get_min_level() const;      // Deprecated: use get_level()
```

### Writer Management

```cpp
// Add writer (unnamed)
common::VoidResult add_writer(std::unique_ptr<base_writer> writer);

// Add writer with name (stores for later retrieval/removal)
common::VoidResult add_writer(const std::string& name, std::unique_ptr<base_writer> writer);

// Remove writer by name
bool remove_writer(const std::string& name);

// Get writer by name
base_writer* get_writer(const std::string& name);

// Remove all writers
common::VoidResult clear_writers();
```

### Lifecycle Management

```cpp
// Start async processing
common::VoidResult start();

// Stop async processing
common::VoidResult stop();

// Check if running
bool is_running() const;
```

### Metrics

```cpp
// Enable/disable metrics
common::VoidResult enable_metrics_collection(bool enable = true);
bool is_metrics_collection_enabled() const;

// Get metrics
result<metrics::logger_performance_stats> get_current_metrics() const;
result<std::unique_ptr<metrics::logger_performance_stats>> get_metrics_history(
    std::chrono::seconds duration) const;
common::VoidResult reset_metrics();
```

### Filtering

```cpp
// Set global filter
void set_filter(std::unique_ptr<log_filter_interface> filter);

// Check if filter is set
bool has_filter() const;
```

### Emergency Flush (Signal Safety)

```cpp
// Get file descriptor for emergency writing
int get_emergency_fd() const override;

// Get emergency buffer
const char* get_emergency_buffer() const override;
size_t get_emergency_buffer_size() const override;
```

## Unified Log Context (v3.3.0)

**Header**: `<kcenon/logger/core/unified_log_context.h>`

### `kcenon::logger::unified_log_context`

A thread-safe, category-based context manager that consolidates all logging context into a single interface. Replaces the previous scattered approach of managing context through separate mechanisms (`log_context_storage`, `otel_context_storage`, etc.).

#### Value Type

```cpp
using context_value = std::variant<std::monostate, bool, int64_t, double, std::string>;
```

Supports common types used in logging context. `std::monostate` represents null/unset values.

#### Context Categories

```cpp
enum class context_category : uint8_t {
    custom = 0,   // User-defined custom fields
    trace = 1,    // Distributed tracing (trace_id, span_id, parent_span_id)
    request = 2,  // Request metadata (request_id, correlation_id)
    otel = 3      // OpenTelemetry specific fields
};
```

#### Constructor

```cpp
unified_log_context() = default;
unified_log_context(const unified_log_context& other);
unified_log_context(unified_log_context&& other) noexcept;
```

#### Setters

```cpp
// Set a custom context value (chainable)
unified_log_context& set(std::string_view key,
                         context_value value,
                         context_category category = context_category::custom);

// Set distributed tracing context
unified_log_context& set_trace(std::string_view trace_id,
                               std::string_view span_id,
                               std::optional<std::string_view> parent_span_id = std::nullopt);

// Set request tracking context
unified_log_context& set_request(std::string_view request_id,
                                 std::optional<std::string_view> correlation_id = std::nullopt);

// Set OpenTelemetry context
unified_log_context& set_otel(const otlp::otel_context& ctx);
```

#### Getters

```cpp
// Get raw context value
[[nodiscard]] std::optional<context_value> get(std::string_view key) const;

// Get value as a specific type (returns nullopt if wrong type or not found)
template <typename T>
[[nodiscard]] std::optional<T> get_as(std::string_view key) const;

// Get value as string with optional default
[[nodiscard]] std::string get_string(std::string_view key,
                                     std::string_view default_value = "") const;

// Get category of a context entry
[[nodiscard]] std::optional<context_category> get_category(std::string_view key) const;
```

#### Query Methods

```cpp
[[nodiscard]] bool has(std::string_view key) const;
[[nodiscard]] bool empty() const;
[[nodiscard]] size_t size() const;
[[nodiscard]] std::vector<std::string> keys() const;
[[nodiscard]] std::vector<std::string> keys(context_category category) const;
```

#### Removal and Export

```cpp
void remove(std::string_view key);
void clear();
void clear(context_category category);

// Export to log_fields format for structured logging
[[nodiscard]] log_fields to_fields() const;

// Merge another context (overwrite controls key conflict resolution)
unified_log_context& merge(const unified_log_context& other, bool overwrite = true);
```

#### Usage Example

```cpp
#include <kcenon/logger/core/unified_log_context.h>

using namespace kcenon::logger;

unified_log_context ctx;

// Set custom fields
ctx.set("user_id", int64_t{12345});
ctx.set("session_active", true);
ctx.set("latency_ms", 42.5);

// Set distributed tracing context
ctx.set_trace("0af7651916cd43dd8448eb211c80319c", "b7ad6b7169203331");

// Set request context
ctx.set_request("req-123", "corr-456");

// Query values
auto user_id = ctx.get_as<int64_t>("user_id");  // std::optional<int64_t>{12345}
auto trace_id = ctx.get_string("trace_id");       // "0af7651916cd43dd..."

// Get keys by category
auto trace_keys = ctx.keys(context_category::trace);  // {"trace_id", "span_id"}

// Export to log_fields for structured logging
auto fields = ctx.to_fields();

// Clear specific category without affecting others
ctx.clear(context_category::trace);
```

#### Thread Safety

All methods are thread-safe. Read operations use shared locks, write operations use exclusive locks (uses `std::shared_mutex` internally).

---

## Scoped Context Guard (v3.3.0)

**Header**: `<kcenon/logger/core/scoped_context_guard.h>`

### `kcenon::logger::scoped_context_guard`

RAII guard for exception-safe context management. Saves the current context state on construction and restores it on destruction.

#### Constructor

```cpp
// Save current context state
explicit scoped_context_guard(logger& log);

// Save context and set a single field
scoped_context_guard(logger& log,
                     std::string_view key,
                     context_value value,
                     context_category category = context_category::custom);
```

#### Setters (Chainable)

```cpp
scoped_context_guard& set(std::string_view key,
                          context_value value,
                          context_category category = context_category::custom);

scoped_context_guard& set_trace(std::string_view trace_id,
                                std::string_view span_id,
                                std::optional<std::string_view> parent_span_id = std::nullopt);

scoped_context_guard& set_request(std::string_view request_id,
                                  std::optional<std::string_view> correlation_id = std::nullopt);

scoped_context_guard& set_otel(const otlp::otel_context& ctx);
```

#### Usage Example

```cpp
#include <kcenon/logger/core/scoped_context_guard.h>

using namespace kcenon::logger;

void handle_request(logger& log, const Request& req) {
    // Context is saved on construction
    scoped_context_guard guard(log);
    guard.set_request(req.id(), req.correlation_id())
         .set("user_id", int64_t{req.user_id()})
         .set("endpoint", std::string{req.path()});

    log.log_structured(log_level::info)
        .message("Processing request")
        .emit();  // Includes request_id, user_id, endpoint

    // Nested scope with additional context
    {
        scoped_context_guard step_guard(log);
        step_guard.set("step", std::string{"validation"});
        log.log_structured(log_level::debug)
            .message("Validating input")
            .emit();  // Includes request_id, user_id, endpoint, step
    } // "step" removed here

    log.log_structured(log_level::info)
        .message("Request complete")
        .emit();  // "step" is no longer in context
} // All context added by guard is restored here
```

The guard is non-copyable but movable. Each instance should be used from a single thread.

---

## Structured Log Builder (v3.1.0)

**Header**: `<kcenon/logger/core/structured_log_builder.h>`

### `kcenon::logger::structured_log_builder`

Fluent builder for creating structured log entries with arbitrary key-value fields. Integrates with the main logger class through the `log_structured()` method.

#### Constructor

```cpp
structured_log_builder(log_level level,
                       emit_callback callback,
                       const log_fields* context_fields = nullptr);
```

**Note**: Builders are typically created through the logger's `log_structured()` method, not constructed directly.

#### Builder Methods

```cpp
// Set the log message
structured_log_builder& message(const std::string& msg);

// Add fields (overloaded for multiple types)
structured_log_builder& field(const std::string& key, const std::string& value);
structured_log_builder& field(const std::string& key, const char* value);
structured_log_builder& field(const std::string& key, int value);
structured_log_builder& field(const std::string& key, int64_t value);
structured_log_builder& field(const std::string& key, double value);
structured_log_builder& field(const std::string& key, bool value);
structured_log_builder& field(const std::string& key, const log_value& value);

// Set the log category
structured_log_builder& category(const std::string& cat);

// Emit the log entry (must be called to actually log)
void emit();
```

#### Logger Integration (`log_structured`)

The canonical way to create a structured log entry is through the logger's `log_structured()` method:

```cpp
// Canonical API on the logger class
[[nodiscard]] structured_log_builder log_structured(log_level level);
```

Context fields set via `set_context()` on the logger are automatically included in all structured log entries.

#### Usage Example

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/structured_log_builder.h>

using namespace kcenon::logger;

// Basic structured logging
logger->log_structured(log_level::info)
    .message("User login")
    .field("user_id", 12345)
    .field("ip_address", "192.168.1.1")
    .field("session_id", "abc-123")
    .emit();

// Error logging with structured fields
logger->log_structured(log_level::error)
    .message("Database connection failed")
    .field("retry_count", 3)
    .field("db_host", "db-primary.internal")
    .field("latency_ms", 1523.4)
    .category("database")
    .emit();

// With context fields (automatically included)
logger->set_context("request_id", "req-789");
logger->set_context("trace_id", "trace-456");

logger->log_structured(log_level::info)
    .message("Processing request")
    .field("step", "validation")
    .emit();
// Output includes: request_id, trace_id, step, and message
```

**Important**: The builder does NOT auto-emit on destruction. You must explicitly call `emit()` to log the entry. If `emit()` is not called, the entry is silently discarded.

#### Thread Safety

Each builder instance should be used by a single thread. The `emit()` operation itself is thread-safe.

---

## Writer Builder (v4.1.0)

**Header**: `<kcenon/logger/builders/writer_builder.h>`

### `kcenon::logger::writer_builder`

Fluent builder for composing log writer decorators in a readable and maintainable way. Instead of manually nesting decorators, you chain method calls to build complex writer configurations.

The builder is move-only to ensure clear ownership semantics.

#### Core Writers (Terminal Nodes)

These methods set the base writer. Exactly one must be called before adding decorators.

```cpp
// File writer
writer_builder& file(const std::string& filename, bool append = true);

// Console writer
writer_builder& console(bool use_stderr = false, bool auto_detect_color = true);

// Custom writer (user-provided implementation)
writer_builder& custom(std::unique_ptr<log_writer_interface> writer);
```

#### Decorator Writers (Middleware)

These methods wrap the current writer with additional functionality. They can be chained in any order.

```cpp
// Asynchronous processing (typically outermost decorator)
writer_builder& async(std::size_t queue_size = 10000,
                      std::chrono::seconds flush_timeout = std::chrono::seconds(5));

// Buffering (batch writes for efficiency)
writer_builder& buffered(std::size_t max_entries = 100,
                         std::chrono::milliseconds flush_interval = std::chrono::milliseconds(5000));

// Encryption (AES-256, requires OpenSSL)
writer_builder& encrypted(security::secure_key key);

// Filtering (skip entries that don't match)
writer_builder& filtered(std::unique_ptr<log_filter_interface> filter);

// Formatting (apply custom format to entries)
writer_builder& formatted(std::unique_ptr<log_formatter_interface> formatter);
```

#### Build and Utility

```cpp
// Build and return the configured writer (transfers ownership, resets builder)
log_writer_ptr build();

// Check if a core writer has been set
bool has_core_writer() const noexcept;

// Reset builder to initial state for reuse
void reset() noexcept;
```

#### Usage Examples

```cpp
#include <kcenon/logger/builders/writer_builder.h>
#include <kcenon/logger/filters/log_filter.h>

using namespace kcenon::logger;

// Simple file writer with buffering and async
auto writer = writer_builder()
    .file("app.log")
    .buffered(100)
    .async(10000)
    .build();

// Console writer with level filtering
auto console = writer_builder()
    .console()
    .filtered(std::make_unique<filters::level_filter>(log_level::warning))
    .build();

// Encrypted file writer with buffering and async
auto key_result = security::secure_key_storage::generate_key(32);
auto secure_writer = writer_builder()
    .file("secure.log.enc")
    .encrypted(std::move(key_result.value()))
    .buffered()
    .async()
    .build();

// Custom writer with formatting
auto custom = writer_builder()
    .custom(std::make_unique<my_custom_writer>())
    .formatted(std::make_unique<json_formatter>(true))
    .buffered(50)
    .build();
```

**Error Handling**: Methods throw `std::logic_error` if called in invalid order (e.g., adding a decorator before setting a core writer, or setting two core writers).

---

## Configuration

### `kcenon::logger::logger_config`

Configuration structure with validation capabilities.

```cpp
struct logger_config {
    // Basic settings
    bool async = true;
    std::size_t buffer_size = 8192;
    logger_system::log_level min_level = logger_system::log_level::info;

    // Performance settings
    std::size_t batch_size = 100;
    std::chrono::milliseconds flush_interval{1000};
    bool use_lock_free = false;
    bool enable_batch_writing = false;

    // Feature flags
    bool enable_metrics = false;
    bool enable_crash_handler = false;
    bool enable_structured_logging = false;
    bool enable_color_output = true;
    bool enable_timestamp = true;
    bool enable_source_location = true;

    // Queue settings
    std::size_t max_queue_size = 10000;
    overflow_policy queue_overflow_policy = overflow_policy::drop_newest;
    std::size_t max_writers = 16;

    // File settings
    std::string log_directory = "";
    std::string log_file_prefix = "app";
    std::size_t max_file_size = 100 * 1024 * 1024;  // 100MB
    std::size_t max_file_count = 5;

    // Network settings
    std::string remote_host = "";
    uint16_t remote_port = 0;
    std::chrono::milliseconds network_timeout{5000};

    // Validation
    common::VoidResult validate() const;
};
```

### Static Factory Methods

```cpp
static logger_config default_config();
static logger_config production();
static logger_config debug_config();
static logger_config high_performance();
static logger_config low_latency();
```

## Builder Pattern

### `kcenon::logger::logger_builder`

Fluent interface for constructing logger instances with validation.

#### Basic Configuration

```cpp
logger_builder& with_config(const logger_config& config);
logger_builder& use_template(const std::string& template_name);
logger_builder& with_async(bool async = true);
logger_builder& with_buffer_size(std::size_t size);
logger_builder& with_min_level(logger_system::log_level level);
logger_builder& with_batch_size(std::size_t size);
logger_builder& with_batch_writing(bool enable = true);
logger_builder& with_flush_interval(std::chrono::milliseconds interval);
logger_builder& with_lock_free(bool enable = true);
```

#### Feature Flags

```cpp
logger_builder& with_metrics(bool enable = true);
logger_builder& with_crash_handler(bool enable = true);
logger_builder& with_structured_logging(bool enable = true);
logger_builder& with_overflow_policy(logger_config::overflow_policy policy);
logger_builder& with_max_queue_size(std::size_t size);
logger_builder& with_default_pattern();
```

#### Output Configuration

```cpp
logger_builder& with_file_output(const std::string& directory,
                                 const std::string& prefix = "app",
                                 std::size_t max_size = 100 * 1024 * 1024,
                                 std::size_t max_count = 5);
logger_builder& with_network_output(const std::string& host,
                                    uint16_t port,
                                    std::chrono::milliseconds timeout =
                                        std::chrono::milliseconds(5000));
```

#### Writers and Filters

```cpp
logger_builder& add_writer(const std::string& name,
                           std::unique_ptr<base_writer> writer);
logger_builder& add_filter(std::unique_ptr<log_filter_interface> filter);
logger_builder& add_level_filter(logger_system::log_level min_level);
logger_builder& add_regex_filter(const std::string& pattern,
                                 bool include_matches = true);
logger_builder& add_function_filter(
    std::function<bool(const log_entry&)> predicate);
logger_builder& with_formatter(std::unique_ptr<log_formatter_interface> formatter);
```

#### Backend Selection

```cpp
// Use custom backend
logger_builder& with_backend(std::unique_ptr<backends::integration_backend> backend);

// Use standalone backend explicitly (default)
logger_builder& with_standalone_backend();
```

#### Environment and Error Handling

```cpp
logger_builder& detect_environment();
logger_builder& auto_configure();
logger_builder& with_error_handler(
    std::function<void(const logger_error_code&)> handler);
```

#### Build

```cpp
common::VoidResult validate() const;
result<std::unique_ptr<logger>> build();
const logger_config& get_config() const;
```

## Configuration Strategies

### Deployment Strategy

```cpp
enum class deployment_env {
    development,
    staging,
    production,
    testing
};

logger_builder& for_environment(deployment_env env);
```

### Performance Strategy

```cpp
enum class performance_level {
    low_latency,
    balanced,
    high_throughput,
    minimal_overhead
};

logger_builder& with_performance_tuning(performance_level level);
```

### Environment Strategy

```cpp
// Auto-configure from LOG_* environment variables
logger_builder& auto_configure();
```

Supported environment variables:
- `LOG_LEVEL`: trace, debug, info, warn, error, fatal
- `LOG_ASYNC`: true/false
- `LOG_BUFFER_SIZE`: buffer size in bytes
- `LOG_BATCH_SIZE`: batch size
- `LOG_FLUSH_INTERVAL`: flush interval in ms
- `LOG_COLOR`: true/false
- `LOG_METRICS`: true/false
- `LOG_ENV`: production/staging/development/testing

### Composite Strategy

```cpp
logger_builder& apply_strategy(std::unique_ptr<config_strategy_interface> strategy);
logger_builder& clear_strategies();
```

## Interfaces

### `kcenon::logger::log_writer_interface`

```cpp
class log_writer_interface {
public:
    virtual ~log_writer_interface() = default;
    virtual common::VoidResult write(const log_entry& entry) = 0;
    virtual common::VoidResult flush() = 0;
    virtual bool is_healthy() const { return true; }
};
```

### `kcenon::logger::log_filter_interface`

```cpp
class log_filter_interface {
public:
    virtual ~log_filter_interface() = default;
    virtual bool should_log(const log_entry& entry) const = 0;
};
```

### `kcenon::logger::log_formatter_interface`

```cpp
class log_formatter_interface {
public:
    virtual ~log_formatter_interface() = default;
    virtual std::string format(const log_entry& entry) const = 0;
};
```

### `kcenon::logger::log_entry`

```cpp
struct log_entry {
    logger_system::log_level level;
    std::string message;
    std::string file;
    int line;
    std::string function;
    std::chrono::system_clock::time_point timestamp;
    std::thread::id thread_id;
    std::unordered_map<std::string, std::string> context;
};
```

## Error Handling

### Error Codes

**Header**: `<kcenon/logger/core/error_codes.h>`

```cpp
enum class logger_error_code {
    // General errors (0-999)
    success = 0,
    unknown_error = 1,
    not_implemented = 2,
    invalid_argument = 3,

    // Writer errors (1000-1099)
    writer_not_found = 1000,
    writer_initialization_failed = 1001,
    writer_already_exists = 1002,
    writer_not_healthy = 1003,

    // File errors (1100-1199)
    file_open_failed = 1100,
    file_write_failed = 1101,
    file_rotation_failed = 1102,
    file_permission_denied = 1103,

    // Network errors (1200-1299)
    network_connection_failed = 1200,
    network_send_failed = 1201,
    network_timeout = 1202,

    // Buffer/Queue errors (1300-1399)
    buffer_overflow = 1300,
    queue_full = 1301,
    queue_stopped = 1302,
    queue_overflow_dropped = 1303,
    queue_overflow_blocked = 1304,

    // Configuration errors (1400-1499)
    invalid_configuration = 1400,
    configuration_missing = 1401,
    configuration_conflict = 1402,

    // Metrics errors (1500-1599)
    metrics_collection_failed = 1500,
    metrics_not_available = 1501,

    // Processing errors (1600-1699)
    flush_timeout = 1600,
    processing_failed = 1601,
    filter_error = 1602,
    formatter_error = 1603,
    batch_processing_timeout = 1604,
    batch_processing_failed = 1605,

    // Security errors (1700-1799)
    encryption_failed = 1700,
    decryption_failed = 1701,
    authentication_failed = 1702,
    sanitization_failed = 1703,
    file_read_failed = 1704,
    insecure_permissions = 1705,
    path_traversal_detected = 1706,
    invalid_key_size = 1707,
    invalid_filename = 1708,

    // DI Container errors (1800-1899)
    di_not_available = 1800,
    component_not_found = 1801,
    registration_failed = 1802,
    creation_failed = 1803,
    operation_failed = 1804,
    async_operation_not_running = 1805,
    async_operation_already_running = 1806,

    // Writer errors - extended (1900-1999)
    writer_not_available = 1900,
    writer_configuration_error = 1901,
    writer_operation_failed = 1902,
    destructor_cleanup_failed = 1903
};
```

#### Error Code Categories Summary

| Range | Category | Description |
|-------|----------|-------------|
| 0-999 | General | Success, unknown, not implemented, invalid argument |
| 1000-1099 | Writer | Writer lifecycle and health errors |
| 1100-1199 | File | File I/O, rotation, and permission errors |
| 1200-1299 | Network | Connection, send, and timeout errors |
| 1300-1399 | Buffer/Queue | Overflow, capacity, and state errors |
| 1400-1499 | Configuration | Validation, missing, and conflict errors |
| 1500-1599 | Metrics | Collection and availability errors |
| 1600-1699 | Processing | Flush, filter, formatter, and batch errors |
| 1700-1799 | Security | Encryption, path traversal, and permission errors |
| 1800-1899 | DI Container | Dependency injection lifecycle errors |
| 1900-1999 | Writer (ext.) | Extended writer operation errors |

### Result Types

The logger uses `common::Result<T>` and `common::VoidResult` from common_system:

```cpp
// Success
return common::VoidResult{};

// Error
return common::make_error<void>(
    common::ErrorCategory::Logger,
    static_cast<int>(logger_error_code::queue_full),
    "Queue is full"
);
```

### Helper Functions

```cpp
// Create void error result (for common::VoidResult)
common::VoidResult make_logger_void_result(logger_error_code code,
                                           const std::string& message = "");

// Create a successful VoidResult
common::VoidResult make_logger_void_success();

// Extract logger_error_code from a VoidResult
logger_error_code get_logger_error_code(const common::VoidResult& result);

// Check if a VoidResult contains an error
bool has_logger_error(const common::VoidResult& result);

// Get error message from a VoidResult
std::string get_logger_error_message(const common::VoidResult& result);

// Convert error code to string
std::string logger_error_to_string(logger_error_code code);
```

#### `kcenon::logger::result<T>` Wrapper

A typed result wrapper built on top of the `common::Result<T>` pattern:

```cpp
template<typename T>
class result {
public:
    result(T&& value);                                    // Move-construct from value
    result(const T& value);                               // Copy-construct from value
    result(logger_error_code code, const std::string& msg = ""); // Error construction
    explicit result(const common::error_info& error);
    static result ok_value(const T& value);               // Factory method

    bool has_value() const;
    explicit operator bool() const;
    T& value();
    const T& value() const;
    logger_error_code error_code() const;
    const std::string& error_message() const;
};
```

---

*Last Updated: 2026-02-08*
