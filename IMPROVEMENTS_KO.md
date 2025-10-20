# Logger System - 개선 계획

> **Language:** [English](IMPROVEMENTS.md) | **한국어**

## 현재 상태

**버전:** 1.0.0
**최종 검토:** 2025-01-20
**전체 점수:** 3.0/5

### 강점
- ABI 안정성을 위한 PIMPL idiom
- 포괄적인 writer 시스템
- 우수한 metrics 통합
- 유연한 DI 지원

### 치명적 이슈
- **async_writer 안전하지 않은 종료** - 데이터 손실 및 리소스 누수 가능
- 생성자에서 예외 throw
- 비정상 종료 시 flush 보장 안됨

---

## 치명적 이슈

### 1. async_writer 안전하지 않은 종료

**위치:** `include/kcenon/logger/writers/async_writer.h:87-131`

**현재 문제:**
```cpp
void stop() {
    // ... timeout 처리 ...
    if (status == std::future_status::timeout) {
        // ❌ 치명적 버그: 보장 없이 스레드 분리
        worker_thread_.detach();  // ❌ flush_remaining()과 경쟁 조건
    }

    // ❌ 분리된 스레드와 경쟁!
    flush_remaining();
}
```

**문제점:**
1. **Data Race:** 분리된 스레드가 여전히 실행 중일 수 있는 상태에서 `flush_remaining()`이 `message_queue_` 접근
2. **리소스 누수:** 분리된 스레드가 무한정 계속 실행
3. **로그 손실:** 중요한 메시지가 조용히 삭제될 수 있음
4. **정의되지 않은 동작:** 분리된 스레드에서 파괴된 객체 접근

**근본 원인:**
- 워커 스레드가 느린 I/O에서 차단될 수 있음 (네트워크, 디스크)
- 차단 작업에 대한 취소 메커니즘 없음
- 임의의 5초 타임아웃

**제안된 해결책:**

3단계 종료 프로세스 구현:
1. **정상 종료** (신호만 보냄)
2. **대기** (타임아웃과 함께)
3. **강제 종료** (필요시)

**우선순위:** P0 (치명적)
**작업량:** 2-3일
**영향:** 치명적 (데이터 무결성, 안정성)

---

### 2. 생성자에서 예외 throw

**위치:** `include/kcenon/logger/writers/async_writer.h:44-52`

**현재 문제:**
```cpp
explicit async_writer(std::unique_ptr<base_writer> wrapped_writer,
                     std::size_t queue_size = 10000) {
    if (!wrapped_writer_) {
        throw std::invalid_argument("Wrapped writer cannot be null");  // ❌
    }
}
```

**문제점:**
- 생성자에서 예외 throw
- 나중에 start() 호출 시 객체가 무효한 상태
- Modern C++ 모범 사례 위반

**제안된 해결책:**

Factory 함수와 Result<T> 사용:
```cpp
static result<std::unique_ptr<async_writer>>
create(std::unique_ptr<base_writer> wrapped_writer,
       std::size_t queue_size = 10000);
```

**우선순위:** P1
**작업량:** 1일
**영향:** 중간 (API 개선, 안전성)

---

## 고우선순위 개선사항

### 3. 구조화된 로깅 지원 추가

JSON 형식의 구조화된 로그 엔트리 지원

**우선순위:** P2
**작업량:** 5-7일

---

### 4. 로그 회전 지원 추가

크기 및 시간 기반 자동 로그 파일 회전

**우선순위:** P2
**작업량:** 4-5일

---

### 5. 샘플링 및 Rate Limiting 추가

높은 빈도 로그의 성능 최적화

**우선순위:** P2
**작업량:** 2-3일

---

## 중우선순위 개선사항

### 6. Context/MDC (Mapped Diagnostic Context) 추가

스레드별 컨텍스트 정보 자동 포함

**우선순위:** P3
**작업량:** 2-3일

---

## 구현 로드맵

| 단계 | 작업 | 기간 | 우선순위 |
|------|------|------|----------|
| Phase 1 | async_writer 종료 수정 | 2-3일 | P0 |
| Phase 2 | 생성자 예외 제거 | 1일 | P1 |
| Phase 3 | 구조화된 로깅 | 5-7일 | P2 |
| Phase 4 | 로그 회전 | 4-5일 | P2 |
| Phase 5 | 샘플링 & rate limiting | 2-3일 | P2 |
| Phase 6 | MDC/Context 지원 | 2-3일 | P3 |

**총 작업량:** 16-23일

---

## 참고 자료

- [Structured Logging Best Practices](https://stackify.com/what-is-structured-logging-and-why-developers-need-it/)
- [Log Rotation Strategies](https://betterstack.com/community/guides/logging/log-rotation/)
- [spdlog - Fast C++ Logging](https://github.com/gabime/spdlog)
- [Log4j2 Architecture](https://logging.apache.org/log4j/2.x/manual/architecture.html)
