> **Language:** [English](README.md) | **한국어**

# Logger System 문서

고성능, 스레드 안전한 C++20 로깅 프레임워크인 Logger System의 종합 문서에 오신 것을 환영합니다.

## 📚 문서 구조

### 핵심 문서
- **[README.md](../README.md)** - 프로젝트 개요, 기능 및 빠른 시작
- **[API_REFERENCE.md](API_REFERENCE.md)** ([한국어](API_REFERENCE_KO.md)) - 예제가 포함된 완전한 API 문서
- **[ARCHITECTURE.md](ARCHITECTURE.md)** ([한국어](ARCHITECTURE_KO.md)) - 스레딩 생태계 아키텍처
- **[LOGGER_SYSTEM_ARCHITECTURE.md](LOGGER_SYSTEM_ARCHITECTURE.md)** ([한국어](LOGGER_SYSTEM_ARCHITECTURE_KO.md)) - 상세한 시스템 아키텍처
- **[CHANGELOG.md](CHANGELOG.md)** ([한국어](CHANGELOG_KO.md)) - 버전 히스토리 및 릴리스 노트
- **[CONTRIBUTING.md](CONTRIBUTING.md)** ([한국어](CONTRIBUTING_KO.md)) - 기여 가이드라인
- **[SECURITY.md](SECURITY.md)** ([한국어](SECURITY_KO.md)) - 보안 고려사항 및 보고

### 가이드 (`docs/guides/`)
시작하기 및 모범 사례를 위한 필수 가이드:

- **[GETTING_STARTED.md](guides/GETTING_STARTED.md)** ([한국어](guides/GETTING_STARTED_KO.md)) - 단계별 설정 및 기본 사용법
- **[BEST_PRACTICES.md](guides/BEST_PRACTICES.md)** ([한국어](guides/BEST_PRACTICES_KO.md)) - 프로덕션 준비 패턴 및 권장사항
- **[MIGRATION_GUIDE.md](guides/MIGRATION_GUIDE.md)** ([한국어](guides/MIGRATION_GUIDE_KO.md)) - 이전 버전에서 마이그레이션
- **[PERFORMANCE.md](guides/PERFORMANCE.md)** ([한국어](guides/PERFORMANCE_KO.md)) - 성능 분석 및 최적화

### 고급 주제 (`docs/advanced/`)
고급 사용자 및 기여자를 위한 심층 문서:

- **[CUSTOM_WRITERS.md](advanced/CUSTOM_WRITERS.md)** ([한국어](advanced/CUSTOM_WRITERS_KO.md)) - 커스텀 로그 작성기 생성
- **[CI_CD_DASHBOARD.md](advanced/CI_CD_DASHBOARD.md)** ([한국어](advanced/CI_CD_DASHBOARD_KO.md)) - 빌드 및 지속적 통합

## 🚀 빠른 탐색

### 신규 사용자
1. 프로젝트 개요를 위해 **[README.md](../README.md)**부터 시작하세요
2. 설정을 위해 **[GETTING_STARTED.md](guides/GETTING_STARTED.md)** ([한국어](guides/GETTING_STARTED_KO.md))를 따라하세요
3. 상세한 API 사용법은 **[API_REFERENCE.md](API_REFERENCE.md)** ([한국어](API_REFERENCE_KO.md))를 참조하세요

### 기존 사용자
1. 최근 변경사항은 **[CHANGELOG.md](CHANGELOG.md)** ([한국어](CHANGELOG_KO.md))를 확인하세요
2. 업그레이드 시 **[MIGRATION_GUIDE.md](guides/MIGRATION_GUIDE.md)** ([한국어](guides/MIGRATION_GUIDE_KO.md))를 사용하세요
3. 최적 사용법은 **[BEST_PRACTICES.md](guides/BEST_PRACTICES.md)** ([한국어](guides/BEST_PRACTICES_KO.md))를 참조하세요

### 기여자
1. 가이드라인을 위해 **[CONTRIBUTING.md](CONTRIBUTING.md)** ([한국어](CONTRIBUTING_KO.md))를 읽으세요
2. 구현 세부사항은 **[LOGGER_SYSTEM_ARCHITECTURE.md](LOGGER_SYSTEM_ARCHITECTURE.md)** ([한국어](LOGGER_SYSTEM_ARCHITECTURE_KO.md))를 학습하세요
3. 확장 패턴은 **[CUSTOM_WRITERS.md](advanced/CUSTOM_WRITERS.md)** ([한국어](advanced/CUSTOM_WRITERS_KO.md))를 탐색하세요

### 성능 엔지니어
1. 벤치마크는 **[PERFORMANCE.md](guides/PERFORMANCE.md)** ([한국어](guides/PERFORMANCE_KO.md))를 검토하세요
2. 설계 결정은 **[LOGGER_SYSTEM_ARCHITECTURE.md](LOGGER_SYSTEM_ARCHITECTURE.md)** ([한국어](LOGGER_SYSTEM_ARCHITECTURE_KO.md))를 확인하세요
3. 최적화 팁은 **[BEST_PRACTICES.md](guides/BEST_PRACTICES.md)** ([한국어](guides/BEST_PRACTICES_KO.md))를 참조하세요

## 🔧 문서 유지관리

이 문서 구조는 다음을 위해 재구성되었습니다:
- **중복 제거**: 각 주제에 대한 단일 진실 공급원
- **탐색 개선**: 명확한 분류 및 논리적 흐름
- **사용성 향상**: 역할 기반 진입점 및 상호 참조

### 최근 변경사항
- 여러 API 참조를 단일 권위 문서로 통합
- 아키텍처 문서 분리 (생태계 vs. 시스템별)
- 가이드를 논리적 카테고리로 재구성
- 상호 참조 및 탐색 개선

## 📖 외부 리소스

- **[GitHub 저장소](https://github.com/kcenon/logger_system)** - 소스 코드 및 이슈
- **[Thread System](https://github.com/kcenon/thread_system)** - 기본 인터페이스
- **[통합 예제](https://github.com/kcenon/integrated_thread_system)** - 완전한 사용 패턴
- **[생성된 문서](https://kcenon.github.io/logger_system/)** - Doxygen API 문서 (GitHub Pages)

## 🤝 도움 및 지원

- **이슈**: [GitHub Issues](https://github.com/kcenon/logger_system/issues)에서 버그 보고 및 기능 요청
- **토론**: [GitHub Discussions](https://github.com/kcenon/logger_system/discussions)에서 유지보수 지원 및 질문
- **보안**: **[SECURITY.md](SECURITY.md)** ([한국어](SECURITY_KO.md)) 가이드라인에 따라 보안 이슈 보고

---

*문서 최종 업데이트: [현재 날짜] - Logger System v2.x*

---

*Last Updated: 2025-10-20*
