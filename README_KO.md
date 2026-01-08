[![CI](https://github.com/kcenon/logger_system/actions/workflows/ci.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/ci.yml)
[![Sanitizers](https://github.com/kcenon/logger_system/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/sanitizers.yml)
[![Benchmarks](https://github.com/kcenon/logger_system/actions/workflows/benchmarks.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/benchmarks.yml)
[![Code Coverage](https://github.com/kcenon/logger_system/actions/workflows/coverage.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/coverage.yml)
[![Static Analysis](https://github.com/kcenon/logger_system/actions/workflows/static-analysis.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/static-analysis.yml)
[![Docs](https://github.com/kcenon/logger_system/actions/workflows/build-Doxygen.yaml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/build-Doxygen.yaml)

# Logger System

> **Language:** [English](README.md) | **한국어**

## 개요

멀티스레드 애플리케이션을 위해 설계된 프로덕션 환경에서 사용 가능한 고성능 C++17 비동기 로깅 프레임워크입니다. 모듈식 인터페이스 기반 아키텍처와 원활한 생태계 통합을 갖추고 있습니다.

**주요 기능**:
- 🚀 **초고속 비동기 로깅**: 초당 434만 메시지, 148ns 지연시간
- 🔒 **설계 단계부터 스레드 세이프**: 제로 데이터 레이스 검증
- 🏗️ **모듈식 아키텍처**: 인터페이스 기반, 플러그 가능한 컴포넌트
- 🛡️ **고품질**: 포괄적인 CI/CD, sanitizer, 벤치마크
- 🔐 **보안 우선**: 경로 검증, 안전한 저장소, 감사 로깅
- 🌐 **크로스 플랫폼**: GCC, Clang, MSVC를 사용한 Windows, Linux, macOS

---

## 빠른 시작

### 기본 예제

```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>

int main() {
    // 자동 validation을 사용하여 builder로 logger 생성
    auto result = kcenon::logger::logger_builder()
        .use_template("production")  // 사전 정의된 configuration
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

    // 에러 핸들링이 포함된 로그 메시지
    logger->log(kcenon::logger::log_level::info, "Application started");
logger->log(kcenon::logger::log_level::error, "Something went wrong");

return 0;
}
```

### Result 처리 요약
- `result.is_err()` / `result.error()` 패턴을 사용해 Builder, writer 추가, validation API의 실패를 감지하세요.
- 실패를 호출자에게 전달할 때는 `return Result<void>::err(result.error());`처럼 `common::error_info`를 그대로 넘기는 방식이 권장됩니다.
- DI 컨테이너나 모듈 경계를 넘는 경우 `err.code`, `err.message`, `err.module`을 로그에 남기면 추적이 쉬워집니다.
- 보다 자세한 내용은 [Result Handling Cheatsheet](docs/guides/INTEGRATION.md#result-handling-cheatsheet)를 참고하세요.

### 설치

**vcpkg 사용**:
```bash
# 기본 기능으로 설치 (fmt만 포함)
vcpkg install kcenon-logger-system

# 벤치마크 포함 설치 (비교용 spdlog 포함)
vcpkg install kcenon-logger-system[benchmarks]
```

> **참고**: 생태계 의존성(common_system, thread_system)은 아직 vcpkg에 등록되지 않았습니다. 등록 전까지는 로컬 클론을 사용한 CMake 빌드를 사용하세요. [의존성과 함께 빌드하기](#의존성과-함께-빌드하기) 참조.

**CMake 사용**:
```bash
mkdir build && cd build
cmake ..
cmake --build .
cmake --build . --target install
```

**프로젝트에서 사용**:
```cmake
find_package(LoggerSystem REQUIRED)
target_link_libraries(your_app PRIVATE LoggerSystem::logger)
```

### 요구사항

| 의존성 | 버전 | 필수 여부 | 설명 |
|--------|------|----------|------|
| C++20 컴파일러 | GCC 11+ / Clang 14+ / MSVC 2022+ / Apple Clang 14+ | 필수 | C++20 기능 필요 |
| CMake | 3.20+ | 필수 | 빌드 시스템 |
| [common_system](https://github.com/kcenon/common_system) | latest | 필수 | 공통 인터페이스 (ILogger, Result<T>) |
| [thread_system](https://github.com/kcenon/thread_system) | latest | 선택 | 스레드 풀을 사용한 비동기 로깅 |
| [fmt](https://github.com/fmtlib/fmt) | 10.0+ | 필수 | 현대적인 포맷팅 라이브러리 |
| vcpkg | latest | 선택 | 패키지 관리 |

> **참고**: spdlog는 logger_system 내부에서 **사용되지 않습니다**. 벤치마크 비교를 위한 선택적 의존성으로만 포함됩니다. 성능 비교는 [벤치마크](docs/BENCHMARKS_KO.md)를 참조하세요.

#### 의존성 구조

```
logger_system
├── common_system (필수)
└── thread_system (선택, 스레드 풀 기반 비동기 로깅용)
    └── common_system (필수)
```

#### 의존성과 함께 빌드하기

```bash
# 의존성 클론
git clone https://github.com/kcenon/common_system.git
git clone https://github.com/kcenon/thread_system.git  # 선택사항

# logger_system 클론 및 빌드
git clone https://github.com/kcenon/logger_system.git
cd logger_system
cmake -B build -DLOGGER_USE_THREAD_SYSTEM=ON  # thread_system 통합 활성화
cmake --build build
```

---

## 핵심 기능

### 비동기 로깅
- **논블로킹 작업**: 백그라운드 스레드가 블로킹 없이 I/O 처리
- **배치 처리**: 여러 로그 항목을 효율적으로 처리
- **적응형 배치**: 큐 활용도에 기반한 지능적 최적화
- **제로카피 설계**: 최소한의 할당과 오버헤드

### 다양한 Writer 유형
- **Console Writer**: 다양한 로그 레벨을 위한 ANSI 색상 출력
- **File Writer**: 설정 가능한 버퍼링 파일 출력
- **Rotating File Writer**: 크기/시간 기반 rotation 및 압축
- **Network Writer**: TCP/UDP 원격 로깅
- **Critical Writer**: 중요 메시지를 위한 동기식 로깅
- **Hybrid Writer**: 로그 레벨에 따른 자동 비동기/동기 전환

[📚 상세 Writer 문서 →](docs/FEATURES.md#writer-types)

### 보안 기능 (v3.0.0)
- **안전한 키 저장소**: 자동 정리 기능을 갖춘 RAII 기반 암호화 키 관리
- **암호화 Writer**: 항목별 IV 회전이 포함된 AES-256-GCM 암호화 로그 저장
- **경로 검증**: 경로 탐색 공격 방지
- **시그널 핸들러 안전성**: 크래시 시나리오를 위한 긴급 flush
- **보안 감사 로깅**: HMAC-SHA256을 사용한 변조 방지 감사 추적
- **규정 준수 지원**: GDPR, PCI DSS, ISO 27001, SOC 2

[🔒 완전한 보안 가이드 →](docs/FEATURES.md#security-features)

---

## 성능 하이라이트

*Apple M1 (8-core) @ 3.2GHz, 16GB, macOS Sonoma에서 벤치마크*

### 처리량

| 구성 | 처리량 | vs spdlog |
|------|--------|-----------|
| **단일 스레드 (async)** | **434만 msg/s** | -19% |
| **4 스레드** | **107만 msg/s** | **+36%** |
| **8 스레드** | **41.2만 msg/s** | **+72%** |
| **16 스레드** | **39만 msg/s** | **+117%** |

### 지연시간

| 지표 | Logger System | spdlog async | 우위 |
|------|---------------|--------------|------|
| **평균** | **148 ns** | 2,325 ns | **15.7배 빠름** |
| **p99** | **312 ns** | 4,850 ns | **15.5배 빠름** |
| **p99.9** | **487 ns** | ~7,000 ns | **14.4배 빠름** |

### 메모리 효율성

- **기본**: 1.8 MB (spdlog: 4.2 MB 대비 **57% 적음**)
- **최대**: 2.4 MB
- **메시지당 할당**: 0.12

**주요 인사이트**:
- 🏃 **멀티스레드 우위**: 적응형 배치가 우수한 확장성 제공
- ⏱️ **초저지연**: 업계 최고 수준의 148ns 평균 enqueue 시간
- 💾 **메모리 효율**: 제로카피 설계로 최소 메모리 사용

[⚡ 전체 벤치마크 및 방법론 →](docs/BENCHMARKS.md)

---

## 아키텍처 개요

### 모듈식 설계

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

### 주요 컴포넌트

- **Logger Core**: Builder 패턴을 사용한 메인 비동기 처리 엔진
- **Writers**: 플러그 가능한 출력 대상 (파일, console, 네트워크 등)
- **Filters**: 레벨, 패턴 또는 사용자 정의 로직 기반 조건부 로깅
- **Formatters**: 설정 가능한 출력 형식 (plain, JSON, logfmt, custom)
- **Security**: 경로 검증, 안전한 저장소, 감사 로깅

[🏛️ 상세 아키텍처 가이드 →](docs/01-architecture.md)

---

## 생태계 통합

깔끔한 인터페이스 경계를 가진 모듈식 C++ 생태계의 일부:

### 의존성

**필수**:
- **[common_system](https://github.com/kcenon/common_system)**: 핵심 인터페이스 (ILogger, IMonitor, Result<T>) 및 C++20 Concepts 지원

**선택사항**:
- **[thread_system](https://github.com/kcenon/thread_system)**: 향상된 Threading primitive (v3.1.0부터 선택사항)
- **[monitoring_system](https://github.com/kcenon/monitoring_system)**: Metric 및 health monitoring

> **참고**: v3.1.0 이후로 `thread_system`은 선택사항입니다. logger 시스템은 기본적으로 독립 std::jthread 구현을 사용합니다. 고급 비동기 처리를 위해 `-DLOGGER_USE_THREAD_SYSTEM=ON`으로 thread_system 통합을 활성화할 수 있습니다 (Issue #224 참고).

### 통합 패턴

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>

int main() {
    // Builder 패턴으로 logger 생성 (standalone 모드, thread_system 불필요)
    auto logger = kcenon::logger::logger_builder()
        .use_template("production")
        .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
        .build()
        .value();

    // 애플리케이션 어디서나 logger 사용
    logger->log(kcenon::logger::log_level::info, "System initialized");

    return 0;
}
```

> **참고**: `thread_system`이 사용 가능하고 `LOGGER_HAS_THREAD_SYSTEM`이 정의된 경우 (`-DLOGGER_USE_THREAD_SYSTEM=ON`으로 설정) 공유 스레드 풀을 통한 비동기 처리 등 추가 통합 기능이 활성화됩니다. 자세한 내용은 [thread_system 통합](docs/integration/THREAD_SYSTEM.md)을 참고하세요.

**장점**:
- 인터페이스 전용 의존성 (순환 참조 없음)
- 독립적인 컴파일 및 배포
- DI 패턴을 통한 런타임 컴포넌트 주입
- 명확한 관심사 분리

[🔗 생태계 통합 가이드 →](docs/guides/INTEGRATION.md)

---

## 문서

### 시작하기
- 📖 [Getting Started Guide](docs/guides/GETTING_STARTED.md) - 단계별 설정 및 기본 사용법
- 🚀 [Quick Start Examples](examples/basic_logging/) - 실습 예제
- 🔧 [Build Guide](docs/guides/BUILD_GUIDE.md) - 상세 빌드 지침

### 핵심 문서
- 📘 [Features](docs/FEATURES.md) - 포괄적인 기능 문서
- 📊 [Benchmarks](docs/BENCHMARKS.md) - 성능 분석 및 비교
- 🏗️ [Architecture](docs/01-architecture.md) - 시스템 설계 및 내부 구조
- 📋 [Project Structure](docs/PROJECT_STRUCTURE.md) - 디렉터리 구성 및 파일
- 🔧 [API Reference](docs/02-API_REFERENCE.md) - 완전한 API 문서

### 고급 주제
- ⚡ [Performance Guide](docs/guides/PERFORMANCE.md) - 최적화 팁 및 튜닝
- 🔒 [Security Guide](docs/SECURITY.md) - 보안 고려사항 및 모범 사례
- ✅ [Production Quality](docs/PRODUCTION_QUALITY.md) - CI/CD, 테스트, 품질 지표
- 🎨 [Custom Writers](docs/advanced/CUSTOM_WRITERS.md) - Custom log writer 생성
- 🔄 [Integration Guide](docs/guides/INTEGRATION.md) - 생태계 통합 패턴

### 개발
- 🤝 [Contributing Guide](docs/CONTRIBUTING.md) - 기여 방법
- 📋 [FAQ](docs/guides/FAQ.md) - 자주 묻는 질문
- 🔍 [Troubleshooting](docs/guides/TROUBLESHOOTING.md) - 일반적인 문제 및 해결방법
- 📝 [Changelog](docs/CHANGELOG.md) - 릴리스 히스토리 및 변경사항

---

## Configuration 템플릿

Logger system은 일반적인 시나리오를 위한 사전 정의된 템플릿을 제공합니다:

```cpp
// Production: 프로덕션 환경을 위해 최적화
auto logger = kcenon::logger::logger_builder()
    .use_template("production")
    .build()
    .value();

// Debug: 개발을 위한 즉시 출력
auto logger = kcenon::logger::logger_builder()
    .use_template("debug")
    .build()
    .value();

// High-performance: 최대화된 처리량
auto logger = kcenon::logger::logger_builder()
    .use_template("high_performance")
    .build()
    .value();

// Low-latency: 실시간 시스템을 위한 최소화된 지연시간
auto logger = kcenon::logger::logger_builder()
    .use_template("low_latency")
    .build()
    .value();
```

### 고급 Configuration

```cpp
auto logger = kcenon::logger::logger_builder()
    // 핵심 설정
    .with_min_level(kcenon::logger::log_level::info)
    .with_buffer_size(16384)
    .with_batch_size(200)
    .with_queue_size(20000)

    // 여러 writer 추가
    .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
    .add_writer("file", std::make_unique<kcenon::logger::rotating_file_writer>(
        "app.log",
        10 * 1024 * 1024,  // 파일당 10MB
        5                   // 5개 파일 유지
    ))

    // Validation과 함께 빌드
    .build()
    .value();
```

[📚 완전한 Configuration 가이드 →](docs/guides/CONFIGURATION.md)

---

## 빌드 Configuration

### CMake Feature Flag

```bash
# 핵심 기능
cmake -DLOGGER_USE_DI=ON              # Dependency injection (기본값: ON)
cmake -DLOGGER_USE_MONITORING=ON      # Monitoring 지원 (기본값: ON)
cmake -DLOGGER_ENABLE_ASYNC=ON        # Async 로깅 (기본값: ON)
cmake -DLOGGER_ENABLE_CRASH_HANDLER=ON # Crash handler (기본값: ON)

# 고급 기능
cmake -DLOGGER_ENABLE_STRUCTURED_LOGGING=ON # JSON 로깅 (기본값: OFF)
cmake -DLOGGER_ENABLE_NETWORK_WRITER=ON # 네트워크 writer (기본값: OFF)
cmake -DLOGGER_ENABLE_FILE_ROTATION=ON  # 파일 rotation (기본값: ON)

# 성능 튜닝
cmake -DLOGGER_DEFAULT_BUFFER_SIZE=16384 # 바이트 단위 버퍼 크기
cmake -DLOGGER_DEFAULT_BATCH_SIZE=200    # 배치 처리 크기
cmake -DLOGGER_DEFAULT_QUEUE_SIZE=20000  # 최대 큐 크기

# 품질 보증
cmake -DLOGGER_ENABLE_SANITIZERS=ON   # Sanitizer 활성화
cmake -DLOGGER_ENABLE_COVERAGE=ON     # 코드 커버리지
cmake -DLOGGER_WARNINGS_AS_ERRORS=ON  # 경고를 에러로 처리
```

[🔧 완전한 빌드 옵션 →](docs/guides/BUILD_GUIDE.md)

---

## 플랫폼 지원

### 공식 지원

| 플랫폼 | 아키텍처 | 컴파일러 | 상태 |
|--------|----------|----------|------|
| **Ubuntu 22.04+** | x86_64, ARM64 | GCC 11+, Clang 15+ | ✅ 완전 테스트 |
| **macOS Sonoma+** | x86_64, ARM64 (M1/M2) | Apple Clang 15+ | ✅ 완전 테스트 |
| **Windows 11** | x86_64 | MSVC 2022, MSYS2 | ✅ 완전 테스트 |

**최소 요구사항**:
- C++17 컴파일러
- CMake 3.20+
- fmt 라이브러리

[🖥️ 플랫폼 세부사항 →](docs/PRODUCTION_QUALITY.md#platform-support)

---

## 테스트

Logger system은 포괄적인 테스트 인프라를 포함합니다:

### 테스트 커버리지

- **Unit 테스트**: 150+ 테스트 케이스 (GTest)
- **Integration 테스트**: 30+ 시나리오
- **벤치마크**: 20+ 성능 테스트
- **커버리지**: ~65% (성장 중)

### 테스트 실행

```bash
# 테스트와 함께 빌드
cmake -DBUILD_TESTS=ON ..
cmake --build .

# 모든 테스트 실행
ctest --output-on-failure

# 특정 테스트 suite 실행
./build/bin/core_tests
./build/bin/writer_tests

# 벤치마크 실행
./build/bin/benchmarks
```

### CI/CD 상태

모든 파이프라인 정상:
- ✅ 멀티 플랫폼 빌드 (Ubuntu, macOS, Windows)
- ✅ Sanitizer (Thread, Address, UB)
- ✅ 성능 벤치마크
- ✅ 코드 커버리지
- ✅ 정적 분석 (clang-tidy, cppcheck)

[✅ 프로덕션 품질 지표 →](docs/PRODUCTION_QUALITY.md)

---

## 기여하기

기여를 환영합니다! 자세한 내용은 [Contributing Guide](docs/CONTRIBUTING.md)를 참조하세요.

### 개발 워크플로우

1. Repository를 fork하세요
2. Feature branch를 생성하세요: `git checkout -b feature/amazing-feature`
3. 변경사항을 커밋하세요: `git commit -m 'Add amazing feature'`
4. Branch에 push하세요: `git push origin feature/amazing-feature`
5. Pull Request를 여세요

### 코드 표준

- 최신 C++ 모범 사례를 따르세요
- RAII 및 smart pointer를 사용하세요
- 포괄적인 unit 테스트를 작성하세요
- 일관된 포매팅 유지 (clang-format)
- Public API 문서화

[🤝 Contributing 가이드라인 →](docs/CONTRIBUTING.md)

---

## 지원

- **Issues**: [GitHub Issues](https://github.com/kcenon/logger_system/issues)
- **Discussions**: [GitHub Discussions](https://github.com/kcenon/logger_system/discussions)
- **Email**: kcenon@naver.com

---

## 라이선스

이 프로젝트는 BSD 3-Clause License에 따라 라이선스가 부여됩니다 - 자세한 내용은 [LICENSE](LICENSE) 파일을 참조하세요.

---

## 감사의 말

- 이 프로젝트 개선에 도움을 주신 모든 기여자분들께 감사드립니다
- 지속적인 피드백과 지원을 해주신 C++ 커뮤니티에 특별히 감사드립니다
- 최신 로깅 프레임워크 (spdlog, Boost.Log, glog)에서 영감을 받았습니다

---

<p align="center">
  Made with ❤️ by 🍀☀🌕🌥 🌊
</p>
