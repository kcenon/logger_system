---
doc_id: "LOG-GUID-009"
doc_title: "Critical Logging Quick Start"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "GUID"
---

> **Language:** **English** | [한국어](CRITICAL_LOGGING_QUICK_START.kr.md)

## Table of Contents

- [🎯 Goal](#-goal)
- [🚀 Quick Start](#-quick-start)
  - [Step 1: Include Headers](#step-1-include-headers)
  - [Step 2: Create Hybrid Writer](#step-2-create-hybrid-writer)
  - [Step 3: Verify](#step-3-verify)
- [📊 3 Usage Scenarios](#-3-usage-scenarios)
  - [Scenario 1: General Web Services (Recommended)](#scenario-1-general-web-services-recommended)
  - [Scenario 2: Finance/Medical Systems (Maximum Reliability)](#scenario-2-financemedical-systems-maximum-reliability)
  - [Scenario 3: Games/Real-time Systems (High Performance)](#scenario-3-gamesreal-time-systems-high-performance)
- [🔧 Configuration Options](#-configuration-options)
- [✅ Verification Methods](#-verification-methods)
  - [Test 1: Check Critical Logs](#test-1-check-critical-logs)
  - [Test 2: Crash Simulation](#test-2-crash-simulation)
  - [Test 3: Ctrl+C Test](#test-3-ctrlc-test)
- [📚 Learn More](#-learn-more)
- [🐛 Troubleshooting](#-troubleshooting)
  - ["WAL file not created"](#wal-file-not-created)
  - ["Performance too slow"](#performance-too-slow)
  - ["Signal handler not working"](#signal-handler-not-working)
- [💡 Key Points](#-key-points)

# Critical Logging Quick Start

> **SSOT**: This document is the single source of truth for **Critical Logging Quick Start**.

**⚠️ Apply Critical Log Loss Prevention in 5 Minutes**

---

## 🎯 Goal

**Guarantee that critical logs are never lost** in production environments.

---

## 🚀 Quick Start

### Step 1: Include Headers

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/critical_writer.h>
```

### Step 2: Create Hybrid Writer

```cpp
using namespace kcenon::logger;

// Existing code (Risky!)
auto logger = logger_builder()
    .with_async(true)
    .add_writer("main", std::make_unique<file_writer>("app.log"))
    .build();

// Improved code (Safe!)
auto logger = logger_builder()
    .with_async(true)
    .add_writer("main",
        std::make_unique<hybrid_writer>(
            std::make_unique<file_writer>("app.log")
        )
    )
    .build();
```

**Changes**: Just wrap `file_writer` with `hybrid_writer`!

### Step 3: Verify

```cpp
// Normal logs → async (fast)
logger->log(log_level::info, "Normal message");

// Critical logs → immediate flush (safe!)
logger->log(log_level::critical, "Critical error");
```

---

## 📊 3 Usage Scenarios

### Scenario 1: General Web Services (Recommended)

```cpp
auto hybrid = std::make_unique<hybrid_writer>(
    std::make_unique<rotating_file_writer>("app.log", 100_MB, 10)
);
logger.add_writer(std::move(hybrid));
```

- ✅ Only critical/fatal immediately flushed
- ✅ Errors async (maintain performance)
- ⚡ Performance impact: < 1%

### Scenario 2: Finance/Medical Systems (Maximum Reliability)

```cpp
auto hybrid = std::make_unique<hybrid_writer>(
    std::make_unique<rotating_file_writer>("audit.log", 50_MB, 20),
    critical_writer_config{
        .force_flush_on_critical = true,
        .force_flush_on_error = true,      // Flush errors too!
        .enable_signal_handlers = true,
        .write_ahead_log = true,           // Enable WAL
        .wal_path = "logs/.audit.wal"
    }
);
logger.add_writer(std::move(hybrid));
```

- ✅ All errors and above flushed immediately
- ✅ Crash recovery possible with WAL
- ✅ Signal handler protection
- ⚡ Performance impact: 5-10% (acceptable)

### Scenario 3: Games/Real-time Systems (High Performance)

```cpp
auto hybrid = std::make_unique<hybrid_writer>(
    std::make_unique<file_writer>("game.log"),
    critical_writer_config{
        .force_flush_on_critical = true,
        .force_flush_on_error = false,     // Errors async
        .sync_on_critical = false          // Skip fsync
    },
    100000  // Large async queue
);
logger.add_writer(std::move(hybrid));
```

- ✅ Only critical flushed (without fsync)
- ✅ Everything else fully async
- ⚡ Performance impact: < 0.1%

---

## 🔧 Configuration Options

```cpp
critical_writer_config{
    .force_flush_on_critical = true,   // Flush critical immediately
    .force_flush_on_error = false,     // Errors async
    .enable_signal_handlers = true,    // Handle SIGTERM/SIGINT
    .write_ahead_log = false,          // WAL disabled
    .sync_on_critical = true,          // Call fsync()
    .critical_write_timeout_ms = 5000  // 5 second timeout
}
```

| Option | Default | Recommended (General) | Recommended (High Reliability) |
|--------|---------|----------------------|-------------------------------|
| `force_flush_on_critical` | true | true | true |
| `force_flush_on_error` | false | false | **true** |
| `write_ahead_log` | false | false | **true** |
| `sync_on_critical` | true | true | true |

---

## ✅ Verification Methods

### Test 1: Check Critical Logs

```cpp
logger.log(log_level::critical, "Test critical log");

// Check file immediately
system("tail -1 app.log");  // Should appear right away!
```

### Test 2: Crash Simulation

```cpp
logger.log(log_level::critical, "Before crash");
std::abort();  // Force terminate

// Check after restart
system("cat app.log | grep 'Before crash'");  // Should exist!
```

### Test 3: Ctrl+C Test

```bash
$ ./your_app
[INFO] App started
^C  # Press Ctrl+C
[critical_writer] Signal SIGINT received, emergency flush
[critical_writer] Emergency flush completed

$ cat app.log
# All logs properly recorded!
```

---

## 📚 Learn More

- **Detailed Guide**: [CRITICAL_LOG_PREVENTION.md](CRITICAL_LOG_PREVENTION.md)
- **Example Code**: [critical_logging_example.cpp](../examples/critical_logging_example.cpp)
- **API Documentation**: [critical_writer.h](../include/kcenon/logger/writers/critical_writer.h)

---

## 🐛 Troubleshooting

### "WAL file not created"

```cpp
// Create directory first
#include <filesystem>
std::filesystem::create_directories("logs");
```

### "Performance too slow"

```cpp
// Disable fsync
config.sync_on_critical = false;  // 10x faster
```

### "Signal handler not working"

```cpp
// Check for conflicts with other libraries
// Google Test etc. install their own handlers
```

---

## 💡 Key Points

1. **Use hybrid_writer** → Performance + Safety
2. **Only critical/fatal immediately flushed** → Minimal overhead
3. **WAL is optional** → Enable only when needed
4. **fsync is expensive** → Disable for high performance needs

**Protect critical logs with just one line change!**

```cpp
// Before
.add_writer("main", std::make_unique<file_writer>("app.log"))

// After
.add_writer("main", std::make_unique<hybrid_writer>(
    std::make_unique<file_writer>("app.log")
))
```

---

**Have questions or issues? Open an issue!**

---

*Last Updated: 2025-10-20*
