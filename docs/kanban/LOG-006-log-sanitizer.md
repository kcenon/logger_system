# LOG-006: Implement Log Sanitizer

## Metadata

| Field | Value |
|-------|-------|
| **ID** | LOG-006 |
| **Priority** | 🔴 HIGH |
| **Est. Duration** | 2-3 days |
| **Dependencies** | None |
| **Status** | TODO |
| **Assignee** | TBD |
| **Created** | 2025-11-23 |

---

## 1. Overview

### 1.1 Problem Statement

The example code `security_demo.cpp` references a `log_sanitizer` class that **does not exist**:

```cpp
// security_demo.cpp line 34
// #include <kcenon/logger/security/log_sanitizer.h>  // TODO: Not implemented yet
```

The function `demonstrate_log_sanitization()` (lines 69-95) demonstrates functionality for a non-existent class.

### 1.2 Objectives

- Create `log_sanitizer` class
- Support common sensitive data patterns (CC, SSN, API keys, passwords, etc.)
- Configurable sanitization rules
- Performance-optimized pattern matching

### 1.3 Scope

- Create `include/kcenon/logger/security/log_sanitizer.h`
- Create `src/security/log_sanitizer.cpp`
- Integration with logger and formatters

---

## 2. Changes

### 2.1 Sanitizer Interface

```cpp
// log_sanitizer.h
namespace kcenon::logger::security {

enum class sensitive_data_type {
    credit_card,
    ssn,               // Social Security Number
    api_key,
    password,
    email,
    ip_address,
    phone_number,
    custom
};

struct sanitization_rule {
    std::string name;
    std::regex pattern;
    std::string replacement;  // e.g., "[REDACTED]", "****1234"
    bool preserve_partial;    // Keep last 4 digits for CC, etc.
};

class log_sanitizer {
public:
    // Add built-in patterns
    log_sanitizer& add_pattern(sensitive_data_type type);

    // Add custom pattern
    log_sanitizer& add_custom_pattern(
        std::string_view name,
        std::string_view regex_pattern,
        std::string_view replacement = "[REDACTED]",
        bool preserve_partial = false);

    // Remove pattern
    log_sanitizer& remove_pattern(std::string_view name);

    // Sanitize string
    std::string sanitize(std::string_view input) const;

    // Check if string contains sensitive data
    bool contains_sensitive_data(std::string_view input) const;

    // Get list of active patterns
    std::vector<std::string> active_patterns() const;
};

} // namespace kcenon::logger::security
```

### 2.2 Built-in Patterns

| Type | Pattern Example | Replacement |
|------|-----------------|-------------|
| Credit Card | `4111-1111-1111-1111` | `****-****-****-1111` |
| SSN | `123-45-6789` | `***-**-6789` |
| API Key | `sk-...`, `api_...` | `[API_KEY_REDACTED]` |
| Password | `password=...` | `password=[REDACTED]` |
| Email | `user@domain.com` | `u***@domain.com` |
| IP Address | `192.168.1.100` | `192.168.x.x` |
| Phone | `+1-555-123-4567` | `+1-555-***-****` |

### 2.3 Integration Points

```cpp
// Option 1: Filter-based integration
class sanitizing_filter : public log_filter_interface {
    log_sanitizer sanitizer_;
public:
    filter_result filter(log_entry& entry) override {
        entry.message = sanitizer_.sanitize(entry.message);
        return filter_result::pass;
    }
};

// Option 2: Formatter integration
class sanitizing_formatter : public log_formatter_interface {
    log_sanitizer sanitizer_;
public:
    std::string format(const log_entry& entry) override {
        auto sanitized_entry = entry;
        sanitized_entry.message = sanitizer_.sanitize(entry.message);
        return base_formatter_->format(sanitized_entry);
    }
};

// Option 3: Direct logger integration
logger_builder& with_sanitizer(log_sanitizer sanitizer);
```

---

## 3. Test Plan

### 3.1 Unit Tests

#### sanitizer_patterns_test.cpp (8)
- [ ] Credit card detection and masking
- [ ] SSN detection and masking
- [ ] API key detection and redaction
- [ ] Password field detection
- [ ] Email partial masking
- [ ] IP address masking
- [ ] Phone number masking
- [ ] Multiple patterns in single string

#### sanitizer_config_test.cpp (5)
- [ ] Add custom pattern
- [ ] Remove pattern
- [ ] Preserve partial data option
- [ ] Custom replacement text
- [ ] Pattern priority/ordering

#### sanitizer_integration_test.cpp (4)
- [ ] Filter-based integration
- [ ] Formatter-based integration
- [ ] Builder integration
- [ ] Thread safety verification

### 3.2 Performance Tests

- [ ] Pattern matching performance (1000 patterns)
- [ ] Long string sanitization performance
- [ ] Overhead comparison: with/without sanitizer

---

## 4. Checklist

### 4.1 Implementation Checklist

- [ ] Create `log_sanitizer.h`
- [ ] Create `log_sanitizer.cpp`
- [ ] Implement `add_pattern()` for built-in types
- [ ] Implement `add_custom_pattern()`
- [ ] Implement `sanitize()` method
- [ ] Implement `contains_sensitive_data()` method
- [ ] Create `sanitizing_filter` class
- [ ] Create `sanitizing_formatter` class
- [ ] Add `with_sanitizer()` to logger_builder
- [ ] Implement all 7 built-in patterns

### 4.2 Test Checklist

- [ ] Write 8+ pattern tests
- [ ] Write 5+ config tests
- [ ] Write 4+ integration tests
- [ ] Write performance tests
- [ ] All tests passing

### 4.3 Documentation Checklist

- [ ] Document log_sanitizer API
- [ ] Document built-in patterns
- [ ] Document custom pattern creation
- [ ] Fix security_demo.cpp to work
- [ ] Add security best practices guide
- [ ] Update CHANGELOG

---

## 5. Acceptance Criteria

1. ✅ `log_sanitizer` class exists and works
2. ✅ All 7 built-in patterns implemented
3. ✅ Custom pattern support works
4. ✅ Partial preservation option works
5. ✅ Integration with logger (at least one method)
6. ✅ `security_demo.cpp` example works
7. ✅ Performance overhead < 5% for typical log messages
8. ✅ Test coverage 90%+ (sanitizer code)

---

## 6. Related Files

- Create: `include/kcenon/logger/security/log_sanitizer.h`
- Create: `src/security/log_sanitizer.cpp`
- Fix: `examples/security_demo.cpp` (lines 34, 69-95)
- Integrate with: `include/kcenon/logger/core/logger_builder.h`

---

## 7. References

- [OWASP Logging Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Logging_Cheat_Sheet.html)
- [PCI DSS logging requirements](https://www.pcisecuritystandards.org/)
- [GDPR data masking guidelines](https://gdpr.eu/)
