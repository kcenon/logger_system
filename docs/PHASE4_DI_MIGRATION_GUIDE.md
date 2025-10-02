# Phase 4: Dependency Injection Pattern Migration Guide

**Document Version**: 1.0
**Date**: 2025-10-02
**Status**: Active

## Overview

Phase 4 introduces Dependency Injection (DI) patterns to logger_system, enabling loose coupling with monitoring systems through common_system interfaces. This guide helps developers migrate to the new DI-based architecture.

---

## What Changed in Phase 4

### Key Improvements

1. **Dependency Injection for Monitors**
   - Monitors are now injected via constructor or setter methods
   - No compile-time dependency on specific monitor implementations
   - Support for optional monitoring (monitor-less operation)

2. **Interface-Based Coupling**
   - Logger only depends on `IMonitor` interface
   - Monitor implementations can be swapped at runtime
   - Supports any `IMonitor` implementation from any source

3. **Enhanced IMonitorable Support**
   - Logger implements `IMonitorable` interface
   - Provides monitoring data and health checks
   - Can be registered with aggregating monitors

---

## Migration Examples

### Example 1: Basic Monitor Injection

**Before (Phase 3):**
```cpp
#include <kcenon/logger/core/logger_builder.h>

// Logger created without monitoring
auto logger = logger_builder()
    .with_async(true)
    .build()
    .value();
```

**After (Phase 4 - with DI):**
```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/common/interfaces/monitoring_interface.h>

// Create or obtain a monitor (any IMonitor implementation)
std::shared_ptr<common::interfaces::IMonitor> monitor = /* ... */;

// Inject monitor during logger creation
auto logger = logger_builder()
    .with_async(true)
    .with_monitoring(monitor)  // DI: inject monitor
    .build()
    .value();
```

### Example 2: Runtime Monitor Injection

**New in Phase 4:**
```cpp
// Create logger without monitor initially
auto logger = logger_builder()
    .with_async(true)
    .build()
    .value();

// Inject monitor at runtime when needed
std::shared_ptr<IMonitor> monitor = std::make_shared<your_monitor>();
logger->set_monitor(monitor);

// Can also swap monitors at runtime
logger->set_monitor(different_monitor);
```

### Example 3: Optional Monitoring

**New in Phase 4:**
```cpp
// Logger works perfectly without a monitor
auto logger = logger_builder()
    .with_async(true)
    .build()
    .value();

// Logging operations work regardless of monitor presence
logger->log(log_level::info, "Application started");

// Conditionally add monitoring later if needed
if (monitoring_enabled) {
    logger->set_monitor(create_monitor());
}
```

### Example 4: Using IMonitorable Interface

**New in Phase 4:**
```cpp
#include <kcenon/common/interfaces/monitoring_interface.h>

auto logger = logger_builder().build().value();

// Logger implements IMonitorable
if (auto monitorable = std::dynamic_pointer_cast<IMonitorable>(logger)) {
    // Get monitoring data
    auto data = monitorable->get_monitoring_data();
    if (data) {
        std::cout << "Logger has " << data.value().metrics.size()
                  << " metrics" << std::endl;
    }

    // Check logger health
    auto health = monitorable->health_check();
    if (health) {
        std::cout << "Logger status: "
                  << to_string(health.value().status) << std::endl;
    }
}
```

---

## Integration with monitoring_system

### Pattern 1: Monitor Provided by monitoring_system

```cpp
// In your application initialization

// Step 1: Create/get monitor from monitoring_system
auto monitor = monitoring_system::get_default_monitor();

// Step 2: Inject into logger
auto logger = logger_builder()
    .with_monitoring(monitor)
    .build()
    .value();

// Step 3: Logger automatically reports metrics to monitor
logger->log(log_level::info, "Message");  // Metrics sent to monitor
```

### Pattern 2: Aggregating Multiple Loggers

```cpp
// monitoring_system creates an aggregating monitor
auto agg_monitor = std::make_shared<aggregating_monitor>();

// Create multiple loggers with same monitor
auto logger1 = logger_builder()
    .with_monitoring(agg_monitor)
    .build().value();

auto logger2 = logger_builder()
    .with_monitoring(agg_monitor)
    .build().value();

// Register loggers as monitored components
agg_monitor->register_component(logger1);
agg_monitor->register_component(logger2);

// Monitor collects metrics from all loggers
auto all_metrics = agg_monitor->get_metrics();
```

### Pattern 3: Health Check Aggregation

```cpp
// monitoring_system can check health of all components
auto monitor = /* ... */;

// Loggers report health through IMonitorable
auto health_result = monitor->check_health();

// Result includes health status of all registered loggers
for (const auto& dep : health_result.value().dependencies) {
    std::cout << dep.name << ": "
              << to_string(dep.status) << std::endl;
}
```

---

## Best Practices

### 1. Use Interface Types in APIs

**Good:**
```cpp
void setup_logging(std::shared_ptr<IMonitor> monitor) {
    auto logger = logger_builder()
        .with_monitoring(monitor)
        .build()
        .value();
    // ...
}
```

**Avoid:**
```cpp
// Don't depend on concrete monitor types
void setup_logging(std::shared_ptr<concrete_monitor> monitor) {
    // This creates tight coupling
}
```

### 2. Make Monitoring Optional

**Good:**
```cpp
auto logger = logger_builder()
    .with_async(true)
    .build()
    .value();

// Add monitoring only if available
if (auto monitor = get_optional_monitor()) {
    logger->set_monitor(monitor);
}
```

### 3. Use Monitor Providers

