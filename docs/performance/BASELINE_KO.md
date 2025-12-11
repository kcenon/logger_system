# Logger System - 성능 기준 메트릭

**[English](BASELINE.md) | 한국어**

---

**Version**: 0.1.0.0
**Date**: 2025-10-09
**Phase**: Phase 0 - Foundation
**Status**: Baseline Established

---

## 시스템 정보

### 하드웨어 구성
- **CPU**: Apple M1 (ARM64)
- **RAM**: 8 GB
- **Storage**: SSD

### 소프트웨어 구성
- **OS**: macOS 26.1
- **Compiler**: Apple Clang 17.0.0.17000319
- **Build Type**: Release (-O3)
- **C++ Standard**: C++20

---

## 성능 메트릭

### 로깅 Throughput
- **Synchronous Logging**: 850,000 messages/second
- **Asynchronous Logging**: 4,340,000 messages/second
- **Console Output**: 120,000 messages/second
- **File Output**: 2,100,000 messages/second

### Latency (Async Mode)
- **P50**: <0.3 μs
- **P95**: <0.8 μs
- **P99**: <1.5 μs

### 메모리 성능
- **Baseline**: 2.5 MB
- **Buffer (1K messages)**: 5.2 MB
- **Buffer (10K messages)**: 18 MB

---

## 벤치마크 결과

| Mode | Throughput | Latency (P50) | Memory | 최적 사용처 |
|------|------------|---------------|--------|----------|
| Sync Console | 120K msg/s | 8 μs | 2.5 MB | 개발 환경 |
| Sync File | 850K msg/s | 1.2 μs | 2.8 MB | 단순 애플리케이션 |
| Async File | 4.34M msg/s | 0.3 μs | 5 MB | 프로덕션 환경 |
| Async Multi-sink | 3.8M msg/s | 0.4 μs | 7 MB | 엔터프라이즈 환경 |

---

## 주요 특징
- ✅ **4.34M messages/second** async mode에서
- ✅ **Sub-microsecond latency** (P50 < 0.3 μs)
- ✅ **낮은 메모리 사용량** (2.5 MB baseline)
- ✅ **Multiple sink 지원** 최소한의 오버헤드로
- ✅ **Zero-allocation hot path** (memory pooling)

---

## Baseline 검증

### Phase 0 요구사항
- [x] Benchmark 인프라 ✅
- [x] 성능 메트릭 기준선 설정 ✅

### 수락 기준
- [x] Throughput > 3M msg/s ✅ (4.34M)
- [x] Latency < 1 μs (P50) ✅ (0.3 μs)
- [x] Memory < 5 MB ✅ (2.5 MB)

---

**Baseline Established**: 2025-10-09
**Maintainer**: kcenon
