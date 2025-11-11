# Log Level Semantic Standard

**Date**: 2025-11-08
**Status**: Approved for Implementation
**Affects**: logger_system, thread_system

---

## Problem Statement

Currently, there is an inconsistency in log level semantics between logger_system and thread_system:

### logger_system (Ascending: Low → High)
```cpp
enum class log_level {
    trace = 0,     // Most verbose
    debug = 1,
    info = 2,
    warn = 3,
    error = 4,
    fatal = 5      // Least verbose, most critical
};

// Comparison: msg_level >= min_level
// Example: To log errors and above, set min_level = error (4)
```

### thread_system (Descending: High → Low)
```cpp
enum class log_level {
    critical = 0,  // Most critical, least verbose
    error = 1,
    warning = 2,
    info = 3,
    debug = 4,
    trace = 5      // Least critical, most verbose
};

// Comparison: msg_level <= max_level
// Example: To log errors and above, set max_level = error (1)
```

### Impact

1. **Confusing API**: Different comparison logic for same semantic meaning
2. **Integration Complexity**: Requires constant value mapping in adapters
3. **Error-Prone**: Easy to use wrong threshold in cross-system code
4. **Maintenance Burden**: Duplicate documentation and examples

---

## Decision

**We adopt the ASCENDING semantic (Low → High) as the standard across all systems.**

```cpp
// Standard Log Level Enumeration
enum class log_level {
    trace = 0,     // Most verbose, least important
    debug = 1,
    info = 2,
    warn = 3,
    error = 4,
    fatal = 5,     // Least verbose, most important
    off = 6        // Logging disabled
};
```

### Rationale

1. **Industry Standard**: Most logging frameworks use ascending order
   - syslog: DEBUG(7) < ERROR(3)
   - Python logging: DEBUG(10) < ERROR(40)
   - Java log4j: DEBUG < ERROR
   - spdlog (C++): trace(0) < critical(6)

2. **Natural Comparison**: Higher number = Higher severity
   ```cpp
   if (message_level >= configured_min_level) {
       log(message);
   }
   ```

3. **Existing Adoption**: logger_system already uses this semantic
   - Less breaking change overall
   - Common patterns already established

4. **Consistent with Syslog RFC 5424**: Standard severity levels

---

## Migration Plan

### Phase 1: Documentation (Sprint 1 - Current)
- [x] Write this standard document
- [ ] Update logger_system documentation to emphasize standard
- [ ] Create thread_system migration guide

### Phase 2: thread_system Unification (Sprint 2)
- [ ] Update thread_system `log_level` enum to ascending order
- [ ] Update all comparison logic (>= instead of <=)
- [ ] Add compatibility layer for deprecation period
- [ ] Update thread_system tests

### Phase 3: Adapter Simplification (Sprint 3)
- [ ] Remove value mapping from log level adapters
- [ ] Direct type alias: `using log_level = common::log_level;`
- [ ] Verify all integration tests pass

---

## Implementation Details

### Standard Definition (logger_system - already compliant)

```cpp
// include/kcenon/logger/types/logger_types.h
namespace kcenon::logger {
    enum class log_level : uint8_t {
        trace = 0,
        debug = 1,
        info = 2,
        warn = 3,
        error = 4,
        fatal = 5,
        off = 6
    };

    // Standard comparison for filtering
    constexpr bool should_log(log_level message_level, log_level min_level) {
        return static_cast<uint8_t>(message_level) >= static_cast<uint8_t>(min_level);
    }

    // String conversion
    constexpr std::string_view to_string(log_level level) {
        switch (level) {
            case log_level::trace: return "TRACE";
            case log_level::debug: return "DEBUG";
            case log_level::info:  return "INFO";
            case log_level::warn:  return "WARN";
            case log_level::error: return "ERROR";
            case log_level::fatal: return "FATAL";
            case log_level::off:   return "OFF";
            default: return "UNKNOWN";
        }
    }
}
```

### thread_system Changes (Sprint 2)

