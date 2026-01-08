# OpenTelemetry Integration Guide

> **Version**: 3.0.0+
> **Status**: Stable

This guide covers the OpenTelemetry (OTLP) integration in logger_system, enabling seamless correlation between logs, traces, and metrics for cloud-native observability.

## Table of Contents

- [Overview](#overview)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Configuration](#configuration)
- [Trace Context](#trace-context)
- [OTLP Writer](#otlp-writer)
- [Integration Examples](#integration-examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

## Overview

OpenTelemetry is the CNCF standard for observability, providing unified APIs for traces, metrics, and logs. Logger System's OTLP integration enables:

| Feature | Description |
|---------|-------------|
| **Trace Correlation** | Logs linked to distributed traces via trace_id/span_id |
| **Unified Export** | Single protocol for exporting to observability platforms |
| **Ecosystem Support** | Works with Jaeger, Zipkin, Prometheus, Grafana, Datadog |
| **Context Propagation** | Automatic thread-local context handling |

## Installation

### With vcpkg

```bash
# Install with OTLP support
vcpkg install kcenon-logger-system[otlp]
```

### With CMake

```bash
cmake -B build -DLOGGER_ENABLE_OTLP=ON
cmake --build build
```

### Dependencies

The OTLP feature requires:
- opentelemetry-cpp
- protobuf
- grpc (for gRPC transport)

## Quick Start

### Basic OTLP Export

```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/otlp_writer.h>
#include <kcenon/logger/otlp/otel_context.h>

using namespace kcenon::logger;

int main() {
    // Configure OTLP writer
    otlp_writer::config otlp_cfg{
        .endpoint = "http://otel-collector:4318/v1/logs",
        .protocol = otlp_writer::protocol_type::http,
        .service_name = "my-service",
        .service_version = "1.0.0",
        .resource_attributes = {
            {"environment", "production"},
            {"region", "us-east-1"}
        }
    };

    // Build logger with OTLP writer
    auto result = logger_builder()
        .with_async(true)
        .add_writer("otlp", std::make_unique<otlp_writer>(otlp_cfg))
        .build();

    if (result.is_err()) {
        return -1;
    }

    auto logger = std::move(result.value());

    // Logs are now exported to OpenTelemetry Collector
    logger->log(log_level::info, "Application started");

    return 0;
}
```

### With Trace Correlation

```cpp
#include <kcenon/logger/otlp/otel_context.h>

void handle_request(const Request& req, logger* log) {
    // Extract trace context from incoming request headers
    otlp::otel_context ctx{
        .trace_id = req.headers["traceparent"].substr(3, 32),
        .span_id = req.headers["traceparent"].substr(36, 16),
        .trace_flags = "01"
    };

    // Set context for this thread
    log->set_otel_context(ctx);

    // All logs now include trace_id and span_id
    log->log(log_level::info, "Processing request");
    process_request(req);
    log->log(log_level::info, "Request completed");

    // Clear context when done
    log->clear_otel_context();
}
```

### Using RAII Scope Guard

```cpp
#include <kcenon/logger/otlp/otel_context.h>

void handle_request(const Request& req) {
    otlp::otel_context ctx{
        .trace_id = extract_trace_id(req),
        .span_id = extract_span_id(req)
    };

    // Automatically clears context when scope ends
    otlp::otel_context_scope scope(ctx);

    // All logs in this scope include trace context
    process_request(req);
} // Context automatically cleared here
```

## Configuration

### OTLP Writer Configuration

```cpp
otlp_writer::config cfg{
    // Endpoint URL
    // HTTP: http://host:4318/v1/logs
    // gRPC: host:4317
    .endpoint = "http://localhost:4318/v1/logs",

    // Transport protocol (http or grpc)
    .protocol = otlp_writer::protocol_type::http,

    // Connection timeout
    .timeout = std::chrono::milliseconds{5000},

    // Enable TLS
    .use_tls = false,

    // Service identification (required)
    .service_name = "my-service",
    .service_version = "1.0.0",
    .service_namespace = "production",
    .service_instance_id = "pod-abc123",

    // Custom resource attributes
    .resource_attributes = {
        {"deployment.environment", "production"},
        {"cloud.region", "us-east-1"},
        {"host.name", "server-01"}
    },

    // Batching configuration
    .max_batch_size = 512,           // Max logs per batch
    .flush_interval = std::chrono::milliseconds{5000},  // Max wait time

    // Queue management
    .max_queue_size = 10000,         // Max queued logs (drops if exceeded)

    // Retry configuration
    .max_retries = 3,
    .retry_delay = std::chrono::milliseconds{100},  // Exponential backoff

    // Authentication headers
    .headers = {
        {"Authorization", "Bearer token123"}
    }
};
```

### Protocol Selection

| Protocol | Port | Use Case |
|----------|------|----------|
| HTTP | 4318 | Simpler setup, firewall-friendly |
| gRPC | 4317 | Better performance, streaming support |

```cpp
// HTTP endpoint (default)
cfg.endpoint = "http://collector:4318/v1/logs";
cfg.protocol = otlp_writer::protocol_type::http;

// gRPC endpoint
cfg.endpoint = "collector:4317";
cfg.protocol = otlp_writer::protocol_type::grpc;
```

## Trace Context

### otel_context Structure

```cpp
struct otel_context {
    std::string trace_id;      // 32 hex chars (128 bits)
    std::string span_id;       // 16 hex chars (64 bits)
    std::string trace_flags;   // 2 hex chars ("00" or "01")
    std::string trace_state;   // Vendor-specific data

    bool is_valid() const;     // Check format validity
    bool is_sampled() const;   // Check if trace is sampled
};
```

### Thread-Local Storage

```cpp
// Set context for current thread
otlp::otel_context_storage::set(ctx);

// Get current context
auto ctx_opt = otlp::otel_context_storage::get();
if (ctx_opt) {
    std::cout << "Trace ID: " << ctx_opt->trace_id << "\n";
}

// Check if context exists
if (otlp::otel_context_storage::has_context()) {
    // ...
}

// Clear context
otlp::otel_context_storage::clear();
```

### Context in Log Entries

```cpp
// log_entry automatically captures thread-local context
log_entry entry(log_level::info, "Message");
// entry.otel_ctx is automatically populated from thread-local storage

// Or set explicitly
entry.otel_ctx = otlp::otel_context{
    .trace_id = "...",
    .span_id = "..."
};
```

## OTLP Writer

### Statistics Monitoring

```cpp
otlp_writer writer(cfg);

// Get export statistics
auto stats = writer.get_stats();
std::cout << "Logs exported: " << stats.logs_exported << "\n";
std::cout << "Logs dropped: " << stats.logs_dropped << "\n";
std::cout << "Export successes: " << stats.export_success << "\n";
std::cout << "Export failures: " << stats.export_failures << "\n";
std::cout << "Retries: " << stats.retries << "\n";
```

### Health Checking

```cpp
if (!writer.is_healthy()) {
    // Collector may be unavailable
    // Writer will retry automatically
}
```

### Force Flush

```cpp
// Export all queued logs immediately
writer.force_export();

// Or use flush()
writer.flush();
```

## Integration Examples

### With OpenTelemetry Collector

```yaml
# otel-collector-config.yaml
receivers:
  otlp:
    protocols:
      http:
        endpoint: 0.0.0.0:4318
      grpc:
        endpoint: 0.0.0.0:4317

exporters:
  jaeger:
    endpoint: jaeger:14250
    tls:
      insecure: true
  loki:
    endpoint: http://loki:3100/loki/api/v1/push

service:
  pipelines:
    logs:
      receivers: [otlp]
      exporters: [loki]
    traces:
      receivers: [otlp]
      exporters: [jaeger]
```

### Docker Compose Setup

```yaml
version: '3.8'
services:
  otel-collector:
    image: otel/opentelemetry-collector-contrib:latest
    volumes:
      - ./otel-collector-config.yaml:/etc/otelcol/config.yaml
    ports:
      - "4317:4317"  # gRPC
      - "4318:4318"  # HTTP

  jaeger:
    image: jaegertracing/all-in-one:latest
    ports:
      - "16686:16686"  # UI
      - "14250:14250"  # gRPC

  loki:
    image: grafana/loki:latest
    ports:
      - "3100:3100"

  grafana:
    image: grafana/grafana:latest
    ports:
      - "3000:3000"
```

### W3C Trace Context Parsing

```cpp
// Parse W3C traceparent header
// Format: version-trace_id-span_id-trace_flags
// Example: 00-0af7651916cd43dd8448eb211c80319c-b7ad6b7169203331-01

std::optional<otlp::otel_context> parse_traceparent(const std::string& header) {
    if (header.length() < 55 || header[0] != '0' || header[1] != '0') {
        return std::nullopt;
    }

    return otlp::otel_context{
        .trace_id = header.substr(3, 32),
        .span_id = header.substr(36, 16),
        .trace_flags = header.substr(53, 2)
    };
}
```

## Best Practices

### 1. Always Set Service Name

```cpp
cfg.service_name = "api-gateway";  // Required for proper identification
cfg.service_version = "1.2.3";     // Helps with debugging
```

### 2. Use RAII Scope Guards

```cpp
// Preferred: automatic cleanup
{
    otlp::otel_context_scope scope(ctx);
    // ... process request ...
} // Context cleared automatically

// Avoid: manual cleanup (error-prone)
ctx_storage::set(ctx);
// ... if exception thrown, context leaks ...
ctx_storage::clear();
```

### 3. Configure Appropriate Batch Sizes

```cpp
// High-throughput service
cfg.max_batch_size = 1000;
cfg.flush_interval = std::chrono::seconds{1};

// Low-latency service
cfg.max_batch_size = 100;
cfg.flush_interval = std::chrono::milliseconds{500};
```

### 4. Handle Collector Unavailability

```cpp
// Large queue for temporary outages
cfg.max_queue_size = 50000;
cfg.max_retries = 5;

// Monitor health
if (!writer.is_healthy()) {
    // Alert operations team
}
```

### 5. Include Meaningful Resource Attributes

```cpp
cfg.resource_attributes = {
    {"deployment.environment", "production"},
    {"service.namespace", "payments"},
    {"cloud.provider", "aws"},
    {"cloud.region", "us-east-1"},
    {"k8s.pod.name", get_pod_name()},
    {"k8s.namespace.name", get_namespace()}
};
```

## Troubleshooting

### Logs Not Appearing in Collector

1. **Check endpoint URL**: Ensure correct port (4318 for HTTP, 4317 for gRPC)
2. **Verify network connectivity**: Test with curl or grpcurl
3. **Check collector logs**: Look for parsing or connection errors
4. **Verify TLS settings**: Match use_tls with collector configuration

### High Log Drop Rate

```cpp
// Increase queue size
cfg.max_queue_size = 100000;

// Increase batch size for higher throughput
cfg.max_batch_size = 2000;

// Check stats for bottleneck
auto stats = writer.get_stats();
if (stats.logs_dropped > 0) {
    // Queue overflow - increase size or reduce log volume
}
```

### Connection Timeouts

```cpp
// Increase timeout for slow networks
cfg.timeout = std::chrono::seconds{30};

// Increase retry attempts
cfg.max_retries = 5;
cfg.retry_delay = std::chrono::seconds{1};
```

### Missing Trace Context

1. **Verify context is set before logging**:
   ```cpp
   log->set_otel_context(ctx);
   log->log(level, msg);  // Context must be set first
   ```

2. **Check thread isolation**: Context is thread-local
3. **Use scope guards** for automatic cleanup

## Related Documentation

- [Writer Types](../FEATURES.md#writer-types)
- [Security Guide](SECURITY.md)
- [Integration Guide](INTEGRATION.md)
- [OpenTelemetry Specification](https://opentelemetry.io/docs/specs/)
- [W3C Trace Context](https://www.w3.org/TR/trace-context/)
