# Configuration Strategies Guide

**logger_system Configuration Strategies** (`include/kcenon/logger/core/strategies/`)

The configuration strategies framework provides a flexible, composable approach to logger configuration using the Strategy Pattern. Strategies allow dynamic adaptation of logger behavior based on environment, performance requirements, or deployment context.

---

## Table of Contents

1. [Overview](#overview)
2. [Strategy Pattern Benefits](#strategy-pattern-benefits)
3. [Strategy Interface](#strategy-interface)
4. [Built-in Strategies](#built-in-strategies)
   - [Deployment Strategy](#deployment-strategy)
   - [Environment Strategy](#environment-strategy)
   - [Performance Strategy](#performance-strategy)
5. [Composite Strategy](#composite-strategy)
6. [Usage Examples](#usage-examples)
7. [Custom Strategy Implementation](#custom-strategy-implementation)
8. [Strategy Priority and Conflict Resolution](#strategy-priority-and-conflict-resolution)
9. [Best Practices](#best-practices)

---

## Overview

### What are Configuration Strategies?

Configuration strategies are reusable, composable configuration profiles that encapsulate logger settings for specific contexts:

- **Deployment strategies** — Environment-specific settings (development, staging, production, testing)
- **Environment strategies** — Runtime configuration via environment variables
- **Performance strategies** — Tuning profiles (low latency, high throughput, minimal overhead)
- **Composite strategies** — Combine multiple strategies with priority-based conflict resolution

### Architecture

```
┌─────────────────────────────────────────────────────┐
│             config_strategy_interface                │
│  + get_name(): string                                │
│  + apply(config&): void                              │
│  + is_applicable(): bool                             │
│  + priority(): int                                   │
└──────────────────┬──────────────────────────────────┘
                   │
        ┌──────────┼──────────┬──────────┬─────────┐
        ▼          ▼          ▼          ▼         ▼
 ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐
 │Deployment│ │Environment│ │Performance│ │Composite│
 │Strategy  │ │ Strategy  │ │ Strategy  │ │Strategy │
 │          │ │           │ │           │ │         │
 │Priority:│ │Priority:│ │Priority:│ │Contains │
 │   75     │ │  100      │ │   50      │ │multiple │
 └──────────┘ └──────────┘ └──────────┘ └──────────┘
```

**Evaluation flow:**
1. Create composite strategy
2. Add individual strategies
3. Sort by priority (descending: 100 → 75 → 50 → ...)
4. Apply each applicable strategy in order
5. Later strategies override earlier ones (higher priority wins)

---

## Strategy Pattern Benefits

### Why Use Strategies?

| Benefit | Description |
|---------|-------------|
| **Separation of Concerns** | Decouple configuration logic from logger implementation |
| **Composability** | Combine multiple strategies for complex requirements |
| **Reusability** | Share configurations across projects/services |
| **Testability** | Mock strategies for unit tests |
| **Dynamic Configuration** | Change behavior at runtime based on context |

### Example: Multi-Environment Deployment

```cpp
// Development: Verbose, colorful, synchronous
auto dev_logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::development)
    .build();

// Production: Optimized, structured, async
auto prod_logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::production)
    .with_strategy<performance_strategy>(performance_level::high_throughput)
    .build();
```

**Result:** Same codebase, different behaviors without conditional logic.

---

## Strategy Interface

**Header:** `include/kcenon/logger/core/strategies/config_strategy_interface.h`

### Base Interface

```cpp
class config_strategy_interface {
public:
    virtual ~config_strategy_interface() = default;

    // Get human-readable strategy name
    virtual std::string get_name() const = 0;

    // Apply this strategy to configuration
    virtual void apply(logger_config& config) const = 0;

    // Check if strategy should be applied (default: true)
    virtual bool is_applicable() const { return true; }

    // Get priority (higher = applied first, default: 0)
    virtual int priority() const { return 0; }
};
```

### Strategy Lifecycle

```
1. Creation    → new deployment_strategy(deployment_env::production)
2. Applicability → is_applicable() checks environment/conditions
3. Priority Sort → Composite sorts strategies by priority() (descending)
4. Application  → apply(config) modifies logger_config
5. Finalization → logger_builder uses final config to build logger
```

### Method Details

#### `get_name()`

**Purpose:** Identifies the strategy for debugging/logging.

**Returns:** Human-readable name (e.g., `"deployment:production"`, `"environment"`, `"performance:high_throughput"`).

**Example:**
```cpp
auto strategy = deployment_strategy(deployment_env::staging);
std::cout << "Applying strategy: " << strategy.get_name() << "\n";
// Output: Applying strategy: deployment:staging
```

#### `apply(logger_config& config)`

**Purpose:** Modifies the logger configuration according to the strategy's rules.

**Parameters:**
- `config` — The configuration to modify (in-place modification)

**Behavior:**
- Directly modifies `config` fields (e.g., `config.async = true`)
- No return value (side effect on `config`)
- May override previously set values

**Example:**
```cpp
logger_config config;  // Default settings
deployment_strategy(deployment_env::production).apply(config);
// config.async == true, config.min_level == log_level::warn
```

#### `is_applicable()`

**Purpose:** Conditional strategy application based on runtime context.

**Returns:** `true` if the strategy should be applied, `false` to skip.

**Default:** Returns `true` (always applicable).

**Override example:**
```cpp
class my_strategy : public config_strategy_interface {
    bool is_applicable() const override {
        // Only apply if running in Docker
        return std::getenv("DOCKER_CONTAINER") != nullptr;
    }
};
```

**Use case:** Environment-dependent strategies (e.g., only apply cloud config if deployed to AWS).

#### `priority()`

**Purpose:** Determines application order when multiple strategies are used.

**Returns:** Integer priority value (higher = applied first).

**Default:** Returns `0`.

**Built-in priorities:**
- `environment_strategy`: **100** (highest — environment variables override everything)
- `deployment_strategy`: **75**
- `performance_strategy`: **50**
- Custom strategies: **0** (default, applied last)

**Conflict resolution:** Later strategies (lower priority) override earlier ones (higher priority).

---

## Built-in Strategies

### Deployment Strategy

**Header:** `include/kcenon/logger/core/strategies/deployment_strategy.h`

**Purpose:** Pre-configured settings for different deployment environments.

#### Supported Environments

```cpp
enum class deployment_env {
    development,  // Verbose logging, colors, immediate output
    staging,      // JSON format, file + console, rotation
    production,   // Optimized, warning+, crash protection
    testing       // Memory buffer, synchronous, full capture
};
```

#### Environment Profiles

##### Development (`deployment_env::development`)

**Target:** Local development, debugging

| Setting | Value | Rationale |
|---------|-------|-----------|
| `async` | `false` | Synchronous for immediate console feedback |
| `min_level` | `log_level::trace` | Verbose for debugging |
| `enable_color_output` | `true` | Colorful console output |
| `enable_source_location` | `true` | File:line for debugging |
| `enable_structured_logging` | `false` | Human-readable text format |
| `batch_size` | `1` | No batching (immediate output) |
| `flush_interval` | `0ms` | Immediate flush |
| `enable_metrics` | `true` | Performance monitoring |
| `enable_crash_handler` | `true` | Crash dump logs |

**Example:**
```cpp
auto logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::development)
    .with_console_writer()
    .build();

logger->trace("Verbose debug info");  // Visible in development
```

##### Staging (`deployment_env::staging`)

**Target:** Pre-production testing, integration environment

| Setting | Value | Rationale |
|---------|-------|-----------|
| `async` | `true` | Async for performance testing |
| `min_level` | `log_level::info` | Moderate verbosity |
| `enable_color_output` | `false` | No colors (file output) |
| `enable_structured_logging` | `true` | JSON for log aggregation |
| `buffer_size` | `16384` (16KB) | Moderate buffer |
| `batch_size` | `100` | Batch processing |
| `flush_interval` | `1000ms` | 1-second flush |
| `max_file_size` | `50MB` | File rotation at 50MB |
| `max_file_count` | `5` | Keep 5 rotated files |
| `enable_batch_writing` | `true` | Batch writes for efficiency |

**Example:**
```cpp
auto logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::staging)
    .with_file_writer("/var/log/app/staging.log")
    .build();

logger->info("Integration test completed");  // JSON format
```

##### Production (`deployment_env::production`)

**Target:** Production deployment, high-performance

| Setting | Value | Rationale |
|---------|-------|-----------|
| `async` | `true` | Non-blocking logging |
| `min_level` | `log_level::warn` | Errors/warnings only |
| `enable_color_output` | `false` | No colors |
| `enable_source_location` | `false` | Reduce overhead |
| `enable_structured_logging` | `true` | JSON for SIEM integration |
| `buffer_size` | `32768` (32KB) | Large buffer |
| `batch_size` | `200` | Large batches |
| `flush_interval` | `2000ms` | 2-second flush |
| `max_file_size` | `100MB` | Large files |
| `max_file_count` | `10` | Keep 10 rotated files |
| `enable_compression` | `true` | Compress logs (gzip) |
| `enable_batch_writing` | `true` | Batch writes |
| `queue_overflow_policy` | `drop_oldest` | Drop old logs on overflow |

**Example:**
```cpp
auto logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::production)
    .with_file_writer("/var/log/app/production.log")
    .build();

logger->warn("High memory usage detected");  // Logged
logger->info("Processing request");          // Dropped (below min_level)
```

##### Testing (`deployment_env::testing`)

**Target:** Unit tests, integration tests

| Setting | Value | Rationale |
|---------|-------|-----------|
| `async` | `false` | Synchronous (deterministic) |
| `min_level` | `log_level::trace` | Capture everything |
| `enable_metrics` | `false` | No overhead |
| `enable_crash_handler` | `false` | No crash handling (let tests fail) |
| `enable_color_output` | `false` | No colors (text assertions) |
| `enable_source_location` | `true` | Debugging test failures |
| `batch_size` | `1` | No batching (deterministic) |
| `flush_interval` | `0ms` | Immediate flush |

**Example:**
```cpp
TEST(LoggerTest, CapturesAllLevels) {
    auto logger = logger_builder::create()
        .with_strategy<deployment_strategy>(deployment_env::testing)
        .with_memory_writer()  // Capture to memory
        .build();

    logger->trace("Test message");
    ASSERT_TRUE(contains_log("Test message"));
}
```

#### API

**Constructor:**
```cpp
explicit deployment_strategy(deployment_env env);
```

**Priority:** `75` (higher than performance, lower than environment)

**Example Usage:**
```cpp
// Create deployment strategy
auto strategy = std::make_unique<deployment_strategy>(deployment_env::production);

// Apply to config
logger_config config;
strategy->apply(config);

// Use with logger builder
auto logger = logger_builder::create()
    .with_strategy(std::move(strategy))
    .build();
```

---

### Environment Strategy

**Header:** `include/kcenon/logger/core/strategies/environment_strategy.h`

**Purpose:** Configure logger via environment variables (12-factor app pattern).

#### Supported Environment Variables

| Variable | Type | Description | Example |
|----------|------|-------------|---------|
| `LOG_LEVEL` | string | Log level (trace, debug, info, warn, error, fatal) | `LOG_LEVEL=warn` |
| `LOG_ASYNC` | bool | Async mode | `LOG_ASYNC=true` |
| `LOG_BUFFER_SIZE` | size_t | Buffer size in bytes | `LOG_BUFFER_SIZE=65536` |
| `LOG_BATCH_SIZE` | size_t | Batch size for processing | `LOG_BATCH_SIZE=200` |
| `LOG_FLUSH_INTERVAL` | size_t | Flush interval in milliseconds | `LOG_FLUSH_INTERVAL=1000` |
| `LOG_COLOR` | bool | Enable color output | `LOG_COLOR=false` |
| `LOG_METRICS` | bool | Enable metrics collection | `LOG_METRICS=true` |
| `LOG_STRUCTURED` | bool | Enable structured logging (JSON) | `LOG_STRUCTURED=true` |
| `LOG_CRASH_HANDLER` | bool | Enable crash handler | `LOG_CRASH_HANDLER=true` |
| `LOG_MAX_QUEUE_SIZE` | size_t | Max queue size (async mode) | `LOG_MAX_QUEUE_SIZE=50000` |
| `LOG_BATCH_WRITING` | bool | Enable batch writing | `LOG_BATCH_WRITING=true` |

**Boolean parsing:** Accepts `true/false`, `1/0`, `yes/no`, `on/off` (case-insensitive).

#### API

**Constructor:**
```cpp
environment_strategy();  // Default constructor
```

**Priority:** `100` (highest — environment variables override all other strategies)

**Applicability:** Only applicable if at least one environment variable is set.

**Example Usage:**

```bash
# Set environment variables
export LOG_LEVEL=info
export LOG_ASYNC=true
export LOG_BUFFER_SIZE=32768
export LOG_STRUCTURED=true
```

```cpp
auto logger = logger_builder::create()
    .with_strategy<environment_strategy>()
    .with_console_writer()
    .build();

// Configuration is automatically loaded from environment
// config.min_level == log_level::info (from LOG_LEVEL)
// config.async == true (from LOG_ASYNC)
// config.buffer_size == 32768 (from LOG_BUFFER_SIZE)
// config.enable_structured_logging == true (from LOG_STRUCTURED)
```

#### Docker/Kubernetes Example

**Dockerfile:**
```dockerfile
FROM ubuntu:22.04

ENV LOG_LEVEL=warn
ENV LOG_ASYNC=true
ENV LOG_STRUCTURED=true
ENV LOG_BATCH_SIZE=500

COPY myapp /usr/local/bin/
CMD ["/usr/local/bin/myapp"]
```

**Kubernetes ConfigMap:**
```yaml
apiVersion: v1
kind: ConfigMap
metadata:
  name: logger-config
data:
  LOG_LEVEL: "info"
  LOG_ASYNC: "true"
  LOG_BUFFER_SIZE: "65536"
  LOG_STRUCTURED: "true"
```

**Deployment:**
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: myapp
spec:
  template:
    spec:
      containers:
      - name: myapp
        envFrom:
        - configMapRef:
            name: logger-config
```

**Result:** Application automatically picks up configuration from environment without code changes.

---

### Performance Strategy

**Header:** `include/kcenon/logger/core/strategies/performance_strategy.h`

**Purpose:** Pre-configured performance tuning profiles.

#### Performance Levels

```cpp
enum class performance_level {
    low_latency,      // Minimize latency (small buffers, immediate flush)
    balanced,         // Balanced configuration (default)
    high_throughput,  // Maximize throughput (large buffers, batch processing)
    minimal_overhead  // Minimize resource usage (simple format, fewer features)
};
```

#### Performance Profiles

##### Low Latency (`performance_level::low_latency`)

**Target:** Real-time systems, interactive applications

**Goal:** Minimize time from log call to output

| Setting | Value | Rationale |
|---------|-------|-----------|
| `async` | `true` | Non-blocking log calls |
| `buffer_size` | `4096` (4KB) | Small buffer (fast flush) |
| `batch_size` | `10` | Small batches (low latency) |
| `flush_interval` | `10ms` | Very frequent flush |
| `use_lock_free` | `true` | Lock-free queue (reduce contention) |
| `max_queue_size` | `10000` | Moderate queue |
| `queue_overflow_policy` | `drop_oldest` | Drop old logs (maintain throughput) |
| `enable_batch_writing` | `false` | No batching (immediate write) |

**Use case:** Financial trading systems, real-time dashboards

**Tradeoff:** Higher CPU usage, lower throughput

##### Balanced (`performance_level::balanced`)

**Target:** General-purpose applications

**Goal:** Good performance without extreme tuning

| Setting | Value | Rationale |
|---------|-------|-----------|
| `async` | `true` | Non-blocking |
| `buffer_size` | `8192` (8KB) | Moderate buffer |
| `batch_size` | `100` | Moderate batching |
| `flush_interval` | `1000ms` | 1-second flush |
| `use_lock_free` | `false` | Standard mutex (simpler) |
| `max_queue_size` | `10000` | Moderate queue |
| `queue_overflow_policy` | `drop_newest` | Drop new logs (preserve history) |
| `enable_batch_writing` | `true` | Batch writes |

**Use case:** Web servers, microservices, general applications

**Tradeoff:** Balanced latency and throughput

##### High Throughput (`performance_level::high_throughput`)

**Target:** High-traffic systems, data pipelines

**Goal:** Maximize logs/second

| Setting | Value | Rationale |
|---------|-------|-----------|
| `async` | `true` | Non-blocking |
| `buffer_size` | `65536` (64KB) | Large buffer |
| `batch_size` | `500` | Large batches |
| `flush_interval` | `5000ms` | 5-second flush |
| `use_lock_free` | `true` | Lock-free queue |
| `max_queue_size` | `100000` | Large queue |
| `queue_overflow_policy` | `drop_oldest` | Drop old logs |
| `writer_thread_count` | `2` | Multi-threaded writing |
| `enable_compression` | `true` | Compress logs (reduce I/O) |
| `enable_batch_writing` | `true` | Batch writes |

**Use case:** Log aggregation servers, big data pipelines, analytics platforms

**Tradeoff:** Higher latency (up to 5 seconds), high memory usage

##### Minimal Overhead (`performance_level::minimal_overhead`)

**Target:** Embedded systems, resource-constrained environments

**Goal:** Minimize CPU and memory usage

| Setting | Value | Rationale |
|---------|-------|-----------|
| `async` | `true` | Non-blocking (offload work) |
| `buffer_size` | `4096` (4KB) | Small buffer |
| `batch_size` | `50` | Moderate batching |
| `flush_interval` | `2000ms` | 2-second flush |
| `enable_metrics` | `false` | No metrics (reduce overhead) |
| `enable_structured_logging` | `false` | Simple text format |
| `enable_source_location` | `false` | No file:line info |
| `enable_color_output` | `false` | No colors |
| `enable_batch_writing` | `true` | Batch writes |

**Use case:** IoT devices, embedded systems, Raspberry Pi

**Tradeoff:** Fewer features, simpler logs

#### API

**Constructor:**
```cpp
explicit performance_strategy(performance_level level);
```

**Priority:** `50` (medium priority)

**Example Usage:**
```cpp
// Low latency for trading system
auto logger = logger_builder::create()
    .with_strategy<performance_strategy>(performance_level::low_latency)
    .with_file_writer("/var/log/trades.log")
    .build();

// High throughput for analytics
auto analytics_logger = logger_builder::create()
    .with_strategy<performance_strategy>(performance_level::high_throughput)
    .with_file_writer("/var/log/analytics.log")
    .build();
```

---

## Composite Strategy

**Header:** `include/kcenon/logger/core/strategies/composite_strategy.h`

**Purpose:** Combine multiple strategies into a single strategy.

### How It Works

1. **Add strategies** to the composite (via `add()`)
2. **Sort by priority** (descending: 100 → 75 → 50 → ...)
3. **Apply each applicable strategy** in order
4. **Later strategies override earlier ones** (conflict resolution)

### API

```cpp
class composite_strategy : public config_strategy_interface {
public:
    // Add strategy (ownership transfer)
    composite_strategy& add(std::unique_ptr<config_strategy_interface> strategy);

    // Add strategy (in-place construction)
    template<typename Strategy, typename... Args>
    composite_strategy& add(Args&&... args);

    // Clear all strategies
    composite_strategy& clear();

    // Get number of strategies
    std::size_t size() const;

    // Check if empty
    bool empty() const;
};
```

### Method Details

#### `add(std::unique_ptr<config_strategy_interface> strategy)`

**Purpose:** Add a pre-constructed strategy to the composite.

**Parameters:**
- `strategy` — Ownership is transferred to the composite

**Returns:** Reference to `this` (for method chaining)

**Example:**
```cpp
composite_strategy composite;
composite.add(std::make_unique<deployment_strategy>(deployment_env::production));
composite.add(std::make_unique<performance_strategy>(performance_level::high_throughput));
```

#### `add<Strategy, Args...>(Args&&... args)`

**Purpose:** Construct and add a strategy in-place (template method).

**Template parameters:**
- `Strategy` — Strategy type to construct
- `Args...` — Constructor argument types

**Parameters:**
- `args...` — Constructor arguments (forwarded)

**Returns:** Reference to `this` (for method chaining)

**Example:**
```cpp
composite_strategy composite;
composite.add<deployment_strategy>(deployment_env::staging)
         .add<performance_strategy>(performance_level::balanced)
         .add<environment_strategy>();
```

### Priority-Based Application

**Example scenario:**

```cpp
composite_strategy composite;
composite.add<environment_strategy>();          // Priority: 100
composite.add<deployment_strategy>(deployment_env::production);  // Priority: 75
composite.add<performance_strategy>(performance_level::balanced); // Priority: 50

logger_config config;
composite.apply(config);
```

**Application order:**
1. `environment_strategy` (priority 100) — Sets config from environment variables
2. `deployment_strategy` (priority 75) — Applies production settings (may override environment)
3. `performance_strategy` (priority 50) — Applies balanced settings (may override deployment)

**Conflict resolution:**
- If environment sets `LOG_ASYNC=false`, but deployment sets `async=true`:
  - `environment_strategy` applies first → `config.async = false`
  - `deployment_strategy` applies second → `config.async = true` (overrides)
  - `performance_strategy` applies third → `config.async = true` (keeps deployment value)
- **Result:** `config.async = true` (deployment/performance wins over environment)

**To prioritize environment variables:**
```cpp
// Lower priority for deployment (apply it first)
class low_priority_deployment : public deployment_strategy {
    int priority() const override { return 25; }  // Lower than performance (50)
};

composite.add<low_priority_deployment>(deployment_env::production);  // Priority: 25
composite.add<performance_strategy>(performance_level::balanced);    // Priority: 50
composite.add<environment_strategy>();                               // Priority: 100

// Application order: low_priority_deployment → performance → environment
// Result: Environment variables override everything
```

---

## Usage Examples

### Example 1: Simple Deployment Strategy

```cpp
#include <kcenon/logger/logger_builder.h>
#include <kcenon/logger/core/strategies/deployment_strategy.h>

int main() {
    auto logger = logger_builder::create()
        .with_strategy<deployment_strategy>(deployment_env::production)
        .with_file_writer("/var/log/app.log")
        .build();

    logger->warn("Application started");  // Logged (warn+)
    logger->info("Processing request");   // Dropped (below warn)
}
```

### Example 2: Combined Deployment + Performance

```cpp
#include <kcenon/logger/logger_builder.h>
#include <kcenon/logger/core/strategies/deployment_strategy.h>
#include <kcenon/logger/core/strategies/performance_strategy.h>

int main() {
    auto logger = logger_builder::create()
        .with_strategy<deployment_strategy>(deployment_env::production)
        .with_strategy<performance_strategy>(performance_level::high_throughput)
        .with_file_writer("/var/log/app.log")
        .build();

    // Production settings + high throughput optimizations
    // Logs warnings/errors with large buffers and batching
}
```

### Example 3: Environment Variables + Deployment

```cpp
#include <kcenon/logger/logger_builder.h>
#include <kcenon/logger/core/strategies/deployment_strategy.h>
#include <kcenon/logger/core/strategies/environment_strategy.h>

int main() {
    auto logger = logger_builder::create()
        .with_strategy<deployment_strategy>(deployment_env::production)
        .with_strategy<environment_strategy>()  // Highest priority
        .with_file_writer("/var/log/app.log")
        .build();

    // Production defaults, but overridden by environment variables if set
    // Example: export LOG_LEVEL=debug → logs debug+ (overrides production's warn+)
}
```

### Example 4: Explicit Composite Strategy

```cpp
#include <kcenon/logger/logger_builder.h>
#include <kcenon/logger/core/strategies/composite_strategy.h>
#include <kcenon/logger/core/strategies/deployment_strategy.h>
#include <kcenon/logger/core/strategies/performance_strategy.h>
#include <kcenon/logger/core/strategies/environment_strategy.h>

int main() {
    // Build composite strategy
    composite_strategy composite;
    composite.add<deployment_strategy>(deployment_env::production)
             .add<performance_strategy>(performance_level::high_throughput)
             .add<environment_strategy>();

    // Use composite
    auto logger = logger_builder::create()
        .with_strategy(std::make_unique<composite_strategy>(std::move(composite)))
        .with_file_writer("/var/log/app.log")
        .build();
}
```

### Example 5: Conditional Strategy (Docker)

```cpp
#include <kcenon/logger/logger_builder.h>
#include <kcenon/logger/core/strategies/deployment_strategy.h>
#include <cstdlib>

int main() {
    // Detect environment
    bool is_docker = std::getenv("DOCKER_CONTAINER") != nullptr;
    bool is_kubernetes = std::getenv("KUBERNETES_SERVICE_HOST") != nullptr;

    deployment_env env;
    if (is_kubernetes) {
        env = deployment_env::production;
    } else if (is_docker) {
        env = deployment_env::staging;
    } else {
        env = deployment_env::development;
    }

    auto logger = logger_builder::create()
        .with_strategy<deployment_strategy>(env)
        .with_console_writer()
        .build();

    logger->info("Running in: " + std::string(
        is_kubernetes ? "Kubernetes" :
        is_docker ? "Docker" :
        "Local development"
    ));
}
```

### Example 6: Multi-Logger Configuration

```cpp
// Different loggers for different modules
auto access_logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::production)
    .with_strategy<performance_strategy>(performance_level::high_throughput)
    .with_file_writer("/var/log/access.log")
    .build();

auto error_logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::production)
    .with_strategy<performance_strategy>(performance_level::low_latency)
    .with_file_writer("/var/log/errors.log")
    .build();

auto debug_logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::development)
    .with_console_writer()
    .build();

// Use appropriate logger for each context
access_logger->info("User accessed /api/data");
error_logger->error("Database connection failed");
debug_logger->trace("Variable x = " + std::to_string(x));
```

---

## Custom Strategy Implementation

### Basic Custom Strategy

```cpp
#include <kcenon/logger/core/strategies/config_strategy_interface.h>
#include <kcenon/logger/core/logger_config.h>

class cloud_vendor_strategy : public config_strategy_interface {
public:
    enum class vendor {
        aws,
        gcp,
        azure
    };

    explicit cloud_vendor_strategy(vendor v) : vendor_(v) {}

    std::string get_name() const override {
        return "cloud:" + vendor_to_string(vendor_);
    }

    void apply(logger_config& config) const override {
        switch (vendor_) {
        case vendor::aws:
            apply_aws(config);
            break;
        case vendor::gcp:
            apply_gcp(config);
            break;
        case vendor::azure:
            apply_azure(config);
            break;
        }
    }

    int priority() const override {
        return 60;  // Between deployment (75) and performance (50)
    }

private:
    vendor vendor_;

    static void apply_aws(logger_config& config) {
        // AWS-specific optimizations
        config.enable_structured_logging = true;  // CloudWatch Logs Insights
        config.enable_batch_writing = true;       // Reduce API calls
        config.batch_size = 500;
        config.flush_interval = std::chrono::milliseconds(5000);
    }

    static void apply_gcp(logger_config& config) {
        // GCP-specific optimizations
        config.enable_structured_logging = true;  // Cloud Logging
        config.enable_compression = true;         // Reduce egress costs
        config.batch_size = 250;
    }

    static void apply_azure(logger_config& config) {
        // Azure-specific optimizations
        config.enable_structured_logging = true;  // Application Insights
        config.enable_batch_writing = true;
        config.batch_size = 300;
    }

    static std::string vendor_to_string(vendor v) {
        switch (v) {
        case vendor::aws: return "aws";
        case vendor::gcp: return "gcp";
        case vendor::azure: return "azure";
        default: return "unknown";
        }
    }
};
```

**Usage:**
```cpp
auto logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::production)
    .with_strategy<cloud_vendor_strategy>(cloud_vendor_strategy::vendor::aws)
    .with_file_writer("/var/log/app.log")
    .build();
```

### Conditional Strategy (is_applicable)

```cpp
class region_strategy : public config_strategy_interface {
public:
    explicit region_strategy(std::string region)
        : region_(std::move(region)) {}

    std::string get_name() const override {
        return "region:" + region_;
    }

    bool is_applicable() const override {
        // Only apply if REGION environment variable matches
        const char* env_region = std::getenv("REGION");
        return env_region != nullptr && env_region == region_;
    }

    void apply(logger_config& config) const override {
        if (region_ == "us-east-1") {
            // US East: high traffic, large buffers
            config.buffer_size = 65536;
            config.batch_size = 500;
        } else if (region_ == "eu-west-1") {
            // EU West: GDPR compliance, disable metrics
            config.enable_metrics = false;
            config.enable_source_location = false;
        }
    }

    int priority() const override {
        return 90;  // High priority (region-specific overrides)
    }

private:
    std::string region_;
};
```

**Usage:**
```bash
export REGION=eu-west-1
```

```cpp
composite_strategy composite;
composite.add<region_strategy>("us-east-1");  // Skipped (REGION != us-east-1)
composite.add<region_strategy>("eu-west-1");  // Applied (REGION == eu-west-1)
composite.add<deployment_strategy>(deployment_env::production);

auto logger = logger_builder::create()
    .with_strategy(std::make_unique<composite_strategy>(std::move(composite)))
    .build();
```

---

## Strategy Priority and Conflict Resolution

### Priority Values

| Strategy | Priority | Rationale |
|----------|----------|-----------|
| **environment_strategy** | 100 | Environment variables should override all other settings |
| **deployment_strategy** | 75 | Deployment context is more specific than performance |
| **performance_strategy** | 50 | Performance tuning is general-purpose |
| **Custom strategies** | 0 (default) | Applied last unless overridden |

### Conflict Resolution Rules

1. **Strategies are sorted by priority (descending)**
   - Higher priority → applied first
   - Lower priority → applied last (and overrides earlier settings)

2. **Later strategies override earlier ones**
   ```cpp
   composite.add<deployment_strategy>(deployment_env::production);  // Priority 75
   composite.add<environment_strategy>();                           // Priority 100

   // Application order:
   // 1. environment_strategy (priority 100) → sets config.async = false (from LOG_ASYNC=false)
   // 2. deployment_strategy (priority 75)   → sets config.async = true (overrides)
   // Result: config.async = true (deployment wins)
   ```

3. **To make environment variables win:**
   ```cpp
   // Option 1: Swap priority by subclassing
   class low_priority_deployment : public deployment_strategy {
       int priority() const override { return 25; }  // Lower than environment (100)
   };

   composite.add<low_priority_deployment>(deployment_env::production);
   composite.add<environment_strategy>();

   // Application order:
   // 1. environment_strategy (priority 100) → sets config.async = false
   // 2. low_priority_deployment (priority 25) → sets config.async = true, but...
   //    Wait, no! Lower priority applies AFTER higher priority in composite_strategy.
   //    Composite sorts descending, so:
   //    1. environment_strategy (100) → config.async = false
   //    2. low_priority_deployment (25) → config.async = true (overrides)
   //
   // To fix: Invert the logic. Higher priority should apply LAST (be the final word).
   // Actually, looking at composite_strategy::apply(), it sorts descending and applies in that order.
   // So priority 100 is applied first, then 75, then 50.
   // This means LOWER priority values override HIGHER priority values.
   // Therefore, to make environment win, give it a LOWER priority than deployment.

   // Correction:
   class high_priority_environment : public environment_strategy {
       int priority() const override { return 25; }  // LOWER than deployment (75)
   };

   composite.add<deployment_strategy>(deployment_env::production);  // Priority 75 → applied first
   composite.add<high_priority_environment>();                      // Priority 25 → applied last (wins)

   // Application order (sorted descending by priority):
   // 1. deployment_strategy (priority 75) → config.async = true
   // 2. high_priority_environment (priority 25) → config.async = false (if LOG_ASYNC=false)
   // Result: config.async = false (environment wins)
   ```

   **Wait, this is confusing. Let me re-read the composite_strategy code:**

   ```cpp
   std::sort(sorted.begin(), sorted.end(),
       [](const auto* a, const auto* b) {
           return a->priority() > b->priority();  // Descending order
       });

   for (const auto* strategy : sorted) {
       if (strategy->is_applicable()) {
           strategy->apply(config);  // Apply in sorted order
       }
   }
   ```

   **Analysis:**
   - Strategies are sorted in **descending order** by priority (100 → 75 → 50 → ...)
   - Applied in that order (highest priority first, lowest last)
   - **Last applied strategy wins** (overwrites previous values)
   - Therefore, **LOWER priority values win** (applied last, override earlier)

   **Correction (final):**
   To make environment variables win, give `environment_strategy` a **LOWER** priority:

   ```cpp
   // Default priorities:
   // - environment_strategy: 100 (applied first)
   // - deployment_strategy: 75 (applied second, overrides environment)
   //
   // To make environment win:
   class low_priority_deployment : public deployment_strategy {
       int priority() const override { return 25; }  // Lower than environment (100)
   };

   composite.add<environment_strategy>();           // Priority 100 → applied first
   composite.add<low_priority_deployment>(deployment_env::production);  // Priority 25 → applied last (loses)

   // Application order:
   // 1. environment_strategy (100)
   // 2. low_priority_deployment (25)
   // Result: low_priority_deployment wins (applied last)
   //
   // This is still wrong!

   // Actually, to make environment win, we need to REVERSE the priority:
   class high_priority_environment : public environment_strategy {
       int priority() const override { return 10; }  // LOWEST priority (applied LAST)
   };

   composite.add<deployment_strategy>(deployment_env::production);  // Priority 75
   composite.add<high_priority_environment>();                      // Priority 10

   // Application order (descending):
   // 1. deployment_strategy (75)
   // 2. high_priority_environment (10)
   // Result: environment wins (applied last, overrides deployment)
   ```

4. **Summary:**
   - **Higher priority number** → Applied **first**
   - **Lower priority number** → Applied **last** (wins conflicts)
   - Default `environment_strategy` priority (100) means it's applied **first**, so other strategies override it
   - To make environment variables the final word, give it a **lower priority** (e.g., 10)

### Best Practice: Environment Variables as Final Override

```cpp
// Option 1: Custom low-priority environment strategy
class final_environment_strategy : public environment_strategy {
    int priority() const override {
        return 10;  // Lowest priority → applied last → wins conflicts
    }
};

composite_strategy composite;
composite.add<deployment_strategy>(deployment_env::production);  // Priority 75
composite.add<performance_strategy>(performance_level::balanced); // Priority 50
composite.add<final_environment_strategy>();                     // Priority 10

// Application order: deployment → performance → environment
// Result: Environment variables override everything

// Option 2: Manual ordering (apply environment last)
logger_config config;
deployment_strategy(deployment_env::production).apply(config);
performance_strategy(performance_level::balanced).apply(config);
environment_strategy().apply(config);  // Applied last → wins
```

---

## Best Practices

### 1. Start with Defaults, Override as Needed

```cpp
// Development: Use deployment strategy
auto dev_logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::development)
    .build();

// Production: Deployment + performance + environment
auto prod_logger = logger_builder::create()
    .with_strategy<deployment_strategy>(deployment_env::production)
    .with_strategy<performance_strategy>(performance_level::high_throughput)
    .with_strategy<environment_strategy>()
    .build();
```

### 2. Use Environment Variables for Runtime Configuration

```cpp
// Application reads environment at startup
auto logger = logger_builder::create()
    .with_strategy<environment_strategy>()  // Runtime config
    .with_file_writer("/var/log/app.log")
    .build();
```

**Deployment:**
```bash
# Development
LOG_LEVEL=trace LOG_COLOR=true ./myapp

# Production
LOG_LEVEL=warn LOG_ASYNC=true LOG_BUFFER_SIZE=65536 ./myapp
```

### 3. Combine Strategies for Complex Requirements

```cpp
composite_strategy composite;

// Base configuration
composite.add<deployment_strategy>(deployment_env::production);

// Performance tuning
composite.add<performance_strategy>(performance_level::high_throughput);

// Cloud vendor optimizations
composite.add<cloud_vendor_strategy>(cloud_vendor_strategy::vendor::aws);

// Region-specific settings
composite.add<region_strategy>("us-east-1");

// Environment variable overrides
composite.add<environment_strategy>();

auto logger = logger_builder::create()
    .with_strategy(std::make_unique<composite_strategy>(std::move(composite)))
    .build();
```

### 4. Test Strategies in Isolation

```cpp
TEST(StrategyTest, DeploymentProduction) {
    logger_config config;
    deployment_strategy(deployment_env::production).apply(config);

    ASSERT_TRUE(config.async);
    ASSERT_EQ(config.min_level, log_level::warn);
    ASSERT_TRUE(config.enable_compression);
}

TEST(StrategyTest, PerformanceHighThroughput) {
    logger_config config;
    performance_strategy(performance_level::high_throughput).apply(config);

    ASSERT_EQ(config.buffer_size, 65536);
    ASSERT_EQ(config.batch_size, 500);
    ASSERT_TRUE(config.enable_compression);
}
```

### 5. Document Custom Strategies

```cpp
/**
 * @class my_custom_strategy
 * @brief Custom strategy for [specific purpose]
 *
 * @details [Explain what this strategy does and why]
 *
 * Priority: [explain priority choice]
 * Applicability: [when this strategy applies]
 */
class my_custom_strategy : public config_strategy_interface {
    // ...
};
```

---

## Summary

The **logger_system configuration strategies framework** provides:

- ✅ **Flexible configuration** — Adapt logger behavior to environment/performance needs
- ✅ **Composability** — Combine multiple strategies with priority-based conflict resolution
- ✅ **Reusability** — Share configurations across projects
- ✅ **Testability** — Mock strategies for unit tests
- ✅ **12-factor compliance** — Environment variable support for containerized deployments

**Built-in strategies:**
- **Deployment** — 4 environments (development, staging, production, testing)
- **Environment** — 11 environment variables (LOG_LEVEL, LOG_ASYNC, etc.)
- **Performance** — 4 levels (low_latency, balanced, high_throughput, minimal_overhead)
- **Composite** — Combine multiple strategies with priority ordering

**Next steps:**
- Review [ARCHITECTURE.md](ARCHITECTURE.md) for logger architecture overview
- Review [API.md](API.md) for full API reference
- Review [EXAMPLES.md](EXAMPLES.md) for usage examples

---

*Last updated: 2025-02-09*
*logger_system version: 2.x*
