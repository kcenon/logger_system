# logger_system

## Overview

High-performance C++20 asynchronous logging framework for multithreaded applications.
Modular, interface-driven architecture with pluggable components via the Decorator pattern.
Targets 4M+ msg/s throughput with sub-microsecond latency.

## Architecture

```
include/kcenon/logger/
  core/          - Logger, logger_builder, logger_config, metrics, structured_log_builder
  interfaces/    - log_writer_interface, log_filter_interface, log_formatter_interface
  writers/       - 17 writer implementations (console, file, rotating_file, async,
                   buffered, batch, encrypted, otlp, network, critical, hybrid, etc.)
  builders/      - writer_builder (fluent API for decorator composition)
  backends/      - standalone_backend, integration_backend, monitoring_backend
  formatters/    - Log formatter implementations
  filters/       - Log filter implementations
  security/      - Secure key storage, path validation, audit logging
  otlp/          - OpenTelemetry context
  adapters/      - common_logger_adapter
```

Key abstractions:
- `log_writer_interface` — Base interface returning `common::VoidResult`
- `decorator_writer_base` — Base class for decorator pattern writers
- `queued_writer_base` — Shared queue management for async/batch writers
- `logger_builder` — Builder pattern for constructing loggers
- `writer_builder` — Fluent API for composing decorator chains

## Build & Test

```bash
# Standard build
cmake -B build && cmake --build build

# With tests
cmake -B build -DBUILD_TESTS=ON && cmake --build build
ctest --output-on-failure

# Using presets
cmake --preset debug && cmake --build --preset debug
```

Key CMake options:
- `BUILD_TESTS` (ON) — Unit tests
- `BUILD_BENCHMARKS` (OFF) — Performance benchmarks
- `LOGGER_USE_THREAD_SYSTEM` (OFF) — Optional thread_system integration
- `LOGGER_USE_ENCRYPTION` (ON) — AES-256 via OpenSSL
- `LOGGER_ENABLE_OTLP` (OFF) — OpenTelemetry export
- `LOGGER_USE_MODULES` (OFF) — C++20 modules (CMake 3.28+)

Presets: `default`, `debug`, `release`, `asan`, `tsan`, `ubsan`, `ci`, `vcpkg`

CI: Multi-platform (Ubuntu GCC/Clang, macOS, Windows MSVC), sanitizers, coverage (65%),
static analysis (clang-tidy, cppcheck), benchmarks, CVE scan.

## Key Patterns

- **Decorator pattern** — `decorator_writer_base` wraps `log_writer_interface` implementations;
  chain: Core Writer -> Filtering -> Buffering -> Encryption -> Thread-Safety -> Async
- **Async writers** — `async_writer` uses `std::jthread` (standalone) or thread_system pool;
  configurable queue size (default 10000)
- **OpenTelemetry** — `otlp_writer` exports via HTTP/gRPC with batch export, retry, trace correlation
- **Result-based errors** — `write()` returns `common::VoidResult`; `build()` returns `Result<T>`
- **Fluent builder** — `writer_builder` for composing decorator chains (preferred over manual nesting)

## Ecosystem Position

**Tier 2** — Depends on common_system (required) and thread_system (optional).

```
common_system (Tier 0) [required] — ILogger, Result<T>, concepts
thread_system (Tier 1) [optional] — Thread pool for async I/O
```

Consumed by higher-tier projects needing structured logging.

## Dependencies

**Required**: kcenon-common-system
**Optional**: OpenSSL (encryption), opentelemetry-cpp 1.14+ (OTLP), kcenon-thread-system
**Dev/test**: Google Test 1.17, Google Benchmark 1.9.5, spdlog 1.13+ (benchmark comparison)

## Known Constraints

- C++20 required (GCC 11+, Clang 14+, MSVC 2022+, Apple Clang 14+)
- MinGW: Tests and benchmarks disabled
- UWP/Xbox unsupported
- thread_system is optional since v3.1.0; standalone `std::jthread` is default
- C++20 modules experimental (CMake 3.28+, Clang 16+/GCC 14+)
- Manual decorator nesting deprecated in favor of `writer_builder`
