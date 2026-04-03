---
doc_id: "LOG-ADR-002"
doc_title: "ADR-002: Async Writer Architecture"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Accepted"
project: "logger_system"
category: "ADR"
---

# ADR-002: Async Writer Architecture

> **SSOT**: This document is the single source of truth for **ADR-002: Async Writer Architecture**.

| Field | Value |
|-------|-------|
| Status | Accepted |
| Date | 2025-04-01 |
| Decision Makers | kcenon ecosystem maintainers |

## Context

Synchronous logging blocks the calling thread until the log entry is written to
its destination (file, network, etc.). For high-throughput applications (4M+ msg/s
target), this blocking time directly impacts application performance.

logger_system needs an async writing mechanism that:
1. Decouples log submission from I/O — callers return immediately.
2. Provides backpressure when the write queue is full.
3. Supports both standalone mode and thread_system integration.
4. Guarantees no log loss during normal shutdown.

## Decision

**Implement a dual-mode async writer** (`async_writer`) that supports two execution
backends:

1. **Standalone mode** — Uses `std::jthread` (or `std::thread` fallback on AppleClang)
   with a dedicated I/O thread. Default when thread_system is not linked.

2. **Integration mode** — Submits write jobs to thread_system's thread pool.
   Enabled via `LOGGER_USE_THREAD_SYSTEM=ON` CMake option.

Both modes share `queued_writer_base`, which provides:
- A bounded concurrent queue (default 10,000 entries)
- Backpressure: `write()` returns an error when the queue is full
- Graceful shutdown: drain the queue before stopping the I/O thread
- Batch flushing: dequeue up to N entries per I/O cycle for throughput

```cpp
// Standalone: dedicated I/O thread
auto writer = writer_builder()
    .file("app.log")
    .with_async(/*queue_size=*/10000)
    .build();

// Integration: uses thread_system pool
auto writer = writer_builder()
    .file("app.log")
    .with_async(/*queue_size=*/10000, thread_pool_ptr)
    .build();
```

## Alternatives Considered

### Always Use `std::jthread`

- **Pros**: No external dependency, simple implementation.
- **Cons**: Each async writer spawns its own thread. Applications with many
  loggers (per-module logging) would create excessive threads.

### Always Require thread_system

- **Pros**: Shared thread pool, efficient resource usage.
- **Cons**: Forces a hard dependency on thread_system for all logger_system
  users, even simple applications that only need basic file logging.

### Lock-Free Ring Buffer

- **Pros**: Highest throughput, no mutex contention.
- **Cons**: Fixed-size ring buffer either wastes memory or truncates entries.
  Complex overflow handling. The bounded concurrent queue with mutex provides
  sufficient performance for the 4M+ msg/s target.

## Consequences

### Positive

- **Flexible deployment**: Standalone mode works without thread_system;
  integration mode leverages shared thread pools for efficiency.
- **Predictable memory**: Bounded queue with configurable size prevents
  unbounded memory growth during log bursts.
- **Graceful shutdown**: `queued_writer_base` drains pending entries on
  destruction, preventing log loss during normal shutdown.
- **Composable**: `async_writer` is a decorator — it wraps any writer,
  including other decorators (e.g., `async(encrypted(file))`).

### Negative

- **Latency variance**: Queue-based async introduces variable latency between
  log submission and actual write (microseconds to milliseconds under load).
- **Log loss on crash**: Entries in the queue at crash time are lost. Mitigated
  by the `crash_safe_logger` component for critical logs.
- **Two code paths**: Standalone and integration modes must both be tested and
  maintained, doubling the async testing surface.
