#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/core/error_codes.h>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <cstring>

// OpenSSL for secure memory cleanup (optional, fallback to manual zeroing)
// Note: OPENSSL_cleanse() and RAND_bytes() are supported in both OpenSSL 1.1.x and 3.x
#if __has_include(<openssl/rand.h>)
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/opensslv.h>
#define HAS_OPENSSL 1
#endif

namespace kcenon::logger::security {

/**
 * @class secure_key
 * @brief RAII wrapper for encryption keys with secure memory management
 *
 * Features:
 * - Automatic memory cleanup on destruction
 * - Move-only semantics (prevents accidental copies)
 * - Secure random generation with OpenSSL (if available)
 */
class secure_key {
public:
    /**
     * @brief Construct with specified size
     * @param size Key size in bytes (32 for AES-256)
     */
    explicit secure_key(size_t size)
        : data_(size, 0) {
    }

    /**
     * @brief Construct with data
     * @param data Key data
     */
    explicit secure_key(std::vector<uint8_t> data)
        : data_(std::move(data)) {
    }

    /**
     * @brief Destructor - securely clears key from memory
     */
    ~secure_key() {
        secure_clear();
    }

    // Prevent copying
    secure_key(const secure_key&) = delete;
    secure_key& operator=(const secure_key&) = delete;

    // Allow move only
    secure_key(secure_key&& other) noexcept
        : data_(std::move(other.data_)) {
        // Clear the moved-from object
        other.secure_clear();
    }

    secure_key& operator=(secure_key&& other) noexcept {
        if (this != &other) {
            secure_clear();
            data_ = std::move(other.data_);
            other.secure_clear();
        }
        return *this;
    }

    /**
     * @brief Get const reference to key data
     */
    const std::vector<uint8_t>& data() const {
        return data_;
    }

    /**
     * @brief Get mutable reference to key data (use with caution)
     */
    std::vector<uint8_t>& mutable_data() {
        return data_;
    }

    /**
     * @brief Get key size in bytes
     */
    size_t size() const {
        return data_.size();
    }

private:
    /**
     * @brief Securely clear key data from memory
     */
    void secure_clear() {
        if (data_.empty()) {
            return;
        }

#ifdef HAS_OPENSSL
        // Use OpenSSL's secure memory clearing
        OPENSSL_cleanse(data_.data(), data_.size());
#else
        // Fallback: manual zeroing with volatile to prevent optimization
        volatile uint8_t* ptr = data_.data();
        for (size_t i = 0; i < data_.size(); ++i) {
            ptr[i] = 0;
        }
#endif
        data_.clear();
    }

    std::vector<uint8_t> data_;
};

/**
 * @class secure_key_storage
 * @brief Secure storage and retrieval of encryption keys
 *
 * Security features:
 * - File permission verification (0600)
 * - Path traversal prevention
 * - Secure random generation
 * - Memory cleanup after use
 */
class secure_key_storage {
public:
    /**
     * @brief Generate a secure random key
     * @param size Key size in bytes (default: 32 for AES-256)
     * @return Result containing the generated key or error
     */
    static result<secure_key> generate_key(size_t size = 32) {
        secure_key key(size);

#ifdef HAS_OPENSSL
        // Use OpenSSL's cryptographically secure random generator
        if (RAND_bytes(key.mutable_data().data(), size) != 1) {
            return make_logger_error<secure_key>(
                logger_error_code::encryption_failed,
                "Failed to generate secure random key"
            );
        }
#else
        // Fallback: use std::random_device (less secure on some platforms)
        std::random_device rd;
        for (size_t i = 0; i < size; ++i) {
            key.mutable_data()[i] = static_cast<uint8_t>(rd() & 0xFF);
        }
#endif

        return result<secure_key>(std::move(key));
    }

    /**
     * @brief Save key to file with secure permissions
     * @param key The key to save
     * @param path File path (must be in allowed directory)
     * @param allowed_base Base directory for key storage
     * @return Success or error
     */
    static common::VoidResult save_key(
        const secure_key& key,
        const std::filesystem::path& path,
        const std::filesystem::path& allowed_base = "/var/log/keys"
    ) {
        // 1. Validate path
        auto validation = validate_key_path(path, allowed_base);
        if (validation.is_err()) {
            return validation;
        }

        // 2. Ensure parent directory exists
        try {
            auto parent = path.parent_path();
            if (!parent.empty() && !std::filesystem::exists(parent)) {
                std::filesystem::create_directories(parent);
            }
        } catch (const std::filesystem::filesystem_error& e) {
            return make_logger_void_result(
                logger_error_code::file_write_failed,
                std::string("Failed to create key directory: ") + e.what()
            );
        }

        // 3. Write key to file
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file) {
            return make_logger_void_result(
                logger_error_code::file_write_failed,
                "Failed to open key file for writing"
            );
        }

