# LOG-002: Complete Routing System Integration

## Metadata

| Field | Value |
|-------|-------|
| **ID** | LOG-002 |
| **Priority** | 🔴 HIGH |
| **Est. Duration** | 3-5 days |
| **Dependencies** | LOG-001 |
| **Status** | TODO |
| **Assignee** | TBD |
| **Created** | 2025-11-23 |

---

## 1. Overview

### 1.1 Problem Statement

Log router implementation exists at `routing/log_router.h` with full interface and `router_builder`, but it is **NOT integrated** into `logger_builder` or `logger` class. The include is commented out in example code with `// TODO: Not implemented yet`.

**Current State:**
- `log_router.h` - Complete interface with routing rules, exclusive mode, stop_propagation
- `router_builder` - Complete builder pattern implementation
- **NOT wired** into logger_builder or logger core class

### 1.2 Objectives

- Integrate routing system into logger_builder
- Enable message routing to specific writers based on rules
- Support exclusive routing and propagation control
- Ensure minimal performance impact

### 1.3 Scope

- `include/kcenon/logger/routing/log_router.h`
- `include/kcenon/logger/core/logger_builder.h`
- `include/kcenon/logger/core/logger.h`
- `src/core/logger.cpp`

---

## 2. Changes

### 2.1 API Additions

```cpp
// Methods to add to logger_builder.h
class logger_builder {
public:
    // Add router with builder pattern
    logger_builder& with_router(std::unique_ptr<log_router> router);

    // Add routing rule directly
    logger_builder& add_route(
        const std::string& name,
        std::function<bool(const log_entry&)> predicate,
        const std::vector<std::string>& target_writers,
        bool exclusive = false,
        bool stop_propagation = false);

    // Convenience: route by level
    logger_builder& route_level_to(
        log_level level,
        const std::vector<std::string>& target_writers);

    // Convenience: route by regex pattern
    logger_builder& route_pattern_to(
        const std::string& pattern,
        const std::vector<std::string>& target_writers);
};
```

### 2.2 Routing Logic

```cpp
// In logger.cpp log dispatch
void logger::write_log(const log_entry& entry) {
    if (router_) {
        auto target_writers = router_->route(entry);
        if (!target_writers.empty()) {
            for (const auto& writer_name : target_writers) {
                dispatch_to_writer(writer_name, entry);
            }
            if (router_->should_stop_propagation(entry)) {
                return; // Don't send to default writers
            }
        }
    }
    // Default behavior: send to all writers
    dispatch_to_all_writers(entry);
}
```

### 2.3 Implementation Steps

1. Uncomment router include in logger_builder.h
2. Add `std::unique_ptr<log_router> router_` member
3. Implement `with_router()` method
4. Add convenience routing methods
5. Integrate routing logic in logger.cpp
6. Add routing configuration to `build()` method

---

## 3. Test Plan

### 3.1 Unit Tests (Minimum 10)

#### routing_rules_test.cpp (5)
- [ ] Basic predicate routing test
- [ ] Exclusive routing test
- [ ] Stop propagation test
- [ ] Multiple routes matching test
- [ ] No route matching (default behavior) test

#### builder_routing_test.cpp (5)
- [ ] `with_router()` integration test
- [ ] `route_level_to()` convenience test
- [ ] `route_pattern_to()` convenience test
- [ ] Combined filters and routes test
- [ ] Route order priority test

### 3.2 Integration Tests

- [ ] Builder → Logger → Router → Writers full flow
- [ ] Runtime route modification (if supported)
- [ ] Thread safety verification
- [ ] Performance comparison: with/without routing

---

## 4. Checklist

### 4.1 Implementation Checklist

- [ ] Uncomment `#include <kcenon/logger/routing/log_router.h>`
- [ ] Add router storage member
- [ ] Implement `with_router()` method
- [ ] Implement `add_route()` method
- [ ] Implement `route_level_to()` convenience method
- [ ] Implement `route_pattern_to()` convenience method
- [ ] Integrate routing in `logger::write_log()`
- [ ] Handle stop_propagation correctly
- [ ] Update `build()` to configure router

### 4.2 Test Checklist

- [ ] Write 10+ unit tests
- [ ] Write 4+ integration tests
- [ ] All tests passing
- [ ] ThreadSanitizer clean

### 4.3 Documentation Checklist

- [ ] Update API documentation
- [ ] Add routing examples
- [ ] Update CHANGELOG

---

## 5. Acceptance Criteria

1. ✅ Router configurable from logger_builder
2. ✅ Routing rules applied during log dispatch
3. ✅ Exclusive routing works correctly
4. ✅ Stop propagation works correctly
5. ✅ Performance overhead < 3% (compared to no routing)
6. ✅ Test coverage 90%+ (routing code)
7. ✅ Example code in security_demo.cpp works

---

## 6. Related Files

- `include/kcenon/logger/routing/log_router.h` (existing, complete)
- `include/kcenon/logger/core/logger_builder.h` (needs integration)
- `include/kcenon/logger/core/logger.h` (needs router member)
- `src/core/logger.cpp` (needs routing logic)
- `examples/security_demo.cpp` (references non-functional router)

---

## 7. References

- [LOG-001](LOG-001-filtering-system.md) - Filtering system (prerequisite)
- [ARCHITECTURE.md](../ARCHITECTURE.md) - Routing design
