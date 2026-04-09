---
doc_id: "LOG-API-002c"
doc_title: "API Reference - Security"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "API"
---

> **Language:** **English** | [한국어](API_REFERENCE.kr.md)

# API Reference - Security

> **Split from**: [API_REFERENCE.md](API_REFERENCE.md)

**Version**: 0.5.0.0
**Last Updated**: 2026-02-08

## Table of Contents
- [Overview](#overview)
- [Security Error Codes](#security-error-codes)
- [Encrypted Writer](#encrypted-writer)
- [Secure Key Storage](#secure-key-storage)
- [Security-Critical Logger Interface](#security-critical-logger-interface)
- [Log Sanitization](#log-sanitization)
- [Path Validation](#path-validation)
- [Security Filters](#security-filters)
- [Audit Logging](#audit-logging)

## Overview

The logger_system security module provides encryption, sanitization, path validation, and audit logging capabilities for production environments with compliance requirements (PCI-DSS, HIPAA, SOC 2, GDPR).

**Headers**: Located under `include/kcenon/logger/security/`

---

## Security Error Codes

Security-related errors use codes in the 1700-1799 range:

```cpp
enum class logger_error_code {
    // Security errors (1700-1799)
    encryption_failed = 1700,
    decryption_failed = 1701,
    authentication_failed = 1702,
    sanitization_failed = 1703,
    file_read_failed = 1704,
    insecure_permissions = 1705,
    path_traversal_detected = 1706,
    invalid_key_size = 1707,
    invalid_filename = 1708,
};
```

| Code | Name | Description |
|------|------|-------------|
| 1700 | `encryption_failed` | AES-256 encryption operation failed |
| 1701 | `decryption_failed` | Decryption failed (wrong key or corrupted data) |
| 1702 | `authentication_failed` | Authentication check failed |
| 1703 | `sanitization_failed` | Log message sanitization error |
| 1704 | `file_read_failed` | Failed to read a security-related file |
| 1705 | `insecure_permissions` | File permissions are too permissive |
| 1706 | `path_traversal_detected` | Path traversal attempt detected in filename |
| 1707 | `invalid_key_size` | Encryption key size does not meet requirements |
| 1708 | `invalid_filename` | Filename contains invalid or dangerous characters |

---

## Encrypted Writer

The encrypted writer decorator provides AES-256 encryption for log data at rest. Requires OpenSSL.

### Writer Builder Integration

```cpp
#include <kcenon/logger/builders/writer_builder.h>

using namespace kcenon::logger;

// Generate encryption key
auto key_result = security::secure_key_storage::generate_key(32);

// Build encrypted file writer
auto secure_writer = writer_builder()
    .file("secure.log.enc")
    .encrypted(std::move(key_result.value()))
    .buffered()
    .async()
    .build();
```

### writer_builder::encrypted()

```cpp
// Encryption decorator (AES-256, requires OpenSSL)
writer_builder& encrypted(security::secure_key key);
```

**Parameters**:
- `key` -- A `security::secure_key` object (must be 32 bytes for AES-256)

**Requires**: `LOGGER_USE_ENCRYPTION=ON` CMake option and OpenSSL

**Error**: Returns `logger_error_code::invalid_key_size` if key is not 32 bytes

---

## Secure Key Storage

**Header**: `<kcenon/logger/security/secure_key_storage.h>`

### Key Generation

```cpp
namespace kcenon::logger::security {

class secure_key_storage {
public:
    // Generate a cryptographically secure random key
    static result<secure_key> generate_key(std::size_t key_size_bytes);

    // Load key from secure storage
    static result<secure_key> load_key(const std::string& key_path);

    // Save key to secure storage (with restrictive permissions)
    static common::VoidResult save_key(const secure_key& key,
                                       const std::string& key_path);
};

} // namespace kcenon::logger::security
```

### Usage Example

```cpp
#include <kcenon/logger/security/secure_key_storage.h>

using namespace kcenon::logger::security;

// Generate new key
auto key_result = secure_key_storage::generate_key(32);  // AES-256
if (!key_result) {
    std::cerr << "Key generation failed: " << key_result.error_message() << "\n";
    return 1;
}

// Save to file with restrictive permissions (0600)
auto save_result = secure_key_storage::save_key(key_result.value(), "/etc/app/log.key");

// Load existing key
auto loaded_key = secure_key_storage::load_key("/etc/app/log.key");
```

---

## Security-Critical Logger Interface

**Header**: `<kcenon/logger/security/critical_logger_interface.h>`

The main logger class implements `security::critical_logger_interface` for crash-safe, signal-safe logging:

```cpp
namespace kcenon::logger::security {

class critical_logger_interface {
public:
    virtual ~critical_logger_interface() = default;

    // Get file descriptor for emergency writing (signal-safe)
    virtual int get_emergency_fd() const = 0;

    // Get emergency buffer (pre-allocated, signal-safe)
    virtual const char* get_emergency_buffer() const = 0;
    virtual size_t get_emergency_buffer_size() const = 0;
};

} // namespace kcenon::logger::security
```

These methods are designed to be called from signal handlers and use only async-signal-safe operations.

---

## Log Sanitization

Log sanitization prevents sensitive data from appearing in log output.

### Common Sanitization Patterns

```cpp
// Sanitize credit card numbers
auto sanitized = sanitizer::mask_credit_card(log_message);
// "Card: 4111111111111111" -> "Card: 4111********1111"

// Sanitize email addresses
auto sanitized = sanitizer::mask_email(log_message);
// "User: user@example.com" -> "User: u***@example.com"

// Sanitize custom patterns
auto sanitized = sanitizer::mask_pattern(log_message, regex_pattern, replacement);
```

### Integration with Filters

```cpp
// Add sanitization as a filter in the logging pipeline
auto logger = logger_builder::create()
    .add_function_filter([](const log_entry& entry) {
        // Reject entries containing raw PII
        return !contains_pii(entry.message);
    })
    .build();
```

**Error Code**: `sanitization_failed` (1703) -- returned when sanitization logic encounters an error

---

## Path Validation

Path validation prevents directory traversal attacks in log file paths.

### Validation Checks

The logger validates all file paths to prevent:

1. **Path traversal** (`../`) -- attempting to write outside the log directory
2. **Absolute path injection** -- overriding the configured log directory
3. **Null byte injection** -- truncating paths with embedded null characters
4. **Invalid characters** -- characters not allowed in filenames

### Error Codes

- `path_traversal_detected` (1706) -- directory traversal attempt detected
- `invalid_filename` (1708) -- filename contains invalid characters
- `insecure_permissions` (1705) -- file permissions are too permissive

### Example

```cpp
// Path validation happens automatically when creating file writers
auto writer = writer_builder()
    .file("/var/log/app/../../etc/passwd")  // Rejected: path traversal
    .build();
// Returns error with code path_traversal_detected

// Safe usage
auto writer = writer_builder()
    .file("/var/log/app/application.log")  // Accepted
    .build();
```

---

## Security Filters

Security-focused filters for compliance requirements.

### Level-Based Security Filtering

```cpp
// Ensure security-related logs are never dropped
auto security_filter = std::make_unique<filters::function_filter>(
    [](const log_entry& entry) {
        // Always log security events regardless of level
        if (entry.context.count("category") &&
            entry.context.at("category") == "security") {
            return true;
        }
        // Apply normal level filtering for non-security events
        return entry.level >= log_level::warning;
    }
);
```

### Sampling Bypass for Security Events

```cpp
#include <kcenon/logger/sampling/log_sampler.h>

using namespace kcenon::logger::sampling;

// Configure sampling to always log security events
sampling_config config = sampling_config::random_sampling(0.1);
config.category_rates["security"] = 1.0;   // 100% of security logs
config.always_log_fields = {"security_event_id", "audit_trail_id"};
```

---

## Audit Logging

Audit logging provides tamper-evident, compliance-ready log records.

### Best Practices for Audit Logs

1. **Use structured logging** for machine-parseable audit trails
2. **Include context** (user ID, session ID, action, timestamp)
3. **Never drop audit logs** -- use sampling bypass or critical_writer
4. **Encrypt at rest** -- use the encrypted writer decorator
5. **Rotate and archive** -- maintain retention policy compliance

### Example: Audit Trail

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/builders/writer_builder.h>

using namespace kcenon::logger;

// Create dedicated audit logger with encryption
auto audit_key = security::secure_key_storage::load_key("/etc/app/audit.key").value();

auto audit_writer = writer_builder()
    .file("/var/log/audit/audit.log.enc")
    .encrypted(std::move(audit_key))
    .buffered(50, std::chrono::milliseconds(1000))
    .async()
    .build();

auto audit_logger = logger_builder::create()
    .with_async(true)
    .add_writer("audit", std::move(audit_writer))
    .with_structured_logging(true)
    .build()
    .value();

// Log audit events
audit_logger->log_structured(log_level::info)
    .message("User data access")
    .field("user_id", 12345)
    .field("action", "read")
    .field("resource", "patient_record")
    .field("record_id", "PR-2025-001")
    .category("audit")
    .emit();
```

---

## Related Documentation

- [API Reference - Core](./API_REFERENCE_CORE.md) -- Logger, builder, context, configuration
- [API Reference - Writers](./API_REFERENCE_WRITERS.md) -- All writer classes, filters, formatters
- [Security Guide](./SECURITY_GUIDE.md) -- Comprehensive security guide

---

*Last Updated: 2026-02-08*
