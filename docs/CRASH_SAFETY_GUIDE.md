---
doc_id: "LOG-GUID-002b"
doc_title: "Crash-Safe Logger Guide"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "GUID"
---

# Crash-Safe Logger Guide

> **Version**: 1.0.0
> **Last Updated**: 2025-02-09
> **Status**: Production Ready
> **Split from**: [LOG_SERVER_AND_CRASH_SAFETY.md](LOG_SERVER_AND_CRASH_SAFETY.md)

## Table of Contents

- [Overview](#overview)
- [Crash Safety Mechanism](#crash-safety-mechanism)
- [Configuration](#configuration)
- [API Reference](#api-reference)
- [Performance Overhead](#performance-overhead)
- [When to Use](#when-to-use)
- [Combined Usage](#combined-usage)
  - [Local Crash Safety + Network Forwarding](#local-crash-safety--network-forwarding)
  - [Server-Side Crash Safety](#server-side-crash-safety)
  - [Complete Production Topology](#complete-production-topology)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)
- [Related Documentation](#related-documentation)

---

## Overview

The crash-safe logger guarantees log persistence during application crashes, providing zero data loss during application failures.

| Feature | Purpose | Key Benefit |
|---------|---------|-------------|
| **Crash-Safe Logger** | Guaranteed log persistence during crashes | Zero data loss during application failures |

**Key Benefits**:
- **Crash Resilience**: Logs survive application crashes via signal handlers
- **Zero Data Loss**: Emergency flush ensures critical logs reach persistent storage
- **Production Ready**: Thread-safe, performant, and battle-tested

---

## Crash Safety Mechanism

The crash-safe logger guarantees log persistence during application crashes through **signal-based emergency flushing**.

### How It Works

```
┌─────────────────────────────────────────────────────────┐
│              Normal Operation Flow                       │
│                                                          │
│  Application Code                                        │
│       │                                                  │
│       ▼                                                  │
│  logger->log(...)                                        │
│       │                                                  │
│       ▼                                                  │
│  crash_safe_logger                                       │
│       │                                                  │
│       ▼                                                  │
│  underlying_logger (buffered)                            │
│       │                                                  │
│       ▼                                                  │
│  Periodic Flush (auto_flush_interval)                    │
│       │                                                  │
│       ▼                                                  │
│  Disk Storage                                            │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│              Crash Scenario Flow                         │
│                                                          │
│  Application Code                                        │
│       │                                                  │
│       ▼                                                  │
│  CRASH (SIGSEGV/SIGABRT)                                │
│       │                                                  │
│       ▼                                                  │
│  Signal Handler (async-signal-safe)                      │
│       │                                                  │
│       ▼                                                  │
│  emergency_flush()                                       │
│       │                                                  │
│       ├─ Set emergency flag (atomic)                     │
│       │                                                  │
│       ├─ Try flush_mutex_.try_lock()                     │
│       │                                                  │
│       └─ Flush buffered logs (best-effort)               │
│       │                                                  │
│       ▼                                                  │
│  Disk Storage (logs saved!)                              │
│       │                                                  │
│       ▼                                                  │
│  Chain to original signal handler or exit               │
└─────────────────────────────────────────────────────────┘
```

**Header File**: `include/kcenon/logger/safety/crash_safe_logger.h`

### Signal Handlers

The crash-safe logger installs handlers for the following signals:

| Signal | Description | Action |
|--------|-------------|--------|
| **SIGSEGV** | Segmentation fault (invalid memory access) | Emergency flush + chain to old handler |
| **SIGABRT** | Abort signal (assert failures, `std::terminate()`) | Emergency flush + chain to old handler |
| **SIGTERM** | Termination request (graceful shutdown) | Emergency flush + exit |
| **SIGINT** | Interrupt signal (Ctrl+C) | Emergency flush + exit |

### Async-Signal-Safe Constraints

Signal handlers must follow strict async-signal-safe rules:

**Allowed Operations**:
- Atomic operations (`std::atomic`)
- `try_lock()` (non-blocking mutex attempt)
- Direct system calls (`write()`, `fsync()`)

**Forbidden Operations**:
- Memory allocation (`new`, `malloc()`)
- `lock()` (blocking mutex)
- Exception throwing
- Non-reentrant library calls

**Rationale**: The application may crash while holding locks or during memory allocation, so the signal handler cannot rely on these mechanisms.

### Recovery Procedure

After an application restart:

1. **Detection**: Check for emergency flush flag or incomplete log files
2. **Recovery**: Read any crash logs from emergency flush location
3. **Analysis**: Parse crash logs to determine failure cause
4. **Cleanup**: Remove emergency markers

**Note**: The current implementation focuses on emergency flushing. Full recovery API would be added in future versions.

---

## Configuration

### Basic Setup

```cpp
#include <kcenon/logger/safety/crash_safe_logger.h>

// Create crash-safe logger
auto safe_logger = crash_safe_logger::create();

// Install signal handlers
if (!safe_logger->install_crash_handlers()) {
    std::cerr << "Failed to install crash handlers\n";
    return 1;
}

// Start logging
safe_logger->start();

// Use as normal logger
safe_logger->log(log_level::info, "Application started with crash protection");
```

### Advanced Configuration

```cpp
// Create with custom underlying logger
auto base_logger = std::make_shared<logger>(
    true,   // Use async mode
    32768   // 32KB buffer
);

auto safe_logger = crash_safe_logger::create(base_logger);

// Install crash handlers
safe_logger->install_crash_handlers();

// Configure auto-flush for durability
safe_logger->set_auto_flush_interval(std::chrono::seconds(5));

// Set log level
safe_logger->set_min_level(log_level::debug);

// Start logging
safe_logger->start();
```

---

## API Reference

### crash_safe_logger Class

```cpp
namespace kcenon::logger::safety {

class crash_safe_logger {
public:
    // Factory method
    static std::shared_ptr<crash_safe_logger> create(
        std::shared_ptr<logger> underlying_logger = nullptr
    );

    // Signal handler management
    bool install_crash_handlers();
    void uninstall_crash_handlers();

    // Flush operations
    bool flush_with_timeout(std::chrono::milliseconds timeout);
    void emergency_flush();  // Async-signal-safe

    // Log level management (thread-safe)
    void set_min_level(log_level level);
    log_level get_min_level() const;

    // Auto-flush configuration
    void set_auto_flush_interval(std::chrono::milliseconds interval);

    // Logging methods
    void log(log_level level, const std::string& message);
    void log(log_level level,
             const std::string& message,
             const std::string& file,
             int line,
             const std::string& function);

    // Lifecycle
    common::VoidResult start();
    common::VoidResult stop();

    // Access underlying logger
    std::shared_ptr<logger> get_underlying_logger();

    ~crash_safe_logger();
};

} // namespace kcenon::logger::safety
```

### Method Details

#### create()

**Description**: Factory method to create a crash-safe logger instance.

**Parameters**:
- `underlying_logger` -- Base logger to wrap (creates default if `nullptr`)

**Returns**: `std::shared_ptr<crash_safe_logger>`

**Default Underlying Logger**:
- Async mode: `true`
- Buffer size: `16384` bytes (16KB)

**Example**:
```cpp
// Create with default underlying logger
auto safe_logger = crash_safe_logger::create();

// Create with custom underlying logger
auto custom_logger = std::make_shared<logger>(true, 65536);
auto safe_logger = crash_safe_logger::create(custom_logger);
```

#### install_crash_handlers()

**Description**: Installs signal handlers for crash detection.

**Returns**: `bool` -- `true` if all handlers installed successfully, `false` otherwise.

**Signals Handled**: SIGSEGV, SIGABRT, SIGTERM, SIGINT

**Idempotent**: Safe to call multiple times (subsequent calls return `true` without reinstalling).

**Example**:
```cpp
auto safe_logger = crash_safe_logger::create();
if (!safe_logger->install_crash_handlers()) {
    std::cerr << "Failed to install signal handlers\n";
    return 1;
}
```

#### uninstall_crash_handlers()

**Description**: Removes signal handlers and restores original handlers.

**Note**: Automatically called in destructor.

**Example**:
```cpp
safe_logger->uninstall_crash_handlers();
// Original signal handlers restored
```

#### flush_with_timeout()

**Description**: Flushes logs with a timeout to prevent deadlocks.

**Parameters**:
- `timeout` -- Maximum time to wait for flush completion

**Returns**: `bool` -- `true` if flushed successfully, `false` on timeout.

**Use Case**: Graceful shutdown where you want to ensure logs are flushed but don't want to wait indefinitely.

**Example**:
```cpp
// Try to flush within 2 seconds
if (!safe_logger->flush_with_timeout(std::chrono::seconds(2))) {
    std::cerr << "Warning: Flush timed out\n";
}
```

#### emergency_flush()

**Description**: Performs best-effort flush in signal handler context.

**Async-Signal-Safe**: Yes (no allocations, non-blocking locks only).

**Returns**: `void` (always succeeds, but flush may be incomplete if locks held).

**Note**: Typically called automatically by signal handler, not by user code.

**Example**:
```cpp
// Rarely called directly (signal handler calls this)
safe_logger->emergency_flush();
```

#### set_auto_flush_interval()

**Description**: Configures automatic background flushing for data durability.

**Parameters**:
- `interval` -- Time between auto-flushes (0 disables auto-flush)

**Spawns Thread**: Yes (background thread for periodic flushing).

**Example**:
```cpp
// Flush every 5 seconds
safe_logger->set_auto_flush_interval(std::chrono::seconds(5));

// Disable auto-flush
safe_logger->set_auto_flush_interval(std::chrono::milliseconds(0));
```

**Trade-off**: More frequent flushes increase durability but may reduce performance.

#### set_min_level() / get_min_level()

**Description**: Thread-safe log level management using atomic operations.

**Parameters**:
- `level` -- Minimum log level to record

**Thread-Safe**: Yes (uses `std::atomic<log_level>`).

**Example**:
```cpp
// Set minimum level to warning
safe_logger->set_min_level(log_level::warning);

// Get current level
auto current_level = safe_logger->get_min_level();
```

---

## Performance Overhead

### Benchmarks

| Configuration | Throughput | Latency | Overhead vs Standard Logger |
|---------------|------------|---------|----------------------------|
| **Standard logger** | 4.34M msg/sec | 148ns | Baseline |
| **Crash-safe (no auto-flush)** | 4.28M msg/sec | 151ns | **+2%** (signal handler check) |
| **Crash-safe (5s auto-flush)** | 4.15M msg/sec | 155ns | **+4.4%** (periodic flush) |
| **Crash-safe (1s auto-flush)** | 3.89M msg/sec | 166ns | **+10.4%** (frequent flush) |

**Test Environment**:
- CPU: Intel Xeon E5-2686 v4 (2.3 GHz)
- Compiler: GCC 11.4.0
- Optimization: `-O3 -march=native`
- Buffer size: 16KB

### Overhead Sources

1. **Signal Handler Check** (~2%): Atomic operations for emergency flush flag
2. **Auto-Flush Thread** (~2-8%): Depends on flush frequency and buffer size
3. **Flush Timeout Logic** (~1%): Try-lock and deadline checking

### Optimization Tips

**Minimize Overhead**:
```cpp
// Use larger buffers to reduce flush frequency
auto base_logger = std::make_shared<logger>(true, 65536);  // 64KB
auto safe_logger = crash_safe_logger::create(base_logger);

// Set longer auto-flush interval
safe_logger->set_auto_flush_interval(std::chrono::seconds(10));
```

**Maximize Durability**:
```cpp
// Use smaller buffers and frequent flushes
auto base_logger = std::make_shared<logger>(true, 8192);  // 8KB
auto safe_logger = crash_safe_logger::create(base_logger);

// Flush every second
safe_logger->set_auto_flush_interval(std::chrono::seconds(1));
```

---

## When to Use

### Use Crash-Safe Logger When:

**Critical Financial Applications**
- Trading systems
- Payment gateways
- Accounting software

**Reason**: Zero tolerance for data loss; logs required for auditing and compliance.

**Medical/Healthcare Systems**
- Patient monitoring
- Medical device control
- Electronic health records

**Reason**: Regulatory requirements (FDA, HIPAA) mandate log completeness.

**Debugging Intermittent Crashes**
- Hard-to-reproduce bugs
- Production crash analysis
- Post-mortem debugging

**Reason**: Logs may reveal crash cause that would otherwise be lost.

**Compliance Requirements**
- SOC 2 auditing
- PCI-DSS logging
- GDPR data processing records

**Reason**: Audit logs must survive system failures.

### Do NOT Use When:

**Low-Latency Requirements**
- High-frequency trading (sub-microsecond latency)
- Real-time control systems

**Reason**: 2-10% overhead may be unacceptable; use specialized logging instead.

**Development/Testing Environments**
- Local development
- Unit tests

**Reason**: No crash safety needed; standard logger is simpler and faster.

**Immutable Infrastructure**
- Containers that restart on crash
- Serverless functions

**Reason**: Logs are externalized before crash via centralized logging; crash safety redundant.

---

## Combined Usage

### Local Crash Safety + Network Forwarding

Combine crash-safe logging locally with network forwarding to a central server.

```cpp
#include <kcenon/logger/safety/crash_safe_logger.h>
#include <kcenon/logger/writers/network_writer.h>
#include <kcenon/logger/writers/rotating_file_writer.h>

// Create local crash-safe file logger
auto local_file_writer = std::make_unique<rotating_file_writer>(
    "/var/log/app/local.log",
    10 * 1024 * 1024,  // 10MB per file
    5                  // Keep 5 files
);

auto local_logger = std::make_shared<logger>(true, 16384);
local_logger->add_writer(std::move(local_file_writer));

auto safe_logger = crash_safe_logger::create(local_logger);
safe_logger->install_crash_handlers();
safe_logger->set_auto_flush_interval(std::chrono::seconds(5));

// Add network writer for centralized logging
auto network_writer = std::make_unique<network_writer>(
    "log-server.internal",  // Server hostname
    9999                    // Server port
);

local_logger->add_writer(std::move(network_writer));

// Start logging
safe_logger->start();

// Now logs go to BOTH local file (crash-safe) AND network server
safe_logger->log(log_level::info, "Application started with dual logging");
```

**Benefits**:
- Local logs survive crashes (emergency flush to disk)
- Centralized logs for aggregation and analysis
- Redundancy: If network fails, local logs still captured

**Deployment Topology**:

```
┌─────────────────────────────────────────────────────────┐
│                  Application Node                        │
│                                                          │
│  ┌──────────────────────────────────────────────────┐   │
│  │            crash_safe_logger                     │   │
│  │                                                  │   │
│  │  ┌────────────────────────────────────────────┐ │   │
│  │  │       underlying logger                    │ │   │
│  │  │                                            │ │   │
│  │  │  ┌─────────────────┐  ┌─────────────────┐ │ │   │
│  │  │  │rotating_file    │  │network_writer   │ │ │   │
│  │  │  │_writer          │  │                 │ │ │   │
│  │  │  └────────┬────────┘  └────────┬────────┘ │ │   │
│  │  └───────────┼────────────────────┼──────────┘ │   │
│  └──────────────┼────────────────────┼────────────┘   │
│                 │                    │                 │
│                 ▼                    │                 │
│    /var/log/app/local.log            │                 │
│    (CRASH-SAFE: emergency_flush)     │                 │
└──────────────────────────────────────┼─────────────────┘
                                       │
                                       │ TCP (network)
                                       │
                                       ▼
                            ┌──────────────────┐
                            │   log_server     │
                            │   (Centralized)  │
                            └────────┬─────────┘
                                     │
                                     ▼
                       /var/log/aggregated/app.log
```

### Server-Side Crash Safety

Protect the log server itself from crashes.

```cpp
#include <kcenon/logger/server/log_server.h>
#include <kcenon/logger/safety/crash_safe_logger.h>

// Create crash-safe logger for server's own logging
auto server_logger = crash_safe_logger::create();
server_logger->install_crash_handlers();
server_logger->set_auto_flush_interval(std::chrono::seconds(5));
server_logger->start();

// Create log server
server_config config;
config.host = "0.0.0.0";
config.port = 9999;
config.max_connections = 500;

auto server = log_server_factory::create_basic(config);

// Start server
if (!server->start()) {
    server_logger->log(log_level::error, "Failed to start log server");
    return 1;
}

server_logger->log(log_level::info,
    "Log server started on port " + std::to_string(config.port));

// Monitor server health
while (server->is_running()) {
    server_logger->log(log_level::debug, "Server health check: OK");
    std::this_thread::sleep_for(std::chrono::minutes(1));
}
```

**Benefits**:
- Server crash logs preserved for debugging
- Server health monitoring with guaranteed persistence
- Operational visibility even during server failures

### Complete Production Topology

Full production deployment with all features enabled.

```cpp
// ============================================================
// CLIENT APPLICATION NODE
// ============================================================
#include <kcenon/logger/safety/crash_safe_logger.h>
#include <kcenon/logger/writers/rotating_file_writer.h>
#include <kcenon/logger/writers/network_writer.h>
#include <kcenon/logger/writers/filtered_writer.h>
#include <kcenon/logger/formatters/json_formatter.h>

// Create base logger with JSON formatting
auto base_logger = std::make_shared<logger>(true, 32768);

// Add local crash-safe file writer (for critical logs)
auto local_writer = std::make_unique<rotating_file_writer>(
    "/var/log/app/critical.log",
    50 * 1024 * 1024,  // 50MB per file
    10                 // Keep 10 files
);

// Filter: Only ERROR and FATAL to local file
auto filtered_local = std::make_unique<filtered_writer>(
    std::move(local_writer),
    log_level::error  // Minimum level
);

base_logger->add_writer(std::move(filtered_local));

// Add network writer (for all logs)
auto network = std::make_unique<network_writer>(
    "log-server.prod.internal",
    9999
);

base_logger->add_writer(std::move(network));

// Wrap in crash-safe logger
auto app_logger = crash_safe_logger::create(base_logger);
app_logger->install_crash_handlers();
app_logger->set_auto_flush_interval(std::chrono::seconds(3));
app_logger->set_min_level(log_level::info);
app_logger->start();

// ============================================================
// LOG SERVER (CENTRALIZED)
// ============================================================
#include <kcenon/logger/server/log_server.h>
#include <kcenon/logger/safety/crash_safe_logger.h>

// Server's own crash-safe logger
auto server_logger = crash_safe_logger::create();
server_logger->install_crash_handlers();
server_logger->set_auto_flush_interval(std::chrono::seconds(5));
server_logger->start();

// Create log server
server_config server_conf;
server_conf.host = "0.0.0.0";
server_conf.port = 9999;
server_conf.max_connections = 1000;
server_conf.buffer_size = 65536;
server_conf.enable_compression = true;

auto log_srv = log_server_factory::create_basic(server_conf);

if (!log_srv->start()) {
    server_logger->log(log_level::fatal, "Failed to start log server");
    return 1;
}

server_logger->log(log_level::info,
    "Production log server started with crash protection");

// Server monitoring loop
while (log_srv->is_running()) {
    server_logger->log(log_level::info, "Server operational");
    std::this_thread::sleep_for(std::chrono::minutes(5));
}
```

**Topology Diagram**:

```
┌─────────────────────────────────────────────────────────┐
│                 Application Node 1                       │
│                                                          │
│  crash_safe_logger                                       │
│    ├─ rotating_file_writer (ERROR+)  → /var/log/app/    │
│    └─ network_writer (ALL)           → log-server:9999  │
└──────────────────────────────────────┼──────────────────┘
                                       │
┌──────────────────────────────────────┼──────────────────┐
│                 Application Node 2   │                   │
│                                      │                   │
│  crash_safe_logger                   │                   │
│    ├─ rotating_file_writer (ERROR+)  │                   │
│    └─ network_writer (ALL) ──────────┘                   │
└──────────────────────────────────────────────────────────┘
                                       │
                                       ▼
                            ┌──────────────────┐
                            │   log_server     │
                            │ (crash_safe_logger│
                            │   protection)    │
                            └────────┬─────────┘
                                     │
                      ┌──────────────┼──────────────┐
                      │              │              │
                      ▼              ▼              ▼
               rotating_file   json_writer    syslog
                  (hourly)     (for ELK)    (legacy)
                      │              │              │
                      ▼              ▼              ▼
              /var/log/       /var/log/      syslog
             aggregated/        json/        daemon
```

**Features**:
- Client-side crash safety (local ERROR+ logs)
- Server-side crash safety (server operational logs)
- Centralized aggregation (all client logs)
- Multi-tier filtering (local ERROR+, remote ALL)
- High availability (local logs if network fails)
- Compression (reduced network bandwidth)

---

## Best Practices

### 1. Choose Appropriate Auto-Flush Interval

| Scenario | Recommended Interval | Rationale |
|----------|---------------------|-----------|
| **Financial transactions** | 1-2 seconds | Minimal data loss acceptable |
| **Web applications** | 5-10 seconds | Balance durability and performance |
| **Batch processing** | 30-60 seconds | Performance priority |
| **Development** | Disabled (0) | Maximize performance |

### 2. Test Signal Handlers

**Crash Test Program**:
```cpp
#include <kcenon/logger/safety/crash_safe_logger.h>
#include <thread>

void crash_test() {
    auto safe_logger = crash_safe_logger::create();
    safe_logger->install_crash_handlers();
    safe_logger->set_auto_flush_interval(std::chrono::seconds(5));
    safe_logger->start();

    // Log some messages
    for (int i = 0; i < 100; ++i) {
        safe_logger->log(log_level::info, "Message before crash: " + std::to_string(i));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Trigger crash
    safe_logger->log(log_level::fatal, "About to crash!");
    int* ptr = nullptr;
    *ptr = 42;  // SIGSEGV
}

int main() {
    crash_test();
    return 0;
}
```

**Verify Emergency Flush**:
```bash
$ ./crash_test
# Application crashes

$ tail /var/log/app.log
...
2025-02-09 14:35:22 [FATAL] About to crash!
# Last log message was flushed before crash
```

### 3. Avoid Signal Handler Conflicts

**Check Existing Handlers**:
```cpp
#include <csignal>

void check_existing_handlers() {
    struct sigaction sa;
    sigaction(SIGSEGV, nullptr, &sa);

    if (sa.sa_handler != SIG_DFL) {
        std::cerr << "Warning: SIGSEGV handler already installed\n";
        // Crash-safe logger will chain to existing handler
    }
}
```

**Multiple Libraries**: If other libraries (e.g., sanitizers, profilers) install signal handlers, `crash_safe_logger` will chain to them.

### 4. Graceful Shutdown

**Always Flush Before Exit**:
```cpp
void graceful_shutdown(std::shared_ptr<crash_safe_logger> logger) {
    logger->log(log_level::info, "Shutting down...");

    // Flush with timeout
    if (!logger->flush_with_timeout(std::chrono::seconds(5))) {
        std::cerr << "Warning: Flush timeout during shutdown\n";
    }

    // Stop logger
    logger->stop();

    // Uninstall handlers (optional, destructor does this)
    logger->uninstall_crash_handlers();
}

int main() {
    auto logger = crash_safe_logger::create();
    logger->install_crash_handlers();
    logger->start();

    // ... application logic ...

    graceful_shutdown(logger);
    return 0;
}
```

---

## Troubleshooting

### Problem: Emergency flush not working

**Symptoms**: Logs missing after crash.

**Debugging**:
```cpp
// Add debug logging to signal handler
safe_logger->log(log_level::info, "Testing crash protection");

// Trigger crash
int* ptr = nullptr;
*ptr = 42;

// Check if last log was written
$ tail /var/log/app.log
# Missing "Testing crash protection" message
```

**Possible Causes**:
1. **Signal handlers not installed**
   ```cpp
   auto safe_logger = crash_safe_logger::create();
   // Missing: safe_logger->install_crash_handlers();
   ```
   **Solution**: Call `install_crash_handlers()` after creation.

2. **Buffer not flushed**
   ```cpp
   // Buffer size too large, emergency flush cannot complete in time
   auto base_logger = std::make_shared<logger>(true, 1048576);  // 1MB
   ```
   **Solution**: Use smaller buffers (16KB-64KB) for critical logs.

3. **Lock held during crash**
   ```cpp
   // If crash occurs while holding flush_mutex_,
   // emergency_flush() try_lock() will fail
   ```
   **Solution**: This is inherent limitation; auto-flush mitigates risk.

### Problem: High CPU usage from auto-flush

**Symptoms**: CPU usage spikes every N seconds.

**Debugging**:
```bash
$ top -p $(pgrep -f my_app)
PID  USER  CPU%  COMMAND
1234 user  35.0  my_app

# Periodic spikes align with auto-flush interval
```

**Possible Causes**:
1. **Auto-flush interval too short**
   ```cpp
   safe_logger->set_auto_flush_interval(std::chrono::milliseconds(100));  // Too frequent
   ```
   **Solution**: Increase interval to 5-10 seconds.

2. **Large buffer size**
   ```cpp
   auto base_logger = std::make_shared<logger>(true, 1048576);  // 1MB
   // Flushing 1MB every second causes high I/O
   ```
   **Solution**: Reduce buffer size or increase interval.

### Problem: Signal handler conflicts

**Symptoms**: Application crashes without emergency flush.

**Debugging**:
```cpp
// Check if signal handler was overwritten
#include <csignal>

void verify_handlers() {
    struct sigaction sa;
    sigaction(SIGSEGV, nullptr, &sa);

    if (sa.sa_handler != &crash_safe_logger::signal_handler) {
        std::cerr << "Warning: Signal handler overwritten\n";
    }
}
```

**Possible Causes**:
1. **Another library installed handlers after crash_safe_logger**
   ```cpp
   safe_logger->install_crash_handlers();  // Installs handlers
   some_library_init();  // Overwrites handlers!
   ```
   **Solution**: Install crash-safe handlers LAST.

2. **Sanitizers (ASan, TSan) override handlers**
   ```bash
   $ ASAN_OPTIONS=handle_segv=1 ./my_app
   # AddressSanitizer overrides SIGSEGV handler
   ```
   **Solution**: Use `handle_segv=0` to let crash-safe logger handle signals.

---

## Related Documentation

- [Log Server Guide](./LOG_SERVER_GUIDE.md) -- Log server architecture, configuration, API, deployment
- [Configuration Strategies](./CONFIGURATION_STRATEGIES.md) -- Strategy pattern for environment-specific configuration
- [Writer Guide](./WRITER_GUIDE.md) -- Comprehensive guide to 18 writer types and decorator pattern
- [Security Guide](./SECURITY_GUIDE.md) -- Audit logging, sanitization, and secure key storage

### Header Files
- `include/kcenon/logger/safety/crash_safe_logger.h` -- Crash-safe logger implementation
- `include/kcenon/logger/writers/network_writer.h` -- Client-side network writer

### External Resources
- [Async-Signal-Safe Functions](https://man7.org/linux/man-pages/man7/signal-safety.7.html) -- POSIX reference

---

**Document Information**:
- **Authors**: logger_system documentation team
- **Last Review**: 2025-02-09
- **Version**: 1.0.0
- **Status**: Production Ready
- **Feedback**: Submit issues to [logger_system GitHub](https://github.com/kcenon/logger_system/issues)
