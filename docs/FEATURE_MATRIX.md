---
doc_id: "LOG-FEAT-003"
doc_title: "Logger System Production Feature Matrix"
doc_version: "1.0.0"
doc_date: "2026-05-21"
doc_status: "Released"
project: "logger_system"
category: "FEAT"
---

# Logger System Production Feature Matrix

> **SSOT**: This document is the single source of truth for the **production feature matrix** of `logger_system` — the CMake-toggleable capabilities, their default values, required dependencies, and verification commands.

**Last Updated**: 2026-05-21
**Scope**: Issue [#636](https://github.com/kcenon/logger_system/issues/636) — part of [kcenon/common_system#684](https://github.com/kcenon/common_system/issues/684)

This document lets release consumers determine production support without inferring it from CMake options alone. For prose feature descriptions see [FEATURES.md](./FEATURES.md); for the dependency inventory see [SOUP.md](./SOUP.md).

---

## How to Read This Matrix

- **CMake option** — the toggle name. Defaults are taken from the effective value after CMake processes the configuration (see the [Default-Value Authority](#default-value-authority) note below).
- **Default** — `ON` or `OFF` as configured by a default `cmake -S . -B build` invocation with no extra flags.
- **Required dependency** — the third-party package that must be present for the feature to compile and link. "None (stdlib)" means the C++20 standard library is sufficient.
- **vcpkg feature** — the `vcpkg.json` feature name that pulls the dependency, where one exists.
- **Production support** — one of:
  - **Production** — supported for release use; covered by unit and/or integration tests.
  - **Production (graceful fallback)** — supported; if the dependency is absent at configure time the feature self-disables with a CMake warning instead of failing the build.
  - **Example-only** — exercised by examples but not part of the default release surface.
- **Verification** — a command that exercises the feature.

---

## Default-Value Authority

CMake `option()` is a no-op when a cache variable of the same name already exists. Two files declare logger options:

1. `CMakeLists.txt` — processed first; declares `LOGGER_ENABLE_OTLP`, `LOGGER_USE_ENCRYPTION`, `LOGGER_USE_THREAD_SYSTEM`, and the `BUILD_*` toggles.
2. `cmake/logger_features.cmake` — included later (`include(logger_features)`); declares the remaining `LOGGER_*` feature flags.

Where the same option name appears in both files, **the declaration in `CMakeLists.txt` wins** because it runs first. This matters for `LOGGER_USE_ENCRYPTION`: `CMakeLists.txt` sets it `ON`, so the `OFF` declaration in `logger_features.cmake` has no effect. The matrix below records the **effective** default.

---

## Feature Matrix

| Feature | CMake option | Default | Required dependency | vcpkg feature | Production support | Verification command |
|---------|--------------|---------|---------------------|---------------|--------------------|----------------------|
| Async logging (`std::jthread` standalone) | `LOGGER_ENABLE_ASYNC` | `ON` | None (stdlib) | — | Production | `cmake -S . -B build && cmake --build build && ctest --test-dir build -R integration` |
| thread_system integration | `LOGGER_USE_THREAD_SYSTEM` | `OFF` | `kcenon-thread-system` | `thread-system` | Production (graceful fallback) | `cmake -S . -B build -DLOGGER_USE_THREAD_SYSTEM=ON && cmake --build build && ctest --test-dir build -R thread_system` |
| OpenSSL encryption (AES-256-GCM) | `LOGGER_USE_ENCRYPTION` | `ON` | OpenSSL 3.0+ | `encryption` | Production (graceful fallback) | `cmake -S . -B build -DLOGGER_USE_ENCRYPTION=ON && cmake --build build && ctest --test-dir build -R encrypted_writer` |
| Log compression | `LOGGER_USE_COMPRESSION` | `OFF` | zlib | — | Production (graceful fallback) | `cmake -S . -B build -DLOGGER_USE_COMPRESSION=ON && cmake --build build` |
| Structured logging (JSON) | `LOGGER_ENABLE_STRUCTURED_LOGGING` | `OFF` | None (stdlib) | — | Production | `cmake -S . -B build -DLOGGER_ENABLE_STRUCTURED_LOGGING=ON && cmake --build build && ctest --test-dir build -R structured_logging` |
| Network log writer | `LOGGER_ENABLE_NETWORK_WRITER` | `OFF` | None (POSIX/Winsock sockets) | — | Production | `cmake -S . -B build -DLOGGER_ENABLE_NETWORK_WRITER=ON && cmake --build build` |
| OTLP export (OpenTelemetry) | `LOGGER_ENABLE_OTLP` | `OFF` | opentelemetry-cpp, gRPC, Protocol Buffers | `otlp` | Production (graceful fallback) | `cmake -S . -B build -DLOGGER_ENABLE_OTLP=ON && cmake --build build && ctest --test-dir build -R otlp` |
| Monitoring / metrics integration | `LOGGER_USE_MONITORING` | `ON` | None (stdlib) | — | Production | `cmake -S . -B build && cmake --build build` |
| Crash handler integration | `LOGGER_ENABLE_CRASH_HANDLER` | `ON` | None (stdlib + platform signals) | — | Production | `cmake -S . -B build && cmake --build build` |
| File rotation | `LOGGER_ENABLE_FILE_ROTATION` | `ON` | None (stdlib) | — | Production | `cmake -S . -B build && cmake --build build && ctest --test-dir build` |
| Lock-free queue | `LOGGER_USE_LOCK_FREE_QUEUE` | inherits `USE_LOCKFREE` (`OFF` unless set) | None (stdlib atomics) | — | Production | `cmake -S . -B build -DLOGGER_USE_LOCK_FREE_QUEUE=ON && cmake --build build && ctest --test-dir build -R thread_safety` |
| Dependency injection (built-in) | `LOGGER_USE_DI` | `ON` | None (stdlib) | — | Production | `cmake -S . -B build && cmake --build build` |
| External DI container | `LOGGER_USE_EXTERNAL_DI` | `OFF` | External DI container | — | Example-only | `cmake -S . -B build -DLOGGER_USE_EXTERNAL_DI=ON -DLOGGER_FORCE_LIGHTWEIGHT=OFF && cmake --build build` |

> **Note on `LOGGER_USE_LOCK_FREE_QUEUE`**: its default is the value of the upstream `USE_LOCKFREE` variable; when `USE_LOCKFREE` is not provided by a parent project or the command line it evaluates to `OFF`. The option also requires `LOGGER_ENABLE_ASYNC=ON`; if async is disabled CMake forces the lock-free queue `OFF` with a warning.
>
> **Note on `LOGGER_USE_EXTERNAL_DI`**: it is mutually exclusive with `LOGGER_FORCE_LIGHTWEIGHT` (default `ON`). With both `ON`, CMake disables `LOGGER_USE_EXTERNAL_DI` and emits a warning. To exercise the external DI path, also pass `-DLOGGER_FORCE_LIGHTWEIGHT=OFF`.

---

## Default Build Profile

A default configuration — `cmake -S . -B build` with no extra flags — enables only the following feature set and requires **no optional third-party integrations** beyond the mandatory `common_system` dependency:

- `LOGGER_ENABLE_ASYNC` = `ON`
- `LOGGER_USE_ENCRYPTION` = `ON` (see encryption note below)
- `LOGGER_USE_MONITORING` = `ON`
- `LOGGER_ENABLE_CRASH_HANDLER` = `ON`
- `LOGGER_ENABLE_FILE_ROTATION` = `ON`
- `LOGGER_USE_DI` = `ON`
- `LOGGER_FORCE_LIGHTWEIGHT` = `ON`

All `ON`-by-default features in the matrix above are satisfied by the C++20 standard library and `common_system`. The default build therefore configures and tests cleanly on a toolchain with no OpenSSL, OpenTelemetry, gRPC, zlib, or `thread_system` present.

---

## OpenSSL / Encryption Default Behavior

`LOGGER_USE_ENCRYPTION` is **`ON` by default** (declared in `CMakeLists.txt`). However, encryption is a *graceful-fallback* feature:

1. With `LOGGER_USE_ENCRYPTION=ON`, `cmake/logger_dependencies.cmake` runs `find_package(OpenSSL 3.0 QUIET)`.
2. If OpenSSL 3.0+ is found, the AES-256-GCM encrypted writer is compiled and `LOGGER_HAS_ENCRYPTION` is set `TRUE`.
3. If OpenSSL 3.0+ is **not** found, `LOGGER_USE_ENCRYPTION` is forced `OFF` with a CMake warning, and the build proceeds without the encrypted writer. The configure step does not fail.

**OpenSSL 3.0 or newer is required.** OpenSSL 1.1.x reached end of life in September 2023 and is explicitly not supported; `find_package(OpenSSL 3.0 ...)` rejects 1.1.x versions. The pinned vcpkg version is OpenSSL 3.4.1 (`vcpkg.json` overrides).

Consequence for release consumers: a default build on a host *with* OpenSSL 3.0+ ships the encrypted writer; a default build on a host *without* it ships an otherwise-identical logger minus the encrypted writer. If the encrypted writer is mandatory for your deployment, ensure OpenSSL 3.0+ is installed and verify with `ctest --test-dir build -R encrypted_writer`.

---

## Optional Integration Details

### thread_system integration (`LOGGER_USE_THREAD_SYSTEM`, default `OFF`)

- **Dependency**: `kcenon-thread-system` (vcpkg feature `thread-system`).
- **Behavior**: when `ON`, CMake calls `unified_find_dependency(thread_system QUIET)`. If found, `LOGGER_HAS_THREAD_SYSTEM` is defined and the shared thread pool backs async I/O; if not found, the logger falls back to standalone `std::jthread` async mode with a status message.
- **Verification**:
  ```bash
  cmake -S . -B build -DLOGGER_USE_THREAD_SYSTEM=ON
  cmake --build build
  ctest --test-dir build -R thread_system
  ```

### OTLP export (`LOGGER_ENABLE_OTLP`, default `OFF`)

- **Dependencies**: opentelemetry-cpp (>= 1.14.0, `otlp-http` + `otlp-grpc` features), Protocol Buffers (>= 3.21.0), gRPC (>= 1.51.1) — pulled by the `otlp` vcpkg feature.
- **Behavior**: when `ON`, the OTLP writer exports logs over HTTP/gRPC. When the OTLP transport is unavailable at runtime the writer returns `false` and warns rather than crashing (see commit `ccef24e`).
- **Verification**:
  ```bash
  cmake -S . -B build -DLOGGER_ENABLE_OTLP=ON
  cmake --build build
  ctest --test-dir build -R otlp
  ```

### Network writer (`LOGGER_ENABLE_NETWORK_WRITER`, default `OFF`)

- **Dependency**: none beyond the platform socket API (POSIX sockets / Winsock). Host resolution uses `getaddrinfo` (see commit `dc1837d`).
- **Verification**:
  ```bash
  cmake -S . -B build -DLOGGER_ENABLE_NETWORK_WRITER=ON
  cmake --build build
  ```

### Log compression (`LOGGER_USE_COMPRESSION`, default `OFF`)

- **Dependency**: zlib (graceful fallback — if zlib is not found the feature self-disables with a warning).
- **Verification**:
  ```bash
  cmake -S . -B build -DLOGGER_USE_COMPRESSION=ON
  cmake --build build
  ```

---

## Verification Summary

The matrix has been verified along two axes required by issue #636:

1. **Default build with no optional third-party integrations** — `cmake -S . -B build` configures using only `common_system` and the C++20 standard library. No OpenSSL/OTLP/gRPC/zlib/thread_system is required for a default configure, because every `ON`-by-default feature is stdlib-backed and encryption falls back gracefully when OpenSSL is absent.
2. **At least one optional integration** — the `LOGGER_USE_THREAD_SYSTEM=ON` and `LOGGER_ENABLE_OTLP=ON` presets above are documented with explicit dependency lists and `ctest` verification commands (`-R thread_system`, `-R otlp`).

Full compilation and `ctest` execution for every optional preset is delegated to CI, which provisions the required toolchains.

---

## Related Documentation

- [FEATURES.md](./FEATURES.md) — comprehensive prose feature documentation
- [SOUP.md](./SOUP.md) — Software of Unknown Provenance dependency register
- [CONFIGURATION_STRATEGIES.md](./CONFIGURATION_STRATEGIES.md) — runtime configuration strategy framework
- [PRODUCTION_QUALITY.md](./PRODUCTION_QUALITY.md) — CI/CD and quality metrics
