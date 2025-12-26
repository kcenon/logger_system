# thread_system 통합 가이드

> **Language:** [English](THREAD_SYSTEM.md) | **한국어**

## 개요

이 가이드는 logger_system에서 고급 비동기 처리를 위한 선택적 `thread_system` 통합을 활성화하고 사용하는 방법을 설명합니다.

v3.1.0 (Issue #222) 이후로 logger_system은 기본적으로 독립형 `std::jthread` 기반 비동기 워커를 사용합니다. `thread_system`의 고급 스레드 풀 기능을 활용하고자 하는 사용자를 위해 `thread_system_integration` 모듈을 통한 선택적 통합이 가능합니다 (Issue #224).

## 빠른 시작

### 빌드 시 활성화

```bash
cmake -S . -B build -DLOGGER_USE_THREAD_SYSTEM=ON
cmake --build build
```

### 런타임에 활성화

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>

using namespace kcenon::logger::integration;

// 옵션 1: 기본 스레드 풀로 활성화
thread_system_integration::enable();

// 옵션 2: 사용자 정의 스레드 풀로 활성화
auto pool = std::make_shared<kcenon::thread::thread_pool>("my_logger_pool");
pool->start();
thread_system_integration::enable(pool);

// 활성화 상태 확인
if (thread_system_integration::is_enabled()) {
    // thread_pool 백엔드 사용 중
}

// 완료 시 비활성화
thread_system_integration::disable();
```

## 설정

### CMake 옵션

| 옵션 | 기본값 | 설명 |
|------|--------|------|
| `LOGGER_USE_THREAD_SYSTEM` | `OFF` | 선택적 thread_system 통합 활성화 |

활성화되면 CMake가:
1. `thread_system` 라이브러리 검색
2. `LOGGER_HAS_THREAD_SYSTEM` 컴파일 정의 설정
3. LoggerSystem을 thread_system과 링크

### 컴파일 정의

| 정의 | 정의되는 시점 | 효과 |
|------|---------------|------|
| `LOGGER_HAS_THREAD_SYSTEM` | `LOGGER_USE_THREAD_SYSTEM=ON`이고 thread_system 발견 시 | 전체 통합 API 활성화 |

## 의존성 구성

### 양방향 의존성 위험 (Issue #252)

`logger_system`과 `thread_system`은 서로를 선택적으로 의존할 수 있습니다:

```
           ┌──────────────────────┐
           │    thread_system     │
           │                      │
           │  BUILD_WITH_LOGGER_  │◄──── 디버그 출력에 사용
           │  SYSTEM              │
           └──────────┬───────────┘
                      │
    BUILD_WITH_       │  LOGGER_USE_
    LOGGER_SYSTEM     │  THREAD_SYSTEM
                      │
           ┌──────────▼───────────┐
           │    logger_system     │
           │                      │
           │  LOGGER_HAS_THREAD_  │◄──── 비동기 I/O에 사용
           │  SYSTEM              │
           └──────────────────────┘
```

**경고**: 두 방향을 동시에 활성화하면 순환 의존성 위험이 발생합니다.

### 권장 구성 매트릭스

| 사용 사례 | thread→logger | logger→thread | CMake 플래그 | 비고 |
|----------|---------------|---------------|-------------|------|
| Logger 독립형 | OFF | OFF | (기본값) | 최소 빌드, 내부 std::jthread |
| Logger with thread_pool | OFF | **ON** | `LOGGER_USE_THREAD_SYSTEM=ON` | 비동기 I/O **권장** |
| Thread with 디버그 로깅 | ON | OFF | `BUILD_WITH_LOGGER_SYSTEM=ON` | thread_system 디버깅용 |
| 프로덕션 | OFF | **ON** | `LOGGER_USE_THREAD_SYSTEM=ON` | 프로덕션에서는 역방향 피함 |

### 안전한 구성 예제

**권장: Logger가 thread_system 사용 (단방향)**
```bash
# logger_system → thread_system만
cmake -B build \
    -DLOGGER_USE_THREAD_SYSTEM=ON \
    -DBUILD_WITH_LOGGER_SYSTEM=OFF
```

**독립형 모드 (상호 의존성 없음)**
```bash
# 통합 없음, 최소 의존성
cmake -B build \
    -DLOGGER_USE_THREAD_SYSTEM=OFF
```

**권장하지 않음: 양방향 의존성**
```bash
# 경고: 빌드 순서 문제가 발생할 수 있음
cmake -B build \
    -DLOGGER_USE_THREAD_SYSTEM=ON \
    -DBUILD_WITH_LOGGER_SYSTEM=ON  # 이 조합은 피하세요!
```

### CMake 충돌 감지

logger_system은 양방향 의존성 위험에 대한 자동 감지를 포함합니다. 두 방향이 모두 활성화되면 CMake가 경고를 출력합니다:

```
=================================================================
BIDIRECTIONAL DEPENDENCY RISK DETECTED (Issue #252)
=================================================================
  logger_system → thread_system (LOGGER_USE_THREAD_SYSTEM=ON)
  thread_system → logger_system (BUILD_WITH_LOGGER_SYSTEM=ON)

This configuration may cause:
  - Build order issues in unified builds
  - Circular header include problems
  - Initialization order complications

RECOMMENDATION:
  Enable only ONE direction. Preferred configuration:
    LOGGER_USE_THREAD_SYSTEM=ON (for async I/O performance)
    BUILD_WITH_LOGGER_SYSTEM=OFF (disable reverse dependency)
=================================================================
```

## API 레퍼런스

### thread_system_integration

thread_system 통합 관리를 제공하는 정적 클래스입니다.

#### 메서드

```cpp
namespace kcenon::logger::integration {

class thread_system_integration {
public:
    // thread_pool 백엔드 활성화 (nullptr이면 기본 풀 생성)
    static void enable(std::shared_ptr<kcenon::thread::thread_pool> pool = nullptr);

    // 비활성화하고 standalone 백엔드로 복귀
    static void disable();

    // thread_pool 백엔드 활성 여부 확인
    [[nodiscard]] static bool is_enabled() noexcept;

    // 현재 백엔드 타입 조회
    [[nodiscard]] static async_backend_type get_backend() noexcept;

    // 사용자 정의 스레드 풀 설정/조회
    static void set_thread_pool(std::shared_ptr<kcenon::thread::thread_pool> pool);
    [[nodiscard]] static std::shared_ptr<kcenon::thread::thread_pool> get_thread_pool() noexcept;

    // thread_pool에 작업 제출 (비활성화 시 false 반환)
    [[nodiscard]] static bool submit_task(std::function<void()> task);

    // 진단용 백엔드 이름 조회
    [[nodiscard]] static std::string get_backend_name() noexcept;
};

} // namespace kcenon::logger::integration
```

### async_backend_type

```cpp
enum class async_backend_type {
    standalone,   // 기본 std::jthread 기반 워커
    thread_pool   // thread_system의 thread_pool
};
```

### 컴파일 타임 감지

```cpp
// thread_system 지원 가능 여부 확인
constexpr bool has_thread_system_support() noexcept;

// 사용 예시
if constexpr (has_thread_system_support()) {
    // thread_system 기능 사용
} else {
    // standalone 기능만 사용
}
```

## 사용 패턴

### 패턴 1: 애플리케이션 전체 공유 풀

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>
#include <kcenon/logger/core/logger_builder.h>

using namespace kcenon::logger;
using namespace kcenon::logger::integration;

int main() {
    // 시작 시 thread_system 통합 활성화
    thread_system_integration::enable();

    // 로거 생성 - 가능하면 thread_pool 사용
    auto logger = logger_builder()
        .use_template("production")
        .build()
        .value();

    // ... 애플리케이션 코드 ...

    // 종료 시 정리
    thread_system_integration::disable();
    return 0;
}
```

### 패턴 2: 사용자 정의 풀 설정

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>
#include <kcenon/thread/core/thread_pool.h>

int main() {
    // 특정 설정으로 사용자 정의 스레드 풀 생성
    auto pool = std::make_shared<kcenon::thread::thread_pool>(
        "high_perf_logger"
    );

    // 풀 시작
    auto result = pool->start();
    if (!result) {
        std::cerr << "풀 시작 실패\n";
        return 1;
    }

    // 로깅에 사용자 정의 풀 사용
    thread_system_integration::enable(pool);

    // ... 애플리케이션 코드 ...

    // 정리
    thread_system_integration::disable();
    pool->stop();
    return 0;
}
```

### 패턴 3: 조건부 통합

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>

void setup_logging() {
    using namespace kcenon::logger::integration;

    // thread_system 사용 가능 시에만 활성화
    if constexpr (has_thread_system_support()) {
        thread_system_integration::enable();
        std::cout << thread_system_integration::get_backend_name()
                  << " 백엔드 사용 중\n";
    } else {
        std::cout << "standalone 백엔드 사용 중 (thread_system 미사용)\n";
    }
}
```

## 스레드 안전성

모든 `thread_system_integration` 메서드는 스레드 안전합니다:

- 백엔드 상태 변경은 원자적
- 풀 접근은 뮤텍스로 보호됨
- 여러 스레드에서 안전하게 `enable()`/`disable()` 호출 가능

## 성능 고려사항

### thread_system 통합 사용 시기

**thread_pool 백엔드 사용 권장:**
- 여러 로거가 동일한 스레드 풀 공유
- 우선순위 기반 로그 처리 필요
- 고급 메트릭 및 모니터링 필요
- 애플리케이션이 이미 thread_system 사용 중

**standalone 백엔드 사용 권장:**
- 최소 의존성 선호
- 애플리케이션당 단일 로거
- 로깅 오버헤드 최소화 필요
- thread_system 미사용

### 오버헤드 비교

| 백엔드 | 작업 제출 | 메모리 | 의존성 |
|--------|-----------|--------|--------|
| Standalone | ~50ns | 낮음 | 없음 |
| thread_pool | ~100ns | 높음 | thread_system |

## 문제 해결

### thread_system 미발견

```
-- Logger System: thread_system not found, using standalone mode
```

**해결책**: thread_system이 설치되어 있거나 FetchContent를 통해 사용 가능한지 확인:
```cmake
set(UNIFIED_USE_LOCAL ON)  # 로컬 형제 디렉토리 사용
cmake -DLOGGER_USE_THREAD_SYSTEM=ON ..
```

### 런타임 감지 미작동

`enable()` 후 `is_enabled()`가 false를 반환하는 경우:

1. 풀이 성공적으로 생성되었는지 확인
2. 풀이 실행 중인지 확인 (`pool->is_running()`)
3. 풀 생성 시 오류 확인

```cpp
auto pool = thread_system_integration::get_thread_pool();
if (pool) {
    std::cout << "풀 실행 중: " << pool->is_running() << "\n";
} else {
    std::cout << "설정된 풀 없음\n";
}
```

## 관련 문서

- [async_worker](../internals/ASYNC_WORKER.md) - Standalone 비동기 구현
- [thread_system](https://github.com/kcenon/thread_system) - 스레드 풀 라이브러리
- [Issue #224](https://github.com/kcenon/logger_system/issues/224) - 선택적 통합 기능 요청
- [Issue #252](https://github.com/kcenon/logger_system/issues/252) - 양방향 의존성 해결

---

*최종 업데이트: 2025-12*
