# 비동기 Writer 구현체 가이드

> **Language:** [English](ASYNC_WRITERS.md) | **한국어**

## 개요

Logger System은 다양한 사용 사례에 최적화된 여러 비동기 writer 구현체를 제공합니다. 이 가이드는 각 구현체의 차이점을 이해하고 적절한 구현체를 선택하는 데 도움을 줍니다.

**버전:** 1.0.0
**최종 업데이트:** 2025-12-31

---

## 목차

- [빠른 선택 가이드](#빠른-선택-가이드)
- [구현체 비교](#구현체-비교)
- [상세 설명](#상세-설명)
  - [async_writer](#1-async_writer-공개-api)
  - [async_worker](#2-async_worker-내부)
  - [high_performance_async_writer](#3-high_performance_async_writer-내부)
- [성능 특성](#성능-특성)
- [사용 예제](#사용-예제)
- [모범 사례](#모범-사례)
- [마이그레이션 가이드](#마이그레이션-가이드)

---

## 빠른 선택 가이드

| 사용 사례 | 권장 구현체 |
|----------|------------|
| **일반적인 비동기 로깅** | `async_writer` |
| **간단한 비동기 래퍼** | `async_writer` |
| **최대 처리량 (>1M msg/sec)** | `high_performance_async_writer` (내부) |
| **커스텀 비동기 태스크 처리** | `async_worker` (내부) |
| **프로덕션 애플리케이션** | `async_writer` (적절한 큐 크기 설정) |

### 선택 플로우차트

```
시작
  │
  ├─ 간단한 비동기 래퍼 필요? ─────────────► async_writer (공개 API)
  │
  ├─ 최대 처리량 필요?
  │   │
  │   └─ 예 ───────────────────────────────► high_performance_async_writer
  │                                           (내부, 커스텀 빌드 필요)
  │
  └─ 내부 컴포넌트 구축? ──────────────────► async_worker (내부 API)
```

---

## 구현체 비교

| 측면 | `async_writer` | `async_worker` | `high_performance_async_writer` |
|------|----------------|----------------|--------------------------------|
| **위치** | 공개 API | 내부 | 내부 |
| **헤더** | `writers/async_writer.h` | `src/impl/async/` | `src/impl/async/` |
| **목적** | 모든 writer 래핑 | 범용 태스크 워커 | 최대 처리량 |
| **큐 타입** | `std::queue` + mutex | `std::queue` + mutex | Lock-free 큐 |
| **메모리 풀링** | 없음 | 없음 | 있음 (설정 가능) |
| **배치 처리** | 없음 | 없음 | 있음 (동적 크기) |
| **jthread 지원** | 없음 | 있음 (fallback 포함) | 없음 |
| **성능 통계** | 없음 | 드롭 카운트만 | 전체 통계 |
| **권장 대상** | 대부분의 사용 사례 | 내부 컴포넌트 | 대용량 로깅 |

---

## 상세 설명

### 1. async_writer (공개 API)

**위치:** `include/kcenon/logger/writers/async_writer.h`

`async_writer`는 **대부분의 애플리케이션에 권장되는 선택**입니다. 모든 `base_writer` 구현체를 래핑하여 백그라운드 스레드를 통한 비동기 쓰기 기능을 제공합니다.

#### 주요 특징

- **범용 래퍼**: 모든 `base_writer`와 호환 (콘솔, 파일, 네트워크 등)
- **설정 가능한 큐 크기**: 기본값 10,000 메시지
- **설정 가능한 플러시 타임아웃**: 기본값 5초
- **정상 종료**: 중지 시 남은 메시지 처리
- **스레드 안전**: 여러 스레드에서 동시 쓰기 가능

#### 아키텍처

```
┌─────────────┐     ┌──────────────┐     ┌────────────────┐
│ 애플리케이션 │────►│ async_writer │────►│ wrapped_writer │
│   스레드    │     │   (큐)       │     │  (모든 writer) │
└─────────────┘     └──────┬───────┘     └────────────────┘
                           │
                    ┌──────┴───────┐
                    │ 워커 스레드   │
                    └──────────────┘
```

#### API 레퍼런스

```cpp
class async_writer : public base_writer {
public:
    // 생성자
    explicit async_writer(
        std::unique_ptr<base_writer> wrapped_writer,
        std::size_t queue_size = 10000,
        std::chrono::seconds flush_timeout = std::chrono::seconds(5));

    // 제어 메서드
    void start();                        // 워커 스레드 시작
    void stop(bool force_flush = true);  // 선택적 플러시와 함께 중지

    // base_writer 인터페이스
    common::VoidResult write(...) override;
    common::VoidResult flush() override;
    bool is_healthy() const override;
    std::string get_name() const override;
};
```

#### 사용 시기

- 표준 비동기 로깅 요구사항
- 느린 writer 래핑 (파일, 네트워크)
- 중간 규모 로깅 볼륨의 애플리케이션 (<100K msg/sec)
- 최대 성능보다 단순성이 중요할 때

---

### 2. async_worker (내부)

**위치:** `src/impl/async/async_worker.h`

`async_worker`는 태스크 기반 비동기 처리를 위한 **저수준 컴포넌트**입니다. 내부 사용을 위해 설계되었으며 비동기 컴포넌트 구축의 기반을 제공합니다.

#### 주요 특징

- **태스크 기반 처리**: 유연성을 위해 `std::function<void()>` 사용
- **jthread 호환성**: 가능한 경우 `std::jthread` 사용, fallback 포함
- **협력적 취소**: stop 토큰을 통한 정상 종료
- **드롭된 태스크 추적**: 큐 오버플로우 모니터링

#### 아키텍처

```
┌──────────────┐     ┌──────────────┐
│ 태스크 소스   │────►│ async_worker │────► 태스크 순차 실행
│   (내부)     │     │   (큐)       │
└──────────────┘     └──────────────┘
```

#### 사용 시기

- 내부 비동기 컴포넌트 구축
- 커스텀 태스크 처리 요구사항
- 다른 비동기 시스템과의 통합

> **참고:** 이것은 내부 API이며 공지 없이 변경될 수 있습니다.

---

### 3. high_performance_async_writer (내부)

**위치:** `src/impl/async/high_performance_async_writer.h`

`high_performance_async_writer`는 **최대 처리량** 시나리오에 최적화되어 있습니다. Lock-free 큐, 메모리 풀링, 배치 처리를 결합하여 대용량 로깅을 지원합니다.

#### 주요 특징

- **Lock-free 큐**: 높은 동시성 시나리오에서 경합 최소화
- **메모리 풀링**: 할당 오버헤드 감소
- **동적 배치 처리**: 부하에 따라 배치 크기 조정
- **백프레셔 처리**: 큐 가득 참 동작 설정 가능
- **성능 통계**: 상세 메트릭 (지연 시간, 처리량, 성공률)

#### 아키텍처

```
┌─────────────┐     ┌─────────────────────────────────────┐
│ 애플리케이션 │     │  high_performance_async_writer      │
│   스레드    │────►│  ┌─────────────┐  ┌──────────────┐ │
└─────────────┘     │  │ Lock-free   │  │ Batch        │ │
                    │  │ Queue       │─►│ Processor    │─┼──► Writer
                    │  └─────────────┘  └──────────────┘ │
                    │  ┌─────────────┐                   │
                    │  │ Memory Pool │                   │
                    │  └─────────────┘                   │
                    └─────────────────────────────────────┘
```

#### 설정

```cpp
struct config {
    // 큐 설정
    size_t queue_size{8192};              // 2의 거듭제곱이어야 함

    // 배치 처리
    batch_processor::config batch_config;
    // - initial_batch_size: 50
    // - max_batch_size: 500
    // - min_batch_size: 10
    // - max_wait_time: 100ms
    // - enable_dynamic_sizing: true
    // - enable_back_pressure: true

    // 메모리 풀
    memory::object_pool<...>::config pool_config;
    // - initial_size: 200
    // - max_size: 2000
    // - allow_growth: true

    // 기능
    bool enable_memory_pooling{true};
    bool enable_batch_processing{true};
    std::chrono::microseconds flush_timeout{1000};
};
```

#### 사용 시기

- 초당 100K 이상의 메시지 로깅
- 지연 시간에 민감한 애플리케이션
- 높은 스레드 경합이 있는 시스템
- 상세한 성능 메트릭이 필요할 때

> **참고:** 현재 내부 API입니다. 프로덕션 사용 가이드는 메인테이너에게 문의하세요.

---

## 성능 특성

### 벤치마크 비교

**테스트 환경:** Apple M1 (8코어), 16GB RAM, macOS

| 메트릭 | async_writer | high_performance_async_writer |
|--------|--------------|------------------------------|
| **단일 스레드 처리량** | ~500K msg/s | ~2M msg/s |
| **8스레드 처리량** | ~200K msg/s | ~800K msg/s |
| **평균 enqueue 지연** | ~500 ns | ~150 ns |
| **메모리 오버헤드** | 낮음 | 중간 (풀) |
| **큐 가득 참 동작** | 에러 반환 | 백프레셔 |

### 지연 시간 분포

```
async_writer:
  P50: 400 ns
  P99: 2 μs
  P99.9: 10 μs

high_performance_async_writer:
  P50: 100 ns
  P99: 500 ns
  P99.9: 2 μs
```

---

## 사용 예제

### 예제 1: 기본 async_writer 사용

```cpp
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/file_writer.h>

// 비동기 파일 writer 생성
auto file_writer = std::make_unique<kcenon::logger::file_writer>("app.log");
auto async = std::make_unique<kcenon::logger::async_writer>(
    std::move(file_writer),
    10000,  // 큐 크기
    std::chrono::seconds(5)  // 플러시 타임아웃
);

// 비동기 writer 시작
async->start();

// writer 사용
async->write(log_level::info, "Hello async!", __FILE__, __LINE__, __func__,
             std::chrono::system_clock::now());

// 플러시 및 중지
async->flush();
async->stop();
```

### 예제 2: 콘솔 출력과 async_writer

```cpp
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/console_writer.h>

// 콘솔 writer를 비동기로 래핑
auto console = std::make_unique<kcenon::logger::console_writer>();
auto async_console = std::make_unique<kcenon::logger::async_writer>(
    std::move(console),
    5000  // 콘솔용 작은 큐
);

async_console->start();
// ... async_console 사용 ...
async_console->stop(true);  // 남은 메시지 플러시
```

### 예제 3: 큐 가득 참 처리

```cpp
auto async = std::make_unique<kcenon::logger::async_writer>(
    std::make_unique<kcenon::logger::file_writer>("app.log"),
    1000  // 데모용 작은 큐
);

async->start();

// 에러 처리와 함께 쓰기
auto result = async->write(log_level::info, "Message", "", 0, "",
                           std::chrono::system_clock::now());

if (!result.is_ok()) {
    if (result.error().code() == logger_error_code::queue_full) {
        // 큐 가득 참 처리 - 동기로 쓰거나 드롭
        std::cerr << "비동기 큐 가득 참, 메시지 드롭됨\n";
    }
}
```

### 예제 4: 정상 종료 패턴

```cpp
class application {
    std::unique_ptr<async_writer> logger_;

public:
    application() {
        auto writer = std::make_unique<file_writer>("app.log");
        logger_ = std::make_unique<async_writer>(std::move(writer));
        logger_->start();
    }

    ~application() {
        // 종료 시 모든 로그 플러시 보장
        if (logger_) {
            logger_->flush();  // 큐가 비워질 때까지 대기
            logger_->stop(true);  // 플러시와 함께 중지
        }
    }

    void log(const std::string& message) {
        logger_->write(log_level::info, message, "", 0, "",
                       std::chrono::system_clock::now());
    }
};
```

---

## 모범 사례

### 1. 큐 크기 선택

```cpp
// 저용량 (<1K msg/sec): 작은 큐
async_writer(writer, 1000);

// 중간 용량 (1K-10K msg/sec): 기본값
async_writer(writer, 10000);

// 고용량 (>10K msg/sec): 큰 큐
async_writer(writer, 100000);
```

### 2. 플러시 타임아웃 설정

```cpp
// 빠른 플러시 필요 (인터랙티브 앱)
async_writer(writer, 10000, std::chrono::seconds(1));

// 표준 애플리케이션
async_writer(writer, 10000, std::chrono::seconds(5));

// 배치 지향 (백그라운드 서비스)
async_writer(writer, 10000, std::chrono::seconds(30));
```

### 3. 에러 처리

항상 `write()` 작업의 결과를 확인하세요:

```cpp
auto result = async->write(...);
if (!result.is_ok()) {
    // 대안: 동기로 쓰거나 드롭
}
```

### 4. 생명주기 관리

- 쓰기 전에 `start()` 호출
- 모든 메시지가 기록되어야 하면 `stop()` 전에 `flush()` 호출
- 자동 정리를 위해 RAII 패턴 사용

### 5. 스레드 안전성 고려사항

- `async_writer`는 쓰기에 대해 스레드 안전
- `start()`/`stop()`을 동시에 호출하지 않기
- 설정 변경 시 외부 동기화 사용

---

## 마이그레이션 가이드

### 동기에서 async_writer로

**이전:**
```cpp
auto writer = std::make_unique<file_writer>("app.log");
writer->write(level, message, file, line, func, timestamp);
```

**이후:**
```cpp
auto writer = std::make_unique<file_writer>("app.log");
auto async = std::make_unique<async_writer>(std::move(writer));
async->start();
async->write(level, message, file, line, func, timestamp);
// 잊지 마세요: 종료 시 async->stop()
```

### 큐 가득 참 마이그레이션

큐 가득 참 에러가 발생하면:

1. **큐 크기 증가:**
   ```cpp
   async_writer(writer, 50000);  // 기본값의 5배
   ```

2. **백프레셔 처리 추가:**
   ```cpp
   while (!async->write(...).is_ok()) {
       std::this_thread::sleep_for(std::chrono::milliseconds(1));
   }
   ```

3. 극단적인 경우 **high_performance_async_writer** 고려

---

## 관련 문서

- [커스텀 Writers 가이드](CUSTOM_WRITERS_KO.md) - 커스텀 writer 구현체 생성
- [아키텍처 개요](ARCHITECTURE_KO.md) - 시스템 아키텍처 및 설계
- [성능 벤치마크](../BENCHMARKS_KO.md) - 상세 성능 분석
- [Thread System 통합](../integration/THREAD_SYSTEM_KO.md) - 스레드 시스템 통합

---

**메인테이너:** kcenon@naver.com
**이슈 트래커:** [GitHub Issues](https://github.com/kcenon/logger_system/issues)
