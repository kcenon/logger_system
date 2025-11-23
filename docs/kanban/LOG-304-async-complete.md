# LOG-304: Complete Async Processing Implementation

## Metadata

| Field | Value |
|-------|-------|
| **ID** | LOG-304 |
| **Priority** | 🟡 MEDIUM |
| **Est. Duration** | 3-5 days |
| **Dependencies** | None |
| **Status** | TODO |
| **Assignee** | TBD |
| **Created** | 2025-11-23 |

---

## 1. Overview

### 1.1 Problem Statement

Async logging is **partially implemented**. In `logger.cpp` line 246:

```cpp
// TODO: Queue message for async processing when async mode is fully implemented
```

**Current State:**
- Messages with file/line/function info → Uses async path through collector
- Simple messages (no source location) → Synchronous write fallback
- Inconsistent behavior based on message type

### 1.2 Objectives

- Make async path consistent for all message types
- Complete async queue implementation
- Ensure proper flush/shutdown behavior
- Optimize throughput

### 1.3 Scope

- `src/core/logger.cpp` (line 246 and async dispatch)
- `include/kcenon/logger/core/logger.h`
- `src/impl/async/lockfree_queue.h`

---

## 2. Changes

### 2.1 Async Path Completion

```cpp
// In logger.cpp - unified async handling
void logger::log_message(log_level level, std::string_view message) {
    log_entry entry{level, std::string(message), /* ... */};

    if (async_enabled_) {
        // Queue for async processing (both simple and detailed messages)
        async_queue_.push(std::move(entry));
    } else {
        // Synchronous path
        dispatch_to_writers(entry);
    }
}
```

### 2.2 Async Configuration

```cpp
// In logger_builder.h
logger_builder& with_async_mode(bool enabled);
logger_builder& with_async_queue_size(size_t size);
logger_builder& with_async_flush_interval(std::chrono::milliseconds interval);
logger_builder& with_async_overflow_policy(overflow_policy policy);

enum class overflow_policy {
    block,        // Block until space available
    drop_oldest,  // Drop oldest message
    drop_newest,  // Drop current message
    grow          // Dynamically grow queue
};
```

### 2.3 Proper Shutdown

```cpp
// Graceful shutdown sequence
void logger::shutdown() {
    // 1. Stop accepting new messages
    accepting_messages_ = false;

    // 2. Signal async worker to flush
    async_queue_.signal_shutdown();

    // 3. Wait for queue to drain (with timeout)
    if (!async_worker_.join_for(shutdown_timeout_)) {
        // Force flush remaining messages
        force_flush();
    }

    // 4. Close all writers
    for (auto& writer : writers_) {
        writer->close();
    }
}
```

---

## 3. Test Plan

### 3.1 Unit Tests

#### async_queue_test.cpp (6)
- [ ] Single producer single consumer
- [ ] Queue full handling (block policy)
- [ ] Queue full handling (drop policies)
- [ ] Shutdown signaling
- [ ] Queue size configuration
- [ ] Concurrent access (ThreadSanitizer)

#### async_logger_test.cpp (6)
- [ ] Async mode enabled - all messages queued
- [ ] Async mode disabled - synchronous write
- [ ] Simple message async path
- [ ] Detailed message async path
- [ ] Flush on shutdown
- [ ] Timeout handling

### 3.2 Integration Tests

- [ ] High-volume async logging (10K msgs/sec)
- [ ] Graceful shutdown with pending messages
- [ ] Writer errors during async processing
- [ ] Mixed sync/async writers

### 3.3 Performance Tests

- [ ] Throughput: sync vs async
- [ ] Latency percentiles (p50, p95, p99)
- [ ] Memory usage under load
- [ ] Queue backpressure behavior

---

## 4. Checklist

### 4.1 Implementation Checklist

- [ ] Remove TODO at line 246
- [ ] Implement unified async path for all messages
- [ ] Add overflow policy handling
- [ ] Implement graceful shutdown
- [ ] Add flush_interval support
- [ ] Add queue size configuration
- [ ] Update builder with async options

### 4.2 Test Checklist

- [ ] Write 6+ queue tests
- [ ] Write 6+ logger async tests
- [ ] Write integration tests
- [ ] Write performance benchmarks
- [ ] All tests passing
- [ ] ThreadSanitizer clean

### 4.3 Documentation Checklist

- [ ] Document async configuration options
- [ ] Document overflow policies
- [ ] Document shutdown behavior
- [ ] Add performance tuning guide
- [ ] Update CHANGELOG

---

## 5. Acceptance Criteria

1. ✅ All message types use async path when enabled
2. ✅ TODO comment removed
3. ✅ Overflow policies work correctly
4. ✅ Graceful shutdown flushes all messages
5. ✅ Async throughput ≥ 100K msgs/sec
6. ✅ No message loss under normal operation
7. ✅ ThreadSanitizer clean
8. ✅ Test coverage 85%+

---

## 6. Related Files

- `src/core/logger.cpp` line 246 (TODO location)
- `include/kcenon/logger/core/logger.h`
- `include/kcenon/logger/core/logger_builder.h`
- `src/impl/async/lockfree_queue.h`

---

## 7. References

- [LOG-401](LOG-401-multi-producer.md) - MPMC queue (related)
- [PERFORMANCE.md](../guides/PERFORMANCE.md)
