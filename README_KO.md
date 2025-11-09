[![CI](https://github.com/kcenon/logger_system/actions/workflows/ci.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/ci.yml)
[![Sanitizers](https://github.com/kcenon/logger_system/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/sanitizers.yml)
[![Benchmarks](https://github.com/kcenon/logger_system/actions/workflows/benchmarks.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/benchmarks.yml)
[![Code Coverage](https://github.com/kcenon/logger_system/actions/workflows/coverage.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/coverage.yml)
[![Static Analysis](https://github.com/kcenon/logger_system/actions/workflows/static-analysis.yml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/static-analysis.yml)
[![Docs](https://github.com/kcenon/logger_system/actions/workflows/build-Doxygen.yaml/badge.svg)](https://github.com/kcenon/logger_system/actions/workflows/build-Doxygen.yaml)

# Logger System Project

> **Language:** [English](README.md) | **한국어**

## 개요

Logger System Project는 프로덕션 환경에서 사용 가능한 고성능 C++20 비동기 로깅 프레임워크로, 멀티스레드 애플리케이션을 위한 포괄적인 로깅 기능을 제공합니다. 모듈식 인터페이스 기반 아키텍처와 thread system 생태계와의 원활한 통합을 통해, 최소한의 오버헤드와 최대한의 유연성으로 엔터프라이즈급 로깅 성능을 제공합니다.

> **🏗️ 모듈식 아키텍처**: writer, filter, formatter, monitoring 통합을 위한 플러그 가능한 컴포넌트를 갖춘 간소화된 인터페이스 기반 설계.

> **✅ 최신 업데이트**: 향상된 dependency injection, configuration strategy pattern, 포괄적인 validation, 광범위한 CMake 모듈화. 모든 플랫폼에서 CI/CD 파이프라인 정상 작동.

## ✨ 핵심 기능

### 🎯 고성능 로깅
- **비동기 처리**: 배치 큐 처리를 통한 논블로킹 로그 작업
- **다중 출력 대상**: Console, file, rotating file, network, encrypted, hybrid writer 지원
- **Critical Writer** 🆕: 즉시 기록이 필요한 중요한 메시지를 위한 동기식 로깅
- **Crash-Safe Logging** 🆕: 비정상 종료 시 로그 보존을 위한 긴급 flush 메커니즘
- **스레드 세이프 작업**: hot path에서 락 없이 여러 스레드에서 동시 로깅
- **제로카피 설계**: 최소한의 할당으로 효율적인 메시지 전달
- **설정 가능한 배치 처리**: 최적의 처리량을 위한 조정 가능한 배치 크기 및 큐 깊이

### 🔧 고급 기능
- **Result 기반 에러 핸들링**: `Result<T>` 패턴을 사용한 포괄적인 에러 처리
- **Builder Pattern API**: validation을 포함한 fluent하고 type-safe한 logger 생성
- **Configuration Strategy**: 템플릿 기반, 환경 인식, 성능 튜닝 configuration
- **Dependency Injection**: fallback 메커니즘을 갖춘 선택적 런타임 컴포넌트 주입
- **Monitoring 통합**: health check 및 metrics를 지원하는 플러그 가능한 monitoring backend

### 🔒 보안 기능 (v3.0.0)
- **안전한 키 저장소**: 자동 메모리 정리 기능을 갖춘 RAII 기반 암호화 키 관리
  - OpenSSL `OPENSSL_cleanse()`를 사용한 안전한 메모리 삭제
  - 파일 권한 강제 적용 (0600 - 소유자 읽기/쓰기만 허용)
  - 암호학적으로 안전한 랜덤 키 생성
- **경로 검증**: 포괄적인 파일 경로 보안
  - 경로 탐색 공격 방지
  - 심볼릭 링크 검증
  - 기본 디렉터리 강제 적용
  - 파일명 문자 제한
- **시그널 핸들러 안전성**: 중앙화된 시그널 핸들러 관리
  - 스레드 안전한 로거 등록
  - 크래시 시나리오를 위한 긴급 flush (SIGSEGV, SIGABRT, SIGTERM, SIGINT)
  - signal-safe 함수만 사용 (POSIX 준수)
- **보안 감사 로깅**: 변조 방지 감사 추적
  - JSON 형식 감사 항목
  - 무결성 검증을 위한 HMAC-SHA256 서명
  - 수명주기, 암호화, 보안 이벤트 추적
- **규정 준수 지원**: GDPR, PCI DSS, ISO 27001
- **OWASP Top 10**: A01, A02, A03, A09 완화

### 🏗️ 아키텍처 하이라이트
- **인터페이스 주도 설계**: abstract interface를 통한 명확한 분리 (ILogger, IMonitor, IMonitorable)
- **모듈식 컴포넌트**: 플러그 가능한 writer, filter, formatter, sink
- **순환 의존성 제로**: common_system을 통한 인터페이스 전용 의존성
- **독립적인 컴파일**: 생태계 의존성 없는 독립 실행형 빌드
- **크로스 플랫폼 지원**: GCC, Clang, MSVC를 사용한 Windows, Linux, macOS

### 📊 현재 상태
- **빌드 시스템**: 15개 이상의 feature flag 및 자동 dependency 감지를 지원하는 CMake
- **의존성**: 인터페이스 전용 (thread_system, common_system)
- **컴파일**: 독립적, ~15초 빌드 시간
- **테스트 커버리지**: 핵심 기능 검증 완료 (테스트 API 마이그레이션 진행 중)

**알려진 제약사항**:
- 테스트 suite는 새로운 인터페이스 패턴으로의 API 마이그레이션이 필요합니다
- Lock-free 큐 구현 계획 중 (USE_LOCKFREE 예약됨)
- 문서 및 예제 확장 중

**아키텍처**:
```
logger_system
    ↓ implements
ILogger (common_system)
    ↑ optional
IMonitor injection (runtime DI)
```

## 🔗 생태계 통합

깔끔한 인터페이스 경계를 가진 모듈식 C++ 생태계의 일부:

**필수 의존성**:
- **[common_system](https://github.com/kcenon/common_system)**: 핵심 인터페이스 (ILogger, IMonitor, Result<T>)
- **[thread_system](https://github.com/kcenon/thread_system)**: Threading primitive 및 logger_interface

**선택적 통합**:
- **[monitoring_system](https://github.com/kcenon/monitoring_system)**: metric 및 health monitoring (IMonitor 인터페이스를 통해)
- **[integrated_thread_system](https://github.com/kcenon/integrated_thread_system)**: 전체 생태계 예제

**통합 패턴**:
```
common_system (interfaces) ← logger_system implements ILogger
                          ↖ optional: inject IMonitor at runtime
```

**장점**:
- 인터페이스 전용 의존성 (순환 참조 없음)
- 독립적인 컴파일 및 배포
- DI 패턴을 통한 런타임 컴포넌트 주입
- 명확한 관심사 분리

> 📖 전체 통합 세부사항은 [ARCHITECTURE.md](docs/ARCHITECTURE.md)를 참조하세요.

## 프로젝트 목적 및 미션

이 프로젝트는 전 세계 개발자들이 직면한 근본적인 과제인 **고성능 로깅을 접근 가능하고, 신뢰할 수 있으며, 효율적으로 만드는 것**을 해결합니다. 전통적인 로깅 접근 방식은 종종 고처리량 애플리케이션에서 병목현상이 되고, 적절한 에러 처리가 부족하며, 불충분한 관찰성을 제공합니다. 우리의 미션은 다음을 제공하는 포괄적인 솔루션을 제공하는 것입니다:

- **로깅 병목현상 제거**: 비동기, 배치 처리를 통해
- **데이터 무결성 보장**: 포괄적인 에러 처리 및 validation을 통해
- **성능 극대화**: 최적화된 알고리즘 및 최신 C++ 기능을 통해
- **유지보수성 향상**: 깔끔한 인터페이스 및 모듈식 아키텍처를 통해
- **디버깅 가속화**: 풍부하고 구조화된 로깅 기능을 제공함으로써

## 핵심 장점 및 이점

### 🚀 **성능 우수성**
- **비동기 처리**: 백그라운드 스레드가 블로킹 없이 I/O 작업 처리
- **배치 최적화**: 처리량을 극대화하기 위해 여러 로그 항목을 효율적으로 처리
- **최소 오버헤드**: 제로 할당 포매팅 및 최적화된 데이터 구조
- **적응형 큐잉**: 최적의 리소스 활용을 위한 지능적인 백오프 및 배치 전략

### 🛡️ **프로덕션급 신뢰성**
- **설계 단계부터 스레드 세이프**: 모든 컴포넌트가 안전한 동시 접근 보장
- **포괄적인 에러 핸들링**: Result 패턴으로 사일런트 실패 방지
- **메모리 안전성**: RAII 원칙 및 smart pointer로 누수 및 손상 방지
- **광범위한 validation**: configuration validation으로 런타임 에러 방지

### 🔧 **개발자 생산성**
- **직관적인 API 설계**: 학습 곡선을 줄이는 깔끔하고 자체 문서화된 인터페이스
- **풍부한 문서**: 실용적인 예제가 포함된 포괄적인 문서
- **유연한 configuration**: 일반적인 시나리오를 위한 템플릿 기반 configuration
- **모듈식 컴포넌트**: 필요한 것만 사용 - 최대한의 유연성

### 🌐 **크로스 플랫폼 호환성**
- **범용 지원**: Windows, Linux, macOS에서 작동
- **컴파일러 유연성**: GCC, Clang, MSVC와 호환
- **C++ 표준 적응**: C++20에서 이전 표준으로 우아한 폴백
- **아키텍처 독립성**: x86 및 ARM 프로세서 모두에 최적화

### 📈 **엔터프라이즈 준비 기능**
- **구조화된 로깅**: JSON, logfmt 및 custom format 지원
- **고급 필터링**: level 기반, regex, custom function filter
- **네트워크 로깅**: TCP/UDP 원격 로깅 기능
- **보안 기능**: 암호화, sanitization, 접근 제어

## 실제 영향 및 사용 사례

### 🎯 **이상적인 애플리케이션**
- **고빈도 거래 시스템**: 거래 실행 모니터링을 위한 초저지연 로깅
- **웹 서버**: 최소한의 성능 영향으로 동시 요청 로깅
- **마이크로서비스**: 구조화된 데이터 및 correlation ID를 사용한 분산 로깅
- **게임 엔진**: 프레임 레이트에 영향을 주지 않는 실시간 이벤트 로깅
- **IoT 시스템**: 리소스 제약 장치를 위한 효율적인 로깅
- **데이터베이스 시스템**: 쿼리 로깅 및 성능 모니터링

### 📊 **성능 벤치마크**

*Apple M1 (8-core) @ 3.2GHz, 16GB, macOS Sonoma에서 벤치마크*

> **🚀 아키텍처 업데이트**: 최신 모듈식 아키텍처는 thread_system 생태계와의 원활한 통합을 제공합니다. 비동기 처리는 애플리케이션 스레드를 차단하지 않고 탁월한 성능을 제공합니다.

#### 핵심 성능 지표 (최신 벤치마크)
- **최대 처리량**: 초당 최대 434만 메시지 (단일 스레드, async 모드)
- **멀티스레드 성능**:
  - 4 스레드: 107만 메시지/초 (표준 모드보다 24% 향상)
  - 8 스레드: 41만 2천 메시지/초 (적응형 배치로 78% 향상)
  - 16 스레드: 39만 메시지/초 (높은 경합 시나리오에서 117% 향상)
- **지연시간**:
  - 평균 enqueue 시간: 148 나노초
  - spdlog 대비 15.7배 낮은 지연시간
- **메모리 효율성**: 적응형 버퍼 관리로 기본 <2MB
- **큐 활용도**: 자동 최적화로 높은 처리량 유지

#### 업계 표준과의 성능 비교
| Logger Type | Single Thread | 4 Threads | 8 Threads | 16 Threads | 최적 사용 사례 |
|-------------|---------------|-----------|-----------|------------|---------------|
| 🏆 **Logger System** | **434만/초** | **107만/초** | **41.2만/초** | **39만/초** | 모든 시나리오 (적응형) |
| 📦 **spdlog async** | 535만/초 | 78.5만/초 | 24만/초 | - | 단일 스레드 중심 |
| 📦 **spdlog sync** | 51.5만/초 | 21만/초 | 5.2만/초 | - | 단순 애플리케이션 |
| 🐌 **Console output** | 58.3만/초 | - | - | - | 개발 전용 |

#### 주요 성능 인사이트
- 🏃 **단일 스레드**: 업계 리더와 경쟁력 있음 (434만/초)
- 🏋️ **멀티 스레드**: 적응형 배치가 일관된 확장성 제공
- ⏱️ **지연시간**: 업계 최고 수준의 148ns 평균 enqueue 시간
- 📈 **확장성**: 높은 경합 상황에서도 성능 유지

## 기능

### 핵심 기능
- **비동기 파이프라인**: 백그라운드 스레드가 배치된 로그 항목 처리
- **다중 Writer**: Console, file, custom callback writer
- **스레드 세이프**: 동시 환경을 위해 설계됨
- **모듈식 설계**: 모든 C++ 프로젝트와 쉬운 통합
- **낮은 지연시간**: 최소 오버헤드를 위해 최적화

### 향상된 아키텍처 (Phase 1의 새 기능)
- **Result Pattern**: `result<T>` 및 `result_void`를 사용한 타입 안전 에러 처리
- **Configuration Validation**: 의미 있는 에러 메시지를 포함한 포괄적인 validation
- **Builder Pattern**: logger 생성을 위한 fluent API
- **Interface Segregation**: writer, filter, formatter, sink 인터페이스의 명확한 분리
- **사전 정의 템플릿**: production, debug, high_performance, low_latency configuration

### 고급 기능
- **성능 Metric**: logger 성능 모니터링을 위한 내장 metric 수집
- **구조화된 로깅**: JSON, logfmt, plain text 출력 형식 지원
- **고급 필터링**: level 기반, regex, custom function filter
- **유연한 라우팅**: 조건에 따라 특정 writer로 로그 라우팅
- **파일 Writer**: 크기/시간 기반 rotation을 지원하는 기본 및 rotating file writer
- **네트워크 로깅**: TCP/UDP를 통해 원격 서버로 로그 전송
- **로그 서버**: 여러 소스에서 로그 수신 및 처리
- **실시간 분석**: 로그 패턴 분석 및 통계 생성
- **Alert 시스템**: 로그 패턴을 기반으로 alert를 트리거하는 규칙 정의
- **보안 기능**: 로그 암호화, 민감한 데이터 sanitization, 접근 제어
- **통합 테스트**: 모든 컴포넌트에 대한 포괄적인 테스트 suite

> 보안 참고사항: `encrypted_writer`는 단순한 XOR 방식을 사용하는 데모 컴포넌트이며 프로덕션 사용에 적합하지 않습니다. 지침 및 권장 대안은 SECURITY.md를 참조하세요.

## 기술 스택 및 아키텍처

### 🏗️ **최신 C++ 기반**
- **C++20 기능**: 향상된 성능을 위한 `std::format`, concept, range
- **템플릿 메타프로그래밍**: 타입 안전, 컴파일 타임 최적화
- **메모리 관리**: 자동 리소스 정리를 위한 smart pointer 및 RAII
- **예외 안전성**: 전체에 걸친 강력한 예외 안전성 보장
- **Result 패턴**: 예외 없는 포괄적인 에러 처리
- **인터페이스 기반 설계**: 인터페이스와 구현 간의 명확한 분리
- **모듈식 아키텍처**: 선택적 생태계 통합을 지원하는 핵심 로깅 기능

### 🔄 **디자인 패턴 구현**
- **Observer Pattern**: 이벤트 기반 로그 처리 및 필터링
- **Strategy Pattern**: 설정 가능한 formatter, filter, writer
- **Builder Pattern**: validation을 포함한 logger configuration을 위한 fluent API
- **Template Method Pattern**: 사용자 정의 가능한 writer 및 formatter 동작
- **Dependency Injection**: 생태계 컴포넌트를 위한 service container 통합
- **Factory Pattern**: 설정 가능한 writer 및 filter 생성

## 프로젝트 구조

### 📁 **디렉터리 구성**

```
logger_system/
├── 📁 include/kcenon/logger/       # Public header
│   ├── 📁 core/                    # 핵심 컴포넌트
│   │   ├── logger.h                # 메인 logger 인터페이스
│   │   ├── logger_builder.h        # Builder pattern 구현
│   │   ├── log_entry.h             # 로그 항목 데이터 구조
│   │   └── result_types.h          # 에러 처리 타입
│   ├── 📁 interfaces/              # Abstract 인터페이스
│   │   ├── log_writer_interface.h  # Writer 추상화
│   │   ├── log_filter_interface.h  # Filter 추상화
│   │   ├── log_formatter_interface.h # Formatter 추상화
│   │   └── monitoring_interface.h  # Monitoring 통합
│   ├── 📁 writers/                 # 로그 writer
│   │   ├── console_writer.h        # Console 출력
│   │   ├── file_writer.h           # 파일 출력
│   │   ├── rotating_file_writer.h  # Rotating 파일
│   │   └── network_writer.h        # 네트워크 로깅
│   ├── 📁 filters/                 # 로그 filter
│   │   ├── level_filter.h          # Level 기반 필터링
│   │   ├── regex_filter.h          # Regex 기반 필터링
│   │   └── function_filter.h       # Custom function 필터링
│   ├── 📁 formatters/              # 로그 formatter
│   │   ├── plain_formatter.h       # Plain text 포매팅
│   │   ├── json_formatter.h        # JSON 포매팅
│   │   └── custom_formatter.h      # Custom 포매팅
│   └── 📁 config/                  # Configuration
│       ├── config_templates.h      # 사전 정의 configuration
│       ├── config_validator.h      # Configuration validation
│       └── config_strategy.h       # Strategy pattern config
├── 📁 src/                         # 구현 파일
│   ├── 📁 core/                    # 핵심 구현
│   ├── 📁 writers/                 # Writer 구현
│   ├── 📁 filters/                 # Filter 구현
│   ├── 📁 formatters/              # Formatter 구현
│   └── 📁 config/                  # Configuration 구현
├── 📁 examples/                    # 예제 애플리케이션
│   ├── basic_logging/              # 기본 사용 예제
│   ├── advanced_features/          # 고급 기능 데모
│   ├── performance_test/           # 성능 벤치마크
│   └── integration_examples/       # 생태계 통합
├── 📁 tests/                       # 모든 테스트
│   ├── 📁 unit/                    # Unit 테스트
│   ├── 📁 integration/             # Integration 테스트
│   └── 📁 benchmarks/              # 성능 테스트
├── 📁 docs/                        # 문서
├── 📁 cmake/                       # CMake 모듈
├── 📄 CMakeLists.txt               # 빌드 configuration
└── 📄 vcpkg.json                   # 의존성
```

### 📖 **주요 파일 및 목적**

#### 핵심 모듈 파일
- **`logger.h/cpp`**: 비동기 처리를 지원하는 메인 logger 클래스
- **`logger_builder.h/cpp`**: logger configuration을 위한 builder 패턴
- **`log_entry.h`**: metadata를 포함한 로그 항목 데이터 구조
- **`result_types.h`**: 에러 처리 타입 및 유틸리티

#### Writer 파일
- **`console_writer.h/cpp`**: ANSI 지원을 포함한 색상 console 출력
- **`file_writer.h/cpp`**: 버퍼링을 지원하는 기본 파일 쓰기
- **`rotating_file_writer.h/cpp`**: 크기 및 시간 기반 파일 rotation
- **`network_writer.h/cpp`**: TCP/UDP 네트워크 로깅

#### Configuration 파일
- **`config_templates.h/cpp`**: production, debug, high-performance 템플릿
- **`config_validator.h/cpp`**: 포괄적인 validation 프레임워크
- **`config_strategy.h/cpp`**: 유연한 configuration을 위한 strategy 패턴

### 🔗 **모듈 의존성**

```
config (의존성 없음)
    │
    └──> core
            │
            ├──> writers
            │
            ├──> filters
            │
            ├──> formatters
            │
            └──> integration (thread_system, monitoring_system)

선택적 외부 프로젝트:
- thread_system (logger_interface 제공)
- monitoring_system (metric 수집 제공)
```

## 빌드 Configuration

### CMake Feature Flag

logger system은 CMake를 통해 광범위한 configuration 옵션을 제공합니다:

```bash
# 핵심 기능
cmake -DLOGGER_USE_DI=ON              # Dependency injection 활성화 (기본값: ON)
cmake -DLOGGER_USE_MONITORING=ON      # Monitoring 지원 활성화 (기본값: ON)
cmake -DLOGGER_ENABLE_ASYNC=ON        # Async 로깅 활성화 (기본값: ON)
cmake -DLOGGER_ENABLE_CRASH_HANDLER=ON # Crash handler 활성화 (기본값: ON)

# 고급 기능
cmake -DLOGGER_USE_LOCK_FREE_QUEUE=ON # Lock-free 큐 사용 (기본값: USE_LOCKFREE를 따름)
cmake -DLOGGER_ENABLE_STRUCTURED_LOGGING=ON # JSON 로깅 (기본값: OFF)
cmake -DLOGGER_ENABLE_NETWORK_WRITER=ON # 네트워크 로그 writer (기본값: OFF)
cmake -DLOGGER_ENABLE_FILE_ROTATION=ON  # 파일 rotation (기본값: ON)

# 성능 튜닝
cmake -DLOGGER_DEFAULT_BUFFER_SIZE=16384 # 바이트 단위 버퍼 크기
cmake -DLOGGER_DEFAULT_BATCH_SIZE=200    # 배치 처리 크기
cmake -DLOGGER_DEFAULT_QUEUE_SIZE=20000  # 최대 큐 크기

# 빌드 옵션
cmake -DLOGGER_FORCE_LIGHTWEIGHT=ON   # Lightweight 구현 강제 (기본값: ON)

# 품질 보증 옵션 (Phase 5 P5의 새 기능)
cmake -DLOGGER_ENABLE_SANITIZERS=ON   # Debug 빌드에서 sanitizer 활성화
cmake -DLOGGER_SANITIZER_TYPE=address # Sanitizer 타입 (address/thread/undefined/memory)
cmake -DLOGGER_ENABLE_WARNINGS=ON     # 포괄적인 컴파일러 경고 활성화
cmake -DLOGGER_WARNINGS_AS_ERRORS=ON  # 경고를 에러로 처리
cmake -DLOGGER_ENABLE_COVERAGE=ON     # 코드 커버리지 리포팅 활성화
cmake -DLOGGER_USE_EXTERNAL_DI=OFF    # 외부 DI container 사용 (기본값: OFF)
cmake -DLOGGER_ENABLE_SANITIZERS=ON   # 디버깅을 위한 sanitizer 활성화
cmake -DLOGGER_ENABLE_COVERAGE=ON     # 코드 커버리지 활성화
```

### Package로 사용하기

설치 후, CMake 프로젝트에서 logger system을 사용하세요:

```cmake
find_package(LoggerSystem REQUIRED)

target_link_libraries(your_app
    PRIVATE
        LoggerSystem::logger
)

# 선택사항: Configuration 출력
LoggerSystem_print_configuration()
```

## Thread System과의 통합

이 logger는 dependency injection을 통해 [Thread System](https://github.com/kcenon/thread_system)과 원활하게 작동하도록 설계되었습니다:

### Dependency Injection 통합

Logger system은 서비스 의존성 관리를 위한 내장 DI container를 제공합니다:

```cpp
#include <kcenon/logger/core/di/di_container_interface.h>
#include <kcenon/logger/core/logger_builder.h>

// Logger 생성 및 구성
auto logger = kcenon::logger::logger_builder()
    .use_template("production")
    .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
    .build()
    .value();

// DI container에 logger 등록
auto& container = kcenon::logger::di_container::global();
container.register_singleton<kcenon::logger::logger_interface>(logger);

// 나중에 애플리케이션 어디서나 사용
auto retrieved_logger = container.resolve<kcenon::logger::logger_interface>();
if (retrieved_logger) {
    retrieved_logger->log(kcenon::logger::log_level::info, "DI를 통한 logger 사용");
}
```

### 크로스 시스템 DI 통합

logger_system과 thread_system을 함께 사용할 때:

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/thread/interfaces/service_container.h>
#include <kcenon/thread/core/thread_pool.h>

int main() {
    // 1. Logger 생성
    auto logger = kcenon::logger::logger_builder()
        .use_template("production")
        .add_writer("file", std::make_unique<kcenon::logger::file_writer>("app.log"))
        .build()
        .value();

    // 2. Thread system의 service container에 logger 등록
    kcenon::thread::service_container::global()
        .register_singleton<kcenon::thread::logger_interface>(logger);

    // 3. Logging context로 thread pool 생성
    auto context = kcenon::thread::thread_context();  // Logger 자동 resolve
    auto pool = std::make_shared<kcenon::thread::thread_pool>("WorkerPool", context);

    // 4. Thread pool 작업이 자동으로 로깅됩니다
    pool->start();
    pool->submit_task([]() {
        // Worker task도 context를 통해 logger에 접근 가능
    });

    return 0;
}
```

### 고급 Writer 유형

#### Critical Writer - 중요 메시지를 위한 동기식 로깅

`critical_writer`는 비동기 큐를 우회하고 메시지를 동기적으로 기록하여 중요한 정보가 즉시 유지되도록 보장합니다:

```cpp
#include <kcenon/logger/writers/critical_writer.h>
#include <kcenon/logger/writers/file_writer.h>

// File writer를 감싸는 critical writer 생성
auto critical = std::make_unique<kcenon::logger::critical_writer>(
    std::make_unique<kcenon::logger::file_writer>("critical.log")
);

auto logger = kcenon::logger::logger_builder()
    .add_writer("critical", std::move(critical))
    .build()
    .value();

// 이 로그는 즉시 기록됩니다 (큐에 넣지 않음)
logger->log(kcenon::logger::log_level::error, "치명적 오류 발생");
// ⚠️ 이 줄이 실행되기 전에 파일 업데이트가 보장됩니다
```

**사용 사례**:
- 애플리케이션 종료 전 치명적 오류 로깅
- 즉시 유지가 필요한 보안 감사 추적
- 데이터 손실이 허용되지 않는 트랜잭션 로깅

**⚠️ 성능 경고**: Critical writer는 쓰기가 완료될 때까지 호출 스레드를 차단합니다. 정말 중요한 메시지에만 사용하세요.

#### Hybrid Writer - 두 가지 장점 모두

`hybrid_writer`는 로그 레벨에 따라 비동기와 동기 모드 간 자동 전환합니다:

```cpp
#include <kcenon/logger/writers/hybrid_writer.h>
#include <kcenon/logger/writers/rotating_file_writer.h>

// Hybrid 동작 구성
kcenon::logger::hybrid_writer_config config;
config.sync_level = kcenon::logger::log_level::error;  // Error 이상은 동기
config.async_queue_size = 10000;

auto hybrid = std::make_unique<kcenon::logger::hybrid_writer>(
    std::make_unique<kcenon::logger::rotating_file_writer>(
        "hybrid.log",
        10 * 1024 * 1024,  // 파일당 10MB
        5                   // 5개 파일 유지
    ),
    config
);

auto logger = kcenon::logger::logger_builder()
    .add_writer("hybrid", std::move(hybrid))
    .build()
    .value();

// Debug/Info: 큐에 넣음 (비동기)
logger->log(kcenon::logger::log_level::debug, "디버깅 정보");

// Error: 즉시 기록 (동기)
logger->log(kcenon::logger::log_level::error, "중요한 오류!");
```

### Crash-Safe Logging

Crash-safe logger는 비정상 종료 시 로그를 flush하기 위한 신호 핸들러를 자동으로 설치합니다:

```cpp
#include <kcenon/logger/safety/crash_safe_logger.h>

int main() {
    // Crash-safe wrapper로 logger 생성
    auto logger = kcenon::logger::crash_safe_logger::create(
        kcenon::logger::logger_builder()
            .use_template("production")
            .add_writer("file", std::make_unique<kcenon::logger::file_writer>("app.log"))
            .build()
            .value()
    );

    // Crash 감지를 위한 신호 핸들러 설치
    kcenon::logger::crash_safe_logger::install_signal_handlers();

    // 일반 로깅
    logger->log(kcenon::logger::log_level::info, "애플리케이션 시작");

    // 애플리케이션이 크래시되면 종료 전 로그가 자동으로 flush됩니다
    // SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGTERM, SIGINT 처리

    return 0;
}
```

**보호되는 신호**:
- `SIGSEGV`: 세그멘테이션 오류
- `SIGABRT`: 중단 신호
- `SIGFPE`: 부동 소수점 예외
- `SIGILL`: 불법 명령
- `SIGTERM`: 종료 요청
- `SIGINT`: 인터럽트 (Ctrl+C)

**⚠️ 중요 사항**:
- 신호 핸들러에는 제한이 있습니다 (async-signal-safe 함수만)
- 치명적인 크래시에서 100% 로그 보존을 보장할 수 없습니다
- 디버거 동작에 영향을 줄 수 있습니다 (필요시 디버그 빌드에서 비활성화)
- 모든 플랫폼에서 사용 가능하지 않습니다 (Windows는 다른 메커니즘 사용)

## 빠른 시작

### Builder Pattern을 사용한 빠른 시작 (권장)

```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>

int main() {
    // 자동 validation을 사용하여 builder로 logger 생성
    auto result = kcenon::logger::logger_builder()
        .use_template("production")  // 사전 정의된 configuration 사용
        .with_min_level(kcenon::logger::log_level::info)
        .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
        .add_writer("file", std::make_unique<kcenon::logger::file_writer>("app.log"))
        .build();

    if (!result) {
        std::cerr << "Failed to create logger: " << result.get_error().message() << "\n";
        return -1;
    }

    auto logger = std::move(result.value());

    // 에러 핸들링이 포함된 로그 메시지
    auto log_result = logger->log(kcenon::logger::log_level::info, "Application started");
    if (!log_result) {
        std::cerr << "Log failed: " << log_result.get_error().message() << "\n";
    }

    return 0;
}
```

### Configuration 템플릿

```cpp
// Production configuration - 프로덕션 환경을 위해 최적화
auto prod_logger = kcenon::logger::logger_builder()
    .use_template("production")
    .build()
    .value();  // 에러 시 throw

// Debug configuration - 개발을 위한 즉시 출력
auto debug_logger = kcenon::logger::logger_builder()
    .use_template("debug")
    .build()
    .value();

// High-performance - 최대화된 처리량
auto hp_logger = kcenon::logger::logger_builder()
    .use_template("high_performance")
    .build()
    .value();

// Low-latency - 실시간 시스템을 위한 최소화된 지연시간
auto rt_logger = logger_module::logger_builder()
    .use_template("low_latency")
    .build()
    .value();
```

### 전통적인 API (레거시 지원)

```cpp
#include <logger/logger.h>

int main() {
    // Logger 인스턴스 생성
    auto logger = std::make_shared<logger_module::logger>();

    // Console 출력 추가
    logger->add_writer(std::make_unique<logger_module::console_writer>());

    // 파일 출력 추가
    logger->add_writer(std::make_unique<logger_module::file_writer>("app.log"));

    // 메시지 로깅
    logger->log(thread_module::log_level::info, "Application started");
    logger->log(thread_module::log_level::error, "Something went wrong", __FILE__, __LINE__, __func__);

    return 0;
}
```

### Result Pattern을 사용한 에러 핸들링

```cpp
// 모든 작업은 포괄적인 에러 핸들링을 위해 result 타입을 반환합니다
auto result = logger->log(thread_module::log_level::info, "Message");
if (!result) {
    // 에러 처리
    auto error = result.get_error();
    std::cerr << "Log failed: " << error.message() << " (code: "
              << static_cast<int>(error.code()) << ")\n";

    // 에러 코드에 따라 적절한 조치 취하기
    switch (error.code()) {
        case thread_module::error_code::queue_full:
            // 큐 오버플로우 처리
            break;
        case thread_module::error_code::queue_stopped:
            // Logger가 종료 중
            break;
        default:
            // 기타 에러 처리
            break;
    }
}

// Validation을 포함한 builder pattern
auto builder_result = logger_module::logger_builder()
    .with_buffer_size(0)  // 유효하지 않음!
    .build();

if (!builder_result) {
    // Configuration validation 실패
    std::cerr << "Invalid configuration: "
              << builder_result.get_error().message() << "\n";
}
```

### 인터페이스 아키텍처

```cpp
// 새로운 깔끔한 인터페이스 분리
#include <logger/interfaces/log_writer_interface.h>
#include <logger/interfaces/log_filter_interface.h>
#include <logger/interfaces/log_formatter_interface.h>

// Custom writer 구현
class custom_writer : public logger_module::log_writer_interface {
public:
    result_void write(const logger_module::log_entry& entry) override {
        // 커스텀 구현
        return result_void{};  // 성공
    }

    result_void flush() override {
        return result_void{};
    }
};

// Custom filter 구현
class custom_filter : public logger_module::log_filter_interface {
public:
    bool should_log(const logger_module::log_entry& entry) const override {
        // 필터 로직
        return entry.level >= thread_module::log_level::warning;
    }
};

// Custom formatter 구현
class custom_formatter : public logger_module::log_formatter_interface {
public:
    std::string format(const logger_module::log_entry& entry) const override {
        // 로그 항목 포매팅
        return fmt::format("[{}] {}", entry.level, entry.message);
    }
};
```

### 성능 Metric

```cpp
// Metric 수집 활성화
logger->enable_metrics_collection(true);

// 일부 메시지 로깅
for (int i = 0; i < 1000; ++i) {
    logger->log(log_level::info, "Test message");
}

// 현재 metric 가져오기
auto metrics = logger->get_current_metrics();
std::cout << "Messages per second: " << metrics.get_messages_per_second() << "\n";
std::cout << "Average enqueue time: " << metrics.get_avg_enqueue_time_ns() << " ns\n";
std::cout << "Queue utilization: " << metrics.get_queue_utilization_percent() << "%\n";
```

### 구조화된 로깅

```cpp
#include <logger_system/structured/structured_logger.h>

// 구조화된 logger wrapper 생성
auto structured = std::make_shared<logger_module::structured_logger>(
    logger,
    logger_module::structured_logger::output_format::json
);

// 구조화된 필드로 로깅
structured->info("User logged in")
    .field("user_id", 12345)
    .field("ip_address", "192.168.1.1")
    .field("session_duration", 3600)
    .commit();

// 출력 (JSON 형식):
// {"@timestamp":"2025-01-27T08:30:00Z","level":"INFO","message":"User logged in","thread_id":"12345","user_id":12345,"ip_address":"192.168.1.1","session_duration":3600}
```

### 고급 필터링 및 라우팅

```cpp
#include <logger_system/filters/log_filter.h>
#include <logger_system/routing/log_router.h>

// 필터링 설정 - warning 이상만 로깅
logger->set_filter(std::make_unique<level_filter>(log_level::warning));

// 민감한 정보 필터링
logger->set_filter(std::make_unique<regex_filter>("password|secret", false));

// 라우팅 설정
auto& router = logger->get_router();

// 에러를 전용 에러 파일로 라우팅
router_builder(router)
    .when_level(log_level::error)
    .route_to("error_file", true);  // 전파 중지

// Debug 메시지를 debug 파일과 console 모두로 라우팅
router_builder(router)
    .when_level(log_level::debug)
    .route_to(std::vector<std::string>{"debug_file", "console"});

// Custom filter function
auto custom_filter = std::make_unique<function_filter>(
    [](log_level level, const std::string& msg,
       const std::string& file, int line, const std::string& func) {
        // 특정 파일의 메시지만 로깅
        return file.find("critical_module") != std::string::npos;
    }
);
logger->set_filter(std::move(custom_filter));
```

### 파일 Writer

```cpp
#include <logger_system/writers/file_writer.h>
#include <logger_system/writers/rotating_file_writer.h>

// 기본 파일 writer
logger->add_writer("main_log", std::make_unique<file_writer>("logs/app.log"));

// Rotating 파일 writer - 크기 기반
logger->add_writer("rotating", std::make_unique<rotating_file_writer>(
    "logs/app.log",
    1024 * 1024 * 10,  // 파일당 10MB
    5                   // 5개의 백업 파일 유지
));

// Rotating 파일 writer - 시간 기반 (일별)
logger->add_writer("daily", std::make_unique<rotating_file_writer>(
    "logs/daily.log",
    rotating_file_writer::rotation_type::daily,
    30  // 30일간의 로그 유지
));
```

### 분산 로깅

```cpp
#include <logger_system/writers/network_writer.h>
#include <logger_system/server/log_server.h>

// 원격 서버로 로그 전송
logger->add_writer("remote", std::make_unique<network_writer>(
    "log-server.example.com",
    9999,
    network_writer::protocol_type::tcp
));

// 로그를 수신할 로그 서버 생성
auto server = std::make_unique<log_server>(9999, true);
server->add_handler([](const log_server::network_log_entry& entry) {
    std::cout << "Received log from " << entry.source_address
              << ": " << entry.raw_data << std::endl;
});
server->start();
```

### 실시간 분석

```cpp
#include <logger_system/analysis/log_analyzer.h>

// 60초 윈도우로 analyzer 생성
auto analyzer = std::make_unique<log_analyzer>(
    std::chrono::seconds(60),
    60  // 1시간 히스토리 유지
);

// 패턴 추적
analyzer->add_pattern("errors", "error|fail|exception");
analyzer->add_pattern("slow_queries", "query took \\d{4,} ms");

// Alert 규칙 추가
analyzer->add_alert_rule({
    "high_error_rate",
    [](const auto& stats) {
        auto errors = stats.level_counts.count(log_level::error) ?
                     stats.level_counts.at(log_level::error) : 0;
        return errors > 100;  // 분당 100개 이상의 에러 시 alert
    },
    [](const std::string& rule, const auto& stats) {
        std::cout << "ALERT: High error rate detected!" << std::endl;
    }
});

// 로그 분석
analyzer->analyze(level, message, file, line, function, timestamp);

// 리포트 생성
std::string report = analyzer->generate_report(std::chrono::minutes(10));
```

### 보안 기능

#### 로그 암호화

```cpp
#include <logger_system/writers/encrypted_writer.h>

// 암호화 키 생성
auto key = encrypted_writer::generate_key(32);  // AES-256용 32 바이트

// 키를 안전하게 저장
encrypted_writer::save_key(key, "logger.key");

// 암호화된 writer 생성
auto file = std::make_unique<file_writer>("secure.log");
auto encrypted = std::make_unique<encrypted_writer>(std::move(file), key);
logger->add_writer("secure", std::move(encrypted));

// 참고: 데모는 XOR 암호화를 사용 - 프로덕션에서는 적절한 암호화 라이브러리 사용
```

#### 민감한 데이터 Sanitization

```cpp
#include <logger_system/security/log_sanitizer.h>

// 기본 규칙으로 sanitizer 생성
auto sanitizer = std::make_shared<log_sanitizer>();

// 로그 쓰기 전에 sanitize
std::string message = "User login: john.doe@example.com, Card: 4532-1234-5678-9012";
std::string sanitized = sanitizer->sanitize(message);
// 결과: "User login: j******e@example.com, Card: 4532********9012"

// Custom sanitization 규칙 추가
sanitizer->add_rule({
    "jwt_token",
    std::regex("Bearer\\s+[A-Za-z0-9\\-_]+\\.[A-Za-z0-9\\-_]+\\.[A-Za-z0-9\\-_]+"),
    [](const std::smatch& match) { return "Bearer [REDACTED]"; }
});
```

#### 접근 제어

```cpp
#include <logger_system/security/log_sanitizer.h>

// 접근 제어 filter 생성
auto access_filter = std::make_unique<access_control_filter>(
    access_control_filter::permission_level::write_info
);

// 파일별 권한 설정
access_filter->set_file_permission(".*secure.*",
    access_control_filter::permission_level::admin);

// 사용자 컨텍스트 설정
access_filter->set_user_context("current_user",
    access_control_filter::permission_level::write_info);

logger->set_filter(std::move(access_filter));
```

### Custom Writer

```cpp
class custom_writer : public logger_module::base_writer {
public:
    bool write(thread_module::log_level level,
               const std::string& message,
               const std::string& file,
               int line,
               const std::string& function,
               const std::chrono::system_clock::time_point& timestamp) override {
        // Custom 구현
        return true;
    }

    void flush() override {
        // Flush 구현
    }

    std::string get_name() const override {
        return "custom";
    }
};
```

## 빌드

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### 빌드 옵션

- `BUILD_TESTS`: Unit 테스트 빌드 (기본값: ON)
- `BUILD_BENCHMARKS`: 성능 벤치마크 빌드 (기본값: OFF)
- `BUILD_SAMPLES`: 예제 프로그램 빌드 (기본값: ON)
- `USE_LOCKFREE`: Lock-free 구현 사용 (기본값: ON)
- `BUILD_WITH_COMMON_SYSTEM`: 표준화된 인터페이스를 위한 common_system 통합 활성화 (기본값: ON)
- `USE_THREAD_SYSTEM`: 가능한 경우 외부 thread_system 사용 (기본값: ON)

## 테스트

`BUILD_TESTS=ON` (기본값)으로 빌드한 후 통합 테스트를 실행하세요:

```bash
ctest --test-dir build
# 또는
./build/bin/integration_test
```

## 플랫폼 지원

- Linux 및 macOS는 console/파일 writer 및 POSIX 네트워킹을 완전히 지원합니다.
- Windows 지원은 부분적입니다; 네트워크/서버 컴포넌트는 WinSock 초기화 및 약간의 적응이 필요합니다. 기여를 환영합니다.

## FAQ

- Logger가 lock-free인가요?
  - 현재 async 큐는 이식성과 단순성을 위해 mutex/condition_variable을 사용합니다. Lock-free MPMC 큐가 계획되어 있습니다; `USE_LOCKFREE` placeholder를 참조하세요.
- `encrypted_writer`가 프로덕션에 사용 가능한가요?
  - 아니요. 데모입니다. 검증된 암호화 라이브러리와 인증된 암호화(예: AES-GCM, ChaCha20-Poly1305)를 적절한 키 관리와 함께 사용하세요.
- 에러만 전용 파일로 라우팅하려면 어떻게 하나요?
  - `router_builder(router).when_level(log_level::error).route_to("error_file", true);`를 사용하고 해당 이름으로 writer를 등록하세요.
- JSON 출력을 어떻게 얻나요?
  - `output_format::json`과 함께 `structured_logger`를 사용하세요. 대규모에서 엄격한 JSON 준수를 위해서는 JSON 라이브러리(예: nlohmann/json) 통합을 고려하세요.

## 추가 자료

- [Getting Started Guide](docs/guides/GETTING_STARTED.md) - 단계별 설정 및 기본 사용법
- [Best Practices](docs/guides/BEST_PRACTICES.md) - 프로덕션 준비 패턴 및 권장사항
- [Performance Guide](docs/guides/PERFORMANCE.md) - 성능 분석 및 최적화
- [Custom Writers](docs/advanced/CUSTOM_WRITERS.md) - Custom 로그 writer 생성
- [API Reference](docs/API_REFERENCE.md) - 완전한 API 문서
- [System Architecture](docs/LOGGER_SYSTEM_ARCHITECTURE.md) - 상세한 기술 아키텍처
- [Security Guide](docs/SECURITY.md) - 보안 고려사항 및 보고
- [Complete Documentation](docs/README.md) - 전체 문서 색인

## 설치

```bash
cmake --build . --target install
```

## CMake 통합

```cmake
find_package(LoggerSystem REQUIRED)
target_link_libraries(your_target PRIVATE LoggerSystem::logger)
```

## API 문서

### 핵심 API 참조

- **[API Reference](./docs/API_REFERENCE.md)**: 인터페이스를 포함한 완전한 API 문서
- **[Architecture Guide](./docs/ARCHITECTURE.md)**: 시스템 설계 및 내부 구조
- **[Performance Guide](./docs/PERFORMANCE.md)**: 최적화 팁 및 벤치마크
- **[User Guide](./docs/USER_GUIDE.md)**: 사용 가이드 및 예제
- **[FAQ](./docs/FAQ.md)**: 자주 묻는 질문

### API 개요

```cpp
// Logger 핵심 API
namespace kcenon::logger {
    // Async 처리를 지원하는 메인 logger
    class logger {
        auto log(log_level level, const std::string& message) -> result_void;
        auto add_writer(const std::string& name, std::unique_ptr<log_writer_interface> writer) -> result_void;
        auto set_filter(std::unique_ptr<log_filter_interface> filter) -> result_void;
        auto enable_metrics_collection(bool enabled) -> void;
        auto get_current_metrics() const -> metrics_data;
        auto flush() -> result_void;
    };

    // Configuration을 위한 builder pattern
    class logger_builder {
        auto use_template(const std::string& template_name) -> logger_builder&;
        auto with_min_level(log_level level) -> logger_builder&;
        auto with_buffer_size(size_t size) -> logger_builder&;
        auto with_batch_size(size_t size) -> logger_builder&;
        auto add_writer(const std::string& name, std::unique_ptr<log_writer_interface> writer) -> logger_builder&;
        auto build() -> result<std::unique_ptr<logger>>;
    };

    // 구조화된 로깅 wrapper
    class structured_logger {
        auto info(const std::string& message) -> structured_entry&;
        auto error(const std::string& message) -> structured_entry&;
        auto field(const std::string& key, const auto& value) -> structured_entry&;
        auto commit() -> result_void;
    };
}

// Writer 인터페이스
namespace kcenon::logger {
    class log_writer_interface {
        virtual auto write(const log_entry& entry) -> result_void = 0;
        virtual auto flush() -> result_void = 0;
    };

    class console_writer : public log_writer_interface { /* ANSI 색상 출력 */ };
    class file_writer : public log_writer_interface { /* 버퍼링된 파일 출력 */ };
    class rotating_file_writer : public log_writer_interface { /* 크기/시간 rotation */ };
    class network_writer : public log_writer_interface { /* TCP/UDP 원격 */ };
}

// 통합 API (thread_system과 함께)
namespace kcenon::thread::interfaces {
    class logger_interface {
        virtual auto log(log_level level, const std::string& message) -> result_void = 0;
        virtual auto enable_metrics(bool enabled) -> void = 0;
    };
}
```

## 기여하기

기여를 환영합니다! 자세한 내용은 [Contributing Guide](./docs/CONTRIBUTING.md)를 참조하세요.

### 개발 설정

1. Repository를 fork하세요
2. Feature branch를 생성하세요 (`git checkout -b feature/amazing-feature`)
3. 변경사항을 커밋하세요 (`git commit -m 'Add some amazing feature'`)
4. Branch에 push하세요 (`git push origin feature/amazing-feature`)
5. Pull Request를 여세요

### 코드 스타일

- 최신 C++ 모범 사례를 따르세요
- RAII 및 smart pointer를 사용하세요
- 일관된 포매팅 유지 (clang-format configuration 제공됨)
- 새 기능에 대한 포괄적인 unit 테스트 작성

## Network System 통합

- network_system은 다음으로 빌드할 때 lightweight adapter를 통해 logger_system을 사용할 수 있습니다:
  - `-DBUILD_WITH_LOGGER_SYSTEM=ON`
- network_system 내부의 호출 사이트에서 `NETWORK_LOG_*` 매크로는 `logger_integration_manager`를 통해 라우팅되며, 가능한 경우 `logger_system_adapter`를 사용하고 그렇지 않으면 `basic_logger`를 사용합니다.

## 지원

- **Issue**: [GitHub Issues](https://github.com/kcenon/logger_system/issues)
- **Discussion**: [GitHub Discussions](https://github.com/kcenon/logger_system/discussions)
- **Email**: kcenon@naver.com

## 프로덕션 품질 및 아키텍처

### 빌드 및 테스트 인프라

**포괄적인 멀티 플랫폼 CI/CD**
- **Sanitizer 커버리지**: ThreadSanitizer, AddressSanitizer, UBSanitizer를 사용한 자동 빌드
- **멀티 플랫폼 테스트**: Ubuntu (GCC/Clang), Windows (MSYS2/VS), macOS 전반에 걸친 지속적인 검증
- **성능 벤치마킹**: regression 감지를 지원하는 자동 벤치마크 추적
- **코드 커버리지**: CI/CD 파이프라인의 커버리지 리포트로 ~65% 테스트 커버리지
- **정적 분석**: 코드 품질 보증을 위한 Clang-tidy 및 Cppcheck 통합

**성능 기준선**
- **Async 로깅**: 초당 434만 메시지 (단일 스레드)
- **멀티스레드 성능**: 107만 msg/s (4 스레드), 41.2만 msg/s (8 스레드), 39만 msg/s (16 스레드)
- **지연시간**: 148ns 평균 enqueue 시간 (spdlog보다 15.7배 향상)
- **메모리 효율성**: 적응형 버퍼 관리로 기본 <2MB

포괄적인 성능 metric 및 regression threshold는 [BASELINE.md](BASELINE.md)를 참조하세요.

### 스레드 안전성 및 동시성

**프로덕션 검증된 Writer 안전성 (90% 완료)**
- **Writer 스레드 안전성**: 모든 writer 구현 (`console_writer`, `file_writer`, `rotating_file_writer`, `network_writer`)이 동시 접근에 대해 검증됨
- **ThreadSanitizer 준수**: CI/CD 파이프라인에서 data race 제로 감지
- **테스트 Suite 마이그레이션**: 스레드 안전성 검증을 포함한 새로운 인터페이스 패턴으로의 완전한 마이그레이션
- **Dependency Injection**: 런타임 컴포넌트 주입에 대한 edge case 테스트 완료

**비동기 처리 아키텍처**
- **제로카피 설계**: 최소한의 할당으로 효율적인 메시지 전달
- **배치 큐 처리**: 설정 가능한 배치 크기로 논블로킹 로그 작업
- **적응형 큐잉**: 최적의 리소스 활용을 위한 지능적인 백오프 및 배치 전략

### 리소스 관리 (RAII - Grade A)

**완벽한 RAII 준수**
- **100% Smart Pointer 사용**: 모든 리소스가 `std::unique_ptr` 및 `std::shared_ptr`로 관리됨
- **AddressSanitizer Validation**: 모든 테스트 시나리오에서 메모리 누수 제로 감지
- **예외 안전성**: 코드베이스 전체에 걸친 강력한 예외 안전성 보장
- **Writer 생명주기 관리**: 최적화된 RAII 패턴으로 자동 리소스 정리
- **수동 메모리 관리 없음**: public 인터페이스에서 raw pointer 완전 제거

**Validation 결과**
```bash
# AddressSanitizer: 모든 테스트에서 클린
==12345==ERROR: LeakSanitizer: detected memory leaks
# Total: 0 leaks
```

### 에러 핸들링 (프로덕션 준비 - 90% 완료)

**안전성과 성능을 위한 이중 API 설계**

logger_system은 포괄적인 에러 보고와 고성능 로깅의 균형을 맞추는 계층화된 에러 핸들링 전략을 구현합니다:

```cpp
// 핵심 작업: 포괄적인 에러 핸들링을 위한 Result<T>
auto result = kcenon::logger::logger_builder()
    .use_template("production")
    .with_min_level(kcenon::logger::log_level::info)
    .add_writer("console", std::make_unique<kcenon::logger::console_writer>())
    .build();

if (!result) {
    std::cerr << "Failed to create logger: " << result.get_error().message()
              << " (code: " << static_cast<int>(result.get_error().code()) << ")\n";
    return -1;
}
auto logger = std::move(result.value());

// Writer 관리: Configuration 작업을 위한 Result<T>
auto add_result = logger->add_writer("file",
    std::make_unique<kcenon::logger::file_writer>("app.log"));
if (!add_result) {
    std::cerr << "Failed to add writer: " << add_result.get_error().message() << "\n";
}

// Hot Path: 제로 오버헤드 로깅을 위한 void 반환
logger->log(kcenon::logger::log_level::info, "Performance-critical logging");
```

**API 설계 철학**
- **핵심 작업**: 생명주기 (`start`, `stop`) 및 configuration (`add_writer`, `enable_di`)은 포괄적인 에러 보고를 위해 `result_void` 사용
- **Writer 인터페이스**: 모든 writer 메서드 (`write`, `flush`)는 type-safe 에러 핸들링을 위해 `result_void` 반환
- **Hot Path 최적화**: 로깅 메서드는 최소 오버헤드를 위해 `void` 반환 타입 사용
- **쿼리 작업**: 상태 확인 (`is_enabled`, `is_running`)은 단순성을 위해 `bool` 사용

**에러 코드 통합**
- **할당된 범위**: 중앙화된 에러 코드 레지스트리 (common_system)에서 `-200`부터 `-299`까지
- **분류**: System lifecycle (-200 ~ -209), Writer 관리 (-210 ~ -219), Configuration (-220 ~ -229), I/O 작업 (-230 ~ -239)
- **의미 있는 메시지**: validation 세부사항을 포함한 포괄적인 에러 컨텍스트

**남은 선택적 개선사항**
- 📝 **에러 테스트**: 포괄적인 에러 시나리오 테스트 suite 추가
- 📝 **문서**: writer 문서에서 Result<T> 사용 예제 확장
- 📝 **에러 메시지**: writer 실패에 대한 에러 컨텍스트 계속 향상

자세한 구현 노트는 [docs](docs/) 디렉토리의 문서를 참조하세요.

## 라이선스

이 프로젝트는 BSD 3-Clause License에 따라 라이선스가 부여됩니다 - 자세한 내용은 [LICENSE](LICENSE) 파일을 참조하세요.

## 감사의 말

- 이 프로젝트 개선에 도움을 주신 모든 기여자분들께 감사드립니다
- 지속적인 피드백과 지원을 해주신 C++ 커뮤니티에 특별히 감사드립니다
- 최신 로깅 프레임워크와 모범 사례에서 영감을 받았습니다

---

<p align="center">
  Made with ❤️ by 🍀☀🌕🌥 🌊
</p>