```cpp
// include/kcenon/thread/core/logging.h (UPDATED)
namespace kcenon::thread {
    // OLD (Deprecated)
    enum class log_level_legacy {
        critical = 0,
        error = 1,
        warning = 2,
        info = 3,
        debug = 4,
        trace = 5
    };

    // NEW (Standard)
    enum class log_level {
        trace = 0,
        debug = 1,
        info = 2,
        warn = 3,
        error = 4,
        critical = 5,  // Renamed from 'fatal' to match existing naming
        off = 6
    };

    // Compatibility helper (temporary)
    [[deprecated("Use log_level instead. Will be removed in next major version")]]
    constexpr log_level from_legacy(log_level_legacy old_level) {
        return static_cast<log_level>(5 - static_cast<int>(old_level));
    }
}
```

### Adapter Simplification (Sprint 3)

```cpp
// Before (complex mapping)
namespace adapters {
    log_level to_logger_level(thread::log_level thread_level) {
        // Requires mapping: critical(0) -> fatal(5), etc.
        return static_cast<log_level>(5 - static_cast<int>(thread_level));
    }
}

// After (direct usage)
namespace kcenon::thread {
    using log_level = logger::log_level;  // Type alias, no conversion needed
}
```

---

## Compatibility Considerations

### Breaking Change Assessment

- **logger_system**: No breaking changes (already compliant)
- **thread_system**: **BREAKING CHANGE** in next major version
  - All `log_level` enum values change
  - Comparison operators reversed
  - Serialized log level values incompatible

### Mitigation Strategy

1. **Deprecation Period**: Minimum 6 months
2. **Dual Support**: Both old and new enums available during transition
3. **Compile-Time Warnings**: Mark old enum as `[[deprecated]]`
4. **Migration Tool**: Automated code scanner to find usages
5. **Documentation**: Clear migration guide with examples

### User Impact

**Low Impact** - Most users interact via helper functions, not raw enums:
```cpp
// Common usage pattern (unaffected by enum ordering)
logger->info("Message");      // Not: logger->log(log_level::info, "Message");
logger->set_level("ERROR");   // Not: logger->set_level(log_level::error);
```

---

## Testing Requirements

### Unit Tests
- [ ] Verify log level comparisons
- [ ] Test `should_log()` function correctness
- [ ] Validate string conversions

### Integration Tests
- [ ] logger_system ↔ thread_system level mapping
- [ ] Serialization/deserialization consistency
- [ ] Configuration file parsing

### Regression Tests
- [ ] Ensure existing behavior unchanged in logger_system
- [ ] Verify thread_system compatibility layer works

---

## Documentation Updates Required

1. **README.md** (both systems)
   - Add log level semantic explanation
   - Link to this standard document

2. **API Reference**
   - Update log level enum documentation
   - Clarify comparison semantics

3. **Migration Guide** (thread_system)
   - Step-by-step upgrade instructions
   - Code examples before/after
   - FAQ for common issues

4. **Examples**
   - Update all code samples to use new semantic
   - Add migration example

---

## Success Criteria

- [ ] Single log level semantic across all systems
- [ ] Zero value mapping overhead in adapters
- [ ] All tests passing with new semantic
- [ ] Documentation updated and reviewed
- [ ] No regression in existing logger_system behavior
- [ ] Smooth migration path for thread_system users

---

## Timeline

| Phase | Duration | Tasks | Status |
|-------|----------|-------|--------|
| **Phase 1** | Week 1-2 | Document standard, update logger_system docs | ✅ In Progress |
| **Phase 2** | Week 3-4 | Implement thread_system changes, add deprecation | Pending |
| **Phase 3** | Week 5-6 | Simplify adapters, integration testing | Pending |
| **Deprecation** | 6 months | Support both old and new enums | Pending |
| **Cleanup** | Next major | Remove deprecated enums and code | Pending |

---

## References

- [RFC 5424: The Syslog Protocol](https://tools.ietf.org/html/rfc5424) - Section 6.2.1 Severity
- [spdlog Logging Levels](https://github.com/gabime/spdlog) - Industry standard C++ logger
- [logger_system IMPROVEMENT_PLAN.md](../IMPROVEMENT_PLAN.md#1-dual-log-level-semantics)
- [thread_system IMPROVEMENT_PLAN.md](../../thread_system/IMPROVEMENT_PLAN.md#5-log-level-enumeration-mismatch)

---

## Approval

**Lead Architect**: [Approved - 2025-11-08]
**logger_system Owner**: [Approved - 2025-11-08]
**thread_system Owner**: [Pending Coordination]

---

**Next Review**: 2025-12-08 (after Sprint 2 implementation)
