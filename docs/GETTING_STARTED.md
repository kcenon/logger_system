---
doc_id: "LOG-GUID-018"
doc_title: "Getting Started with logger_system"
doc_version: "1.0.0"
doc_date: "2026-04-05"
doc_status: "Released"
project: "logger_system"
category: "GUID"
---

# Getting Started with logger_system

> **See also**: The short-form canonical guide is [docs/guides/GETTING_STARTED.md](guides/GETTING_STARTED.md). This document is the long-form tutorial.

A step-by-step guide from zero to production-ready logging in C++20.

## Prerequisites

| Requirement | Minimum Version |
|-------------|-----------------|
| C++ standard | C++20 |
| GCC | 11+ |
| Clang | 14+ (Apple Clang 14+) |
| MSVC | 2022+ |
| CMake | 3.21+ |

**Required dependency**: `kcenon-common-system` (provides `ILogger`, `Result<T>`, concepts).

**Optional dependencies**:

| Dependency | Purpose |
|------------|---------|
| OpenSSL | AES-256-GCM encrypted logging |
| opentelemetry-cpp 1.14+ | OTLP export (HTTP/gRPC) |
| kcenon-thread-system | Thread pool for async I/O (standalone `std::jthread` used by default) |

## Installation

### Option A: CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    logger_system
    GIT_REPOSITORY https://github.com/kcenon/logger_system.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(logger_system)

target_link_libraries(your_target PRIVATE kcenon::logger)
```

### Option B: Build from source

```bash
git clone https://github.com/kcenon/logger_system.git
cd logger_system
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build
```

Key CMake options:

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTS` | ON | Build unit tests |
| `BUILD_BENCHMARKS` | OFF | Build performance benchmarks |
| `LOGGER_USE_ENCRYPTION` | ON | AES-256 via OpenSSL |
| `LOGGER_ENABLE_OTLP` | OFF | OpenTelemetry export |
| `LOGGER_USE_THREAD_SYSTEM` | OFF | Use thread_system pool instead of std::jthread |

### Option C: vcpkg preset

```bash
cmake --preset vcpkg
cmake --build --preset vcpkg
```

## First Logger

The smallest working example --- create a logger, attach a console writer, log a message:

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/console_writer.h>

using namespace kcenon::logger;
namespace ci = kcenon::common::interfaces;

int main() {
    // 1. Create a synchronous logger
    auto log = std::make_shared<logger>();

    // 2. Attach a console writer
    log->add_writer(std::make_unique<console_writer>());

    // 3. Log at different severity levels
    log->log(ci::log_level::info,    std::string("Application started"));
    log->log(ci::log_level::warning, std::string("Disk usage at 85%"));
    log->log(ci::log_level::error,   std::string("Connection refused"));

    return 0;
}
```

**Async mode** --- pass `true` to the constructor to use a background thread:

```cpp
auto log = std::make_shared<logger>(true, 16384); // async, 16 K buffer
log->add_writer(std::make_unique<console_writer>());
log->start();   // launch background thread

log->log(ci::log_level::info, std::string("Non-blocking write"));

log->stop();     // flush + join
```

> See `examples/basic_usage.cpp` for the full runnable version.

## Writer Composition (Decorator Pattern)

`writer_builder` is the recommended way to compose writers. Decorators
stack from the inside out:

```
Core Writer -> Filtering -> Buffering -> Encryption -> Thread-Safety -> Async
```

### Console with level filtering

```cpp
#include <kcenon/logger/builders/writer_builder.h>

auto writer = writer_builder()
    .console()
    .filtered(std::make_unique<level_filter>(log_level::warning))
    .build();
```

### Buffered + async file writer

```cpp
auto writer = writer_builder()
    .file("app.log")
    .buffered(500)       // batch 500 entries before writing
    .async(20000)        // 20 K entry async queue
    .build();
```

### Production multi-writer setup

```cpp
logger log;

// Main log: async + buffered
auto main_writer = writer_builder()
    .file("app.log")
    .buffered(500)
    .async(20000)
    .build();
log.add_writer("main", std::move(main_writer));

// Error log: errors only
auto err_filter = std::make_unique<level_filter>(log_level::error);
auto err_writer = writer_builder()
    .file("errors.log")
    .filtered(std::move(err_filter))
    .async()
    .build();
log.add_writer("errors", std::move(err_writer));
```

> See `examples/decorator_usage.cpp` and `examples/writer_builder_example.cpp`.

## Backends

logger_system ships several writer backends:

| Writer | Header | Purpose |
|--------|--------|---------|
| `console_writer` | `writers/console_writer.h` | stdout/stderr with optional color |
| `file_writer` | `writers/file_writer.h` | Single file output |
| `rotating_file_writer` | `writers/rotating_file_writer.h` | Size-based rotation with backup count |
| `network_writer` | `writers/network_writer.h` | TCP/UDP to a remote log server |
| `otlp_writer` | `writers/otlp_writer.h` | OpenTelemetry OTLP export |
| `encrypted_writer` | `writers/encrypted_writer.h` | AES-256-GCM encrypted output |
| `critical_writer` | `writers/critical_writer.h` | Immediate flush for critical logs |

**Decorator writers** (wrap any backend):

| Decorator | Header | Purpose |
|-----------|--------|---------|
| `async_writer` | `writers/async_writer.h` | Background thread processing |
| `buffered_writer` | `writers/buffered_writer.h` | Batch entries before writing |
| `filtered_writer` | `writers/filtered_writer.h` | Conditional write based on filter |
| `batch_writer` | `writers/batch_writer.h` | Time or size-triggered batches |

### Custom writers

Inherit from `thread_safe_writer` and implement `write_entry_impl()` and
`flush_impl()`. The base class handles all locking:

```cpp
#include <kcenon/logger/writers/thread_safe_writer.h>

