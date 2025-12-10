> **Language:** [English](ARCHITECTURE.md) | **한국어**

# 스레딩 생태계 아키텍처

**버전**: 3.0.0
**최종 업데이트**: 2025-12-10

> 이 문서는 [ARCHITECTURE.md](ARCHITECTURE.md)의 한국어 번역 버전입니다.

## v3.0 주요 변경사항

### 의존성 구조 변경

```
┌─────────────────┐
│  common_system  │ ← 기반 시스템 (필수)
└────────┬────────┘
         │
         ├─────────────────────┐
         ▼                     ▼
┌─────────────────┐   ┌─────────────────┐
│  logger_system  │   │  thread_system  │
│     v3.0        │   │    (선택적)     │
└─────────────────┘   └─────────────────┘
```

### 핵심 변경사항

| 항목 | v2.x | v3.0 |
|------|------|------|
| 필수 의존성 | thread_system | common_system |
| thread_system | 필수 | **선택적** |
| 기본 모드 | thread_system 기반 | 독립 실행 (std::jthread) |
| 인터페이스 | `thread_module::logger_interface` | `common::interfaces::ILogger` |

### 독립 실행 모드 vs 통합 모드

**독립 실행 모드 (v3.0 기본값):**
```cpp
// thread_system 의존성 없음
auto logger = kcenon::logger::logger_builder()
    .with_standalone_backend()  // std::jthread 사용
    .add_writer("console", std::make_unique<console_writer>())
    .build();
```

**통합 모드 (선택적):**
```cpp
// thread_system 통합 (LOGGER_USE_THREAD_SYSTEM=ON 필요)
auto logger = kcenon::logger::logger_builder()
    .with_thread_system_backend()
    .add_writer("console", std::make_unique<console_writer>())
    .build();
```

### 성능 비교

| 모드 | 지연 시간 | 처리량 | 메모리 |
|------|----------|--------|--------|
| 독립 실행 (비동기) | ~148ns | 4.34M msg/s | ~2MB |
| thread_system 통합 | ~140ns | 4.34M msg/s | ~2.5MB |

---

*자세한 내용은 영문 버전 [ARCHITECTURE.md](ARCHITECTURE.md)를 참조하세요.*

---

*Last Updated: 2025-12-10*
