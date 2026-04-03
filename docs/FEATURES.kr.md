---
doc_id: "LOG-FEAT-001"
doc_title: "Logger System - 상세 기능"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "FEAT"
---

# Logger System - 상세 기능

**언어:** [English](README.md) | **한국어**

**최종 업데이트**: 2026-02-08
**버전**: 0.4.0

이 문서는 Logger System의 모든 기능에 대한 포괄적인 세부 정보를 제공합니다.

---

## 목차

- [핵심 기능](#핵심-기능)
- [로그 레벨](#로그-레벨)
- [로그 작성자](#로그-작성자)
- [포맷팅](#포맷팅)
- [필터링](#필터링)
- [비동기 로깅](#비동기-로깅)
- [성능 특성](#성능-특성)
- [통합 기능](#통합-기능)
- [OTLP 통합](#otlp-통합)
- [로그 샘플링](#로그-샘플링)
- [실시간 로그 분석](#실시간-로그-분석)

---

## 핵심 기능

### 설계 원칙

Logger System은 다음 원칙을 따라 설계되었습니다:

- **고성능**: 비동기 로깅으로 애플리케이션 성능에 최소한의 영향
- **유연성**: 다양한 출력 대상 및 포맷 지원
- **스레드 안전성**: 다중 스레드 환경에서 안전한 사용
- **확장성**: 커스텀 작성자 및 포맷터 지원
- **통합 용이성**: 에코시스템 컴포넌트와 원활한 통합

### 주요 기능

| 기능 | 설명 | 상태 |
|------|------|------|
| **비동기 로깅** | 백그라운드 스레드에서 로그 처리 | ✅ |
| **다중 출력** | 콘솔, 파일, 원격 등 | ✅ |
| **로그 로테이션** | 크기/시간 기반 자동 로테이션 | ✅ |
| **구조화된 로깅** | JSON 형식 로그 지원 | ✅ |
| **컨텍스트 추적** | 요청 ID, 트레이스 ID 전파 | ✅ |
| **조건부 로깅** | 런타임 레벨 조정 | ✅ |

---

## 로그 레벨

### 레벨 정의

```cpp
namespace kcenon::logger {
    enum class log_level {
        trace = 0,    // 상세 추적 정보
        debug = 1,    // 디버깅 정보
        info = 2,     // 일반 정보
        warning = 3,  // 경고 (비치명적 문제)
        error = 4,    // 오류 (복구 가능한 문제)
        critical = 5  // 치명적 오류 (시스템 위험)
    };
}
```

### 레벨별 사용 지침

| 레벨 | 사용 시나리오 | 예시 |
|------|--------------|------|
| **TRACE** | 상세 실행 흐름 추적 | 함수 진입/종료, 변수 값 |
| **DEBUG** | 개발 중 디버깅 정보 | 알고리즘 단계, 중간 결과 |
| **INFO** | 일반 운영 정보 | 서비스 시작/종료, 설정 로드 |
| **WARNING** | 잠재적 문제 경고 | 사용 중단 API, 리소스 부족 |
| **ERROR** | 복구 가능한 오류 | 네트워크 오류, 파일 읽기 실패 |
| **CRITICAL** | 시스템 위험 상황 | 메모리 부족, 데이터 손상 |

### 사용 예시

```cpp
#include <kcenon/logger/logger.h>

using namespace kcenon::logger;

auto logger = create_logger("my_app");

// 다양한 레벨 로깅
logger->trace("함수 진입: process_data()");
logger->debug("처리할 항목 수: {}", items.size());
logger->info("서버가 포트 {}에서 시작됨", port);
logger->warning("캐시 사용량 {}% 도달", cache_usage);
logger->error("데이터베이스 연결 실패: {}", error_msg);
logger->critical("메모리 할당 실패, 시스템 종료 중");
```

---

## 로그 작성자

### 콘솔 작성자

```cpp
#include <kcenon/logger/writers/console_writer.h>

// 콘솔 작성자 생성
auto console = std::make_shared<console_writer>();

// 색상 출력 설정
console->enable_colors(true);

// 로거에 추가
logger->add_writer(console);
```

### 파일 작성자

```cpp
#include <kcenon/logger/writers/file_writer.h>

// 기본 파일 작성자
auto file = std::make_shared<file_writer>("/var/log/app.log");

// 로테이션 설정
file->set_max_size(10 * 1024 * 1024);  // 10MB
file->set_max_files(5);                 // 최대 5개 파일 유지

// 로거에 추가
logger->add_writer(file);
```

### 회전 파일 작성자

```cpp
#include <kcenon/logger/writers/rotating_file_writer.h>

// 일별 로테이션
auto daily = std::make_shared<rotating_file_writer>(
    "/var/log/app.log",
    rotation_policy::daily
);

// 크기 기반 로테이션
auto sized = std::make_shared<rotating_file_writer>(
    "/var/log/app.log",
    rotation_policy::size_based,
    50 * 1024 * 1024  // 50MB
);

logger->add_writer(daily);
```

### 원격 작성자

```cpp
#include <kcenon/logger/writers/remote_writer.h>

// 원격 로그 서버로 전송
auto remote = std::make_shared<remote_writer>(
    "log-server.example.com",
    5514,
    remote_protocol::syslog
);

// TLS 암호화
remote->enable_tls(true);

logger->add_writer(remote);
```

### 커스텀 작성자

```cpp
class custom_writer : public log_writer {
public:
    void write(const log_entry& entry) override {
        // 커스텀 로직 구현
        send_to_external_service(entry);
    }

    void flush() override {
        // 버퍼 플러시
    }
};

auto custom = std::make_shared<custom_writer>();
logger->add_writer(custom);
```

---

## 포맷팅

### 기본 포맷

```cpp
// 기본 포맷 설정
logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] %v");

// 사용 가능한 패턴:
// %Y - 연도 (4자리)
// %m - 월 (2자리)
// %d - 일 (2자리)
// %H - 시 (24시간)
// %M - 분
// %S - 초
// %e - 밀리초
// %l - 레벨 (INFO, ERROR 등)
// %n - 로거 이름
// %v - 메시지
// %t - 스레드 ID
// %P - 프로세스 ID
// %@ - 소스 위치 (파일:라인)
```

### JSON 포맷

```cpp
#include <kcenon/logger/formatters/json_formatter.h>

auto json_formatter = std::make_shared<json_formatter>();
logger->set_formatter(json_formatter);

// 출력 예시:
// {"timestamp":"2025-11-28T10:30:00.123Z","level":"INFO","logger":"my_app","message":"서버 시작됨","thread":12345}
```

### 커스텀 포맷터

```cpp
class custom_formatter : public log_formatter {
public:
    std::string format(const log_entry& entry) override {
        return fmt::format("[{}] {} - {}",
            entry.level_name(),
            entry.timestamp_str(),
            entry.message());
    }
};

auto custom = std::make_shared<custom_formatter>();
logger->set_formatter(custom);
```

---

## 필터링

### 레벨 필터링

```cpp
// 전역 최소 레벨 설정
logger->set_level(log_level::info);  // INFO 이상만 로깅

// 작성자별 레벨 설정
console_writer->set_level(log_level::debug);  // 콘솔은 DEBUG 이상
file_writer->set_level(log_level::warning);   // 파일은 WARNING 이상
```

### 패턴 필터링

```cpp
#include <kcenon/logger/filters/pattern_filter.h>

// 특정 패턴 포함/제외
auto filter = std::make_shared<pattern_filter>();
filter->include_pattern(".*database.*");  // database 포함 메시지만
filter->exclude_pattern(".*password.*");   // password 포함 메시지 제외

logger->add_filter(filter);
```

### 조건부 필터링

```cpp
#include <kcenon/logger/filters/conditional_filter.h>

// 조건부 필터
auto filter = std::make_shared<conditional_filter>([](const log_entry& entry) {
    // 영업 시간에만 INFO 레벨 허용
    auto hour = entry.timestamp().hour();
    if (entry.level() == log_level::info) {
        return hour >= 9 && hour <= 18;
    }
    return true;
});

logger->add_filter(filter);
```

---

## 비동기 로깅

### 비동기 모드 활성화

```cpp
#include <kcenon/logger/async_logger.h>

// 비동기 로거 생성
auto async_logger = create_async_logger("my_app", {
    .queue_size = 8192,           // 로그 큐 크기
    .flush_interval_ms = 100,     // 플러시 간격
    .overflow_policy = overflow_policy::block  // 큐 오버플로우 정책
});

// 오버플로우 정책:
// - block: 큐가 빌 때까지 대기
// - drop_oldest: 가장 오래된 로그 삭제
// - drop_newest: 새 로그 삭제
```

### 성능 비교

| 모드 | 처리량 | 지연시간 (p99) |
|------|--------|----------------|
| 동기 | 100K logs/s | 10μs |
| 비동기 (block) | 1M logs/s | 1μs |
| 비동기 (drop) | 2M logs/s | 500ns |

---

## 성능 특성

### 벤치마크 결과

| 시나리오 | 처리량 | 지연시간 (p50) | 지연시간 (p99) |
|----------|--------|----------------|----------------|
| 콘솔 출력 | 50K/s | 5μs | 20μs |
| 파일 쓰기 | 200K/s | 2μs | 10μs |
| 비동기 큐 | 1M/s | 200ns | 1μs |
| JSON 포맷 | 100K/s | 3μs | 15μs |

### 최적화 팁

1. **비동기 로깅 사용**: 고처리량 애플리케이션에 필수
2. **적절한 레벨 설정**: 프로덕션에서는 INFO 이상 권장
3. **버퍼링 활용**: 파일 작성자의 버퍼 크기 조정
4. **필터 최소화**: 필터가 많을수록 성능 저하

---

## 통합 기능

### thread_system 통합

```cpp
#include <kcenon/logger/integration/thread_logger.h>

// 스레드 풀과 통합
auto pool = create_thread_pool(4);
auto logger = create_thread_logger("worker", pool);

// 스레드별 컨텍스트 자동 추가
pool->enqueue([logger]() {
    logger->info("작업 시작");  // 스레드 ID 자동 포함
});
```

### monitoring_system 통합

```cpp
#include <kcenon/logger/integration/metrics_logger.h>

// 메트릭 수집과 통합
auto metrics = create_metrics_collector();
logger->set_metrics_collector(metrics);

// 자동으로 수집되는 메트릭:
// - 레벨별 로그 수
// - 로그 처리 지연시간
// - 큐 사용률
```

### common_system 통합

```cpp
#include <kcenon/logger/integration/result_logger.h>

// Result<T> 오류 자동 로깅
auto result = perform_operation();
if (!result) {
    logger->log_error(result.error());  // ErrorInfo 자동 포맷
}
```

---

## 구성 예시

### 프로덕션 설정

```cpp
// 프로덕션 권장 설정
auto logger = create_async_logger("production_app", {
    .queue_size = 16384,
    .flush_interval_ms = 50,
    .overflow_policy = overflow_policy::drop_oldest
});

logger->set_level(log_level::info);

// 파일 작성자 (일별 로테이션, 30일 보관)
auto file = std::make_shared<rotating_file_writer>(
    "/var/log/app/app.log",
    rotation_policy::daily,
    0,  // 크기 제한 없음
    30  // 30개 파일 보관
);
file->set_level(log_level::info);
logger->add_writer(file);

// 오류 전용 파일
auto error_file = std::make_shared<file_writer>("/var/log/app/error.log");
error_file->set_level(log_level::error);
logger->add_writer(error_file);
```

### 개발 설정

```cpp
// 개발 환경 설정
auto logger = create_logger("dev_app");
logger->set_level(log_level::debug);

// 색상 콘솔 출력
auto console = std::make_shared<console_writer>();
console->enable_colors(true);
console->set_level(log_level::debug);
logger->add_writer(console);

// 상세 포맷
logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%@] %v");
```

---

## OTLP 통합

관찰성 백엔드로 로그를 내보내기 위한 OpenTelemetry Protocol (OTLP) 지원입니다.

### 개요

OTLP writer는 분산 추적 상관관계를 위한 W3C Trace Context 지원과 함께 OpenTelemetry 호환 컬렉터(Jaeger, Zipkin, Grafana Tempo 등)로 로그 항목을 내보냅니다.

### OTLP Writer 설정

```cpp
#include <kcenon/logger/writers/otlp_writer.h>

kcenon::logger::writers::otlp_writer::config cfg;
cfg.endpoint = "http://localhost:4318/v1/logs";
cfg.protocol = kcenon::logger::writers::otlp_writer::protocol_type::http;
cfg.service_name = "my-service";
cfg.service_version = "1.0.0";
cfg.max_batch_size = 512;
cfg.flush_interval = std::chrono::milliseconds{5000};
cfg.max_retries = 3;
cfg.retry_delay = std::chrono::milliseconds{100};  // 지수 백오프

auto otlp = std::make_unique<kcenon::logger::writers::otlp_writer>(cfg);
logger->add_writer("otlp", std::move(otlp));
```

### Trace Context 전파

```cpp
using namespace kcenon::logger::otlp;

// 현재 스레드에 trace context 설정
otel_context ctx;
ctx.trace_id = "0af7651916cd43dd8448eb211c80319c";
ctx.span_id = "b7ad6b7169203331";
ctx.trace_flags = "01";  // Sampled

// RAII 스코프 - 소멸 시 이전 context 자동 복원
{
    otel_context_scope scope(ctx);
    logger->log(log_level::info, "Processing request");
    // 로그 항목에 trace_id와 span_id 자동 포함
}
```

### 프로토콜 지원

| 프로토콜 | 기본 포트 | 사용 사례 |
|----------|----------|----------|
| `http` | 4318 | 표준 OTLP/HTTP, 방화벽 친화적 |
| `grpc` | 4317 | 고처리량, 스트리밍 |

### 기능

- **W3C Trace Context**: trace_id, span_id, trace_flags 전파
- **배치 내보내기**: 구성 가능한 배치 크기 및 플러시 간격
- **지수 백오프 재시도**: 내보내기 실패 시 자동 재시도
- **리소스 속성**: 서비스명, 버전, 네임스페이스, 커스텀 속성
- **이중 프로토콜**: HTTP 및 gRPC 지원
- **TLS 지원**: 프로덕션 배포를 위한 보안 전송

---

## 로그 샘플링

중요한 메시지를 보존하면서 볼륨을 제어하는 적응형 로그 샘플링입니다.

### 샘플링 전략

| 전략 | 설명 | 적합한 용도 |
|------|------|-------------|
| `random` | 비율 기반 확률적 샘플링 | 일반 볼륨 감소 |
| `rate_limiting` | 시간 윈도우당 N개 로그 | 엄격한 비율 제어 |
| `adaptive` | 볼륨 기반 비율 자동 조절 | 가변 트래픽 패턴 |
| `hash_based` | 메시지 내용 기반 결정적 샘플링 | 재현 가능한 샘플링 |

### 빠른 설정

```cpp
using namespace kcenon::logger::sampling;

// 일반적인 설정을 위한 팩토리 메서드
auto sampler = sampler_factory::create_random(0.1);          // 10% 샘플링
auto sampler = sampler_factory::create_rate_limited(1000);   // 초당 최대 1000개
auto sampler = sampler_factory::create_adaptive(10000, 0.01); // 10K/s에서 적응
auto sampler = sampler_factory::create_production(0.1,
    {log_level::error, log_level::critical});                // 프로덕션 프리셋

// 로거와 통합
logger->set_sampler(std::move(sampler));
```

### 설정

```cpp
sampling_config config;
config.enabled = true;
config.rate = 0.1;  // 10% 기본 비율
config.strategy = sampling_strategy::adaptive;

// 중요 메시지는 항상 로깅
config.always_log_levels = {log_level::error, log_level::critical};

// 카테고리별 비율 오버라이드
config.category_rates = {
    {"security", 1.0},    // 보안 이벤트는 항상 로깅
    {"debug", 0.01},      // 디버그 메시지의 1%
};

// 적응형 설정
config.adaptive_enabled = true;
config.adaptive_threshold = 10000;  // 10K msgs/sec에서 트리거
config.adaptive_min_rate = 0.01;    // 최소 1%

auto sampler = std::make_unique<log_sampler>(config);
logger->set_sampler(std::move(sampler));
```

### 기능

- **항상 로깅 레벨**: 중요 메시지는 샘플링을 완전히 우회
- **카테고리별 비율**: 로그 카테고리별 다른 샘플링 비율
- **적응형 스로틀링**: 고부하 시 자동 비율 감소
- **런타임 재설정**: 재시작 없이 샘플링 설정 변경
- **고성능**: 빠른 xorshift64 PRNG, FNV-1a 해싱

---

## 실시간 로그 분석

실시간 이상 감지 및 패턴 기반 알림 시스템입니다.

### 이상 유형

| 유형 | 설명 | 트리거 |
|------|------|--------|
| `error_spike` | 갑작스러운 오류 증가 | 윈도우당 오류 수 임계값 초과 |
| `pattern_match` | Regex 패턴 감지 | 로그 메시지가 설정된 패턴과 일치 |
| `rate_anomaly` | 비정상 로그 비율 | 비율이 기준선에서 크게 벗어남 |
| `new_error_type` | 이전에 없던 오류 | 처음 보는 오류 메시지 |

### 사용법

```cpp
#include <kcenon/logger/analysis/realtime_log_analyzer.h>

// 프로덕션 기본값으로 생성
auto analyzer = kcenon::logger::analysis::realtime_analyzer_factory::create_production(
    50,  // 에러 스파이크: 분당 50개 오류
    [](const kcenon::logger::analysis::anomaly_event& event) {
        std::cerr << "이상 감지: " << event.description << std::endl;
    }
);

// 패턴 알림 추가
analyzer->add_pattern_alert("OutOfMemory|OOM", log_level::error);

// 로거와 통합
logger->set_realtime_analyzer(std::move(analyzer));
```

---

## 참고사항

### 스레드 안전성

- **로거**: 스레드 안전 (내부 동기화)
- **작성자**: 대부분 스레드 안전 (구현에 따름)
- **포맷터**: 상태 없음, 스레드 안전

### 주의사항

- 로거 생성은 비용이 있음 - 한 번 생성 후 재사용
- 비동기 로거는 종료 시 flush() 호출 필요
- 로그 레벨을 낮게 설정하면 성능 저하 가능

---

**최종 업데이트**: 2026-02-08
**버전**: 0.4.0

---

Made with ❤️ by 🍀☀🌕🌥 🌊
