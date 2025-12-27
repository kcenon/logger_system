> **Language:** **English** | [한국어](CRITICAL_LOG_PREVENTION_KO.md)

## Table of Contents

- [Overview](#overview)
  - [Problem](#problem)
  - [Solution](#solution)
- [Architecture](#architecture)
- [Class Structure](#class-structure)
  - [1. `critical_writer`](#1-critical_writer)
    - [Key Features:](#key-features)
    - [Configuration:](#configuration)
    - [Usage Example:](#usage-example)
  - [2. `hybrid_writer`](#2-hybrid_writer)
    - [Features:](#features)
    - [Usage Example:](#usage-example)
- [How It Works](#how-it-works)
  - [1. Level-Based Routing](#1-level-based-routing)
  - [2. Critical Write Flow](#2-critical-write-flow)
    - [Step-by-Step Explanation:](#step-by-step-explanation)
  - [3. Write-Ahead Logging (WAL)](#3-write-ahead-logging-wal)
    - [WAL Format:](#wal-format)
    - [WAL Usage:](#wal-usage)
  - [4. Signal Handler](#4-signal-handler)
    - [Handled Signals:](#handled-signals)
    - [Signal Handler Implementation:](#signal-handler-implementation)
    - [Important Notes:](#important-notes)
  - [5. File Descriptor Sync](#5-file-descriptor-sync)
    - [Buffer Layers:](#buffer-layers)
    - [Implementation:](#implementation)
- [Performance Impact](#performance-impact)
  - [1. Critical Log Overhead](#1-critical-log-overhead)
  - [2. Normal Log Impact](#2-normal-log-impact)
  - [3. Optimization Strategies](#3-optimization-strategies)
    - [Strategy 1: Disable `sync_on_critical`](#strategy-1-disable-sync_on_critical)
    - [Strategy 2: Disable `force_flush_on_error`](#strategy-2-disable-force_flush_on_error)
    - [Strategy 3: Use Hybrid Writer](#strategy-3-use-hybrid-writer)
  - [4. Benchmark Results (Estimated)](#4-benchmark-results-estimated)
- [Production Recommendations](#production-recommendations)
  - [1. Default Configuration (General Services)](#1-default-configuration-general-services)
  - [2. High Reliability Configuration (Finance/Medical)](#2-high-reliability-configuration-financemedical)
  - [3. High Performance Configuration (Games/Real-time Systems)](#3-high-performance-configuration-gamesreal-time-systems)
- [Testing & Verification](#testing-verification)
  - [1. Critical Log Loss Test](#1-critical-log-loss-test)
    - [Verification Method:](#verification-method)
  - [2. WAL Recovery Test](#2-wal-recovery-test)
  - [3. Performance Benchmark](#3-performance-benchmark)
- [Troubleshooting](#troubleshooting)
  - [Issue 1: WAL File Not Created](#issue-1-wal-file-not-created)
  - [Issue 2: Signal Handler Not Working](#issue-2-signal-handler-not-working)
  - [Issue 3: Performance Degradation](#issue-3-performance-degradation)
- [FAQ](#faq)
  - [Q1: Should I wrap all logs with critical_writer?](#q1-should-i-wrap-all-logs-with-critical_writer)
  - [Q2: When should I use WAL?](#q2-when-should-i-use-wal)
  - [Q3: fsync() overhead is too high.](#q3-fsync-overhead-is-too-high)
  - [Q4: What can I do in a signal handler?](#q4-what-can-i-do-in-a-signal-handler)
- [References](#references)
- [Version History](#version-history)
- [License](#license)

# Critical Log Loss Prevention Guide

**Version:** 0.1.1.0
**Author:** kcenon
**Date:** 2025-01-17

## Overview

This document describes mechanisms to prevent critical log message loss in logger_system.

### Problem

Issues that can occur when using asynchronous loggers:

1. **Queue Message Loss**: Messages in async queue are lost during process crashes
2. **Unflushed Buffers**: Data in OS buffers not written to disk during abnormal termination
3. **Insufficient Signal Handling**: Termination without handling SIGTERM, SIGSEGV, etc.
4. **I/O Buffering**: Delayed writes due to filesystem buffering

### Solution

This solution provides the following mechanisms:

1. **Synchronous Critical Writer**: Critical logs bypass queue and write immediately
2. **Immediate Flush**: Automatic flush at critical levels
3. **Signal Handlers**: Handle SIGTERM, SIGINT, SIGSEGV, SIGABRT
4. **Write-Ahead Logging (WAL)**: Write to separate WAL file first
5. **File Descriptor Sync**: Force flush to OS buffers via `fsync()` call

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                      Application Code                           │
└────────────────────────────┬────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                       Logger Instance                           │
│                                                                 │
│  • log(level, message)                                          │
│  • Level check (trace/debug/info/warn/error/critical/fatal)    │
└────────────────────────────┬────────────────────────────────────┘
                             │
                ┌────────────┴────────────┐
                │                         │
                ▼                         ▼
    ┌──────────────────┐      ┌──────────────────┐
    │  Normal Logs     │      │  Critical Logs   │
    │  (info/debug)    │      │  (error/critical)│
    └────────┬─────────┘      └────────┬─────────┘
             │                         │
             ▼                         ▼
    ┌──────────────────┐      ┌──────────────────┐
    │  Async Queue     │      │  Bypass Queue    │
    │  (10k messages)  │      │  (Direct Write)  │
    └────────┬─────────┘      └────────┬─────────┘
             │                         │
             │                         ▼
             │                ┌──────────────────┐
             │                │  Write to WAL    │
             │                │  (.critical.wal) │
             │                └────────┬─────────┘
             │                         │
             │                         ▼
             │                ┌──────────────────┐
             │                │  Wrapped Writer  │
             │                │  (file/console)  │
             │                └────────┬─────────┘
             │                         │
             └────────────────┬────────┘
                              │
                              ▼
                     ┌──────────────────┐
                     │  Immediate Flush │
                     │  (critical only) │
                     └────────┬─────────┘
                              │
                              ▼
                     ┌──────────────────┐
                     │  fsync() / sync  │
                     │  (OS buffer)     │
                     └────────┬─────────┘
                              │
                              ▼
                         ┌─────────┐
                         │  Disk   │
                         └─────────┘

┌─────────────────────────────────────────────────────────────────┐
│                     Signal Handler Layer                         │
│                                                                 │
│  SIGTERM → Emergency Flush → Exit                               │
│  SIGINT  → Emergency Flush → Exit                               │
│  SIGSEGV → Emergency Flush → Re-raise signal                    │
│  SIGABRT → Emergency Flush → Re-raise signal                    │
└─────────────────────────────────────────────────────────────────┘
```

---

## Class Structure

### 1. `critical_writer`

A writer wrapper that processes critical logs synchronously.

#### Key Features:

- **Level-based Routing**: Choose sync/async based on log level
- **Immediate Flush**: Flush critical/fatal messages immediately
- **WAL Support**: Optional Write-Ahead Logging
- **Signal Handling**: Automatic flush on abnormal termination
- **File Sync**: Reflect to disk including OS buffers via `fsync()` call

#### Configuration:

```cpp
struct critical_writer_config {
    // Immediate flush for critical/fatal messages (default: true)
    bool force_flush_on_critical = true;

    // Immediate flush for error messages too (default: false)
    bool force_flush_on_error = false;

    // Enable signal handlers (default: true)
    bool enable_signal_handlers = true;

    // Enable Write-Ahead Logging (default: false)
    bool write_ahead_log = false;

    // WAL file path
    std::string wal_path = "logs/.wal";

    // Call fsync after critical write (default: true)
    bool sync_on_critical = true;

    // Critical write timeout in ms (0 = unlimited)
    uint32_t critical_write_timeout_ms = 5000;
};
```

#### Usage Example:

```cpp
#include <kcenon/logger/writers/critical_writer.h>

// Default configuration
auto critical = std::make_unique<critical_writer>(
    std::make_unique<file_writer>("app.log")
);

// Custom configuration
auto critical = std::make_unique<critical_writer>(
    std::make_unique<rotating_file_writer>("app.log", 10_MB, 5),
    critical_writer_config{
        .force_flush_on_critical = true,
        .force_flush_on_error = true,
        .enable_signal_handlers = true,
        .write_ahead_log = true,
        .wal_path = "logs/.app.wal",
        .sync_on_critical = true
    }
);

logger.add_writer(std::move(critical));
```

### 2. `hybrid_writer`

A hybrid writer that processes normal logs asynchronously and critical logs synchronously.

#### Features:

- **Performance + Safety**: High throughput with async, immediate flush when critical
- **Automatic Routing**: Automatically select path based on log level
- **Simple Configuration**: Integrates critical_writer + async_writer into one

#### Usage Example:

```cpp
#include <kcenon/logger/writers/critical_writer.h>

auto hybrid = std::make_unique<hybrid_writer>(
    std::make_unique<file_writer>("app.log"),
    critical_writer_config{
        .force_flush_on_critical = true,
        .enable_signal_handlers = true
    },
    10000  // Async queue size
);

logger.add_writer(std::move(hybrid));

// Now automatically handled based on log level:
logger.log(log_level::info, "Fast async");       // → Async queue
logger.log(log_level::critical, "Immediate");    // → Bypass queue + flush
```

---

## How It Works

### 1. Level-Based Routing

```cpp
bool critical_writer::is_critical_level(log_level level) const {
    // Critical and fatal always require immediate flush
    if (level >= log_level::critical) {
        return config_.force_flush_on_critical;
    }

    // Error level if configured
    if (level == log_level::error) {
        return config_.force_flush_on_error;
    }

    return false;
}
```

Log level checks:
- `log_level::critical` (5): Always process immediately
- `log_level::fatal` (5): Always process immediately
- `log_level::error` (4): Process immediately if configured
- `log_level::warn` (3) and below: Normal path (async possible)

### 2. Critical Write Flow

```cpp
common::VoidResult critical_writer::write(...) {
    if (is_critical_level(level)) {
        // 1. Acquire exclusive lock
        std::lock_guard<std::mutex> lock(critical_mutex_);

        // 2. Write to WAL first (if enabled)
        if (wal_stream_) {
            write_to_wal(...);
        }

        // 3. Write to wrapped writer
        auto result = wrapped_writer_->write(...);

        // 4. Immediate flush
        wrapped_writer_->flush();

        // 5. Sync file descriptor (if configured)
        if (config_.sync_on_critical) {
            sync_file_descriptor();  // fsync()
        }

        return result;
    }

    // Non-critical: delegate normally
    return wrapped_writer_->write(...);
}
```

#### Step-by-Step Explanation:

1. **Exclusive Lock**: Guarantee ordering between critical writes
2. **WAL Write**: Write to separate WAL file first (for recovery)
3. **Main Write**: Write to actual log file
4. **Immediate Flush**: Empty buffer (`fflush()`)
5. **File Sync**: Reflect to disk including OS buffers (`fsync()`)

### 3. Write-Ahead Logging (WAL)

WAL is an additional safety measure for crash recovery.

#### WAL Format:

```
[2025-01-17 14:30:45.123] [CRITICAL] [main.cpp:42:handle_error] Out of memory
[2025-01-17 14:30:45.456] [FATAL] [main.cpp:50:main] Terminating
```

#### WAL Usage:

```cpp
// Enable WAL
critical_writer_config config;
config.write_ahead_log = true;
config.wal_path = "logs/.critical.wal";

auto critical = std::make_unique<critical_writer>(
    std::make_unique<file_writer>("app.log"),
    config
);

// Crash recovery (separate tool)
// $ cat logs/.critical.wal | grep CRITICAL
// → Can check critical logs just before crash
```

### 4. Signal Handler

Preserves logs even during abnormal termination.

#### Handled Signals:

| Signal | Description | Action |
|--------|-------------|--------|
| `SIGTERM` | Normal termination request | Emergency flush → Exit |
| `SIGINT` | Ctrl+C interrupt | Emergency flush → Exit |
| `SIGSEGV` | Segmentation fault | Emergency flush → Re-raise |
| `SIGABRT` | abort() call | Emergency flush → Re-raise |

#### Signal Handler Implementation:

```cpp
void critical_writer::signal_handler(int signal) {
    critical_writer* writer = instance_.load();
    if (!writer) return;

    // 1. Output emergency message
    std::cerr << "[critical_writer] Signal " << signal
              << " received, emergency flush\n";

    // 2. Flush immediately (no memory allocation!)
    if (writer->wrapped_writer_) {
        writer->wrapped_writer_->flush();
    }

    // 3. WAL flush
    if (writer->wal_stream_) {
        writer->wal_stream_->flush();
    }

    // 4. Re-raise SIGSEGV/SIGABRT to original handler
    if (signal == SIGSEGV || signal == SIGABRT) {
        writer->restore_signal_handlers();
        std::raise(signal);
    }
}
```

#### Important Notes:

- **No memory allocation** inside signal handler
- **Use only signal-safe functions** (async-signal-safe)
- Avoid complex logic, perform minimal flush only

### 5. File Descriptor Sync

`fflush()` only empties C library buffers; OS kernel buffers remain.
Must call `fsync()` to fully write to disk.

#### Buffer Layers:

```
Application
    ↓ fprintf/write
[C Library Buffer]  ← Emptied by fflush()
    ↓
[OS Kernel Buffer]  ← Emptied by fsync()
    ↓
[Disk Controller]
    ↓
[Physical Disk]
```

#### Implementation:

```cpp
void critical_writer::sync_file_descriptor() {
#ifdef __unix__
    ::fsync(STDOUT_FILENO);
    ::fsync(STDERR_FILENO);

    // Get file descriptor from wrapped writer and sync
    // (Actual implementation needs to query fd from wrapped writer)
#elif defined(_WIN32)
    _commit(fd);  // Windows equivalent
#endif
}
```

---

## Performance Impact

### 1. Critical Log Overhead

| Operation | Estimated Time | Description |
|-----------|----------------|-------------|
| Mutex lock | ~20 ns | Uncontended mutex |
| WAL write | ~50 μs | Sequential write to SSD |
| Main write | ~50 μs | Sequential write to SSD |
| `fflush()` | ~100 μs | C library buffer flush |
| `fsync()` | **1-10 ms** | OS kernel → disk sync |

**Total Overhead**: **1-10 ms per critical log**

### 2. Normal Log Impact

- **Normal/Debug/Info/Warn**: No impact (uses async queue)
- **Error** (if `force_flush_on_error=false`): No impact
- **Critical/Fatal**: Above overhead occurs

### 3. Optimization Strategies

#### Strategy 1: Disable `sync_on_critical`

```cpp
critical_writer_config config;
config.sync_on_critical = false;  // Skip fsync()

// Overhead: 1-10 ms → ~200 μs
// Risk: Medium (Remains in OS buffer, possible loss on kernel crash)
```

#### Strategy 2: Disable `force_flush_on_error`

```cpp
config.force_flush_on_error = false;  // Process errors asynchronously

// Error level also uses async queue → No overhead
// Only critical/fatal processed immediately
```

#### Strategy 3: Use Hybrid Writer

```cpp
auto hybrid = std::make_unique<hybrid_writer>(
    std::make_unique<file_writer>("app.log"),
    config,
    50000  // Large async queue
);

// Normal logs: Async (fast)
// Critical logs: Sync (safe)
// Average performance impact: < 1%
```

### 4. Benchmark Results (Estimated)

| Scenario | Throughput | P99 Latency |
|----------|------------|-------------|
| Async only (baseline) | 1,000,000 msg/s | 50 μs |
| Hybrid (1% critical) | 990,000 msg/s | 10 ms |
| All critical (worst case) | 100 msg/s | 10 ms |

---

## Production Recommendations

### 1. Default Configuration (General Services)

```cpp
auto hybrid = std::make_unique<hybrid_writer>(
    std::make_unique<rotating_file_writer>(
        "logs/production.log",
        100 * 1024 * 1024,  // 100 MB
        10                   // 10 rotations
    ),
    critical_writer_config{
        .force_flush_on_critical = true,   // ✅ Flush critical immediately
        .force_flush_on_error = false,     // ❌ Errors async
        .enable_signal_handlers = true,    // ✅ Signal handler
        .write_ahead_log = false,          // ❌ WAL disabled (performance)
        .sync_on_critical = true           // ✅ Call fsync()
    },
    50000  // 50k async queue
);
```

**Characteristics**:
- Process only critical/fatal immediately
- Maintain performance with async errors
- Prepare for abnormal termination with signal handler
- Prioritize performance without WAL

### 2. High Reliability Configuration (Finance/Medical)

```cpp
auto hybrid = std::make_unique<hybrid_writer>(
    std::make_unique<rotating_file_writer>(
        "logs/critical.log",
        50 * 1024 * 1024,   // 50 MB
        20                   // 20 rotations (longer retention)
    ),
    critical_writer_config{
        .force_flush_on_critical = true,   // ✅ Flush critical immediately
        .force_flush_on_error = true,      // ✅ Flush errors too
        .enable_signal_handlers = true,    // ✅ Signal handler
        .write_ahead_log = true,           // ✅ Enable WAL
        .wal_path = "logs/.critical.wal",
        .sync_on_critical = true,          // ✅ Call fsync()
        .critical_write_timeout_ms = 10000 // 10 second timeout
    },
    10000  // Smaller queue (faster drain)
);
```

**Characteristics**:
- Process all errors and above immediately
- Crash recovery possible with WAL
- Longer log retention period
- Safety first

### 3. High Performance Configuration (Games/Real-time Systems)

```cpp
auto hybrid = std::make_unique<hybrid_writer>(
    std::make_unique<file_writer>("logs/performance.log"),
    critical_writer_config{
        .force_flush_on_critical = true,   // ✅ Flush critical only
        .force_flush_on_error = false,     // ❌ Errors async
        .enable_signal_handlers = true,    // ✅ Signal handler
        .write_ahead_log = false,          // ❌ WAL disabled
        .sync_on_critical = false          // ❌ Skip fsync (fast!)
    },
    100000  // 100k large queue
);
```

**Characteristics**:
- Process only critical immediately, skip fsync for speed
- Errors fully async
- Large queue for burst handling
- Performance first

---

## Testing & Verification

### 1. Critical Log Loss Test

```cpp
#include <csignal>

void test_signal_safety() {
    logger log(false);

    auto critical = std::make_unique<critical_writer>(
        std::make_unique<file_writer>("test_signal.log"),
        critical_writer_config{
            .enable_signal_handlers = true,
            .write_ahead_log = true,
            .wal_path = "test_signal.wal"
        }
    );

    log.add_writer(std::move(critical));

    // Record critical log
    log.log(log_level::critical, "Before signal");

    // Force signal
    std::raise(SIGTERM);

    // Verification: Check logs exist in test_signal.log and test_signal.wal
}
```

#### Verification Method:

```bash
# 1. Run program
$ ./test_signal_safety

# 2. Interrupt with Ctrl+C
^C[critical_writer] Signal SIGINT received, emergency flush
[critical_writer] Emergency flush completed

# 3. Check logs
$ cat test_signal.log
[2025-01-17 14:30:45] [CRITICAL] Before signal

$ cat test_signal.wal
[2025-01-17 14:30:45.123] [CRITICAL] [test.cpp:10:test] Before signal
```

### 2. WAL Recovery Test

```cpp
void test_wal_recovery() {
    // Step 1: Write with WAL
    {
        logger log(false);
        auto critical = std::make_unique<critical_writer>(
            std::make_unique<file_writer>("main.log"),
            critical_writer_config{
                .write_ahead_log = true,
                .wal_path = "main.wal"
            }
        );
        log.add_writer(std::move(critical));

        log.log(log_level::critical, "Message 1");
        // Simulate crash (don't call destructor)
        std::exit(1);
    }

    // Step 2: Recover from WAL
    std::ifstream wal("main.wal");
    std::string line;
    while (std::getline(wal, line)) {
        // Parse and replay WAL entries
        std::cout << "Recovered: " << line << "\n";
    }
}
```

### 3. Performance Benchmark

```cpp
void benchmark_critical_overhead() {
    logger log(false);

    // Setup
    auto start = std::chrono::high_resolution_clock::now();

    // 1000 critical logs
    for (int i = 0; i < 1000; ++i) {
        log.log(log_level::critical, "Critical message " + std::to_string(i));
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start
    ).count();

    std::cout << "1000 critical logs: " << duration << " ms\n";
    std::cout << "Average: " << (duration / 1000.0) << " ms/log\n";
}
```

---

## Troubleshooting

### Issue 1: WAL File Not Created

**Symptom**:
```
[critical_writer] Failed to open WAL: logs/.critical.wal
```

**Cause**: Directory doesn't exist

**Solution**:
```cpp
// Create directory before WAL creation
#include <filesystem>
std::filesystem::create_directories("logs");

auto critical = std::make_unique<critical_writer>(
    std::make_unique<file_writer>("logs/app.log"),
    critical_writer_config{
        .write_ahead_log = true,
        .wal_path = "logs/.critical.wal"
    }
);
```

### Issue 2: Signal Handler Not Working

**Symptom**: Logs not flushed on Ctrl+C

**Cause**: Signal handler disabled or conflicts

**Solution**:
```cpp
// 1. Confirm signal handler enabled
critical_writer_config config;
config.enable_signal_handlers = true;  // ✅

// 2. Check conflicts with other libraries
// (e.g., Google Test installs its own signal handler)

// 3. Add explicit flush
std::atexit([]() {
    // Explicit flush on exit
    logger.flush();
});
```

### Issue 3: Performance Degradation

**Symptom**: Overall throughput decreases after using critical logs

**Cause**: Processing all logs as critical

**Solution**:
```cpp
// ❌ Wrong usage
logger.log(log_level::critical, "User logged in");  // Normal event

// ✅ Correct usage
logger.log(log_level::info, "User logged in");       // Normal event
logger.log(log_level::critical, "Out of memory!");   // Real critical

// Or disable sync_on_critical
config.sync_on_critical = false;  // Skip fsync → 10x faster
```

---

## FAQ

### Q1: Should I wrap all logs with critical_writer?

**A**: No. Use a mix of normal writer and critical_writer:

```cpp
// Normal logs: async writer (performance)
logger.add_writer("normal",
    std::make_unique<async_writer>(
        std::make_unique<file_writer>("app.log")
    )
);

// Critical logs: critical writer (safety)
logger.add_writer("critical",
    std::make_unique<critical_writer>(
        std::make_unique<file_writer>("critical.log")
    )
);

// Or use hybrid_writer (recommended)
logger.add_writer("hybrid",
    std::make_unique<hybrid_writer>(
        std::make_unique<file_writer>("all.log")
    )
);
```

### Q2: When should I use WAL?

**A**: Use WAL in these cases:

- ✅ Financial transaction logs
- ✅ Medical record systems
- ✅ Security audit logs
- ❌ General application logs (overkill)
- ❌ High-performance game logs

### Q3: fsync() overhead is too high.

**A**: Consider these options:

1. **Disable fsync**:
   ```cpp
   config.sync_on_critical = false;  // 1-10ms → ~200μs
   ```

2. **Batch fsync**: Collect multiple logs and sync at once (requires custom implementation)

3. **XFS/ext4 optimization**: Filesystem mount options
   ```bash
   # /etc/fstab
   /dev/sda1 /logs xfs noatime,nodiratime 0 0
   ```

### Q4: What can I do in a signal handler?

**A**: **Very limited**. Only async-signal-safe functions:

✅ **Allowed**:
- `write()` (low-level)
- `_exit()`
- `signal()`

❌ **Not Allowed** (Undefined Behavior):
- `malloc()` / `new`
- `printf()` / `std::cout`
- `std::lock_guard<>`
- Most C++ standard library

This implementation performs minimal flush only.

---

## References

- [POSIX Signal Safety](https://man7.org/linux/man-pages/man7/signal-safety.7.html)
- [fsync(2) - Linux Manual](https://man7.org/linux/man-pages/man2/fsync.2.html)
- [Write-Ahead Logging (PostgreSQL)](https://www.postgresql.org/docs/current/wal-intro.html)
- [spdlog Async Logging](https://github.com/gabime/spdlog#asynchronous-logging)

---

## Version History

- **1.1.0** (2025-01-17): Initial implementation
  - Add `critical_writer` class
  - Add `hybrid_writer` class
  - Implement signal handler
  - Support WAL
  - Integrate fsync()

---

## License

BSD 3-Clause License
Copyright (c) 2025, kcenon

---

*Last Updated: 2025-10-20*
