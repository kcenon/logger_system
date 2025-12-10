> **Language:** [English](API_REFERENCE.md) | **한국어**

# API 레퍼런스

**버전**: 3.0.0
**최종 업데이트**: 2025-12-10

> 이 문서는 [API_REFERENCE.md](API_REFERENCE.md)의 한국어 번역 버전입니다.

## v3.0 주요 변경사항

### 네임스페이스 변경
- `logger_module` → `kcenon::logger`

### 인터페이스 변경
- `thread_module::logger_interface` → `common::interfaces::ILogger`

### 헤더 경로 변경
- `<logger_system/...>` → `<kcenon/logger/...>`

### 주요 기능
- **ILogger 인터페이스 구현**: common_system의 표준 인터페이스 구현
- **독립 실행 모드**: thread_system 없이 `std::jthread` 사용
- **Dual API**: `common::interfaces::log_level`과 네이티브 `log_level` 모두 지원
- **C++20 지원**: Concepts와 `source_location` 활용

## 빠른 시작

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>

using namespace kcenon::logger;

int main() {
    auto result = logger_builder()
        .use_template("production")
        .add_writer("console", std::make_unique<console_writer>())
        .build();

    if (!result) {
        std::cerr << "실패: " << result.error().message() << "\n";
        return 1;
    }

    auto logger = std::move(result.value());

    // ILogger 인터페이스 사용 (권장)
    logger->log(common::interfaces::log_level::info, "애플리케이션 시작");

    // C++20 source_location (자동 캡처)
    logger->log(common::interfaces::log_level::debug, "디버그 메시지");

    logger->flush();
    return 0;
}
```

---

*자세한 내용은 영문 버전 [API_REFERENCE.md](API_REFERENCE.md)를 참조하세요.*

---

*Last Updated: 2025-12-10*
