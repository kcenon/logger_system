---
doc_id: "LOG-API-003"
doc_title: "API Quick Reference"
doc_version: "1.0.0"
doc_date: "2026-04-05"
doc_status: "Released"
project: "logger_system"
category: "API"
---

# API Quick Reference

A compact cheat sheet for the most common logger_system APIs.
For the full reference see [API_REFERENCE.md](./API_REFERENCE.md).

## Logger Creation

```cpp
#include <kcenon/logger/core/logger.h>
using namespace kcenon::logger;
namespace ci = kcenon::common::interfaces;

// Synchronous logger (default)
auto log = std::make_shared<logger>();

// Asynchronous logger with custom buffer size
auto log = std::make_shared<logger>(true, 16384);
log->start();
// ... log messages ...
log->stop();
```

### Logger Builder

```cpp
#include <kcenon/logger/core/logger_builder.h>

auto result = logger_builder()
    .with_async(true)
    .with_buffer_size(32768)
    .with_min_level(ci::log_level::info)
    .with_monitoring(monitor_ptr)           // optional IMonitor
    .add_writer("name", std::move(writer))
    .build();                               // returns Result<unique_ptr<logger>>

auto log = std::move(result.value());
log->start();
```

### Config Builder

```cpp
#include <kcenon/logger/core/logger_config_builder.h>

auto cfg = logger_config_builder()
    .use_production_defaults()          // or use_debug_defaults()
    .set_buffer_size(32768)             //    use_high_performance_defaults()
    .set_batch_size(100)
    .set_flush_interval(std::chrono::milliseconds(500))
    .set_log_directory("./logs")
    .set_max_queue_size(50000)
    .set_overflow_policy(logger_config::overflow_policy::drop_oldest)
    .enable_metrics(true)
    .build();                           // returns Result<logger_config>
```

## Logging Messages

```cpp
// Simple string message
log->log(ci::log_level::info,    std::string("message"));
log->log(ci::log_level::warning, std::string("something wrong"));
log->log(ci::log_level::error,   std::string("failed"));

// Level check
if (log->is_enabled(ci::log_level::debug)) { /* ... */ }

// Set minimum level
log->set_level(ci::log_level::warning);

// Flush pending writes
log->flush();
```

### Log Levels (low to high severity)

| Level | Use for |
|-------|---------|
| `trace` | Fine-grained debug flow |
| `debug` | Developer diagnostics |
| `info` | Normal operational events |
| `warning` | Recoverable anomalies |
| `error` | Failed operations |
| `critical` | System-wide failures |
| `fatal` | Imminent shutdown |

## Writer Builder (Decorator Chain)

```cpp
#include <kcenon/logger/builders/writer_builder.h>

auto writer = writer_builder()
    .file("app.log")           // or .console()
    .filtered(std::move(f))    // optional: log_filter_interface
    .buffered(500)             // optional: batch size
    .encrypted(std::move(key)) // optional: secure_key (requires OpenSSL)
    .async(20000)              // optional: queue size (default 10000)
    .build();                  // returns unique_ptr<log_writer_interface>
```

**Recommended decorator order** (innermost to outermost):

```
Core Writer -> Filtering -> Buffering -> Encryption -> Thread-Safety -> Async
```

### Adding Writers to Logger

```cpp
// Unnamed (appended)
log->add_writer(std::move(writer));

// Named (retrievable by key)
log->add_writer("main", std::move(writer));
```

### Starting Async Writers

```cpp
if (auto* aw = dynamic_cast<async_writer*>(writer.get())) {
    aw->start();
    // ... use logger ...
    aw->stop();
}
```

## Core Writers

| Writer | Construction |
|--------|-------------|
| Console | `writer_builder().console().build()` |
| File | `writer_builder().file("path.log").build()` |
| Rotating File | `std::make_unique<rotating_file_writer>("path.log", max_bytes, backups)` |
| Network | `std::make_unique<network_writer>(host, port, protocol)` |
| OTLP | `std::make_unique<otlp_writer>(endpoint, proto)` |
| Critical | `std::make_unique<critical_writer>(inner_writer, config)` |
| Hybrid | `std::make_unique<hybrid_writer>(inner_writer, config, queue_size)` |

## Structured Logging

```cpp
#include <kcenon/logger/core/structured_log_builder.h>

log->log_structured(log_level::info)
    .message("User login")
    .field("user_id", 12345)
    .field("ip", "10.0.0.1")
    .field("mfa", true)
    .emit();
```

### Logger Context (applies to all subsequent logs)

```cpp
log->context()
    .set("service", std::string("order-svc"))
    .set("version", std::string("1.2.3"));

log->context().remove("version");
log->context().clear();
```

### RAII Context Scopes (thread-local)

```cpp
#include <kcenon/logger/core/log_context_scope.h>

{
    log_context_scope scope({
        {"request_id", std::string("req-123")},
        {"tenant",     std::string("acme")}
    });
    // all logs include request_id + tenant
}
// scope destroyed -- context removed

// Single-field shorthand
{
    scoped_context ctx("order_id", static_cast<int64_t>(456));
}
```

