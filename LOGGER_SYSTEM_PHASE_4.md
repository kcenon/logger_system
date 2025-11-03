# Phase 4: Security Hardening

**Phase:** 4/5
**Priority:** 🟡 Medium
**Estimated Duration:** 2-3 weeks
**Objective:** Address security vulnerabilities and ensure compliance
**Status:** ⬜ Not Started
**Dependencies:** Phase 3 completion recommended

---

## Table of Contents

1. [Overview](#overview)
2. [Work Items](#work-items)
3. [Security Vulnerabilities and Solutions](#security-vulnerabilities-and-solutions)
4. [Security Testing](#security-testing)
5. [Compliance](#compliance)

---

## Overview

### Purpose
Phase 4 ensures safe usage in enterprise environments through **security vulnerability mitigation and regulatory compliance**.

### Key Improvements
1. **Secure Encryption Key Management:** Permission verification, memory cleanup
2. **File Path Validation:** Path traversal defense
3. **Signal Handler Safety:** Prevent global state conflicts
4. **Security Audit Logging:** Track critical events

### Compliance Objectives
- ✅ OWASP Top 10 mitigation
- ✅ GDPR log encryption requirements
- ✅ PCI DSS log security
- ✅ ISO 27001 access control

---

## Work Items

### Task 4.1: Improve Encryption Key Management
- [ ] 4.1.1 Design secure_key_storage class
- [ ] 4.1.2 Verify file permissions (0600)
- [ ] 4.1.3 Memory cleanup (OPENSSL_cleanse)
- [ ] 4.1.4 Support key rotation
- [ ] 4.1.5 KMS integration (optional)
- [ ] 4.1.6 Write unit tests

### Task 4.2: File Path Validation
- [ ] 4.2.1 Design path_validator class
- [ ] 4.2.2 Verify allowed directories
- [ ] 4.2.3 Validate symbolic links
- [ ] 4.2.4 Normalize relative paths
- [ ] 4.2.5 Apply to all writers
- [ ] 4.2.6 Write unit tests

### Task 4.3: Signal Handler Safety
- [ ] 4.3.1 Design signal_manager singleton
- [ ] 4.3.2 Manage global state
- [ ] 4.3.3 Use only signal-safe functions
- [ ] 4.3.4 Integrate with critical_writer
- [ ] 4.3.5 Write unit tests

### Task 4.4: Security Audit
- [ ] 4.4.1 Design audit_logger
- [ ] 4.4.2 Define critical events
- [ ] 4.4.3 Tamper prevention (HMAC)
- [ ] 4.4.4 Documentation

---

## Security Vulnerabilities and Solutions

### Vulnerability 4.1: Encryption Key Management

#### Current Code (Problem)
**File:** `include/kcenon/logger/writers/encrypted_writer.h`

```cpp
class encrypted_writer {
public:
    static std::vector<uint8_t> generate_key(size_t size = 32);

    static void save_key_to_file(
        const std::vector<uint8_t>& key,
        const std::string& filename
    ) {
        std::ofstream file(filename, std::ios::binary);
        file.write(reinterpret_cast<const char*>(key.data()), key.size());
        // ⚠️ No file permission verification
        // ⚠️ No memory cleanup
    }
};
```

**Vulnerabilities:**
1. File permissions are 644 (readable by all users)
2. Key remains in memory (vulnerable to memory dump attacks)
3. No key file path validation

#### Improved Code: Secure Key Management

```cpp
// secure_key_storage.h

namespace kcenon::logger::security {

class secure_key {
public:
    explicit secure_key(size_t size)
        : data_(size, 0) {
        // Secure random generation
        if (!RAND_bytes(data_.data(), data_.size())) {
            throw std::runtime_error("Failed to generate secure random key");
        }
    }

    ~secure_key() {
        // ✅ Memory cleanup (overwrite)
        OPENSSL_cleanse(data_.data(), data_.size());
    }

    // Prevent copying
    secure_key(const secure_key&) = delete;
    secure_key& operator=(const secure_key&) = delete;

    // Allow move only
    secure_key(secure_key&& other) noexcept
        : data_(std::move(other.data_)) {
        OPENSSL_cleanse(other.data_.data(), other.data_.size());
    }

    const std::vector<uint8_t>& data() const { return data_; }

private:
    std::vector<uint8_t> data_;
};

class secure_key_storage {
public:
    // Save key (with secure permissions)
    static result_void save_key(
        const secure_key& key,
        const std::filesystem::path& path
    ) {
        // 1. Path validation
        auto validation = validate_key_path(path);
        if (!validation) return validation;

        // 2. Write file
        std::ofstream file(path, std::ios::binary);
        if (!file) {
            return make_logger_error(
                logger_error_code::file_write_failed,
                "Failed to open key file"
            );
        }

        file.write(
            reinterpret_cast<const char*>(key.data().data()),
            key.data().size()
        );

        if (file.fail()) {
            return make_logger_error(
                logger_error_code::file_write_failed,
                "Failed to write key"
            );
        }

        file.close();

        // ✅ 3. Set file permissions (0600)
        std::filesystem::permissions(
            path,
            std::filesystem::perms::owner_read |
            std::filesystem::perms::owner_write,
            std::filesystem::perm_options::replace
        );

        return {};
    }

    // Load key (with permission verification)
    static result<secure_key> load_key(
        const std::filesystem::path& path
    ) {
        // 1. Check file permissions
        auto status = std::filesystem::status(path);
        auto perms = status.permissions();

        // ✅ Must not be readable by other users
        if ((perms & std::filesystem::perms::group_read) != std::filesystem::perms::none ||
            (perms & std::filesystem::perms::others_read) != std::filesystem::perms::none) {
            return make_logger_error(
                logger_error_code::insecure_permissions,
                "Key file permissions too permissive (must be 0600)"
            );
        }

        // 2. Read file
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            return make_logger_error(
                logger_error_code::file_read_failed,
                "Failed to open key file"
            );
        }

        // 3. Verify size
        auto size = std::filesystem::file_size(path);
        if (size != 32) {  // AES-256 key size
            return make_logger_error(
                logger_error_code::invalid_key_size,
                "Invalid key size"
            );
        }

        // 4. Load key
        secure_key key(size);
        file.read(
            reinterpret_cast<char*>(const_cast<uint8_t*>(key.data().data())),
            size
        );

        if (file.fail()) {
            return make_logger_error(
                logger_error_code::file_read_failed,
                "Failed to read key"
            );
        }

        return key;
    }

private:
    static result_void validate_key_path(const std::filesystem::path& path) {
        // Verify path is within allowed key directory
        auto canonical = std::filesystem::weakly_canonical(path);
        auto allowed = std::filesystem::weakly_canonical("/etc/logger_keys/");

        // Compare paths
        auto [mismatch_iter, _] = std::mismatch(
            allowed.begin(), allowed.end(),
            canonical.begin(), canonical.end()
        );

        if (mismatch_iter != allowed.end()) {
            return make_logger_error(
                logger_error_code::path_traversal_detected,
                "Key path must be within /etc/logger_keys/"
            );
        }

        return {};
    }
};

} // namespace
```

#### KMS Integration (Optional)

```cpp
// kms_key_storage.h - AWS KMS example

class kms_key_storage {
public:
    static result<secure_key> load_key_from_kms(
        const std::string& key_id
    ) {
        // AWS KMS SDK call
        Aws::KMS::KMSClient kms_client;

        Aws::KMS::Model::DecryptRequest request;
        request.SetKeyId(key_id);
        // ...

        auto outcome = kms_client.Decrypt(request);

        if (outcome.IsSuccess()) {
            auto& plaintext = outcome.GetResult().GetPlaintext();
            secure_key key(plaintext.GetLength());
            std::memcpy(
                const_cast<uint8_t*>(key.data().data()),
                plaintext.GetUnderlyingData(),
                plaintext.GetLength()
            );
            return key;
        } else {
            return make_logger_error(
                logger_error_code::kms_error,
                outcome.GetError().GetMessage()
            );
        }
    }
};
```

---

### Vulnerability 4.2: Path Traversal

#### Current Code (Problem)
**File:** `src/impl/writers/file_writer.cpp`

```cpp
file_writer::file_writer(const std::string& filename, ...)
    : filename_(filename) {
    open();  // ⚠️ No filename validation
}
```

**Vulnerabilities:**
- Paths like `../../../etc/passwd` are possible
- Symbolic link attacks
- Writing outside allowed log directories

#### Improved Code: Path Validation

```cpp
// path_validator.h

namespace kcenon::logger::security {

class path_validator {
public:
    explicit path_validator(std::filesystem::path allowed_base)
        : allowed_base_(std::filesystem::canonical(allowed_base)) {
    }

    result_void validate(const std::filesystem::path& path) const {
        // 1. Convert to absolute path
        auto canonical = std::filesystem::weakly_canonical(path);

        // 2. Check symbolic links
        if (std::filesystem::is_symlink(canonical)) {
            canonical = std::filesystem::read_symlink(canonical);
        }

        // 3. Verify within allowed base
        auto [base_end, path_end] = std::mismatch(
            allowed_base_.begin(), allowed_base_.end(),
            canonical.begin(), canonical.end()
        );

        if (base_end != allowed_base_.end()) {
            return make_logger_error(
                logger_error_code::path_traversal_detected,
                "Path must be within allowed log directory"
            );
        }

        // 4. Validate filename (restrict special characters)
        auto filename = canonical.filename().string();
        if (!is_safe_filename(filename)) {
            return make_logger_error(
                logger_error_code::invalid_filename,
                "Filename contains invalid characters"
            );
        }

        return {};
    }

private:
    static bool is_safe_filename(const std::string& name) {
        // Allow: alphanumeric, hyphen, underscore, period
        return std::all_of(name.begin(), name.end(), [](char c) {
            return std::isalnum(c) || c == '-' || c == '_' || c == '.';
        });
    }

    std::filesystem::path allowed_base_;
};

} // namespace
```

#### file_writer Application

```cpp
// file_writer.cpp (improved)

class file_writer {
public:
    file_writer(
        const std::string& filename,
        const std::filesystem::path& allowed_base = "/var/log/myapp"
    ) {
        // ✅ Path validation
        path_validator validator(allowed_base);
        auto validation = validator.validate(filename);

        if (!validation) {
            throw std::runtime_error(validation.get_error().message());
        }

        filename_ = filename;
        open();
    }

    // ...
};
```

---

### Vulnerability 4.3: Signal Handler Safety

#### Current Code (Problem)
**File:** `include/kcenon/logger/writers/critical_writer.h`

```cpp
struct critical_writer_config {
    bool enable_signal_handlers = true;  // ⚠️ Modifies global state
};

// Conflicts possible when multiple critical_writers are created
```

#### Improved Code: Signal Manager

```cpp
// signal_manager.h

namespace kcenon::logger::security {

class signal_manager {
public:
    // Singleton
    static signal_manager& instance() {
        static signal_manager mgr;
        return mgr;
    }

    // Register logger
    void register_logger(logger* log) {
        std::lock_guard lock(mutex_);
        loggers_.insert(log);

        // Install handlers only on first registration
        if (loggers_.size() == 1) {
            install_handlers();
        }
    }

    // Unregister logger
    void unregister_logger(logger* log) {
        std::lock_guard lock(mutex_);
        loggers_.erase(log);

        // Remove handlers when last logger is unregistered
        if (loggers_.empty()) {
            uninstall_handlers();
        }
    }

private:
    signal_manager() = default;

    void install_handlers() {
        std::signal(SIGSEGV, signal_handler);
        std::signal(SIGABRT, signal_handler);
        std::signal(SIGTERM, signal_handler);
        // ...
    }

    void uninstall_handlers() {
        std::signal(SIGSEGV, SIG_DFL);
        std::signal(SIGABRT, SIG_DFL);
        std::signal(SIGTERM, SIG_DFL);
        // ...
    }

    static void signal_handler(int sig) {
        // ✅ Use only signal-safe functions
        auto& mgr = instance();

        // Emergency flush for all registered loggers
        for (auto* log : mgr.loggers_) {
            // flush() may not be signal-safe, so
            // use direct write() system call
            emergency_flush(log);
        }

        // Call original handler
        std::raise(sig);
    }

    static void emergency_flush(logger* log) {
        // Use only signal-safe functions
        // write(), fsync(), etc.
    }

    std::set<logger*> loggers_;
    std::mutex mutex_;
};

} // namespace
```

---

### Vulnerability 4.4: Security Audit Logging

```cpp
// audit_logger.h

namespace kcenon::logger::security {

class audit_logger {
public:
    enum class audit_event {
        logger_started,
        logger_stopped,
        writer_added,
        writer_removed,
        encryption_key_loaded,
        encryption_key_rotated,
        permission_denied,
        path_traversal_attempt,
        suspicious_activity
    };

    static void log_audit_event(
        audit_event event,
        const std::string& details,
        const std::map<std::string, std::string>& metadata = {}
    ) {
        // 1. Timestamp
        auto now = std::chrono::system_clock::now();

        // 2. Event serialization
        std::ostringstream oss;
        oss << "{"
            << "\"timestamp\":\"" << format_iso8601(now) << "\","
            << "\"event\":\"" << event_to_string(event) << "\","
            << "\"details\":\"" << escape_json(details) << "\"";

        // 3. Metadata
        if (!metadata.empty()) {
            oss << ",\"metadata\":{";
            bool first = true;
            for (const auto& [key, value] : metadata) {
                if (!first) oss << ",";
                oss << "\"" << escape_json(key) << "\":"
                    << "\"" << escape_json(value) << "\"";
                first = false;
            }
            oss << "}";
        }

        oss << "}";

        auto json = oss.str();

        // 4. HMAC signature (tamper prevention)
        auto signature = calculate_hmac(json, get_audit_key());

        // 5. Write to audit log file
        write_to_audit_log(json + "\n" + "SIGNATURE:" + signature + "\n");
    }

private:
    static std::string calculate_hmac(
        const std::string& message,
        const secure_key& key
    );

    static void write_to_audit_log(const std::string& entry);
};

} // namespace
```

---

## Security Testing

### Test 4.1: Key Management Testing

```cpp
TEST(SecureKeyStorageTest, FilePermissions) {
    secure_key key(32);
    auto path = "/tmp/test_key.bin";

    auto result = secure_key_storage::save_key(key, path);
    ASSERT_TRUE(result);

    // Check permissions
    auto status = std::filesystem::status(path);
    auto perms = status.permissions();

    // Must be 0600
    EXPECT_EQ(perms & std::filesystem::perms::group_all, std::filesystem::perms::none);
    EXPECT_EQ(perms & std::filesystem::perms::others_all, std::filesystem::perms::none);
}

TEST(SecureKeyStorageTest, MemoryCleansing) {
    uint8_t* leaked_ptr = nullptr;

    {
        secure_key key(32);
        leaked_ptr = const_cast<uint8_t*>(key.data().data());

        // Store pattern in key
        std::fill_n(leaked_ptr, 32, 0xAA);
    }

    // Check memory after destructor call
    // OPENSSL_cleanse should have overwritten with zeros
    bool all_zeroed = std::all_of(leaked_ptr, leaked_ptr + 32, [](uint8_t b) {
        return b == 0;
    });

    EXPECT_TRUE(all_zeroed);
}
```

### Test 4.2: Path Traversal Testing

```cpp
TEST(PathValidatorTest, PathTraversalAttack) {
    path_validator validator("/var/log/myapp");

    // Attack attempt
    auto result = validator.validate("../../../etc/passwd");

    EXPECT_FALSE(result);
    EXPECT_EQ(result.get_error().code(), logger_error_code::path_traversal_detected);
}

TEST(PathValidatorTest, SymlinkAttack) {
    // Create symbolic link: /tmp/safe_link -> /etc/passwd
    std::filesystem::create_symlink("/etc/passwd", "/tmp/safe_link");

    path_validator validator("/tmp");
    auto result = validator.validate("/tmp/safe_link");

    // Symbolic links not allowed
    EXPECT_FALSE(result);
}
```

---

## Compliance

### GDPR (General Data Protection Regulation)
- ✅ Personal information encryption (encrypted_writer)
- ✅ Log access control (file permissions 0600)
- ✅ Audit trail (audit_logger)

### PCI DSS (Payment Card Industry Data Security)
- ✅ Log encryption
- ✅ Access logging
- ✅ Key management (key rotation)

### ISO 27001 (Information Security Management)
- ✅ Security policy documentation
- ✅ Risk assessment and mitigation
- ✅ Audit and monitoring

---

## Work Checklist

### Pre-Work Preparation
- [ ] Confirm Phase 3 completion
- [ ] Install security audit tools (Valgrind, AddressSanitizer)
- [ ] Create Git branch (`feature/phase4-security`)

### During Work
- [ ] Task 4.1: Encryption key management
- [ ] Task 4.2: File path validation
- [ ] Task 4.3: Signal handler safety
- [ ] Task 4.4: Security audit

### Post-Work Validation
- [ ] Pass security tests
- [ ] Permission verification tests
- [ ] Confirm path traversal defense
- [ ] Complete code review

### Deployment
- [ ] Create Git tag (`v1.3.0`)
- [ ] Write Release Notes
- [ ] Prepare for Phase 5 (optional)

---

## Next Steps

After Phase 4 completion:
- **Phase 5 (Optional):** Architecture refactoring (PIMPL removal, etc.)
- **Documentation:** [LOGGER_SYSTEM_PHASE_5.md](./LOGGER_SYSTEM_PHASE_5.md)

---

**Start Date:** TBD
**Assignee:** TBD
**Reviewer:** TBD
