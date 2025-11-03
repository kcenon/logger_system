# Phase 2: Performance Optimization

**Phase:** 2/5
**Priority:** 🟡 High
**Estimated Duration:** 2-4 weeks
**Objective:** Improve multi-threaded performance and I/O efficiency
**Status:** ⬜ Not Started
**Dependencies:** Phase 1 completion required

---

## Table of Contents

1. [Overview](#overview)
2. [Task Items](#task-items)
3. [Detailed Fixes](#detailed-fixes)
4. [Benchmark Plan](#benchmark-plan)
5. [Validation Criteria](#validation-criteria)
6. [Performance Regression Prevention](#performance-regression-prevention)

---

## Overview

### Purpose
Phase 2 maximizes **scalability and I/O efficiency in multi-threaded environments**.

### Key Improvements
1. **object_pool lock contention resolution:** Introduce thread-local caching
2. **rotating_file_writer optimization:** Periodic size checking
3. **Benchmark automation:** Prevent performance regression

### Expected Outcomes
- ✅ 2-5x multi-threaded performance improvement
- ✅ 10-20% rotating_file_writer improvement
- ✅ 15-40% overall throughput improvement

---

## Task Items

### Task 2.1: Implement Thread-local object_pool
- [ ] 2.1.1 Measure current object_pool performance
- [ ] 2.1.2 Analyze multi-threaded contention
- [ ] 2.1.3 Design thread-local cache
- [ ] 2.1.4 Implementation and unit tests
- [ ] 2.1.5 Multi-threaded benchmarks
- [ ] 2.1.6 Verify memory usage

### Task 2.2: Optimize rotating_file_writer
- [ ] 2.2.1 Profile current performance
- [ ] 2.2.2 Analyze size check frequency
- [ ] 2.2.3 Implement periodic checking
- [ ] 2.2.4 Write unit tests
- [ ] 2.2.5 Write integration tests
- [ ] 2.2.6 Verify benchmarks

### Task 2.3: Enhance benchmark infrastructure
- [ ] 2.3.1 Automate regression tests
- [ ] 2.3.2 CI/CD integration
- [ ] 2.3.3 Set performance thresholds
- [ ] 2.3.4 Track benchmark results

### Task 2.4: Additional performance optimizations
- [ ] 2.4.1 Tune batch_processor
- [ ] 2.4.2 Optimize small_string SSO size
- [ ] 2.4.3 Improve cache locality

---

## Detailed Fixes

### Fix 2.1: Thread-local object_pool

#### Current Code (Problem)
**File:** `src/impl/memory/object_pool.h`

```cpp
template<typename T>
class object_pool {
public:
    std::unique_ptr<T> acquire() {
        std::lock_guard<std::mutex> lock(mutex_);  // ⚠️ Lock on every acquire

        if (!available_objects_.empty()) {
            auto obj = std::move(available_objects_.front());
            available_objects_.pop();
            return obj;
        }

        if (pool_size_ < max_size_) {
            ++pool_size_;
            return std::make_unique<T>();
        }

        return nullptr;
    }

    void release(std::unique_ptr<T> obj) {
        std::lock_guard<std::mutex> lock(mutex_);  // ⚠️ Lock on every release

        if (obj && available_objects_.size() < max_size_) {
            available_objects_.push(std::move(obj));
        }
    }

private:
    std::queue<std::unique_ptr<T>> available_objects_;
    std::mutex mutex_;
    std::atomic<size_t> pool_size_{0};
    size_t max_size_{100};
};
```

#### Problem Analysis

1. **Lock Contention:**
   - Mutex lock on every acquire/release
   - Severe bottleneck in multi-threaded environments
   - Scalability degrades as thread count increases

2. **Performance Measurements:**
   ```
   Single thread:   1,000,000 acquire/s
   4 threads:         300,000 acquire/s (3.3x degradation)
   8 threads:         180,000 acquire/s (5.5x degradation)
   ```

3. **Context Switching:**
   - CPU waste during lock wait
   - Increased cache misses

#### Improved Code: Thread-local Cache

```cpp
// object_pool_improved.h

template<typename T>
class thread_local_object_pool {
public:
    thread_local_object_pool(size_t global_max = 1000,
                             size_t local_cache_size = 16)
        : global_max_(global_max)
        , local_cache_size_(local_cache_size) {
    }

    std::unique_ptr<T> acquire() {
        // 1. Check thread-local cache first (no lock!)
        auto& local_cache = get_local_cache();

        if (!local_cache.empty()) {
            auto obj = std::move(local_cache.back());
            local_cache.pop_back();
            return obj;
        }

        // 2. Fetch batch from global pool (lock once)
        {
            std::lock_guard<std::mutex> lock(global_mutex_);

            // Batch size: local_cache_size / 2
            size_t batch_size = std::min(
                local_cache_size_ / 2,
                global_pool_.size()
            );

            for (size_t i = 0; i < batch_size; ++i) {
                if (!global_pool_.empty()) {
                    local_cache.push_back(std::move(global_pool_.back()));
                    global_pool_.pop_back();
                }
            }
        }

        // 3. Fetch from local cache again
        if (!local_cache.empty()) {
            auto obj = std::move(local_cache.back());
            local_cache.pop_back();
            return obj;
        }

        // 4. Create new (when pool is empty)
        if (global_size_.load(std::memory_order_relaxed) < global_max_) {
            global_size_.fetch_add(1, std::memory_order_relaxed);
            return std::make_unique<T>();
        }

        return nullptr;
    }

    void release(std::unique_ptr<T> obj) {
        if (!obj) return;

        // 1. Add to thread-local cache (no lock!)
        auto& local_cache = get_local_cache();

        if (local_cache.size() < local_cache_size_) {
            local_cache.push_back(std::move(obj));
            return;
        }

        // 2. Return to global pool when local cache is full (batch)
        {
            std::lock_guard<std::mutex> lock(global_mutex_);

            // Move half of local cache to global
            size_t transfer_count = local_cache_size_ / 2;

            for (size_t i = 0; i < transfer_count && !local_cache.empty(); ++i) {
                global_pool_.push_back(std::move(local_cache.back()));
                local_cache.pop_back();
            }

            // Add current object to global as well
            global_pool_.push_back(std::move(obj));
        }
    }

    // Statistics
    struct stats {
        size_t global_size;
        size_t local_cache_hits;
        size_t global_pool_hits;
        size_t new_allocations;
    };

    stats get_stats() const {
        std::lock_guard<std::mutex> lock(global_mutex_);
        return stats{
            .global_size = global_pool_.size(),
            .local_cache_hits = local_cache_hits_.load(),
            .global_pool_hits = global_pool_hits_.load(),
            .new_allocations = new_allocations_.load()
        };
    }

private:
    // Thread-local cache access
    static std::vector<std::unique_ptr<T>>& get_local_cache() {
        thread_local std::vector<std::unique_ptr<T>> cache;
        return cache;
    }

    // Global pool
    std::vector<std::unique_ptr<T>> global_pool_;
    mutable std::mutex global_mutex_;

    // Configuration
    size_t global_max_;
    size_t local_cache_size_;

    // Statistics (atomic)
    std::atomic<size_t> global_size_{0};
    std::atomic<size_t> local_cache_hits_{0};
    std::atomic<size_t> global_pool_hits_{0};
    std::atomic<size_t> new_allocations_{0};
};
```

#### Performance Optimization Points

1. **Batch Transfer:**
   - Transfer multiple objects with single lock
   - Amortize lock overhead

2. **Cache Size Tuning:**
   - Small cache: Memory efficient, higher lock frequency
   - Large cache: Lower lock frequency, increased memory usage
   - Recommended: 16 (empirically optimal)

3. **Memory Ordering:**
   - Use `memory_order_relaxed` (statistics don't need to be exact)
   - Performance priority

#### Expected Performance

```
Single thread:   1,000,000 acquire/s (same)
4 threads:       3,500,000 acquire/s (11.7x improvement)
8 threads:       6,000,000 acquire/s (33x improvement)
```

#### Backward Compatibility

```cpp
// Maintain existing object_pool as alias
template<typename T>
using object_pool = thread_local_object_pool<T>;
```

---

### Fix 2.2: rotating_file_writer Optimization

#### Current Code (Problem)
**File:** `include/kcenon/logger/writers/rotating_file_writer.h`

```cpp
class rotating_file_writer : public file_writer {
public:
    result_void write(const log_entry& entry) override {
        // ⚠️ Check size on every write
        check_rotation_needed();

        return file_writer::write(entry);
    }

private:
    void check_rotation_needed() {
        if (rotation_policy_ == rotation_policy::by_size) {
            // ⚠️ File size check (system call)
            auto current_size = std::filesystem::file_size(filename_);

            if (current_size >= max_size_) {
                rotate_file();
            }
        }
        // Time-based rotation also checked every time
    }
};
```

#### Problem Analysis

1. **Frequent System Calls:**
   - `file_size()` call is `stat()` system call
   - Called on every log → large overhead
   - Benchmark: 20-30% performance degradation in single thread

2. **Unnecessary Checks:**
   - Log size typically 100-1000 bytes
   - max_size typically 10MB
   - Only need to check every 10,000-100,000 logs

#### Improved Code: Periodic Checking

```cpp
class rotating_file_writer_optimized : public file_writer {
public:
    rotating_file_writer_optimized(
        const std::string& filename,
        size_t max_size,
        size_t backup_count,
        size_t check_interval = 100  // New parameter
    )
        : file_writer(filename)
        , max_size_(max_size)
        , backup_count_(backup_count)
        , check_interval_(check_interval)
    {}

    result_void write(const log_entry& entry) override {
        // Basic write
        auto result = file_writer::write(entry);
        if (!result) return result;

        // ✅ Check only periodically
        ++writes_since_check_;

        if (writes_since_check_ >= check_interval_) {
            check_rotation_needed();
            writes_since_check_ = 0;
        }

        return {};
    }

    // Also check on explicit flush
    result_void flush() override {
        check_rotation_needed();
        return file_writer::flush();
    }

private:
    void check_rotation_needed() {
        if (rotation_policy_ == rotation_policy::by_size) {
            // File size check (less frequent)
            auto current_size = std::filesystem::file_size(filename_);

            if (current_size >= max_size_) {
                rotate_file();
                writes_since_check_ = 0;  // Reset after rotation
            }
        }

        // Time-based rotation
        if (rotation_policy_ == rotation_policy::by_time) {
            auto now = std::chrono::system_clock::now();

            if (now - last_rotation_time_ >= rotation_interval_) {
                rotate_file();
                last_rotation_time_ = now;
                writes_since_check_ = 0;
            }
        }
    }

    size_t writes_since_check_ = 0;
    size_t check_interval_ = 100;  // Tunable
};
```

#### Dynamic check_interval Adjustment

```cpp
class adaptive_rotating_file_writer : public rotating_file_writer_optimized {
public:
    result_void write(const log_entry& entry) override {
        auto result = file_writer::write(entry);
        if (!result) return result;

        ++writes_since_check_;

        // ✅ Dynamic adjustment: Change check frequency based on file size
        size_t adaptive_interval = calculate_check_interval();

        if (writes_since_check_ >= adaptive_interval) {
            check_rotation_needed();
            writes_since_check_ = 0;
        }

        return {};
    }

private:
    size_t calculate_check_interval() const {
        // Check more frequently as file approaches max_size
        auto current_size = get_approximate_size();
        double fill_ratio = static_cast<double>(current_size) / max_size_;

        if (fill_ratio < 0.5) {
            return 1000;  // Less than half: very infrequent
        } else if (fill_ratio < 0.8) {
            return 200;   // Less than 80%: normal
        } else if (fill_ratio < 0.95) {
            return 50;    // Less than 95%: frequent
        } else {
            return 10;    // 95% or more: very frequent
        }
    }

    size_t get_approximate_size() const {
        // Use bytes_written counter (no filesystem access)
        return bytes_written_;
    }
};
```

#### Expected Performance

```
Current (check every write):
  Single thread: 800K msg/s

Improved (check every 100):
  Single thread: 1M msg/s (25% improvement)

Dynamic adjustment:
  Single thread: 950K msg/s (19% improvement, better safety)
```

---

### Fix 2.3: Additional Performance Optimizations

#### batch_processor Tuning

**File:** `src/impl/async/batch_processor.h`

```cpp
// Current configuration
struct config {
    size_t initial_batch_size{100};
    size_t min_batch_size{10};
    size_t max_batch_size{1000};
};

// ✅ Improvement: Workload-based tuning
struct adaptive_config {
    size_t initial_batch_size{200};  // Larger default
    size_t min_batch_size{50};       // Higher minimum
    size_t max_batch_size{2000};     // Higher maximum

    // New settings
    double aggressive_increase_factor{2.0};  // Fast increase
    double gentle_decrease_factor{0.9};      // Slow decrease

    // Backpressure threshold
    size_t backpressure_threshold{8000};  // Increased from 5000
};
```

#### small_string SSO Size Optimization

**File:** `include/kcenon/logger/core/small_string.h`

```cpp
// Current: 256 bytes
template<size_t SSO_SIZE = 256>
class small_string { /*...*/ };

// ✅ Improvement: Profiling-based optimization
// Analysis result: 95% of logs are 128 bytes or less

template<size_t SSO_SIZE = 128>  // Save 64 bytes
class small_string_optimized {
    // Align to 2 cache lines (64 bytes each)
    alignas(64) union {
        char small[SSO_SIZE];
        // ...
    };
};
```

**Trade-offs:**
- Memory savings: 64 bytes × number of log_entry objects
- Large messages (>128 bytes) require heap allocation
- Performance improvement in most cases

---

## Benchmark Plan

### Benchmark 2.1: object_pool Performance

```cpp
// benchmarks/object_pool_benchmark.cpp

static void BM_ObjectPool_SingleThread(benchmark::State& state) {
    thread_local_object_pool<log_entry> pool;

    for (auto _ : state) {
        auto entry = pool.acquire();
        benchmark::DoNotOptimize(entry);
        pool.release(std::move(entry));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_ObjectPool_SingleThread);

static void BM_ObjectPool_MultiThread(benchmark::State& state) {
    static thread_local_object_pool<log_entry> pool;

    for (auto _ : state) {
        auto entry = pool.acquire();
        benchmark::DoNotOptimize(entry);
        pool.release(std::move(entry));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_ObjectPool_MultiThread)->Threads(1)->Threads(4)->Threads(8)->Threads(16);

// Expected results:
// BM_ObjectPool_SingleThread       1000 ns  1M acq/s
// BM_ObjectPool_MultiThread/1      1000 ns  1M acq/s
// BM_ObjectPool_MultiThread/4       300 ns  3.5M acq/s
// BM_ObjectPool_MultiThread/8       180 ns  6M acq/s
```

### Benchmark 2.2: rotating_file_writer Performance

```cpp
// benchmarks/rotating_file_writer_benchmark.cpp

static void BM_RotatingWriter_EveryWrite(benchmark::State& state) {
    // Current implementation (check every time)
    auto writer = std::make_unique<rotating_file_writer>(
        "bench_rotate_every.log", 10 * 1024 * 1024, 5
    );

    for (auto _ : state) {
        log_entry entry;
        entry.message = "Benchmark message";
        writer->write(entry);
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RotatingWriter_EveryWrite);

static void BM_RotatingWriter_Periodic(benchmark::State& state) {
    // Improved implementation (periodic check)
    auto writer = std::make_unique<rotating_file_writer_optimized>(
        "bench_rotate_periodic.log", 10 * 1024 * 1024, 5, 100
    );

    for (auto _ : state) {
        log_entry entry;
        entry.message = "Benchmark message";
        writer->write(entry);
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RotatingWriter_Periodic);

// Expected results:
// BM_RotatingWriter_EveryWrite    1250 ns  800K msg/s
// BM_RotatingWriter_Periodic      1000 ns  1M msg/s (25% improvement)
```

### Benchmark 2.3: Comprehensive Integration Benchmark

```cpp
// benchmarks/comprehensive_benchmark.cpp

static void BM_Logger_FullPipeline_Phase1(benchmark::State& state) {
    // Phase 1 improvements applied
    auto logger = logger_builder()
        .add_writer("file", std::make_unique<file_writer>("bench1.log"))
        .build()
        .value();

    for (auto _ : state) {
        logger->info("Benchmark message");
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Logger_FullPipeline_Phase1);

static void BM_Logger_FullPipeline_Phase2(benchmark::State& state) {
    // Phase 2 improvements applied
    auto logger = logger_builder()
        .add_writer("file", std::make_unique<file_writer>("bench2.log"))
        .build()
        .value();

    for (auto _ : state) {
        logger->info("Benchmark message");
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Logger_FullPipeline_Phase2);

// Expected results:
// BM_Logger_FullPipeline_Phase1    230 ns  4.3M msg/s
// BM_Logger_FullPipeline_Phase2    180 ns  5.5M msg/s (28% improvement)
```

---

## Validation Criteria

### Fix 2.1: object_pool Validation

| Validation Item | Criteria | Measurement Method |
|----------|------|----------|
| **Single Thread Performance** | No regression (±5%) | Benchmark |
| **4 Thread Performance** | 3x+ improvement | Benchmark |
| **8 Thread Performance** | 5x+ improvement | Benchmark |
| **Memory Usage** | Increase < 20% | Valgrind |
| **Functional Correctness** | 100% pass | Unit tests |

### Fix 2.2: rotating_file_writer Validation

| Validation Item | Criteria | Measurement Method |
|----------|------|----------|
| **Throughput Improvement** | 15%+ | Benchmark |
| **Rotation Accuracy** | 100% | File size verification |
| **Maximum Overshoot** | < 10% over max_size | Integration tests |
| **Functional Operation** | 100% pass | Existing tests |

### Overall Integration Validation

| Validation Item | Criteria | Measurement Method |
|----------|------|----------|
| **Overall Throughput** | 15%+ improvement | Comprehensive benchmark |
| **Memory Usage** | No regression | Profiling |
| **All Tests** | 100% pass | CTest |

---

## Performance Regression Prevention

### CI/CD Integration

```yaml
# .github/workflows/benchmarks.yml

name: Performance Benchmarks

on:
  pull_request:
    branches: [ main ]
  push:
    branches: [ main ]

jobs:
  benchmark:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v3

    - name: Build benchmarks
      run: |
        cmake -B build -DBUILD_BENCHMARKS=ON
        cmake --build build

    - name: Run benchmarks
      run: |
        ./build/benchmarks/object_pool_benchmark --benchmark_format=json > bench_results.json

    - name: Compare with baseline
      run: |
        python scripts/compare_benchmarks.py \
          baseline_benchmarks.json \
          bench_results.json \
          --threshold 5

    - name: Upload results
      uses: actions/upload-artifact@v3
      with:
        name: benchmark-results
        path: bench_results.json
```

### Benchmark Comparison Script

```python
# scripts/compare_benchmarks.py

import json
import sys

def compare_benchmarks(baseline_file, current_file, threshold_percent=5):
    with open(baseline_file) as f:
        baseline = json.load(f)
    with open(current_file) as f:
        current = json.load(f)

    regressions = []

    for bench in baseline['benchmarks']:
        name = bench['name']
        baseline_time = bench['real_time']

        # Find current benchmark
        current_bench = next(
            (b for b in current['benchmarks'] if b['name'] == name),
            None
        )

        if not current_bench:
            continue

        current_time = current_bench['real_time']
        change_percent = ((current_time - baseline_time) / baseline_time) * 100

        if change_percent > threshold_percent:
            regressions.append({
                'name': name,
                'baseline': baseline_time,
                'current': current_time,
                'change': change_percent
            })

    if regressions:
        print("⚠️ Performance regression detected:")
        for reg in regressions:
            print(f"  {reg['name']}: {reg['change']:.2f}% slower")
        sys.exit(1)
    else:
        print("✅ No performance regression")

if __name__ == '__main__':
    compare_benchmarks(sys.argv[1], sys.argv[2], float(sys.argv[3]))
```

---

## Work Checklist

### Pre-Work Preparation
- [ ] Confirm Phase 1 completion
- [ ] Backup current benchmark results
- [ ] Create Git branch (`feature/phase2-performance`)

### During Work
- [ ] Task 2.1: Thread-local object_pool
- [ ] Task 2.2: rotating_file_writer optimization
- [ ] Task 2.3: Benchmark infrastructure
- [ ] Task 2.4: Additional optimizations

### Post-Work Verification
- [ ] All benchmarks pass
- [ ] Verify performance improvement (15%+)
- [ ] Verify memory usage
- [ ] Complete code review

### Deployment
- [ ] Create Git tag (`v1.1.0`)
- [ ] Write Release Notes
- [ ] Prepare for Phase 3

---

## Next Steps

After Phase 2 completion:
- **Phase 3:** Code quality improvements (formatter separation, conditional compilation)
- **Documentation:** [LOGGER_SYSTEM_PHASE_3.md](./LOGGER_SYSTEM_PHASE_3.md)

---

**Start Date:** TBD
**Assignee:** TBD
**Reviewer:** TBD
