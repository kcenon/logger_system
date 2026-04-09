---
doc_id: "LOG-GUID-002a"
doc_title: "Log Server Guide"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "GUID"
---

# Log Server Guide

> **Version**: 1.0.0
> **Last Updated**: 2025-02-09
> **Status**: Production Ready
> **Split from**: [LOG_SERVER_AND_CRASH_SAFETY.md](LOG_SERVER_AND_CRASH_SAFETY.md)

## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Configuration](#configuration)
- [API Reference](#api-reference)
- [Deployment Patterns](#deployment-patterns)
- [Integration Examples](#integration-examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)
- [Related Documentation](#related-documentation)

---

## Overview

logger_system provides a centralized log collection server for production-grade logging infrastructure.

| Feature | Purpose | Key Benefit |
|---------|---------|-------------|
| **Log Server** | Centralized log collection server | Aggregates logs from distributed applications |

**Distributed Logging Topology**:

```
┌──────────────────────────────────────────────────────────┐
│                    Production Environment                 │
│                                                           │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐     │
│  │ App Node 1  │  │ App Node 2  │  │ App Node N  │     │
│  │             │  │             │  │             │     │
│  │ ┌─────────┐ │  │ ┌─────────┐ │  │ ┌─────────┐ │     │
│  │ │crash_safe│ │  │ │crash_safe│ │  │ │crash_safe│ │     │
│  │ │_logger   │ │  │ │_logger   │ │  │ │_logger   │ │     │
│  │ └────┬────┘ │  │ └────┬────┘ │  │ └────┬────┘ │     │
│  │      │      │  │      │      │  │      │      │     │
│  │   network   │  │   network   │  │   network   │     │
│  │   _writer   │  │   _writer   │  │   _writer   │     │
│  └──────┼──────┘  └──────┼──────┘  └──────┼──────┘     │
│         │                │                │             │
│         └────────────────┼────────────────┘             │
│                          ▼                              │
│                   ┌─────────────┐                       │
│                   │ log_server  │                       │
│                   │  (Port 9999)│                       │
│                   └──────┬──────┘                       │
│                          │                              │
│                   rotating_file_writer                  │
│                          │                              │
│                          ▼                              │
│                 /var/log/aggregated/                    │
└──────────────────────────────────────────────────────────┘
```

**Key Benefits**:
- **Centralized Management**: Single collection point for distributed application logs
- **Production Ready**: Thread-safe, performant, and battle-tested

---

## Architecture

The log server acts as a centralized collection point in a distributed logging topology.

### Components

```cpp
┌─────────────────────────────────────────────────────────┐
│                      log_server                          │
│                                                          │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  │
│  │   Worker 1   │  │   Worker 2   │  │   Worker N   │  │
│  │   Thread     │  │   Thread     │  │   Thread     │  │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘  │
│         │                 │                 │           │
│         └─────────────────┼─────────────────┘           │
│                           │                             │
│                    ┌──────▼──────┐                      │
│                    │  Connection  │                      │
│                    │   Handler    │                      │
│                    └──────┬───────┘                      │
│                           │                             │
│                    ┌──────▼──────┐                      │
│                    │   Storage    │                      │
│                    │  (Writers)   │                      │
│                    └──────────────┘                      │
└─────────────────────────────────────────────────────────┘
```

**Header File**: `include/kcenon/logger/server/log_server.h`

### Role in Distributed Logging

| Component | Role | Interaction |
|-----------|------|-------------|
| **Client Applications** | Generate logs | Send to server via `network_writer` |
| **Log Server** | Aggregate logs | Receive from all clients |
| **Storage Backend** | Persist logs | Files, databases, or external systems (ELK, Loki) |

### Protocol

The log server communicates with clients using the `network_writer` protocol:

1. **Connection**: Client establishes TCP connection to server
2. **Authentication**: Optional credential verification
3. **Message Transfer**: Client sends serialized log messages
4. **Compression**: Optional gzip compression for large messages
5. **Encryption**: Optional TLS encryption for secure transport

---

## Configuration

### server_config Structure

```cpp
struct server_config {
    std::string host = "localhost";      // Bind address
    uint16_t port = 9999;                // Listen port
    size_t max_connections = 100;        // Maximum concurrent clients
    size_t buffer_size = 8192;           // Per-connection buffer (bytes)
    bool enable_compression = false;     // Enable gzip compression
    bool enable_encryption = false;      // Enable TLS/SSL
};
```

### Configuration Examples

<details>
<summary><b>Basic Configuration</b></summary>

```cpp
#include <kcenon/logger/server/log_server.h>

// Default configuration
server_config config;
config.host = "0.0.0.0";  // Listen on all interfaces
config.port = 9999;
config.max_connections = 50;

auto server = log_server_factory::create_basic(config);
server->start();

// Server is now accepting connections on 0.0.0.0:9999
```

**Use Case**: Development environment with basic logging needs.

</details>

<details>
<summary><b>High-Volume Configuration</b></summary>

```cpp
// High-throughput configuration
server_config config;
config.host = "0.0.0.0";
config.port = 9999;
config.max_connections = 500;       // Support 500 concurrent clients
config.buffer_size = 65536;         // 64KB buffers for high throughput
config.enable_compression = true;   // Reduce network bandwidth

auto server = log_server_factory::create_basic(config);
server->start();
```

**Use Case**: Production environment with hundreds of application instances.

**Performance**: Handles 10,000+ messages/second with compression enabled.

</details>

<details>
<summary><b>Secure Configuration</b></summary>

```cpp
// Secure production configuration
server_config config;
config.host = "10.0.1.100";         // Internal network only
config.port = 9999;
config.max_connections = 200;
config.enable_encryption = true;    // TLS encryption

auto server = log_server_factory::create_basic(config);
server->start();

// Note: TLS certificate/key configuration would be added via
// additional API methods (implementation-specific)
```

**Use Case**: Production environment with security compliance requirements (PCI-DSS, HIPAA).

**Security**: All log data encrypted in transit.

</details>

---

## API Reference

### log_server Class

```cpp
namespace kcenon::logger::server {

class log_server {
public:
    explicit log_server(const server_config& config = {});
    ~log_server();

    // Lifecycle management
    bool start();                           // Start server and worker threads
    void stop();                            // Gracefully stop server
    bool is_running() const;                // Check server status

    // Configuration access
    const server_config& get_config() const;

private:
    void worker_loop();                     // Internal worker thread implementation
};

} // namespace kcenon::logger::server
```

### log_server_factory Class

```cpp
class log_server_factory {
public:
    // Create server with custom configuration
    static std::unique_ptr<log_server> create_basic(
        const server_config& config = {}
    );

    // Create server with default configuration
    static std::unique_ptr<log_server> create_default();
};
```

### Method Details

#### start()

**Description**: Starts the log server and spawns worker threads.

**Returns**: `bool` -- `true` if started successfully, `false` if already running.

**Thread Count**: Spawns `std::thread::hardware_concurrency()` worker threads (typically equal to CPU core count).

**Example**:
```cpp
auto server = log_server_factory::create_default();
if (server->start()) {
    std::cout << "Server started successfully\n";
} else {
    std::cerr << "Server already running\n";
}
```

#### stop()

**Description**: Gracefully stops the server and joins all worker threads.

**Blocks**: Until all worker threads complete (may take up to 100ms per worker).

**Example**:
```cpp
server->stop();
// All connections closed, workers stopped
```

#### is_running()

**Description**: Checks if the server is currently running.

**Returns**: `bool` -- `true` if running, `false` otherwise.

**Thread-Safe**: Yes (uses atomic operations).

**Example**:
```cpp
if (server->is_running()) {
    std::cout << "Server status: RUNNING\n";
}
```

---

## Deployment Patterns

### Pattern 1: Single Server with Multiple Clients

```
┌─────────────┐  ┌─────────────┐  ┌─────────────┐
│ Web Server  │  │ API Server  │  │ Worker Node │
│ (Node 1)    │  │ (Node 2)    │  │ (Node 3)    │
└──────┬──────┘  └──────┬──────┘  └──────┬──────┘
       │                │                │
       │     network_writer (TCP)       │
       │                │                │
       └────────────────┼────────────────┘
                        ▼
                 ┌─────────────┐
                 │ log_server  │
                 │   :9999     │
                 └──────┬──────┘
                        │
                 rotating_file_writer
                        │
                        ▼
              /var/log/aggregated/app.log
```

**Configuration**:

Server:
```cpp
server_config config;
config.host = "10.0.1.100";
config.port = 9999;
config.max_connections = 100;

auto server = log_server_factory::create_basic(config);
server->start();
```

Clients:
```cpp
#include <kcenon/logger/writers/network_writer.h>

auto logger = logger_builder::create()
    .with_writer(std::make_unique<network_writer>(
        "10.0.1.100",  // Server IP
        9999           // Server port
    ))
    .build();
```

**Benefits**:
- Centralized log storage
- Simplified log analysis
- Reduced disk I/O on application nodes

### Pattern 2: Log Aggregation for ELK Stack

```
┌─────────────────────────────────────────────────────────┐
│                Application Fleet                         │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐              │
│  │ App 1    │  │ App 2    │  │ App N    │              │
│  └────┬─────┘  └────┬─────┘  └────┬─────┘              │
│       └─────────────┼─────────────┘                     │
│                     ▼                                    │
│              ┌─────────────┐                             │
│              │ log_server  │                             │
│              └──────┬──────┘                             │
│                     │                                    │
│              ┌──────▼──────┐                             │
│              │ JSON Writer │                             │
│              │ (Formatted) │                             │
│              └──────┬──────┘                             │
│                     ▼                                    │
│            /var/log/json/app.log                         │
└─────────────────────┼───────────────────────────────────┘
                      │
                  Filebeat
                      │
                      ▼
              ┌─────────────┐
              │ Elasticsearch│
              │   Logstash   │
              │   Kibana     │
              └─────────────┘
```

**Server Configuration**:
```cpp
#include <kcenon/logger/server/log_server.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/formatters/json_formatter.h>

// Log server writes JSON logs for Filebeat ingestion
server_config config;
config.host = "0.0.0.0";
config.port = 9999;
config.max_connections = 200;
config.enable_compression = true;

auto server = log_server_factory::create_basic(config);
server->start();

// Note: Actual storage writer would be configured separately
// via dependency injection or builder pattern
```

**Benefits**:
- Seamless integration with ELK stack
- Centralized search and visualization
- Historical log analysis

### Pattern 3: Multi-Region Deployment

```
┌─────────────────────────────────────────────────────────┐
│                   Region: us-east-1                      │
│  ┌──────────┐  ┌──────────┐                             │
│  │ App 1    │  │ App 2    │                             │
│  └────┬─────┘  └────┬─────┘                             │
│       └─────────────┼──────────┐                        │
│                     ▼          │                        │
│              ┌─────────────┐   │                        │
│              │log_server 1 │   │                        │
│              └──────┬──────┘   │                        │
└─────────────────────┼──────────┘                        │
                      │
┌─────────────────────┼──────────┐
│                     │          │   Region: eu-west-1    │
│                     │    ┌──────────┐  ┌──────────┐    │
│                     │    │ App 3    │  │ App 4    │    │
│                     │    └────┬─────┘  └────┬─────┘    │
│                     │         └─────────────┼──────┐   │
│                     │                       ▼      │   │
│                     │                ┌─────────────┐   │
│                     │                │log_server 2 │   │
│                     │                └──────┬──────┘   │
└─────────────────────┼───────────────────────┼──────────┘
                      │                       │
                      └───────────┬───────────┘
                                  ▼
                         ┌─────────────────┐
                         │Central Log Store│
                         │  (S3, GCS, etc) │
                         └─────────────────┘
```

**Configuration**:

Region 1 Server:
```cpp
server_config config_us;
config_us.host = "10.1.1.100";
config_us.port = 9999;
config_us.max_connections = 500;
```

Region 2 Server:
```cpp
server_config config_eu;
config_eu.host = "10.2.1.100";
config_eu.port = 9999;
config_eu.max_connections = 500;
```

**Benefits**:
- Low-latency logging within each region
- Geographic redundancy
- Centralized long-term storage

---

## Integration Examples

### Example 1: Basic Server Setup

```cpp
#include <kcenon/logger/server/log_server.h>
#include <iostream>

int main() {
    // Create server with default configuration
    auto server = log_server_factory::create_default();

    // Start server
    if (!server->start()) {
        std::cerr << "Failed to start log server\n";
        return 1;
    }

    std::cout << "Log server running on "
              << server->get_config().host << ":"
              << server->get_config().port << "\n";

    // Keep server running until user interruption
    std::cout << "Press Enter to stop server...\n";
    std::cin.get();

    // Graceful shutdown
    server->stop();
    std::cout << "Server stopped\n";

    return 0;
}
```

### Example 2: Programmatic Control

```cpp
#include <kcenon/logger/server/log_server.h>
#include <thread>
#include <chrono>

class LogServerManager {
private:
    std::unique_ptr<log_server> server_;

public:
    bool initialize() {
        server_config config;
        config.host = "0.0.0.0";
        config.port = 9999;
        config.max_connections = 200;
        config.enable_compression = true;

        server_ = log_server_factory::create_basic(config);
        return server_->start();
    }

    void run_health_check_loop() {
        while (server_->is_running()) {
            std::cout << "Server status: HEALTHY\n";
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    }

    void shutdown() {
        if (server_ && server_->is_running()) {
            server_->stop();
        }
    }

    ~LogServerManager() {
        shutdown();
    }
};

int main() {
    LogServerManager manager;

    if (!manager.initialize()) {
        std::cerr << "Failed to initialize log server\n";
        return 1;
    }

    manager.run_health_check_loop();
    return 0;
}
```

### Example 3: Docker Deployment

**Dockerfile**:
```dockerfile
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

# Copy log server binary
COPY log_server /usr/local/bin/
COPY libkcenon_logger.so /usr/local/lib/

# Create log directory
RUN mkdir -p /var/log/aggregated

# Expose port
EXPOSE 9999

# Run server
CMD ["/usr/local/bin/log_server", "--host=0.0.0.0", "--port=9999"]
```

**Docker Compose**:
```yaml
version: '3.8'
services:
  log-server:
    build: .
    ports:
      - "9999:9999"
    volumes:
      - ./logs:/var/log/aggregated
    restart: unless-stopped
    environment:
      - MAX_CONNECTIONS=500
      - ENABLE_COMPRESSION=true
```

---

## Best Practices

### 1. Capacity Planning

**Rule of Thumb**: 1 server handles ~100-500 clients depending on log volume.

**Calculation**:
```
Server Capacity = (Network Bandwidth / Avg Message Size) x 0.7

Example:
- Network: 1 Gbps = 125 MB/sec
- Avg message: 250 bytes
- Capacity: (125 MB/sec / 250 bytes) x 0.7 = 350,000 msg/sec
- If each client sends 1000 msg/sec -> 350 clients max
```

**Recommendation**: Monitor CPU and network utilization; scale horizontally if >70% utilized.

### 2. Network Security

**Firewall Rules**:
```bash
# Allow only internal network to log server port
sudo iptables -A INPUT -p tcp --dport 9999 -s 10.0.0.0/8 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 9999 -j DROP
```

**TLS Configuration** (if `enable_encryption = true`):
```cpp
// Future API (not yet implemented)
server_config config;
config.enable_encryption = true;
// config.tls_cert_path = "/etc/certs/log-server.crt";
// config.tls_key_path = "/etc/certs/log-server.key";
```

### 3. Storage Management

**Rotate Logs Regularly**:
```cpp
// Server-side storage with rotation
auto storage_writer = std::make_unique<rotating_file_writer>(
    "/var/log/aggregated/app.log",
    100 * 1024 * 1024,  // 100MB per file
    20                  // Keep 20 files (2GB total)
);
```

**External Archival**:
```bash
# Cron job to archive old logs to S3
0 2 * * * /usr/local/bin/archive-logs.sh /var/log/aggregated s3://log-archive
```

### 4. Monitoring

**Metrics to Track**:
- Active connections (`max_connections` limit)
- Messages received per second
- Disk space usage
- Network bandwidth utilization

**Health Check Script**:
```bash
#!/bin/bash
# Check if log server is accepting connections
nc -zv log-server.internal 9999
if [ $? -eq 0 ]; then
    echo "Log server: HEALTHY"
else
    echo "Log server: DOWN"
    # Alert on-call engineer
fi
```

---

## Troubleshooting

### Problem: Server fails to start

**Symptoms**:
```cpp
auto server = log_server_factory::create_default();
if (!server->start()) {
    std::cerr << "Server failed to start\n";
}
```

**Possible Causes**:
1. **Port already in use**
   ```bash
   $ sudo lsof -i :9999
   COMMAND  PID USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
   old_srv  1234 user   3u  IPv4  12345      0t0  TCP *:9999 (LISTEN)
   ```
   **Solution**: Kill existing process or use different port.

2. **Permission denied (port < 1024)**
   ```bash
   $ ./log_server --port=514
   # Error: Permission denied
   ```
   **Solution**: Use port >=1024 or run with `sudo` (not recommended).

3. **Server already running**
   ```cpp
   server->start();  // Returns true
   server->start();  // Returns false (already running)
   ```
   **Solution**: Check `is_running()` before calling `start()`.

### Problem: Clients cannot connect

**Symptoms**: `network_writer` fails to connect to server.

**Debugging**:
```bash
# Check if server is listening
$ netstat -tlnp | grep 9999
tcp  0  0.0.0.0:9999  0.0.0.0:*  LISTEN  1234/log_server

# Test connectivity
$ telnet log-server.internal 9999
Trying 10.0.1.100...
Connected to log-server.internal.
```

**Possible Causes**:
1. **Firewall blocking port**
   ```bash
   $ sudo iptables -L -n | grep 9999
   DROP  tcp  --  0.0.0.0/0  0.0.0.0/0  tcp dpt:9999
   ```
   **Solution**: Add firewall rule to allow port 9999.

2. **Server bound to localhost only**
   ```cpp
   server_config config;
   config.host = "localhost";  // Only accepts local connections
   ```
   **Solution**: Bind to `0.0.0.0` for all interfaces.

3. **DNS resolution failure**
   ```bash
   $ dig log-server.internal
   # No A record found
   ```
   **Solution**: Use IP address or fix DNS.

---

## Related Documentation

- [Crash-Safe Logger Guide](./CRASH_SAFETY_GUIDE.md) -- Crash-safe logger, emergency flush, recovery
- [Configuration Strategies](./CONFIGURATION_STRATEGIES.md) -- Strategy pattern for environment-specific configuration
- [Writer Guide](./WRITER_GUIDE.md) -- Comprehensive guide to 18 writer types and decorator pattern
- [Security Guide](./SECURITY_GUIDE.md) -- Audit logging, sanitization, and secure key storage

### Header Files
- `include/kcenon/logger/server/log_server.h` -- Log server implementation
- `include/kcenon/logger/writers/network_writer.h` -- Client-side network writer

### External Resources
- [ELK Stack Integration](https://www.elastic.co/guide/en/elasticsearch/reference/current/index.html) -- Elasticsearch documentation
- [12-Factor App Logging](https://12factor.net/logs) -- Best practices for distributed logging

---

**Document Information**:
- **Authors**: logger_system documentation team
- **Last Review**: 2025-02-09
- **Version**: 1.0.0
- **Status**: Production Ready
- **Feedback**: Submit issues to [logger_system GitHub](https://github.com/kcenon/logger_system/issues)
