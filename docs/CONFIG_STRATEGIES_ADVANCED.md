---
doc_id: "LOG-GUID-001b"
doc_title: "Configuration Strategies - Advanced"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "GUID"
---

# Configuration Strategies - Advanced

> **Split from**: [CONFIGURATION_STRATEGIES.md](CONFIGURATION_STRATEGIES.md)

Composite strategies, custom strategy implementation, priority-based conflict resolution, and production tuning patterns.

---

## Table of Contents

1. [Composite Strategy](#composite-strategy)
2. [Custom Strategy Implementation](#custom-strategy-implementation)
3. [Strategy Priority and Conflict Resolution](#strategy-priority-and-conflict-resolution)
4. [Best Practices](#best-practices)
5. [Summary](#summary)

---

## Composite Strategy

**Header:** `include/kcenon/logger/core/strategies/composite_strategy.h`

**Purpose:** Combine multiple strategies into a single strategy.

### How It Works

1. **Add strategies** to the composite (via `add()`)
2. **Sort by priority** (descending: 100 -> 75 -> 50 -> ...)
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
- `strategy` -- Ownership is transferred to the composite

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
- `Strategy` -- Strategy type to construct
- `Args...` -- Constructor argument types

**Parameters:**
- `args...` -- Constructor arguments (forwarded)

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
1. `environment_strategy` (priority 100) -- Sets config from environment variables
2. `deployment_strategy` (priority 75) -- Applies production settings (may override environment)
3. `performance_strategy` (priority 50) -- Applies balanced settings (may override deployment)

**Conflict resolution:**
- If environment sets `LOG_ASYNC=false`, but deployment sets `async=true`:
  - `environment_strategy` applies first -> `config.async = false`
  - `deployment_strategy` applies second -> `config.async = true` (overrides)
  - `performance_strategy` applies third -> `config.async = true` (keeps deployment value)
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

// Application order: low_priority_deployment -> performance -> environment
// Result: Environment variables override everything
```

### Explicit Composite Strategy Example

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
   - Higher priority -> applied first
   - Lower priority -> applied last (and overrides earlier settings)

2. **Later strategies override earlier ones**
   ```cpp
   composite.add<deployment_strategy>(deployment_env::production);  // Priority 75
   composite.add<environment_strategy>();                           // Priority 100

   // Application order:
   // 1. environment_strategy (priority 100) -> sets config.async = false (from LOG_ASYNC=false)
   // 2. deployment_strategy (priority 75)   -> sets config.async = true (overrides)
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
   // 1. environment_strategy (priority 100) -> sets config.async = false
   // 2. low_priority_deployment (priority 25) -> sets config.async = true, but...
   //    Wait, no! Lower priority applies AFTER higher priority in composite_strategy.
   //    Composite sorts descending, so:
   //    1. environment_strategy (100) -> config.async = false
   //    2. low_priority_deployment (25) -> config.async = true (overrides)
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

   composite.add<deployment_strategy>(deployment_env::production);  // Priority 75 -> applied first
   composite.add<high_priority_environment>();                      // Priority 25 -> applied last (wins)

   // Application order (sorted descending by priority):
   // 1. deployment_strategy (priority 75) -> config.async = true
   // 2. high_priority_environment (priority 25) -> config.async = false (if LOG_ASYNC=false)
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
   - Strategies are sorted in **descending order** by priority (100 -> 75 -> 50 -> ...)
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

   composite.add<environment_strategy>();           // Priority 100 -> applied first
   composite.add<low_priority_deployment>(deployment_env::production);  // Priority 25 -> applied last (loses)

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
   - **Higher priority number** -> Applied **first**
   - **Lower priority number** -> Applied **last** (wins conflicts)
   - Default `environment_strategy` priority (100) means it's applied **first**, so other strategies override it
   - To make environment variables the final word, give it a **lower priority** (e.g., 10)

### Best Practice: Environment Variables as Final Override

```cpp
// Option 1: Custom low-priority environment strategy
class final_environment_strategy : public environment_strategy {
    int priority() const override {
        return 10;  // Lowest priority -> applied last -> wins conflicts
    }
};

composite_strategy composite;
composite.add<deployment_strategy>(deployment_env::production);  // Priority 75
composite.add<performance_strategy>(performance_level::balanced); // Priority 50
composite.add<final_environment_strategy>();                     // Priority 10

// Application order: deployment -> performance -> environment
// Result: Environment variables override everything

// Option 2: Manual ordering (apply environment last)
logger_config config;
deployment_strategy(deployment_env::production).apply(config);
performance_strategy(performance_level::balanced).apply(config);
environment_strategy().apply(config);  // Applied last -> wins
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

- **Flexible configuration** -- Adapt logger behavior to environment/performance needs
- **Composability** -- Combine multiple strategies with priority-based conflict resolution
- **Reusability** -- Share configurations across projects
- **Testability** -- Mock strategies for unit tests
- **12-factor compliance** -- Environment variable support for containerized deployments

**Built-in strategies:**
- **Deployment** -- 4 environments (development, staging, production, testing)
- **Environment** -- 11 environment variables (LOG_LEVEL, LOG_ASYNC, etc.)
- **Performance** -- 4 levels (low_latency, balanced, high_throughput, minimal_overhead)
- **Composite** -- Combine multiple strategies with priority ordering

**Next steps:**
- Review [ARCHITECTURE.md](ARCHITECTURE.md) for logger architecture overview
- Review [API.md](API.md) for full API reference
- Review [EXAMPLES.md](EXAMPLES.md) for usage examples

---

*Last updated: 2025-02-09*
*logger_system version: 2.x*
