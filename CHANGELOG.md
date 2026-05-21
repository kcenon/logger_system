# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Documentation

- Add "Project Layout" section to `README.md` and `README.kr.md` linking to the kcenon ecosystem layout standard ([#629](https://github.com/kcenon/logger_system/issues/629))

## [1.0.0] - 2026-04-15

### Changed

- **BREAKING**: Remove 8 deprecated context methods from `logger` public API; use `context()` unified API instead ([#534](https://github.com/kcenon/logger_system/issues/534))
- Fix CMake exported target name from `logger_system::logger` to `logger_system::logger_system` in config template ([#606](https://github.com/kcenon/logger_system/issues/606))
- Align `find_package()` and `target_link_libraries()` documentation with actual CMake target names ([#606](https://github.com/kcenon/logger_system/issues/606))

### Added

- Unit tests for `logger_registry`, `console_writer`, and `standalone_executor` (46 new test cases) ([#607](https://github.com/kcenon/logger_system/issues/607))

### Performance

- Remove unused `sequence_` array from `lockfree_spsc_queue` ([#533](https://github.com/kcenon/logger_system/issues/533))
- Eliminate string copies in `high_performance_async_writer` hot path ([#532](https://github.com/kcenon/logger_system/issues/532))

## [0.1.3] - 2026-03-14

### Fixed
- CMake: add thread_system::ThreadSystem to unified target map

## [0.1.2] - 2026-03-14

### Fixed
- CMake: use configured GIT_TAG instead of hardcoded 'main' in find_package fallback (#492)
- Build: add DLL export macros, fix CMake install/export and include paths (#487)
- CMake: use lowercase thread_system as find_package name (#486)

### Documentation
- Replace GIT_TAG main with tagged versions in FetchContent examples (#491)
- Fix Doxyfile portability and improve comment coverage (#484)

## [0.1.1] - 2026-03-11

### Fixed
- CMake: use lowercase common_system as find_package name in UnifiedDependencies (#481)

## [0.1.0] - 2026-03-10

### Added
- High-performance async logging with 4.34M messages/second throughput
- Decorator pattern composition (console, file, rotating, encrypted writers)
- OpenTelemetry (OTLP) integration for observability
- AES-256-GCM encrypted log writer
- Structured logging with key-value pairs
- Sampling strategies (rate-based, priority-based)
- Security features (log signing, tamper detection)
- C++20 module support
- Dependabot and OSV-Scanner vulnerability monitoring (#472)
- SBOM generation and CVE scanning workflows (#466)
- IEC 62304 SOUP register (#467)

### Infrastructure
- GitHub Actions CI/CD with sanitizer testing
- Doxygen documentation workflow
- vcpkg manifest with optional features (encryption, otlp, benchmarks)
- Cross-platform support (Linux, macOS, Windows)
