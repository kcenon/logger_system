---
doc_id: "LOG-GUID-028"
doc_title: "Logger System Build Guide"
doc_version: "1.0.0"
doc_date: "2026-04-18"
doc_status: "Released"
project: "logger_system"
category: "GUID"
---

# Logger System Build Guide

> **SSOT**: This document is the single source of truth for **Logger System Build Guide**.

> **Language:** **English**

This guide provides complete build instructions for `logger_system`, including
prerequisites, CMake options, optional features, and presets.

For a 5-minute smoke-test walkthrough, see
[Quick Start Guide](QUICK_START.md). For getting started in code, see
[Getting Started Guide](GETTING_STARTED.md).

---

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Source Layout](#source-layout)
3. [Standard Build](#standard-build)
4. [CMake Presets](#cmake-presets)
5. [CMake Options](#cmake-options)
6. [Optional Features](#optional-features)
7. [Installing](#installing)
8. [Consuming from Another Project](#consuming-from-another-project)
9. [Platform Notes](#platform-notes)
10. [Next Steps](#next-steps)

---

## Prerequisites

| Requirement | Minimum Version |
|-------------|-----------------|
| CMake       | 3.20 (3.28+ for C++20 modules) |
| C++ compiler | GCC 11+, Clang 14+, Apple Clang 14+, MSVC 2022+ |
| Git         | any recent version |

Required dependency:

- [`common_system`](https://github.com/kcenon/common_system) - must be cloned
  alongside `logger_system` in the same parent directory.

Optional dependencies (enabled per-feature below):

- OpenSSL - required when `LOGGER_USE_ENCRYPTION=ON` (default ON).
- [`thread_system`](https://github.com/kcenon/thread_system) - required when
  `LOGGER_USE_THREAD_SYSTEM=ON` (default OFF). Standalone `std::jthread` is
  used otherwise.
- [`opentelemetry-cpp`](https://github.com/open-telemetry/opentelemetry-cpp)
  1.14+ - required when `LOGGER_ENABLE_OTLP=ON` (default OFF).
- Google Test 1.17 - required when `BUILD_TESTS=ON` (default ON).
- Google Benchmark 1.9.5 - required when `BUILD_BENCHMARKS=ON` (default OFF).

---

## Source Layout

`logger_system` expects a peer-directory layout so CMake can locate optional
sibling projects:

```
workspace/
  common_system/        # required
  thread_system/        # optional
  logger_system/        # this project
```

Clone them side by side:

```bash
git clone https://github.com/kcenon/common_system.git
git clone https://github.com/kcenon/thread_system.git   # optional
git clone https://github.com/kcenon/logger_system.git
cd logger_system
```

---

## Standard Build

```bash
# Configure (Release build with tests)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Compile
cmake --build build -j

# Run unit tests
ctest --test-dir build --output-on-failure
```

Helper scripts are also provided:

```bash
# Linux / macOS
./scripts/dependency.sh
./scripts/build.sh

# Windows
./scripts/dependency.bat
./scripts/build.bat
```

The build produces:

- Static library `liblogger_system.a` (or `logger_system.lib` on MSVC)
- CMake package files for downstream `find_package(logger_system)`
- Test executables under `build/bin/` when `BUILD_TESTS=ON`

---

## CMake Presets

Presets wrap the common configurations documented in `CMakePresets.json`:

| Preset        | Purpose                                      |
|---------------|----------------------------------------------|
| `default`     | Release, tests on, warnings-as-errors off    |
| `debug`       | `-DCMAKE_BUILD_TYPE=Debug`                   |
| `release`     | `-DCMAKE_BUILD_TYPE=Release`                 |
| `asan`        | AddressSanitizer + UBSan                     |
| `tsan`        | ThreadSanitizer                              |
| `ubsan`       | UndefinedBehaviorSanitizer                   |
| `ci`          | Configuration used by the CI pipeline        |
| `vcpkg`       | Uses the vcpkg toolchain file                |

Example:

```bash
cmake --preset debug
cmake --build --preset debug
```

---

## CMake Options

### Core Features

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTS` | `ON` | Build unit tests (Google Test) |
| `BUILD_BENCHMARKS` | `OFF` | Build micro-benchmarks (Google Benchmark) |
| `LOGGER_USE_DI` | `ON` | Dependency-injection-style writer composition |
| `LOGGER_USE_MONITORING` | `ON` | Metrics and monitoring backend |
| `LOGGER_ENABLE_ASYNC` | `ON` | Async writer implementations |
| `LOGGER_ENABLE_CRASH_HANDLER` | `ON` | Crash-safe flush on signal/abort |

### Advanced Features

| Option | Default | Description |
|--------|---------|-------------|
| `LOGGER_ENABLE_STRUCTURED_LOGGING` | `OFF` | JSON / key-value structured logs |
| `LOGGER_ENABLE_NETWORK_WRITER` | `OFF` | TCP/UDP network writer |
| `LOGGER_ENABLE_FILE_ROTATION` | `ON` | Rotating file writer |
| `LOGGER_USE_MODULES` | `OFF` | Experimental C++20 modules (CMake 3.28+) |
| `LOGGER_USE_THREAD_SYSTEM` | `OFF` | Use `thread_system` pool for async I/O |
| `LOGGER_USE_ENCRYPTION` | `ON` | AES-256 encrypted writer (requires OpenSSL) |
| `LOGGER_ENABLE_OTLP` | `OFF` | OpenTelemetry log export |

### Performance Tuning

| Option | Default | Description |
|--------|---------|-------------|
| `LOGGER_DEFAULT_BUFFER_SIZE` | `16384` | Default per-writer buffer size (bytes) |
| `LOGGER_DEFAULT_BATCH_SIZE` | `200` | Default batch-writer flush size |
| `LOGGER_DEFAULT_QUEUE_SIZE` | `20000` | Default async queue depth |

### Quality Assurance

| Option | Default | Description |
|--------|---------|-------------|
| `LOGGER_ENABLE_SANITIZERS` | `OFF` | Enable sanitizer instrumentation |
| `LOGGER_ENABLE_COVERAGE` | `OFF` | Emit coverage flags (gcov / llvm-cov) |
| `LOGGER_WARNINGS_AS_ERRORS` | `OFF` | Treat compiler warnings as errors |

See [Performance Guide](PERFORMANCE.md) for how these values affect throughput
and latency in practice.

---

## Optional Features

### Encryption (OpenSSL)

```bash
cmake -S . -B build -DLOGGER_USE_ENCRYPTION=ON
```

Install OpenSSL via your package manager (`apt install libssl-dev`,
`brew install openssl@3`, or vcpkg `openssl`). See
[Security Guide](SECURITY.md) for key-management details.

### `thread_system` Integration

```bash
cmake -S . -B build -DLOGGER_USE_THREAD_SYSTEM=ON
```

When enabled, async writers submit work to the `thread_system` thread pool
rather than spawning `std::jthread`s. See
[Integration Guide](INTEGRATION.md).

### OpenTelemetry Export

```bash
cmake -S . -B build -DLOGGER_ENABLE_OTLP=ON
```

Requires `opentelemetry-cpp` 1.14+ available via vcpkg or a system install.
See [OpenTelemetry Guide](OPENTELEMETRY.md).

### C++20 Modules (Experimental)

```bash
cmake -S . -B build -DLOGGER_USE_MODULES=ON
```

Requires CMake 3.28+, Clang 16+ or GCC 14+. The header-based interface remains
the primary, supported API.

---

## Installing

```bash
cmake --install build --prefix /opt/logger_system
```

The install step writes headers, the static library, and CMake package files:

```
<prefix>/include/kcenon/logger/
<prefix>/lib/liblogger_system.a
<prefix>/lib/cmake/logger_system/logger_system-config.cmake
```

---

## Consuming from Another Project

### With `find_package`

```cmake
find_package(logger_system CONFIG REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE kcenon::logger)
```

### With `FetchContent`

```cmake
include(FetchContent)

FetchContent_Declare(
    common_system
    GIT_REPOSITORY https://github.com/kcenon/common_system.git
    GIT_TAG        v0.2.0
)
FetchContent_Declare(
    logger_system
    GIT_REPOSITORY https://github.com/kcenon/logger_system.git
    GIT_TAG        v0.1.0
)
FetchContent_MakeAvailable(common_system logger_system)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE kcenon::logger)
```

Pin to a release tag; do not track `main`.

---

## Platform Notes

| Platform | Status | Notes |
|----------|--------|-------|
| Ubuntu 22.04+ (GCC 11+ / Clang 14+) | Supported | Primary CI target |
| macOS 13+ (Apple Clang 14+) | Supported | Primary CI target |
| Windows 11 (MSVC 2022) | Supported | Primary CI target |
| MinGW | Limited | Tests and benchmarks disabled |
| UWP / Xbox | Not supported | Use a different logging backend |

---

## Next Steps

- Encountered a build error? See [Troubleshooting Guide](TROUBLESHOOTING.md).
- Want to measure throughput? See [Performance Guide](PERFORMANCE.md).
- Ready to write code? See [Getting Started Guide](GETTING_STARTED.md).