        file.write(
            reinterpret_cast<const char*>(key.data().data()),
            key.size()
        );

        if (file.fail()) {
            return make_logger_void_result(
                logger_error_code::file_write_failed,
                "Failed to write key data"
            );
        }

        file.close();

        // 4. Set secure file permissions (0600 = owner read/write only)
        try {
            std::filesystem::permissions(
                path,
                std::filesystem::perms::owner_read |
                std::filesystem::perms::owner_write,
                std::filesystem::perm_options::replace
            );
        } catch (const std::filesystem::filesystem_error& e) {
            // Permission setting failed, delete the insecure file
            std::filesystem::remove(path);
            return make_logger_void_result(
                logger_error_code::file_permission_denied,
                std::string("Failed to set secure permissions: ") + e.what()
            );
        }

        return common::ok();
    }

    /**
     * @brief Load key from file with permission verification
     * @param path File path
     * @param expected_size Expected key size (default: 32 for AES-256)
     * @param allowed_base Base directory for key storage
     * @return Result containing the loaded key or error
     */
    static result<secure_key> load_key(
        const std::filesystem::path& path,
        size_t expected_size = 32,
        const std::filesystem::path& allowed_base = "/var/log/keys"
    ) {
        // 1. Validate path
        auto validation = validate_key_path(path, allowed_base);
        if (validation.is_err()) {
            return make_logger_error<secure_key>(
                get_logger_error_code(validation),
                get_logger_error_message(validation)
            );
        }

        // 2. Check if file exists
        if (!std::filesystem::exists(path)) {
            return make_logger_error<secure_key>(
                logger_error_code::file_open_failed,
                "Key file does not exist"
            );
        }

        // 3. Verify file permissions (must not be readable by group/others)
        try {
            auto status = std::filesystem::status(path);
            auto perms = status.permissions();

            if ((perms & std::filesystem::perms::group_read) != std::filesystem::perms::none ||
                (perms & std::filesystem::perms::others_read) != std::filesystem::perms::none) {
                return make_logger_error<secure_key>(
                    logger_error_code::insecure_permissions,
                    "Key file permissions are too permissive (must be 0600 or stricter)"
                );
            }
        } catch (const std::filesystem::filesystem_error& e) {
            return make_logger_error<secure_key>(
                logger_error_code::file_permission_denied,
                std::string("Failed to check file permissions: ") + e.what()
            );
        }

        // 4. Verify file size
        size_t file_size = 0;
        try {
            file_size = std::filesystem::file_size(path);
        } catch (const std::filesystem::filesystem_error& e) {
            return make_logger_error<secure_key>(
                logger_error_code::file_read_failed,
                std::string("Failed to get file size: ") + e.what()
            );
        }

        if (file_size != expected_size) {
            return make_logger_error<secure_key>(
                logger_error_code::invalid_key_size,
                "Invalid key file size (expected " + std::to_string(expected_size) +
                " bytes, got " + std::to_string(file_size) + ")"
            );
        }

        // 5. Read key from file
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            return make_logger_error<secure_key>(
                logger_error_code::file_open_failed,
                "Failed to open key file for reading"
            );
        }

        secure_key key(expected_size);
        file.read(
            reinterpret_cast<char*>(key.mutable_data().data()),
            expected_size
        );

        if (file.fail()) {
            return make_logger_error<secure_key>(
                logger_error_code::file_read_failed,
                "Failed to read key data"
            );
        }

        return result<secure_key>(std::move(key));
    }

private:
    /**
     * @brief Validate key file path (prevent path traversal)
     * @param path Path to validate
     * @param allowed_base Base directory that path must be within
     * @return Success or error
     */
    static common::VoidResult validate_key_path(
        const std::filesystem::path& path,
        const std::filesystem::path& allowed_base
    ) {
        try {
            // Convert both paths to absolute canonical form
            auto canonical_path = std::filesystem::weakly_canonical(path);
            auto canonical_base = std::filesystem::weakly_canonical(allowed_base);

            // Check if path starts with allowed_base
            auto [mismatch_iter, _] = std::mismatch(
                canonical_base.begin(), canonical_base.end(),
                canonical_path.begin(), canonical_path.end()
            );

            if (mismatch_iter != canonical_base.end()) {
                return make_logger_void_result(
                    logger_error_code::path_traversal_detected,
                    "Key path must be within allowed directory: " +
                    canonical_base.string()
                );
            }

            return common::ok();
        } catch (const std::filesystem::filesystem_error& e) {
            return make_logger_void_result(
                logger_error_code::path_traversal_detected,
                std::string("Path validation failed: ") + e.what()
            );
        }
    }
};

} // namespace kcenon::logger::security
