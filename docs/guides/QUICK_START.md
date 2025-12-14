# Quick Start Guide

> **Language:** **English** | [한국어](QUICK_START_KO.md)

Get up and running with Logger System in 5 minutes.

---

## Prerequisites

- CMake 3.20 or later
- C++20 capable compiler (GCC 11+, Clang 14+, MSVC 2022+, Apple Clang 14+)
- Git
- **[common_system](https://github.com/kcenon/common_system)** - Required dependency (must be cloned alongside logger_system)
- **[thread_system](https://github.com/kcenon/thread_system)** - Optional dependency (for advanced async logging with thread pool)

## Installation

### 1. Clone the Repositories

```bash
# Clone common_system first (required dependency)
git clone https://github.com/kcenon/common_system.git

# Clone thread_system (optional, for async logging with thread pool)
git clone https://github.com/kcenon/thread_system.git

# Clone logger_system alongside the dependencies
git clone https://github.com/kcenon/logger_system.git
cd logger_system
```

> **Note:** All repositories must be in the same parent directory for the build to work correctly.

### 2. Install Dependencies

```bash
# Linux/macOS
./scripts/dependency.sh

# Windows
./scripts/dependency.bat
```

### 3. Build

```bash
# Linux/macOS
./scripts/build.sh

# Windows
./scripts/build.bat

# Or using CMake directly
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

# With thread_system integration (optional)
cmake -S . -B build -DLOGGER_USE_THREAD_SYSTEM=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### 4. Verify Installation

```bash
# Run the sample application
./build/bin/basic_logging
```

---

## Your First Logger

Create a simple logging application:

```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>

#include <iostream>
#include <memory>

using namespace kcenon::logger;

int main() {
    // 1. Create a logger using the builder pattern
    auto result = logger_builder()
        .with_min_level(log_level::info)
        .add_writer("console", std::make_unique<console_writer>())
        .add_writer("file", std::make_unique<file_writer>("app.log"))
        .build();

    // 2. Check for errors
    if (result.is_err()) {
        std::cerr << "Failed to create logger: " << result.error().message << "\n";
        return 1;
    }

    // 3. Get the logger instance
    auto logger = std::move(result.value());

    // 4. Log messages at different levels
    logger->log(log_level::info, "Application started");
    logger->log(log_level::debug, "Debug information");
    logger->log(log_level::warn, "Warning: something might be wrong");
    logger->log(log_level::error, "Error occurred");

    // 5. Flush before exit
    logger->flush();

    std::cout << "Logging completed! Check app.log for file output.\n";
    return 0;
}
```

### Building Your Application

Add to your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)
project(my_app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find logger_system
find_package(LoggerSystem REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE LoggerSystem::logger)
```

Or using FetchContent:

```cmake
include(FetchContent)

FetchContent_Declare(
    common_system
    GIT_REPOSITORY https://github.com/kcenon/common_system.git
    GIT_TAG main
)

FetchContent_Declare(
    logger_system
    GIT_REPOSITORY https://github.com/kcenon/logger_system.git
    GIT_TAG main
)

FetchContent_MakeAvailable(common_system logger_system)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE kcenon::logger)
```

---

## Key Concepts

### Logger Builder
The recommended way to create loggers with validation.

```cpp
auto logger = logger_builder()
    .use_template("production")  // Use predefined template
    .with_min_level(log_level::info)
    .build()
    .value();
```

### Writers
Output destinations for log messages.

```cpp
// Console output with colors
.add_writer("console", std::make_unique<console_writer>())

// File output
.add_writer("file", std::make_unique<file_writer>("app.log"))

// Rotating file (10MB per file, keep 5 files)
.add_writer("rotating", std::make_unique<rotating_file_writer>(
    "app.log", 10 * 1024 * 1024, 5))
```

### Log Levels
Standard severity levels.

```cpp
logger->log(log_level::trace, "Detailed trace info");
logger->log(log_level::debug, "Debug information");
logger->log(log_level::info, "General information");
logger->log(log_level::warn, "Warning message");
logger->log(log_level::error, "Error occurred");
logger->log(log_level::fatal, "Fatal error - system shutting down");
```

---

## Configuration Templates

### Production

```cpp
auto logger = logger_builder()
    .use_template("production")
    .build()
    .value();
```

Optimized for production environments with:
- Async logging enabled
- Batched processing
- Info level minimum

### Debug

```cpp
auto logger = logger_builder()
    .use_template("debug")
    .build()
    .value();
```

Ideal for development with:
- Immediate output
- Debug level minimum
- Colored console output

### High Performance

```cpp
auto logger = logger_builder()
    .use_template("high_performance")
    .build()
    .value();
```

Maximum throughput with:
- Large buffer sizes
- Aggressive batching
- Optimized for high message volume

---

## CMake Configuration

### Core Build Options

```bash
# Standalone mode (default, no thread_system required)
cmake -DLOGGER_STANDALONE_MODE=ON ..

# With thread_system integration
cmake -DLOGGER_USE_THREAD_SYSTEM=ON ..

# Enable tests
cmake -DBUILD_TESTS=ON ..

# Enable examples
cmake -DBUILD_EXAMPLES=ON ..
```

### Feature Flags

| Option | Default | Description |
|--------|---------|-------------|
| `LOGGER_USE_THREAD_SYSTEM` | `OFF` | Enable optional thread_system integration |
| `LOGGER_STANDALONE_MODE` | `ON` | Use standalone std::jthread-based async worker |
| `BUILD_TESTS` | `ON` | Build test suite |
| `BUILD_EXAMPLES` | `ON` | Build example applications |

---

## thread_system Integration (Optional)

For advanced async logging with thread pool support:

### Enable at Build Time

```bash
cmake -DLOGGER_USE_THREAD_SYSTEM=ON ..
cmake --build .
```

### Enable at Runtime

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>

using namespace kcenon::logger::integration;

// Enable thread_pool backend
thread_system_integration::enable();

// Create logger (will use thread_pool if enabled)
auto logger = logger_builder()
    .use_template("production")
    .build()
    .value();

// ... use logger ...

// Disable when done
thread_system_integration::disable();
```

See [thread_system Integration Guide](../integration/THREAD_SYSTEM.md) for details.

---

## Common Patterns

### Error Handling

```cpp
auto result = logger_builder()
    .use_template("production")
    .build();

if (result.is_err()) {
    std::cerr << "Logger creation failed: "
              << result.error().message << "\n";
    return 1;
}

auto logger = std::move(result.value());
```

### Multiple Writers

```cpp
auto logger = logger_builder()
    .add_writer("console", std::make_unique<console_writer>())
    .add_writer("file", std::make_unique<file_writer>("app.log"))
    .add_writer("errors", std::make_unique<file_writer>("errors.log"))
    .build()
    .value();
```

### Graceful Shutdown

```cpp
// Ensure all pending logs are written
logger->flush();

// Logger will be properly cleaned up when it goes out of scope
```

---

## Next Steps

- **[Getting Started Guide](GETTING_STARTED.md)** - Detailed setup instructions
- **[Best Practices](BEST_PRACTICES.md)** - Recommended patterns and practices
- **[Migration Guide](MIGRATION_GUIDE.md)** - Migrating from other loggers or older versions
- **[Performance Guide](PERFORMANCE.md)** - Optimization tips and tuning
- **[API Reference](../API_REFERENCE.md)** - Complete API documentation
- **[Examples](../../examples/)** - More sample applications

---

## Troubleshooting

### Common Issues

**Build fails with C++20 errors:**
```bash
# Ensure you have a compatible compiler
g++ --version  # Should be 11+
clang++ --version  # Should be 14+
```

**common_system not found:**
```bash
# Ensure common_system is cloned in the same parent directory
ls ../common_system  # Should show common_system files
```

**vcpkg installation fails:**
```bash
rm -rf vcpkg
./scripts/dependency.sh
```

**Tests fail to run:**
```bash
cd build && ctest --verbose
```

For more troubleshooting help, see [FAQ](FAQ.md).

---

*Last Updated: 2025-12-14*
