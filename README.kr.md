[![CI](https://github.com/kcenon/logger_system/actions/workflows/ci.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/ci.yml)
[![Sanitizers](https://github.com/kcenon/logger_system/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/sanitizers.yml)
[![Benchmarks](https://github.com/kcenon/logger_system/actions/workflows/benchmarks.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/benchmarks.yml)
[![Code Coverage](https://github.com/kcenon/logger_system/actions/workflows/coverage.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/coverage.yml)
[![Static Analysis](https://github.com/kcenon/logger_system/actions/workflows/static-analysis.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/static-analysis.yml)
[![codecov](https://codecov.io/gh/kcenon/logger_system/branch/main/graph/badge.svg)](https://codecov.io/gh/kcenon/logger_system)
[![Docs](https://github.com/kcenon/logger_system/actions/workflows/build-Doxygen.yaml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/build-Doxygen.yaml)
[![License](https://img.shields.io/github/license/kcenon/logger_system)](https://github.com/kcenon/logger_system/blob/main/LICENSE)

# Logger System

> **Language:** [English](README.md) | **한국어**

멀티스레드 애플리케이션을 위해 설계된 고성능 C++20 비동기 로깅 프레임워크입니다.

## 목차

- [개요](#개요)
- [주요 기능](#주요-기능)
- [요구사항](#요구사항)
- [빠른 시작](#빠른-시작)
- [설치](#설치)
- [아키텍처](#아키텍처)
- [핵심 개념](#핵심-개념)
- [API 개요](#api-개요)
- [예제](#예제)
- [성능](#성능)
- [생태계 통합](#생태계-통합)
- [기여하기](#기여하기)
- [라이선스](#라이선스)

---

## 개요

Logger System은 모듈식 인터페이스 기반 아키텍처와 원활한 생태계 통합을 갖춘 고성능 비동기 로깅 프레임워크입니다.

**핵심 가치**:
- **초고속 비동기 로깅**: 초당 4.34M 메시지, 148ns 지연
- **설계 기반 스레드 안전성**: 데이터 레이스 제로, 프로덕션 검증
- **모듈식 아키텍처**: 인터페이스 기반, 플러그인 가능한 컴포넌트
- **프로덕션 등급**: 포괄적 CI/CD, sanitizer, 벤치마크
- **보안 우선**: 경로 검증, 보안 저장소, 감사 로깅
- **크로스 플랫폼**: Windows, Linux, macOS (GCC, Clang, MSVC)

---

## 주요 기능

| 기능 | 설명 | 상태 |
|------|------|------|
| **비동기 로깅** | 백그라운드 스레드의 논블로킹 I/O | 안정 |
| **콘솔 Writer** | ANSI 컬러 레벨별 출력 | 안정 |
| **파일 Writer** | 로테이팅, 배치, 비동기 파일 기록 | 안정 |
| **네트워크 Writer** | 원격 로그 수집기 전송 | 안정 |
| **암호화 Writer** | AES-256-GCM 암호화 로그 | 안정 |
| **OTLP Writer** | OpenTelemetry 내보내기 | 안정 |
| **Writer Builder** | 데코레이터 패턴 플루언트 API | 안정 |
| **구조화 로깅** | 키-값 쌍 기반 로그 항목 | 안정 |
| **구성 템플릿** | 사전 정의된 프로덕션/개발 구성 | 안정 |
| **C++20 모듈** | 헤더 기반 인터페이스 대안 | 실험적 |

---

## 요구사항

### 컴파일러 매트릭스

| 컴파일러 | 최소 버전 | 비고 |
|----------|----------|------|
| GCC | 11+ | C++20 기능 필수 |
| Clang | 14+ | C++20 기능 필수 |
| Apple Clang | 14+ | macOS 지원 |
| MSVC | 2022+ | C++20 기능 필수 |

### 빌드 도구 및 의존성

| 의존성 | 버전 | 필수 | 설명 |
|--------|------|------|------|
| CMake | 3.20+ | 예 | 빌드 시스템 |
| [common_system](https://github.com/kcenon/common_system) | latest | 예 | 공통 인터페이스 (ILogger, Result<T>) |
| [thread_system](https://github.com/kcenon/thread_system) | latest | 아니오 | 스레드 풀 기반 비동기 로깅 |

### 선택적 기능 의존성

| 기능 | 의존성 | 목적 |
|------|--------|------|
| `encryption` | OpenSSL | AES-256-GCM 암호화 로그 Writer |
| `otlp` | OpenTelemetry C++ SDK, gRPC, Protocol Buffers | OTLP 텔레메트리 내보내기 |
| `benchmarks` | spdlog | 벤치마크 비교 |
| `thread-system` | kcenon-thread-system | 스레드 풀 및 비동기 실행기 통합 |

---

## 빠른 시작

```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>

int main() {
    // 빌더 패턴으로 logger 생성 (자동 검증)
    auto result = kcenon::logger::logger_builder()
        .use_template("production")  // 사전 정의된 구성
        .with_min_level(kcenon::logger::log_level::info)
        .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
        .add_writer("file", std::make_unique<kcenon::logger::file_writer>("app.log"))
        .build();

    if (result.is_err()) {
        const auto& err = result.error();
        std::cerr << "Logger 생성 실패: " << err.message << "\n";
        return -1;
    }

    auto logger = std::move(result.value());
    logger->log(kcenon::logger::log_level::info, "Application started");
    return 0;
}
```

---

## 설치

### vcpkg를 통한 설치

```bash
# 기본 기능 세트로 설치
vcpkg install kcenon-logger-system

# 벤치마크 포함 (spdlog 비교)
vcpkg install kcenon-logger-system[benchmarks]

# OpenTelemetry 통합
vcpkg install kcenon-logger-system[otlp]

# 암호화 지원
vcpkg install kcenon-logger-system[encryption]
```

### CMake를 통한 설치

```bash
mkdir build && cd build
cmake ..
cmake --build .
cmake --build . --target install
```

프로젝트에서 사용:
```cmake
find_package(LoggerSystem REQUIRED)
target_link_libraries(your_app PRIVATE LoggerSystem::logger)
```

### 의존성과 함께 빌드

```bash
# 의존성 클론
git clone https://github.com/kcenon/common_system.git
git clone https://github.com/kcenon/thread_system.git  # 선택 사항

# logger_system 클론 및 빌드
git clone https://github.com/kcenon/logger_system.git
cd logger_system
cmake -B build -DLOGGER_USE_THREAD_SYSTEM=ON
cmake --build build
```

---

## 아키텍처

### 모듈 구조

```
include/kcenon/logger/
  core/          - Logger, logger_builder, logger_config, metrics
  interfaces/    - log_writer_interface, log_filter_interface, log_formatter_interface
  writers/       - 17개 Writer 구현 (console, file, rotating_file, async,
                   buffered, batch, encrypted, otlp, network 등)
  builders/      - writer_builder (데코레이터 조합 플루언트 API)
  backends/      - standalone_backend, integration_backend, monitoring_backend
  formatters/    - Log 포매터 구현
  filters/       - Log 필터 구현
  security/      - 보안 키 저장소, 경로 검증, 감사 로깅
  adapters/      - common_logger_adapter
```

### 의존성 흐름

```
logger_system
+-- common_system (필수)
+-- thread_system (선택, 스레드 풀 기반 비동기 로깅)
    +-- common_system (필수)
```

---

## 핵심 개념

### Writer Builder (데코레이터 패턴)

`writer_builder`는 여러 Writer 동작을 데코레이터 패턴으로 조합하는 플루언트 API를 제공합니다:

```cpp
#include <kcenon/logger/builders/writer_builder.h>

auto writer = kcenon::logger::writer_builder()
    .file("app.log")
    .buffered(500)           // 최대 500개 항목 버퍼링
    .async(20000)            // 비동기 큐 크기 20000
    .build();
```

**코어 Writer**: `.file(path)`, `.console()`, `.custom(writer)`

**데코레이터**: `.async(queue_size)`, `.buffered(max_entries)`, `.filtered(filter)`, `.formatted(formatter)`, `.encrypted(key)`, `.thread_safe()`

### 데코레이터 적용 순서

최적 성능을 위해 데코레이터는 다음 순서로 적용합니다:

```
Core Writer -> Filtering -> Buffering -> Encryption -> Thread-Safety -> Async
```

1. **Filtering 먼저** - 다운스트림 데코레이터의 작업량 감소
2. **Buffering** - I/O 및 처리 비용 분산
3. **Encryption** - 배치 단위 효율적 암호화
4. **Thread-safety** - 비동기 처리 전 일관성 보장
5. **Async 마지막** - 논블로킹 이점 극대화

### 구성 템플릿

사전 정의된 구성으로 빠른 설정:

```cpp
auto result = kcenon::logger::logger_builder()
    .use_template("production")
    .build();
```

### OpenTelemetry 통합

OTLP 프로토콜을 통한 텔레메트리 내보내기를 지원합니다. 배치 내보내기, 재시도, 트레이스 상관관계를 포함합니다.

---

## API 개요

| API | 헤더 | 설명 |
|-----|------|------|
| `logger` | `core/logger.h` | 핵심 로거 클래스 |
| `logger_builder` | `core/logger_builder.h` | 빌더 패턴 로거 생성 |
| `writer_builder` | `builders/writer_builder.h` | 데코레이터 체인 빌더 |
| `console_writer` | `writers/console_writer.h` | ANSI 컬러 콘솔 출력 |
| `file_writer` | `writers/file_writer.h` | 파일 기록 |
| `rotating_file_writer` | `writers/rotating_file_writer.h` | 로테이팅 파일 기록 |
| `async_writer` | `writers/async_writer.h` | 비동기 기록 |
| `batch_writer` | `writers/batch_writer.h` | 배치 기록 |
| `network_writer` | `writers/network_writer.h` | 네트워크 전송 |

---

## 예제

| 예제 | 난이도 | 설명 |
|------|--------|------|
| [basic_example](examples/basic_example.cpp) | 초급 | 기본 콘솔 및 파일 로깅 |
| [decorator_usage](examples/decorator_usage.cpp) | 중급 | 데코레이터 패턴 가이드 |
| [writer_builder_example](examples/writer_builder_example.cpp) | 중급 | 빌더 패턴 예제 |

### 일반적인 사용 패턴

**고처리량 패턴** (4M+ 메시지/초):
```cpp
auto writer = kcenon::logger::writer_builder()
    .file("app.log")
    .buffered(1000)
    .async(50000)
    .build();
```

**보안 로깅 패턴**:
```cpp
auto writer = kcenon::logger::writer_builder()
    .file("audit.log.enc")
    .buffered(200)
    .encrypted(std::move(encryption_key))
    .async(10000)
    .build();
```

---

## 성능

| 메트릭 | 값 | 구성 |
|--------|------|------|
| **처리량** | 4.34M msg/s | 비동기 + 버퍼링 |
| **지연** | 148 ns | 단일 메시지 |
| **배치 효율** | 3.8x 향상 | 500 항목 배치 |
| **메모리 사용** | <2 MB | 기본 구성 |

### 품질 메트릭

- 65% 코드 커버리지
- ThreadSanitizer / AddressSanitizer 클린
- 다중 플랫폼 CI/CD (Ubuntu, macOS, Windows)
- 정적 분석 (clang-tidy, cppcheck)

---

## 생태계 통합

### 의존성 계층

```
common_system (Tier 0) [필수] -- ILogger, Result<T>, Concepts
thread_system (Tier 1) [선택] -- 스레드 풀 기반 비동기 I/O
```

### 통합 프로젝트

| 프로젝트 | 설명 | logger_system 역할 |
|----------|------|-------------------|
| [common_system](https://github.com/kcenon/common_system) | C++20 기반 유틸리티 | 필수 의존성 |
| [thread_system](https://github.com/kcenon/thread_system) | 멀티스레딩 프레임워크 | 선택적 비동기 백엔드 |
| [monitoring_system](https://github.com/kcenon/monitoring_system) | 관측성 플랫폼 | 로깅 제공 |
| [network_system](https://github.com/kcenon/network_system) | 네트워크 라이브러리 | 로깅 제공 |

### 플랫폼 지원

| 플랫폼 | 컴파일러 | 상태 |
|--------|----------|------|
| **Linux** | GCC 11+, Clang 14+ | 완전 지원 |
| **macOS** | Apple Clang 14+ | 완전 지원 |
| **Windows** | MSVC 2022+ | 완전 지원 |

---

## 기여하기

기여를 환영합니다! 자세한 내용은 [기여 가이드](docs/contributing/CONTRIBUTING.md)를 참조하세요.

1. 리포지토리 포크
2. 기능 브랜치 생성
3. 테스트와 함께 변경 사항 작성
4. 로컬에서 테스트 실행
5. Pull Request 열기

---

## 라이선스

이 프로젝트는 BSD 3-Clause 라이선스에 따라 배포됩니다 - 자세한 내용은 [LICENSE](LICENSE) 파일을 참조하세요.

---

<p align="center">
  Made with ❤️ by 🍀☀🌕🌥 🌊
</p>
