# Phase 5: Architecture Refactoring

**Phase:** 5/5
**Priority:** 🟢 Low (Optional)
**Estimated Duration:** 6-8 weeks
**Objective:** Improve long-term maintainability
**Status:** ⬜ Not Started
**Dependencies:** Phase 1-4 completion required
**Warning:** ⚠️ Impacts backward compatibility, requires careful review

---

## Table of Contents

1. [Overview](#overview)
2. [Work Items](#work-items)
3. [Major Refactoring](#major-refactoring)
4. [Backward Compatibility Strategy](#backward-compatibility-strategy)
5. [Migration Guide](#migration-guide)
6. [Validation Criteria](#validation-criteria)

---

## Overview

### Purpose
Phase 5 focuses on architectural improvements for **long-term maintainability**. This phase is **optional** and impacts backward compatibility.

### Major Refactoring
1. **PIMPL → Template Policy:** Remove indirect calls, compile-time optimization
2. **Unified Metrics System:** Integrate logger_metrics + common::metrics_snapshot
3. **Writer Pipeline:** Chaining optimization
4. **Backward Compatibility Layer:** Maintain existing API

### Expected Benefits
- ✅ 5-10% performance improvement from removing indirect calls
- ✅ Compile-time optimization
- ✅ Improved metrics consistency

### Warnings
⚠️ **This phase requires careful review:**
- May break backward compatibility
- Large-scale code changes
- Possible build time increase
- Complex template error messages

---

## Work Items

### Task 5.1: PIMPL → Template Policy Conversion
- [ ] 5.1.1 Design logger_policy
- [ ] 5.1.2 Implement logger_t template class
- [ ] 5.1.3 Provide type aliases
- [ ] 5.1.4 Verify benchmarks
- [ ] 5.1.5 Create backward compatibility layer
- [ ] 5.1.6 Migrate unit tests

### Task 5.2: Unified Metrics System
- [ ] 5.2.1 Design unified_metrics
- [ ] 5.2.2 Integrate logger_metrics
- [ ] 5.2.3 Create common::metrics_snapshot adapter
- [ ] 5.2.4 Write unit tests

### Task 5.3: Writer Pipeline Optimization
- [ ] 5.3.1 Design writer_pipeline
- [ ] 5.3.2 Integrate filters/transformers
- [ ] 5.3.3 Single-threaded processing
- [ ] 5.3.4 Verify benchmarks

### Task 5.4: Backward Compatibility Layer
- [ ] 5.4.1 Create legacy_logger alias
- [ ] 5.4.2 Create migration tools
- [ ] 5.4.3 Documentation

---

## Major Refactoring

### Refactoring 5.1: PIMPL → Template Policy

#### Current Structure (PIMPL)
**File:** `include/kcenon/logger/core/logger.h`

```cpp
class logger {
public:
    logger(bool async = true, size_t buffer_size = 8192);
    ~logger();

    void log(log_level level, const std::string& message);
    // ...

private:
    class impl;  // ⚠️ PIMPL: 2-level indirect calls
    std::unique_ptr<impl> pimpl_;
};
```

**Problems:**
1. **Indirect calls:** logger → impl → actual implementation
2. **Heap allocation:** `std::unique_ptr<impl>` creation
3. **No compile-time optimization:** Not templates, difficult to inline

#### Improved Structure: Template Policy

```cpp
// logger_policy.h

namespace kcenon::logger {

// Policy: Async processing
template<bool Enabled>
struct async_policy {
    static constexpr bool is_async = Enabled;
};

// Policy: Buffer size
template<size_t Size>
struct buffer_policy {
    static constexpr size_t buffer_size = Size;
};

// Policy: Queue type
template<typename QueueType>
struct queue_policy {
    using queue_type = QueueType;
};

// Default policies
using default_async = async_policy<true>;
using default_buffer = buffer_policy<8192>;
using default_queue = queue_policy<
    lockfree_spsc_queue<log_entry, 10000>
>;

} // namespace
```

```cpp
// logger_template.h

template<
    typename AsyncPolicy = default_async,
    typename BufferPolicy = default_buffer,
    typename QueuePolicy = default_queue
>
class logger_t {
public:
    logger_t() {
        // ✅ All configuration determined at compile time
        if constexpr (AsyncPolicy::is_async) {
            // Async initialization
            queue_.reserve(QueuePolicy::queue_type::capacity);
            worker_thread_ = std::thread([this] { process_loop(); });
        }
    }

    void log(log_level level, const std::string& message) {
        if constexpr (AsyncPolicy::is_async) {
            // ✅ Compile-time branching (no runtime overhead)
            enqueue_async(level, message);
        } else {
            // Sync processing
            write_sync(level, message);
        }
    }

private:
    // ✅ Member variables also conditionally compiled
    typename std::conditional<
        AsyncPolicy::is_async,
        typename QueuePolicy::queue_type,
        std::monostate  // No queue needed in sync mode
    >::type queue_;

    typename std::conditional<
        AsyncPolicy::is_async,
        std::thread,
        std::monostate
    >::type worker_thread_;

    std::array<char, BufferPolicy::buffer_size> buffer_;
};
```

#### Type Aliases Provided

```cpp
// logger.h (aliases)

namespace kcenon::logger {

// Default logger (async, 8KB buffer)
using logger = logger_t<>;

// Sync logger
using sync_logger = logger_t<
    async_policy<false>
>;

// High-performance logger (large buffer, lock-free queue)
using high_performance_logger = logger_t<
    async_policy<true>,
    buffer_policy<32768>,
    queue_policy<lockfree_spsc_queue<log_entry, 50000>>
>;

// Low-latency logger (small buffer, small queue)
using low_latency_logger = logger_t<
    async_policy<true>,
    buffer_policy<1024>,
    queue_policy<lockfree_spsc_queue<log_entry, 1000>>
>;

} // namespace
```

#### Backward Compatibility Layer

```cpp
// legacy_logger.h

namespace kcenon::logger {

// Maintain existing API
class legacy_logger {
public:
    legacy_logger(bool async = true, size_t buffer_size = 8192)
        : impl_(create_impl(async, buffer_size)) {
    }

    void log(log_level level, const std::string& message) {
        impl_->log(level, message);
    }

    // ... other methods

private:
    static std::unique_ptr<logger_interface> create_impl(
        bool async,
        size_t buffer_size
    ) {
        if (async) {
            if (buffer_size == 8192) {
                return std::make_unique<logger>();
            } else if (buffer_size == 32768) {
                return std::make_unique<high_performance_logger>();
            }
            // Custom buffer sizes not supported (warning log)
            return std::make_unique<logger>();
        } else {
            return std::make_unique<sync_logger>();
        }
    }

    std::unique_ptr<logger_interface> impl_;
};

// Alias for backward compatibility with existing code
#ifndef LOGGER_USE_NEW_API
    using logger = legacy_logger;
#endif

} // namespace
```

**Expected Performance Improvement:**
```
Current (PIMPL):       logger->log() → impl->log() → actual implementation
                       2 indirect calls + virtual function call

Improved (Template):   logger->log() → actual implementation (inlined)
                       0 indirect calls + compile-time optimization

Benchmark Estimate:    5-10% performance improvement
```

---

### Refactoring 5.2: Unified Metrics System

#### Current Structure (Dual Management)

```cpp
// logger_metrics.h
struct logger_metrics {
    std::atomic<uint64_t> messages_logged{0};
    std::atomic<uint64_t> messages_dropped{0};
    // ...
};

// common::interfaces::IMonitorable
class logger : public IMonitorable {
    metrics_snapshot get_monitoring_data() const override {
        // Convert logger_metrics → metrics_snapshot
        // ⚠️ Dual management
    }
};
```

**Problems:**
- Separate management of logger_metrics and metrics_snapshot
- Synchronization overhead
- Difficult to guarantee consistency

#### Improved Structure: Unified Metrics

```cpp
// unified_metrics.h

namespace kcenon::logger {

class unified_metrics {
public:
    // Record metrics
    void record_message_logged() {
        ++messages_logged_;
    }

    void record_message_dropped() {
        ++messages_dropped_;
    }

    void record_enqueue_time(std::chrono::nanoseconds duration) {
        enqueue_times_.push(duration.count());
    }

    // Export as logger_metrics format
    logger_metrics to_logger_metrics() const {
        return logger_metrics{
            .messages_logged = messages_logged_.load(),
            .messages_dropped = messages_dropped_.load(),
            // ...
        };
    }

    // Export as common::metrics_snapshot format
#ifdef BUILD_WITH_COMMON_SYSTEM
    common::interfaces::metrics_snapshot to_common_metrics() const {
        common::interfaces::metrics_snapshot snapshot;

        snapshot.add_counter("logger.messages_logged", messages_logged_.load());
        snapshot.add_counter("logger.messages_dropped", messages_dropped_.load());
        snapshot.add_gauge("logger.queue_size", current_queue_size_.load());

        // Calculate average enqueue time
        auto avg = enqueue_times_.average();
        snapshot.add_gauge("logger.avg_enqueue_time_ns", avg);

        return snapshot;
    }
#endif

private:
    std::atomic<uint64_t> messages_logged_{0};
    std::atomic<uint64_t> messages_dropped_{0};
    std::atomic<size_t> current_queue_size_{0};

    // For statistics calculation (thread-safe ring buffer)
    thread_safe_ring_buffer<uint64_t, 1000> enqueue_times_;
};

} // namespace
```

**Benefits:**
- Single source of truth
- Eliminates synchronization overhead
- Guarantees consistency

---

### Refactoring 5.3: Writer Pipeline Optimization

#### Current Structure (Multiple Wrappers)

```cpp
// Current: Multiple wrapper chaining
auto writer = std::make_unique<async_writer>(        // Wrapper 1
    std::make_unique<batch_writer>(                   // Wrapper 2
        std::make_unique<encrypted_writer>(           // Wrapper 3
            std::make_unique<file_writer>("app.log")  // Actual writer
        )
    )
);
```

**Problems:**
- Each wrapper has independent queue/thread
- Increased context switching
- Increased memory copying

#### Improved Structure: Single Pipeline

```cpp
// writer_pipeline.h

namespace kcenon::logger {

class writer_pipeline {
public:
    // Add filter (preprocessing)
    writer_pipeline& add_filter(std::unique_ptr<log_filter> filter) {
        filters_.push_back(std::move(filter));
        return *this;
    }

    // Add transformer (formatting, encryption, etc.)
    writer_pipeline& add_transformer(std::unique_ptr<log_transformer> transformer) {
        transformers_.push_back(std::move(transformer));
        return *this;
    }

    // Final output
    writer_pipeline& set_sink(std::unique_ptr<base_writer> sink) {
        sink_ = std::move(sink);
        return *this;
    }

    // Execute pipeline
    result_void write(log_entry entry) {
        // 1. Apply filters
        for (const auto& filter : filters_) {
            if (!filter->should_process(entry)) {
                return {};  // Filtered out
            }
        }

        // 2. Apply transformations (sequential processing in single thread)
        for (const auto& transformer : transformers_) {
            auto result = transformer->transform(entry);
            if (!result) return result.error();
            entry = result.value();
        }

        // 3. Final output
        return sink_->write(entry);
    }

private:
    std::vector<std::unique_ptr<log_filter>> filters_;
    std::vector<std::unique_ptr<log_transformer>> transformers_;
    std::unique_ptr<base_writer> sink_;
};

// Transformer interface
class log_transformer {
public:
    virtual ~log_transformer() = default;
    virtual result<log_entry> transform(log_entry entry) = 0;
};

// Example: Encryption transformer
class encryption_transformer : public log_transformer {
public:
    result<log_entry> transform(log_entry entry) override {
        // Encrypt message
        entry.message = encrypt(entry.message, key_);
        return entry;
    }

private:
    secure_key key_;
};

} // namespace
```

#### Usage Example

```cpp
// Integrate into single pipeline
auto pipeline = writer_pipeline()
    .add_filter(std::make_unique<level_filter>(log_level::info))
    .add_transformer(std::make_unique<encryption_transformer>(key))
    .add_transformer(std::make_unique<compression_transformer>())
    .set_sink(std::make_unique<file_writer>("app.log"));

// Integrate with logger (single async thread)
auto logger = logger_builder()
    .add_writer("pipeline", std::move(pipeline))
    .build()
    .value();
```

**Benefits:**
- Reduced thread count (multiple wrappers → single pipeline)
- Reduced context switching
- Reduced memory copying

---

## Backward Compatibility Strategy

### 1. Phased Migration

```cpp
// Phase 5.1: Introduce new API (maintain existing API)
#define LOGGER_USE_NEW_API 0  // Default: use existing API

#if LOGGER_USE_NEW_API
    using logger = logger_t<>;  // New API
#else
    using logger = legacy_logger;  // Existing API
#endif
```

### 2. Migration Tools

```python
# scripts/migrate_to_new_api.py

import re
import sys

def migrate_logger_creation(code):
    # Existing: logger(true, 8192)
    # New: logger<> (default)

    patterns = [
        (r'logger\(true,\s*8192\)', 'logger<>'),
        (r'logger\(false,\s*\d+\)', 'sync_logger<>'),
    ]

    for pattern, replacement in patterns:
        code = re.sub(pattern, replacement, code)

    return code

if __name__ == '__main__':
    with open(sys.argv[1], 'r') as f:
        code = f.read()

    migrated = migrate_logger_creation(code)

    with open(sys.argv[1], 'w') as f:
        f.write(migrated)
```

### 3. Warning Messages

```cpp
// legacy_logger.h

#ifdef LOGGER_DEPRECATED_WARNING
    #warning "legacy_logger is deprecated. Please migrate to logger<>."
#endif
```

---

## Migration Guide

### Migration Steps

#### Step 1: Verify in Test Environment

```bash
# Enable new API
cmake -B build -DLOGGER_USE_NEW_API=ON

# Build and test
cmake --build build
ctest --test-dir build
```

#### Step 2: Code Migration

```cpp
// Existing code
auto logger = std::make_shared<kcenon::logger::logger>(true, 8192);

// New code
auto logger = std::make_shared<kcenon::logger::logger<>>();
```

#### Step 3: Production Deployment

```bash
# Gradual rollout
# 1. Canary deployment (5%)
# 2. Monitoring (24 hours)
# 3. Full deployment (100%)
```

---

## Validation Criteria

| Validation Item | Criteria | Measurement Method |
|----------|------|----------|
| **Performance Improvement** | 5% or more | Benchmark |
| **Compile Time** | Regression < 10% | CI/CD |
| **Binary Size** | Increase < 20% | size command |
| **Backward Compatibility** | 100% | Run existing examples |
| **All Tests** | 100% pass | CTest |

---

## Work Checklist

### Pre-Work Preparation
- [ ] Confirm Phase 1-4 completion
- [ ] Assess backward compatibility impact
- [ ] Stakeholder approval
- [ ] Create Git branch (`feature/phase5-architecture`)

### During Work
- [ ] Task 5.1: PIMPL → Template conversion
- [ ] Task 5.2: Unified metrics
- [ ] Task 5.3: Writer pipeline
- [ ] Task 5.4: Backward compatibility layer

### Post-Work Validation
- [ ] Confirm performance improvement (5% or more)
- [ ] Verify backward compatibility
- [ ] Complete code review
- [ ] Write migration guide

### Deployment
- [ ] Create Git tag (`v2.0.0`)
- [ ] Write Release Notes
- [ ] Migration support

---

## Conclusion

Phase 5 is **optional** and should only be undertaken under the following conditions:

### Recommended Conditions
✅ All of Phase 1-4 completed
✅ Long-term maintenance plan in place
✅ Backward compatibility breaking acceptable
✅ Sufficient testing resources secured

### Not Required Conditions
❌ Short-term projects
❌ Backward compatibility mandatory
❌ Difficulty handling template error messages
❌ Resource constraints

### Alternatives
Phases 1-4 alone provide sufficient improvements without Phase 5:
- Phase 1: Stability assurance
- Phase 2: Performance improvement
- Phase 3: Maintainability improvement
- Phase 4: Security hardening

---

**Start Date:** TBD
**Assignee:** TBD
**Reviewer:** TBD
**Approver:** TBD (Phase 5 requires senior approval)
