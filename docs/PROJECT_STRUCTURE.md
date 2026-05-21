---
doc_id: "LOG-PROJ-004"
doc_title: "Logger System Project Structure"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "PROJ"
---

# Logger System Project Structure

> **SSOT**: This document is the single source of truth for **Logger System Project Structure**.

**Last Updated**: 2026-02-08
**Version**: 0.4.0.0

This document provides detailed descriptions of the logger system's directory structure, key files, and module dependencies.

---

## Table of Contents

- [Directory Overview](#directory-overview)
- [Core Module Files](#core-module-files)
- [Builder Components](#builder-components)
- [Writer Implementations](#writer-implementations)
- [Filter Implementations](#filter-implementations)
- [Formatter Implementations](#formatter-implementations)
- [Factory Components](#factory-components)
- [Adapter Components](#adapter-components)
- [Backend Components](#backend-components)
- [Sink Components](#sink-components)
- [Integration Components](#integration-components)
- [Sampling Components](#sampling-components)
- [Utility Components](#utility-components)
- [Security Components](#security-components)
- [Additional Components](#additional-components)
- [Testing Organization](#testing-organization)
- [Build System](#build-system)
- [Module Dependencies](#module-dependencies)

---

## Directory Overview

**Header files**: 93 | **Source files**: 32 (.cpp) + 4 (.cppm)

```
logger_system/
├── 📁 include/kcenon/logger/       # Public headers (API) — 93 headers
│   ├── 📄 compatibility.h          # Backward compatibility support
│   ├── 📄 forward.h                # Forward declarations
│   ├── 📁 adapters/                # System adapter layers (3 headers)
│   ├── 📁 analysis/                # Real-time log analysis (2 headers)
│   ├── 📁 backends/                # Backend implementations (3 headers)
│   ├── 📁 builders/                # Builder pattern classes (1 header)
│   ├── 📁 core/                    # Core logging components (18 headers)
│   │   ├── 📁 metrics/             # Logger metrics (1 header)
│   │   └── 📁 strategies/          # Configuration strategies (5 headers)
│   ├── 📁 di/                      # Dependency injection (1 header)
│   ├── 📁 factories/               # Factory classes (3 headers)
│   ├── 📁 filters/                 # Log filter implementations (1 header)
│   ├── 📁 formatters/              # Log formatter implementations (5 headers)
│   ├── 📁 integration/             # Thread system integration (3 headers)
│   ├── 📁 interfaces/              # Abstract interfaces (8 headers)
│   ├── 📁 otlp/                    # OpenTelemetry support (1 header)
│   ├── 📁 routing/                 # Log routing system (1 header)
│   ├── 📁 safety/                  # Crash safety mechanisms (1 header)
│   ├── 📁 sampling/                # Log sampling (2 headers)
│   ├── 📁 security/                # Security features (6 headers)
│   ├── 📁 server/                  # Network log server (1 header)
│   ├── 📁 sinks/                   # Output sink implementations (2 headers)
│   ├── 📁 structured/              # Structured logging (1 header)
│   ├── 📁 utils/                   # Utility functions (4 headers)
│   └── 📁 writers/                 # Log writer implementations (18 headers)
├── 📁 src/                         # Implementation files — 32 .cpp + 4 .cppm
│   ├── 📁 core/                    # Core implementations (9 .cpp)
│   ├── 📁 impl/                    # Internal implementation details
│   │   ├── 📁 async/               # Async processing (3 .cpp + internal .h)
│   │   ├── 📁 builders/            # Builder implementations (1 .cpp)
│   │   ├── 📁 di/                  # DI container internals (internal .h)
│   │   ├── 📁 filters/             # Filter internals (internal .h)
│   │   ├── 📁 memory/              # Memory pool internals (internal .h)
│   │   ├── 📁 monitoring/          # Monitoring internals (internal .h)
│   │   └── 📁 writers/             # Writer implementations (14 .cpp)
│   ├── 📁 integration/             # Integration implementations (3 .cpp)
│   ├── 📁 modules/                 # C++20 module files (.cppm)
│   │   ├── logger.cppm             # Primary module interface
│   │   ├── core.cppm               # Core partition
│   │   ├── backends.cppm           # Backends partition
│   │   └── analysis.cppm           # Analysis partition
│   ├── 📁 sampling/                # Sampling implementations (1 .cpp)
│   └── 📁 security/                # Security implementations (1 .cpp)
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
└── 📄 README.kr.md                 # Korean documentation
```

---

## Core Module Files

The `core/` directory contains 18 headers and 9 source files, plus 2 subdirectories (`metrics/` and `strategies/`).

### Header Files (`include/kcenon/logger/core/`)

| File | Purpose |
|------|---------|
| `logger.h` | Main logger class with asynchronous processing |
| `logger_builder.h` | Builder pattern for logger configuration with validation |
| `logger_config.h` | Logger configuration data structures |
| `logger_config_builder.h` | Builder for logger configuration objects |
| `logger_context.h` | Logger context management |
| `logger_registry.h` | Global logger registry for named loggers |
| `log_collector.h` | Log collection and aggregation |
| `log_context.h` | Logging context data |
| `log_context_scope.h` | Scoped context management (RAII) |
| `error_codes.h` | Error code definitions and error handling types |
| `filtered_logger.h` | Logger with built-in filtering |
| `monitoring_integration_detector.h` | Auto-detection of monitoring system availability |
| `thread_integration_detector.h` | Auto-detection of thread system availability |
| `scoped_context_guard.h` | RAII guard for context scope |
| `signal_manager_context.h` | Signal manager context for crash handling |
| `small_string.h` | Small string optimization |
| `structured_log_builder.h` | Builder for structured log entries |
| `unified_log_context.h` | Unified log context across subsystems |

### Core Subdirectories

**`core/metrics/`** (1 header):
| File | Purpose |
|------|---------|
| `logger_metrics.h` | Metrics collection and reporting |

**`core/strategies/`** (5 headers):
| File | Purpose |
|------|---------|
| `config_strategy_interface.h` | Abstract interface for configuration strategies |
| `composite_strategy.h` | Composite pattern for combining strategies |
| `deployment_strategy.h` | Deployment-specific configuration |
| `environment_strategy.h` | Environment-based configuration |
| `performance_strategy.h` | Performance-tuned configuration |

### Source Files (`src/core/`)

| File | Purpose |
|------|---------|
| `logger.cpp` | Logger core implementation |
| `logger_context.cpp` | Logger context implementation |
| `logger_metrics.cpp` | Metrics collection implementation |
| `logger_registry.cpp` | Logger registry implementation |
| `log_collector.cpp` | Log collector implementation |
| `log_context_scope.cpp` | Context scope implementation |
| `scoped_context_guard.cpp` | Scoped guard implementation |
| `signal_manager_context.cpp` | Signal manager context implementation |
| `unified_log_context.cpp` | Unified log context implementation |

### Interfaces (`include/kcenon/logger/interfaces/`)

| File | Purpose |
|------|---------|
| `log_entry.h` | Log entry data structure with metadata |
| `log_filter_interface.h` | Abstract filter interface |
| `log_formatter_interface.h` | Abstract formatter interface |
| `log_sink_interface.h` | Abstract sink interface |
| `log_writer_interface.h` | Abstract writer interface |
| `logger_types.h` | Common logger type definitions |
| `output_sink_interface.h` | Abstract output sink interface |
| `writer_category.h` | Writer categorization definitions |

### Root-Level Headers (`include/kcenon/logger/`)

| File | Purpose |
|------|---------|
| `compatibility.h` | Backward compatibility support |
| `forward.h` | Forward declarations |

---

## Builder Components

### writer_builder.h / writer_builder.cpp
**Location**: `include/kcenon/logger/builders/` | `src/impl/builders/`

**Purpose**: Fluent builder for constructing writer pipelines

**Features**:
- Fluent API for composing writers with decorators
- Chainable methods for adding formatting, filtering, buffering
- Automatic writer pipeline assembly
- Type-safe builder pattern

---

## Writer Implementations

The `writers/` directory contains 18 headers with implementations in `src/impl/writers/` (14 .cpp files).

### Header Files (`include/kcenon/logger/writers/`)

| File | Purpose |
|------|---------|
| `base_writer.h` | Base class for all writer implementations |
| `console_writer.h` | Colored console output with ANSI support |
| `file_writer.h` | Basic file writing with buffering |
| `rotating_file_writer.h` | Size and time-based file rotation |
| `network_writer.h` | TCP/UDP network logging |
| `critical_writer.h` | Synchronous logging for critical messages |
| `async_writer.h` | Asynchronous writer with queue-based processing |
| `batch_writer.h` | Batched write operations for throughput |
| `buffered_writer.h` | Buffered I/O writer |
| `composite_writer.h` | Fan-out writer dispatching to multiple writers |
| `decorator_writer_base.h` | Base class for writer decorators |
| `encrypted_writer.h` | Encrypted log output |
| `filtered_writer.h` | Writer with built-in log filtering |
| `formatted_writer.h` | Writer with built-in log formatting |
| `legacy_writer_adapter.h` | Adapter for legacy writer interfaces |
| `otlp_writer.h` | OpenTelemetry Protocol (OTLP) log export |
| `queued_writer_base.h` | Base class for queue-backed writers |
| `thread_safe_writer.h` | Thread-safe writer wrapper |

### Source Files (`src/impl/writers/`)

| File | Purpose |
|------|---------|
| `base_writer.cpp` | Base writer implementation |
| `batch_writer.cpp` | Batch writer implementation |
| `buffered_writer.cpp` | Buffered writer implementation |
| `console_writer.cpp` | Console writer implementation |
| `critical_writer.cpp` | Critical writer implementation |
| `decorator_writer_base.cpp` | Decorator base implementation |
| `encrypted_writer.cpp` | Encrypted writer implementation |
| `file_writer.cpp` | File writer implementation |
| `filtered_writer.cpp` | Filtered writer implementation |
| `formatted_writer.cpp` | Formatted writer implementation |
| `network_writer.cpp` | Network writer implementation |
| `otlp_writer.cpp` | OTLP writer implementation |
| `rotating_file_writer.cpp` | Rotating file writer implementation |
| `thread_safe_writer.cpp` | Thread-safe writer implementation |

### Async Processing (`src/impl/async/`)

| File | Purpose |
|------|---------|
| `async_worker.h` / `.cpp` | Async worker thread management |
| `batch_processor.h` / `.cpp` | Batch processing engine |
| `high_performance_async_writer.h` / `.cpp` | High-performance async writer |
| `jthread_compat.h` | jthread compatibility layer |
| `lockfree_queue.h` | Lock-free queue for async operations |

---

## Filter Implementations

### log_filter.h
**Location**: `include/kcenon/logger/filters/`

**Purpose**: Unified log filter implementation supporting level-based, regex, and custom function filtering

**Internal Implementation**: `src/impl/filters/log_filter.h`

---

## Formatter Implementations

### Header Files (`include/kcenon/logger/formatters/`)

| File | Purpose |
|------|---------|
| `base_formatter.h` | Base class for all formatters |
| `json_formatter.h` | Machine-parseable JSON output |
| `logfmt_formatter.h` | Logfmt-style key=value output |
| `template_formatter.h` | Template-based custom formatting |
| `timestamp_formatter.h` | Timestamp formatting utilities |

---

## Factory Components

### Header Files (`include/kcenon/logger/factories/`)

| File | Purpose |
|------|---------|
| `writer_factory.h` | Factory for creating writer instances |
| `filter_factory.h` | Factory for creating filter instances |
| `formatter_factory.h` | Factory for creating formatter instances |

---

## Adapter Components

### Header Files (`include/kcenon/logger/adapters/`)

| File | Purpose |
|------|---------|
| `common_logger_adapter.h` | Adapter for common_system logger interface |
| `common_system_adapter.h` | Adapter for common_system integration |
| `logger_adapter.h` | Generic logger adapter interface |

---

## Backend Components

### Header Files (`include/kcenon/logger/backends/`)

| File | Purpose |
|------|---------|
| `integration_backend.h` | Backend for thread_system integration mode |
| `monitoring_backend.h` | Backend for monitoring_system integration |
| `standalone_backend.h` | Backend for standalone operation mode |

---

## Sink Components

### Header Files (`include/kcenon/logger/sinks/`)

| File | Purpose |
|------|---------|
| `console_sink.h` | Console output sink |
| `file_sink.h` | File output sink |

---

## Integration Components

### Header Files (`include/kcenon/logger/integration/`)

| File | Purpose |
|------|---------|
| `executor_integration.h` | Executor-based integration |
| `standalone_executor.h` | Standalone executor for independent operation |
| `thread_system_integration.h` | Thread system integration layer |

### Source Files (`src/integration/`)

| File | Purpose |
|------|---------|
| `executor_integration.cpp` | Executor integration implementation |
| `standalone_executor.cpp` | Standalone executor implementation |
| `thread_system_integration.cpp` | Thread system integration implementation |

---

## Sampling Components

### Header Files (`include/kcenon/logger/sampling/`)

| File | Purpose |
|------|---------|
| `log_sampler.h` | Log sampling engine |
| `sampling_config.h` | Sampling configuration and policies |

### Source Files (`src/sampling/`)

| File | Purpose |
|------|---------|
| `log_sampler.cpp` | Log sampler implementation |

---

## Utility Components

### Header Files (`include/kcenon/logger/utils/`)

| File | Purpose |
|------|---------|
| `error_handling_utils.h` | Error handling helper functions |
| `file_utils.h` | File system utility functions |
| `string_utils.h` | String manipulation utilities |
| `time_utils.h` | Time and timestamp utilities |

---

## Security Components

### Header Files (`include/kcenon/logger/security/`)

| File | Purpose |
|------|---------|
| `secure_key_storage.h` | RAII-based encryption key management (OpenSSL) |
| `path_validator.h` | Path validation and traversal prevention |
| `log_sanitizer.h` | Sensitive data sanitization (PII masking) |
| `audit_logger.h` | Audit trail logging for compliance |
| `signal_manager_interface.h` | Abstract signal manager interface |
| `signal_manager.h` | Signal and crash handler management |

### Source Files (`src/security/`)

| File | Purpose |
|------|---------|
| `signal_manager.cpp` | Signal manager implementation |

---

## Additional Components

### Dependency Injection (`include/kcenon/logger/di/`)

| File | Purpose |
|------|---------|
| `service_registration.h` | DI service registration utilities |

**DI Integration**: Uses `common_system::di::service_container` for dependency
injection. See `service_registration.h` for logger service registration utilities.

### OpenTelemetry (`include/kcenon/logger/otlp/`)

| File | Purpose |
|------|---------|
| `otel_context.h` | OpenTelemetry context integration |

### Routing (`include/kcenon/logger/routing/`)

| File | Purpose |
|------|---------|
| `log_router.h` | Log routing and dispatching |

### Safety (`include/kcenon/logger/safety/`)

| File | Purpose |
|------|---------|
| `crash_safe_logger.h` | Crash-safe logging mechanisms |

### Analysis (`include/kcenon/logger/analysis/`)

| File | Purpose |
|------|---------|
| `log_analyzer.h` | Log analysis engine |
| `realtime_log_analyzer.h` | Real-time log analysis |

### Structured Logging (`include/kcenon/logger/structured/`)

| File | Purpose |
|------|---------|
| `structured_logger.h` | Structured (JSON-based) logging |

### Server (`include/kcenon/logger/server/`)

| File | Purpose |
|------|---------|
| `log_server.h` | Network log server |

### Internal Implementation Details (`src/impl/`)

The `src/impl/` directory contains internal headers and implementations not part of the public API:

| Subdirectory | Contents |
|-------------|----------|
| `memory/` | `object_pool.h`, `log_entry_pool.h` - Memory pool implementations |
| `monitoring/` | `basic_monitor.h`, `monitoring_interface.h` |

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
┌─────────────────────┐
│   interfaces        │  (No dependencies)
└─────────┬───────────┘
          │
          ▼
┌─────────────────────┐
│   core module       │
│ (strategies,metrics)│
└─────────┬───────────┘
          │
  ┌───────┼──────────┬──────────────┐
  │       │          │              │
  ▼       ▼          ▼              ▼
┌──────┐┌──────┐┌──────────┐┌──────────┐
│write-││filte-││formatters││ builders │
│ rs   ││ rs   ││          ││          │
└──┬───┘└──┬───┘└─────┬────┘└────┬─────┘
   │       │          │          │
   └───────┴──────────┴──────────┘
                  │
      ┌───────────┼───────────┐
      │           │           │
      ▼           ▼           ▼
┌──────────┐┌──────────┐┌──────────┐
│ backends ││ adapters ││factories │
└────┬─────┘└────┬─────┘└──────────┘
     │           │
     └─────┬─────┘
           ▼
┌──────────────────────────────┐
│  integration (optional)      │
│  - thread_system             │
│  - monitoring_system         │
│  - common_system             │
└──────────────────────────────┘
```

### External Dependencies

**Required**:
- **fmt**: String formatting library (header-only mode available)
- **C++20 compiler**: GCC 11+, Clang 14+, MSVC 2022+

**Optional**:
- **thread_system**: Threading primitives and logger interface
- **common_system**: Core interfaces (ILogger, IMonitor, Result<T>)
- **monitoring_system**: Metrics collection and health monitoring
- **OpenSSL 3.0+** (required for encryption): Secure key storage and encryption support

---

## File Naming Conventions

### Header Files
- **Interface headers**: `*_interface.h` (e.g., `log_writer_interface.h`)
- **Implementation headers**: `*.h` (e.g., `logger.h`, `file_writer.h`)
- **Builder headers**: `*_builder.h` (e.g., `writer_builder.h`, `logger_builder.h`)
- **Factory headers**: `*_factory.h` (e.g., `writer_factory.h`)
- **Adapter headers**: `*_adapter.h` (e.g., `logger_adapter.h`)
- **Strategy headers**: `*_strategy.h` (e.g., `deployment_strategy.h`)

### Source Files
- **Public implementation**: `src/core/*.cpp` (core logic)
- **Internal implementation**: `src/impl/**/*.cpp` (private implementation details)
- **Tests**: `*_test.cpp` (e.g., `logger_test.cpp`)
- **Benchmarks**: `*_benchmark.cpp` (e.g., `throughput_benchmark.cpp`)

### Documentation Files
- **Markdown**: `*.md` (English) or `*.kr.md` (Korean)
- **API docs**: `API_REFERENCE.md`
- **Guides**: `GUIDE_NAME.md` in `docs/guides/`

---

## See Also

- [Architecture Overview](ARCHITECTURE.md) - System design and architecture
- [API Reference](API_REFERENCE.md) - Complete API documentation
- [Quick Start Guide](guides/QUICK_START.md) - Build and startup instructions
- [Build Guide](guides/BUILD.md) - Complete CMake options, presets, and optional features
- [Troubleshooting Guide](guides/TROUBLESHOOTING.md) - Common build and runtime issues
- [Contributing Guide](contributing/CONTRIBUTING.md) - Contribution guidelines
