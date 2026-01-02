# Logger System - Project Structure

**English | [한국어](STRUCTURE_KO.md)**

---

## Table of Contents

- [Directory Layout](#directory-layout)
- [Namespace Organization](#namespace-organization)
- [Component Layout](#component-layout)
- [Module Dependencies](#module-dependencies)
- [Key Components Overview](#key-components-overview)

## Directory Layout

```
logger_system/
├── 📁 include/kcenon/logger/       # Public headers & interfaces
│   ├── 📁 core/                     # Core logger components
│   │   ├── logger.h                 # Main logger implementation
│   │   ├── logger_builder.h         # Builder pattern API
│   │   ├── logger_config.h          # Configuration management
│   │   ├── log_collector.h          # Log entry collection
│   │   ├── error_codes.h            # Logger-specific error codes
│   │   ├── small_string.h           # Optimized string storage
│   │   ├── thread_integration_detector.h  # Thread system detection
│   │   ├── 📁 metrics/              # Performance metrics
│   │   │   └── logger_metrics.h     # Metrics data structures
│   │   └── 📁 monitoring/           # Monitoring integration
│   │       ├── monitoring_interface.h           # IMonitor interface (deprecated)
│   │       └── monitoring_interface_transition.h # Transition helpers
│   ├── 📁 interfaces/               # Abstract interface definitions
│   │   ├── logger_interface.h       # Base logger interface
│   │   ├── logger_types.h           # Common type definitions
│   │   ├── log_entry.h              # Log entry data structure
│   │   ├── log_writer_interface.h   # Writer abstraction
│   │   ├── log_formatter_interface.h # Formatter abstraction
│   │   ├── log_filter_interface.h   # Filter abstraction
│   │   └── log_sink_interface.h     # Sink abstraction
│   ├── 📁 writers/                  # Log writer implementations
│   │   ├── base_writer.h            # Common writer base class
│   │   ├── console_writer.h         # Console output (ANSI colors)
│   │   ├── file_writer.h            # Basic file output
│   │   ├── rotating_file_writer.h   # Size/time-based rotation
│   │   ├── network_writer.h         # TCP/UDP remote logging
│   │   ├── async_writer.h           # Asynchronous wrapper
│   │   ├── batch_writer.h           # Batched output
│   │   ├── critical_writer.h        # Synchronous critical logs
│   │   └── encrypted_writer.h       # Encrypted log output
│   ├── 📁 formatters/               # Message formatters
│   │   └── base_formatter.h         # Base formatter interface
│   ├── 📁 filters/                  # Log filtering
│   │   └── log_filter.h             # Filter implementations
│   ├── 📁 routing/                  # Log routing system
│   │   └── log_router.h             # Routing logic
│   ├── 📁 analysis/                 # Log analysis tools
│   │   └── log_analyzer.h           # Pattern analysis
│   ├── 📁 structured/               # Structured logging
│   │   └── structured_logger.h      # JSON/structured output
│   ├── 📁 safety/                   # Crash-safe logging
│   │   └── crash_safe_logger.h      # Emergency flush on crash
│   ├── 📁 server/                   # Network log server
│   │   └── log_server.h             # Remote log receiver
│   ├── 📁 adapters/                 # System integration adapters
│   │   ├── common_logger_adapter.h  # common_system adapter
│   │   ├── common_system_adapter.h  # Legacy adapter
│   │   └── logger_adapter.h         # Generic adapter
│   └── compatibility.h              # Backward compatibility helpers
├── 📁 src/                          # Implementation files
│   ├── 📁 core/                     # Core implementations
│   │   ├── logger.cpp               # Main logger logic
│   │   ├── logger_builder.cpp       # Builder implementation
│   │   ├── logger_config.cpp        # Configuration logic
│   │   ├── log_collector.cpp        # Collection implementation
│   │   └── small_string.cpp         # String optimization
│   ├── 📁 writers/                  # Writer implementations
│   │   ├── base_writer.cpp
│   │   ├── console_writer.cpp
│   │   ├── file_writer.cpp
│   │   ├── rotating_file_writer.cpp
│   │   ├── network_writer.cpp
│   │   ├── async_writer.cpp
│   │   ├── batch_writer.cpp
│   │   ├── critical_writer.cpp
│   │   └── encrypted_writer.cpp
│   ├── 📁 formatters/               # Formatter implementations
│   │   └── base_formatter.cpp
│   ├── 📁 filters/                  # Filter implementations
│   │   └── log_filter.cpp
│   ├── 📁 routing/                  # Routing implementations
│   │   └── log_router.cpp
│   ├── 📁 analysis/                 # Analysis implementations
│   │   └── log_analyzer.cpp
│   ├── 📁 structured/               # Structured logging impl
│   │   └── structured_logger.cpp
│   ├── 📁 safety/                   # Safety implementations
│   │   └── crash_safe_logger.cpp
│   ├── 📁 server/                   # Server implementations
│   │   └── log_server.cpp
│   └── 📁 adapters/                 # Adapter implementations
│       ├── common_logger_adapter.cpp
│       └── logger_adapter.cpp
├── 📁 tests/                        # Test suite
│   ├── 📁 unit/                     # Unit tests
│   │   ├── core_tests/              # Core functionality tests
│   │   ├── writer_tests/            # Writer component tests
│   │   ├── formatter_tests/         # Formatter tests
│   │   └── filter_tests/            # Filter tests
│   └── 📁 performance/              # Performance tests
│       └── benchmark_tests/         # Benchmark suite
├── 📁 integration_tests/            # Integration tests
│   ├── 📁 framework/                # Test framework
│   │   ├── test_helpers.h           # Test utilities
│   │   └── system_fixture.h         # System-wide fixtures
│   └── 📁 scenarios/                # Integration scenarios
│       ├── common_system_integration_test.cpp
│       ├── thread_system_integration_test.cpp
│       └── monitoring_system_integration_test.cpp
├── 📁 benchmarks/                   # Performance benchmarks
│   ├── async_benchmark.cpp          # Async logging performance
│   ├── throughput_benchmark.cpp     # Throughput measurements
│   └── comparison_benchmark.cpp     # spdlog comparison
├── 📁 examples/                     # Usage examples
│   ├── 📁 basic/                    # Basic usage examples
│   │   ├── simple_logger.cpp        # Minimal example
│   │   └── file_logging.cpp         # File output example
│   ├── 📁 advanced/                 # Advanced examples
│   │   ├── structured_logging.cpp   # JSON logging
│   │   ├── network_logging.cpp      # Remote logging
│   │   └── crash_safe_example.cpp   # Crash safety
│   └── 📁 integration/              # System integration
│       ├── with_thread_system.cpp
│       ├── with_monitoring.cpp
│       └── full_stack.cpp
├── 📁 docs/                         # Comprehensive documentation
│   ├── 📁 api/                      # API documentation
│   │   └── API_REFERENCE.md
│   ├── 📁 guides/                   # User guides
│   │   ├── GETTING_STARTED.md
│   │   ├── BEST_PRACTICES.md
│   │   └── PERFORMANCE.md
│   ├── 📁 architecture/             # Architecture docs
│   │   ├── LOGGER_SYSTEM_ARCHITECTURE.md
│   │   └── PHASE_3_PREPARATION.md
│   └── SECURITY.md                  # Security considerations
├── 📁 cmake/                        # CMake modules
│   ├── LoggerFeatures.cmake         # Feature configuration
│   ├── LoggerDependencies.cmake     # Dependency detection
│   ├── LoggerSanitizers.cmake       # Sanitizer support
│   ├── LoggerWarnings.cmake         # Warning configuration
│   ├── LoggerCoverage.cmake         # Coverage support
│   ├── CompilerChecks.cmake         # Compiler validation
│   └── LoggerSystemConfig.cmake.in  # Package config template
├── 📁 scripts/                      # Build & utility scripts
│   ├── build.sh                     # Build automation
│   ├── test.sh                      # Test execution
│   └── benchmark.sh                 # Benchmark runner
├── 📄 CMakeLists.txt                # Main build configuration
├── 📄 vcpkg.json                    # Dependency manifest
├── 📄 README.md                     # Project overview
├── 📄 INTEGRATION.md                # Integration guide (this doc)
├── 📄 STRUCTURE.md                  # Structure documentation
├── 📄 BASELINE.md                   # Performance baselines
├── 📄 CHANGELOG.md                  # Version history
├── 📄 LICENSE                       # BSD 3-Clause License
└── 📄 .clang-format                 # Code formatting rules
```

## Namespace Organization

### Primary Namespace Hierarchy

```cpp
kcenon::logger                          // Root namespace
├── core                                // Core functionality
│   ├── logger                          // Main logger class
│   ├── logger_builder                  // Builder pattern
│   ├── logger_config                   // Configuration
│   ├── log_collector                   // Collection logic
│   ├── metrics                         // Performance metrics
│   │   └── logger_metrics              // Metrics structures
│   └── monitoring                      // Monitoring integration
│       └── monitoring_interface        // IMonitor interface (deprecated)
├── interfaces                          // Abstract interfaces
│   ├── logger_interface                // Base logger
│   ├── log_entry                       // Entry structure
│   ├── log_writer_interface            // Writer abstraction
│   ├── log_formatter_interface         // Formatter abstraction
│   ├── log_filter_interface            // Filter abstraction
│   └── log_sink_interface              // Sink abstraction
├── writers                             // Writer implementations
│   ├── base_writer                     // Common base
│   ├── console_writer                  // Console output
│   ├── file_writer                     // File output
│   ├── rotating_file_writer            // Rotating files
│   ├── network_writer                  // Network output
│   ├── async_writer                    // Async wrapper
│   ├── batch_writer                    // Batched output
│   ├── critical_writer                 // Sync critical
│   └── encrypted_writer                // Encrypted output
├── formatters                          // Message formatting
│   └── base_formatter                  // Base formatter
├── filters                             // Log filtering
│   └── log_filter                      // Filter logic
├── routing                             // Log routing
│   └── log_router                      // Router logic
├── analysis                            // Log analysis
│   └── log_analyzer                    // Analyzer
├── structured                          // Structured logging
│   └── structured_logger               // JSON logger
├── safety                              // Crash safety
│   └── crash_safe_logger               // Crash handler
├── server                              // Network server
│   └── log_server                      // Server impl
└── adapters                            // System adapters
    ├── common_logger_adapter           // common_system
    └── logger_adapter                  // Generic adapter
```

### Namespace Usage Examples

```cpp
// Core components
using namespace kcenon::logger::core;
auto logger = logger_builder()
    .use_template("production")
    .build()
    .value();

// Writers
using namespace kcenon::logger::writers;
auto console = std::make_unique<console_writer>();
auto file = std::make_unique<file_writer>("app.log");

// Interfaces
using namespace kcenon::logger::interfaces;
std::shared_ptr<log_writer_interface> writer = console;

// Adapters
using namespace kcenon::logger::adapters;
auto adapted = make_common_logger_adapter(std::move(logger));
```

## Component Layout

### Core Components

#### Logger Core (`core/`)
- **Purpose**: Main logger implementation and configuration
- **Key Classes**:
  - `logger`: Asynchronous logging engine
  - `logger_builder`: Fluent configuration API
  - `logger_config`: Configuration management
  - `log_collector`: Log entry collection
- **Dependencies**: `common_system` (ILogger, Result<T>)

#### Interfaces (`interfaces/`)
- **Purpose**: Abstract base classes for extensibility
- **Key Interfaces**:
  - `logger_interface`: Main logger contract
  - `log_writer_interface`: Writer abstraction
  - `log_formatter_interface`: Formatter abstraction
  - `log_filter_interface`: Filter abstraction
  - `log_sink_interface`: Sink abstraction
- **Dependencies**: None (pure interfaces)

#### Writers (`writers/`)
- **Purpose**: Output destination implementations
- **Key Classes**:
  - `console_writer`: ANSI-colored console output
  - `file_writer`: Buffered file writing
  - `rotating_file_writer`: Automatic log rotation
  - `network_writer`: TCP/UDP remote logging
  - `async_writer`: Asynchronous wrapper
  - `critical_writer`: Synchronous critical logs
  - `encrypted_writer`: Encrypted output (demo)
- **Dependencies**: `interfaces/`, filesystem, networking

#### Formatters (`formatters/`)
- **Purpose**: Message formatting implementations
- **Key Classes**:
  - `base_formatter`: Common formatting logic
  - JSON, XML, plain text formatters (planned)
- **Dependencies**: `interfaces/`

#### Filters (`filters/`)
- **Purpose**: Log filtering and routing logic
- **Key Classes**:
  - `log_filter`: Level-based, regex, custom filters
- **Dependencies**: `interfaces/`, regex

#### Routing (`routing/`)
- **Purpose**: Conditional log routing
- **Key Classes**:
  - `log_router`: Route logs to specific writers
- **Dependencies**: `interfaces/`, `writers/`

#### Analysis (`analysis/`)
- **Purpose**: Real-time log analysis
- **Key Classes**:
  - `log_analyzer`: Pattern detection, statistics
- **Dependencies**: `interfaces/`, regex

#### Structured Logging (`structured/`)
- **Purpose**: JSON and structured output
- **Key Classes**:
  - `structured_logger`: Structured log builder
- **Dependencies**: `core/`, JSON library

#### Safety (`safety/`)
- **Purpose**: Crash-safe logging mechanisms
- **Key Classes**:
  - `crash_safe_logger`: Signal handlers, emergency flush
- **Dependencies**: `core/`, platform signal APIs

#### Server (`server/`)
- **Purpose**: Network log server
- **Key Classes**:
  - `log_server`: TCP/UDP log receiver
- **Dependencies**: `core/`, `writers/`, networking

#### Adapters (`adapters/`)
- **Purpose**: Integration with other systems
- **Key Classes**:
  - `common_logger_adapter`: common_system integration
  - `logger_adapter`: Generic adapter pattern
- **Dependencies**: `common_system`, `thread_system` (optional)

### Monitoring (`core/monitoring/`)
- **Purpose**: Integration with monitoring_system
- **Key Classes**:
  - `monitoring_interface`: IMonitor abstraction (deprecated, use common_system)
- **Dependencies**: `common_system` (IMonitor)

## Module Dependencies

### Internal Dependencies

```
interfaces (no dependencies)
    │
    ├──> core (depends on interfaces)
    │      │
    │      ├──> writers (depends on interfaces, core)
    │      │
    │      ├──> formatters (depends on interfaces)
    │      │
    │      ├──> filters (depends on interfaces)
    │      │
    │      ├──> routing (depends on interfaces, writers)
    │      │
    │      ├──> analysis (depends on interfaces)
    │      │
    │      ├──> structured (depends on core, formatters)
    │      │
    │      ├──> safety (depends on core)
    │      │
    │      └──> server (depends on core, writers)
    │
    └──> adapters (depends on interfaces, core)
```

### External Dependencies

```
common_system (REQUIRED)
    ├── interfaces/logger_interface.h   → Implemented by logger
    ├── interfaces/monitoring_interface.h → Used for metrics
    ├── patterns/result.h               → Used for error handling
    └── error/error_codes.h             → Error code registry

thread_system (OPTIONAL)
    ├── core/thread_pool.h              → Async operations
    ├── interfaces/executor_interface.h → Task execution
    └── adapters/                       → DI integration

monitoring_system (OPTIONAL)
    ├── core/performance_monitor.h      → Metrics collection
    └── interfaces/monitor_interface.h  → Health checks
```

### Compilation Units

```cpp
// Header-only components (no .cpp files)
- interfaces/*
- core/di/*
- adapters/* (mostly inline)

// Compiled components (.h + .cpp)
- core/logger
- core/logger_builder
- writers/*
- formatters/*
- filters/*
- routing/*
- analysis/*
- structured/*
- safety/*
- server/*
```

## Key Components Overview

### Public API Layer (`include/kcenon/logger/`)

| Component | File | Purpose | Dependencies |
|-----------|------|---------|--------------|
| **Main Logger** | `core/logger.h` | Primary logging interface | `common_system`, `interfaces/` |
| **Builder** | `core/logger_builder.h` | Fluent logger construction | `core/logger.h` |
| **Configuration** | `core/logger_config.h` | Configuration management | `common_system` |
| **Log Collector** | `core/log_collector.h` | Entry collection | `interfaces/log_entry.h` |
| **Error Codes** | `core/error_codes.h` | Logger-specific errors | `common_system` |
| **DI Container** | `core/di/di_container_interface.h` | Dependency injection | None |
| **Metrics** | `core/metrics/logger_metrics.h` | Performance metrics | None |
| **Monitoring** | `core/monitoring/monitoring_interface.h` | IMonitor integration | `common_system` |
| **Writer Interface** | `interfaces/log_writer_interface.h` | Base writer contract | `common_system` |
| **Logger Interface** | `interfaces/logger_interface.h` | Base logger contract | `common_system` |
| **Console Writer** | `writers/console_writer.h` | ANSI console output | `interfaces/` |
| **File Writer** | `writers/file_writer.h` | File output | `interfaces/` |
| **Rotating Writer** | `writers/rotating_file_writer.h` | Log rotation | `writers/file_writer.h` |
| **Network Writer** | `writers/network_writer.h` | TCP/UDP logging | `interfaces/` |
| **Async Writer** | `writers/async_writer.h` | Async wrapper | `interfaces/` |
| **Critical Writer** | `writers/critical_writer.h` | Sync critical logs | `interfaces/` |
| **Encrypted Writer** | `writers/encrypted_writer.h` | Encrypted output | `interfaces/` |
| **Base Formatter** | `formatters/base_formatter.h` | Formatting base | `interfaces/` |
| **Log Filter** | `filters/log_filter.h` | Filtering logic | `interfaces/` |
| **Log Router** | `routing/log_router.h` | Routing logic | `interfaces/`, `writers/` |
| **Log Analyzer** | `analysis/log_analyzer.h` | Pattern analysis | `interfaces/` |
| **Structured Logger** | `structured/structured_logger.h` | JSON logging | `core/logger.h` |
| **Crash-Safe Logger** | `safety/crash_safe_logger.h` | Emergency flush | `core/logger.h` |
| **Log Server** | `server/log_server.h` | Network receiver | `core/`, `writers/` |
| **Common Adapter** | `adapters/common_logger_adapter.h` | common_system bridge | `common_system` |

### Implementation Layer (`src/`)

| Directory | Purpose | Key Files | Build Type |
|-----------|---------|-----------|------------|
| `core/` | Core implementations | `logger.cpp`, `logger_builder.cpp`, `logger_config.cpp` | Compiled |
| `writers/` | Writer implementations | `console_writer.cpp`, `file_writer.cpp`, `rotating_file_writer.cpp`, `network_writer.cpp`, `async_writer.cpp`, `critical_writer.cpp` | Compiled |
| `formatters/` | Formatter implementations | `base_formatter.cpp` | Compiled |
| `filters/` | Filter implementations | `log_filter.cpp` | Compiled |
| `routing/` | Routing implementations | `log_router.cpp` | Compiled |
| `analysis/` | Analysis implementations | `log_analyzer.cpp` | Compiled |
| `structured/` | Structured logging impl | `structured_logger.cpp` | Compiled |
| `safety/` | Safety implementations | `crash_safe_logger.cpp` | Compiled |
| `server/` | Server implementations | `log_server.cpp` | Compiled |
| `adapters/` | Adapter implementations | `common_logger_adapter.cpp` | Compiled |

### Test Organization

| Directory | Purpose | Framework |
|-----------|---------|-----------|
| `tests/unit/` | Unit tests for individual components | Google Test |
| `tests/performance/` | Performance benchmarks | Google Benchmark |
| `integration_tests/` | Cross-system integration tests | Google Test |
| `benchmarks/` | Throughput and latency benchmarks | Google Benchmark |

## Performance Characteristics

### Throughput
- **Peak**: 4.34M messages/second (single thread, async mode)
- **Multi-threaded**: 1.07M messages/s (4 threads), 412K messages/s (8 threads)
- **Latency**: 148ns average enqueue time

### Memory
- **Baseline**: <2MB with adaptive buffer management
- **Queue**: Configurable (default 20,000 entries)
- **Buffer**: Configurable (default 16KB per writer)

### Threading
- **Thread-Safe**: All components safe for concurrent access
- **Lock-Free**: Async queue uses lock-free design where possible
- **ThreadSanitizer**: Zero data races detected

### Resource Management
- **RAII**: Grade A compliance with smart pointers
- **AddressSanitizer**: Zero memory leaks detected
- **Exception Safety**: Strong exception safety guarantees

## Build Configuration

### CMake Targets

```cmake
# Main library
LoggerSystem::logger          # Header-only or compiled library

# Component targets (internal)
LoggerSystem::core            # Core logger components
LoggerSystem::writers         # Writer implementations
LoggerSystem::formatters      # Formatter implementations
```

### Feature Flags

Configure via CMake options:
```bash
# Core Features
-DLOGGER_USE_DI=ON                  # Enable dependency injection
-DLOGGER_USE_MONITORING=ON          # Enable monitoring support
-DLOGGER_ENABLE_ASYNC=ON            # Enable async logging

# Integration
-DBUILD_WITH_COMMON_SYSTEM=ON       # Enable common_system integration
-DUSE_THREAD_SYSTEM=ON              # Use thread_system if available

# Performance
-DLOGGER_DEFAULT_BUFFER_SIZE=16384  # Buffer size
-DLOGGER_DEFAULT_BATCH_SIZE=200     # Batch size
-DLOGGER_DEFAULT_QUEUE_SIZE=20000   # Queue size

# Quality
-DLOGGER_ENABLE_SANITIZERS=ON       # Enable sanitizers
-DLOGGER_ENABLE_COVERAGE=ON         # Enable coverage
```

## References

- [README.md](README.md) - Project overview and features
- [INTEGRATION.md](INTEGRATION.md) - Integration guide with other systems
- [BASELINE.md](BASELINE.md) - Performance baselines and benchmarks
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) - Detailed architecture documentation
- [docs/API_REFERENCE.md](docs/API_REFERENCE.md) - Complete API reference
