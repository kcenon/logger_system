# Test & CI Inventory

Factual re-verification of the test and CI surface of `logger_system`,
recorded for audit traceability (issue #641). Captured from the
`test/issue-641-fuzz-harness` branch via `git ls-files` and direct file
inspection.

## Test build wiring

The root `CMakeLists.txt` delegates test configuration to
`cmake/testing.cmake` (`include(testing)`). When `BUILD_TESTS` is set, that
module calls `enable_testing()` and adds the `tests/` subdirectory, and
(when integration tests are enabled) the `integration_tests/` subdirectory.

- `tests/` — unit, component, compatibility, and benchmark targets
  (`tests/CMakeLists.txt`, `tests/unit/CMakeLists.txt`).
- `integration_tests/` — cross-system scenario / failure / performance tests
  (`integration_tests/CMakeLists.txt`).

## Test sources (tracked counts)

- Tracked `.cpp` under `tests/`: 65 files (includes correctness tests and
  `tests/benchmarks/` Google Benchmark sources).
- Tracked `.cpp` under `integration_tests/`: 6 files.

### `tests/` layout

- Top-level component tests: `min_level_filter_test.cpp`,
  `thread_safety_test.cpp`, `log_sampling_test.cpp`,
  `routing_integration_test.cpp`, `otlp_test.cpp`,
  `encrypted_writer_test.cpp`, `structured_logging_test.cpp`,
  `realtime_analyzer_test.cpp`.
- `tests/integration/`: `thread_system_integration_test.cpp`,
  `monitoring_integration_test.cpp` (wired by `if(EXISTS ...)` guards in
  `tests/CMakeLists.txt`).
- `tests/compatibility/`: `version_compatibility_test.cpp`.
- `tests/unit/` and its subdirectories (`logger_test/`, `writers_test/`,
  `collectors_test/`, `config_test/`, `di_test/`, `monitoring_test/`,
  `safety_test/`, `health_test/`, `flow_test/`, `stress_test/`,
  `core_test/`, `security_test/`, `server_test/`, `analysis_test/`,
  `formatters_test/`, `filters_test/`, `utils_test/`, `interfaces_test/`,
  `integration_test/`): the bulk of unit coverage.
- `tests/benchmarks/`: Google Benchmark targets (separate from correctness
  tests).

Most targets in `tests/CMakeLists.txt` are guarded by `if(EXISTS ...)` and
tagged with the issue number that introduced them (e.g. #441, #442, #566,
#607), and each is registered for coverage via
`logger_register_coverage_target`.

### `integration_tests/` layout

`integration_tests/CMakeLists.txt` builds a single `logger_integration_tests`
executable by GLOB-ing `scenarios/*.cpp`, `failures/*.cpp`, and
`performance/*.cpp`. Tracked sources:

- `scenarios/logger_lifecycle_test.cpp`,
  `scenarios/multi_writer_integration_test.cpp`,
  `scenarios/log_formatting_integration_test.cpp`,
  `scenarios/thread_system_integration_test.cpp`
- `failures/error_handling_test.cpp`
- `performance/logger_performance_test.cpp`

Under thread sanitizer this target is skipped by design (heavy file I/O is
impractical under TSan; unit tests cover race detection).

## CI workflows (`.github/workflows/`)

13 tracked workflow files (`.yml`), plus the non-workflow files
`build-Doxygen.yaml` and `WORKFLOW_UPDATES.md`. Names, triggers, and cron
schedules below are read directly from each file.

| Workflow file                   | `name:`                  | Triggers | Schedule (cron, UTC) |
|---------------------------------|--------------------------|----------|----------------------|
| `ci.yml`                        | CI                       | push, pull_request | — |
| `coverage.yml`                  | Code Coverage            | push, pull_request | — |
| `static-analysis.yml`           | Static Analysis          | push, pull_request | — |
| `sanitizers.yml`                | Sanitizer Tests          | push, pull_request, workflow_dispatch | — |
| `integration-tests.yml`         | Integration Tests        | push, pull_request, workflow_dispatch | — |
| `benchmarks.yml`                | Benchmarks               | push, pull_request, workflow_dispatch, schedule | `0 2 * * *` (daily) |
| `performance-regression.yml`    | Performance Regression   | pull_request | — |
| `cve-scan.yml`                  | CVE Security Scan        | push, pull_request, schedule, workflow_dispatch | `0 2 * * *` (daily) |
| `osv-scanner.yml`               | OSV Vulnerability Scan   | push, pull_request, schedule, workflow_dispatch | `17 3 * * 0` (weekly, Sun) |
| `sbom.yml`                      | SBOM Generation          | push, pull_request, release, schedule, workflow_dispatch | `0 3 * * 0` (weekly, Sun) |
| `doc-audit.yml`                 | Documentation Audit      | pull_request (docs/README/CLAUDE paths) | — |
| `on-release-sync-registry.yml`  | Sync Registry on Release | release (published) | — |
| `fuzzing.yml` (added by #641)   | Fuzzing                  | schedule, workflow_dispatch | `17 6 * * 1` (weekly, Mon) |

## Coverage observations (factual)

- Before this change there was no dedicated fuzzing workflow. Scheduled
  workflows already present: `benchmarks.yml` (daily), `cve-scan.yml`
  (daily), `osv-scanner.yml` (weekly), and `sbom.yml` (weekly). Security
  scanning was therefore dependency-focused (CVE/OSV/SBOM); no in-process
  input fuzzing existed.
- Sanitizer coverage (`sanitizers.yml`) runs ASan/TSan/UBSan over the test
  suite but is driven by the existing fixed test inputs, not by
  fuzz-generated inputs.

## Added by this change (issue #641)

- `fuzz/log_entry_fuzzer.cpp` — libFuzzer harness over the public
  `kcenon::logger::log_entry` deserialize/serialize surface (field splitting,
  `log_level`/line parsing, optional-field handling, control-char escaping).
- `BUILD_FUZZERS` CMake option + `fuzz/CMakeLists.txt`
  (Clang + `-fsanitize=fuzzer,address`); off by default, does not affect the
  default build.
- `fuzz/corpus/` seed inputs and `fuzz/README.md`.
- `.github/workflows/fuzzing.yml` — weekly `schedule` (`17 6 * * 1`) +
  `workflow_dispatch`, mirroring the existing scheduled-scan workflows.

## Scope note

The harness includes the public interface header
`include/kcenon/logger/interfaces/log_entry.h`, which transitively includes
`kcenon/common/interfaces/logger_interface.h` from the `common_system`
dependency. That dependency is resolved at CMake configure time and is not
vendored in this checkout, so the harness cannot be compiled standalone here;
the `fuzzing.yml` workflow checks out dependencies and performs the first real
build.
