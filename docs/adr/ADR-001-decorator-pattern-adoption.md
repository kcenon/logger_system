---
doc_id: "LOG-ADR-001"
doc_title: "ADR-001: Decorator Pattern for Writer Composition"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Accepted"
project: "logger_system"
category: "ADR"
---

# ADR-001: Decorator Pattern for Writer Composition

> **SSOT**: This document is the single source of truth for **ADR-001: Decorator Pattern for Writer Composition**.

| Field | Value |
|-------|-------|
| Status | Accepted |
| Date | 2025-02-01 |
| Decision Makers | kcenon ecosystem maintainers |

## Context

logger_system provides multiple log output destinations (console, file, rotating file,
network) with cross-cutting concerns (buffering, encryption, async I/O, filtering,
thread-safety). These concerns can be combined in arbitrary ways.

The combinatorial explosion of `N` writers x `M` concerns produces `N * 2^M` concrete
classes with naive inheritance. For example, 5 writers and 4 concerns would require
up to 80 distinct classes.

Requirements:
1. Writers must be composable — users should combine features freely.
2. Adding a new cross-cutting concern must not require modifying existing writers.
3. The composition must be configurable at runtime (builder pattern).
4. Performance: the decorator chain should add minimal overhead per log entry.

## Decision

**Adopt the Decorator pattern** with a shared base class `decorator_writer_base` that
wraps any `log_writer_interface` implementation.

The standard decoration chain is:

```
Core Writer → Filtering → Buffering → Encryption → Thread-Safety → Async
```

Each decorator wraps the next and delegates `write()` after applying its concern:

```cpp
// Fluent builder API for composition
auto writer = writer_builder()
    .file("app.log")            // Core writer
    .with_rotation(10_MB, 5)    // Rotating file decorator
    .with_buffering(1000)       // Buffer decorator
    .with_encryption(key)       // Encryption decorator
    .with_async(queue_size)     // Async decorator
    .build();
```

Key implementation details:
- `log_writer_interface` — Pure virtual base: `write() -> VoidResult`
- `decorator_writer_base` — Holds `unique_ptr<log_writer_interface>` to wrapped writer
- `queued_writer_base` — Shared queue management for `async_writer` and `batch_writer`
- `writer_builder` — Fluent API that constructs the decorator chain

## Alternatives Considered

### Monolithic Writer Classes

- **Pros**: Simple implementation, no indirection overhead.
- **Cons**: Combinatorial explosion of classes. Adding encryption to all writers
  requires modifying each one. Violates Open-Closed Principle.

### Template-Based Policy Composition (Mixin)

- **Pros**: Zero runtime overhead (static dispatch), compile-time composition.
- **Cons**: Template complexity for users, no runtime reconfiguration, error
  messages from deep template nesting are impractical, prevents storing
  heterogeneous writers in a container.

### Strategy Pattern with Feature Flags

- **Pros**: Single writer class with boolean flags for each concern.
- **Cons**: God-class anti-pattern, every feature check adds branching to the
  hot path, impossible to extend without modifying the class.

## Consequences

### Positive

- **Open-Closed Principle**: New cross-cutting concerns (e.g., compression, rate
  limiting) are added as new decorator classes without modifying existing writers.
- **Runtime composition**: The `writer_builder` fluent API allows configuration-driven
  writer construction, enabling different compositions per environment.
- **17 writers from 5 cores**: 5 core writers + 12 decorators provide 17+
  combinations without class explosion.
- **Testability**: Each decorator can be unit-tested in isolation by wrapping a
  mock writer.

### Negative

- **Indirection overhead**: Each decorator adds one virtual dispatch (~1-2 ns)
  per log entry. A chain of 4 decorators adds ~4-8 ns. Acceptable given the
  typical I/O cost of logging (microseconds to milliseconds).
- **Ordering matters**: Some decorator orderings are invalid (e.g., encryption
  after async would encrypt in the wrong thread). The `writer_builder` enforces
  valid orderings, but manual composition does not.
- **Debugging complexity**: Stack traces through decorator chains are deeper and
  harder to read.
