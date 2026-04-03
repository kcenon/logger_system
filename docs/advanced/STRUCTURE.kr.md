---
doc_id: "LOG-ARCH-008"
doc_title: "Logger System - 프로젝트 구조"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "ARCH"
---

# Logger System - 프로젝트 구조

**[English](STRUCTURE.md) | 한국어**

---

## 📁 디렉토리 레이아웃

```
logger_system/
├── 📁 include/kcenon/logger/    # Public headers & interfaces
│   ├── 📁 core/                 # Core APIs (logger, config, builder)
│   │   ├── logger.h             # Main logger interface
│   │   ├── config.h             # Configuration management
│   │   └── builder.h            # Logger builder pattern
│   ├── 📁 interfaces/           # Interface definitions
│   │   ├── writer_interface.h   # Base writer interface
│   │   ├── formatter_interface.h # Base formatter interface
│   │   └── filter_interface.h   # Base filter interface
│   ├── 📁 writers/              # Writer interfaces
│   │   ├── file_writer.h        # File output writer
│   │   ├── console_writer.h     # Console output writer
│   │   └── network_writer.h     # Network output writer
│   ├── 📁 formatters/           # Formatter interfaces
│   │   ├── text_formatter.h     # Plain text formatting
│   │   ├── json_formatter.h     # JSON formatting
│   │   └── xml_formatter.h      # XML formatting
│   └── 📁 utils/                # Public utilities
│       ├── log_level.h          # Log level definitions
│       ├── timestamp.h          # Timestamp utilities
│       └── thread_safe.h        # Thread safety utilities
├── 📁 src/                      # Implementation files
│   ├── 📁 core/                 # Core implementation
│   │   ├── logger.cpp           # Main logger implementation
│   │   ├── config.cpp           # Configuration implementation
│   │   └── builder.cpp          # Builder implementation
│   ├── 📁 impl/                 # Private implementations
│   │   ├── 📁 writers/          # Writer implementations
│   │   │   ├── file_writer.cpp
│   │   │   ├── console_writer.cpp
│   │   │   └── network_writer.cpp
│   │   ├── 📁 formatters/       # Formatter implementations
│   │   │   ├── text_formatter.cpp
│   │   │   ├── json_formatter.cpp
│   │   │   └── xml_formatter.cpp
│   │   ├── 📁 filters/          # Filter implementations
│   │   │   ├── level_filter.cpp
│   │   │   ├── pattern_filter.cpp
│   │   │   └── rate_limiter.cpp
│   │   ├── 📁 async/            # Async components
│   │   │   ├── async_logger.cpp
│   │   │   ├── buffer_manager.cpp
│   │   │   └── worker_thread.cpp
│   │   ├── 📁 di/               # Dependency injection container
│   │   │   ├── container.cpp
│   │   │   └── factory.cpp
│   │   └── 📁 monitoring/       # Performance monitoring adapters
│   │       ├── metrics_collector.cpp
│   │       └── health_monitor.cpp
│   └── 📁 utils/                # Utility implementations
│       ├── timestamp.cpp        # Timestamp implementation
│       ├── thread_safe.cpp      # Thread safety implementation
│       └── string_utils.cpp     # String utility functions
├── 📁 tests/                    # Comprehensive test suite
│   ├── 📁 unit/                 # Unit tests
│   │   ├── core_tests/          # Core functionality tests
│   │   ├── writer_tests/        # Writer component tests
│   │   ├── formatter_tests/     # Formatter component tests
│   │   └── filter_tests/        # Filter component tests
│   ├── 📁 integration/          # Integration tests
│   │   ├── ecosystem_tests/     # Cross-system integration
│   │   ├── performance_tests/   # Performance integration
│   │   └── stress_tests/        # Stress testing
│   └── 📁 benchmarks/           # Performance benchmarks
│       ├── throughput_bench/    # Throughput measurements
│       ├── latency_bench/       # Latency measurements
│       └── memory_bench/        # Memory usage benchmarks
├── 📁 examples/                 # Usage examples & demos
│   ├── 📁 basic/                # Basic usage examples
│   ├── 📁 advanced/             # Advanced configuration examples
│   └── 📁 integration/          # System integration examples
├── 📁 docs/                     # Comprehensive documentation
│   ├── 📁 api/                  # API documentation
│   ├── 📁 guides/               # User guides & tutorials
│   ├── 📁 architecture/         # Architecture documentation
│   └── 📁 performance/          # Performance guides & benchmarks
├── 📁 scripts/                  # Build & utility scripts
│   ├── build.sh                 # Build automation
│   ├── test.sh                  # Test execution
│   └── benchmark.sh             # Performance testing
├── 📄 CMakeLists.txt            # Build configuration
├── 📄 .clang-format             # Code formatting rules
└── 📄 README.md                 # Project overview & documentation
```

