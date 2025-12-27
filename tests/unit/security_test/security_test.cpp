/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>
#include <kcenon/logger/security/secure_key_storage.h>
#include <kcenon/logger/security/path_validator.h>
#include <kcenon/logger/security/audit_logger.h>
#include <filesystem>
#include <fstream>

using namespace kcenon::logger;
using namespace kcenon::logger::security;

// Test fixture for security tests
class SecurityTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create temporary test directory
        test_dir_ = std::filesystem::temp_directory_path() / "logger_security_test";
        std::filesystem::create_directories(test_dir_);
    }

    void TearDown() override {
        // Clean up test directory
        if (std::filesystem::exists(test_dir_)) {
            std::filesystem::remove_all(test_dir_);
        }
    }

    std::filesystem::path test_dir_;
};

// ============================================================================
// secure_key Tests
// ============================================================================

TEST_F(SecurityTest, SecureKeyConstruction) {
    secure_key key(32);
    EXPECT_EQ(key.size(), 32);
}

TEST_F(SecurityTest, SecureKeyMove) {
    secure_key key1(32);
    [[maybe_unused]] auto* original_data = key1.data().data();

    secure_key key2(std::move(key1));

    EXPECT_EQ(key2.size(), 32);
    EXPECT_EQ(key1.size(), 0);  // Moved-from key should be cleared
}

// ============================================================================
// secure_key_storage Tests
// ============================================================================

TEST_F(SecurityTest, GenerateKey) {
    auto result = secure_key_storage::generate_key(32);
    ASSERT_TRUE(result.has_value());

    auto& key = result.value();
    EXPECT_EQ(key.size(), 32);

    // Verify key is not all zeros (statistically unlikely)
    bool has_nonzero = false;
    for (auto byte : key.data()) {
        if (byte != 0) {
            has_nonzero = true;
            break;
        }
    }
    EXPECT_TRUE(has_nonzero);
}

TEST_F(SecurityTest, SaveAndLoadKey) {
    auto key_path = test_dir_ / "test_key.bin";

    // Generate and save key
    auto gen_result = secure_key_storage::generate_key(32);
    ASSERT_TRUE(gen_result.has_value());

    auto save_result = secure_key_storage::save_key(
        gen_result.value(),
        key_path,
        test_dir_
    );
    ASSERT_TRUE(save_result.is_ok());

    // Verify file exists
    EXPECT_TRUE(std::filesystem::exists(key_path));

    // Verify file permissions (0600)
    auto status = std::filesystem::status(key_path);
    auto perms = status.permissions();

    EXPECT_EQ(perms & std::filesystem::perms::group_read, std::filesystem::perms::none);
    EXPECT_EQ(perms & std::filesystem::perms::others_read, std::filesystem::perms::none);

    // Load key and verify
    auto load_result = secure_key_storage::load_key(key_path, 32, test_dir_);
    ASSERT_TRUE(load_result.has_value());

    auto& loaded_key = load_result.value();
    EXPECT_EQ(loaded_key.size(), 32);

    // Verify keys match
    EXPECT_EQ(loaded_key.data(), gen_result.value().data());
}

TEST_F(SecurityTest, LoadKeyWithInsecurePermissions) {
    auto key_path = test_dir_ / "insecure_key.bin";

    // Create key file with insecure permissions (644)
    std::ofstream file(key_path, std::ios::binary);
    std::vector<uint8_t> dummy_key(32, 0xAA);
    file.write(reinterpret_cast<const char*>(dummy_key.data()), 32);
    file.close();

    // Set insecure permissions
    std::filesystem::permissions(
        key_path,
        std::filesystem::perms::owner_read |
        std::filesystem::perms::owner_write |
        std::filesystem::perms::group_read |  // Insecure!
        std::filesystem::perms::others_read,  // Insecure!
        std::filesystem::perm_options::replace
    );

    // Attempt to load should fail
    auto result = secure_key_storage::load_key(key_path, 32, test_dir_);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error_code(), logger_error_code::insecure_permissions);
}

TEST_F(SecurityTest, LoadKeyWithInvalidSize) {
    auto key_path = test_dir_ / "wrong_size_key.bin";

    // Create key file with wrong size (16 bytes instead of 32)
    std::ofstream file(key_path, std::ios::binary);
    std::vector<uint8_t> dummy_key(16, 0xAA);
    file.write(reinterpret_cast<const char*>(dummy_key.data()), 16);
    file.close();

    // Set secure permissions
    std::filesystem::permissions(
        key_path,
        std::filesystem::perms::owner_read |
        std::filesystem::perms::owner_write,
        std::filesystem::perm_options::replace
    );

    // Attempt to load with expected size 32 should fail
    auto result = secure_key_storage::load_key(key_path, 32, test_dir_);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error_code(), logger_error_code::invalid_key_size);
}

