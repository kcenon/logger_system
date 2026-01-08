# 변경 이력 - Logger System

> **언어:** [English](CHANGELOG.md) | **한국어**

Logger System 프로젝트의 모든 주요 변경 사항이 이 파일에 문서화됩니다.

형식은 [Keep a Changelog](https://keepachangelog.com/ko/1.0.0/)를 따르며,
이 프로젝트는 [Semantic Versioning](https://semver.org/lang/ko/spec/v2.0.0.html)을 준수합니다.

---

## [3.0.0] - 2025-12-31

### 제거됨 - Deprecated API 정리 (Issue #268)

이 릴리스는 "Fewest Elements" Simple Design 원칙에 따라 deprecated API 호환성 레이어를 제거합니다.

#### Breaking Changes

- **`logger_interface.h` 헤더 제거** (`include/kcenon/logger/interfaces/logger_interface.h`)
  - `logger_system::logger_interface` 클래스 제거 - `common::interfaces::ILogger` 사용
  - `logger_system::logger_registry` 클래스 제거
  - `THREAD_LOG_*` 매크로 제거 - common_system의 `LOG_*` 매크로 사용

- **`with_thread_system_backend()` 메서드 제거** (`logger_builder`)
  - `with_standalone_backend()` 또는 `with_backend()` 사용

- **deprecated `result_void` 클래스 제거** (`error_codes.h`)
  - `common::VoidResult` 직접 사용

- **deprecated `make_logger_error` 함수 제거**
  - `result<T>{error_code, message}` 생성자 직접 사용

#### 마이그레이션 가이드

```cpp
// 이전 (deprecated):
#include <kcenon/logger/interfaces/logger_interface.h>
auto logger = logger_builder().with_thread_system_backend().build();
result_void res = make_logger_error(code, "message");

// 이후:
#include <kcenon/common/interfaces/logger_interface.h>
auto logger = logger_builder().with_standalone_backend().build();
common::VoidResult res = make_logger_void_result(code, "message");
```

---

## [Unreleased]

### OpenTelemetry 통합 수정 (Issue #283) - 2026-01-08

#### 수정됨
- **macOS 호환성**: `otlp_writer`에서 `std::jthread`를 `std::thread`로 교체
  - Apple Clang의 libc++에서 `std::jthread`와 `std::stop_token`이 지원되지 않음
  - 기존 `running_` atomic 플래그를 종료 메커니즘으로 사용
  - 동일한 동작 및 스레드 안전성 보장 유지

- **Windows 빌드 수정**: `signal_manager.h`에서 `write`/`fsync` 매크로 오염 제거
  - `#define write _write` 매크로가 다른 헤더로 누출되는 문제
  - `batch_writer.h`와 `otlp_writer.h`의 `write()` 메서드가 `_write()`로 이름 변경되는 문제 발생
  - 매크로를 `detail::safe_write()` 및 `detail::safe_fsync()` 인라인 함수로 대체하여 수정
  - 래퍼 함수가 내부적으로 Windows POSIX 호환성 처리

---

### C++20 모듈 파일 (Issue #275) - 2026-01-03

#### 추가됨
- **C++20 모듈 파일**: `kcenon.logger` 모듈 인터페이스 생성
  - `src/modules/logger.cppm`: 기본 모듈 인터페이스
  - `src/modules/core.cppm`: Core 파티션 (Logger, LogContext, LogCollector)
  - `src/modules/backends.cppm`: Backends 파티션 (integration_backend, standalone_backend)
  - `src/modules/analysis.cppm`: Analysis 파티션 (log_analyzer, analysis_stats)

- **CMake 모듈 지원**: 선택적 C++20 모듈 지원 추가
  - 모듈 스캔에 CMake 3.28 이상 필요
  - 모듈 빌드 활성화를 위한 `LOGGER_USE_MODULES` 옵션 추가
  - 모듈 활성 시 `LOGGER_HAS_MODULES` 컴파일 정의
  - 전환 기간 동안 헤더 기반 빌드 계속 동작

#### 모듈 구조
```cpp
export module kcenon.logger;

import kcenon.common;           // 필수 의존성

export import :core;            // Logger, LogContext, LogCollector
export import :backends;        // Integration backends
export import :analysis;        // Log analysis 유틸리티
```

#### CMake 사용법
```cmake
cmake -DLOGGER_USE_MODULES=ON ..  # CMake 3.28 이상 필요
```

---

### 중복 Writer 추가 로직 수정 (Issue #270) - 2025-12-31

#### 수정됨
- **Named writer 저장소**: `add_writer(name, writer)`가 이제 이름으로 writer를 올바르게 저장함
  - 이전에는 `name` 파라미터가 무시됨
  - 이제 `get_writer(name)`로 조회하고 `remove_writer(name)`로 제거 가능

#### 변경됨
- **통일된 반환 타입**: `add_writer(name, writer)`가 이제 `void` 대신 `common::VoidResult` 반환
  - 이름 없는 `add_writer(writer)` 오버로드와 일관성 유지
  - null writer와 초기화되지 않은 logger에 대한 적절한 오류 처리

#### 추가됨
- **Named writer 구현**: Named writer 저장소의 완전한 구현
  - `add_writer(name, writer)`가 일반 목록과 이름 맵 모두에 writer 저장
  - `remove_writer(name)`가 두 저장소 모두에서 writer 제거
  - `get_writer(name)`가 이름으로 writer 포인터 조회
  - `clear_writers()`가 이제 named writer 저장소도 정리

---

### KCENON 기능 감지 사용 (Issue #250) - 2025-12-22

#### 변경됨
- **error_handling_utils.h**: common_system의 통합된 KCENON_HAS_SOURCE_LOCATION 사용
  - `<kcenon/common/config/feature_flags.h>` 포함하여 기능 감지
  - 커스텀 source_location 감지 로직을 KCENON_HAS_SOURCE_LOCATION으로 대체
  - 하위 호환성을 위해 LOGGER_HAS_SOURCE_LOCATION을 레거시 별칭으로 유지

- **jthread_compat.h**: common_system의 통합된 KCENON_HAS_JTHREAD 사용
  - `<kcenon/common/config/feature_flags.h>` 포함하여 기능 감지
  - 커스텀 jthread 감지 로직을 KCENON_HAS_JTHREAD로 대체
  - 하위 호환성을 위해 LOGGER_HAS_JTHREAD를 레거시 별칭으로 유지

#### 수정됨
- **logger.h**: common_system v3.0.0 호환성 빌드 오류 수정
  - deprecated `log(level, message, file, line, function)` 메서드에서 `override` 키워드 제거
  - 이 메서드는 common_system v3.0.0의 `common::interfaces::ILogger`에서 제거됨 (Issue #217)
  - 하위 호환성을 위해 메서드는 유지하되 더 이상 인터페이스를 override하지 않음

- **Windows MSVC LNK2019**: `thread_pool::is_running()` unresolved external symbol 오류 수정
  - 원인: thread_system이 서브모듈로 빌드될 때 `KCENON_HAS_COMMON_EXECUTOR`가 정의되지 않음
  - 해결: thread_system `core/CMakeLists.txt`에서 executor_interface.h 발견 시 KCENON_HAS_COMMON_EXECUTOR=1 정의 추가
  - CI workflow에 `UNIFIED_USE_LOCAL=ON` 추가하여 로컬 checkout된 의존성 사용

- **ilogger_interface_test.cpp**: common_system v3.0.0 API 변경에 맞춰 테스트 업데이트
  - deprecated 메서드를 ILogger 포인터 대신 logger 클래스에서 직접 호출하도록 변경

#### 마이그레이션 가이드
LOGGER_HAS_* 매크로는 이제 common_system의 KCENON_HAS_*에 대한 별칭입니다.
새 코드에서는 KCENON_HAS_*를 직접 사용:
```cpp
// 레거시 (계속 동작함)
#if LOGGER_HAS_JTHREAD
    std::jthread worker(...);
#endif

// 권장
#include <kcenon/common/config/feature_flags.h>
#if KCENON_HAS_JTHREAD
    std::jthread worker(...);
#endif
```

---

### Deprecated common_system API 마이그레이션 (Issue #248) - 2025-12-22

#### 변경됨
- **common_system_adapter.h**: source_location 기반 로깅 API 추가
  - 새로운 `log(log_level, std::string_view, const source_location&)` 메서드
  - Deprecated `log(log_level, string, file, line, function)` 메서드는 새 API로 위임
  - 하위 호환성 코드에서 deprecation 경고 억제

- **logger_interface.h**: THREAD_LOG_* 매크로 처리 방식 업데이트
  - common_system 사용 가능 시 THREAD_LOG_* 매크로가 LOG_*로 리다이렉트됨
  - 컴파일러 경고를 발생시키던 중복 매크로 재정의 제거
  - common_system 없는 빌드를 위한 standalone 폴백 유지

#### 마이그레이션 가이드
새 코드에서는 common_system의 LOG_* 매크로를 직접 사용:
```cpp
// Deprecated (v3.0.0에서 제거 예정)
THREAD_LOG_INFO("message");

// 권장
#include <kcenon/common/logging/log_macros.h>
LOG_INFO("message");
```

ILogger 인터페이스 구현 시 새로운 source_location 기반 메서드 오버라이드:
```cpp
// Deprecated (v3.0.0에서 제거 예정)
VoidResult log(log_level level, const std::string& msg,
               const std::string& file, int line, const std::string& func) override;

// 권장
VoidResult log(log_level level, std::string_view msg,
               const source_location& loc = source_location::current()) override;
```

---

### thread_system v3.0 호환성 (Issue #244) - 2025-12-19

#### 주요 변경 (Breaking Changes)
- **logger_adapter.h**: `shared::ILogger` 및 `shared::IService` 구현 제거
  - `USE_THREAD_SYSTEM` 조건부 컴파일 분기가 제거됨
  - thread_system 통합을 위해서는 `kcenon::common::interfaces::ILogger`를 제공하는
    `common_logger_adapter.h`를 사용

#### 변경됨
- **thread_integration_detector.h**: 탐지 헤더 업데이트
  - 제거된 `<kcenon/thread/interfaces/logger_interface.h>` 대신 `<kcenon/thread/thread_pool.h>` 사용
  - thread_system v3.0은 thread 전용 인터페이스 대신 common_system 인터페이스를 사용

#### 마이그레이션 가이드
thread_system 통합 시 다음과 같이 변경:
```cpp
// 이전 (v2.x)
#include <kcenon/logger/adapters/logger_adapter.h>
// shared::ILogger 및 shared::IService 사용

// 현재 (v3.0+)
#include <kcenon/logger/adapters/common_logger_adapter.h>
// kcenon::common::interfaces::ILogger 사용
```

---

### #226 API 변경 후 손상된 예제 파일 수정 (Issue #228) - 2025-12-06

#### 제거됨
- **migration_example.cpp**: 존재하지 않는 v1 API를 시연하는 예제 파일 제거
  - 구현되지 않은 `logger_module::v1_compat` 네임스페이스 참조
  - 정의되지 않은 함수 사용: `create_logger()`, `create_file_logger()`, `add_file_writer()`
  - 정의되지 않은 매크로 사용: `LOG_INFO`, `LOG_DEBUG`, `LOG_ERROR`, `LOG_WARNING`
  - 정의되지 않은 타입 사용: `logger_config_v1`

#### 변경됨
- **advanced_features_demo.cpp**: 현재 API에 맞게 전체 재작성
  - `thread_module` 네임스페이스 제거 (독립 모드에서 사용 불가)
  - 필터 클래스에 `filters::` 네임스페이스 추가
  - 라우팅 코드 제거 (get_router() 미구현)
  - 디렉토리 생성을 위한 `<filesystem>` 추가
  - 명확성을 위한 출력 메시지 개선

#### 비활성화됨 (기능 구현 대기)
- **distributed_logging_demo.cpp**: CMakeLists.txt에서 주석 처리
  - 미구현 기능 필요: `log_server`, `log_aggregator`, `log_analyzer::time_window_stats`
- **security_demo.cpp**: CMakeLists.txt에서 주석 처리
  - 미구현 기능 필요: `log_sanitizer`, `structured_logger`, `get_router()`

#### 관련 이슈
- #228 수정: fix(examples): #226 API 변경 후 손상된 예제 파일

---

### common::interfaces::ILogger 구현 - 2025-12-06

#### 추가됨
- **ILogger 인터페이스 구현 (Issue #223)**: Logger가 이제 `common::interfaces::ILogger`를 구현
  - common_system의 표준화된 로깅 인터페이스와 완전한 호환성
  - thread_system 및 기타 kcenon 생태계 컴포넌트와 원활한 통합
  - `logger_system::log_level`과 `common::interfaces::log_level` 간 타입 안전 로그 레벨 변환

- **새로운 level_converter.h**: 로그 레벨 타입 간 변환을 위한 유틸리티 헤더
  - `to_logger_system_level()`: common에서 네이티브 로그 레벨로 변환
  - `to_common_level()`: 네이티브에서 common 로그 레벨로 변환
  - 제로 오버헤드 변환을 위한 constexpr 함수

- **ILogger 인터페이스 메서드**:
  - `log(log_level, const std::string&)` → `VoidResult` 반환
  - `log(log_level, std::string_view, const source_location&)` → `VoidResult` 반환
  - `log(log_level, message, file, line, function)` → `VoidResult` 반환
  - `log(const log_entry&)` → 구조화된 로깅 지원
  - `is_enabled(log_level)` → 레벨 활성화 여부 확인
  - `set_level(log_level)` → 최소 로그 레벨 설정, `VoidResult` 반환
  - `get_level()` → `common::interfaces::log_level`로 현재 최소 레벨 반환
  - `flush()` → 버퍼 플러시, `VoidResult` 반환

- **ILogger 인터페이스 호환성 테스트**: `tests/unit/ilogger_interface_test.cpp`에 새 테스트 스위트
  - 인터페이스 호환성을 위한 13개의 포괄적인 테스트
  - 다형적 사용, 레벨 변환 및 하위 호환성 테스트

#### 변경됨
- **logger 클래스가 이제 두 인터페이스 모두 상속**:
  - `security::critical_logger_interface` (기존)
  - `common::interfaces::ILogger` (새로운 표준화된 인터페이스)

- **ILogger 준수를 위한 반환 타입 업데이트**:
  - `log()` 메서드가 이제 `void` 대신 `common::VoidResult` 반환
  - `flush()`가 이제 `void` 대신 `common::VoidResult` 반환
  - `set_level()`이 이제 `void` 대신 `common::VoidResult` 반환

#### 사용 중단됨
- **logger_types.h**: `kcenon/common/interfaces/logger_interface.h`를 사용하도록 권장
  - `logger_system::log_level` 사용 중단 표시, `common::interfaces::log_level` 사용
  - v3.0.0에서 제거 예정

- **logger_interface.h** (logger_system 네임스페이스): common_system의 ILogger를 사용하도록 권장
  - `logger_system::logger_interface` 사용 중단 표시, `common::interfaces::ILogger` 사용
  - v3.0.0에서 제거 예정

#### 하위 호환성
- **모든 기존 API 유지**: 네이티브 `logger_system::log_level` 오버로드가 여전히 작동
  - `log(log_level, message)` - 시그니처 및 동작 변경 없음
  - `log(log_level, message, file, line, function)` - 변경 없음
  - `is_enabled(log_level)` - 변경 없음
  - `set_min_level(log_level)` - 변경 없음
  - `get_min_level()` - 네이티브 log_level 반환

#### 마이그레이션 가이드
기존 코드는 변경 없이 계속 작동합니다. 새로운 ILogger 인터페이스를 사용하려면:

```cpp
// ILogger 인터페이스를 통해 logger 사용
std::shared_ptr<common::interfaces::ILogger> ilogger = my_logger;
ilogger->log(common::interfaces::log_level::info, "Message");

// 또는 두 스타일 혼합 사용
logger->log(log_level::info, "Native style");  // 여전히 작동
logger->log(ci::log_level::info, std::string("ILogger style"));  // 새로운 방식
```

#### 관련 이슈
- #223 종료 (common::interfaces::ILogger 구현)
- #224의 선행 작업 (thread_system 통합)
- #225의 선행 작업 (thread_system_backend 제거)

---

### 독립형 비동기 구현 (Issue #222) - 2025-12-06

#### 변경됨
- **thread_system이 이제 선택 사항**: Logger 시스템은 독립적으로 빌드 및 실행 가능
  - CMake 옵션 `LOGGER_USE_THREAD_SYSTEM` (기본값: OFF)으로 통합 제어
  - 비활성화 시 모든 비동기 작업에 C++20 std::jthread 사용
  - 활성화 시 전체 thread_system 통합 지원

- **C++20 std::jthread 마이그레이션**: 모든 내부 스레딩이 std::jthread 사용
  - `log_collector.cpp`: stop_token을 사용하는 `log_collector_jthread_worker`
  - `network_writer.cpp`: `network_send_jthread_worker` 및 `network_reconnect_jthread_worker`
  - `batch_processor.cpp`: `batch_processing_jthread_worker`
  - `async_worker.h/cpp`: 새로운 독립형 비동기 워커 구현

#### 추가됨
- **async_worker 클래스**: std::jthread를 사용하는 새로운 독립형 비동기 워커
  - `src/impl/async/async_worker.h` 및 `async_worker.cpp`에 위치
  - std::stop_token을 통한 협력적 취소 기능
  - 오버플로우 감지가 있는 락프리 enqueue 경로
  - 설정 가능한 큐 크기 및 드롭된 작업 카운팅

#### 이점
- **순환 의존성 해결**: thread_system에서 이제 logger_system을 로깅에 사용 가능
- **독립적 운영**: 외부 스레딩 의존성 없이 logger_system 작동
- **더 나은 종료**: std::jthread가 더 깔끔한 RAII 기반 스레드 관리 제공
- **협력적 취소**: 종료 경로에서 폴링/바쁜 대기 없음

#### 마이그레이션 가이드
- **기본 동작**: logger_system이 이제 독립적으로 빌드됨 (thread_system 없음)
- **thread_system 통합을 활성화하려면**:
  ```cmake
  cmake -DLOGGER_USE_THREAD_SYSTEM=ON ...
  ```
- 통합 활성화 시 기존 기능 유지

#### 관련 이슈
- #222 종료: std::jthread를 사용하는 async_worker 클래스 구현
- #223, #224, #225의 선행 작업

---

### CMake fmt Fallback 제거 - 2025-12-03

#### 변경됨
- **CMake 설정에서 std::format 필수**: 모든 fmt 라이브러리 fallback 로직 제거
  - `cmake/CompilerChecks.cmake`: std::format을 사용할 수 없으면 FATAL_ERROR 발생
  - `cmake/LoggerCompatibility.cmake`: fmt 탐지, C++17 모드, vcpkg fallback 제거

#### 제거됨
- **CMake의 fmt 라이브러리 fallback**:
  - `check_std_format()`: find_package(fmt) 및 vcpkg fallback 로직 제거
  - `configure_logger_compatibility()`: C++17 호환 모드 완전 제거
  - `setup_formatting_library()`: fmt::format 및 basic formatting 분기 제거
  - `setup_vcpkg_fmt_fallback()`: 함수 완전 제거

#### 이점
- **깔끔한 CMake 설정**: fallback 복잡성 없이 단순화된 기능 감지
- **일관된 빌드 동작**: 모든 플랫폼과 CI 환경에서 동일한 동작
- **의존성 감소**: 조건부 fmt 라이브러리 링킹 없음
- **프로젝트 간 정렬**: thread_system, container_system, network_system, database_system과 일치

#### 관련 이슈
- 프로젝트 간 표준화 작업의 일부
- 참고: thread_system#219, container_system#168, network_system#257, database_system#203

---

## [1.0.0] - 2025-10-22

### 추가됨
- **핵심 Logger System**: 고성능 C++20 비동기 로깅 프레임워크
  - 논블로킹 로그 작업을 통한 비동기 처리
  - 다중 출력 대상 (콘솔, 파일, 로테이팅 파일, 네트워크, 암호화, 하이브리드)
  - 동시 로깅을 지원하는 스레드 안전 작업
  - 효율적인 메시지 전달을 위한 제로카피 설계

- **Critical Writer**: 즉각적인 기록이 필요한 중요 메시지를 위한 동기식 로깅
  - 비정상 종료 시 로그 보존을 위한 긴급 플러시 메커니즘
  - 자동 버퍼 플러싱을 통한 크래시 안전 로깅

- **고급 아키텍처**:
  - ILogger, IMonitor, IMonitorable 추상화를 통한 인터페이스 기반 설계
  - 플러그 가능한 writer, filter, formatter를 갖춘 모듈화된 컴포넌트
  - 인터페이스 전용 의존성을 통한 순환 의존성 제로
  - 생태계 의존성 없이 독립적인 컴파일

- **Builder Pattern API**: 유창하고 타입 안전한 logger 구축
  - 구성 전략 (템플릿 기반, 환경 인식, 성능 튜닝)
  - Result<T> 오류 처리를 통한 포괄적인 유효성 검사
  - 선택적 런타임 컴포넌트 주입을 위한 의존성 주입

- **CMake 빌드 시스템**: 포괄적인 빌드 구성
  - 유연한 빌드를 위한 15개 이상의 기능 플래그
  - 자동 의존성 감지
  - 크로스 플랫폼 지원 (Windows, Linux, macOS)
  - 컴파일러 지원 (GCC, Clang, MSVC)

- **Writer 구현**:
  - `console_writer`: 표준 출력/오류 스트림 로깅
  - `file_writer`: 기본 파일 출력
  - `rotating_file_writer`: 크기/시간별 자동 로그 로테이션
  - `network_writer`: 네트워크를 통한 원격 로깅
  - `critical_writer`: 동기식 중요 메시지 로깅
  - `hybrid_writer`: 다중 writer 결합 지원

- **Formatter 구현**:
  - `text_formatter`: 일반 텍스트 형식
  - `json_formatter`: 구조화된 JSON 로깅
  - `xml_formatter`: 레거시 시스템을 위한 XML 형식
  - IFormatter 인터페이스를 통한 커스텀 formatter 지원

- **Filter 구현**:
  - 로그 레벨 필터링
  - 패턴 기반 메시지 필터링
  - IFilter 인터페이스를 통한 커스텀 필터 지원

- **통합 지원**:
  - common_system 통합 (ILogger, IMonitor, Result<T> 패턴)
  - thread_system 통합 (스레딩 프리미티브, logger_interface)
  - monitoring_system 통합 (IMonitor를 통한 메트릭 및 헬스 모니터링)

- **성능 기능**:
  - 조정 가능한 배치 크기 및 큐 깊이를 통한 구성 가능한 배칭
  - 핫 패스에서의 락프리 작업
  - 효율적인 메모리 관리를 통한 최소 할당
  - 독립 컴파일 시 약 15초의 빌드 시간

### 변경됨
- 해당 없음 (최초 릴리스)

### 사용 중단됨
- 해당 없음 (최초 릴리스)

### 제거됨
- 해당 없음 (최초 릴리스)

### 수정됨
- 해당 없음 (최초 릴리스)

### 보안
- 스레드 안전 작업으로 데이터 경쟁 없는 동시 로깅 보장
- 민감한 로그 데이터를 위한 암호화 writer 지원

---

## 프로젝트 정보

**저장소:** https://github.com/kcenon/logger_system
**문서:** [docs/](docs/)
**라이선스:** LICENSE 파일 참조
**관리자:** kcenon@naver.com

---

## 버전 지원 매트릭스

| 버전  | 릴리스 날짜 | C++ 표준 | CMake 최소 버전 | 상태   |
|-------|-------------|----------|----------------|--------|
| 1.0.0 | 2025-10-22  | C++20    | 3.16           | 현재   |

---

## 마이그레이션 가이드

통합 thread_system 또는 독립형 logger 구현에서 마이그레이션하려면 [MIGRATION.md](MIGRATION.md)를 참조하십시오.

---

**최종 업데이트:** 2025-10-22
