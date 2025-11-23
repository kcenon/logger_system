# LOG-004: Reactivate Builder Strategy Pattern

## Metadata

| Field | Value |
|-------|-------|
| **ID** | LOG-004 |
| **Priority** | 🟡 MEDIUM |
| **Est. Duration** | 2-3 days |
| **Dependencies** | LOG-001 |
| **Status** | TODO |
| **Assignee** | TBD |
| **Created** | 2025-11-23 |

---

## 1. Overview

### 1.1 Problem Statement

Logger builder has extensive strategy pattern and configuration template code that is **completely commented out**:

- **Line 69**: `config_strategy_interface` include commented
- **Lines 422-454**: Strategy methods (`apply_strategy`, `for_environment`, `with_performance_tuning`, `auto_configure`) commented
- **Lines 456-482**: Configuration template methods commented
- **Lines 562-575**: DI container integration commented
- **Lines 649-655**: `clear_strategies()` commented
- **Lines 690-706**: Strategy application in `build()` commented
- **Line 813**: `strategies_` vector commented

### 1.2 Objectives

- Implement `config_strategy_interface`
- Re-enable all strategy-related methods
- Support environment-based auto-configuration
- Enable configuration templates (development, production, performance)

### 1.3 Scope

- `include/kcenon/logger/core/logger_builder.h`
- `include/kcenon/logger/config/config_strategy_interface.h` (may need creation)
- Environment detection and template application

---

## 2. Changes

### 2.1 Strategy Interface

```cpp
// config_strategy_interface.h
class config_strategy_interface {
public:
    virtual ~config_strategy_interface() = default;

    // Apply strategy to builder
    virtual void configure(logger_builder& builder) = 0;

    // Strategy name for identification
    virtual std::string_view name() const = 0;

    // Priority (higher = applied first)
    virtual int priority() const { return 0; }
};
```

### 2.2 Built-in Strategies

```cpp
// Environment strategies
class development_strategy : public config_strategy_interface;
class production_strategy : public config_strategy_interface;
class testing_strategy : public config_strategy_interface;

// Performance strategies
class high_throughput_strategy : public config_strategy_interface;
class low_latency_strategy : public config_strategy_interface;
class memory_efficient_strategy : public config_strategy_interface;
```

### 2.3 Configuration Templates

```cpp
enum class configuration_template {
    development,    // Debug level, console + file, verbose format
    production,     // Info level, file + async, minimal format
    performance,    // Warning level, async only, bare minimum format
    testing,        // Debug level, in-memory buffer
    silent          // No logging (for benchmarks)
};
```

### 2.4 Methods to Uncomment/Implement

```cpp
class logger_builder {
public:
    // Strategy application
    logger_builder& apply_strategy(std::unique_ptr<config_strategy_interface> strategy);

    // Environment-based configuration
    logger_builder& for_environment(std::string_view env_name);

    // Performance tuning presets
    logger_builder& with_performance_tuning(performance_preset preset);

    // Auto-configure from environment variables
    logger_builder& auto_configure();

    // Apply configuration template
    logger_builder& with_template(configuration_template tpl);

    // Clear all strategies
    logger_builder& clear_strategies();
};
```

---

## 3. Test Plan

### 3.1 Unit Tests

#### strategy_test.cpp (existing, needs update)
- [ ] Development strategy applies correct settings
- [ ] Production strategy applies correct settings
- [ ] Testing strategy applies correct settings
- [ ] Strategy priority ordering
- [ ] Strategy clear functionality

#### template_test.cpp (5)
- [ ] Development template test
- [ ] Production template test
- [ ] Performance template test
- [ ] Testing template test
- [ ] Custom template override test

#### environment_test.cpp (5)
- [ ] `LOG_ENV` variable detection
- [ ] `LOG_LEVEL` variable override
- [ ] Unknown environment fallback
- [ ] `auto_configure()` full flow
- [ ] Environment + template combination

### 3.2 Integration Tests

- [ ] Builder → Strategy → Logger creation flow
- [ ] Multiple strategies combination
- [ ] Environment variable integration

---

## 4. Checklist

### 4.1 Implementation Checklist

- [ ] Create/complete `config_strategy_interface.h`
- [ ] Implement `development_strategy`
- [ ] Implement `production_strategy`
- [ ] Implement `testing_strategy`
- [ ] Implement performance preset strategies
- [ ] Uncomment `apply_strategy()` method
- [ ] Uncomment `for_environment()` method
- [ ] Uncomment `with_performance_tuning()` method
- [ ] Uncomment `auto_configure()` method
- [ ] Implement `configuration_template` enum
- [ ] Uncomment template methods
- [ ] Uncomment `clear_strategies()` method
- [ ] Uncomment `strategies_` vector
- [ ] Uncomment strategy application in `build()`

### 4.2 Test Checklist

- [ ] Update existing strategy_test.cpp
- [ ] Write template tests
- [ ] Write environment tests
- [ ] All tests passing

### 4.3 Documentation Checklist

- [ ] Document strategy interface
- [ ] Document built-in strategies
- [ ] Document configuration templates
- [ ] Add usage examples
- [ ] Update CHANGELOG

---

## 5. Acceptance Criteria

1. ✅ All strategy methods uncommented and functional
2. ✅ At least 3 environment strategies implemented
3. ✅ At least 5 configuration templates working
4. ✅ `auto_configure()` reads environment variables
5. ✅ Strategy priority ordering correct
6. ✅ Test coverage 85%+ (strategy code)

---

## 6. Related Files

- `include/kcenon/logger/core/logger_builder.h` (lines 69, 422-454, 456-482, 562-575, 649-655, 690-706, 813)
- `include/kcenon/logger/config/config_strategy_interface.h` (to create)
- `tests/unit/config_test/strategy_test.cpp` (exists, needs update)

---

## 7. References

- [LOG-001](LOG-001-filtering-system.md) - Filtering system (prerequisite for some strategies)
- [ARCHITECTURE.md](../ARCHITECTURE.md) - Strategy pattern design