// ============================================================================
// path_validator Tests
// ============================================================================

TEST_F(SecurityTest, PathValidatorValidPath) {
    path_validator validator(test_dir_);

    auto valid_path = test_dir_ / "logs" / "test.log";
    auto result = validator.validate(valid_path);

    EXPECT_TRUE(result.is_ok());
}

TEST_F(SecurityTest, PathValidatorPathTraversal) {
    path_validator validator(test_dir_);

    // Attempt path traversal attack
    auto attack_path = test_dir_ / ".." / ".." / "etc" / "passwd";
    auto result = validator.validate(attack_path);

    EXPECT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result), logger_error_code::path_traversal_detected);
}

TEST_F(SecurityTest, PathValidatorSymlink) {
    path_validator validator(test_dir_);

    // Create a symlink pointing outside allowed directory
    auto symlink_path = test_dir_ / "evil_link";
    auto target_path = std::filesystem::temp_directory_path() / "outside.txt";

    // Create target file
    std::ofstream(target_path).close();

    // Create symlink
    try {
        std::filesystem::create_symlink(target_path, symlink_path);
    } catch (const std::filesystem::filesystem_error&) {
        // Skip test if symlinks are not supported
        GTEST_SKIP() << "Symlinks not supported on this platform";
    }

    // Validate with symlinks not allowed (default)
    auto result = validator.validate(symlink_path, false);

    EXPECT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result), logger_error_code::path_traversal_detected);

    // Clean up
    std::filesystem::remove(symlink_path);
    std::filesystem::remove(target_path);
}

TEST_F(SecurityTest, PathValidatorInvalidFilename) {
    path_validator validator(test_dir_);

    // Filename with invalid characters
    auto invalid_path = test_dir_ / "test*.log";  // '*' is not allowed
    auto result = validator.validate(invalid_path, false, true);

    EXPECT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result), logger_error_code::invalid_filename);
}

TEST_F(SecurityTest, IsSafeFilename) {
    // Valid filenames
    EXPECT_TRUE(path_validator::is_safe_filename("test.log"));
    EXPECT_TRUE(path_validator::is_safe_filename("test_file-123.txt"));
    EXPECT_TRUE(path_validator::is_safe_filename("data.2025-01-01.log"));

    // Invalid filenames
    EXPECT_FALSE(path_validator::is_safe_filename("test*.log"));      // '*' not allowed
    EXPECT_FALSE(path_validator::is_safe_filename("test/file.log"));  // '/' not allowed
    EXPECT_FALSE(path_validator::is_safe_filename(".."));             // Special name
    EXPECT_FALSE(path_validator::is_safe_filename("."));              // Special name
    EXPECT_FALSE(path_validator::is_safe_filename(""));               // Empty
}

TEST_F(SecurityTest, SanitizeFilename) {
    EXPECT_EQ(path_validator::sanitize_filename("test*.log"), "test_.log");
    EXPECT_EQ(path_validator::sanitize_filename("test/file.log"), "test_file.log");
    EXPECT_EQ(path_validator::sanitize_filename("hello world"), "hello_world");
    EXPECT_EQ(path_validator::sanitize_filename(".."), "_.");
    EXPECT_EQ(path_validator::sanitize_filename(""), "unnamed");
}

TEST_F(SecurityTest, SafeJoin) {
    auto base = test_dir_;
    auto relative = std::filesystem::path("logs") / "test.log";

    auto result = path_validator::safe_join(base, relative);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), base / relative);
}

TEST_F(SecurityTest, SafeJoinWithAbsolutePath) {
    auto base = test_dir_;
    auto absolute = std::filesystem::path("/etc/passwd");

    auto result = path_validator::safe_join(base, absolute);

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error_code(), logger_error_code::path_traversal_detected);
}

// ============================================================================
// audit_logger Tests
// ============================================================================

