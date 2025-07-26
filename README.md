# Logger System

[![CI](https://github.com/kcenon/logger_system/actions/workflows/ci.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

A high-performance, modular C++20 logging system with lock-free implementation designed for multithreaded applications.

## Features

- **Lock-free Implementation**: High-performance logging without mutex contention
- **Multiple Writers**: Console, file, and custom callback writers
- **Asynchronous Logging**: Non-blocking log operations
- **Thread-safe**: Designed for concurrent environments
- **Modular Design**: Easy integration with any C++ project
- **Low Latency**: Optimized for minimal overhead

## Integration with Thread System

This logger is designed to work seamlessly with the [Thread System](https://github.com/kcenon/thread_system) through dependency injection:

```cpp
#include <logger_system/logger.h>
#include <thread_system/interfaces/service_container.h>

// Register logger in the service container
auto logger = std::make_shared<logger_module::logger>();
logger->add_writer(std::make_unique<logger_module::console_writer>());

thread_module::service_container::global()
    .register_singleton<thread_module::logger_interface>(logger);

// Now thread system components will automatically use this logger
auto context = thread_module::thread_context(); // Will resolve logger from container
```

## Quick Start

### Basic Usage

```cpp
#include <logger_system/logger.h>

int main() {
    // Create logger instance
    auto logger = std::make_shared<logger_module::logger>();
    
    // Add console output
    logger->add_writer(std::make_unique<logger_module::console_writer>());
    
    // Add file output
    logger->add_writer(std::make_unique<logger_module::file_writer>("app.log"));
    
    // Log messages
    logger->log(log_level::info, "Application started");
    logger->log(log_level::error, "Something went wrong", __FILE__, __LINE__, __func__);
    
    return 0;
}
```

### Custom Writers

```cpp
class custom_writer : public logger_module::base_writer {
public:
    void write(const logger_module::log_entry& entry) override {
        // Custom implementation
    }
    
    void flush() override {
        // Flush implementation
    }
};
```

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Build Options

- `BUILD_TESTS`: Build unit tests (default: ON)
- `BUILD_BENCHMARKS`: Build performance benchmarks (default: OFF)
- `BUILD_SAMPLES`: Build example programs (default: ON)
- `USE_LOCKFREE`: Use lock-free implementation (default: ON)

## Installation

```bash
cmake --build . --target install
```

## CMake Integration

```cmake
find_package(LoggerSystem REQUIRED)
target_link_libraries(your_target PRIVATE LoggerSystem::logger)
```

## License

BSD 3-Clause License - see LICENSE file for details.