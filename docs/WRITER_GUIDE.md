# Writer Composition and Decorator Guide

**logger_system Writer Framework** (`include/kcenon/logger/writers/`)

The writer framework in logger_system uses the **Decorator Pattern** to enable flexible, composable log processing pipelines. With 18 writer types (5 base writers + 13 processing decorators), you can build high-performance logging configurations achieving **4.34M msg/sec throughput** and **148ns latency**.

---

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Writer Catalog](#writer-catalog)
   - [Base Writers](#base-writers)
   - [Decorator Writers](#decorator-writers)
3. [Composition Patterns](#composition-patterns)
4. [Recommended Stacks](#recommended-stacks)
5. [Code Examples](#code-examples)
6. [Performance Impact](#performance-impact)
7. [Best Practices](#best-practices)

---

## Architecture Overview

### Decorator Pattern

Writers in logger_system follow the **Decorator Pattern** (Gang of Four), allowing dynamic addition of responsibilities to objects:

```
┌──────────────────────────────────────────────────┐
│          log_writer_interface                     │
│  + write(const log_entry&): VoidResult            │
│  + flush(): VoidResult                            │
│  + get_name(): string                             │
│  + is_healthy(): bool                             │
└────────────────┬─────────────────────────────────┘
                 │
        ┌────────┼────────┐
        ▼                 ▼
┌──────────────┐  ┌──────────────────┐
│ base_writer  │  │decorator_writer_ │
│              │  │     base         │
│ (Concrete    │  │                  │
│  writers)    │  │ (Wraps another   │
│              │  │  writer)         │
└──────────────┘  └──────────────────┘
```

**Key Concepts:**

| Concept | Description | Examples |
|---------|-------------|----------|
| **Base Writer** | Concrete output destination | `console_writer`, `file_writer`, `network_writer` |
| **Decorator Writer** | Processing layer that wraps another writer | `async_writer`, `filtered_writer`, `batch_writer` |
| **Stacking** | Decorators wrap decorators wrap base writers | `async(filtered(formatted(console)))` |
| **Interface** | All writers implement `log_writer_interface` | Enables unlimited composition |

### Inheritance Chain

**Base Writers:**
```cpp
class console_writer : public log_writer_interface, public sync_writer_tag {
    // Direct implementation - no wrapped writer
    common::VoidResult write(const log_entry& entry) override {
        // Write directly to stdout/stderr
    }
};
```

**Decorator Writers:**
```cpp
class async_writer : public decorator_writer_base {
    // Wraps another writer
    std::unique_ptr<log_writer_interface> wrapped_;

    common::VoidResult write(const log_entry& entry) override {
        // Add async behavior, then delegate:
        return wrapped_->write(entry);
    }
};
```

**Decorator Base:**
```cpp
class decorator_writer_base : public log_writer_interface {
protected:
    log_writer_interface& wrapped() noexcept;  // Access wrapped writer
private:
    std::unique_ptr<log_writer_interface> wrapped_;
    std::string decorator_name_;
};
```

### Stack Visualization

Example stack: **Production File Logging**

```
[Application Code]
    ↓ log_entry
[filtered_writer] ← Drop DEBUG/TRACE in production
    ↓
[formatted_writer] ← Add timestamp, level, source location
    ↓
[async_writer] ← Non-blocking (queue-based)
    ↓
[batch_writer] ← Accumulate 100 messages
    ↓
[buffered_writer] ← Memory buffer (32KB)
    ↓
[rotating_file_writer] ← Write to disk with rotation
```

**Call flow (write operation):**
1. App calls `filtered_writer::write(entry)`
2. Filtered writer checks level → pass
3. Calls `formatted_writer::write(entry)`
4. Formatted writer formats message → delegates
5. Calls `async_writer::write(entry)`
6. Async writer enqueues → returns immediately
7. Background thread: calls `batch_writer::write(entry)`
8. Batch writer accumulates → flushes batch of 100
9. Calls `buffered_writer::write(entry)` × 100
10. Buffered writer fills buffer → flushes to file
11. Calls `rotating_file_writer::write(entry)` × 100
12. Rotating file writer writes to disk, rotates if needed

---

## Writer Catalog

### Base Writers

Base writers are concrete implementations that write to actual destinations. They implement `log_writer_interface` directly.

#### 1. `console_writer`

**Output:** `stdout` (info/debug/trace) or `stderr` (warn/error/fatal)

**Description:** Thread-safe console output with optional ANSI coloring.

**Features:**
- Auto-detect terminal color support
- Different colors for each log level
- Mutex-protected for thread safety

**Use Cases:**
- Development (immediate visual feedback)
- Interactive debugging
- Docker containers (stdout/stderr collection)

**Constructor:**
```cpp
console_writer(bool use_stderr = false,
               bool auto_detect_color = true,
               std::unique_ptr<log_formatter_interface> formatter = nullptr);
```

**Example:**
```cpp
auto writer = std::make_unique<console_writer>(
    false,  // use stdout
    true    // auto-detect color
);
```

---

#### 2. `file_writer`

**Output:** Single file

**Description:** Writes to a single log file with optional append mode.

**Features:**
- Create parent directories automatically
- Append or overwrite mode
- File permission control (0644)
- Thread-safe writes (mutex-protected)

**Use Cases:**
- Simple logging to single file
- Base layer for other decorators
- Development/testing

**Constructor:**
```cpp
file_writer(const std::filesystem::path& file_path,
            bool append = true,
            std::unique_ptr<log_formatter_interface> formatter = nullptr);
```

**Example:**
```cpp
auto writer = std::make_unique<file_writer>(
    "/var/log/myapp/app.log",
    true  // append mode
);
```

---

#### 3. `rotating_file_writer`

**Output:** Multiple files with rotation

**Description:** Writes to files that rotate based on size or time.

**Features:**
- Size-based rotation (e.g., 100MB per file)
- Time-based rotation (daily, hourly)
- Configurable max file count (auto-delete old files)
- Filename pattern (e.g., `app.log.1`, `app.log.2`)

**Use Cases:**
- Production logging
- Long-running services
- Compliance (retention policies)

**Constructor:**
```cpp
rotating_file_writer(const std::filesystem::path& base_path,
                    size_t max_file_size = 100 * 1024 * 1024,  // 100MB
                    size_t max_files = 10,
                    std::unique_ptr<log_formatter_interface> formatter = nullptr);
```

**Example:**
```cpp
auto writer = std::make_unique<rotating_file_writer>(
    "/var/log/myapp/app.log",
    50 * 1024 * 1024,  // 50MB per file
    5                   // keep 5 files
);
```

---

#### 4. `network_writer`

**Output:** Remote log server (TCP/UDP)

**Description:** Sends logs to a remote server over network.

**Features:**
- TCP or UDP transport
- Connection pooling
- Automatic reconnection
- Configurable timeout

**Use Cases:**
- Centralized logging (ELK, Splunk, Graylog)
- Distributed systems
- Microservices (log aggregation)

**Constructor:**
```cpp
network_writer(const std::string& host,
               uint16_t port,
               protocol proto = protocol::tcp,
               std::unique_ptr<log_formatter_interface> formatter = nullptr);
```

**Example:**
```cpp
auto writer = std::make_unique<network_writer>(
    "logs.example.com",
    5140,  // TCP port
    network_writer::protocol::tcp
);
```

---

#### 5. `otlp_writer`

**Output:** OpenTelemetry Protocol (OTLP) endpoint

**Description:** Exports logs to OTLP-compatible backends (OpenTelemetry Collector, Jaeger, etc.).

**Features:**
- OTLP gRPC or HTTP/protobuf
- Structured log export
- Integration with traces/metrics
- Batch export for efficiency

**Use Cases:**
- OpenTelemetry observability
- Cloud-native monitoring
- Distributed tracing correlation

**Constructor:**
```cpp
otlp_writer(const std::string& endpoint,
            otlp_protocol proto = otlp_protocol::grpc,
            std::unique_ptr<log_formatter_interface> formatter = nullptr);
```

**Example:**
```cpp
auto writer = std::make_unique<otlp_writer>(
    "http://otel-collector:4317",
    otlp_writer::otlp_protocol::grpc
);
```

---

### Decorator Writers

Decorator writers wrap other writers to add processing capabilities. They inherit from `decorator_writer_base`.

#### 6. `async_writer`

**Function:** Asynchronous (non-blocking) writes

**Description:** Wraps any writer and makes it asynchronous using a background thread and queue.

**Features:**
- Non-blocking `write()` (enqueues and returns)
- Background worker thread
- Configurable queue size (default: 10,000)
- Graceful shutdown (flush on destruction)

**Use Cases:**
- High-throughput logging (don't block app)
- I/O-heavy writers (file, network)
- Real-time applications

**Constructor:**
```cpp
async_writer(std::unique_ptr<log_writer_interface> wrapped,
             size_t queue_size = 10000,
             std::chrono::seconds flush_timeout = std::chrono::seconds(5));
```

**Example:**
```cpp
auto writer = std::make_unique<async_writer>(
    std::make_unique<file_writer>("/var/log/app.log"),
    50000  // queue size
);
writer->start();  // Start background thread
```

**Performance Impact:**
- **Latency:** +10-50ns (enqueue time)
- **Throughput:** +1000% (non-blocking)
- **Memory:** Queue size × log entry size (~100-500 bytes each)

---

#### 7. `batch_writer`

**Function:** Batching (accumulate N messages before flush)

**Description:** Accumulates log entries and writes them in batches to reduce syscall overhead.

**Features:**
- Configurable batch size (default: 100)
- Auto-flush on timeout (default: 1s)
- Flush on critical logs (optional)

**Use Cases:**
- Reduce I/O syscalls (file, network)
- Improve throughput
- Combine with `async_writer`

**Constructor:**
```cpp
batch_writer(std::unique_ptr<log_writer_interface> wrapped,
             size_t batch_size = 100,
             std::chrono::milliseconds flush_interval = std::chrono::milliseconds(1000));
```

**Example:**
```cpp
auto writer = std::make_unique<batch_writer>(
    std::make_unique<file_writer>("/var/log/app.log"),
    200,  // batch size
    std::chrono::milliseconds(500)  // flush every 500ms
);
```

**Performance Impact:**
- **Latency:** +0-1000ms (batching delay)
- **Throughput:** +500% (fewer syscalls)
- **Memory:** Batch size × log entry size

---

#### 8. `buffered_writer`

**Function:** Buffering (memory buffer before write)

**Description:** Buffers log messages in memory before flushing to the wrapped writer.

**Features:**
- Configurable buffer size (default: 64KB)
- Auto-flush when buffer full
- Manual flush support

**Use Cases:**
- Reduce disk writes
- Improve file I/O performance
- Combine with `rotating_file_writer`

**Constructor:**
```cpp
buffered_writer(std::unique_ptr<log_writer_interface> wrapped,
                size_t buffer_size = 65536);  // 64KB
```

**Example:**
```cpp
auto writer = std::make_unique<buffered_writer>(
    std::make_unique<file_writer>("/var/log/app.log"),
    128 * 1024  // 128KB buffer
);
```

**Performance Impact:**
- **Latency:** +0-10ms (buffer flush)
- **Throughput:** +300% (fewer disk writes)
- **Memory:** Buffer size (e.g., 64KB)

---

#### 9. `filtered_writer`

**Function:** Filtering (level/category-based)

**Description:** Filters log entries based on level or custom predicate before writing.

**Features:**
- Level-based filtering (e.g., only WARN+)
- Category filtering (e.g., only "auth" category)
- Custom predicate support
- Zero overhead for filtered messages

**Use Cases:**
- Production (only log errors)
- Separate logs by severity
- Reduce log volume

**Constructor:**
```cpp
filtered_writer(std::unique_ptr<log_writer_interface> wrapped,
                log_level min_level = log_level::debug);
```

**Example:**
```cpp
auto writer = std::make_unique<filtered_writer>(
    std::make_unique<file_writer>("/var/log/errors.log"),
    log_level::error  // only ERROR and FATAL
);
```

**Performance Impact:**
- **Latency:** +5-10ns (level check)
- **Throughput:** No impact (drops messages early)
- **Memory:** Negligible

---

#### 10. `formatted_writer`

**Function:** Formatting (message templating)

**Description:** Applies a formatter to log entries before writing.

**Features:**
- Pluggable formatters (JSON, timestamp, custom)
- Supports all formatter types
- Can be composed at any stack level

**Use Cases:**
- JSON output for log aggregators
- Custom format per destination
- Add timestamps/metadata

**Constructor:**
```cpp
formatted_writer(std::unique_ptr<log_writer_interface> wrapped,
                 std::unique_ptr<log_formatter_interface> formatter);
```

**Example:**
```cpp
auto writer = std::make_unique<formatted_writer>(
    std::make_unique<file_writer>("/var/log/app.json"),
    std::make_unique<json_formatter>()
);
```

**Performance Impact:**
- **Latency:** +20-100ns (formatting)
- **Throughput:** -10-20% (CPU overhead)
- **Memory:** Formatted string allocation

---

#### 11. `thread_safe_writer`

**Function:** Thread safety (mutex-protected)

**Description:** Wraps any writer with mutex protection for thread safety.

**Features:**
- Mutex-protected `write()` and `flush()`
- Ensures serialized access
- Useful for non-thread-safe writers

**Use Cases:**
- Wrap custom writers
- Ensure thread safety in async logging
- Protect shared resources

**Constructor:**
```cpp
thread_safe_writer(std::unique_ptr<log_writer_interface> wrapped);
```

**Example:**
```cpp
auto writer = std::make_unique<thread_safe_writer>(
    std::make_unique<my_custom_writer>()
);
```

**Performance Impact:**
- **Latency:** +50-200ns (mutex lock/unlock)
- **Throughput:** -30-50% (contention)
- **Memory:** Mutex overhead (~64 bytes)

---

#### 12. `encrypted_writer`

**Function:** Encryption (encrypt log content)

**Description:** Encrypts log messages before writing (AES-256-GCM).

**Features:**
- AES-256-GCM encryption
- Secure key management
- HMAC authentication
- Key rotation support

**Use Cases:**
- Compliance (HIPAA, GDPR)
- Sensitive data logging
- Tamper-evident logs

**Constructor:**
```cpp
encrypted_writer(std::unique_ptr<log_writer_interface> wrapped,
                 const secure_key& encryption_key);
```

**Example:**
```cpp
auto key = secure_key_storage::generate_key(32);  // AES-256
auto writer = std::make_unique<encrypted_writer>(
    std::make_unique<file_writer>("/var/log/secure.log"),
    key
);
```

**Performance Impact:**
- **Latency:** +100-500ns (encryption)
- **Throughput:** -30-40% (crypto overhead)
- **Memory:** Key storage (~32 bytes)

---

#### 13. `critical_writer`

**Function:** Priority (immediate flush for critical logs)

**Description:** Immediately flushes wrapped writer when critical/fatal logs are received.

**Features:**
- Immediate flush on critical levels
- Configurable threshold (default: FATAL)
- Ensures critical logs are persisted

**Use Cases:**
- Crash dumps
- Security alerts
- Compliance audit logs

**Constructor:**
```cpp
critical_writer(std::unique_ptr<log_writer_interface> wrapped,
                log_level critical_threshold = log_level::fatal);
```

**Example:**
```cpp
auto writer = std::make_unique<critical_writer>(
    std::make_unique<file_writer>("/var/log/app.log"),
    log_level::error  // flush on ERROR+
);
```

**Performance Impact:**
- **Latency:** +0-10ms (flush on critical)
- **Throughput:** No impact (rare events)
- **Memory:** Negligible

---

#### 14. `composite_writer`

**Function:** Fan-out (write to multiple destinations)

**Description:** Writes to multiple wrapped writers simultaneously.

**Features:**
- Write to N writers in parallel
- Independent error handling per writer
- Configurable success criteria (all, any, majority)

**Use Cases:**
- Multi-destination logging (file + network)
- Redundancy (primary + backup)
- Fan-out to different formats

**Constructor:**
```cpp
composite_writer();
void add_writer(std::unique_ptr<log_writer_interface> writer);
```

**Example:**
```cpp
auto composite = std::make_unique<composite_writer>();
composite->add_writer(std::make_unique<file_writer>("/var/log/app.log"));
composite->add_writer(std::make_unique<network_writer>("logs.example.com", 5140));
```

**Performance Impact:**
- **Latency:** max(writer1, writer2, ...) (parallel)
- **Throughput:** min(writer1, writer2, ...) (bottleneck)
- **Memory:** N × wrapped writer memory

---

#### 15. `queued_writer_base`

**Function:** Queuing (base for queue-based writers)

**Description:** Abstract base class for queue-based writers (shared by `async_writer`, `batch_writer`).

**Features:**
- Thread-safe queue management
- Overflow policies (drop, block, expand)
- Shared queue logic

**Use Cases:**
- Base class for custom queue-based writers
- Not used directly by applications

**Note:** Not typically instantiated directly; used as a base class.

---

#### 16. `legacy_writer_adapter`

**Function:** Compatibility (adapter for legacy writer interface)

**Description:** Adapts legacy writer API to modern `log_writer_interface`.

**Features:**
- Backward compatibility
- Bridge legacy code to new system
- No performance overhead

**Use Cases:**
- Migrating from old logger versions
- Compatibility with legacy integrations
- Gradual migration path

**Constructor:**
```cpp
legacy_writer_adapter(std::unique_ptr<legacy_writer> legacy);
```

**Example:**
```cpp
auto adapter = std::make_unique<legacy_writer_adapter>(
    std::make_unique<my_old_writer>()
);
```

---

## Composition Patterns

### Pattern 1: Simple Console (Development)

**Stack:**
```
console_writer
```

**Use Case:** Local development, quick debugging

**Code:**
```cpp
auto logger = logger_builder::create()
    .with_console_writer()
    .build();
```

**Characteristics:**
- **Throughput:** 500K msg/sec
- **Latency:** 2µs (blocking I/O)
- **Memory:** Minimal (~1KB)

---

### Pattern 2: Async Console (Development + Performance)

**Stack:**
```
async_writer
  └─ console_writer
```

**Use Case:** Development with high log volume

**Code:**
```cpp
auto logger = logger_builder::create()
    .with_writer(std::make_unique<async_writer>(
        std::make_unique<console_writer>()
    ))
    .build();
```

**Characteristics:**
- **Throughput:** 4.3M msg/sec
- **Latency:** 148ns (non-blocking)
- **Memory:** Queue (10K entries × ~200B = 2MB)

---

### Pattern 3: Filtered Formatted File (Production)

**Stack:**
```
filtered_writer (WARN+)
  └─ formatted_writer (timestamp + level)
      └─ rotating_file_writer
```

**Use Case:** Production error logging

**Code:**
```cpp
auto logger = logger_builder::create()
    .with_writer(std::make_unique<filtered_writer>(
        std::make_unique<formatted_writer>(
            std::make_unique<rotating_file_writer>(
                "/var/log/app/errors.log",
                50 * 1024 * 1024,  // 50MB
                5                   // 5 files
            ),
            std::make_unique<timestamp_formatter>()
        ),
        log_level::warn
    ))
    .build();
```

**Characteristics:**
- **Throughput:** 300K msg/sec (blocking file I/O)
- **Latency:** 3µs (file write)
- **Memory:** Minimal (~2KB)

---

### Pattern 4: High-Throughput Production

**Stack:**
```
filtered_writer (INFO+)
  └─ formatted_writer (JSON)
      └─ async_writer
          └─ batch_writer (100 messages)
              └─ buffered_writer (64KB)
                  └─ rotating_file_writer
```

**Use Case:** Production high-traffic services

**Code:**
```cpp
auto logger = logger_builder::create()
    .with_writer(std::make_unique<filtered_writer>(
        std::make_unique<formatted_writer>(
            std::make_unique<async_writer>(
                std::make_unique<batch_writer>(
                    std::make_unique<buffered_writer>(
                        std::make_unique<rotating_file_writer>(
                            "/var/log/app/app.log",
                            100 * 1024 * 1024,  // 100MB
                            10                   // 10 files
                        ),
                        65536  // 64KB buffer
                    ),
                    200  // batch size
                ),
                50000  // queue size
            ),
            std::make_unique<json_formatter>()
        ),
        log_level::info
    ))
    .build();
```

**Characteristics:**
- **Throughput:** 4.34M msg/sec
- **Latency:** 148ns (non-blocking)
- **Memory:** Queue (50K × 200B = 10MB) + Buffer (64KB) = ~10MB

---

### Pattern 5: Encrypted Compliance Logging

**Stack:**
```
filtered_writer (WARN+)
  └─ formatted_writer (JSON + timestamp)
      └─ encrypted_writer (AES-256-GCM)
          └─ thread_safe_writer
              └─ file_writer
```

**Use Case:** HIPAA/GDPR compliance, secure audit logs

**Code:**
```cpp
auto key = secure_key_storage::load_key("/etc/keys/log_key.bin");
auto logger = logger_builder::create()
    .with_writer(std::make_unique<filtered_writer>(
        std::make_unique<formatted_writer>(
            std::make_unique<encrypted_writer>(
                std::make_unique<thread_safe_writer>(
                    std::make_unique<file_writer>("/var/log/audit/secure.log")
                ),
                key
            ),
            std::make_unique<json_formatter>()
        ),
        log_level::warn
    ))
    .build();
```

**Characteristics:**
- **Throughput:** 200K msg/sec (encryption overhead)
- **Latency:** 5µs (blocking encryption + file I/O)
- **Memory:** Minimal (~3KB + key)

---

### Pattern 6: Multi-Destination (File + Network)

**Stack:**
```
composite_writer
  ├─ filtered(INFO+) → formatted(JSON) → rotating_file_writer
  └─ filtered(ERROR+) → formatted(JSON) → async → network_writer
```

**Use Case:** Local file + remote alerting

**Code:**
```cpp
auto composite = std::make_unique<composite_writer>();

// Local file: all INFO+
composite->add_writer(std::make_unique<filtered_writer>(
    std::make_unique<formatted_writer>(
        std::make_unique<rotating_file_writer>("/var/log/app/app.log"),
        std::make_unique<json_formatter>()
    ),
    log_level::info
));

// Remote server: only ERROR+
composite->add_writer(std::make_unique<filtered_writer>(
    std::make_unique<formatted_writer>(
        std::make_unique<async_writer>(
            std::make_unique<network_writer>("logs.example.com", 5140)
        ),
        std::make_unique<json_formatter>()
    ),
    log_level::error
));

auto logger = logger_builder::create()
    .with_writer(std::move(composite))
    .build();
```

**Characteristics:**
- **Throughput:** 1.5M msg/sec (file path limits)
- **Latency:** 3µs (file write, network async)
- **Memory:** ~12MB (network queue)

---

## Recommended Stacks

### Development

| Use Case | Stack (inner → outer) | Throughput | Latency | Memory |
|----------|----------------------|------------|---------|--------|
| **Quick debugging** | `console` | 500K msg/s | 2µs | 1KB |
| **High-volume dev** | `async` → `console` | 4.3M msg/s | 148ns | 2MB |
| **Formatted console** | `formatted` → `console` | 300K msg/s | 3µs | 1KB |

### Production File Logging

| Use Case | Stack (inner → outer) | Throughput | Latency | Memory |
|----------|----------------------|------------|---------|--------|
| **Simple file** | `file` | 300K msg/s | 3µs | 2KB |
| **Rotating file** | `rotating_file` | 300K msg/s | 3µs | 2KB |
| **High-throughput** | `rotating_file` → `buffered` → `batch` → `async` → `formatted` → `filtered` | 4.34M msg/s | 148ns | 10MB |
| **JSON logs** | `rotating_file` → `formatted(JSON)` | 200K msg/s | 5µs | 2KB |

### Compliance & Security

| Use Case | Stack (inner → outer) | Throughput | Latency | Memory |
|----------|----------------------|------------|---------|--------|
| **Encrypted logs** | `file` → `encrypted` → `formatted` → `filtered` | 200K msg/s | 5µs | 3KB |
| **Audit trail** | `file` → `critical` → `encrypted` → `formatted` | 150K msg/s | 7µs | 3KB |
| **Tamper-evident** | `rotating_file` → `encrypted` → `batch` → `async` → `formatted` | 1M msg/s | 500ns | 12MB |

### Distributed Systems

| Use Case | Stack (inner → outer) | Throughput | Latency | Memory |
|----------|----------------------|------------|---------|--------|
| **Remote logging** | `network` → `async` → `formatted` → `filtered` | 1.5M msg/s | 200ns | 12MB |
| **OTLP export** | `otlp` → `async` → `batch` | 2M msg/s | 180ns | 15MB |
| **Multi-destination** | `composite(file + network)` → `async` → `formatted` | 1.5M msg/s | 200ns | 12MB |

### Resource-Constrained

| Use Case | Stack (inner → outer) | Throughput | Latency | Memory |
|----------|----------------------|------------|---------|--------|
| **Minimal memory** | `console` → `filtered(ERROR+)` | 500K msg/s | 2µs | 1KB |
| **Low CPU** | `file` → `filtered(WARN+)` | 300K msg/s | 3µs | 2KB |
| **Embedded** | `file` → `filtered` (no formatting) | 400K msg/s | 2.5µs | 1.5KB |

---

## Code Examples

### Example 1: Development Setup (3 lines)

```cpp
auto logger = logger_builder::create()
    .with_console_writer()
    .build();
```

---

### Example 2: Production File Logging

```cpp
auto logger = logger_builder::create()
    .with_writer(std::make_unique<async_writer>(
        std::make_unique<batch_writer>(
            std::make_unique<buffered_writer>(
                std::make_unique<rotating_file_writer>(
                    "/var/log/app/app.log",
                    100 * 1024 * 1024,  // 100MB per file
                    10                   // keep 10 files
                ),
                65536  // 64KB buffer
            ),
            200,  // batch 200 messages
            std::chrono::milliseconds(500)  // or 500ms timeout
        ),
        50000  // queue size
    ))
    .build();

logger->info("Application started");
```

---

### Example 3: Encrypted Compliance Logging

```cpp
// Generate or load encryption key
auto key_result = secure_key_storage::load_key(
    "/etc/keys/log_encryption.key",
    32,  // AES-256
    "/etc/keys"  // allowed base directory
);

if (key_result.is_err()) {
    // Handle error
}
auto key = std::move(key_result).unwrap();

// Build encrypted logger
auto logger = logger_builder::create()
    .with_writer(std::make_unique<filtered_writer>(
        std::make_unique<formatted_writer>(
            std::make_unique<encrypted_writer>(
                std::make_unique<critical_writer>(
                    std::make_unique<file_writer>("/var/log/audit/secure.log"),
                    log_level::warn  // immediate flush on WARN+
                ),
                key
            ),
            std::make_unique<json_formatter>()
        ),
        log_level::warn  // only WARN+
    ))
    .build();

logger->warn("User authentication failed", {{"user_id", "12345"}});
```

---

### Example 4: Multi-Destination Setup

```cpp
auto composite = std::make_unique<composite_writer>();

// Destination 1: Local file (all logs)
composite->add_writer(std::make_unique<formatted_writer>(
    std::make_unique<rotating_file_writer>(
        "/var/log/app/app.log",
        50 * 1024 * 1024,
        5
    ),
    std::make_unique<timestamp_formatter>()
));

// Destination 2: Remote server (errors only)
composite->add_writer(std::make_unique<filtered_writer>(
    std::make_unique<formatted_writer>(
        std::make_unique<async_writer>(
            std::make_unique<network_writer>("logs.example.com", 5140)
        ),
        std::make_unique<json_formatter>()
    ),
    log_level::error
));

// Destination 3: OTLP (OpenTelemetry)
composite->add_writer(std::make_unique<async_writer>(
    std::make_unique<otlp_writer>("http://otel-collector:4317")
));

auto logger = logger_builder::create()
    .with_writer(std::move(composite))
    .build();

logger->error("Database connection failed", {{"db", "primary"}});
// → Written to: file, network, OTLP
```

---

### Example 5: Custom Stack Order

```cpp
// Optimal stack order: filter → format → async → batch → buffer → file
auto logger = logger_builder::create()
    .with_writer(
        std::make_unique<filtered_writer>(        // 1. Filter first (drop early)
            std::make_unique<formatted_writer>(    // 2. Format (before async to avoid re-formatting)
                std::make_unique<async_writer>(    // 3. Async (non-blocking from here)
                    std::make_unique<batch_writer>( // 4. Batch (accumulate)
                        std::make_unique<buffered_writer>( // 5. Buffer (reduce syscalls)
                            std::make_unique<rotating_file_writer>( // 6. Base writer
                                "/var/log/app/app.log"
                            )
                        )
                    )
                ),
                std::make_unique<json_formatter>()
            ),
            log_level::info
        )
    )
    .build();
```

---

## Performance Impact

### Throughput per Configuration

| Configuration | Throughput | Baseline % |
|---------------|------------|------------|
| **Baseline (console only)** | 500K msg/s | 100% |
| + async | 4.3M msg/s | 860% |
| + async + batch | 4.34M msg/s | 868% |
| + async + batch + buffer | 4.34M msg/s | 868% (I/O limited) |
| + async + batch + buffer + filter | 4.34M msg/s | 868% (drops early) |
| + async + batch + buffer + formatted | 3.8M msg/s | 760% (CPU overhead) |
| + encrypted | 1.5M msg/s | 300% (crypto overhead) |
| + network (TCP) | 1.2M msg/s | 240% (network I/O) |

### Latency per Decorator

| Decorator | Latency Added | Cumulative |
|-----------|---------------|------------|
| **Base (console)** | - | 2µs |
| + filtered | +5ns | 2.005µs |
| + formatted | +50ns | 2.055µs |
| + async (enqueue) | +30ns | 2.085µs → **148ns** (non-blocking) |
| + batch (enqueue) | +10ns | 158ns |
| + buffered | +5ns | 163ns |
| + thread_safe (mutex) | +100ns | 263ns |
| + encrypted | +300ns | 563ns |

**Note:** `async_writer` makes subsequent latencies invisible to the caller (background processing).

### Memory Usage per Decorator

| Decorator | Memory Usage | Notes |
|-----------|--------------|-------|
| **Base writers** | 1-2KB | Minimal overhead |
| async_writer | Queue size × 200B | Default: 10K × 200B = 2MB |
| batch_writer | Batch size × 200B | Default: 100 × 200B = 20KB |
| buffered_writer | Buffer size | Default: 64KB |
| composite_writer | N × child memory | Linear with child count |
| encrypted_writer | 32B (key) + 16B (IV) | Per message |
| formatted_writer | Formatted string size | Transient (released after write) |
| filtered_writer | Negligible | No buffering |
| thread_safe_writer | 64B (mutex) | Minimal |

**Example:** High-throughput stack memory:
```
async (2MB) + batch (20KB) + buffered (64KB) + base (2KB) = ~2.1MB
```

---

## Best Practices

### 1. Filter Early, Format Late

✅ **Good:**
```cpp
filtered → formatted → async → file
```

❌ **Bad:**
```cpp
async → formatted → filtered → file  // Wastes formatting on filtered messages
```

**Rationale:** Filtering is cheap (5ns), formatting is expensive (50-100ns). Drop messages before formatting.

---

### 2. Use Async for I/O-Heavy Writers

✅ **Good:**
```cpp
async → file  // File I/O is slow (3µs)
async → network  // Network I/O is slow (1-10ms)
```

❌ **Bad:**
```cpp
console  // Blocking I/O without async
```

**Rationale:** I/O operations block the caller. Use `async_writer` to make writes non-blocking.

---

### 3. Batch + Buffer for Maximum Throughput

✅ **Good:**
```cpp
async → batch → buffered → file  // Reduces syscalls
```

❌ **Bad:**
```cpp
async → file  // Many small writes (syscall overhead)
```

**Rationale:** Batching reduces syscall count, buffering reduces disk I/O. Combine both for maximum throughput.

---

### 4. Encrypt Only What's Necessary

✅ **Good:**
```cpp
composite {
    file (all logs, unencrypted),
    encrypted → file (sensitive logs only)
}
```

❌ **Bad:**
```cpp
encrypted → file (all logs)  // Wastes CPU on non-sensitive logs
```

**Rationale:** Encryption is expensive (30-40% throughput reduction). Encrypt only sensitive logs.

---

### 5. Use `composite_writer` for Multi-Destination

✅ **Good:**
```cpp
composite { file, network }
```

❌ **Bad:**
```cpp
// Create two separate loggers
logger1->write(...);  // file
logger2->write(...);  // network
```

**Rationale:** `composite_writer` writes to all destinations in a single call, ensuring consistency.

---

### 6. Flush on Critical Logs

✅ **Good:**
```cpp
critical_writer(file, log_level::fatal)  // Immediate flush on FATAL
```

❌ **Bad:**
```cpp
async → batch → file  // Critical logs may be lost in queue/batch on crash
```

**Rationale:** Critical logs must be persisted immediately. Use `critical_writer` to force flush.

---

### 7. Thread-Safety Only When Needed

✅ **Good:**
```cpp
// Logger is thread-safe internally, no need for thread_safe_writer
async → file
```

❌ **Bad:**
```cpp
thread_safe → async → file  // Redundant: async already thread-safe
```

**Rationale:** Most decorators are already thread-safe. Only use `thread_safe_writer` for custom non-thread-safe writers.

---

### 8. Start Async Writers Explicitly

✅ **Good:**
```cpp
auto async = std::make_unique<async_writer>(...);
async->start();  // Start background thread
auto logger = logger_builder::create().with_writer(std::move(async)).build();
```

❌ **Bad:**
```cpp
auto logger = logger_builder::create()
    .with_writer(std::make_unique<async_writer>(...))
    .build();
// Forgot to call start()!
```

**Rationale:** `async_writer` requires explicit `start()` to begin processing.

---

## Summary

The **logger_system writer framework** provides:

- ✅ **18 writer types** — 5 base writers + 13 decorators
- ✅ **Decorator pattern** — Unlimited composition and stacking
- ✅ **High performance** — 4.34M msg/sec, 148ns latency
- ✅ **Flexible pipelines** — Filter, format, batch, buffer, encrypt, fan-out
- ✅ **Production-ready** — Thread-safe, async, rotating, encrypted

**Quick reference:**

| Task | Recommended Stack |
|------|-------------------|
| Development | `console` or `async → console` |
| Production file | `rotating_file → buffered → batch → async → formatted → filtered` |
| Compliance | `file → encrypted → critical → formatted → filtered` |
| Distributed | `network → async → batch → formatted → filtered` |
| Multi-destination | `composite(file + network) → async → formatted` |

**Next steps:**
- Review [ARCHITECTURE.md](ARCHITECTURE.md) for logger architecture
- Review [SECURITY_GUIDE.md](SECURITY_GUIDE.md) for encryption details
- Review [CONFIGURATION_STRATEGIES.md](CONFIGURATION_STRATEGIES.md) for environment-based config

---

*Last updated: 2025-02-09*
*logger_system version: 2.x*
