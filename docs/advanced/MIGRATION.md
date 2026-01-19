# Migration Guide - Logger System

> **Language:** **English** | [한국어](MIGRATION.kr.md)

## Overview

This guide helps you migrate to the standalone logger_system from previous integrated implementations or other logging frameworks.

**Migration Paths Available:**
1. From integrated thread_system logger → standalone logger_system
2. From spdlog or other frameworks → logger_system
3. From custom logging implementations → logger_system

**Estimated Time:** 1-3 hours for typical applications
**Breaking Changes:** Yes (API, namespace, build configuration)

---

## Table of Contents

- [Quick Migration Checklist](#quick-migration-checklist)
- [Migration Path 1: From Integrated thread_system](#migration-path-1-from-integrated-thread_system)
- [Migration Path 2: From spdlog](#migration-path-2-from-spdlog)
- [Migration Path 3: From Custom Logger](#migration-path-3-from-custom-logger)
- [Common Migration Issues](#common-migration-issues)
- [Testing Your Migration](#testing-your-migration)
- [Rollback Plan](#rollback-plan)

---

## Quick Migration Checklist

- [ ] Update CMakeLists.txt dependencies
- [ ] Replace header includes
- [ ] Update namespace references
- [ ] Migrate logger initialization code
- [ ] Update log call sites (if needed)
- [ ] Rebuild and test
- [ ] Update integration points

---

## Migration Path 1: From Integrated thread_system

### Background

In earlier versions, logging functionality was integrated into thread_system. The logger has been extracted into a standalone logger_system module for better modularity and independent development.

### Step 1: Update CMakeLists.txt

**Before** (integrated thread_system):
```cmake
find_package(thread_system CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE kcenon::thread_system)
```

**After** (standalone logger_system):
```cmake
find_package(logger_system CONFIG REQUIRED)
find_package(thread_system CONFIG REQUIRED) # Still needed if using thread pool

target_link_libraries(your_target PRIVATE
    kcenon::logger_system
    kcenon::thread_system  # Optional, only if using threads
)
```

### Step 2: Update Header Includes

**Before**:
```cpp
#include <thread_system/logger/logger.h>
#include <thread_system/logger/console_writer.h>
#include <thread_system/logger/file_writer.h>
```

**After**:
```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>
```

### Step 3: Update Namespace

**Before**:
```cpp
using namespace thread_system;

auto logger_instance = thread_system::create_logger();
logger_instance->info("Message");
```

**After**:
```cpp
using namespace kcenon::logger;

auto logger_instance = logger_builder()
    .with_console_output()
    .build()
    .value();
logger_instance->info("Message");
```

### Step 4: Migrate Logger Creation

**Before** (old integrated API):
```cpp
#include <thread_system/logger/logger.h>

auto logger_instance = std::make_shared<thread_system::logger>();
logger_instance->add_writer(std::make_shared<console_writer>());
logger_instance->set_level(log_level::info);
```

**After** (new builder pattern):
```cpp
#include <kcenon/logger/core/logger_builder.h>

auto result = logger_builder()
    .with_console_output()
    .with_level(log_level::info)
    .build();

if (result.is_ok()) {
    auto logger_instance = result.value();
    logger_instance->info("Logger created successfully");
} else {
    // Handle error
    std::cerr << "Logger creation failed: " << result.error().message << std::endl;
}
```

### Step 5: Update Thread System Integration (Optional)

If you're using thread_system with logger integration:

**Before** (integrated):
```cpp
auto pool = thread_system::create_thread_pool();
pool->set_logger(logger_instance); // Direct logger
```

**After** (adapter pattern):
```cpp
#include <kcenon/logger/adapters/logger_adapter.h>

auto logger_instance = logger_builder()
    .with_console_output()
    .build()
    .value();

auto adapter = std::make_shared<logger_adapter>(logger_instance);
pool->set_logger(adapter); // Use adapter
```

---

## Migration Path 2: From spdlog

### Overview

Migrating from spdlog to logger_system provides better ecosystem integration, Result<T> error handling, and performance improvements in multi-threaded scenarios.

### API Mapping

| spdlog API | logger_system API | Notes |
|------------|-------------------|-------|
| `spdlog::info()` | `logger->info()` | Instance method instead of static |
| `spdlog::get("name")` | `logger_builder().build()` | Builder pattern |
| `spdlog::rotating_logger_mt()` | `.with_rotating_file()` | Builder method |
| `spdlog::set_level()` | `.with_level()` | Builder method |
| `spdlog::flush_on()` | `.with_flush_policy()` | Builder method |

### Example Migration

**Before** (spdlog):
```cpp
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

auto logger = spdlog::rotating_logger_mt(
    "app_logger",
    "logs/app.log",
    1024 * 1024 * 10, // 10MB
    3                 // 3 rotating files
);

spdlog::set_level(spdlog::level::info);
logger->info("Application started");
```

**After** (logger_system):
```cpp
#include <kcenon/logger/core/logger_builder.h>

auto result = logger_builder()
    .with_rotating_file("logs/app.log", 10 * 1024 * 1024) // 10MB
    .with_level(log_level::info)
    .build();

if (result.is_ok()) {
    auto logger = result.value();
    logger->info("Application started");
}
```

### Pattern Support

**spdlog**:
```cpp
logger->info("User {} logged in from {}", username, ip);
```

**logger_system** (C++20 std::format):
```cpp
logger->info(std::format("User {} logged in from {}", username, ip));
```

Or use direct string:
```cpp
logger->info("User " + username + " logged in from " + ip);
```

---

## Migration Path 3: From Custom Logger

### Common Custom Logger Patterns

#### Pattern 1: Direct File Writing

**Before** (custom):
```cpp
class MyLogger {
    std::ofstream file;
public:
    void log(const std::string& message) {
        file << message << std::endl;
    }
};
```

**After** (logger_system):
```cpp
auto logger = logger_builder()
    .with_file_output("app.log")
    .with_level(log_level::info)
    .build()
    .value();

logger->info(message);
```

**Benefits**:
- Asynchronous I/O (non-blocking)
- Thread-safe by default
- Automatic rotation
- Error handling with Result<T>

#### Pattern 2: Multiple Output Targets

**Before** (custom):
```cpp
class MyLogger {
    std::ofstream file;
    bool use_console;
public:
    void log(const std::string& message) {
        if (use_console) std::cout << message << std::endl;
        file << message << std::endl;
    }
};
```

**After** (logger_system):
```cpp
auto logger = logger_builder()
    .with_console_output()
    .with_file_output("app.log")
    .with_level(log_level::info)
    .build()
    .value();

logger->info(message);
```

**Benefits**:
- Multiple writers without manual branching
- Each writer independently configurable
- Filters per writer

#### Pattern 3: Thread-Safe Singleton

**Before** (custom):
```cpp
class Logger {
    static std::mutex mutex;
    static std::shared_ptr<Logger> instance;
public:
    static std::shared_ptr<Logger> get_instance() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!instance) {
            instance = std::make_shared<Logger>();
        }
        return instance;
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        // Write log
    }
};
```

**After** (logger_system):
```cpp
// Create once (application startup)
class LoggerRegistry {
    static std::shared_ptr<kcenon::logger::logger> instance;
public:
    static std::shared_ptr<kcenon::logger::logger> get() {
        static std::once_flag flag;
        std::call_once(flag, [] {
            instance = logger_builder()
                .with_console_output()
                .build()
                .value();
        });
        return instance;
    }
};

// Usage
LoggerRegistry::get()->info("Message");
```

**Benefits**:
- Thread-safe by design (no locks in application code)
- Lock-free enqueue on hot path
- Batched background processing

---

## Common Migration Issues

### Issue 1: Namespace Conflicts

**Problem**: Both old and new logger in same file

**Solution**: Use explicit namespaces during transition
```cpp
// Temporary migration phase
namespace old_logger = thread_system::logger;
namespace new_logger = kcenon::logger;

// Use old_logger temporarily
auto old_log = old_logger::create_logger();

// Migrate to new_logger
auto new_log = new_logger::logger_builder().build().value();
```

### Issue 2: CMake Cache Issues

**Problem**: CMake still finds old thread_system with integrated logger

**Solution**: Clear CMake cache
```bash
rm -rf build
mkdir build && cd build
cmake ..
```

### Issue 3: Header Not Found

**Problem**:
```
fatal error: kcenon/logger/core/logger.h: No such file or directory
```

**Solution**: Ensure logger_system is properly installed
```bash
# Build and install logger_system
cd /path/to/logger_system
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make
sudo make install
```

Or use FetchContent in CMakeLists.txt:
```cmake
include(FetchContent)
FetchContent_Declare(
    logger_system
    GIT_REPOSITORY https://github.com/kcenon/logger_system.git
    GIT_TAG main
)
FetchContent_MakeAvailable(logger_system)
```

### Issue 4: Result<T> Pattern Unfamiliar

**Problem**: Don't know how to handle Result<T> from build()

**Solution**: Use .is_ok() and .value()
```cpp
auto result = logger_builder()
    .with_console_output()
    .build();

if (result.is_ok()) {
    auto logger = result.value();
    // Use logger
} else {
    auto error = result.error();
    std::cerr << "Error: " << error.message << std::endl;
}
```

Or use monadic operations:
```cpp
auto logger = logger_builder()
    .with_console_output()
    .build()
    .map([](auto logger) {
        logger->info("Logger initialized");
        return logger;
    })
    .or_else([](const auto& error) {
        std::cerr << "Failed: " << error.message << std::endl;
        return create_default_logger(); // Fallback
    })
    .value();
```

### Issue 5: Performance Regression

**Problem**: Slower than previous custom logger

**Solution**: Use high-performance template
```cpp
auto logger = logger_builder()
    .with_template(config_template::high_performance)
    .with_file_output("app.log")
    .build()
    .value();
```

Or disable unnecessary features:
```cmake
# CMakeLists.txt
set(ENABLE_SMALL_STRING_OPTIMIZATION ON)
set(USE_LOCKFREE ON) # If available
```

---

## Testing Your Migration

### 1. Compile Test

```bash
mkdir build && cd build
cmake ..
make
```

Expected: Clean build with no errors

### 2. Unit Tests

```bash
# Run your existing tests
ctest

# Or manually
./build/tests/your_tests
```

Expected: All tests pass

### 3. Integration Tests

```bash
# Test with actual application
./build/your_app --log-level debug

# Check log output
cat logs/app.log
```

Expected: Logs appear correctly formatted

### 4. Performance Benchmark

```bash
# Run benchmarks
./build/benchmarks/logger_benchmark

# Compare with baseline
# Expected: Similar or better performance
```

---

## Rollback Plan

If migration fails, you can rollback:

### Option 1: Revert Changes

```bash
# Revert git changes
git checkout -- .

# Or restore from backup
cp -r backup_before_migration/* .

# Rebuild with old version
mkdir build && cd build
cmake ..
make
```

### Option 2: Gradual Migration

Keep both old and new logger during transition:

```cmake
# CMakeLists.txt
find_package(thread_system CONFIG REQUIRED)  # Old logger
find_package(logger_system CONFIG REQUIRED)  # New logger

target_link_libraries(your_target PRIVATE
    kcenon::thread_system   # Old logger still available
    kcenon::logger_system   # New logger available
)
```

```cpp
// Gradual migration in code
#ifdef USE_NEW_LOGGER
    #include <kcenon/logger/core/logger.h>
    namespace logger = kcenon::logger;
#else
    #include <thread_system/logger/logger.h>
    namespace logger = thread_system::logger;
#endif
```

---

## Migration Assistance

### Automated Migration Script

We provide a migration script to automate common changes:

```bash
# Download migration script
curl -O https://raw.githubusercontent.com/kcenon/logger_system/main/scripts/migrate_from_thread_system.sh
chmod +x migrate_from_thread_system.sh

# Run migration on your source directory
./migrate_from_thread_system.sh /path/to/your/source

# Review changes
git diff

# Rebuild
mkdir build && cd build
cmake ..
make
```

The script performs:
- Header include updates
- Namespace replacements
- CMakeLists.txt modifications
- Basic API pattern updates

**Note**: Manual review is still required for complex logic.

---

## Support

Need help with migration?

- **Migration Issues**: [GitHub Issues](https://github.com/kcenon/logger_system/issues)
- **Migration Questions**: [GitHub Discussions](https://github.com/kcenon/logger_system/discussions)
- **Email Support**: kcenon@naver.com

---

## Next Steps After Migration

1. **Read Documentation**:
   - [README.md](README.md) - Feature overview
   - [ARCHITECTURE.md](ARCHITECTURE.md) - System design
   - [examples/](examples/) - Code examples

2. **Optimize Configuration**:
   - Review log levels
   - Configure rotation policies
   - Set up monitoring integration

3. **Enable Advanced Features**:
   - Structured logging (JSON)
   - Network logging
   - Monitoring integration

---

**Last Updated:** 2025-10-22
**Migration Script Version:** 1.0.0
