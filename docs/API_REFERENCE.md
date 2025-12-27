> **Language:** **English** | [한국어](API_REFERENCE_KO.md)

# API Reference

**Version**: 3.0.0
**Last Updated**: 2025-12-10

## Table of Contents
- [Overview](#overview)
- [Core Classes](#core-classes)
- [ILogger Interface (Phase 2.0)](#ilogger-interface-phase-20)
- [Configuration](#configuration)
- [Builder Pattern](#builder-pattern)
- [Configuration Strategies](#configuration-strategies)
- [Interfaces](#interfaces)
- [Error Handling](#error-handling)
- [Writers](#writers)
- [Filters](#filters)
- [Formatters](#formatters)
- [Backend Integration](#backend-integration)
- [Monitoring Integration](#monitoring-integration)

## Overview

The Logger System (v3.0) provides a high-performance, thread-safe logging framework with:

- **Dual API Design**: Supports both `common::interfaces::ILogger` (standardized) and native `logger_system::log_level` (backward compatible)
- **Standalone Mode**: No external dependencies required (uses `std::jthread` internally)
- **Optional Integration**: Thread_system integration is optional since v3.0
- **C++20 Features**: Leverages Concepts and `source_location` for enhanced developer experience

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

// Add writer with name
void add_writer(const std::string& name, std::unique_ptr<base_writer> writer);

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

### Dependency Injection

```cpp
// Set external DI container
void set_di_container(di::di_container_interface* container);
bool has_di_container() const;

// Add writer from DI
common::VoidResult add_writer_from_di(const std::string& name);

// Register writer factory
common::VoidResult register_writer_factory(
    const std::string& name,
    std::function<std::shared_ptr<base_writer>()> factory);

// Enable internal DI
common::VoidResult enable_di(di::di_container_factory::container_type type =
                      di::di_container_factory::container_type::automatic);
```

### Emergency Flush (Signal Safety)

```cpp
// Get file descriptor for emergency writing
int get_emergency_fd() const override;

// Get emergency buffer
const char* get_emergency_buffer() const override;
size_t get_emergency_buffer_size() const override;
```

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

// Deprecated: thread_system backend removed in v3.0
[[deprecated("Use with_standalone_backend() instead")]]
logger_builder& with_thread_system_backend();
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

```cpp
enum class logger_error_code {
    // General errors
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

    // Configuration errors (1400-1499)
    invalid_configuration = 1400,
    configuration_missing = 1401,
    configuration_conflict = 1402
};
```

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

// Create typed error result
template<typename T>
result<T> make_logger_error(logger_error_code code,
                            const std::string& message = "");

// Convert error code to string
std::string logger_error_to_string(logger_error_code code);
```

## Writers

### `kcenon::logger::console_writer`

```cpp
class console_writer : public base_writer {
public:
    console_writer(bool use_color = true);
    void set_use_color(bool use_color);
};
```

### `kcenon::logger::file_writer`

```cpp
class file_writer : public base_writer {
public:
    file_writer(const std::string& filename);
    common::VoidResult open();
    common::VoidResult close();
};
```

### `kcenon::logger::rotating_file_writer`

```cpp
class rotating_file_writer : public base_writer {
public:
    enum class rotation_type {
        size_based,
        time_based,
        daily,
        hourly
    };

    rotating_file_writer(const std::string& filename,
                         std::size_t max_size,
                         std::size_t max_files);

    rotating_file_writer(const std::string& filename,
                         rotation_type type,
                         std::size_t max_files);
};
```

### `kcenon::logger::network_writer`

```cpp
class network_writer : public base_writer {
public:
    enum class protocol_type { tcp, udp };

    network_writer(const std::string& host,
                   uint16_t port,
                   protocol_type protocol = protocol_type::tcp);
};
```

### `kcenon::logger::critical_writer`

Synchronous writer for critical messages that bypass async queue:

```cpp
class critical_writer : public base_writer {
public:
    critical_writer(std::unique_ptr<base_writer> underlying_writer);
};
```

### `kcenon::logger::batch_writer`

Wraps another writer with batching support:

```cpp
class batch_writer : public base_writer {
public:
    struct config {
        std::size_t max_batch_size = 100;
        std::chrono::milliseconds flush_interval{1000};
    };

    batch_writer(std::unique_ptr<base_writer> underlying_writer,
                 const config& cfg = {});
};
```

## Filters

### `kcenon::logger::filters::level_filter`

```cpp
class level_filter : public log_filter_interface {
public:
    level_filter(logger_system::log_level min_level);
};
```

### `kcenon::logger::filters::regex_filter`

```cpp
class regex_filter : public log_filter_interface {
public:
    regex_filter(const std::string& pattern, bool include = true);
};
```

### `kcenon::logger::filters::function_filter`

```cpp
class function_filter : public log_filter_interface {
public:
    using filter_func = std::function<bool(const log_entry&)>;
    function_filter(filter_func func);
};
```

### `kcenon::logger::filters::composite_filter`

```cpp
class composite_filter : public log_filter_interface {
public:
    enum class logic_type { AND, OR };

    composite_filter(logic_type logic = logic_type::AND);
    void add_filter(std::unique_ptr<log_filter_interface> filter);
};
```

## Formatters

### `kcenon::logger::plain_formatter`

```cpp
class plain_formatter : public base_formatter {
public:
    std::string format(const log_entry& entry) const override;
};
```

### `kcenon::logger::json_formatter`

```cpp
class json_formatter : public base_formatter {
public:
    json_formatter(bool pretty_print = false);
    std::string format(const log_entry& entry) const override;
};
```

## Backend Integration

### Integration Backend Interface

```cpp
namespace backends {

class integration_backend {
public:
    virtual ~integration_backend() = default;

    // Convert between log level types
    virtual common::interfaces::log_level to_common_level(
        logger_system::log_level level) const = 0;
    virtual logger_system::log_level from_common_level(
        common::interfaces::log_level level) const = 0;
};

}  // namespace backends
```

### Standalone Backend (Default)

```cpp
class standalone_backend : public integration_backend {
public:
    standalone_backend() = default;
    // Uses std::jthread for async processing
    // No external dependencies
};
```

## Monitoring Integration

### IMonitor Integration (Phase 2.2.4)

```cpp
logger_builder& with_monitoring(
    std::shared_ptr<common::interfaces::IMonitor> monitor);
logger_builder& with_health_check_interval(std::chrono::milliseconds interval);
```

## Usage Examples

### Basic Usage with ILogger Interface

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>

using namespace kcenon::logger;

int main() {
    auto result = logger_builder()
        .use_template("production")
        .add_writer("console", std::make_unique<console_writer>())
        .build();

    if (!result) {
        std::cerr << "Failed: " << result.error().message() << "\n";
        return 1;
    }

    auto logger = std::move(result.value());

    // Using ILogger interface (recommended)
    logger->log(common::interfaces::log_level::info, "Application started");

    // Using C++20 source_location (automatic)
    logger->log(common::interfaces::log_level::debug, "Debug message");
    // Source location captured automatically!

    logger->flush();
    return 0;
}
```

### Native API (Backward Compatible)

```cpp
// Using native logger_system types
logger->log(log_level::info, "Message with native types");

// Note: The source location is auto-captured internally in v2.x+
// The legacy API with __FILE__, __LINE__, __func__ is deprecated and will be removed in v3.0.0
logger->log(log_level::error, "Error occurred");
```

### Configuration Strategies

```cpp
auto logger = logger_builder()
    .for_environment(deployment_env::production)
    .with_performance_tuning(performance_level::high_throughput)
    .auto_configure()  // Override with environment variables
    .add_writer("file", std::make_unique<rotating_file_writer>(
        "logs/app.log", 10 * 1024 * 1024, 5))
    .build()
    .value();
```

### Custom Writer

```cpp
class custom_writer : public kcenon::logger::log_writer_interface {
public:
    common::VoidResult write(const kcenon::logger::log_entry& entry) override {
        // Custom implementation
        return common::ok();
    }

    common::VoidResult flush() override {
        return common::ok();
    }
};
```

### Filtering

```cpp
auto logger = logger_builder()
    .add_level_filter(log_level::warning)
    .add_regex_filter("critical|important", true)
    .add_function_filter([](const log_entry& entry) {
        return entry.file.find("debug") == std::string::npos;
    })
    .build();
```

## Thread Safety

All public methods of the logger class are thread-safe. Writers are called sequentially (no concurrent writes to the same writer).

## Performance Considerations

- Use asynchronous mode for better performance
- Batch size affects latency vs throughput trade-off
- Larger buffer sizes reduce contention but increase memory usage
- Use `is_enabled()` check before expensive message construction
- Consider `critical_writer` only for truly critical messages

## Migration from v2.x

See [MIGRATION_GUIDE.md](guides/MIGRATION_GUIDE.md) for detailed migration instructions.

Key changes in v3.0:
- Namespace: `logger_module` → `kcenon::logger`
- Interface: Implements `common::interfaces::ILogger`
- thread_system: Now optional (standalone mode is default)
- C++20 source_location support

---

*Last Updated: 2025-12-10*
