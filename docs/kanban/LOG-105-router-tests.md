# LOG-105: Add Router Integration Tests

## Metadata

| Field | Value |
|-------|-------|
| **ID** | LOG-105 |
| **Priority** | 🔴 HIGH |
| **Est. Duration** | 1-2 days |
| **Dependencies** | LOG-002 |
| **Status** | TODO |
| **Assignee** | TBD |
| **Created** | 2025-11-23 |

---

## 1. Overview

### 1.1 Problem Statement

The `log_router.h` implementation has **zero test coverage**. No test files exist for routing functionality.

### 1.2 Objectives

- Add unit tests for routing rules
- Add unit tests for router_builder
- Add integration tests for logger + router
- Achieve 85%+ coverage for routing code

### 1.3 Scope

- Create `tests/unit/routing_test/` directory
- Test `include/kcenon/logger/routing/log_router.h`

---

## 2. Test Categories

### 2.1 Routing Rule Tests (6)

```cpp
// routing_rule_test.cpp
TEST(RoutingRuleTest, PredicateMatches);
TEST(RoutingRuleTest, PredicateNoMatch);
TEST(RoutingRuleTest, ExclusiveFlag);
TEST(RoutingRuleTest, StopPropagationFlag);
TEST(RoutingRuleTest, MultipleTargetWriters);
TEST(RoutingRuleTest, EmptyTargetWriters);
```

### 2.2 Router Builder Tests (5)

```cpp
// router_builder_test.cpp
TEST(RouterBuilderTest, AddSingleRule);
TEST(RouterBuilderTest, AddMultipleRules);
TEST(RouterBuilderTest, RulePriority);
TEST(RouterBuilderTest, BuildEmptyRouter);
TEST(RouterBuilderTest, ChainedBuilderCalls);
```

### 2.3 Router Tests (7)

```cpp
// log_router_test.cpp
TEST(LogRouterTest, RouteToSingleWriter);
TEST(LogRouterTest, RouteToMultipleWriters);
TEST(LogRouterTest, NoMatchingRoute);
TEST(LogRouterTest, ExclusiveRouteStopsOthers);
TEST(LogRouterTest, StopPropagation);
TEST(LogRouterTest, MultipleMatchingRoutes);
TEST(LogRouterTest, RouteOrderPriority);
```

### 2.4 Integration Tests (4)

```cpp
// router_integration_test.cpp
TEST(RouterIntegrationTest, BuilderWithRouter);
TEST(RouterIntegrationTest, LoggerRoutesToCorrectWriter);
TEST(RouterIntegrationTest, CombinedFilterAndRouter);
TEST(RouterIntegrationTest, ThreadSafeRouting);
```

---

## 3. Checklist

### 3.1 Implementation Checklist

- [ ] Create `tests/unit/routing_test/` directory
- [ ] Create `routing_rule_test.cpp` with 6 tests
- [ ] Create `router_builder_test.cpp` with 5 tests
- [ ] Create `log_router_test.cpp` with 7 tests
- [ ] Create `router_integration_test.cpp` with 4 tests
- [ ] Add tests to CMakeLists.txt
- [ ] Verify all tests pass

### 3.2 Coverage Targets

| Component | Current | Target |
|-----------|---------|--------|
| routing_rule | 0% | 90%+ |
| router_builder | 0% | 85%+ |
| log_router | 0% | 85%+ |

---

## 4. Acceptance Criteria

1. ✅ 22+ total router tests
2. ✅ All router components have unit tests
3. ✅ Integration tests cover builder + router
4. ✅ All tests passing
5. ✅ Router code coverage 85%+
6. ✅ ThreadSanitizer clean

---

## 5. Related Files

- `include/kcenon/logger/routing/log_router.h`
- Create: `tests/unit/routing_test/routing_rule_test.cpp`
- Create: `tests/unit/routing_test/router_builder_test.cpp`
- Create: `tests/unit/routing_test/log_router_test.cpp`
- Create: `tests/unit/routing_test/router_integration_test.cpp`

---

## 6. References

- [LOG-002](LOG-002-routing-system.md) - Routing system implementation
- [TESTING_GUIDE.md](../contributing/TESTING_GUIDE.md)
