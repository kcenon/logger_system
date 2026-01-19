# Async Integration Guide

> **Language:** **English** | [한국어](THREAD_SYSTEM.kr.md)

## Overview

This guide explains how to enable and use async processing integration in logger_system.

Since v3.1.0 (Issue #222), logger_system uses a standalone `std::jthread`-based async worker by default. Multiple integration options are available:

1. **Standalone** (default): Built-in async worker using `std::jthread`
2. **IExecutor** (v1.5.0+, Issue #253): Interface-based integration using `common_system::IExecutor`
3. **thread_system** (legacy): Direct integration with `thread_system::thread_pool`

## Integration Approaches

### Recommended: IExecutor Interface (v1.5.0+)

The `executor_integration` module provides IExecutor-based async processing without compile-time dependency on thread_system. This is the recommended approach for new projects.

```cpp
#include <kcenon/logger/integration/executor_integration.h>

using namespace kcenon::logger::integration;

// Option 1: Enable with default standalone executor
executor_integration::enable();

// Option 2: Enable with custom IExecutor (e.g., from thread_system)
auto executor = create_thread_pool_executor();  // Your executor
executor_integration::set_executor(executor);

// Submit tasks
executor_integration::submit_task([]() {
    // async work
});

// Check status
if (executor_integration::is_enabled()) {
    std::cout << "Using " << executor_integration::get_executor_name() << "\n";
}

// Disable when done
executor_integration::disable();
```

### Legacy: Direct thread_system Integration

For projects already using thread_system directly, the `thread_system_integration` module is still available.

## Quick Start

### Enable at Build Time

```bash
cmake -S . -B build -DLOGGER_USE_THREAD_SYSTEM=ON
cmake --build build
```

### Enable at Runtime

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>

using namespace kcenon::logger::integration;

// Option 1: Enable with default thread pool
thread_system_integration::enable();

// Option 2: Enable with custom thread pool
auto pool = std::make_shared<kcenon::thread::thread_pool>("my_logger_pool");
pool->start();
thread_system_integration::enable(pool);

// Check if enabled
if (thread_system_integration::is_enabled()) {
    // Using thread_pool backend
}

// Disable when done
thread_system_integration::disable();
```

## Configuration

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `LOGGER_USE_THREAD_SYSTEM` | `OFF` | Enable optional thread_system integration |

When enabled, CMake will:
1. Search for `thread_system` library
2. Define `LOGGER_HAS_THREAD_SYSTEM` compile definition
3. Link LoggerSystem with thread_system

### Compile Definitions

| Definition | When Defined | Effect |
|------------|--------------|--------|
| `LOGGER_HAS_THREAD_SYSTEM` | `LOGGER_USE_THREAD_SYSTEM=ON` and thread_system found | Enables full integration API |

## Dependency Configuration

### Bidirectional Dependency Risk (Issue #252)

Both `logger_system` and `thread_system` can optionally depend on each other:

```
           ┌──────────────────────┐
           │    thread_system     │
           │                      │
           │  BUILD_WITH_LOGGER_  │◄──── Uses for debug output
           │  SYSTEM              │
           └──────────┬───────────┘
                      │
    BUILD_WITH_       │  LOGGER_USE_
    LOGGER_SYSTEM     │  THREAD_SYSTEM
                      │
           ┌──────────▼───────────┐
           │    logger_system     │
           │                      │
           │  LOGGER_HAS_THREAD_  │◄──── Uses for async I/O
           │  SYSTEM              │
           └──────────────────────┘
```

**WARNING**: Enabling both directions simultaneously creates circular dependency risk.

### Recommended Configuration Matrix

| Use Case | thread→logger | logger→thread | CMake Flags | Notes |
|----------|---------------|---------------|-------------|-------|
| Logger standalone | OFF | OFF | (default) | Minimal build, internal std::jthread |
| Logger with thread_pool | OFF | **ON** | `LOGGER_USE_THREAD_SYSTEM=ON` | **Recommended** for async I/O |
| Thread with debug logging | ON | OFF | `BUILD_WITH_LOGGER_SYSTEM=ON` | For debugging thread_system |
| Production | OFF | **ON** | `LOGGER_USE_THREAD_SYSTEM=ON` | Avoid reverse for production |

### Safe Configuration Examples

**Recommended: Logger uses thread_system (one-way)**
```bash
# logger_system → thread_system only
cmake -B build \
    -DLOGGER_USE_THREAD_SYSTEM=ON \
    -DBUILD_WITH_LOGGER_SYSTEM=OFF
```

**Standalone mode (no cross-dependency)**
```bash
# No integration, minimal dependencies
cmake -B build \
    -DLOGGER_USE_THREAD_SYSTEM=OFF
```

**NOT recommended: Bidirectional dependency**
```bash
# WARNING: May cause build order issues
cmake -B build \
    -DLOGGER_USE_THREAD_SYSTEM=ON \
    -DBUILD_WITH_LOGGER_SYSTEM=ON  # Avoid this combination!
```

### CMake Conflict Detection

logger_system includes automatic detection for bidirectional dependency risk. When both directions are enabled, CMake will emit a warning:

```
=================================================================
BIDIRECTIONAL DEPENDENCY RISK DETECTED (Issue #252)
=================================================================
  logger_system → thread_system (LOGGER_USE_THREAD_SYSTEM=ON)
  thread_system → logger_system (BUILD_WITH_LOGGER_SYSTEM=ON)

This configuration may cause:
  - Build order issues in unified builds
  - Circular header include problems
  - Initialization order complications

RECOMMENDATION:
  Enable only ONE direction. Preferred configuration:
    LOGGER_USE_THREAD_SYSTEM=ON (for async I/O performance)
    BUILD_WITH_LOGGER_SYSTEM=OFF (disable reverse dependency)
=================================================================
```

## API Reference

### thread_system_integration

Static class providing thread_system integration management.

#### Methods

```cpp
namespace kcenon::logger::integration {

class thread_system_integration {
public:
    // Enable thread_pool backend (creates default pool if nullptr)
    static void enable(std::shared_ptr<kcenon::thread::thread_pool> pool = nullptr);

    // Disable and fall back to standalone backend
    static void disable();

    // Check if thread_pool backend is active
    [[nodiscard]] static bool is_enabled() noexcept;

    // Get current backend type
    [[nodiscard]] static async_backend_type get_backend() noexcept;

    // Set/get custom thread pool
    static void set_thread_pool(std::shared_ptr<kcenon::thread::thread_pool> pool);
    [[nodiscard]] static std::shared_ptr<kcenon::thread::thread_pool> get_thread_pool() noexcept;

    // Submit task to thread_pool (returns false if not enabled)
    [[nodiscard]] static bool submit_task(std::function<void()> task);

    // Get backend name for diagnostics
    [[nodiscard]] static std::string get_backend_name() noexcept;
};

} // namespace kcenon::logger::integration
```

### async_backend_type

```cpp
enum class async_backend_type {
    standalone,   // Default std::jthread-based worker
    thread_pool   // thread_system's thread_pool
};
```

### Compile-time Detection

```cpp
// Check if thread_system support is available
constexpr bool has_thread_system_support() noexcept;

// Usage
if constexpr (has_thread_system_support()) {
    // Use thread_system features
} else {
    // Use standalone features only
}
```

## Usage Patterns

### Pattern 1: Application-wide Shared Pool

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>
#include <kcenon/logger/core/logger_builder.h>

using namespace kcenon::logger;
using namespace kcenon::logger::integration;

int main() {
    // Enable thread_system integration at startup
    thread_system_integration::enable();

    // Create logger - will use thread_pool if available
    auto logger = logger_builder()
        .use_template("production")
        .build()
        .value();

    // ... application code ...

    // Cleanup at shutdown
    thread_system_integration::disable();
    return 0;
}
```

### Pattern 2: Custom Pool Configuration

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>
#include <kcenon/thread/core/thread_pool.h>

int main() {
    // Create custom thread pool with specific settings
    auto pool = std::make_shared<kcenon::thread::thread_pool>(
        "high_perf_logger"
    );

    // Start the pool
    auto result = pool->start();
    if (!result) {
        std::cerr << "Failed to start pool\n";
        return 1;
    }

    // Use custom pool for logging
    thread_system_integration::enable(pool);

    // ... application code ...

    // Cleanup
    thread_system_integration::disable();
    pool->stop();
    return 0;
}
```

### Pattern 3: Conditional Integration

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>

void setup_logging() {
    using namespace kcenon::logger::integration;

    // Only enable if thread_system is available
    if constexpr (has_thread_system_support()) {
        thread_system_integration::enable();
        std::cout << "Using " << thread_system_integration::get_backend_name()
                  << " backend\n";
    } else {
        std::cout << "Using standalone backend (thread_system not available)\n";
    }
}
```

## Thread Safety

All `thread_system_integration` methods are thread-safe:

- Backend state changes are atomic
- Pool access is protected by mutex
- Multiple threads can safely call `enable()`/`disable()`

## Performance Considerations

### When to Use thread_system Integration

**Use thread_pool backend when:**
- Multiple loggers share the same thread pool
- You need priority-based log processing
- Advanced metrics and monitoring are required
- Your application already uses thread_system

**Use standalone backend when:**
- Minimal dependencies are preferred
- Single logger per application
- Logging overhead should be minimal
- thread_system is not available

### Overhead Comparison

| Backend | Task Submission | Memory | Dependencies |
|---------|-----------------|--------|--------------|
| Standalone | ~50ns | Lower | None |
| thread_pool | ~100ns | Higher | thread_system |

## Troubleshooting

### thread_system not found

```
-- Logger System: thread_system not found, using standalone mode
```

**Solution**: Ensure thread_system is installed or available via FetchContent:
```cmake
set(UNIFIED_USE_LOCAL ON)  # Use local sibling directories
cmake -DLOGGER_USE_THREAD_SYSTEM=ON ..
```

### Runtime detection not working

If `is_enabled()` returns false after `enable()`:

1. Check that pool was created successfully
2. Verify pool is running (`pool->is_running()`)
3. Check for errors in pool creation

```cpp
auto pool = thread_system_integration::get_thread_pool();
if (pool) {
    std::cout << "Pool running: " << pool->is_running() << "\n";
} else {
    std::cout << "No pool configured\n";
}
```

## IExecutor Integration API (v1.5.0+)

### executor_integration

Static class providing IExecutor-based integration management.

```cpp
namespace kcenon::logger::integration {

class executor_integration {
public:
    // Enable async processing (creates default standalone executor if nullptr)
    static void enable(std::shared_ptr<common::interfaces::IExecutor> executor = nullptr);

    // Disable async processing
    static void disable();

    // Check if async processing is enabled
    [[nodiscard]] static bool is_enabled() noexcept;

    // Get current executor type
    [[nodiscard]] static executor_type get_executor_type() noexcept;

    // Set/get executor
    static void set_executor(std::shared_ptr<common::interfaces::IExecutor> executor);
    [[nodiscard]] static std::shared_ptr<common::interfaces::IExecutor> get_executor() noexcept;

    // Submit task
    [[nodiscard]] static bool submit_task(std::function<void()> task);
    [[nodiscard]] static bool submit_task_delayed(std::function<void()> task,
                                                   std::chrono::milliseconds delay);

    // Metrics
    [[nodiscard]] static std::string get_executor_name() noexcept;
    [[nodiscard]] static size_t pending_tasks() noexcept;
    [[nodiscard]] static size_t worker_count() noexcept;
};

} // namespace kcenon::logger::integration
```

### executor_type

```cpp
enum class executor_type {
    none,       // No executor configured
    standalone, // Built-in standalone executor
    external    // User-provided IExecutor
};
```

### standalone_executor

Built-in IExecutor implementation using a single worker thread.

```cpp
namespace kcenon::logger::integration {

class standalone_executor : public common::interfaces::IExecutor {
public:
    explicit standalone_executor(std::size_t queue_size = 8192,
                                  std::string name = "standalone_executor");

    void start();

    // IExecutor interface
    Result<std::future<void>> execute(std::unique_ptr<IJob>&& job) override;
    Result<std::future<void>> execute_delayed(std::unique_ptr<IJob>&& job,
                                               std::chrono::milliseconds delay) override;
    size_t worker_count() const override;
    bool is_running() const override;
    size_t pending_tasks() const override;
    void shutdown(bool wait_for_completion = true) override;
};

// Factory for creating standalone executors
class standalone_executor_factory {
public:
    static std::shared_ptr<IExecutor> create(
        std::size_t queue_size = 8192,
        const std::string& name = "standalone_executor");
};

} // namespace kcenon::logger::integration
```

### Compile-time Detection

```cpp
// Check if IExecutor support is available
constexpr bool has_executor_support() noexcept;

// Check if IExecutor interface is detected
constexpr bool has_iexecutor_interface() noexcept;

// Usage
if constexpr (has_executor_support()) {
    executor_integration::enable();
}
```

## Migration from thread_system_integration

If you're currently using `thread_system_integration`, you can migrate to `executor_integration`:

```cpp
// Before (legacy)
#include <kcenon/logger/integration/thread_system_integration.h>
thread_system_integration::enable(pool);
thread_system_integration::submit_task(task);

// After (recommended)
#include <kcenon/logger/integration/executor_integration.h>
#include <kcenon/thread/adapters/common_executor_adapter.h>

// Wrap thread_pool in IExecutor adapter
auto executor = kcenon::thread::adapters::common_executor_factory::create_from_thread_pool(pool);
executor_integration::set_executor(executor);
executor_integration::submit_task(task);
```

## Related Documentation

- [async_worker](../internals/ASYNC_WORKER.md) - Standalone async implementation
- [thread_system](https://github.com/kcenon/thread_system) - Thread pool library
- [common_system IExecutor](https://github.com/kcenon/common_system) - Executor interface
- [Issue #224](https://github.com/kcenon/logger_system/issues/224) - Optional integration feature request
- [Issue #252](https://github.com/kcenon/logger_system/issues/252) - Bidirectional dependency resolution
- [Issue #253](https://github.com/kcenon/logger_system/issues/253) - IExecutor migration

---

*Last updated: 2025-12*
