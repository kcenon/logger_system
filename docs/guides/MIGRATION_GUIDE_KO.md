> **Language:** [English](MIGRATION_GUIDE.md) | **한국어**

# 로거 시스템 마이그레이션 가이드

**버전**: 3.0.0
**최종 업데이트**: 2025-12-10

> 이 문서는 [MIGRATION_GUIDE.md](MIGRATION_GUIDE.md)의 한국어 번역 버전입니다.

## 개요

이 가이드는 다음으로부터의 마이그레이션을 도와줍니다:
- Logger System 이전 버전 (v2.x → v3.0, v1.x → v2.x)
- 다른 로깅 라이브러리 (spdlog, Boost.Log, glog, log4cpp)
- 커스텀 로깅 솔루션

## v2.x에서 v3.0으로 마이그레이션

### 주요 변경사항 요약

| 버전 | 항목 | 변경 내용 | 영향도 |
|------|------|----------|--------|
| **v3.0** | 네임스페이스 | `logger_module` → `kcenon::logger` | **높음** |
| **v3.0** | 인터페이스 | `common::interfaces::ILogger` 구현 | **높음** |
| **v3.0** | 의존성 | thread_system 선택적 | 중간 |
| **v3.0** | C++ 표준 | C++20 필요 | 중간 |
| **v3.0** | Result 타입 | `common::VoidResult` 사용 | 중간 |

### 1. 네임스페이스 마이그레이션

**기존 (v2.x):**
```cpp
#include <logger_system/logger.h>
using namespace logger_module;
```

**신규 (v3.0):**
```cpp
#include <kcenon/logger/core/logger.h>
using namespace kcenon::logger;
```

### 2. 인터페이스 마이그레이션

**기존 (v2.x):**
```cpp
logger->log(thread_module::log_level::info, "메시지");
```

**신규 (v3.0):**
```cpp
// ILogger 인터페이스 (권장)
logger->log(common::interfaces::log_level::info, "메시지");

// 네이티브 API (하위 호환)
logger->log(log_level::info, "메시지");
```

### 3. Result 타입 마이그레이션

**기존 (v2.x):**
```cpp
result_void result = logger->log(level, message);
```

**신규 (v3.0):**
```cpp
common::VoidResult result = logger->log(level, message);
// 또는
auto result = logger->log(level, message);
```

### 4. thread_system 의존성 변경

**기존 (v2.x):**
```cmake
find_package(thread_system REQUIRED)
```

**신규 (v3.0):**
```cmake
find_package(common_system REQUIRED)  # 필수
find_package(logger_system REQUIRED)
# find_package(thread_system OPTIONAL)  # 필요시에만
```

### 5. 백엔드 선택 (v3.0 신규)

```cpp
// 독립 실행 모드 (기본값, thread_system 불필요)
auto logger = logger_builder()
    .with_standalone_backend()
    .build();

// thread_system 통합 (LOGGER_USE_THREAD_SYSTEM=ON 필요)
auto logger = logger_builder()
    .with_thread_system_backend()  // deprecated
    .build();
```

### 6. C++20 source_location (v3.0 신규)

```cpp
// 소스 위치 자동 캡처
logger->log(common::interfaces::log_level::info, "디버그 메시지");
// 출력: [INFO] [main.cpp:42] [main()] 디버그 메시지
```

## 마이그레이션 체크리스트

### v2.x → v3.0 마이그레이션

- [ ] 빌드 시스템에서 C++20 활성화
- [ ] common_system 의존성 업데이트
- [ ] include 경로 업데이트 (`logger_system/` → `kcenon/logger/`)
- [ ] 네임스페이스 업데이트 (`logger_module` → `kcenon::logger`)
- [ ] 인터페이스 참조 업데이트 (`thread_module::logger_interface` → `common::interfaces::ILogger`)
- [ ] 로그 레벨 타입 업데이트 (`thread_module::log_level` → `common::interfaces::log_level`)
- [ ] Result 타입 업데이트 (`result_void` → `common::VoidResult`)
- [ ] 불필요시 thread_system 의존성 제거
- [ ] deprecated 메서드 호출 업데이트 (`set_min_level` → `set_level`)
- [ ] sanitizer 테스트
- [ ] 성능 벤치마크
- [ ] 문서 업데이트

## 일반적인 문제와 해결책

### 문제 1: 네임스페이스를 찾을 수 없음

**에러:**
```
error: 'logger_module' is not a namespace-name
```

**해결:**
```cpp
// 변경 전
using namespace logger_module;

// 변경 후
using namespace kcenon::logger;
```

### 문제 2: 인터페이스 타입 불일치

**에러:**
```
error: 'thread_module::logger_interface' is not a base of 'kcenon::logger::logger'
```

**해결:**
```cpp
// 변경 전
thread_module::logger_interface* logger = ...;

// 변경 후
common::interfaces::ILogger* logger = ...;
```

### 문제 3: C++20 미활성화

**에러:**
```
error: 'source_location' is not a member of 'std'
```

**해결:**
```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

---

*자세한 내용은 영문 버전 [MIGRATION_GUIDE.md](MIGRATION_GUIDE.md)를 참조하세요.*

---

*Last Updated: 2025-12-10*