### Trace Context (distributed tracing)

```cpp
log->context()
    .set("trace_id", std::string("0af765..."), context_category::trace)
    .set("span_id",  std::string("b7ad6b..."), context_category::trace);

// Clear trace context when request ends
log->context().clear(context_category::trace);
```

## Filters

```cpp
#include <kcenon/logger/filters/log_filter.h>

// Level filter
log->set_filter(std::make_unique<level_filter>(log_level::warning));

// Regex filter (exclude matches)
log->set_filter(std::make_unique<regex_filter>("sensitive", false));

// Composite (AND / OR)
auto composite = std::make_unique<composite_filter>(composite_filter::logic_type::AND);
composite->add_filter(std::make_unique<level_filter>(log_level::warning));
composite->add_filter(std::make_unique<regex_filter>("ignore", false));
log->set_filter(std::move(composite));

// Custom function filter
log->set_filter(std::make_unique<function_filter>(
    [](const log_entry& e) { return e.message.to_string().size() < 1000; }
));

// Clear filter
log->set_filter(nullptr);
```

### Per-Writer Filtering (via decorator)

```cpp
auto writer = writer_builder()
    .file("errors.log")
    .filtered(std::make_unique<level_filter>(log_level::error))
    .build();
```

## Formatters

```cpp
#include <kcenon/logger/formatters/json_formatter.h>
#include <kcenon/logger/formatters/logfmt_formatter.h>
#include <kcenon/logger/formatters/template_formatter.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>

json_formatter      json_fmt;                              // {"level":"info",...}
logfmt_formatter    logfmt_fmt;                            // level=info msg="..."
template_formatter  tmpl_fmt("[{timestamp_local}] [{level:8}] {message}");
timestamp_formatter ts_fmt;                                // ISO-8601 prefix

std::string output = json_fmt.format(entry);
```

## Metrics

```cpp
log->enable_metrics_collection(true);

auto result = log->get_current_metrics();
if (result.has_value()) {
    auto& m = result.value();
    m.messages_logged.load();          // total messages
    m.messages_dropped.load();         // overflow drops
    m.get_messages_per_second();       // throughput
    m.get_queue_utilization_percent(); // queue fill %
    m.get_avg_enqueue_time_ns();       // avg latency
    m.writer_errors.load();            // write failures
}
```

## Critical Logging

```cpp
#include <kcenon/logger/writers/critical_writer.h>

auto critical = std::make_unique<critical_writer>(
    std::make_unique<file_writer>("logs/critical.log"),
    critical_writer_config{
        .force_flush_on_critical = true,
        .force_flush_on_error    = true,
        .write_ahead_log         = true,
        .wal_path                = "logs/.critical.wal",
        .sync_on_critical        = true
    }
);
```

### Hybrid Writer (async normal + sync critical)

```cpp
#include <kcenon/logger/writers/critical_writer.h>

auto hybrid = std::make_unique<hybrid_writer>(
    std::make_unique<file_writer>("logs/app.log"),
    critical_writer_config{
        .force_flush_on_critical = true,
        .force_flush_on_error    = true
    },
    10000   // async queue size
);
```

## Custom Writer

```cpp
#include <kcenon/logger/writers/thread_safe_writer.h>

class my_writer : public thread_safe_writer {
public:
    std::string get_name() const override { return "my_writer"; }

protected:
    kcenon::common::VoidResult write_entry_impl(const log_entry& entry) override {
        // already under lock -- write entry here
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult flush_impl() override {
        return kcenon::common::ok();
    }
};
```

## Composite Writer (SRP Architecture)

```cpp
#include <kcenon/logger/writers/composite_writer.h>
#include <kcenon/logger/sinks/console_sink.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>

auto writer = make_composite_writer(
    std::make_unique<timestamp_formatter>(),
    std::make_unique<console_sink>()
);
writer->write(entry);
```

## Header Quick Index

| Area | Header |
|------|--------|
| Logger | `kcenon/logger/core/logger.h` |
| Logger builder | `kcenon/logger/core/logger_builder.h` |
| Config builder | `kcenon/logger/core/logger_config_builder.h` |
| Writer builder | `kcenon/logger/builders/writer_builder.h` |
| Structured logging | `kcenon/logger/core/structured_log_builder.h` |
| Context scope | `kcenon/logger/core/log_context_scope.h` |
| Filters | `kcenon/logger/filters/log_filter.h` |
| Filter interface | `kcenon/logger/interfaces/log_filter_interface.h` |
| Log entry | `kcenon/logger/interfaces/log_entry.h` |
| JSON formatter | `kcenon/logger/formatters/json_formatter.h` |
| Logfmt formatter | `kcenon/logger/formatters/logfmt_formatter.h` |
| Template formatter | `kcenon/logger/formatters/template_formatter.h` |
| Common interfaces | `kcenon/common/interfaces/logger_interface.h` |
