[![CI](https://github.com/kcenon/logger_system/actions/workflows/ci.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/ci.yml)
[![Sanitizers](https://github.com/kcenon/logger_system/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/sanitizers.yml)
[![Benchmarks](https://github.com/kcenon/logger_system/actions/workflows/benchmarks.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/benchmarks.yml)
[![Code Coverage](https://github.com/kcenon/logger_system/actions/workflows/coverage.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/coverage.yml)
[![Static Analysis](https://github.com/kcenon/logger_system/actions/workflows/static-analysis.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/static-analysis.yml)
[![Docs](https://github.com/kcenon/logger_system/actions/workflows/build-Doxygen.yaml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/build-Doxygen.yaml)

# Logger System

> **Language:** **English** | [한국어](README_KO.md)

## Overview

A production-ready, high-performance C++20 asynchronous logging framework designed for multithreaded applications. Built with a modular, interface-based architecture and seamless ecosystem integration.

**Key Features**:
- 🚀 **Ultra-fast async logging**: 4.34M messages/second, 148ns latency
- 🔒 **Thread-safe by design**: Zero data races, production-proven
- 🏗️ **Modular architecture**: Interface-driven, pluggable components
- 🛡️ **Production-grade**: Comprehensive CI/CD, sanitizers, benchmarks
- 🔐 **Security-first**: Path validation, secure storage, audit logging
- 🌐 **Cross-platform**: Windows, Linux, macOS with GCC, Clang, MSVC

---

## Quick Start

### Basic Example

```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>

int main() {
    // Create logger using builder pattern with automatic validation
    auto result = kcenon::logger::logger_builder()
        .use_template("production")  // Predefined configuration
        .with_min_level(kcenon::logger::log_level::info)
        .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
        .add_writer("file", std::make_unique<kcenon::logger::file_writer>("app.log"))
        .build();

    if (result.is_err()) {
        const auto& err = result.error();
        std::cerr << "Failed to create logger: " << err.message
                  << " (code: " << err.code << ")\n";
        return -1;
    }

    auto logger = std::move(result.value());

    // Log messages with error handling
    logger->log(kcenon::logger::log_level::info, "Application started");
logger->log(kcenon::logger::log_level::error, "Something went wrong");

return 0;
}
```

Need a quick reminder later? See the [Result Handling Cheatsheet](docs/guides/INTEGRATION.md#result-handling-cheatsheet) for canonical snippets that can be reused across the ecosystem.

### Installation

**Using CMake**:
```bash
mkdir build && cd build
cmake ..
cmake --build .
cmake --build . --target install
```

**Using in Your Project**:
```cmake
find_package(LoggerSystem REQUIRED)
target_link_libraries(your_app PRIVATE LoggerSystem::logger)
```

### Prerequisites

- **CMake**: 3.20+
- **C++ Compiler**: GCC 11+, Clang 14+, MSVC 2022+, Apple Clang 14+
- **C++ Standard**: C++20
- **Dependencies**: fmt library (header-only mode available)

---

## Core Features

### Asynchronous Logging
- **Non-blocking operations**: Background thread handles I/O without blocking
- **Batched processing**: Processes multiple log entries efficiently
- **Adaptive batching**: Intelligent optimization based on queue utilization
- **Zero-copy design**: Minimal allocations and overhead

### Multiple Writer Types
- **Console Writer**: ANSI colored output for different log levels
- **File Writer**: Buffered file output with configurable settings
- **Rotating File Writer**: Size/time-based rotation with compression
- **Network Writer**: TCP/UDP remote logging
- **Critical Writer**: Synchronous logging for critical messages
- **Hybrid Writer**: Automatic async/sync switching based on log level

[📚 Detailed Writer Documentation →](docs/FEATURES.md#writer-types)

### Security Features (v3.0.0)
- **Secure Key Storage**: RAII-based encryption key management with automatic cleanup
- **Path Validation**: Protection against path traversal attacks
- **Signal Handler Safety**: Emergency flush for crash scenarios
- **Security Audit Logging**: Tamper-evident audit trail with HMAC-SHA256
- **Compliance Support**: GDPR, PCI DSS, ISO 27001, SOC 2

[🔒 Complete Security Guide →](docs/FEATURES.md#security-features)

---

## Performance Highlights

*Benchmarked on Apple M1 (8-core) @ 3.2GHz, 16GB, macOS Sonoma*

### Throughput

| Configuration | Throughput | vs spdlog |
|---------------|------------|-----------|
| **Single thread (async)** | **4.34M msg/s** | -19% |
| **4 threads** | **1.07M msg/s** | **+36%** |
| **8 threads** | **412K msg/s** | **+72%** |
| **16 threads** | **390K msg/s** | **+117%** |

### Latency

| Metric | Logger System | spdlog async | Advantage |
|--------|---------------|--------------|-----------|
| **Average** | **148 ns** | 2,325 ns | **15.7x faster** |
| **p99** | **312 ns** | 4,850 ns | **15.5x faster** |
| **p99.9** | **487 ns** | ~7,000 ns | **14.4x faster** |

### Memory Efficiency

- **Baseline**: 1.8 MB (vs spdlog: 4.2 MB, **57% less**)
- **Peak**: 2.4 MB
- **Allocations/msg**: 0.12

**Key Insights**:
- 🏃 **Multi-threaded advantage**: Adaptive batching provides superior scaling
- ⏱️ **Ultra-low latency**: Industry-leading 148ns average enqueue time
- 💾 **Memory efficient**: Minimal footprint with zero-copy design

[⚡ Full Benchmarks & Methodology →](docs/BENCHMARKS.md)

---

## Architecture Overview

### Modular Design

```
┌─────────────────────────────────────────────────────────────┐
│                      Logger Core                            │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │   Builder    │  │  Async Queue │  │   Metrics    │      │
│  └──────────────┘  └──────────────┘  └──────────────┘      │
└───────────────────────┬─────────────────────────────────────┘
                        │
        ┌───────────────┼───────────────┐
        │               │               │
        ▼               ▼               ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────┐
│   Writers    │ │   Filters    │ │  Formatters  │
│              │ │              │ │              │
│ • Console    │ │ • Level      │ │ • Plain Text │
│ • File       │ │ • Regex      │ │ • JSON       │
│ • Rotating   │ │ • Function   │ │ • Logfmt     │
│ • Network    │ │ • Composite  │ │ • Custom     │
│ • Critical   │ │              │ │              │
│ • Hybrid     │ │              │ │              │
└──────────────┘ └──────────────┘ └──────────────┘
```

### Key Components

- **Logger Core**: Main async processing engine with builder pattern
- **Writers**: Pluggable output destinations (file, console, network, etc.)
- **Filters**: Conditional logging based on level, pattern, or custom logic
- **Formatters**: Configurable output formats (plain, JSON, logfmt, custom)
- **Security**: Path validation, secure storage, audit logging

[🏛️ Detailed Architecture Guide →](docs/01-architecture.md)

---

## Ecosystem Integration

Part of a modular C++ ecosystem with clean interface boundaries:

### Dependencies

**Required**:
- **[common_system](https://github.com/kcenon/common_system)**: Core interfaces (ILogger, IMonitor, Result<T>)
- **[thread_system](https://github.com/kcenon/thread_system)**: Threading primitives

**Optional**:
- **[monitoring_system](https://github.com/kcenon/monitoring_system)**: Metrics and health monitoring

### Integration Pattern

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/thread/interfaces/service_container.h>

int main() {
    // 1. Create logger
    auto logger = kcenon::logger::logger_builder()
        .use_template("production")
        .build()
        .value();

    // 2. Register in service container (optional, for ecosystem integration)
    kcenon::thread::service_container::global()
        .register_singleton<kcenon::thread::logger_interface>(logger);

    // 3. Use logger anywhere in your application
    logger->log(kcenon::logger::log_level::info, "System initialized");

    return 0;
}
```

**Benefits**:
- Interface-only dependencies (no circular references)
- Independent compilation and deployment
- Runtime component injection via DI pattern
- Clean separation of concerns

[🔗 Ecosystem Integration Guide →](docs/guides/INTEGRATION.md)

---

## Documentation

### Getting Started
- 📖 [Getting Started Guide](docs/guides/GETTING_STARTED.md) - Step-by-step setup and basic usage
- 🚀 [Quick Start Examples](examples/basic_logging/) - Hands-on examples
- 🔧 [Build Guide](docs/guides/BUILD_GUIDE.md) - Detailed build instructions

### Core Documentation
- 📘 [Features](docs/FEATURES.md) - Comprehensive feature documentation
- 📊 [Benchmarks](docs/BENCHMARKS.md) - Performance analysis and comparisons
- 🏗️ [Architecture](docs/01-architecture.md) - System design and internals
- 📋 [Project Structure](docs/PROJECT_STRUCTURE.md) - Directory organization and files
- 🔧 [API Reference](docs/02-API_REFERENCE.md) - Complete API documentation

### Advanced Topics
- ⚡ [Performance Guide](docs/guides/PERFORMANCE.md) - Optimization tips and tuning
- 🔒 [Security Guide](docs/SECURITY.md) - Security considerations and best practices
- ✅ [Production Quality](docs/PRODUCTION_QUALITY.md) - CI/CD, testing, quality metrics
- 🎨 [Custom Writers](docs/advanced/CUSTOM_WRITERS.md) - Creating custom log writers
- 🔄 [Integration Guide](docs/guides/INTEGRATION.md) - Ecosystem integration patterns

### Development
- 🤝 [Contributing Guide](docs/CONTRIBUTING.md) - How to contribute
- 📋 [FAQ](docs/guides/FAQ.md) - Frequently asked questions
- 🔍 [Troubleshooting](docs/guides/TROUBLESHOOTING.md) - Common issues and solutions
- 📝 [Changelog](docs/CHANGELOG.md) - Release history and changes

---

## Configuration Templates

The logger system provides predefined templates for common scenarios:

```cpp
// Production: Optimized for production environments
auto logger = kcenon::logger::logger_builder()
    .use_template("production")
    .build()
    .value();

// Debug: Immediate output for development
auto logger = kcenon::logger::logger_builder()
    .use_template("debug")
    .build()
    .value();

// High-performance: Maximized throughput
auto logger = kcenon::logger::logger_builder()
    .use_template("high_performance")
    .build()
    .value();

// Low-latency: Minimized latency for real-time systems
auto logger = kcenon::logger::logger_builder()
    .use_template("low_latency")
    .build()
    .value();
```

### Advanced Configuration

```cpp
auto logger = kcenon::logger::logger_builder()
    // Core settings
    .with_min_level(kcenon::logger::log_level::info)
    .with_buffer_size(16384)
    .with_batch_size(200)
    .with_queue_size(20000)

    // Add multiple writers
    .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
    .add_writer("file", std::make_unique<kcenon::logger::rotating_file_writer>(
        "app.log",
        10 * 1024 * 1024,  // 10MB per file
        5                   // Keep 5 files
    ))

    // Build with validation
    .build()
    .value();
```

[📚 Complete Configuration Guide →](docs/guides/CONFIGURATION.md)

---

## Build Configuration

### CMake Feature Flags

```bash
# Core Features
cmake -DLOGGER_USE_DI=ON              # Dependency injection (default: ON)
cmake -DLOGGER_USE_MONITORING=ON      # Monitoring support (default: ON)
cmake -DLOGGER_ENABLE_ASYNC=ON        # Async logging (default: ON)
cmake -DLOGGER_ENABLE_CRASH_HANDLER=ON # Crash handler (default: ON)

# Advanced Features
cmake -DLOGGER_ENABLE_STRUCTURED_LOGGING=ON # JSON logging (default: OFF)
cmake -DLOGGER_ENABLE_NETWORK_WRITER=ON # Network writer (default: OFF)
cmake -DLOGGER_ENABLE_FILE_ROTATION=ON  # File rotation (default: ON)

# Performance Tuning
cmake -DLOGGER_DEFAULT_BUFFER_SIZE=16384 # Buffer size in bytes
cmake -DLOGGER_DEFAULT_BATCH_SIZE=200    # Batch processing size
cmake -DLOGGER_DEFAULT_QUEUE_SIZE=20000  # Maximum queue size

# Quality Assurance
cmake -DLOGGER_ENABLE_SANITIZERS=ON   # Enable sanitizers
cmake -DLOGGER_ENABLE_COVERAGE=ON     # Code coverage
cmake -DLOGGER_WARNINGS_AS_ERRORS=ON  # Treat warnings as errors
```

[🔧 Complete Build Options →](docs/guides/BUILD_GUIDE.md)

---

## Platform Support

### Officially Supported

| Platform | Architecture | Compilers | Status |
|----------|--------------|-----------|--------|
| **Ubuntu 22.04+** | x86_64, ARM64 | GCC 11+, Clang 14+ | ✅ Fully tested |
| **macOS Sonoma+** | x86_64, ARM64 (M1/M2) | Apple Clang 14+ | ✅ Fully tested |
| **Windows 11** | x86_64 | MSVC 2022 | ✅ Fully tested |

**Minimum Requirements**:
- C++20 compiler
- CMake 3.20+
- fmt library

[🖥️ Platform Details →](docs/PRODUCTION_QUALITY.md#platform-support)

---

## Testing

The logger system includes comprehensive testing infrastructure:

### Test Coverage

- **Unit Tests**: 150+ test cases (GTest)
- **Integration Tests**: 30+ scenarios
- **Benchmarks**: 20+ performance tests
- **Coverage**: ~65% (growing)

### Running Tests

```bash
# Build with tests
cmake -DBUILD_TESTS=ON ..
cmake --build .

# Run all tests
ctest --output-on-failure

# Run specific test suite
./build/bin/core_tests
./build/bin/writer_tests

# Run benchmarks
./build/bin/benchmarks
```

### CI/CD Status

All pipelines green:
- ✅ Multi-platform builds (Ubuntu, macOS, Windows)
- ✅ Sanitizers (Thread, Address, UB)
- ✅ Performance benchmarks
- ✅ Code coverage
- ✅ Static analysis (clang-tidy, cppcheck)

[✅ Production Quality Metrics →](docs/PRODUCTION_QUALITY.md)

---

## Contributing

We welcome contributions! Please see our [Contributing Guide](docs/CONTRIBUTING.md) for details.

### Development Workflow

1. Fork the repository
2. Create your feature branch: `git checkout -b feature/amazing-feature`
3. Commit your changes: `git commit -m 'Add amazing feature'`
4. Push to the branch: `git push origin feature/amazing-feature`
5. Open a Pull Request

### Code Standards

- Follow modern C++ best practices
- Use RAII and smart pointers
- Write comprehensive unit tests
- Maintain consistent formatting (clang-format)
- Document public APIs

[🤝 Contributing Guidelines →](docs/CONTRIBUTING.md)

---

## Support

- **Issues**: [GitHub Issues](https://github.com/kcenon/logger_system/issues)
- **Discussions**: [GitHub Discussions](https://github.com/kcenon/logger_system/discussions)
- **Email**: kcenon@naver.com

---

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- Inspired by modern logging frameworks (spdlog, Boost.Log, glog)
- Built with C++20 features (GCC 11+, Clang 14+, MSVC 2022+) for maximum performance and safety
- Maintained by kcenon@naver.com

---

<p align="center">
  Made with ❤️ by 🍀☀🌕🌥 🌊
</p>
