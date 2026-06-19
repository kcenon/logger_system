---
doc_id: "LOG-GUID-029"
doc_title: "Logger System Troubleshooting Guide"
doc_version: "1.0.0"
doc_date: "2026-04-18"
doc_status: "Released"
project: "logger_system"
category: "GUID"
---

# Logger System Troubleshooting Guide

> **SSOT**: This document is the single source of truth for **Logger System
> Troubleshooting Guide**.

> **Language:** **English**

Quick reference for diagnosing common build, configuration, and runtime
issues in `logger_system`. For general usage questions, see the
[FAQ](FAQ.md); for build options, see the [Build Guide](BUILD.md).

---

## Table of Contents

1. [Build Errors](#build-errors)
2. [Linker Errors](#linker-errors)
3. [Runtime Issues](#runtime-issues)
4. [Performance Issues](#performance-issues)
5. [Integration Issues](#integration-issues)
6. [Platform-Specific Issues](#platform-specific-issues)
7. [Getting More Help](#getting-more-help)

---

## Build Errors

### `common_system` not found

Symptom:

```
CMake Error: Could not find a package configuration file provided by "common_system"
```

Cause: `common_system` was not cloned next to `logger_system`, or it was not
built/installed.

Fix:

```bash
cd ..   # parent of logger_system
git clone https://github.com/kcenon/common_system.git
cd logger_system
cmake -S . -B build
```

See [Build Guide - Source Layout](BUILD.md#source-layout).

### C++20 feature not available

Symptom (GCC/Clang):

```
error: 'std::jthread' was not declared in this scope
error: 'std::format' was not declared in this scope
```

Cause: Compiler does not meet the C++20 baseline (GCC 11+, Clang 14+,
Apple Clang 14+, MSVC 2022+).

Fix: Upgrade the compiler, or switch to a compatible preset:

```bash
cmake -S . -B build -DCMAKE_CXX_COMPILER=g++-11
```

### OpenSSL headers missing

Symptom:

```
fatal error: 'openssl/evp.h' file not found
```

Cause: `LOGGER_USE_ENCRYPTION=ON` (the default) but OpenSSL is not installed.

Fix (either install OpenSSL or disable encryption):

```bash
# Ubuntu/Debian
sudo apt install libssl-dev

# macOS
brew install openssl@3

# Or disable the feature
cmake -S . -B build -DLOGGER_USE_ENCRYPTION=OFF
```

### `opentelemetry-cpp` not found

Symptom:

```
CMake Error: Could not find a package configuration file provided by "opentelemetry-cpp"
```

Cause: `LOGGER_ENABLE_OTLP=ON` but `opentelemetry-cpp` 1.14+ is not available.

Fix: Install via vcpkg (`vcpkg install opentelemetry-cpp`) or disable OTLP:

```bash
cmake -S . -B build -DLOGGER_ENABLE_OTLP=OFF
```

### C++20 modules build fails

Symptom (CMake 3.27 or older):

```
Error: C++20 module support requires CMake 3.28 or newer
```

Fix: Either upgrade CMake and use a modules-capable compiler
(Clang 16+ / GCC 14+), or disable modules:

```bash
cmake -S . -B build -DLOGGER_USE_MODULES=OFF
```

The header-based interface is the supported primary API.

---

## Linker Errors

### Undefined references to `kcenon::logger::logger_builder`

Symptom:

```
undefined reference to `kcenon::logger::logger_builder::build()'
```

Cause: Application does not link the logger library, or links it before its
own object files (order matters on some GNU linkers).

Fix:

```cmake
target_link_libraries(my_app PRIVATE kcenon::logger)
```

Place the logger target after the consuming target in the link line.

### Multiple definitions when mixing static and shared builds

Cause: Both a system-installed and a `FetchContent`-built copy of
`logger_system` are linked into the same binary.

Fix: Choose one source. If using `FetchContent`, remove the system install
from `CMAKE_PREFIX_PATH`.

### Missing OpenSSL symbols at link time

Cause: Encryption is enabled but OpenSSL libraries are not in the link line
(can happen with custom toolchains that bypass CMake).

Fix: Ensure `find_package(OpenSSL REQUIRED)` succeeds and link
`OpenSSL::SSL OpenSSL::Crypto`, or disable `LOGGER_USE_ENCRYPTION`.

---

## Runtime Issues

### `logger_builder::build()` returns an error

Check the `Result` before using it:

```cpp
auto result = kcenon::logger::logger_builder()
    .add_writer("file", std::make_unique<file_writer>("app.log"))
    .build();

if (result.is_err()) {
    std::cerr << "build failed: " << result.error().message << "\n";
    return 1;
}
auto logger = std::move(result.value());
```

Common causes:

- No writers were added.
- A file path is not writable.
- Buffer or queue size is out of range.

### No output appears in the console

Possible causes:

1. Minimum level is above the messages being logged. Lower it:
   ```cpp
   .with_min_level(kcenon::logger::log_level::trace)
   ```
2. Async writer did not flush before exit. Always call:
   ```cpp
   logger->flush();
   ```
3. A crash or signal terminated the process before the async queue drained.
   Enable the crash handler (`LOGGER_ENABLE_CRASH_HANDLER=ON`, default) or use
   the `crash_safe` writer variant.

### File writer produces an empty or truncated file

- Confirm `flush()` is called before the process exits.
- Check permissions on the target directory.
- When using rotating file writer, verify the per-file size limit and max
  file count are sensible (very small limits rotate before flush).

### Messages are lost under high load

Async writer queue is full and the configured policy drops messages.

Fix (pick one):

- Increase queue depth:
  ```cpp
  .with_queue_size(100000)
  ```
- Use a blocking overflow policy instead of drop.
- Use a batching writer to reduce per-message overhead.

See [Performance Guide](PERFORMANCE.md).

### Crash or deadlock on program exit

Typical cause: static-destruction order between the logger and a writer's
owned resources.

Fix: Destroy the logger explicitly before `main` returns, or hold it in a
local `std::unique_ptr`.

---

## Performance Issues

| Symptom | Likely Cause | Fix |
|---------|-------------|-----|
| High latency per `log()` call | Synchronous writer | Use async writer chain |
| Throughput plateaus early | Small queue / batch size | Increase `LOGGER_DEFAULT_QUEUE_SIZE`, `LOGGER_DEFAULT_BATCH_SIZE` |
| High CPU with few logs | Formatter running on hot path | Defer formatting to async writer |
| Sporadic latency spikes | File rotation mid-flush | Pre-size log files, reduce rotation frequency |

See [Performance Guide](PERFORMANCE.md) and
[Benchmarks](../BENCHMARKS.md).

---

## Integration Issues

### `thread_system` pool not used

Ensure both the build option and runtime wiring are in place:

```bash
cmake -S . -B build -DLOGGER_USE_THREAD_SYSTEM=ON
```

See [Integration Guide](INTEGRATION.md) for constructor wiring.

### OTLP exporter fails with connection refused

- Verify the collector endpoint is reachable.
- Confirm the correct transport (HTTP vs gRPC) is selected.
- Check TLS settings if the collector requires them.

See [OpenTelemetry Guide](OPENTELEMETRY.md).

---

## Platform-Specific Issues

### MinGW

Tests and benchmarks are disabled on MinGW; the library itself builds. Use
a Windows MSVC build for test coverage.

### macOS: `std::format` unavailable

Apple Clang 14 has partial C++20 support. If `std::format` is not found,
upgrade Xcode command-line tools, or build with `clang++` from Homebrew.

### Windows: `dll not found` at runtime

When building shared OpenSSL, ensure the DLLs are on `PATH` or co-located
with the executable.

---

## Getting More Help

- [FAQ](FAQ.md) - conceptual and usage questions.
- [Build Guide](BUILD.md) - every CMake option, preset, and install step.
- [Performance Guide](PERFORMANCE.md) - tuning async, batch, and queue sizes.
- [Security Guide](SECURITY.md) - encryption, key storage, and auditing.
- [GitHub Issues](https://github.com/kcenon/logger_system/issues) - search
  before filing; include compiler version, CMake version, OS, and the exact
  error message.
