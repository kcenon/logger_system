# Test & CI Inventory

Factual re-verification of the test and CI surface of `logger_system`,
recorded for audit traceability (issue #641). Captured from branch `develop`
via `git ls-files` and direct file inspection.

## Test build wiring

The root `CMakeLists.txt` delegates test configuration to
`cmake/testing.cmake` (`include(testing)`). When `BUILD_TESTS` (or
`LOGGER_BUILD_TESTS`) is set, that module calls `enable_testing()` and adds
two subdirectories:

- `tests/` — unit, component, compatibility, and benchmark targets
  (`tests/CMakeLists.txt`).
- `integration_tests/` — cross-system scenario / failure / performance tests
  (`integration_tests/CMakeLists.txt`).

## Test sources (tracked counts)

- Tracked `.cpp` under `tests/`: 67 files.
- Tracked `.cpp` under `integration_tests/`: 6 files.

### `tests/` layout

- Top-level component tests: `min_level_filter_test.cpp`,
  `thread_safety_test.cpp`, `log_sampling_test.cpp`,
  `routing_integration_test.cpp`, `otlp_test.cpp`,
  `encrypted_writer_test.cpp`, `structured_logging_test.cpp`,
  `realtime_analyzer_test.cpp`.
- `tests/integration/`: `thread_system_integration_test.cpp`,
  `monitoring_integration_test.cpp`.
- `tests/compatibility/`: `version_compatibility_test.cpp`.
- `tests/unit/` (and its subdirectories `logger_test/`, `writers_test/`,
  `collectors_test/`, `config_test/`, `di_test/`, `monitoring_test/`,
  `safety_test/`, `health_test/`, `flow_test/`, `stress_test/`,
  `core_test/`, `security_test/`, `server_test/`, `analysis_test/`,
  `formatters_test/`, `filters_test/`, `utils_test/`, `interfaces_test/`,
  `integration_test/`): the bulk of unit coverage, wired through
  `tests/CMakeLists.txt` and `tests/unit/CMakeLists.txt`.
- `tests/benchmarks/`: Google Benchmark targets (separate from correctness
  tests).

Most targets in `tests/CMakeLists.txt` are guarded by
`if(EXISTS ...)` and tagged with the issue number that introduced them
(e.g. #441, #442, #566, #607), and each is registered for coverage via
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

12 tracked workflow files (plus `WORKFLOW_UPDATES.md`):

| Workflow file                   | `name:`                          | Triggers | Schedule |
|---------------------------------|----------------------------------|----------|----------|
| `ci.yml`                        | CI                               | push, pull_request | — |
| `coverage.yml`                  | Code Coverage                    | push, pull_request, workflow_dispatch | — |
| `static-analysis.yml`           | Static Analysis                  | push, pull_request, workflow_dispatch | — |
| `sanitizers.yml`                | Sanitizer Tests                  | push, pull_request, workflow_dispatch | — |
| `integration-tests.yml`         | Integration Tests                | push, pull_request, workflow_dispatch | — |
| `benchmarks.yml`                | Performance Benchmarks           | push, pull_request, workflow_dispatch, schedule | weekly (`0 0 * * 0`) |
| `performance-regression.yml`    | Performance Regression Detection | pull_request, workflow_dispatch | — |
| `cve-scan.yml`                  | CVE Vulnerability Scan           | push, pull_request, schedule, workflow_dispatch | daily (`0 2 * * *`) |
| `osv-scanner.yml`               | OSV Vulnerability Scanner        | push, pull_request, schedule, workflow_dispatch | weekly (`0 8 * * 1`) |
| `sbom.yml`                      | SBOM Generation                  | push, release, workflow_dispatch | — |
| `doc-audit.yml`                 | Documentation Audit              | pull_request (docs/md paths), workflow_dispatch | — |
| `on-release-sync-registry.yml`  | Sync to Central Registry         | release, workflow_dispatch | — |

## Coverage observations (factual)

- Before this change there was no dedicated fuzzing workflow. Scheduled
  workflows already present: `cve-scan.yml` (daily), `osv-scanner.yml`
  (weekly), and `benchmarks.yml` (weekly). Security scanning was therefore
  dependency-focused (CVE/OSV/SBOM); no in-process input fuzzing existed.
- Sanitizer coverage (`sanitizers.yml`) runs ASan/TSan/UBSan over the test
  suite but is driven by the existing fixed test inputs, not by fuzz-generated
  inputs.

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

The harness targets the public `log_entry` interface header
(`include/kcenon/logger/interfaces/log_entry.h`), which transitively includes
`kcenon/common/interfaces/logger_interface.h` from the `common_system`
dependency. That dependency is fetched at configure time and is not vendored
in this checkout, so the harness cannot be compiled locally here; the
`fuzzing.yml` workflow checks out dependencies and performs the first real
build.
