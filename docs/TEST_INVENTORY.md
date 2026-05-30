# Test & CI Inventory

Factual re-verification of the test and CI surface of `logger_system`,
recorded for audit traceability (issue #641). Captured from branch `develop`.

## Test sources

### Wired into the top-level CMake build

The top-level `CMakeLists.txt` adds `tests/` only when `BUILD_TESTS=ON`
(default `ON`). Within `tests/`, the following targets are defined:

| Target                    | Source                                         | Framework |
|---------------------------|------------------------------------------------|-----------|
| `test_basic_logging`      | `tests/unit/test_basic_logging.cpp`            | GoogleTest |
| `logger_integration_test` | `tests/integration/logger_integration_test.cpp`| GoogleTest |

`test_basic_logging` covers the public `log_entry` struct and the `log_level`
enum values. `logger_integration_test` links `logger_impl` and exercises the
same public interface headers.

### Present in the tree but NOT built

These `.cpp` files are tracked but are not referenced by any `CMakeLists.txt`,
so they are not compiled or run by the default build:

- `tests/integration/cross_system_integration_test.cpp`
- `tests/integration/end_to_end_pipeline_test.cpp`
- `integration_tests/cross_system_test.cpp`
- `integration_tests/messaging_integration_test.cpp`
- `integration_tests/phase4_integration_test.cpp`

The top-level `integration_tests/` directory is not added by the root
`CMakeLists.txt`; its `CMakeLists.txt` is a stub. The two unbuilt files under
`tests/integration/` are also not listed in `tests/integration/CMakeLists.txt`,
which wires `logger_integration_test` only.

## CI workflows (`.github/workflows/`)

| Workflow file              | `name:`                     | Triggers |
|----------------------------|-----------------------------|----------|
| `ci.yml`                   | CI                          | push, pull_request, workflow_dispatch |
| `build-Linux.yml`          | Build-Linux                 | push, pull_request, workflow_dispatch |
| `build-Linux-Arm.yml`      | Build-Linux-Arm             | push, pull_request, workflow_dispatch |
| `build-macOS.yml`          | Build-macOS                 | push, pull_request, workflow_dispatch |
| `build-Windows-MSYS2.yml`  | Build-Windows-MSYS2         | push, pull_request, workflow_dispatch |
| `build-Windows-VS.yml`     | Build-Windows-VS            | push, pull_request, workflow_dispatch |
| `build-Doxygen.yml`        | Build Doxygen Documentation | push, pull_request, workflow_dispatch |
| `sanitizers.yml`           | Sanitizers                  | push, pull_request, workflow_dispatch |
| `coverage.yml`             | Code Coverage               | push, pull_request, workflow_dispatch |
| `codeql.yml`               | CodeQL Security Analysis    | push, pull_request, schedule, workflow_dispatch |
| `static-analysis.yml`      | Static Analysis             | push, pull_request, workflow_dispatch |
| `dependency-graph.yml`     | Dependency Graph            | push, pull_request, workflow_dispatch |
| `docs.yml`                 | (docs)                      | push, pull_request, workflow_dispatch |
| `auto-merge.yml`           | Auto-merge Dependabot PRs   | pull_request_target |

Total tracked workflow files: 14 (plus `.github/workflows/README.md`).

## Coverage observations (factual)

- Before this change, no fuzzing workflow existed; `codeql.yml` was the only
  security-oriented scheduled workflow.
- The default test build exercises only the public interface headers
  (`log_entry`, `log_level`). Internal sink/formatter/structured sources
  referenced by `src/impl/logger.cpp` are not part of this module's tracked
  headers in this checkout.

## Added by this change (issue #641)

- `fuzz/log_entry_fuzzer.cpp` — libFuzzer harness over the `log_entry`
  parse/serialize surface, gated behind `BUILD_FUZZERS` (Clang +
  `-fsanitize=fuzzer,address`); does not affect the default build.
- `fuzz/CMakeLists.txt`, `fuzz/corpus/` seeds, `fuzz/README.md`.
- `.github/workflows/fuzzing.yml` — weekly `schedule` + `workflow_dispatch`.
