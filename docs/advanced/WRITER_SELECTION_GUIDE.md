> **Language:** **English** | [한국어](WRITER_SELECTION_GUIDE_KO.md)

# Writer Selection Guide

This guide helps you choose the right writer for your logging needs.

## Quick Selection Flowchart

```
                    ┌─────────────────────────────┐
                    │  What is your output        │
                    │  destination?               │
                    └──────────────┬──────────────┘
                                   │
         ┌─────────────────────────┼─────────────────────────┐
         ▼                         ▼                         ▼
    ┌─────────┐              ┌─────────┐              ┌───────────┐
    │ Console │              │  File   │              │ Network   │
    └────┬────┘              └────┬────┘              └─────┬─────┘
         │                        │                         │
         ▼                        ▼                         ▼
  ┌──────────────┐    ┌─────────────────────┐    ┌──────────────────┐
  │console_writer│    │ Need rotation?      │    │ OpenTelemetry?   │
  └──────────────┘    └──────────┬──────────┘    └────────┬─────────┘
                           Y/    │    \N                   │
                          /      │     \           ┌───────┴───────┐
                         ▼       │      ▼          ▼               ▼
          ┌───────────────────┐  │  ┌───────────┐  │    ┌──────────────┐
          │rotating_file_writer│ │  │file_writer│  │    │ network_writer│
          └───────────────────┘  │  └───────────┘  │    └──────────────┘
                                 │                 ▼
                                 │         ┌──────────┐
                                 │         │otlp_writer│
                                 │         └──────────┘
                                 ▼
                    ┌─────────────────────────┐
                    │ Need high throughput?   │
                    │ (>10K msg/sec)          │
                    └───────────┬─────────────┘
                           Y/   │   \N
                          /     │    \
                         ▼      │     ▼
              ┌─────────────┐   │   (Use writer directly)
              │ Wrap with   │   │
              │ async_writer│   │
              │ or          │   │
              │ batch_writer│   │
              └─────────────┘   │
                                ▼
                    ┌─────────────────────────┐
                    │ Need encryption?        │
                    └───────────┬─────────────┘
                           Y/   │   \N
                          /     │    \
                         ▼      │     ▼
              ┌────────────────┐│   (Done!)
              │encrypted_writer││
              └────────────────┘│
                                ▼
                    ┌─────────────────────────┐
                    │ Need critical message   │
                    │ guaranteed delivery?    │
                    └───────────┬─────────────┘
                           Y/   │   \N
                          /     │    \
                         ▼      │     ▼
              ┌────────────────┐│   (Done!)
              │critical_writer ││
              │  or            ││
              │hybrid_writer   ││
              └────────────────┘│
                                ▼
                            (Done!)
```

## Writer Selection by Use Case

### 1. Simple Console Logging

**Use:** `console_writer`

```cpp
auto writer = std::make_unique<console_writer>();
```

**Best for:** Development, debugging, CLI applications

---

### 2. Basic File Logging

**Use:** `file_writer`

```cpp
auto writer = std::make_unique<file_writer>("app.log");
```

**Best for:** Small applications, simple logging needs

---

### 3. Production File Logging with Rotation

**Use:** `rotating_file_writer`

```cpp
// Size-based rotation: 10MB max, keep 5 backups
auto writer = std::make_unique<rotating_file_writer>(
    "app.log",
    10 * 1024 * 1024,  // 10MB
    5                   // max backups
);

// Time-based rotation: daily
auto writer = std::make_unique<rotating_file_writer>(
    "app.log",
    rotation_type::daily,
    30  // keep 30 days
);
```

**Best for:** Production applications with log management needs

---

### 4. High-Throughput Logging (>10K msg/sec)

**Use:** `async_writer` or `batch_writer`

```cpp
// Option 1: async_writer - queue-based async
auto file = std::make_unique<file_writer>("app.log");
auto async = std::make_unique<async_writer>(
    std::move(file),
    10000,                     // queue size
    std::chrono::seconds(5)    // flush timeout
);
async->start();

// Option 2: batch_writer - batched writes
auto file = std::make_unique<file_writer>("app.log");
batch_writer::config cfg;
cfg.max_batch_size = 100;
cfg.flush_interval = std::chrono::milliseconds(100);
auto batch = std::make_unique<batch_writer>(std::move(file), cfg);
```

**Best for:** High-volume applications, microservices, data processing

---

### 5. Network/Remote Logging

**Use:** `network_writer` or `otlp_writer`