class my_writer : public thread_safe_writer {
public:
    std::string get_name() const override { return "my_writer"; }

protected:
    kcenon::common::VoidResult write_entry_impl(const log_entry& entry) override {
        // your write logic (already under lock)
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult flush_impl() override {
        return kcenon::common::ok();
    }
};
```

> See `examples/custom_writer_example.cpp` for a full in-memory writer example.

## Structured Logging

Structured logging attaches typed key-value fields to each log entry,
enabling machine-parseable output in JSON or logfmt:

```cpp
#include <kcenon/logger/core/structured_log_builder.h>
#include <kcenon/logger/formatters/json_formatter.h>

logger_instance->log_structured(log_level::info)
    .message("User login successful")
    .field("user_id", 12345)
    .field("ip_address", "192.168.1.100")
    .field("mfa_enabled", true)
    .emit();
```

### Context propagation

Set logger-level context that automatically appears in every log entry:

```cpp
logger_instance->context()
    .set("service", std::string("order-service"))
    .set("version", std::string("1.2.3"));
```

### RAII context scopes

Thread-local context that cleans up automatically:

```cpp
#include <kcenon/logger/core/log_context_scope.h>

{
    log_context_scope scope({
        {"request_id", std::string("req-12345")},
        {"tenant",     std::string("acme-corp")}
    });

    // all logs in this scope include request_id and tenant
    logger_instance->log_structured(log_level::info)
        .message("Processing request")
        .emit();
}
// scope ended -- context removed
```

### Formatters

| Formatter | Output style |
|-----------|-------------|
| `json_formatter` | `{"level":"info","message":"...","user_id":12345}` |
| `logfmt_formatter` | `level=info msg="..." user_id=12345` |
| `template_formatter` | Custom pattern: `[{timestamp}] [{level}] {message}` |
| `timestamp_formatter` | Prepends ISO-8601 timestamp |

> See `examples/structured_logging_example.cpp`.

## OpenTelemetry Integration

When built with `LOGGER_ENABLE_OTLP=ON`, the `otlp_writer` exports logs
to any OTLP-compatible collector (Grafana, Jaeger, Datadog, etc.):

```cpp
#include <kcenon/logger/writers/otlp_writer.h>

auto otlp = std::make_unique<otlp_writer>(
    "http://localhost:4318/v1/logs",  // OTLP HTTP endpoint
    otlp_writer::protocol::http       // or ::grpc
);

logger_instance->add_writer("otlp", std::move(otlp));
```

Trace context propagation is automatic when `trace_id` / `span_id` are
set via `logger_instance->context()`.

> See the OTLP section in `docs/FEATURES.md` for batch export, retry
> policies, and TLS configuration.

## Configuration Builder

For comprehensive logger configuration use `logger_config_builder`:

```cpp
#include <kcenon/logger/core/logger_config_builder.h>

auto config = logger_config_builder()
    .use_production_defaults()
    .set_buffer_size(32768)
    .set_log_directory("./logs")
    .build();
```

Or construct a complete logger via `logger_builder`:

```cpp
#include <kcenon/logger/core/logger_builder.h>

auto result = logger_builder()
    .with_async(true)
    .with_buffer_size(32768)
    .with_min_level(log_level::info)
    .add_writer("main", std::make_unique<rotating_file_writer>(
        "logs/app.log", 100 * 1024 * 1024, 10))
    .build();

auto log = std::move(result.value());
log->start();
```

> See `examples/logger_config_builder_example.cpp` and
> `examples/critical_logging_example.cpp`.

## Next Steps

| Topic | Resource |
|-------|----------|
| Full API surface | [API_REFERENCE.md](./API_REFERENCE.md) |
| Quick cheat sheet | [API_QUICK_REFERENCE.md](./API_QUICK_REFERENCE.md) |
| Writer decorator deep-dive | [WRITER_GUIDE.md](./WRITER_GUIDE.md) |
| Configuration strategies | [CONFIGURATION_STRATEGIES.md](./CONFIGURATION_STRATEGIES.md) |
| Security (encryption, sanitization) | [SECURITY_GUIDE.md](./SECURITY_GUIDE.md) |
| Architecture overview | [ARCHITECTURE.md](./ARCHITECTURE.md) |
| Performance benchmarks | [BENCHMARKS.md](./BENCHMARKS.md) |
| Crash protection | [LOG_SERVER_AND_CRASH_SAFETY.md](./LOG_SERVER_AND_CRASH_SAFETY.md) |
| Custom writer creation | [advanced/CUSTOM_WRITERS.md](./advanced/CUSTOM_WRITERS.md) |
| All examples | [../examples/](../examples/) (16 runnable programs) |