## 🏗️ Namespace 구조

### Core Namespaces
- **Root**: `kcenon::logger` - 메인 logger namespace
- **Core functionality**: `kcenon::logger::core` - 핵심 logger 컴포넌트
- **Interfaces**: `kcenon::logger::interfaces` - 추상 기본 클래스
- **Writers**: `kcenon::logger::writers` - 출력 대상 구현체
- **Formatters**: `kcenon::logger::formatters` - 메시지 포맷팅 구현체
- **Implementation details**: `kcenon::logger::impl` - 내부 구현 클래스
- **Utilities**: `kcenon::logger::utils` - 헬퍼 함수 및 유틸리티

### Nested Namespaces
- `kcenon::logger::impl::async` - 비동기 로깅 컴포넌트
- `kcenon::logger::impl::di` - Dependency injection container
- `kcenon::logger::impl::monitoring` - 성능 모니터링 컴포넌트

## 🔧 주요 컴포넌트 개요

### 🎯 Public API Layer (`include/kcenon/logger/`)
| 컴포넌트 | 파일 | 목적 |
|-----------|------|---------|
| **Main Logger** | `core/logger.h` | 주요 로깅 interface |
| **Configuration** | `core/config.h` | Logger 설정 관리 |
| **Builder Pattern** | `core/builder.h` | Fluent logger 생성 |
| **Writer Interface** | `interfaces/writer_interface.h` | 모든 writer의 기본 클래스 |
| **Formatter Interface** | `interfaces/formatter_interface.h` | 모든 formatter의 기본 클래스 |
| **File Writer** | `writers/file_writer.h` | 파일 출력 구현체 |
| **Console Writer** | `writers/console_writer.h` | 콘솔 출력 구현체 |
| **JSON Formatter** | `formatters/json_formatter.h` | JSON 메시지 포맷팅 |
| **XML Formatter** | `formatters/xml_formatter.h` | XML 메시지 포맷팅 |

### ⚙️ Implementation Layer (`src/`)
| 컴포넌트 | 디렉토리 | 목적 |
|-----------|-----------|---------|
| **Async Pipeline** | `impl/async/` | Non-blocking 로깅 작업 |
| **DI Container** | `impl/di/` | Dependency injection framework |
| **Writer Implementations** | `impl/writers/` | 구체적인 writer 클래스 |
| **Formatter Implementations** | `impl/formatters/` | 구체적인 formatter 클래스 |
| **Filter System** | `impl/filters/` | 메시지 필터링 로직 |
| **Monitoring** | `impl/monitoring/` | 성능 메트릭 수집 |

## 📊 성능 특성

- **Throughput**: 4.34M+ messages/second (async mode)
- **Latency**: Sub-microsecond 로깅 호출 (async mode)
- **Memory**: 가능한 경우 zero-copy 메시지 pipeline
- **Thread Safety**: 고성능 async 로깅을 위한 lock-free queue

## 🔄 마이그레이션 가이드

### Step 1: 현재 설정 백업
```bash
# Automatic backup of old structure
mkdir -p old_structure/
cp -r include/ old_structure/include_backup/
cp -r src/ old_structure/src_backup/
```

### Step 2: Include 경로 업데이트
```cpp
// Old style
#include "logger/logger.h"

// New style
#include "kcenon/logger/core/logger.h"
```

### Step 3: Namespace 사용 업데이트
```cpp
// Old style
using namespace logger;

// New style
using namespace kcenon::logger::core;
```

### Step 4: 마이그레이션 스크립트 실행
```bash
# Automated namespace migration
./scripts/migrate_namespaces.sh
./scripts/update_cmake.sh
```

## 🚀 새로운 구조로 빠른 시작

```cpp
#include "kcenon/logger/core/logger.h"
#include "kcenon/logger/core/builder.h"
#include "kcenon/logger/writers/file_writer.h"
#include "kcenon/logger/formatters/json_formatter.h"

int main() {
    using namespace kcenon::logger;

    // Build logger with new structure
    auto logger = core::logger_builder()
        .add_writer(std::make_shared<writers::file_writer>("app.log"))
        .set_formatter(std::make_shared<formatters::json_formatter>())
        .set_level(core::log_level::info)
        .build();

    // Use logger
    logger->info("Application started with new structure");

    return 0;
}
```
