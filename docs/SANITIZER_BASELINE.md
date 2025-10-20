> **Language:** **English** | [한국어](SANITIZER_BASELINE_KO.md)

# logger_system Sanitizer Baseline

**Phase**: 0 - Foundation and Tooling
**Task**: 0.1 - CI/CD Pipeline Enhancement
**Date Created**: 2025-10-07
**Status**: Baseline Establishment

---

## Executive Summary

This document records the baseline sanitizer warnings for logger_system. During Phase 0, sanitizer tests are configured and running, but warnings are allowed to establish a baseline. Phase 1 will address thread safety issues identified by sanitizers.

---

## Sanitizer Configuration

### Enabled Sanitizers

1. **ThreadSanitizer (TSan)**
   - Detects data races and thread safety issues
   - Flags: `-fsanitize=thread`
   - Environment: `TSAN_OPTIONS=halt_on_error=0 second_deadlock_stack=1`

2. **AddressSanitizer (ASan)**
   - Detects memory errors (use-after-free, buffer overflow, etc.)
   - Flags: `-fsanitize=address`
   - Environment: `ASAN_OPTIONS=halt_on_error=0 detect_leaks=1`

3. **UndefinedBehaviorSanitizer (UBSan)**
   - Detects undefined behavior
   - Flags: `-fsanitize=undefined`
   - Environment: `UBSAN_OPTIONS=halt_on_error=0 print_stacktrace=1`

---

## Baseline Warnings

### ThreadSanitizer Warnings

**Status**: ⏳ To be measured

| Component | Warning Count | Severity | Phase 1 Target |
|-----------|---------------|----------|----------------|
| rotating_file_writer | TBD | TBD | 0 warnings |
| async_writer | TBD | TBD | 0 warnings |
| file_writer | TBD | TBD | 0 warnings |
| console_writer | TBD | TBD | 0 warnings |
| logger | TBD | TBD | 0 warnings |

**Common Issues Expected**:
- Unprotected access to rotation state
- Concurrent file handle access
- Async queue operations without proper synchronization

### AddressSanitizer Warnings

**Status**: ⏳ To be measured

| Component | Warning Count | Severity | Phase 2 Target |
|-----------|---------------|----------|----------------|
| Memory allocations | TBD | TBD | 0 warnings |
| String operations | TBD | TBD | 0 warnings |
| Buffer operations | TBD | TBD | 0 warnings |

**Common Issues Expected**:
- Potential memory leaks in writer cleanup
- Buffer overruns in message formatting
- Use-after-free in async operations

### UndefinedBehaviorSanitizer Warnings

**Status**: ⏳ To be measured

| Component | Warning Count | Severity | Phase 1 Target |
|-----------|---------------|----------|----------------|
| Type conversions | TBD | TBD | 0 warnings |
| Arithmetic operations | TBD | TBD | 0 warnings |
| Pointer operations | TBD | TBD | 0 warnings |

---

## How to Run Sanitizers Locally

### ThreadSanitizer

```bash
cd logger_system
cmake -B build -S . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=thread -fno-omit-frame-pointer -g" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=thread" \
  -DLOGGER_BUILD_TESTS=ON

cmake --build build
cd build && ctest --output-on-failure
```

### AddressSanitizer

```bash
cd logger_system
cmake -B build -S . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address" \
  -DLOGGER_BUILD_TESTS=ON

cmake --build build
cd build && ctest --output-on-failure
```

### UndefinedBehaviorSanitizer

```bash
cd logger_system
cmake -B build -S . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=undefined -fno-omit-frame-pointer -g" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=undefined" \
  -DLOGGER_BUILD_TESTS=ON

cmake --build build
cd build && ctest --output-on-failure
```

---

## CI/CD Integration

Sanitizers run automatically on:
- Every push to main/phase-* branches
- Every pull request
- Manual workflow dispatch

See `.github/workflows/sanitizers.yml` for configuration.

---

## Phase 1 Action Items

Based on baseline measurements, Phase 1 will address:

1. **Thread Safety** (High Priority)
   - Add mutexes to protect shared state in rotating_file_writer
   - Implement thread-safe async queue operations
   - Add synchronization to file handle access

2. **Memory Management** (Medium Priority)
   - Fix memory leaks in writer cleanup paths
   - Add proper buffer size checks
   - Ensure exception-safe resource management

3. **Undefined Behavior** (Low Priority)
   - Fix any type conversion issues
   - Address arithmetic overflow concerns
   - Resolve pointer aliasing problems

---

## Exemptions

Currently no exemptions. All sanitizer warnings will be addressed in Phase 1-2.

---

**Last Updated**: 2025-10-07
**Next Review**: After first sanitizer CI run
**Owner**: Development Team
