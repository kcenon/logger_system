---
doc_id: "LOG-ARCH-003"
doc_title: "Dependency Architecture"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "ARCH"
---

# Dependency Architecture

## Overview

logger_system follows a strict unidirectional dependency model using a tiered architecture. The bidirectional dependency risk identified in Issue #252 has been resolved.

## Dependency Tiers

```
Tier 0: common_system (ILogger, IExecutor, Result<T>)
           ^            ^
           |            |
Tier 1: thread_system   |
           ^            |
           |            |
Tier 2: logger_system --+
           (optional: LOGGER_USE_THREAD_SYSTEM)
```

- **Tier 0 (common_system)**: Provides shared interfaces (`ILogger`, `IExecutor`) and types (`Result<T>`). No dependencies on higher tiers.
- **Tier 1 (thread_system)**: Depends only on common_system. Uses `ILogger` for logging via dependency injection.
- **Tier 2 (logger_system)**: Depends on common_system (required) and thread_system (optional, `LOGGER_USE_THREAD_SYSTEM=OFF` by default).

## Abstraction Interfaces

| Interface | Defined In | Purpose |
|-----------|-----------|---------|
| `ILogger` | common_system | Allows thread_system to log without depending on logger_system |
| `IExecutor` | common_system | Allows logger_system to use thread pools without tight coupling |
| `Result<T>` | common_system | Shared error handling type across all tiers |

## Resolved: Bidirectional Dependency (Issue #252)

### Previous State

Both systems could optionally depend on each other:
- `logger_system -> thread_system` via `LOGGER_USE_THREAD_SYSTEM=ON`
- `thread_system -> logger_system` via `BUILD_WITH_LOGGER_SYSTEM=ON`

### Current State (Resolved)

- **`logger_system -> thread_system`**: Optional, off by default. Clean unidirectional dependency.
- **`thread_system -> logger_system`**: **Deprecated** via thread_system#336. `BUILD_WITH_LOGGER_SYSTEM` is off by default and scheduled for removal in **v0.5.0.0**. thread_system now uses `ILogger` dependency injection from common_system instead.

### CMake Guard

A transitional guard in `CMakeLists.txt` warns if both flags are enabled simultaneously. This guard will be removed once thread_system#336 is complete and `BUILD_WITH_LOGGER_SYSTEM` is removed.

## Configuration

### Recommended (Default)

```bash
cmake -B build   # Both integrations OFF, standalone mode
```

### With thread_pool Backend

```bash
cmake -B build -DLOGGER_USE_THREAD_SYSTEM=ON
```

### Deprecated (Avoid)

```bash
# BUILD_WITH_LOGGER_SYSTEM is deprecated in thread_system
cmake -B build -DLOGGER_USE_THREAD_SYSTEM=ON -DBUILD_WITH_LOGGER_SYSTEM=ON
```

## Migration Guide

If you are using `BUILD_WITH_LOGGER_SYSTEM=ON` in thread_system:

1. Set `BUILD_WITH_LOGGER_SYSTEM=OFF`
2. Inject an `ILogger` implementation at runtime instead:
   ```cpp
   #include <kcenon/logger/adapters/common_logger_adapter.h>
   auto logger = std::make_shared<kcenon::logger::adapters::common_logger_adapter>();
   thread_pool.set_logger(logger);  // ILogger dependency injection
   ```
3. This approach has no compile-time dependency on logger_system

## Timeline

| Milestone | Status | Description |
|-----------|--------|-------------|
| Issue #252 | Resolved | Bidirectional risk identified and analyzed |
| Issue #336 | In Progress | thread_system deprecates BUILD_WITH_LOGGER_SYSTEM |
| v0.5.0.0 | Planned | BUILD_WITH_LOGGER_SYSTEM removed from thread_system |
| Post-v0.5.0.0 | Planned | CMake bidirectional guard can be removed from logger_system |

## Related

- [Integration Guide](integration/THREAD_SYSTEM.md) - Detailed integration options
- [Issue #252](https://github.com/kcenon/logger_system/issues/252) - Original bidirectional dependency issue
- [Issue #458](https://github.com/kcenon/logger_system/issues/458) - Resolution tracking
- [thread_system#336](https://github.com/kcenon/thread_system/issues/336) - Deprecation of BUILD_WITH_LOGGER_SYSTEM
