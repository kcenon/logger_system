# Logger System 프로젝트 구조

**언어:** [English](PROJECT_STRUCTURE.md) | **한국어**

**최종 업데이트**: 2025-11-28
**버전**: 3.0.0

이 문서는 logger system의 디렉토리 구조, 주요 파일 및 모듈 의존성에 대한 상세 설명을 제공합니다.

---

## 목차

- [디렉토리 개요](#디렉토리-개요)
- [코어 모듈 파일](#코어-모듈-파일)
- [Writer 구현](#writer-구현)
- [Filter 구현](#filter-구현)
- [Formatter 구현](#formatter-구현)
- [설정 시스템](#설정-시스템)
- [보안 컴포넌트](#보안-컴포넌트)
- [테스팅 구성](#테스팅-구성)
- [빌드 시스템](#빌드-시스템)
- [모듈 의존성](#모듈-의존성)

---

## 디렉토리 개요

```
logger_system/
├── 📁 include/kcenon/logger/       # 공개 헤더 (API)
│   ├── 📁 core/                    # 코어 로깅 컴포넌트
│   ├── 📁 interfaces/              # 추상 인터페이스
│   ├── 📁 writers/                 # 로그 writer 구현
│   ├── 📁 filters/                 # 로그 filter 구현
│   ├── 📁 formatters/              # 로그 formatter 구현
│   ├── 📁 config/                  # 설정 및 템플릿
│   ├── 📁 security/                # 보안 기능
│   ├── 📁 safety/                  # 크래시 안전 메커니즘
│   ├── 📁 structured/              # 구조화된 로깅
│   ├── 📁 routing/                 # 로그 라우팅 시스템
│   ├── 📁 analysis/                # 실시간 로그 분석
│   └── 📁 server/                  # 네트워크 로그 서버
├── 📁 src/                         # 구현 파일 (.cpp)
│   ├── 📁 core/                    # 코어 구현
│   ├── 📁 modules/                 # C++20 모듈 파일 (.cppm)
│   │   ├── logger.cppm             # 기본 모듈 인터페이스
│   │   ├── core.cppm               # Core 파티션
│   │   ├── backends.cppm           # Backends 파티션
│   │   └── analysis.cppm           # Analysis 파티션
│   ├── 📁 writers/                 # Writer 구현
│   ├── 📁 filters/                 # Filter 구현
│   ├── 📁 formatters/              # Formatter 구현
│   ├── 📁 config/                  # 설정 구현
│   ├── 📁 security/                # 보안 구현
│   ├── 📁 safety/                  # 안전 구현
│   ├── 📁 structured/              # 구조화된 로깅 구현
│   ├── 📁 routing/                 # 라우팅 구현
│   ├── 📁 analysis/                # 분석 구현
│   └── 📁 server/                  # 서버 구현
├── 📁 examples/                    # 예제 애플리케이션
│   ├── 📁 basic_logging/           # 기본 사용 예제
│   ├── 📁 advanced_features/       # 고급 기능 데모
│   ├── 📁 performance_test/        # 성능 벤치마크
│   └── 📁 integration_examples/    # 에코시스템 통합
├── 📁 tests/                       # 모든 테스트
│   ├── 📁 unit/                    # 유닛 테스트 (GTest)
│   │   ├── core_tests/             # 코어 컴포넌트 테스트
│   │   ├── writer_tests/           # Writer 테스트
│   │   ├── filter_tests/           # Filter 테스트
│   │   ├── formatter_tests/        # Formatter 테스트
│   │   └── config_tests/           # 설정 테스트
│   ├── 📁 integration/             # 통합 테스트
│   │   ├── ecosystem_tests/        # 에코시스템 통합
│   │   ├── performance_tests/      # 성능 검증
│   │   └── security_tests/         # 보안 검증
│   └── 📁 benchmarks/              # 성능 벤치마크
│       ├── throughput_benchmarks/  # 처리량 테스트
│       ├── latency_benchmarks/     # 지연시간 테스트
│       └── memory_benchmarks/      # 메모리 프로파일링
├── 📁 docs/                        # 문서
│   ├── 📁 guides/                  # 사용자 가이드
│   ├── 📁 advanced/                # 고급 주제
│   ├── 📁 performance/             # 성능 문서
│   └── 📁 contributing/            # 기여 가이드
├── 📁 cmake/                       # CMake 모듈
│   ├── LoggerSystemConfig.cmake.in # 패키지 설정 템플릿
│   ├── FindThreadSystem.cmake      # Thread 시스템 finder
│   └── CompilerWarnings.cmake      # 컴파일러 경고 플래그
├── 📁 .github/                     # GitHub 워크플로우
│   └── 📁 workflows/               # CI/CD 파이프라인
│       ├── ci.yml                  # 메인 CI 파이프라인
│       ├── sanitizers.yml          # 새니타이저 테스트
│       ├── benchmarks.yml          # 성능 벤치마크
│       ├── coverage.yml            # 코드 커버리지
│       └── static-analysis.yml     # 정적 분석
├── 📄 CMakeLists.txt               # 루트 빌드 설정
├── 📄 vcpkg.json                   # 의존성 매니페스트
├── 📄 LICENSE                      # BSD 3-Clause 라이선스
├── 📄 README.md                    # 메인 문서
└── 📄 README.kr.md                 # 한국어 문서
```

---

## 코어 모듈 파일

### logger.h / logger.cpp
**위치**: `include/kcenon/logger/core/` | `src/core/`

**목적**: 비동기 처리를 지원하는 메인 로거 클래스

**주요 컴포넌트**:
- `logger` 클래스: 기본 로깅 인터페이스
- 비동기 큐 관리
- Writer 조정
- Filter 적용
- 메트릭 수집

**공개 API**:
```cpp
class logger {
public:
    // 로깅 연산
    auto log(log_level level, const std::string& message) -> void;
    auto log(log_level level, const std::string& message,
             const std::string& file, int line, const std::string& func) -> void;

    // Writer 관리
    auto add_writer(const std::string& name,
                    std::unique_ptr<log_writer_interface> writer) -> common::VoidResult;
    auto remove_writer(const std::string& name) -> common::VoidResult;

    // 설정
    auto set_filter(std::unique_ptr<log_filter_interface> filter) -> common::VoidResult;
    auto set_min_level(log_level level) -> void;

    // 제어
    auto start() -> common::VoidResult;
    auto stop() -> common::VoidResult;
    auto flush() -> common::VoidResult;

    // 메트릭
    auto enable_metrics_collection(bool enabled) -> void;
    auto get_current_metrics() const -> metrics_data;
};
```

**의존성**:
- `log_entry.h` - 로그 엔트리 데이터 구조
- `log_writer_interface.h` - Writer 추상화
- `log_filter_interface.h` - Filter 추상화
- Thread 시스템 큐 (선택적)

---

### logger_builder.h / logger_builder.cpp
**위치**: `include/kcenon/logger/core/` | `src/core/`

**목적**: 검증이 포함된 로거 설정을 위한 빌더 패턴

**주요 기능**:
- 플루언트 API 설계
- 자동 검증
- 템플릿 지원 (production, debug, high_performance, low_latency)
- Result<T>를 이용한 오류 처리

**공개 API**:
```cpp
class logger_builder {
public:
    // 템플릿 설정
    auto use_template(const std::string& template_name) -> logger_builder&;

    // 기본 설정
    auto with_min_level(log_level level) -> logger_builder&;
    auto with_buffer_size(size_t size) -> logger_builder&;
    auto with_batch_size(size_t size) -> logger_builder&;
    auto with_queue_size(size_t size) -> logger_builder&;

    // 컴포넌트 추가
    auto add_writer(const std::string& name,
                    std::unique_ptr<log_writer_interface> writer) -> logger_builder&;
    auto add_filter(std::unique_ptr<log_filter_interface> filter) -> logger_builder&;

    // 빌드
    auto build() -> result<std::unique_ptr<logger>>;

private:
    auto validate() const -> common::VoidResult;
};
```

---

### log_entry.h
**위치**: `include/kcenon/logger/core/`

**목적**: 메타데이터가 포함된 로그 엔트리 데이터 구조

**구조**:
```cpp
struct log_entry {
    log_level level;                                    // 로그 심각도 레벨
    std::string message;                                // 로그 메시지
    std::chrono::system_clock::time_point timestamp;    // 로깅 시간
    std::string file;                                   // 소스 파일
    int line;                                           // 라인 번호
    std::string function;                               // 함수 이름
    std::thread::id thread_id;                          // 스레드 ID
    std::map<std::string, std::string> context;         // 추가 컨텍스트
};
```

**기능**:
- 경량 및 복사 가능
- 완전한 메타데이터 캡처
- 확장 가능한 컨텍스트 필드
- 스레드 안전한 설계

---

### result_types.h
**위치**: `include/kcenon/logger/core/`

**목적**: 오류 처리 타입 및 유틸리티

**오류 코드** (범위: -200 ~ -299):
- 시스템 라이프사이클: -200 ~ -209
- Writer 관리: -210 ~ -219
- 설정: -220 ~ -229
- I/O 연산: -230 ~ -239

---

## Writer 구현

### console_writer.h / console_writer.cpp
**위치**: `include/kcenon/logger/writers/` | `src/writers/`

**목적**: ANSI 지원 컬러 콘솔 출력

**기능**:
- 다른 로그 레벨에 대한 ANSI 컬러 코드
- 크로스 플랫폼 지원 (Windows, Linux, macOS)
- 설정 가능한 컬러 스킴
- 스레드 안전 출력

---

### file_writer.h / file_writer.cpp
**위치**: `include/kcenon/logger/writers/` | `src/writers/`

**목적**: 버퍼링이 포함된 기본 파일 쓰기

**기능**:
- 성능을 위한 버퍼링 I/O
- 자동 디렉토리 생성
- 설정 가능한 파일 권한 (기본: 0600)
- 스레드 안전 파일 접근

**설정**:
```cpp
struct file_writer_config {
    std::string filename;
    size_t buffer_size = 8192;           // 8KB 기본
    bool append = true;
    mode_t file_permissions = 0600;      // 소유자 읽기/쓰기만
    std::chrono::milliseconds flush_interval = std::chrono::milliseconds(100);
};
```

---

### rotating_file_writer.h / rotating_file_writer.cpp
**위치**: `include/kcenon/logger/writers/` | `src/writers/`

**목적**: 크기 및 시간 기반 파일 로테이션

**기능**:
- 크기 기반 로테이션
- 시간 기반 로테이션 (일별, 시간별, 주별)
- 백업 파일 관리
- 선택적 압축 (gzip, bzip2)

**설정**:
```cpp
enum class rotation_type {
    size,       // 파일 크기 기반 로테이션
    daily,      // 일별 로테이션
    hourly,     // 시간별 로테이션
    weekly      // 주별 로테이션
};

struct rotating_file_writer_config {
    std::string base_filename;
    rotation_type type = rotation_type::size;
    size_t max_size_bytes = 10 * 1024 * 1024;  // 10MB
    size_t max_files = 5;                       // 5개 백업 유지
    compression_type compression = compression_type::none;
    int rotation_hour = 0;                      // 시간 기반 로테이션용
};
```

---

### network_writer.h / network_writer.cpp
**위치**: `include/kcenon/logger/writers/` | `src/writers/`

**목적**: TCP/UDP 네트워크 로깅

**기능**:
- TCP 및 UDP 프로토콜 지원
- 자동 재연결
- 설정 가능한 재시도 전략
- 메시지 배칭

---

### critical_writer.h / critical_writer.cpp
**위치**: `include/kcenon/logger/writers/` | `src/writers/`

**목적**: 중요 메시지를 위한 동기식 로깅

**기능**:
- 비동기 큐 우회
- 즉시 쓰기 보장
- 래퍼 패턴 (모든 writer 래핑 가능)
- 크래시 안전

**사용 패턴**:
```cpp
auto critical = std::make_unique<critical_writer>(
    std::make_unique<file_writer>("critical.log")
);
```

---

### hybrid_writer.h / hybrid_writer.cpp
**위치**: `include/kcenon/logger/writers/` | `src/writers/`

**목적**: 로그 레벨에 따른 적응형 async/sync

**기능**:
- 자동 모드 전환
- 설정 가능한 임계값 레벨
- 별도 비동기 큐
- 성능 + 신뢰성

---

## Filter 구현

### level_filter.h / level_filter.cpp
**위치**: `include/kcenon/logger/filters/` | `src/filters/`

**목적**: 최소 로그 레벨로 필터링

---

### regex_filter.h / regex_filter.cpp
**위치**: `include/kcenon/logger/filters/` | `src/filters/`

**목적**: 로그 메시지에 대한 패턴 매칭

**기능**:
- 포함 또는 제외 패턴
- 대소문자 구분 또는 무시
- 다중 패턴 지원

---

### function_filter.h / function_filter.cpp
**위치**: `include/kcenon/logger/filters/` | `src/filters/`

**목적**: 람다/함수를 통한 커스텀 필터링 로직

---

## Formatter 구현

### plain_formatter.h / plain_formatter.cpp
**목적**: 간단하고 사람이 읽을 수 있는 텍스트 형식

---

### json_formatter.h / json_formatter.cpp
**목적**: 기계가 파싱 가능한 JSON 출력

---

### custom_formatter.h / custom_formatter.cpp
**목적**: 커스텀 포매팅 로직을 위한 베이스 클래스

---

## 설정 시스템

### config_templates.h / config_templates.cpp
**위치**: `include/kcenon/logger/config/` | `src/config/`

**목적**: 사전 정의된 설정

**템플릿**:
- `production`: 프로덕션 환경에 최적화
- `debug`: 개발을 위한 즉시 출력
- `high_performance`: 최대 처리량
- `low_latency`: 최소 지연시간

---

### config_validator.h / config_validator.cpp
**위치**: `include/kcenon/logger/config/` | `src/config/`

**목적**: 포괄적인 검증 프레임워크

**검증 항목**:
- 버퍼 크기 범위
- 큐 크기 제한
- Writer 설정
- Filter 설정

---

## 보안 컴포넌트

### secure_key_storage.h / secure_key_storage.cpp
**위치**: `include/kcenon/logger/security/` | `src/security/`

**목적**: RAII 기반 암호화 키 관리

**기능**:
- 보안 삭제를 위한 OpenSSL `OPENSSL_cleanse()`
- 파일 권한 강제 (0600)
- 암호학적으로 안전한 키 생성

---

### path_validator.h / path_validator.cpp
**위치**: `include/kcenon/logger/security/` | `src/security/`

**목적**: 경로 검증 및 보안

**보호 기능**:
- 경로 순회 방지
- 심볼릭 링크 검증
- 기본 디렉토리 강제
- 파일명 문자 제한

---

### log_sanitizer.h / log_sanitizer.cpp
**위치**: `include/kcenon/logger/security/` | `src/security/`

**목적**: 민감한 데이터 새니타이징

**기능**:
- 이메일 마스킹
- 신용카드 마스킹
- 토큰 삭제
- 커스텀 패턴 지원

---

## 테스팅 구성

### 유닛 테스트
**위치**: `tests/unit/`

**구조**:
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

**테스팅 프레임워크**: Google Test (GTest)
**커버리지 목표**: 80%+

---

### 통합 테스트
**위치**: `tests/integration/`

**중점 영역**:
- 에코시스템 통합 (thread_system, monitoring_system)
- 성능 검증
- 보안 검증
- 멀티 writer 시나리오

---

### 벤치마크
**위치**: `tests/benchmarks/`

**벤치마크 유형**:
- 처리량 벤치마크 (싱글/멀티스레드)
- 지연시간 벤치마크 (백분위 분석)
- 메모리 벤치마크 (풋프린트, 할당 패턴)

---

## 빌드 시스템

### CMakeLists.txt (루트)
**위치**: `CMakeLists.txt`

**주요 옵션**:
```cmake
# 코어 기능
option(LOGGER_USE_DI "의존성 주입 활성화" ON)
option(LOGGER_USE_MONITORING "모니터링 지원 활성화" ON)
option(LOGGER_ENABLE_ASYNC "비동기 로깅 활성화" ON)
option(LOGGER_ENABLE_CRASH_HANDLER "크래시 핸들러 활성화" ON)

# 고급 기능
option(LOGGER_USE_LOCK_FREE_QUEUE "락프리 큐 사용" OFF)
option(LOGGER_ENABLE_STRUCTURED_LOGGING "JSON 로깅 활성화" OFF)
option(LOGGER_ENABLE_NETWORK_WRITER "네트워크 writer 활성화" OFF)
option(LOGGER_ENABLE_FILE_ROTATION "파일 로테이션 활성화" ON)

# 성능 튜닝
set(LOGGER_DEFAULT_BUFFER_SIZE 16384 CACHE STRING "바이트 단위 버퍼 크기")
set(LOGGER_DEFAULT_BATCH_SIZE 200 CACHE STRING "배치 처리 크기")
set(LOGGER_DEFAULT_QUEUE_SIZE 20000 CACHE STRING "최대 큐 크기")

# 품질 보증
option(LOGGER_ENABLE_SANITIZERS "새니타이저 활성화" OFF)
set(LOGGER_SANITIZER_TYPE "address" CACHE STRING "새니타이저 유형")
option(LOGGER_ENABLE_WARNINGS "컴파일러 경고 활성화" ON)
option(LOGGER_WARNINGS_AS_ERRORS "경고를 오류로 처리" OFF)
option(LOGGER_ENABLE_COVERAGE "코드 커버리지 활성화" OFF)
```

---

## 모듈 의존성

### 의존성 그래프

```
┌─────────────────┐
│  config 모듈    │  (의존성 없음)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   core 모듈     │
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
         │  integration (선택적)         │
         │  - thread_system             │
         │  - monitoring_system         │
         └──────────────────────────────┘
```

### 외부 의존성

**필수**:
- **fmt**: 문자열 포매팅 라이브러리 (헤더 전용 모드 가능)
- **C++17 컴파일러**: GCC 7+, Clang 5+, MSVC 2017+

**선택적**:
- **thread_system**: 스레딩 프리미티브 및 로거 인터페이스
- **common_system**: 코어 인터페이스 (ILogger, IMonitor, Result<T>)
- **monitoring_system**: 메트릭 수집 및 헬스 모니터링
- **OpenSSL 3.x** (권장): 보안 키 저장 및 암호화 지원
  - OpenSSL 1.1.x 지원되지만 deprecated (2023년 9월 EOL)

---

## 파일 명명 규칙

### 헤더 파일
- **인터페이스 헤더**: `*_interface.h` (예: `log_writer_interface.h`)
- **구현 헤더**: `*.h` (예: `logger.h`, `file_writer.h`)
- **설정 헤더**: `config_*.h` (예: `config_validator.h`)

### 소스 파일
- **구현**: `*.cpp` (헤더 이름과 일치)
- **테스트**: `*_test.cpp` (예: `logger_test.cpp`)
- **벤치마크**: `*_benchmark.cpp` (예: `throughput_benchmark.cpp`)

### 문서 파일
- **Markdown**: `*.md` (영어) 또는 `*.kr.md` (한국어)
- **API 문서**: `API_REFERENCE.md`
- **가이드**: `docs/guides/`의 `GUIDE_NAME.md`

---

## 참고 문서

- [아키텍처 개요](01-architecture.md) / [아키텍처 (한국어)](ARCHITECTURE.kr.md) - 시스템 설계 및 아키텍처
- [API 레퍼런스](02-API_REFERENCE.md) - 완전한 API 문서
- [빌드 가이드](guides/BUILD_GUIDE.md) - 상세 빌드 지침
- [기여 가이드](contributing/CONTRIBUTING.md) - 기여 가이드라인

---

**최종 업데이트**: 2025-11-28
**버전**: 3.0.0

---

Made with ❤️ by 🍀☀🌕🌥 🌊
