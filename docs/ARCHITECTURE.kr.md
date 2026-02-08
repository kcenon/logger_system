> **Language:** [English](ARCHITECTURE.md) | **한국어**

# Logger System 아키텍처

**버전**: 0.4.0
**최종 업데이트**: 2026-02-08

> 이 문서는 [ARCHITECTURE.md](ARCHITECTURE.md)의 한국어 요약 버전입니다.

## Logger 파이프라인 아키텍처

Logger 시스템은 애플리케이션 코드에서 출력 대상까지 로그 항목을 처리하는 다단계 비동기 파이프라인을 구현합니다.

### 파이프라인 흐름

```
애플리케이션 코드
    │
    ▼
┌─────────────────────────────────────────┐
│  logger (PIMPL)                         │
│                                         │
│  Sampler → Filter → Async Queue        │
│                         │               │
│  처리 스레드 (std::jthread / thread_pool) │
│     │                                   │
│     ├── Router → Formatter → Writers    │
│     ├── Realtime Analyzer (이상 감지)     │
│     └── Metrics Collector (성능 통계)     │
└─────────────────────────────────────────┘
```

### 핵심 아키텍처 결정

1. **PIMPL 패턴**: ABI 안정성과 컴파일 방화벽
2. **Move-Only log_entry**: `small_string_256`으로 힙 할당 최소화
3. **선택적 컴포넌트**: Sampler, Filter, Router, Analyzer는 비활성화 시 제로 오버헤드
4. **데코레이터 패턴**: Writer를 감싸서(encrypted, buffered, async) 조합 가능

### OTLP 관찰성 파이프라인

- OTLP writer로 OpenTelemetry 컬렉터에 로그 내보내기
- W3C Trace Context (trace_id, span_id) 자동 전파
- HTTP/gRPC 이중 프로토콜 지원
- 지수 백오프 재시도 엔진

### 샘플링 및 분석 파이프라인

- 샘플링은 큐 삽입 **전** 수행 (큐 압력 감소)
- 분석은 큐 추출 **후** 수행 (생산자 비차단)
- `always_log_levels`는 샘플링을 완전 우회 (error/critical 미삭제)

---

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

### 성능 비교

| 모드 | 지연 시간 | 처리량 | 메모리 |
|------|----------|--------|--------|
| 독립 실행 (비동기) | ~148ns | 4.34M msg/s | ~2MB |
| thread_system 통합 | ~140ns | 4.34M msg/s | ~2.5MB |

---

*자세한 내용은 영문 버전 [ARCHITECTURE.md](ARCHITECTURE.md)를 참조하세요.*

---

*Last Updated: 2026-02-08*
