# LOG-005: Complete Structured Logger Implementation

## Metadata

| Field | Value |
|-------|-------|
| **ID** | LOG-005 |
| **Priority** | 🔴 HIGH |
| **Est. Duration** | 2-3 days |
| **Dependencies** | None |
| **Status** | TODO |
| **Assignee** | TBD |
| **Created** | 2025-11-23 |

---

## 1. Overview

### 1.1 Problem Statement

The `structured_logger.h` file contains a **placeholder implementation**. The `basic_structured_logger::log_structured()` method is a no-op:

```cpp
// structured_logger.h lines 136-140
void log_structured(const structured_log_entry& entry) override {
    // Implementation would format and output the structured log
    // For now, this is a placeholder
    (void)entry;
}
```

The example code (`security_demo.cpp` line 35) references this non-functional feature.

### 1.2 Objectives

- Implement actual structured log output
- Support JSON format output
- Integrate with existing writers
- Enable type-safe field access

### 1.3 Scope

- `include/kcenon/logger/structured/structured_logger.h`
- `include/kcenon/logger/structured/structured_log_entry.h`
- JSON formatter integration
- Writer output integration

---

## 2. Changes

### 2.1 Complete structured_log_entry

```cpp
class structured_log_entry {
public:
    // Builder pattern for structured entries
    structured_log_entry& with_field(std::string_view key, std::string_view value);
    structured_log_entry& with_field(std::string_view key, int64_t value);
    structured_log_entry& with_field(std::string_view key, double value);
    structured_log_entry& with_field(std::string_view key, bool value);

    // Nested objects
    structured_log_entry& with_object(std::string_view key, const structured_log_entry& obj);

    // Arrays
    structured_log_entry& with_array(std::string_view key, const std::vector<std::string>& values);

    // Standard fields
    structured_log_entry& with_timestamp();
    structured_log_entry& with_level(log_level level);
    structured_log_entry& with_message(std::string_view msg);

    // Serialization
    std::string to_json() const;
    std::string to_logfmt() const;  // key=value format
};
```

### 2.2 Implement log_structured()

```cpp
void basic_structured_logger::log_structured(const structured_log_entry& entry) override {
    // Format based on configured output format
    std::string formatted;
    switch (output_format_) {
        case structured_format::json:
            formatted = entry.to_json();
            break;
        case structured_format::logfmt:
            formatted = entry.to_logfmt();
            break;
    }

    // Output to underlying logger
    if (logger_) {
        logger_->log(log_level::info, formatted);
    }
}
```

### 2.3 JSON Output Format

```json
{
    "timestamp": "2025-11-23T10:30:00.123Z",
    "level": "INFO",
    "message": "User logged in",
    "user_id": "12345",
    "ip_address": "192.168.1.1",
    "session_id": "abc123",
    "metadata": {
        "browser": "Chrome",
        "os": "macOS"
    }
}
```

---

## 3. Test Plan

### 3.1 Unit Tests

#### structured_entry_test.cpp (7)
- [ ] String field addition
- [ ] Numeric field addition
- [ ] Boolean field addition
- [ ] Nested object creation
- [ ] Array field addition
- [ ] JSON serialization correctness
- [ ] Logfmt serialization correctness

#### structured_logger_test.cpp (5)
- [ ] Basic log_structured() output
- [ ] JSON format output verification
- [ ] Logfmt format output verification
- [ ] Integration with logger instance
- [ ] Field type preservation

### 3.2 Integration Tests

- [ ] Structured logger → File writer integration
- [ ] Structured logger → Console output
- [ ] Thread safety with concurrent structured logs
- [ ] Performance comparison: structured vs plain logging

---

## 4. Checklist

### 4.1 Implementation Checklist

- [ ] Complete `structured_log_entry` field methods
- [ ] Implement `to_json()` serialization
- [ ] Implement `to_logfmt()` serialization
- [ ] Implement `basic_structured_logger::log_structured()`
- [ ] Add format selection (json/logfmt)
- [ ] Integrate with existing logger/writers
- [ ] Handle special characters and escaping
- [ ] Support Unicode in field values

### 4.2 Test Checklist

- [ ] Write 7+ entry tests
- [ ] Write 5+ logger tests
- [ ] Write integration tests
- [ ] All tests passing

### 4.3 Documentation Checklist

- [ ] Document structured_log_entry API
- [ ] Document structured_logger usage
- [ ] Add JSON output examples
- [ ] Update security_demo.cpp to work
- [ ] Update CHANGELOG

---

## 5. Acceptance Criteria

1. ✅ `log_structured()` produces actual output
2. ✅ JSON format is valid and parseable
3. ✅ Logfmt format follows standard conventions
4. ✅ All field types (string, int, double, bool, object, array) work
5. ✅ Special characters properly escaped
6. ✅ Example in security_demo.cpp works
7. ✅ Test coverage 90%+ (structured logging code)

---

## 6. Related Files

- `include/kcenon/logger/structured/structured_logger.h` (placeholder implementation)
- `include/kcenon/logger/structured/structured_log_entry.h`
- `examples/security_demo.cpp` line 35 (references non-functional feature)

---

## 7. References

- [Logfmt specification](https://brandur.org/logfmt)
- [JSON logging best practices](https://www.datadoghq.com/blog/log-collection-json/)