```cpp
// TCP/UDP logging
auto writer = std::make_unique<network_writer>(
    "log-server.example.com",
    5514,
    network_writer::protocol_type::tcp
);

// OpenTelemetry collector
otlp_writer::config cfg;
cfg.endpoint = "http://otel-collector:4318/v1/logs";
cfg.service_name = "my-service";
auto writer = std::make_unique<otlp_writer>(cfg);
```

**Best for:** Centralized logging, observability, microservices

---

### 6. Encrypted/Secure Logging

**Use:** `encrypted_writer`

```cpp
auto key_result = secure_key_storage::generate_key(32);
encryption_config config{
    encryption_algorithm::aes_256_gcm,
    std::move(key_result.value())
};

auto writer = std::make_unique<encrypted_writer>(
    "/var/log/secure.log.enc",
    std::move(config)
);
```

**Best for:** HIPAA, GDPR, PCI DSS compliance, sensitive data

---

### 7. Critical/Safety-Critical Logging

**Use:** `critical_writer` or `hybrid_writer`

```cpp
// Option 1: critical_writer - ensures critical logs are never lost
auto file = std::make_unique<file_writer>("app.log");
critical_writer_config cfg;
cfg.force_flush_on_critical = true;
cfg.write_ahead_log = true;
auto writer = std::make_unique<critical_writer>(std::move(file), cfg);

// Option 2: hybrid_writer - best of both worlds
// Async for normal logs, sync for critical
auto file = std::make_unique<file_writer>("app.log");
auto writer = std::make_unique<hybrid_writer>(std::move(file));
```

**Best for:** Financial systems, safety-critical applications, audit logging

---

### 8. Custom Pipeline (Formatter + Sink)

**Use:** `composite_writer`

```cpp
auto writer = make_composite_writer(
    std::make_unique<json_formatter>(),
    std::make_unique<file_sink>("app.json")
);
```

**Best for:** Custom output formats, structured logging

---

## Decorator Stacking Patterns

Writers can be combined by stacking decorators:

### Pattern 1: Async + Encrypted

```cpp
auto file = std::make_unique<file_writer>("secure.log");
auto encrypted = std::make_unique<encrypted_writer>(
    std::move(file), encryption_config{...}
);
auto async = std::make_unique<async_writer>(std::move(encrypted));
async->start();
```

### Pattern 2: Critical + Async + Rotating

```cpp
auto rotating = std::make_unique<rotating_file_writer>("app.log", 10_MB, 5);
auto async = std::make_unique<async_writer>(std::move(rotating));
async->start();
auto critical = std::make_unique<critical_writer>(std::move(async));
```

### Pattern 3: Batch + Network

```cpp
auto network = std::make_unique<network_writer>("log-server", 5514);
auto batch = std::make_unique<batch_writer>(
    std::move(network),
    batch_writer::config{.max_batch_size = 50}
);
```

## Performance Comparison

| Writer | Throughput | Latency | Data Safety |
|--------|------------|---------|-------------|
| `console_writer` | Low | Immediate | High |
| `file_writer` | Medium | Low | High |
| `rotating_file_writer` | Medium | Low | High |
| `async_writer` | High | Variable | Medium |
| `batch_writer` | Very High | Variable | Medium |
| `network_writer` | Medium | Variable | Medium |
| `critical_writer` | Low | Immediate | Very High |
| `hybrid_writer` | High (normal), Low (critical) | Variable | High |

## Decision Matrix

| Requirement | Recommended Writer(s) |
|-------------|----------------------|
| Simple debugging | `console_writer` |
| Basic file logging | `file_writer` |
| Log rotation | `rotating_file_writer` |
| High throughput | `async_writer`, `batch_writer` |
| Network destination | `network_writer`, `otlp_writer` |
| Encryption | `encrypted_writer` |
| Critical data | `critical_writer`, `hybrid_writer` |
| Custom format | `composite_writer` |
| OpenTelemetry | `otlp_writer` |

## Related Documentation

- [Writer Hierarchy](WRITER_HIERARCHY.md) - Detailed hierarchy and category system
- [Custom Writers Guide](CUSTOM_WRITERS.md) - Creating custom writers
- [Async Writers Guide](ASYNC_WRITERS.md) - Async patterns and performance
- [Performance Guide](../guides/PERFORMANCE.md) - Performance optimization

---

*Last Updated: 2025-01-11*
*Version: 1.4.0* (Phase 2.0 - Writer Hierarchy Clarification)
