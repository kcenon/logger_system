# 빠른 시작 가이드

> **Language:** [English](QUICK_START.md) | **한국어**

Logger System을 5분 만에 시작하세요.

---

## 사전 요구사항

- CMake 3.20 이상
- C++20 지원 컴파일러 (GCC 11+, Clang 14+, MSVC 2022+, Apple Clang 14+)
- Git
- **[common_system](https://github.com/kcenon/common_system)** - 필수 의존성 (logger_system과 같은 위치에 클론 필요)
- **[thread_system](https://github.com/kcenon/thread_system)** - 선택적 의존성 (스레드 풀을 사용한 고급 비동기 로깅용)

## 설치

### 1. 저장소 클론

```bash
# common_system을 먼저 클론 (필수 의존성)
git clone https://github.com/kcenon/common_system.git

# thread_system 클론 (선택 사항, 스레드 풀 비동기 로깅용)
git clone https://github.com/kcenon/thread_system.git

# 의존성과 같은 위치에 logger_system 클론
git clone https://github.com/kcenon/logger_system.git
cd logger_system
```

> **참고:** 빌드가 올바르게 작동하려면 모든 저장소가 같은 상위 디렉토리에 있어야 합니다.

### 2. 의존성 설치

```bash
# Linux/macOS
./scripts/dependency.sh

# Windows
./scripts/dependency.bat
```

### 3. 빌드

```bash
# Linux/macOS
./scripts/build.sh

# Windows
./scripts/build.bat

# 또는 CMake 직접 사용
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

# thread_system 통합 사용 시 (선택 사항)
cmake -S . -B build -DLOGGER_USE_THREAD_SYSTEM=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### 4. 설치 확인

```bash
# 샘플 애플리케이션 실행
./build/bin/basic_logging
```

---

## 첫 번째 로거 만들기

간단한 로깅 애플리케이션을 만들어 봅시다:

```cpp
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>

#include <iostream>
#include <memory>

using namespace kcenon::logger;

int main() {
    // 1. 빌더 패턴을 사용하여 로거 생성
    auto result = logger_builder()
        .with_min_level(log_level::info)
        .add_writer("console", std::make_unique<console_writer>())
        .add_writer("file", std::make_unique<file_writer>("app.log"))
        .build();

    // 2. 오류 확인
    if (result.is_err()) {
        std::cerr << "로거 생성 실패: " << result.error().message << "\n";
        return 1;
    }

    // 3. 로거 인스턴스 획득
    auto logger = std::move(result.value());

    // 4. 다양한 레벨로 메시지 로깅
    logger->log(log_level::info, "애플리케이션 시작됨");
    logger->log(log_level::debug, "디버그 정보");
    logger->log(log_level::warn, "경고: 문제가 발생할 수 있음");
    logger->log(log_level::error, "오류 발생");

    // 5. 종료 전 플러시
    logger->flush();

    std::cout << "로깅 완료! 파일 출력은 app.log를 확인하세요.\n";
    return 0;
}
```

### 애플리케이션 빌드

`CMakeLists.txt`에 추가:

```cmake
cmake_minimum_required(VERSION 3.20)
project(my_app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# logger_system 찾기
find_package(LoggerSystem REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE LoggerSystem::logger)
```

또는 FetchContent 사용:

```cmake
include(FetchContent)

FetchContent_Declare(
    common_system
    GIT_REPOSITORY https://github.com/kcenon/common_system.git
    GIT_TAG main
)

FetchContent_Declare(
    logger_system
    GIT_REPOSITORY https://github.com/kcenon/logger_system.git
    GIT_TAG main
)

FetchContent_MakeAvailable(common_system logger_system)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE kcenon::logger)
```

---

## 핵심 개념

### 로거 빌더
검증이 포함된 로거 생성의 권장 방식입니다.

```cpp
auto logger = logger_builder()
    .use_template("production")  // 미리 정의된 템플릿 사용
    .with_min_level(log_level::info)
    .build()
    .value();
```

### 라이터
로그 메시지의 출력 대상입니다.

```cpp
// 컬러 콘솔 출력
.add_writer("console", std::make_unique<console_writer>())

// 파일 출력
.add_writer("file", std::make_unique<file_writer>("app.log"))

// 로테이팅 파일 (파일당 10MB, 5개 파일 유지)
.add_writer("rotating", std::make_unique<rotating_file_writer>(
    "app.log", 10 * 1024 * 1024, 5))
```

### 로그 레벨
표준 심각도 레벨입니다.

```cpp
logger->log(log_level::trace, "상세 추적 정보");
logger->log(log_level::debug, "디버그 정보");
logger->log(log_level::info, "일반 정보");
logger->log(log_level::warn, "경고 메시지");
logger->log(log_level::error, "오류 발생");
logger->log(log_level::fatal, "치명적 오류 - 시스템 종료 중");
```

---

## 구성 템플릿

### Production (프로덕션)

```cpp
auto logger = logger_builder()
    .use_template("production")
    .build()
    .value();
```

프로덕션 환경에 최적화:
- 비동기 로깅 활성화
- 배치 처리
- 최소 레벨 Info

### Debug (디버그)

```cpp
auto logger = logger_builder()
    .use_template("debug")
    .build()
    .value();
```

개발에 이상적:
- 즉시 출력
- 최소 레벨 Debug
- 컬러 콘솔 출력

### High Performance (고성능)

```cpp
auto logger = logger_builder()
    .use_template("high_performance")
    .build()
    .value();
```

최대 처리량:
- 큰 버퍼 크기
- 적극적인 배치 처리
- 대량 메시지에 최적화

---

## CMake 구성

### 핵심 빌드 옵션

```bash
# 독립 실행 모드 (기본값, thread_system 불필요)
cmake -DLOGGER_STANDALONE_MODE=ON ..

# thread_system 통합 사용
cmake -DLOGGER_USE_THREAD_SYSTEM=ON ..

# 테스트 활성화
cmake -DBUILD_TESTS=ON ..

# 예제 활성화
cmake -DBUILD_EXAMPLES=ON ..
```

### 기능 플래그

| 옵션 | 기본값 | 설명 |
|------|--------|------|
| `LOGGER_USE_THREAD_SYSTEM` | `OFF` | 선택적 thread_system 통합 활성화 |
| `LOGGER_STANDALONE_MODE` | `ON` | 독립형 std::jthread 기반 비동기 워커 사용 |
| `BUILD_TESTS` | `ON` | 테스트 스위트 빌드 |
| `BUILD_EXAMPLES` | `ON` | 예제 애플리케이션 빌드 |

---

## thread_system 통합 (선택 사항)

스레드 풀 지원을 통한 고급 비동기 로깅:

### 빌드 시 활성화

```bash
cmake -DLOGGER_USE_THREAD_SYSTEM=ON ..
cmake --build .
```

### 런타임 활성화

```cpp
#include <kcenon/logger/integration/thread_system_integration.h>

using namespace kcenon::logger::integration;

// thread_pool 백엔드 활성화
thread_system_integration::enable();

// 로거 생성 (활성화되면 thread_pool 사용)
auto logger = logger_builder()
    .use_template("production")
    .build()
    .value();

// ... 로거 사용 ...

// 완료 시 비활성화
thread_system_integration::disable();
```

자세한 내용은 [thread_system 통합 가이드](../integration/THREAD_SYSTEM_KO.md)를 참조하세요.

---

## 일반적인 패턴

### 오류 처리

```cpp
auto result = logger_builder()
    .use_template("production")
    .build();

if (result.is_err()) {
    std::cerr << "로거 생성 실패: "
              << result.error().message << "\n";
    return 1;
}

auto logger = std::move(result.value());
```

### 여러 라이터 사용

```cpp
auto logger = logger_builder()
    .add_writer("console", std::make_unique<console_writer>())
    .add_writer("file", std::make_unique<file_writer>("app.log"))
    .add_writer("errors", std::make_unique<file_writer>("errors.log"))
    .build()
    .value();
```

### 정상 종료

```cpp
// 대기 중인 모든 로그가 기록되도록 보장
logger->flush();

// 로거는 스코프를 벗어날 때 적절히 정리됨
```

---

## 다음 단계

- **[시작하기 가이드](GETTING_STARTED.md)** - 상세한 설정 지침
- **[모범 사례](BEST_PRACTICES.md)** - 권장 패턴 및 사례
- **[마이그레이션 가이드](MIGRATION_GUIDE_KO.md)** - 다른 로거 또는 이전 버전에서 마이그레이션
- **[성능 가이드](PERFORMANCE.md)** - 최적화 팁 및 튜닝
- **[API 레퍼런스](../API_REFERENCE.md)** - 전체 API 문서
- **[예제](../../examples/)** - 더 많은 샘플 애플리케이션

---

## 문제 해결

### 일반적인 문제

**C++20 오류로 빌드 실패:**
```bash
# 호환되는 컴파일러가 있는지 확인
g++ --version  # 11+ 이어야 함
clang++ --version  # 14+ 이어야 함
```

**common_system을 찾을 수 없음:**
```bash
# common_system이 같은 상위 디렉토리에 클론되어 있는지 확인
ls ../common_system  # common_system 파일이 보여야 함
```

**vcpkg 설치 실패:**
```bash
rm -rf vcpkg
./scripts/dependency.sh
```

**테스트 실행 실패:**
```bash
cd build && ctest --verbose
```

더 많은 문제 해결 도움말은 [FAQ](FAQ.md)를 참조하세요.

---

*최종 업데이트: 2025-12-14*