TEST_F(SecurityTest, AuditLoggerInitialize) {
    auto audit_file = test_dir_ / "audit.log";

    audit_logger::initialize(audit_file);

    // Log an event
    audit_logger::log_audit_event(
        audit_logger::audit_event::logger_started,
        "Test logger started",
        {{"version", "1.0.0"}, {"mode", "test"}}
    );

    // Verify audit file was created
    EXPECT_TRUE(std::filesystem::exists(audit_file));

    // Verify file permissions (check that owner has read/write at minimum)
    auto status = std::filesystem::status(audit_file);
    auto perms = status.permissions();

    EXPECT_NE(perms & std::filesystem::perms::owner_read, std::filesystem::perms::none);
    EXPECT_NE(perms & std::filesystem::perms::owner_write, std::filesystem::perms::none);

    // Read and verify content
    std::ifstream file(audit_file);
    std::string line;
    std::getline(file, line);

    // Verify JSON contains expected fields
    EXPECT_NE(line.find("\"timestamp\""), std::string::npos);
    EXPECT_NE(line.find("\"event\":\"logger_started\""), std::string::npos);
    EXPECT_NE(line.find("\"details\":\"Test logger started\""), std::string::npos);
    EXPECT_NE(line.find("\"metadata\""), std::string::npos);
    EXPECT_NE(line.find("\"version\":\"1.0.0\""), std::string::npos);
}

TEST_F(SecurityTest, AuditLoggerWithHMAC) {
    auto audit_file = test_dir_ / "audit_hmac.log";

    // Generate HMAC key
    auto key_result = secure_key_storage::generate_key(32);
    ASSERT_TRUE(key_result.has_value());

    audit_logger::initialize(audit_file, &key_result.value());

    // Log an event
    audit_logger::log_audit_event(
        audit_logger::audit_event::encryption_key_loaded,
        "Encryption key loaded successfully"
    );

    // Verify signature was written
    std::ifstream file(audit_file);
    std::string json_line, signature_line;
    std::getline(file, json_line);
    std::getline(file, signature_line);

    EXPECT_TRUE(signature_line.find("SIGNATURE:") != std::string::npos);

    // Verify signature
    auto sig_start = signature_line.find("SIGNATURE:") + 10;
    auto signature = signature_line.substr(sig_start);

    EXPECT_TRUE(audit_logger::verify_entry(json_line, signature));
}

TEST_F(SecurityTest, AuditLoggerSecurityEvents) {
    auto audit_file = test_dir_ / "audit_security.log";
    audit_logger::initialize(audit_file);

    // Test various security events
    audit_logger::log_audit_event(
        audit_logger::audit_event::path_traversal_attempt,
        "Attempted to access ../../../etc/passwd",
        {{"source", "file_writer"}, {"blocked", "true"}}
    );

    audit_logger::log_audit_event(
        audit_logger::audit_event::insecure_permissions_detected,
        "Key file has insecure permissions (644)",
        {{"file", "test_key.bin"}, {"expected", "0600"}}
    );

    audit_logger::log_audit_event(
        audit_logger::audit_event::permission_denied,
        "Failed to write to restricted directory",
        {{"path", "/var/log/system"}}
    );

    // Verify all events were logged
    std::ifstream file(audit_file);
    std::string line;
    int event_count = 0;

    while (std::getline(file, line)) {
        if (line.find("\"event\":") != std::string::npos) {
            event_count++;
        }
    }

    EXPECT_EQ(event_count, 3);
}

// ============================================================================
// Integration Tests
// ============================================================================

TEST_F(SecurityTest, IntegrationSecureKeyWorkflow) {
    auto key_path = test_dir_ / "integration_key.bin";

    // 1. Generate key
    auto gen_result = secure_key_storage::generate_key(32);
    ASSERT_TRUE(gen_result.has_value());

    // 2. Save key with path validation
    path_validator validator(test_dir_);
    auto path_validation = validator.validate(key_path);
    ASSERT_TRUE(path_validation.is_ok());

    auto save_result = secure_key_storage::save_key(
        gen_result.value(),
        key_path,
        test_dir_
    );
    ASSERT_TRUE(save_result.is_ok());

    // 3. Log audit event
    auto audit_file = test_dir_ / "integration_audit.log";
    audit_logger::initialize(audit_file);
    audit_logger::log_audit_event(
        audit_logger::audit_event::encryption_key_generated,
        "New encryption key generated and saved",
        {{"key_file", key_path.string()}}
    );

    // 4. Load key
    auto load_result = secure_key_storage::load_key(key_path, 32, test_dir_);
    ASSERT_TRUE(load_result.has_value());

    // 5. Log another audit event
    audit_logger::log_audit_event(
        audit_logger::audit_event::encryption_key_loaded,
        "Encryption key loaded successfully",
        {{"key_file", key_path.string()}}
    );

    // Verify audit log has both events
    std::ifstream file(audit_file);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    EXPECT_NE(content.find("encryption_key_generated"), std::string::npos);
    EXPECT_NE(content.find("encryption_key_loaded"), std::string::npos);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
