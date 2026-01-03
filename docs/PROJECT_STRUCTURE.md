# Logger System Project Structure

**Last Updated**: 2025-11-15
**Version**: 3.0.0

This document provides detailed descriptions of the logger system's directory structure, key files, and module dependencies.

---

## Table of Contents

- [Directory Overview](#directory-overview)
- [Core Module Files](#core-module-files)
- [Writer Implementations](#writer-implementations)
- [Filter Implementations](#filter-implementations)
- [Formatter Implementations](#formatter-implementations)
- [Configuration System](#configuration-system)
- [Security Components](#security-components)
- [Testing Organization](#testing-organization)
- [Build System](#build-system)
- [Module Dependencies](#module-dependencies)

---

## Directory Overview

```
logger_system/
├── 📁 include/kcenon/logger/       # Public headers (API)
│   ├── 📁 core/                    # Core logging components
│   ├── 📁 interfaces/              # Abstract interfaces
│   ├── 📁 writers/                 # Log writer implementations
│   ├── 📁 filters/                 # Log filter implementations
│   ├── 📁 formatters/              # Log formatter implementations
│   ├── 📁 config/                  # Configuration and templates
│   ├── 📁 security/                # Security features
│   ├── 📁 safety/                  # Crash safety mechanisms
│   ├── 📁 structured/              # Structured logging
│   ├── 📁 routing/                 # Log routing system
│   ├── 📁 analysis/                # Real-time log analysis
│   └── 📁 server/                  # Network log server
├── 📁 src/                         # Implementation files (.cpp)
│   ├── 📁 core/                    # Core implementations
│   ├── 📁 modules/                 # C++20 module files (.cppm)
│   │   ├── logger.cppm             # Primary module interface
│   │   ├── core.cppm               # Core partition
│   │   ├── backends.cppm           # Backends partition
│   │   └── analysis.cppm           # Analysis partition
│   ├── 📁 writers/                 # Writer implementations
│   ├── 📁 filters/                 # Filter implementations
│   ├── 📁 formatters/              # Formatter implementations
│   ├── 📁 config/                  # Configuration implementations
│   ├── 📁 security/                # Security implementations
│   ├── 📁 safety/                  # Safety implementations
│   ├── 📁 structured/              # Structured logging impl
│   ├── 📁 routing/                 # Routing implementations
│   ├── 📁 analysis/                # Analysis implementations
│   └── 📁 server/                  # Server implementations
├── 📁 examples/                    # Example applications
│   ├── 📁 basic_logging/           # Basic usage examples
│   ├── 📁 advanced_features/       # Advanced feature demos
│   ├── 📁 performance_test/        # Performance benchmarks
│   └── 📁 integration_examples/    # Ecosystem integration
├── 📁 tests/                       # All tests
│   ├── 📁 unit/                    # Unit tests (GTest)
│   │   ├── core_tests/             # Core component tests
│   │   ├── writer_tests/           # Writer tests
│   │   ├── filter_tests/           # Filter tests
│   │   ├── formatter_tests/        # Formatter tests
│   │   └── config_tests/           # Configuration tests
│   ├── 📁 integration/             # Integration tests
│   │   ├── ecosystem_tests/        # Ecosystem integration
│   │   ├── performance_tests/      # Performance validation
│   │   └── security_tests/         # Security validation
│   └── 📁 benchmarks/              # Performance benchmarks
│       ├── throughput_benchmarks/  # Throughput tests
│       ├── latency_benchmarks/     # Latency tests
│       └── memory_benchmarks/      # Memory profiling
├── 📁 docs/                        # Documentation
│   ├── 📁 guides/                  # User guides
│   ├── 📁 advanced/                # Advanced topics
│   ├── 📁 performance/             # Performance docs
│   └── 📁 contributing/            # Contribution guides
├── 📁 cmake/                       # CMake modules
│   ├── LoggerSystemConfig.cmake.in # Package config template
│   ├── FindThreadSystem.cmake      # Thread system finder
│   └── CompilerWarnings.cmake      # Compiler warning flags
├── 📁 .github/                     # GitHub workflows
│   └── 📁 workflows/               # CI/CD pipelines
│       ├── ci.yml                  # Main CI pipeline
│       ├── sanitizers.yml          # Sanitizer tests
│       ├── benchmarks.yml          # Performance benchmarks
│       ├── coverage.yml            # Code coverage
│       └── static-analysis.yml     # Static analysis
├── 📄 CMakeLists.txt               # Root build configuration
├── 📄 vcpkg.json                   # Dependencies manifest
├── 📄 LICENSE                      # BSD 3-Clause License
├── 📄 README.md                    # Main documentation
└── 📄 README_KO.md                 # Korean documentation
```

---

## Core Module Files

### logger.h / logger.cpp
**Location**: `include/kcenon/logger/core/` | `src/core/`

**Purpose**: Main logger class with asynchronous processing

**Key Components**:
- `logger` class: Primary logging interface
- Asynchronous queue management
- Writer coordination
- Filter application
- Metrics collection

**Public API**:
```cpp
class logger {
public:
    // Logging operations
    auto log(log_level level, const std::string& message) -> void;
    auto log(log_level level, const std::string& message,
             const std::string& file, int line, const std::string& func) -> void;

    // Writer management
    auto add_writer(const std::string& name,
                    std::unique_ptr<log_writer_interface> writer) -> common::VoidResult;
    auto remove_writer(const std::string& name) -> common::VoidResult;

    // Configuration
    auto set_filter(std::unique_ptr<log_filter_interface> filter) -> common::VoidResult;
    auto set_min_level(log_level level) -> void;

    // Control
    auto start() -> common::VoidResult;
    auto stop() -> common::VoidResult;
    auto flush() -> common::VoidResult;

    // Metrics
    auto enable_metrics_collection(bool enabled) -> void;
    auto get_current_metrics() const -> metrics_data;
};
```

**Dependencies**:
- `log_entry.h` - Log entry data structure
- `log_writer_interface.h` - Writer abstraction
- `log_filter_interface.h` - Filter abstraction
- Thread system queue (optional)

---

### logger_builder.h / logger_builder.cpp
**Location**: `include/kcenon/logger/core/` | `src/core/`

**Purpose**: Builder pattern for logger configuration with validation

**Key Features**:
- Fluent API design
- Automatic validation
- Template support (production, debug, high_performance, low_latency)
- Error handling with Result<T>

**Public API**:
```cpp
class logger_builder {
public:
    // Template configuration
    auto use_template(const std::string& template_name) -> logger_builder&;

    // Basic configuration
    auto with_min_level(log_level level) -> logger_builder&;
    auto with_buffer_size(size_t size) -> logger_builder&;
    auto with_batch_size(size_t size) -> logger_builder&;
    auto with_queue_size(size_t size) -> logger_builder&;

    // Component addition
    auto add_writer(const std::string& name,
                    std::unique_ptr<log_writer_interface> writer) -> logger_builder&;
    auto add_filter(std::unique_ptr<log_filter_interface> filter) -> logger_builder&;

    // Build
    auto build() -> result<std::unique_ptr<logger>>;

private:
    auto validate() const -> common::VoidResult;
};
```

**Dependencies**:
- `logger.h` - Logger class
- `config_validator.h` - Configuration validation
- `config_templates.h` - Predefined templates

---

### log_entry.h
**Location**: `include/kcenon/logger/core/`

**Purpose**: Data structure for log entries with metadata

**Structure**:
```cpp
struct log_entry {
    log_level level;                                    // Log severity level
    std::string message;                                // Log message
    std::chrono::system_clock::time_point timestamp;    // When logged
    std::string file;                                   // Source file
    int line;                                           // Line number
    std::string function;                               // Function name
    std::thread::id thread_id;                          // Thread ID
    std::map<std::string, std::string> context;         // Additional context
};
```

**Features**:
- Lightweight and copyable
- Complete metadata capture
- Extensible context fields
- Thread-safe by design

---

### result_types.h
**Location**: `include/kcenon/logger/core/`

**Purpose**: Error handling types and utilities

**Key Types**:
```cpp
// Result type for operations returning values
template<typename T>
class result {
public:
    bool is_ok() const;
    bool is_err() const;
    const T& value() const;
    const error_info& error() const;

    // Note: boolean conversion is removed in common::Result
};

// Result type for void operations (unified with common_system)
using VoidResult = common::VoidResult;  // common::Result<std::monostate>

// For checking void results:
if (result.is_ok()) { /* success */ }
if (result.is_err()) { /* error: result.error() */ }

// Error information
struct error_info {
    error_code code;
    std::string message;
    std::string context;
};
```

**Error Codes** (Range: -200 to -299):
- System lifecycle: -200 to -209
- Writer management: -210 to -219
- Configuration: -220 to -229
- I/O operations: -230 to -239

---

## Writer Implementations

### console_writer.h / console_writer.cpp
**Location**: `include/kcenon/logger/writers/` | `src/writers/`

**Purpose**: Colored console output with ANSI support

**Features**:
- ANSI color codes for different log levels
- Cross-platform support (Windows, Linux, macOS)
- Configurable color schemes
- Thread-safe output

**Configuration**:
```cpp
struct console_writer_config {
    bool colors_enabled = true;
    bool stderr_for_errors = true;
    std::map<log_level, std::string> color_map;
};
```

---

### file_writer.h / file_writer.cpp
**Location**: `include/kcenon/logger/writers/` | `src/writers/`

**Purpose**: Basic file writing with buffering

**Features**:
- Buffered I/O for performance
- Automatic directory creation
- Configurable file permissions (default: 0600)
- Thread-safe file access

**Configuration**:
```cpp
struct file_writer_config {
    std::string filename;
    size_t buffer_size = 8192;           // 8KB default
    bool append = true;
    mode_t file_permissions = 0600;      // Owner read/write only
    std::chrono::milliseconds flush_interval = std::chrono::milliseconds(100);
};
```

---

### rotating_file_writer.h / rotating_file_writer.cpp
**Location**: `include/kcenon/logger/writers/` | `src/writers/`

**Purpose**: Size and time-based file rotation

**Features**:
- Size-based rotation
- Time-based rotation (daily, hourly, weekly)
- Backup file management
- Optional compression (gzip, bzip2)

**Configuration**:
```cpp
enum class rotation_type {
    size,       // Rotate based on file size
    daily,      // Rotate daily
    hourly,     // Rotate hourly
    weekly      // Rotate weekly
};

struct rotating_file_writer_config {
    std::string base_filename;
    rotation_type type = rotation_type::size;
    size_t max_size_bytes = 10 * 1024 * 1024;  // 10MB
    size_t max_files = 5;                       // Keep 5 backups
    compression_type compression = compression_type::none;
    int rotation_hour = 0;                      // For time-based rotation
};
```

---

### network_writer.h / network_writer.cpp
**Location**: `include/kcenon/logger/writers/` | `src/writers/`

**Purpose**: TCP/UDP network logging

**Features**:
- TCP and UDP protocol support
- Automatic reconnection
- Configurable retry strategies
- Message batching

**Configuration**:
```cpp
enum class protocol_type {
    tcp,
    udp
};

struct network_writer_config {
    std::string host;
    uint16_t port;
    protocol_type protocol = protocol_type::tcp;
    std::chrono::seconds connection_timeout = std::chrono::seconds(5);
    int max_retry_attempts = 3;
    std::chrono::milliseconds retry_backoff = std::chrono::milliseconds(100);
    size_t batch_size = 100;
};
```

---

### critical_writer.h / critical_writer.cpp
**Location**: `include/kcenon/logger/writers/` | `src/writers/`

**Purpose**: Synchronous logging for critical messages

**Features**:
- Bypasses async queue
- Immediate write guarantee
- Wrapper pattern (wraps any writer)
- Crash-safe

**Usage Pattern**:
```cpp
auto critical = std::make_unique<critical_writer>(
    std::make_unique<file_writer>("critical.log")
);
```

---

### hybrid_writer.h / hybrid_writer.cpp
**Location**: `include/kcenon/logger/writers/` | `src/writers/`

**Purpose**: Adaptive async/sync based on log level

**Features**:
- Automatic mode switching
- Configurable threshold level
- Separate async queue
- Performance + reliability

**Configuration**:
```cpp
struct hybrid_writer_config {
    log_level sync_level = log_level::error;  // Errors and above sync
    size_t async_queue_size = 10000;
    std::chrono::milliseconds flush_interval = std::chrono::milliseconds(100);
};
```

---

## Filter Implementations

### level_filter.h / level_filter.cpp
**Location**: `include/kcenon/logger/filters/` | `src/filters/`

**Purpose**: Filter by minimum log level

---

### regex_filter.h / regex_filter.cpp
**Location**: `include/kcenon/logger/filters/` | `src/filters/`

**Purpose**: Pattern matching on log messages

**Features**:
- Include or exclude patterns
- Case-sensitive or insensitive
- Multiple pattern support

---

### function_filter.h / function_filter.cpp
**Location**: `include/kcenon/logger/filters/` | `src/filters/`

**Purpose**: Custom filtering logic via lambda/function

---

## Formatter Implementations

### plain_formatter.h / plain_formatter.cpp
**Location**: `include/kcenon/logger/formatters/` | `src/formatters/`

**Purpose**: Simple, human-readable text format

---

### json_formatter.h / json_formatter.cpp
**Location**: `include/kcenon/logger/formatters/` | `src/formatters/`

**Purpose**: Machine-parseable JSON output

---

### custom_formatter.h / custom_formatter.cpp
**Location**: `include/kcenon/logger/formatters/` | `src/formatters/`

**Purpose**: Base class for custom formatting logic

---

## Configuration System

### config_templates.h / config_templates.cpp
**Location**: `include/kcenon/logger/config/` | `src/config/`

**Purpose**: Predefined configurations

**Templates**:
- `production`: Optimized for production environments
- `debug`: Immediate output for development
- `high_performance`: Maximized throughput
- `low_latency`: Minimized latency

---

### config_validator.h / config_validator.cpp
**Location**: `include/kcenon/logger/config/` | `src/config/`

**Purpose**: Comprehensive validation framework

**Validations**:
- Buffer size ranges
- Queue size limits
- Writer configuration
- Filter configuration

---

### config_strategy.h / config_strategy.cpp
**Location**: `include/kcenon/logger/config/` | `src/config/`

**Purpose**: Strategy pattern for flexible configuration

---

## Security Components

### secure_key_storage.h / secure_key_storage.cpp
**Location**: `include/kcenon/logger/security/` | `src/security/`

**Purpose**: RAII-based encryption key management

**Features**:
- OpenSSL `OPENSSL_cleanse()` for secure erasure
- File permission enforcement (0600)
- Cryptographically secure key generation

---

### path_validator.h / path_validator.cpp
**Location**: `include/kcenon/logger/security/` | `src/security/`

**Purpose**: Path validation and security

**Protections**:
- Path traversal prevention
- Symlink validation
- Base directory enforcement
- Filename character restrictions

---

### log_sanitizer.h / log_sanitizer.cpp
**Location**: `include/kcenon/logger/security/` | `src/security/`

**Purpose**: Sensitive data sanitization

**Features**:
- Email masking
- Credit card masking
- Token redaction
- Custom pattern support

---

## Testing Organization

### Unit Tests
**Location**: `tests/unit/`

**Structure**:
```
tests/unit/
├── core_tests/
│   ├── logger_test.cpp
│   ├── logger_builder_test.cpp
│   └── log_entry_test.cpp
├── writer_tests/
│   ├── console_writer_test.cpp
│   ├── file_writer_test.cpp
│   ├── rotating_file_writer_test.cpp
│   └── network_writer_test.cpp
├── filter_tests/
│   ├── level_filter_test.cpp
│   ├── regex_filter_test.cpp
│   └── function_filter_test.cpp
└── config_tests/
    ├── config_validator_test.cpp
    └── config_templates_test.cpp
```

**Testing Framework**: Google Test (GTest)
**Coverage Target**: 80%+

---

### Integration Tests
**Location**: `tests/integration/`

**Focus Areas**:
- Ecosystem integration (thread_system, monitoring_system)
- Performance validation
- Security validation
- Multi-writer scenarios

---

### Benchmarks
**Location**: `tests/benchmarks/`

**Benchmark Types**:
- Throughput benchmarks (single/multi-threaded)
- Latency benchmarks (percentile analysis)
- Memory benchmarks (footprint, allocation patterns)

---

## Build System

### CMakeLists.txt (Root)
**Location**: `CMakeLists.txt`

**Key Options**:
```cmake
# Core Features
option(LOGGER_USE_DI "Enable dependency injection" ON)
option(LOGGER_USE_MONITORING "Enable monitoring support" ON)
option(LOGGER_ENABLE_ASYNC "Enable async logging" ON)
option(LOGGER_ENABLE_CRASH_HANDLER "Enable crash handler" ON)

# Advanced Features
option(LOGGER_USE_LOCK_FREE_QUEUE "Use lock-free queue" OFF)
option(LOGGER_ENABLE_STRUCTURED_LOGGING "Enable JSON logging" OFF)
option(LOGGER_ENABLE_NETWORK_WRITER "Enable network writer" OFF)
option(LOGGER_ENABLE_FILE_ROTATION "Enable file rotation" ON)

# Performance Tuning
set(LOGGER_DEFAULT_BUFFER_SIZE 16384 CACHE STRING "Buffer size in bytes")
set(LOGGER_DEFAULT_BATCH_SIZE 200 CACHE STRING "Batch processing size")
set(LOGGER_DEFAULT_QUEUE_SIZE 20000 CACHE STRING "Maximum queue size")

# Quality Assurance
option(LOGGER_ENABLE_SANITIZERS "Enable sanitizers" OFF)
set(LOGGER_SANITIZER_TYPE "address" CACHE STRING "Sanitizer type")
option(LOGGER_ENABLE_WARNINGS "Enable compiler warnings" ON)
option(LOGGER_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(LOGGER_ENABLE_COVERAGE "Enable code coverage" OFF)
```

---

### CMake Modules
**Location**: `cmake/`

**Key Files**:
- `LoggerSystemConfig.cmake.in`: Package configuration template
- `FindThreadSystem.cmake`: Thread system finder
- `CompilerWarnings.cmake`: Compiler warning flags
- `Sanitizers.cmake`: Sanitizer configuration
- `CodeCoverage.cmake`: Coverage reporting

---

## Module Dependencies

### Dependency Graph

```
┌─────────────────┐
│  config module  │  (No dependencies)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   core module   │
└────────┬────────┘
         │
         ├──────────────┬──────────────┐
         │              │              │
         ▼              ▼              ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────┐
│   writers    │ │   filters    │ │  formatters  │
└──────────────┘ └──────────────┘ └──────────────┘
         │              │              │
         └──────────────┴──────────────┘
                        │
                        ▼
         ┌──────────────────────────────┐
         │  integration (optional)      │
         │  - thread_system             │
         │  - monitoring_system         │
         └──────────────────────────────┘
```

### External Dependencies

**Required**:
- **fmt**: String formatting library (header-only mode available)
- **C++17 compiler**: GCC 7+, Clang 5+, MSVC 2017+

**Optional**:
- **thread_system**: Threading primitives and logger interface
- **common_system**: Core interfaces (ILogger, IMonitor, Result<T>)
- **monitoring_system**: Metrics collection and health monitoring
- **OpenSSL 3.x** (recommended): For secure key storage and encryption support
  - OpenSSL 1.1.x is supported but deprecated (EOL September 2023)

---

## File Naming Conventions

### Header Files
- **Interface headers**: `*_interface.h` (e.g., `log_writer_interface.h`)
- **Implementation headers**: `*.h` (e.g., `logger.h`, `file_writer.h`)
- **Configuration headers**: `config_*.h` (e.g., `config_validator.h`)

### Source Files
- **Implementation**: `*.cpp` (matches header name)
- **Tests**: `*_test.cpp` (e.g., `logger_test.cpp`)
- **Benchmarks**: `*_benchmark.cpp` (e.g., `throughput_benchmark.cpp`)

### Documentation Files
- **Markdown**: `*.md` (English) or `*_KO.md` (Korean)
- **API docs**: `API_REFERENCE.md`
- **Guides**: `GUIDE_NAME.md` in `docs/guides/`

---

## See Also

- [Architecture Overview](01-architecture.md) - System design and architecture
- [API Reference](02-API_REFERENCE.md) - Complete API documentation
- [Build Guide](guides/BUILD_GUIDE.md) - Detailed build instructions
- [Contributing Guide](contributing/CONTRIBUTING.md) - Contribution guidelines
