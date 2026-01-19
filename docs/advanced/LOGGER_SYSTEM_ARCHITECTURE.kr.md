> **Language:** [English](LOGGER_SYSTEM_ARCHITECTURE.md) | **한국어**

# 로거 시스템 아키텍처

**버전**: 3.0.0
**최종 업데이트**: 2025-12-10

> 이 문서는 [LOGGER_SYSTEM_ARCHITECTURE.md](LOGGER_SYSTEM_ARCHITECTURE.md)의 한국어 번역 버전입니다.

## 개요

Logger System (v3.0)은 common_system의 `common::interfaces::ILogger`를 구현하는 고성능 모듈러 로깅 프레임워크입니다. 기본적으로 `std::jthread`를 사용한 **독립 실행 모드**로 동작하며, thread_system과의 통합은 선택적입니다.

### 주요 기능 (v3.0)
- **ILogger 구현**: `common::interfaces::ILogger` 완전 구현
- **독립 실행 모드**: thread_system 의존성 불필요 (`std::jthread` 사용)
- **Dual API**: `common::interfaces::log_level`과 네이티브 `logger_system::log_level` 모두 지원
- **C++20 지원**: Concepts와 `source_location` 활용
- **포괄적 에러 처리**: Result 패턴 전면 적용

## 아키텍처 다이어그램

```
┌─────────────────────────────────────────────────────────────────┐
│                      클라이언트 애플리케이션                      │
├─────────────────────────────────────────────────────────────────┤
│             common::interfaces::ILogger (API)                    │
├─────────────────────────────────────────────────────────────────┤
│                  kcenon::logger::logger                          │
│  ┌─────────────────────────────────────────────────────────┐    │
│  │                   비동기 모드 (기본값)                    │    │
│  │  ┌────────────┐  ┌──────────────┐  ┌────────────┐       │    │
│  │  │    큐      │→ │ async_worker │→ │   배치     │       │    │
│  │  │            │  │ (std::jthread│  │  프로세서  │       │    │
│  │  │            │  │  독립 실행)  │  │            │       │    │
│  │  └────────────┘  └──────────────┘  └────────────┘       │    │
│  └─────────────────────────────────────────────────────────┘    │
├─────────────────────────────────────────────────────────────────┤
│               백엔드 추상화 레이어 (v3.0)                         │
│  ┌──────────────────────┐  ┌─────────────────────────────────┐  │
│  │  standalone_backend  │  │  thread_system_backend          │  │
│  │  (기본값)            │  │  (선택적, 링크 필요)              │  │
│  └──────────────────────┘  └─────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
```

## 핵심 컴포넌트

### 1. ILogger 인터페이스 구현

```cpp
namespace kcenon::logger {

class logger : public common::interfaces::ILogger,
               public security::critical_logger_interface {
public:
    // ILogger 인터페이스 구현
    common::VoidResult log(common::interfaces::log_level level,
                           const std::string& message) override;

    common::VoidResult log(common::interfaces::log_level level,
                           std::string_view message,
                           const common::source_location& loc =
                               common::source_location::current()) override;

    bool is_enabled(common::interfaces::log_level level) const override;
    common::VoidResult set_level(common::interfaces::log_level level) override;
    common::interfaces::log_level get_level() const override;
    common::VoidResult flush() override;
};

}  // namespace kcenon::logger
```

### 2. Dual API 설계

표준화된 인터페이스와 네이티브 API 모두 지원:

```cpp
// ILogger 인터페이스 (표준화, 권장)
logger->log(common::interfaces::log_level::info, "ILogger 인터페이스 사용");

// C++20 source_location (자동)
logger->log(common::interfaces::log_level::debug, "소스 위치 자동 캡처");

// 네이티브 API (하위 호환)
logger->log(log_level::info, "네이티브 API 사용");
```

### 3. 백엔드 추상화

```cpp
namespace kcenon::logger::backends {

// 추상 백엔드 인터페이스
class integration_backend {
public:
    virtual common::interfaces::log_level to_common_level(
        logger_system::log_level level) const = 0;
    virtual logger_system::log_level from_common_level(
        common::interfaces::log_level level) const = 0;
};

// 독립 실행 백엔드 (v3.0 기본값)
class standalone_backend : public integration_backend {
    // std::jthread로 비동기 처리
    // 외부 의존성 없음
};

}  // namespace kcenon::logger::backends
```

## 성능 특성

### 벤치마크 (v3.0)

**플랫폼**: Apple M1 (8코어) @ 3.2GHz, 16GB RAM, macOS Sonoma

| 모드 | 작업 | 지연 시간 | 처리량 | 메모리 |
|------|------|----------|--------|--------|
| 비동기 (독립 실행) | 큐잉 | ~148ns | 4.34M msg/s | ~2MB |
| 비동기 (thread_sys) | 큐잉 | ~140ns | 4.34M msg/s | ~2.5MB |
| 동기 | 직접 쓰기 | ~100μs | I/O 제한 | 최소 |

### 멀티스레드 성능

| 스레드 | 독립 실행 | thread_system 통합 |
|--------|----------|-------------------|
| 1 | 4.34M msg/s | 4.34M msg/s |
| 4 | 1.07M msg/s | 1.15M msg/s |
| 8 | 412K msg/s | 450K msg/s |
| 16 | 390K msg/s | 420K msg/s |

---

*자세한 내용은 영문 버전 [LOGGER_SYSTEM_ARCHITECTURE.md](LOGGER_SYSTEM_ARCHITECTURE.md)를 참조하세요.*

---

*Last Updated: 2025-12-10*
