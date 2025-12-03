# 변경 이력 - Logger System

> **언어:** [English](CHANGELOG.md) | **한국어**

Logger System 프로젝트의 모든 주요 변경 사항이 이 파일에 문서화됩니다.

형식은 [Keep a Changelog](https://keepachangelog.com/ko/1.0.0/)를 따르며,
이 프로젝트는 [Semantic Versioning](https://semver.org/lang/ko/spec/v2.0.0.html)을 준수합니다.

---

## [Unreleased]

### CMake fmt Fallback 제거 - 2025-12-03

#### 변경됨
- **CMake 설정에서 std::format 필수**: 모든 fmt 라이브러리 fallback 로직 제거
  - `cmake/CompilerChecks.cmake`: std::format을 사용할 수 없으면 FATAL_ERROR 발생
  - `cmake/LoggerCompatibility.cmake`: fmt 탐지, C++17 모드, vcpkg fallback 제거

#### 제거됨
- **CMake의 fmt 라이브러리 fallback**:
  - `check_std_format()`: find_package(fmt) 및 vcpkg fallback 로직 제거
  - `configure_logger_compatibility()`: C++17 호환 모드 완전 제거
  - `setup_formatting_library()`: fmt::format 및 basic formatting 분기 제거
  - `setup_vcpkg_fmt_fallback()`: 함수 완전 제거

#### 이점
- **깔끔한 CMake 설정**: fallback 복잡성 없이 단순화된 기능 감지
- **일관된 빌드 동작**: 모든 플랫폼과 CI 환경에서 동일한 동작
- **의존성 감소**: 조건부 fmt 라이브러리 링킹 없음
- **프로젝트 간 정렬**: thread_system, container_system, network_system, database_system과 일치

#### 관련 이슈
- 프로젝트 간 표준화 작업의 일부
- 참고: thread_system#219, container_system#168, network_system#257, database_system#203

---

## [1.0.0] - 2025-10-22

### 추가됨
- **핵심 Logger System**: 프로덕션 준비 완료된 C++20 비동기 로깅 프레임워크
  - 논블로킹 로그 작업을 통한 비동기 처리
  - 다중 출력 대상 (콘솔, 파일, 로테이팅 파일, 네트워크, 암호화, 하이브리드)
  - 동시 로깅을 지원하는 스레드 안전 작업
  - 효율적인 메시지 전달을 위한 제로카피 설계

- **Critical Writer**: 즉각적인 기록이 필요한 중요 메시지를 위한 동기식 로깅
  - 비정상 종료 시 로그 보존을 위한 긴급 플러시 메커니즘
  - 자동 버퍼 플러싱을 통한 크래시 안전 로깅

- **고급 아키텍처**:
  - ILogger, IMonitor, IMonitorable 추상화를 통한 인터페이스 기반 설계
  - 플러그 가능한 writer, filter, formatter를 갖춘 모듈화된 컴포넌트
  - 인터페이스 전용 의존성을 통한 순환 의존성 제로
  - 생태계 의존성 없이 독립적인 컴파일

- **Builder Pattern API**: 유창하고 타입 안전한 logger 구축
  - 구성 전략 (템플릿 기반, 환경 인식, 성능 튜닝)
  - Result<T> 오류 처리를 통한 포괄적인 유효성 검사
  - 선택적 런타임 컴포넌트 주입을 위한 의존성 주입

- **CMake 빌드 시스템**: 포괄적인 빌드 구성
  - 유연한 빌드를 위한 15개 이상의 기능 플래그
  - 자동 의존성 감지
  - 크로스 플랫폼 지원 (Windows, Linux, macOS)
  - 컴파일러 지원 (GCC, Clang, MSVC)

- **Writer 구현**:
  - `console_writer`: 표준 출력/오류 스트림 로깅
  - `file_writer`: 기본 파일 출력
  - `rotating_file_writer`: 크기/시간별 자동 로그 로테이션
  - `network_writer`: 네트워크를 통한 원격 로깅
  - `critical_writer`: 동기식 중요 메시지 로깅
  - `hybrid_writer`: 다중 writer 결합 지원

- **Formatter 구현**:
  - `text_formatter`: 일반 텍스트 형식
  - `json_formatter`: 구조화된 JSON 로깅
  - `xml_formatter`: 레거시 시스템을 위한 XML 형식
  - IFormatter 인터페이스를 통한 커스텀 formatter 지원

- **Filter 구현**:
  - 로그 레벨 필터링
  - 패턴 기반 메시지 필터링
  - IFilter 인터페이스를 통한 커스텀 필터 지원

- **통합 지원**:
  - common_system 통합 (ILogger, IMonitor, Result<T> 패턴)
  - thread_system 통합 (스레딩 프리미티브, logger_interface)
  - monitoring_system 통합 (IMonitor를 통한 메트릭 및 헬스 모니터링)

- **성능 기능**:
  - 조정 가능한 배치 크기 및 큐 깊이를 통한 구성 가능한 배칭
  - 핫 패스에서의 락프리 작업
  - 효율적인 메모리 관리를 통한 최소 할당
  - 독립 컴파일 시 약 15초의 빌드 시간

### 변경됨
- 해당 없음 (최초 릴리스)

### 사용 중단됨
- 해당 없음 (최초 릴리스)

### 제거됨
- 해당 없음 (최초 릴리스)

### 수정됨
- 해당 없음 (최초 릴리스)

### 보안
- 스레드 안전 작업으로 데이터 경쟁 없는 동시 로깅 보장
- 민감한 로그 데이터를 위한 암호화 writer 지원

---

## 프로젝트 정보

**저장소:** https://github.com/kcenon/logger_system
**문서:** [docs/](docs/)
**라이선스:** LICENSE 파일 참조
**관리자:** kcenon@naver.com

---

## 버전 지원 매트릭스

| 버전  | 릴리스 날짜 | C++ 표준 | CMake 최소 버전 | 상태   |
|-------|-------------|----------|----------------|--------|
| 1.0.0 | 2025-10-22  | C++20    | 3.16           | 현재   |

---

## 마이그레이션 가이드

통합 thread_system 또는 독립형 logger 구현에서 마이그레이션하려면 [MIGRATION.md](MIGRATION.md)를 참조하십시오.

---

**최종 업데이트:** 2025-10-22
