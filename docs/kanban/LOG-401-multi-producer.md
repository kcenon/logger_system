# LOG-401: Implement Multi-Producer Lock-free Queue

## Metadata

| Field | Value |
|-------|-------|
| **ID** | LOG-401 |
| **Priority** | 🟢 LOW |
| **Est. Duration** | 2-3 days |
| **Dependencies** | None |
| **Status** | TODO |
| **Assignee** | TBD |
| **Created** | 2025-11-23 |

---

## 1. Overview

### 1.1 Problem Statement

In `lockfree_queue.h` lines 211-215, the MPMC (Multi-Producer Multi-Consumer) queue is a **stub**:

```cpp
template <typename T, size_t Capacity = 1024>
class lockfree_mpmc_queue {
    static_assert(sizeof(T) == 0,
                  "MPMC queue not yet implemented. "
                  "TODO: Implement if multi-producer scenarios are needed");
};
```

Currently only SPSC (Single-Producer Single-Consumer) is implemented.

### 1.2 Objectives

- Implement lock-free MPMC queue
- Support multiple producer threads
- Maintain high throughput
- Ensure correctness under concurrent access

### 1.3 Scope

- `src/impl/async/lockfree_queue.h`
- Supporting atomic operations and memory ordering

---

## 2. Changes

### 2.1 MPMC Queue Interface

```cpp
template <typename T, size_t Capacity = 1024>
class lockfree_mpmc_queue {
public:
    // Non-blocking operations
    bool try_push(const T& item);
    bool try_push(T&& item);
    bool try_pop(T& item);

    // Blocking operations (with timeout)
    bool push(const T& item, std::chrono::milliseconds timeout);
    bool pop(T& item, std::chrono::milliseconds timeout);

    // Query
    size_t size() const;
    bool empty() const;
    bool full() const;

    // Shutdown support
    void close();
    bool is_closed() const;

private:
    struct cell {
        std::atomic<size_t> sequence;
        T data;
    };

    alignas(64) std::array<cell, Capacity> buffer_;
    alignas(64) std::atomic<size_t> enqueue_pos_;
    alignas(64) std::atomic<size_t> dequeue_pos_;
    std::atomic<bool> closed_;
};
```

### 2.2 Implementation Strategy

Use **bounded MPMC queue** based on Dmitry Vyukov's algorithm:

1. Each cell has a sequence number
2. Producer: CAS on enqueue position, then write data
3. Consumer: CAS on dequeue position, then read data
4. Use cache-line padding to prevent false sharing

### 2.3 Memory Ordering

```cpp
// Producer
bool try_push(T&& item) {
    size_t pos = enqueue_pos_.load(std::memory_order_relaxed);

    for (;;) {
        cell& c = buffer_[pos % Capacity];
        size_t seq = c.sequence.load(std::memory_order_acquire);
        intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos);

        if (diff == 0) {
            if (enqueue_pos_.compare_exchange_weak(pos, pos + 1,
                    std::memory_order_relaxed)) {
                c.data = std::move(item);
                c.sequence.store(pos + 1, std::memory_order_release);
                return true;
            }
        } else if (diff < 0) {
            return false; // Queue full
        } else {
            pos = enqueue_pos_.load(std::memory_order_relaxed);
        }
    }
}
```

---

## 3. Test Plan

### 3.1 Correctness Tests

#### mpmc_queue_test.cpp (10)
- [ ] Single producer, single consumer
- [ ] Multiple producers, single consumer
- [ ] Single producer, multiple consumers
- [ ] Multiple producers, multiple consumers
- [ ] Queue full behavior
- [ ] Queue empty behavior
- [ ] Close/shutdown behavior
- [ ] Timeout behavior (blocking operations)
- [ ] Element ordering preservation
- [ ] No data corruption under stress

### 3.2 Stress Tests

- [ ] 4 producers × 4 consumers, 1M messages
- [ ] High contention (small queue, many threads)
- [ ] Memory ordering (weak memory model simulation)

### 3.3 Performance Benchmarks

- [ ] Throughput: SPSC vs MPMC comparison
- [ ] Latency percentiles under various thread counts
- [ ] Scaling with producer/consumer count

---

## 4. Checklist

### 4.1 Implementation Checklist

- [ ] Remove `static_assert` stub
- [ ] Implement `try_push()` (both overloads)
- [ ] Implement `try_pop()`
- [ ] Implement blocking `push()` with timeout
- [ ] Implement blocking `pop()` with timeout
- [ ] Implement `size()`, `empty()`, `full()`
- [ ] Implement `close()` and `is_closed()`
- [ ] Add cache-line padding for false sharing prevention
- [ ] Verify memory ordering correctness

### 4.2 Test Checklist

- [ ] Write 10+ correctness tests
- [ ] Write stress tests
- [ ] Write benchmarks
- [ ] All tests passing
- [ ] ThreadSanitizer clean
- [ ] AddressSanitizer clean

### 4.3 Documentation Checklist

- [ ] Document MPMC queue API
- [ ] Document thread safety guarantees
- [ ] Document performance characteristics
- [ ] Update CHANGELOG

---

## 5. Acceptance Criteria

1. ✅ MPMC queue fully implemented (no static_assert)
2. ✅ Support 16+ concurrent producers
3. ✅ Support 16+ concurrent consumers
4. ✅ No data loss or corruption under stress
5. ✅ Throughput ≥ 50% of SPSC for 4×4 threads
6. ✅ All sanitizers pass (TSan, ASan)
7. ✅ Test coverage 95%+

---

## 6. Related Files

- `src/impl/async/lockfree_queue.h` (lines 211-215)
- SPSC implementation in same file (reference)

---

## 7. References

- [Vyukov's MPMC queue](https://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue)
- [C++ Memory Model](https://en.cppreference.com/w/cpp/atomic/memory_order)
- [LOG-304](LOG-304-async-complete.md) - Async processing (uses queue)