**Good:**
```cpp
// Get monitor from provider interface
if (auto provider = get_monitor_provider()) {
    auto monitor = provider->get_monitor();
    logger->set_monitor(monitor);
}
```

### 4. Check Capabilities at Runtime

**Good:**
```cpp
std::shared_ptr<ILogger> logger = get_logger();

// Check if logger supports monitoring
if (auto monitorable = std::dynamic_pointer_cast<IMonitorable>(logger)) {
    auto health = monitorable->health_check();
    // Use monitoring features
}
```

---

## Testing DI Integration

### Unit Test Example

```cpp
#include <gtest/gtest.h>
#include <kcenon/logger/core/logger_builder.h>

class MockMonitor : public IMonitor {
    // Mock implementation
};

TEST(LoggerDITest, MonitorInjection) {
    auto mock_monitor = std::make_shared<MockMonitor>();

    auto logger = logger_builder()
        .with_monitoring(mock_monitor)
        .build()
        .value();

    EXPECT_EQ(logger->get_monitor(), mock_monitor);

    // Test that monitor receives metrics
    EXPECT_CALL(*mock_monitor, record_metric(_, _))
        .Times(AtLeast(1));

    logger->log(log_level::info, "test");
}

TEST(LoggerDITest, OptionalMonitor) {
    // Logger should work without monitor
    auto logger = logger_builder()
        .build()
        .value();

    EXPECT_EQ(logger->get_monitor(), nullptr);

    // Should not crash
    EXPECT_NO_THROW(logger->log(log_level::info, "test"));
}

TEST(LoggerDITest, RuntimeMonitorSwap) {
    auto monitor1 = std::make_shared<MockMonitor>();
    auto monitor2 = std::make_shared<MockMonitor>();

    auto logger = logger_builder()
        .with_monitoring(monitor1)
        .build()
        .value();

    logger->set_monitor(monitor2);

    EXPECT_EQ(logger->get_monitor(), monitor2);
}
```

---

## Performance Considerations

### Monitor Injection Overhead

- **Monitor check**: ~1-2 ns (pointer null check)
- **Metric recording**: Depends on monitor implementation
- **No monitor**: Zero overhead when monitor is nullptr

### Recommendations

1. **For High-Performance Scenarios**:
   ```cpp
   // Create logger without monitor for maximum performance
   auto logger = logger_builder().build().value();
   ```

2. **For Production with Monitoring**:
   ```cpp
   // Use lightweight monitor implementations
   auto logger = logger_builder()
       .with_monitoring(lightweight_monitor)
       .build()
       .value();
   ```

3. **For Development/Debug**:
   ```cpp
   // Use full-featured monitor for detailed insights
   auto logger = logger_builder()
       .with_monitoring(detailed_monitor)
       .build()
       .value();
   ```

---

## Troubleshooting

### Issue: Monitor Not Receiving Metrics

**Solution:**
```cpp
// Verify monitor is set
if (logger->get_monitor() == nullptr) {
    logger->set_monitor(your_monitor);
}

// Check monitor implementation
auto result = logger->get_monitor()->record_metric("test", 1.0);
if (!result) {
    std::cerr << "Monitor error: " << result.error().message << std::endl;
}
```

### Issue: Circular Dependency Errors

**Solution:**
```cpp
// Use forward declarations and interfaces
#include <kcenon/common/interfaces/monitoring_interface.h>  // Interface only
#include <kcenon/common/interfaces/logger_interface.h>

// DO NOT include concrete implementations in headers
// #include <kcenon/monitoring/core/performance_monitor.h>  // Avoid this
```

### Issue: Health Check Not Working

**Solution:**
```cpp
// Ensure logger implements IMonitorable
auto logger = logger_builder().build().value();

if (auto monitorable = std::dynamic_pointer_cast<IMonitorable>(logger)) {
    auto health = monitorable->health_check();
    // Now works correctly
} else {
    std::cerr << "Logger does not implement IMonitorable" << std::endl;
}
```

---

## API Reference

### Builder API

```cpp
logger_builder& with_monitoring(std::shared_ptr<IMonitor> monitor);
```
- Injects monitor during logger construction
- Monitor can be nullptr for monitor-less operation

### Runtime API

```cpp
void logger::set_monitor(std::shared_ptr<IMonitor> monitor);
std::shared_ptr<IMonitor> logger::get_monitor() const;
```
- Set or replace monitor at runtime
- Returns current monitor (may be nullptr)

### IMonitorable API

```cpp
Result<metrics_snapshot> get_monitoring_data() override;
Result<health_check_result> health_check() override;
std::string get_component_name() const override;
```
- Implemented by logger class
- Provides monitoring and health data

---

## Examples

See the following example programs:
- `examples/di_pattern_example.cpp` - DI pattern demonstrations
- `examples/monitoring_integration_example.cpp` - Integration with monitors

Build examples:
```bash
cd logger_system
mkdir build && cd build
cmake .. -DBUILD_SAMPLES=ON -DBUILD_WITH_COMMON_SYSTEM=ON
cmake --build .
./bin/di_pattern_example
./bin/monitoring_integration_example
```

---

## Summary

Phase 4 introduces:
- ✅ Dependency Injection for monitors
- ✅ Runtime monitor swapping
- ✅ Optional monitoring support
- ✅ IMonitorable interface implementation
- ✅ Zero circular dependencies
- ✅ Interface-based loose coupling

**Next Steps:**
- Review example programs
- Update your logger creation code to use DI
- Test with your monitor implementations
- Refer to this guide for migration patterns

---

**Document Version**: 1.0
**Last Updated**: 2025-10-02
**Next Review**: Phase 5 Start
