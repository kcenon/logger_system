# Security Module Guide

**logger_system Security Module** (`include/kcenon/logger/security/`)

The security module provides defense-in-depth capabilities for logging infrastructure, protecting against common security threats while maintaining compliance requirements.

---

## Table of Contents

1. [Overview](#overview)
2. [Threat Model](#threat-model)
3. [Architecture](#architecture)
4. [Audit Logger](#audit-logger)
5. [Log Sanitizer](#log-sanitizer)
6. [Path Validator](#path-validator)
7. [Secure Key Storage](#secure-key-storage)
8. [Signal Manager](#signal-manager)
9. [Configuration Examples](#configuration-examples)
10. [Compliance Considerations](#compliance-considerations)

---

## Overview

### Purpose

The security module implements multiple layers of defense to protect logging infrastructure:

- **Tamper detection** — HMAC-signed audit logs for integrity verification
- **Data privacy** — Automatic PII/sensitive data scrubbing
- **Path security** — Prevention of path traversal and symlink attacks
- **Key management** — Secure storage and retrieval of encryption keys
- **Graceful shutdown** — Signal handling for emergency log flushing

### Components

| Component | Header | Purpose |
|-----------|--------|---------|
| Audit Logger | `audit_logger.h` | Tamper-evident audit logging with HMAC signatures |
| Log Sanitizer | `log_sanitizer.h` | PII/sensitive data detection and masking |
| Path Validator | `path_validator.h` | Path traversal prevention and filename validation |
| Secure Key Storage | `secure_key_storage.h` | Secure key generation, storage, and retrieval |
| Signal Manager | `signal_manager.h` | Signal-safe emergency log flushing |
| Signal Manager Interface | `signal_manager_interface.h` | Abstraction for dependency injection |

---

## Threat Model

### Threats Defended Against

| Threat | Defense Mechanism | Components |
|--------|-------------------|------------|
| **Log tampering** | HMAC-signed audit entries | Audit Logger |
| **Data leakage** | PII detection and masking | Log Sanitizer |
| **Path traversal** | Canonical path validation | Path Validator |
| **Symlink attacks** | Symlink resolution and checking | Path Validator |
| **Insecure key storage** | File permission enforcement (0600) | Secure Key Storage |
| **Data loss on crash** | Signal-safe emergency flush | Signal Manager |
| **Unauthorized access** | Directory-based access control | Path Validator, Secure Key Storage |

### Security Assumptions

- **File system security** — Host OS enforces file permissions correctly
- **Process isolation** — Each process has its own secure key storage area
- **Cryptographic library trust** — OpenSSL (if available) provides secure HMAC and random generation
- **Signal handling** — OS signal delivery is reliable

### Out of Scope

- **Network security** — No network-based log transmission protection (use TLS at transport layer)
- **Encryption at rest** — Use `encrypted_writer` for log file encryption (see `ENCRYPTION.md`)
- **Authentication** — No user identity verification (handled by application layer)

---

## Architecture

### Component Integration

```
┌─────────────────────────────────────────────────────────┐
│                    Application Code                      │
└───────────────────────────┬─────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────┐
│                     logger_builder                       │
│  .with_audit()  .with_sanitizer()  .with_path_validator()│
└───────────────────────────┬─────────────────────────────┘
                            │
        ┌───────────────────┼───────────────────┐
        ▼                   ▼                   ▼
┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│ Audit Logger │    │Log Sanitizer │    │Path Validator│
│              │    │              │    │              │
│ - HMAC sign  │    │ - PII detect │    │ - Path check │
│ - JSON logs  │    │ - Data mask  │    │ - Symlink    │
└──────────────┘    └──────────────┘    └──────────────┘
        │                                       │
        ▼                                       ▼
┌──────────────────────────────────────────────────────────┐
│              Secure Key Storage                          │
│  - Key generation (RAND_bytes)                           │
│  - Key persistence (0600 permissions)                    │
│  - Path traversal prevention                             │
└──────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────┐
│              Signal Manager                              │
│  - SIGSEGV/SIGABRT/SIGTERM/SIGINT handlers               │
│  - Signal-safe emergency flush                           │
│  - Logger registration                                   │
└──────────────────────────────────────────────────────────┘
```

### Dependency Graph

- **Audit Logger** → Secure Key Storage (HMAC key)
- **Path Validator** → Error Codes
- **Secure Key Storage** → Error Codes, OpenSSL (optional)
- **Signal Manager** → Signal Manager Interface, Critical Logger Interface

---

## Audit Logger

**Header:** `include/kcenon/logger/security/audit_logger.h`

### Purpose

Creates tamper-evident audit logs with HMAC signatures for compliance and forensic analysis.

### Key Features

- **JSON-formatted entries** — Structured logs for easy parsing and analysis
- **HMAC signatures** — Cryptographic integrity verification (SHA-256)
- **Thread-safe logging** — Safe for concurrent access
- **ISO 8601 timestamps** — Standardized time representation
- **OpenSSL 3.x and 1.1.x support** — Adapts to available OpenSSL version

### Audit Event Types

```cpp
enum class audit_event {
    logger_started,                    // Logger initialization
    logger_stopped,                    // Logger shutdown
    writer_added,                      // Writer registered
    writer_removed,                    // Writer unregistered
    encryption_key_loaded,             // Encryption key loaded from file
    encryption_key_rotated,            // Encryption key rotated
    encryption_key_generated,          // New encryption key generated
    permission_denied,                 // Access denied (file, resource)
    path_traversal_attempt,            // Path traversal detected
    insecure_permissions_detected,     // File permissions too permissive
    suspicious_activity,               // Generic security alert
    configuration_changed,             // Logger configuration modified
    authentication_success,            // Authentication succeeded
    authentication_failure,            // Authentication failed
    file_access_denied,                // File access denied
    file_access_granted                // File access granted
};
```

### API

#### Initialize Audit Logger

```cpp
void audit_logger::initialize(
    const std::filesystem::path& audit_file,
    const secure_key* hmac_key = nullptr
);
```

**Parameters:**
- `audit_file` — Path to audit log file (created with 0600 permissions)
- `hmac_key` — Optional HMAC key for tamper detection (recommended)

**Behavior:**
- Creates parent directory if needed
- Sets secure file permissions (owner read/write only)
- Copies HMAC key if provided

#### Log an Audit Event

```cpp
void audit_logger::log_audit_event(
    audit_event event,
    const std::string& details,
    const std::map<std::string, std::string>& metadata = {}
);
```

**Parameters:**
- `event` — Type of security event (see `audit_event` enum)
- `details` — Human-readable description
- `metadata` — Optional key-value pairs for additional context

**Output format:**
```json
{"timestamp":"2025-02-09T14:30:45","event":"encryption_key_loaded","details":"Loaded AES-256 key from /var/log/keys/master.key","metadata":{"key_size":"32","algorithm":"AES-256-GCM"}}
SIGNATURE:a3f5d8c92b1e... (if HMAC key provided)
```

#### Verify Entry Integrity

```cpp
bool audit_logger::verify_entry(
    const std::string& json_entry,
    const std::string& signature
);
```

**Returns:** `true` if signature matches, `false` otherwise

**Use case:** Verify audit log integrity after-the-fact (forensic analysis)

### Example Usage

```cpp
#include <kcenon/logger/security/audit_logger.h>
#include <kcenon/logger/security/secure_key_storage.h>

// Generate HMAC key for audit log integrity
auto hmac_key_result = secure_key_storage::generate_key(32);
if (hmac_key_result.is_err()) {
    // Handle error
}
secure_key hmac_key = std::move(hmac_key_result).unwrap();

// Save key for future verification
secure_key_storage::save_key(hmac_key, "/var/log/keys/audit_hmac.key");

// Initialize audit logger
audit_logger::initialize("/var/log/audit/security.log", &hmac_key);

// Log security-relevant events
audit_logger::log_audit_event(
    audit_event::encryption_key_loaded,
    "Loaded master encryption key",
    {{"key_id", "master-key-2025-02-09"}, {"algorithm", "AES-256-GCM"}}
);

audit_logger::log_audit_event(
    audit_event::permission_denied,
    "User attempted to write to restricted directory",
    {{"user", "jdoe"}, {"path", "/root/restricted"}}
);
```

### Compliance Considerations

- **SOX (Sarbanes-Oxley)** — Tamper-evident audit logs for financial systems
- **HIPAA** — Audit trail for healthcare data access
- **GDPR** — Data processing activity logs
- **PCI-DSS** — Security event monitoring (Requirement 10.x)

**Best practices:**
- Rotate audit logs regularly (daily or weekly)
- Store HMAC keys securely (separate from audit logs)
- Verify signatures periodically to detect tampering
- Archive audit logs to immutable storage (S3 Glacier, tape)

---

## Log Sanitizer

**Header:** `include/kcenon/logger/security/log_sanitizer.h`

### Purpose

Automatically detects and masks sensitive data (PII, credentials, financial information) in log messages to prevent data leakage.

### Key Features

- **Built-in patterns** — Credit cards, SSNs, API keys, passwords, emails, IPs, phone numbers
- **Custom patterns** — Add regex-based rules for organization-specific sensitive data
- **Partial preservation** — Keep last N characters visible (e.g., last 4 digits of credit card)
- **Thread-safe** — Safe for concurrent use
- **Zero configuration** — Use `make_default_sanitizer()` for quick setup

### Sensitive Data Types

```cpp
enum class sensitive_data_type {
    credit_card,     // Visa, MC, Amex (masks all but last 4)
    ssn,             // US Social Security Numbers (masks all but last 4)
    api_key,         // API keys (sk-, api_, bearer tokens) — fully redacted
    password,        // Password fields (password=xxx) — fully redacted
    email,           // Email addresses (partial masking: j***@example.com)
    ip_address,      // IPv4 addresses (masks last 2 octets: 192.168.x.x)
    phone_number,    // Phone numbers (masks middle digits)
    custom           // User-defined patterns
};
```

### API

#### Add Built-in Pattern

```cpp
log_sanitizer& log_sanitizer::add_pattern(sensitive_data_type type);
```

**Returns:** Reference to `this` for method chaining

#### Add Custom Pattern

```cpp
log_sanitizer& log_sanitizer::add_custom_pattern(
    std::string_view name,
    std::string_view regex_pattern,
    std::string_view replacement = "[REDACTED]",
    bool preserve_partial = false
);
```

**Parameters:**
- `name` — Identifier for the pattern (used for removal)
- `regex_pattern` — Regular expression to match sensitive data
- `replacement` — Replacement string (supports `$1`, `$2` for capture groups)
- `preserve_partial` — If `true`, preserve some characters (e.g., last 4 digits)

#### Remove Pattern

```cpp
log_sanitizer& log_sanitizer::remove_pattern(std::string_view name);
```

#### Sanitize a String

```cpp
std::string log_sanitizer::sanitize(std::string_view input) const;
```

**Returns:** Sanitized string with sensitive data masked

#### Check for Sensitive Data

```cpp
bool log_sanitizer::contains_sensitive_data(std::string_view input) const;
```

**Returns:** `true` if any pattern matches, `false` otherwise

#### Get Active Patterns

```cpp
std::vector<std::string> log_sanitizer::active_patterns() const;
```

**Returns:** List of currently active pattern names

#### Add All Common Patterns

```cpp
log_sanitizer& log_sanitizer::add_common_patterns();
```

**Adds:** `credit_card`, `ssn`, `api_key`, `password`, `email`

### Example Usage

```cpp
#include <kcenon/logger/security/log_sanitizer.h>

// Option 1: Use default sanitizer
auto sanitizer = make_default_sanitizer();

// Option 2: Custom configuration
log_sanitizer sanitizer;
sanitizer.add_pattern(sensitive_data_type::credit_card)
         .add_pattern(sensitive_data_type::email)
         .add_pattern(sensitive_data_type::api_key);

// Add organization-specific pattern
sanitizer.add_custom_pattern(
    "employee_id",
    R"(\bEMP-\d{6}\b)",     // Matches EMP-123456
    "EMP-******",
    false
);

// Sanitize log messages before writing
std::string message = "User john@example.com paid with card 4111-1111-1111-1111 using API key sk_live_abc123xyz";
std::string safe_message = sanitizer.sanitize(message);
// Result: "User j***@example.com paid with card ****-****-****-1111 using API key sk_[REDACTED]"

// Check if a string contains sensitive data
if (sanitizer.contains_sensitive_data(user_input)) {
    // Log warning or reject input
}
```

### Performance Impact

- **Regex matching** — O(n × m) where n = input length, m = number of patterns
- **Mitigation** — Use sanitizer selectively (e.g., only on user-facing logs)
- **Benchmark** — ~50-100 µs for typical log message with 5 patterns

**Optimization tips:**
- Add only necessary patterns
- Use `contains_sensitive_data()` for fast pre-check
- Consider sampling (sanitize 1% of logs for monitoring)

### Integration with Logger

```cpp
// Apply sanitization in formatter or writer
class sanitizing_writer {
    log_sanitizer sanitizer_;
    std::unique_ptr<writer_interface> inner_writer_;

public:
    void write(const log_message& msg) override {
        log_message safe_msg = msg;
        safe_msg.text = sanitizer_.sanitize(msg.text);
        inner_writer_->write(safe_msg);
    }
};
```

---

## Path Validator

**Header:** `include/kcenon/logger/security/path_validator.h`

### Purpose

Prevents path traversal attacks (`../../../etc/passwd`) and validates file paths for secure log file creation.

### Key Features

- **Canonical path resolution** — Resolves `.` and `..` components
- **Directory enforcement** — Ensures paths stay within allowed base directory
- **Symlink validation** — Detects and optionally rejects symbolic links
- **Filename character restrictions** — Allows only alphanumeric + `-_. `
- **Static utility functions** — Filename sanitization and safe path joining

### Security Checks

| Check | Default | Configurable |
|-------|---------|--------------|
| Path traversal (../) | ✅ Enabled | ❌ No |
| Symlink detection | ✅ Enabled | ✅ Yes (`allow_symlinks`) |
| Filename characters | ✅ Enabled | ✅ Yes (`strict_filename`) |
| Base directory enforcement | ✅ Enabled | ❌ No |

### API

#### Constructor

```cpp
explicit path_validator::path_validator(std::filesystem::path allowed_base);
```

**Parameters:**
- `allowed_base` — All validated paths must be within this directory

**Behavior:**
- Converts `allowed_base` to canonical form (if exists)
- Uses `weakly_canonical` for non-existent paths

#### Validate Path

```cpp
common::VoidResult path_validator::validate(
    const std::filesystem::path& path,
    bool allow_symlinks = false,
    bool strict_filename = true
) const;
```

**Parameters:**
- `path` — Path to validate
- `allow_symlinks` — If `false`, reject symbolic links
- `strict_filename` — If `true`, enforce alphanumeric + `-_.` restriction

**Returns:** `ok()` on success, `err(...)` with error code/message on failure

**Error codes:**
- `logger_error_code::path_traversal_detected` — Path escapes allowed base
- `logger_error_code::invalid_filename` — Filename contains dangerous characters

#### Static: Check Filename Safety

```cpp
static bool path_validator::is_safe_filename(const std::string& name);
```

**Allowed characters:** `a-z`, `A-Z`, `0-9`, `-`, `_`, `.`

**Returns:** `false` for empty string, `.`, `..`, or any invalid character

#### Static: Sanitize Filename

```cpp
static std::string path_validator::sanitize_filename(
    const std::string& name,
    char replacement = '_'
);
```

**Behavior:**
- Replaces invalid characters with `replacement` (default: `_`)
- Handles special cases (`.` → `_.`, `..` → `_..`, empty → `"unnamed"`)

#### Static: Safe Path Join

```cpp
static result<std::filesystem::path> path_validator::safe_join(
    const std::filesystem::path& base,
    const std::filesystem::path& relative
);
```

**Parameters:**
- `base` — Base directory
- `relative` — Relative path to join (must not be absolute)

**Returns:** `ok(joined_path)` or `err(...)` if validation fails

**Security:** Validates the joined path using `path_validator(base)`

### Example Usage

```cpp
#include <kcenon/logger/security/path_validator.h>

// Create validator for log directory
path_validator validator("/var/log/myapp");

// Validate user-provided log file path
std::string user_input = "../../../etc/passwd";  // Attack attempt
auto result = validator.validate(user_input);

if (result.is_err()) {
    std::cerr << "Invalid path: " << get_logger_error_message(result) << "\n";
    // Output: "Invalid path: Path must be within allowed directory: /var/log/myapp"
}

// Safe path joining
auto safe_path = path_validator::safe_join(
    "/var/log/myapp",
    "2025-02-09/server.log"  // OK
);

auto unsafe_path = path_validator::safe_join(
    "/var/log/myapp",
    "/etc/passwd"  // Rejected: absolute path
);

// Sanitize user-provided filename
std::string unsafe_name = "my log!@#$%file.txt";
std::string safe_name = path_validator::sanitize_filename(unsafe_name);
// Result: "my_log_____file.txt"
```

### Integration with Writers

```cpp
class validated_file_writer : public writer_interface {
    path_validator validator_;
    std::filesystem::path current_path_;

public:
    explicit validated_file_writer(std::filesystem::path base_dir)
        : validator_(std::move(base_dir)) {}

    void set_file_path(const std::filesystem::path& path) {
        auto result = validator_.validate(path, false, true);
        if (result.is_err()) {
            throw std::runtime_error("Invalid log file path: " +
                                     get_logger_error_message(result));
        }
        current_path_ = path;
        // Open file...
    }
};
```

---

## Secure Key Storage

**Header:** `include/kcenon/logger/security/secure_key_storage.h`

### Purpose

Securely generates, stores, and retrieves encryption keys with automatic memory cleanup and permission enforcement.

### Key Features

- **Secure random generation** — OpenSSL `RAND_bytes()` (or `std::random_device` fallback)
- **RAII memory management** — Automatic zeroing on destruction
- **Move-only semantics** — Prevents accidental key copies
- **File permission enforcement** — Keys saved with 0600 (owner read/write only)
- **Path traversal prevention** — Uses `path_validator` internally
- **OpenSSL support** — Uses `OPENSSL_cleanse()` for secure memory wiping

### Classes

#### `secure_key`

RAII wrapper for encryption keys with secure memory cleanup.

```cpp
class secure_key {
public:
    explicit secure_key(size_t size);               // Create empty key
    explicit secure_key(std::vector<uint8_t> data); // Create from data
    ~secure_key();                                  // Securely clears memory

    const std::vector<uint8_t>& data() const;       // Read-only access
    std::vector<uint8_t>& mutable_data();           // Mutable access (use with caution)
    size_t size() const;                            // Key size in bytes

    // Move-only (no copying)
    secure_key(const secure_key&) = delete;
    secure_key& operator=(const secure_key&) = delete;
    secure_key(secure_key&&) noexcept;
    secure_key& operator=(secure_key&&) noexcept;
};
```

**Memory cleanup:**
- OpenSSL available: `OPENSSL_cleanse(data, size)`
- Fallback: Volatile pointer zeroing (prevents compiler optimization)

#### `secure_key_storage`

Static utility class for key lifecycle management.

### API

#### Generate Secure Random Key

```cpp
static result<secure_key> secure_key_storage::generate_key(size_t size = 32);
```

**Parameters:**
- `size` — Key size in bytes (default: 32 for AES-256)

**Returns:** `ok(secure_key)` or `err(encryption_failed)` if random generation fails

**Randomness source:**
- OpenSSL available: `RAND_bytes()` (cryptographically secure)
- Fallback: `std::random_device` (platform-dependent quality)

#### Save Key to File

```cpp
static common::VoidResult secure_key_storage::save_key(
    const secure_key& key,
    const std::filesystem::path& path,
    const std::filesystem::path& allowed_base = "/var/log/keys"
);
```

**Parameters:**
- `key` — Key to save
- `path` — File path (must be within `allowed_base`)
- `allowed_base` — Base directory for key storage (default: `/var/log/keys`)

**Security:**
- Validates path (prevents traversal)
- Creates parent directory if needed
- Sets file permissions to 0600 (owner read/write only)
- Deletes file if permission setting fails (fail-secure)

**Returns:** `ok()` on success, `err(...)` on failure

**Error codes:**
- `path_traversal_detected` — Path escapes `allowed_base`
- `file_write_failed` — Failed to write key data
- `file_permission_denied` — Failed to set secure permissions

#### Load Key from File

```cpp
static result<secure_key> secure_key_storage::load_key(
    const std::filesystem::path& path,
    size_t expected_size = 32,
    const std::filesystem::path& allowed_base = "/var/log/keys"
);
```

**Parameters:**
- `path` — File path
- `expected_size` — Expected key size (default: 32 for AES-256)
- `allowed_base` — Base directory for key storage

**Security checks:**
1. Path validation (traversal prevention)
2. File existence check
3. **Permission verification** — Rejects files readable by group/others
4. File size verification (must match `expected_size`)
5. Key data read

**Returns:** `ok(secure_key)` or `err(...)`

**Error codes:**
- `path_traversal_detected` — Invalid path
- `file_open_failed` — File doesn't exist
- `insecure_permissions` — File permissions too permissive (not 0600)
- `invalid_key_size` — File size mismatch
- `file_read_failed` — Read error

### Example Usage

```cpp
#include <kcenon/logger/security/secure_key_storage.h>

// Generate a new AES-256 key
auto key_result = secure_key_storage::generate_key(32);
if (key_result.is_err()) {
    std::cerr << "Failed to generate key: "
              << get_logger_error_message(key_result) << "\n";
    return;
}

secure_key master_key = std::move(key_result).unwrap();

// Save key to file (0600 permissions)
auto save_result = secure_key_storage::save_key(
    master_key,
    "/var/log/keys/master.key",
    "/var/log/keys"  // allowed base directory
);

if (save_result.is_err()) {
    std::cerr << "Failed to save key: "
              << get_logger_error_message(save_result) << "\n";
    return;
}

// Later: Load key from file
auto load_result = secure_key_storage::load_key(
    "/var/log/keys/master.key",
    32,  // expected size
    "/var/log/keys"
);

if (load_result.is_err()) {
    std::cerr << "Failed to load key: "
              << get_logger_error_message(load_result) << "\n";
    return;
}

secure_key loaded_key = std::move(load_result).unwrap();

// Key is automatically zeroed when `loaded_key` goes out of scope
```

### Key Rotation

```cpp
// Generate new key
auto new_key = secure_key_storage::generate_key(32).unwrap();

// Save with timestamp suffix
std::string key_path = "/var/log/keys/master-" +
                       std::to_string(std::time(nullptr)) + ".key";
secure_key_storage::save_key(new_key, key_path);

// Log audit event
audit_logger::log_audit_event(
    audit_event::encryption_key_rotated,
    "Rotated master encryption key",
    {{"old_key", "master-1707491245.key"}, {"new_key", key_path}}
);

// Archive old key (or delete after re-encrypting data)
```

### Platform-Specific Considerations

#### Linux/macOS
- Uses OpenSSL for secure random generation and memory cleanup
- File permissions enforced via `chmod 600`
- Consider using kernel keyring (`keyctl`) for production

#### Windows
- Uses OpenSSL or `std::random_device` (backed by `CryptGenRandom`)
- File permissions less granular (ACLs recommended)
- Consider using DPAPI (`CryptProtectData`) for production

---

## Signal Manager

**Header:** `include/kcenon/logger/security/signal_manager.h`

**Interface:** `include/kcenon/logger/security/signal_manager_interface.h`

### Purpose

Ensures graceful shutdown and emergency log flushing when process receives fatal signals (SIGSEGV, SIGABRT, SIGTERM, SIGINT).

### Key Features

- **Signal-safe implementation** — Uses only signal-safe functions in handlers
- **Cross-platform** — Works on POSIX (Linux, macOS) and Windows
- **Centralized management** — Prevents conflicts when multiple loggers exist
- **Dependency injection** — Implements `signal_manager_interface` for testing
- **Emergency flush** — Flushes buffered logs using raw `write()` and `fsync()`

### Handled Signals

| Signal | Description | Behavior |
|--------|-------------|----------|
| `SIGSEGV` | Segmentation fault (crash) | Emergency flush → call original handler → abort |
| `SIGABRT` | Abort signal (assert failure) | Emergency flush → call original handler → abort |
| `SIGTERM` | Termination request (graceful shutdown) | Emergency flush → call original handler → exit |
| `SIGINT` | Interrupt (Ctrl+C) | Emergency flush → call original handler → exit |

### Signal-Safe Guarantees

**Functions allowed in signal handlers (POSIX):**
- ✅ `write()`, `fsync()`, `_exit()`
- ✅ Most syscalls (file I/O, process control)

**Functions NOT allowed (undefined behavior):**
- ❌ `malloc()`, `free()`, `new`, `delete`
- ❌ `printf()`, `std::cout`, `std::cerr` (not signal-safe)
- ❌ `std::mutex::lock()` (may deadlock)

**Implementation:** `signal_manager` uses only signal-safe functions in handlers.

### API

#### Register Logger

```cpp
void signal_manager::register_logger(critical_logger_interface* log) override;
```

**Behavior:**
- Adds logger to the set of loggers to flush on signal
- Installs signal handlers when first logger is registered
- Thread-safe (uses `std::mutex` outside of signal handler)

#### Unregister Logger

```cpp
void signal_manager::unregister_logger(critical_logger_interface* log) override;
```

**Behavior:**
- Removes logger from the set
- Uninstalls signal handlers when last logger is unregistered

#### Check Handler Installation

```cpp
bool signal_manager::are_handlers_installed() const override;
```

**Returns:** `true` if signal handlers are currently installed

#### Get Logger Count

```cpp
size_t signal_manager::logger_count() const override;
```

**Returns:** Number of registered loggers

### Critical Logger Interface

Loggers that support emergency flushing must implement:

```cpp
class critical_logger_interface {
public:
    virtual ~critical_logger_interface() = default;

    // Get file descriptor for emergency writing
    virtual int get_emergency_fd() const = 0;

    // Get emergency buffer pointer
    virtual const char* get_emergency_buffer() const = 0;

    // Get emergency buffer size
    virtual size_t get_emergency_buffer_size() const = 0;
};
```

**Requirements:**
- All methods must be **signal-safe** (no allocations, no locks)
- File descriptor must be valid (>= 0)
- Buffer must be pre-allocated (no dynamic allocation in handler)

### Example Usage

```cpp
#include <kcenon/logger/security/signal_manager.h>

// Logger implementation with emergency flush support
class my_logger : public critical_logger_interface {
    int fd_ = -1;
    std::array<char, 4096> emergency_buffer_;
    size_t buffer_pos_ = 0;

public:
    int get_emergency_fd() const override {
        return fd_;
    }

    const char* get_emergency_buffer() const override {
        return emergency_buffer_.data();
    }

    size_t get_emergency_buffer_size() const override {
        return buffer_pos_;
    }

    void write_log(const std::string& message) {
        // Normal logging (buffered)
        if (buffer_pos_ + message.size() < emergency_buffer_.size()) {
            std::memcpy(emergency_buffer_.data() + buffer_pos_,
                        message.data(), message.size());
            buffer_pos_ += message.size();
        }
    }
};

// Register logger with signal manager
signal_manager sig_mgr;
my_logger logger;

sig_mgr.register_logger(&logger);

// Now, if SIGSEGV/SIGABRT/SIGTERM/SIGINT is received:
// 1. Emergency flush writes buffer to disk using write() + fsync()
// 2. Original signal handler is called (or process terminates)

// Cleanup
sig_mgr.unregister_logger(&logger);
```

### Emergency Flush Implementation

```cpp
// Signal handler (in signal_manager.h)
static void signal_handler(int sig) {
    // Write signal message to stderr (signal-safe)
    const char* msg = "\n[CRITICAL] SIGSEGV received - emergency flush\n";
    write(STDERR_FILENO, msg, 45);

    // Flush all registered loggers
    for (auto* log : loggers_) {
        int fd = log->get_emergency_fd();
        const char* buffer = log->get_emergency_buffer();
        size_t size = log->get_emergency_buffer_size();

        // Write using signal-safe write() syscall
        ssize_t written = 0;
        while (written < size) {
            ssize_t n = write(fd, buffer + written, size - written);
            if (n <= 0) break;
            written += n;
        }

        // Sync to disk (signal-safe)
        fsync(fd);
    }

    // Call original handler or terminate
    _exit(128 + sig);
}
```

### Testing Considerations

Use `signal_manager_interface` for dependency injection:

```cpp
class mock_signal_manager : public signal_manager_interface {
    std::vector<critical_logger_interface*> loggers_;

public:
    void register_logger(critical_logger_interface* log) override {
        loggers_.push_back(log);
    }

    void unregister_logger(critical_logger_interface* log) override {
        loggers_.erase(std::remove(loggers_.begin(), loggers_.end(), log),
                       loggers_.end());
    }

    bool are_handlers_installed() const override {
        return !loggers_.empty();
    }

    size_t logger_count() const override {
        return loggers_.size();
    }

    // Test-only: Simulate signal
    void simulate_signal(int sig) {
        for (auto* log : loggers_) {
            // Call emergency flush logic
        }
    }
};
```

---

## Configuration Examples

### Minimal Security Setup

```cpp
#include <kcenon/logger/security/log_sanitizer.h>
#include <kcenon/logger/security/path_validator.h>

// Create logger with basic security
auto logger = logger_builder::create()
    .with_path_validator("/var/log/myapp")
    .with_sanitizer(make_default_sanitizer())
    .with_console_writer()
    .with_file_writer("/var/log/myapp/app.log")
    .build();
```

### High-Security Setup (Compliance)

```cpp
#include <kcenon/logger/security/audit_logger.h>
#include <kcenon/logger/security/log_sanitizer.h>
#include <kcenon/logger/security/path_validator.h>
#include <kcenon/logger/security/secure_key_storage.h>
#include <kcenon/logger/security/signal_manager.h>

// 1. Generate and save HMAC key for audit log
auto hmac_key = secure_key_storage::generate_key(32).unwrap();
secure_key_storage::save_key(hmac_key, "/var/log/keys/audit_hmac.key");

// 2. Initialize audit logger
audit_logger::initialize("/var/log/audit/security.log", &hmac_key);

// 3. Configure sanitizer
log_sanitizer sanitizer;
sanitizer.add_common_patterns()
         .add_custom_pattern("account_number", R"(\b\d{10,12}\b)", "[ACCT]");

// 4. Create logger with all security features
auto logger = logger_builder::create()
    .with_path_validator("/var/log/myapp")
    .with_sanitizer(std::move(sanitizer))
    .with_encrypted_writer("/var/log/myapp/encrypted.log")  // See ENCRYPTION.md
    .build();

// 5. Register signal handler for emergency flush
signal_manager sig_mgr;
sig_mgr.register_logger(logger.get());

// 6. Log audit events
audit_logger::log_audit_event(
    audit_event::logger_started,
    "Application logging initialized",
    {{"version", "2.0.0"}, {"security_level", "high"}}
);
```

### Custom Security Layer

```cpp
// Wrap writer with sanitization
class sanitizing_writer : public writer_interface {
    log_sanitizer sanitizer_;
    std::unique_ptr<writer_interface> inner_;

public:
    sanitizing_writer(log_sanitizer sanitizer, std::unique_ptr<writer_interface> inner)
        : sanitizer_(std::move(sanitizer)), inner_(std::move(inner)) {}

    void write(const log_message& msg) override {
        log_message safe_msg = msg;
        safe_msg.text = sanitizer_.sanitize(msg.text);
        inner_->write(safe_msg);
    }
};

// Usage
auto writer = std::make_unique<sanitizing_writer>(
    make_default_sanitizer(),
    std::make_unique<file_writer>("/var/log/app.log")
);
```

---

## Compliance Considerations

### SOX (Sarbanes-Oxley)

**Requirements:**
- Audit trail for financial system access
- Tamper-evident logs
- Access control for log files

**Implementation:**
- Use `audit_logger` with HMAC signatures
- Log all authentication events (`authentication_success`, `authentication_failure`)
- Store audit logs with 0600 permissions
- Verify signatures periodically

**Retention:** Typically 7 years for financial records

### HIPAA (Health Insurance Portability and Accountability Act)

**Requirements:**
- Audit trail for PHI (Protected Health Information) access
- PII masking in non-audit logs
- Secure storage and transmission

**Implementation:**
- Use `log_sanitizer` for application logs (mask SSN, email, phone)
- Use `audit_logger` for access logs (who accessed what PHI, when)
- Encrypt audit logs with `encrypted_writer`

**Retention:** Minimum 6 years

### GDPR (General Data Protection Regulation)

**Requirements:**
- Data processing activity logs
- PII masking or pseudonymization
- Right to erasure (log retention limits)

**Implementation:**
- Use `log_sanitizer` to mask email, IP address, phone numbers
- Log data processing events:
  ```cpp
  audit_logger::log_audit_event(
      audit_event::configuration_changed,
      "User updated privacy settings",
      {{"user_id", "12345"}, {"action", "opt_out_marketing"}}
  );
  ```
- Implement log rotation and deletion after retention period

**Retention:** Varies by purpose (typically 1-3 years)

### PCI-DSS (Payment Card Industry Data Security Standard)

**Requirements:**
- Requirement 10.x: Track and monitor all access to network resources and cardholder data
- Mask PAN (Primary Account Number) in logs
- Secure audit log storage

**Implementation:**
- Use `log_sanitizer` with `credit_card` pattern (masks all but last 4 digits)
- Use `audit_logger` for access events:
  ```cpp
  audit_logger::log_audit_event(
      audit_event::file_access_granted,
      "User accessed payment processing module",
      {{"user", "admin"}, {"resource", "payment_gateway"}}
  );
  ```
- Store audit logs on separate server (or encrypted partition)

**Retention:** Minimum 1 year, 3 months online

### ISO 27001 (Information Security Management)

**Requirements:**
- Security event logging
- Log integrity protection
- Access control

**Implementation:**
- Use all security components (audit, sanitizer, path validator, key storage)
- Implement log monitoring and alerting
- Document security procedures in ISMS

---

## Frequently Asked Questions

### Q: Can I use the security module without OpenSSL?

**A:** Yes, but with reduced security:
- **Audit logger:** Falls back to simple hash (not cryptographically secure)
- **Secure key storage:** Falls back to `std::random_device` (platform-dependent quality)

**Recommendation:** Build with OpenSSL for production use.

### Q: How do I rotate encryption keys?

**A:** See [Key Rotation](#key-rotation) section. Recommended frequency: Every 90 days (or per compliance requirements).

### Q: What's the performance overhead of log sanitization?

**A:** ~50-100 µs per message (5 patterns). Use selectively or sample logs (e.g., 1% of traffic).

### Q: How do I verify audit log integrity?

**A:** Read the audit log file and verify each entry:

```cpp
std::ifstream audit_file("/var/log/audit/security.log");
std::string json_entry, signature_line;

while (std::getline(audit_file, json_entry)) {
    if (std::getline(audit_file, signature_line)) {
        // Extract signature from "SIGNATURE:abc123..."
        std::string signature = signature_line.substr(10);

        if (!audit_logger::verify_entry(json_entry, signature)) {
            std::cerr << "TAMPERING DETECTED: " << json_entry << "\n";
        }
    }
}
```

### Q: Can I use multiple loggers with one signal manager?

**A:** Yes. `signal_manager` maintains a set of all registered loggers and flushes all of them on signal.

### Q: How do I handle signal manager cleanup?

**A:** Unregister loggers before destroying them:

```cpp
{
    signal_manager sig_mgr;
    my_logger logger;

    sig_mgr.register_logger(&logger);

    // Use logger...

    sig_mgr.unregister_logger(&logger);  // Unregister before logger destructor
}
// sig_mgr destructor automatically uninstalls signal handlers
```

---

## Summary

The **logger_system security module** provides enterprise-grade protection for logging infrastructure:

- ✅ **Tamper detection** — HMAC-signed audit logs
- ✅ **Data privacy** — PII/sensitive data masking
- ✅ **Path security** — Traversal attack prevention
- ✅ **Key management** — Secure generation and storage
- ✅ **Graceful shutdown** — Signal-safe emergency flush
- ✅ **Compliance ready** — SOX, HIPAA, GDPR, PCI-DSS support

**Next steps:**
- Review [ENCRYPTION.md](ENCRYPTION.md) for encrypted log storage
- Review [ARCHITECTURE.md](ARCHITECTURE.md) for module integration
- Configure security features in `logger_builder`

**Related documentation:**
- [ENCRYPTION.md](ENCRYPTION.md) — Encrypted log file storage
- [API.md](API.md) — Full API reference
- [EXAMPLES.md](EXAMPLES.md) — Usage examples

---

*Last updated: 2025-02-09*
*logger_system version: 2.x*
