#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/**
 * @file encrypted_writer.h
 * @brief Encryption wrapper for log writers providing AES-256-GCM encryption
 * @author 🍀☀🌕🌥 🌊
 * @since 1.3.0
 *
 * @details This file provides an encrypted_writer class that wraps any existing
 * writer and encrypts log data before writing. Essential for compliance with
 * GDPR, HIPAA, PCI DSS, and SOC 2 regulations.
 *
 * Security features:
 * - AES-256-GCM authenticated encryption
 * - Unique IV per log entry
 * - Secure key management via secure_key_storage
 * - Key rotation support
 * - RAII-based key cleanup
 *
 * @example Basic usage:
 * @code
 * auto key_result = secure_key_storage::generate_key(32);
 * if (key_result.has_value()) {
 *     encryption_config config{
 *         .algorithm = encryption_algorithm::aes_256_gcm,
 *         .key = std::move(key_result.value())
 *     };
 *
 *     auto logger = logger_builder()
 *         .add_writer("encrypted_file",
 *             std::make_unique<encrypted_writer>(
 *                 std::make_unique<file_writer>("secure.log.enc"),
 *                 std::move(config)
 *             ))
 *         .build();
 * }
 * @endcode
 */

#include "base_writer.h"
#include "../interfaces/writer_category.h"
#include "../security/secure_key_storage.h"
#include "../core/error_codes.h"

#include <array>
#include <memory>
#include <mutex>
#include <atomic>
#include <chrono>
#include <optional>
#include <vector>
#include <cstdint>
#include <functional>

// OpenSSL for AES-GCM encryption
#if __has_include(<openssl/evp.h>)
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>
#define LOGGER_HAS_OPENSSL_CRYPTO 1
#endif

namespace kcenon::logger {

/**
 * @enum encryption_algorithm
 * @brief Supported encryption algorithms for log encryption
 *
 * @note AES-256-GCM is recommended as it provides both confidentiality
 * and authenticity (AEAD - Authenticated Encryption with Associated Data).
 */
enum class encryption_algorithm {
    aes_256_gcm,       ///< AES-256 in GCM mode (recommended)
    aes_256_cbc,       ///< AES-256 in CBC mode (legacy support)
    chacha20_poly1305  ///< ChaCha20-Poly1305 (modern alternative)
};

/**
 * @struct encryption_config
 * @brief Configuration for encrypted_writer
 *
 * @details Contains all parameters needed for log encryption including
 * the encryption algorithm, key, and optional key rotation settings.
 */
struct encryption_config {
    /**
     * @brief Encryption algorithm to use
     * @note Default: AES-256-GCM (authenticated encryption)
     */
    encryption_algorithm algorithm = encryption_algorithm::aes_256_gcm;

    /**
     * @brief Encryption key (must be 32 bytes for AES-256)
     * @warning Key is moved into the writer; original becomes invalid
     */
    security::secure_key key;

    /**
     * @brief Whether to generate unique IV for each log entry
     * @note Default: true (highly recommended for security)
     */
    bool rotate_iv_per_entry = true;

    /**
     * @brief Optional key rotation interval
     * @note When set, triggers key rotation after specified duration
     */
    std::optional<std::chrono::hours> key_rotation_interval;

    /**
     * @brief Path for key rotation (new keys saved here)
     * @note Required if key_rotation_interval is set
     */
    std::filesystem::path key_rotation_path;

    /**
     * @brief Allowed base directory for key storage
     * @note Used for path traversal prevention
     */
    std::filesystem::path key_storage_base = "/var/log/keys";

    /**
     * @brief Constructor for encryption_config
     * @param algo Encryption algorithm
     * @param encryption_key The encryption key (moved)
     */
    encryption_config(
        encryption_algorithm algo,
        security::secure_key encryption_key
    ) : algorithm(algo), key(std::move(encryption_key)) {}

    // Allow default construction with explicit key setting
    encryption_config() : key(32) {}

    // Move-only semantics (secure_key is move-only)
    encryption_config(encryption_config&&) noexcept = default;
    encryption_config& operator=(encryption_config&&) noexcept = default;
    encryption_config(const encryption_config&) = delete;
    encryption_config& operator=(const encryption_config&) = delete;
};

/**
 * @struct encrypted_log_header
 * @brief Header prepended to each encrypted log entry
 *
 * @details Contains metadata needed for decryption including IV,
 * authentication tag, and original data length.
 */
struct encrypted_log_header {
    static constexpr uint32_t kMagic = 0x454E4352;  // "ENCR"
    static constexpr uint8_t kVersion = 1;
    static constexpr size_t kIvSize = 16;
    static constexpr size_t kTagSize = 16;

    uint32_t magic = kMagic;          ///< Magic number for validation
    uint8_t version = kVersion;       ///< Header format version
    uint8_t algorithm = 0;            ///< Encryption algorithm used
    uint16_t reserved = 0;            ///< Reserved for future use
    uint32_t original_length = 0;     ///< Length of plaintext
    uint32_t encrypted_length = 0;    ///< Length of ciphertext
    std::array<uint8_t, kIvSize> iv{};   ///< Initialization vector
    std::array<uint8_t, kTagSize> tag{}; ///< GCM authentication tag
};

/**
 * @class encrypted_writer
 * @brief Decorator that encrypts log data before writing
 *
 * @details Wraps any base_writer and transparently encrypts all log data
 * using AES-256-GCM before passing to the inner writer. Provides:
 * - Per-entry IV rotation for semantic security
 * - Authenticated encryption to prevent tampering
 * - Optional key rotation
 * - Secure key cleanup on destruction
 *
 * Thread safety: Thread-safe when inner_writer is thread-safe.
 *
 * Category: Decorator (wraps another writer to add encryption)
 *
 * @since 1.3.0
 * @since 1.4.0 Added decorator_writer_tag for category classification
 */
class encrypted_writer : public base_writer, public decorator_writer_tag {
public:
    /**
     * @brief Construct encrypted writer with output file path
     * @param output_path Path to encrypted log file (binary output)
     * @param config Encryption configuration (moved)
     * @throws std::invalid_argument if key is invalid
     * @throws std::runtime_error if file cannot be opened
     *
     * @note This constructor writes encrypted binary data directly to file.
     * This is the recommended constructor for encrypted logging.
     */
    encrypted_writer(
        const std::filesystem::path& output_path,
        encryption_config config
    );

    /**
     * @brief Construct encrypted writer with inner writer and config
     * @param inner_writer Writer to wrap (encrypts before passing data)
     * @param config Encryption configuration (moved)
     * @throws std::invalid_argument if inner_writer is null or key is invalid
     *
     * @warning When using this constructor, the inner_writer receives
     * formatted log entries with encrypted binary data in the message field.
     * For proper binary output, use the file path constructor instead.
     */
    encrypted_writer(
        std::unique_ptr<base_writer> inner_writer,
        encryption_config config
    );

    /**
     * @brief Destructor - securely cleans up key material
     */
    ~encrypted_writer() override;

    // Non-copyable and non-movable (mutex cannot be moved)
    encrypted_writer(const encrypted_writer&) = delete;
    encrypted_writer& operator=(const encrypted_writer&) = delete;
    encrypted_writer(encrypted_writer&&) = delete;
    encrypted_writer& operator=(encrypted_writer&&) = delete;

    /**
     * @brief Write encrypted log entry
     * @param entry The log entry to write
     * @return common::VoidResult Success or error code
     * @since 3.5.0 Changed to use log_entry directly
     */
    common::VoidResult write(const log_entry& entry) override;

    /**
     * @brief Flush underlying writer
     * @return common::VoidResult Success or error code
     */
    common::VoidResult flush() override;

    /**
     * @brief Get writer name
     * @return "encrypted" + inner writer name
     */
    std::string get_name() const override;

    /**
     * @brief Check if writer is healthy
     * @return true if encryption context and inner writer are operational
     */
    bool is_healthy() const override;

    /**
     * @brief Rotate encryption key
     * @param new_key New key to use (moved)
     * @return common::VoidResult Success or error code
     *
     * @details Safely rotates the encryption key:
     * 1. Validates new key
     * 2. Flushes pending writes
     * 3. Swaps key atomically
     * 4. Securely clears old key
     * 5. Optionally logs to audit log
     */
    common::VoidResult rotate_key(security::secure_key new_key);

    /**
     * @brief Get encryption statistics
     * @return Number of log entries encrypted since creation
     */
    uint64_t get_entries_encrypted() const;

    /**
     * @brief Get time of last key rotation
     * @return Timestamp of last key rotation (or construction time)
     */
    std::chrono::system_clock::time_point get_last_key_rotation() const;

    /**
     * @brief Decrypt a single encrypted log entry
     * @param encrypted_data Raw encrypted data including header
     * @param key Decryption key
     * @return Result containing decrypted string or error
     *
     * @note Static utility method for log decryption tools
     */
    static result<std::string> decrypt_entry(
        const std::vector<uint8_t>& encrypted_data,
        const security::secure_key& key
    );

private:
    /**
     * @brief Encrypt plaintext using configured algorithm
     * @param plaintext Data to encrypt
     * @param output Buffer to receive encrypted data with header
     * @return common::VoidResult Success or error code
     */
    common::VoidResult encrypt_data(
        const std::string& plaintext,
        std::vector<uint8_t>& output
    );

    /**
     * @brief Generate secure random IV
     * @param iv Buffer to receive IV (must be 16 bytes)
     * @return common::VoidResult Success or error code
     */
    common::VoidResult generate_iv(uint8_t* iv);

    /**
     * @brief Check if key rotation is needed
     * @return true if rotation interval has elapsed
     */
    bool should_rotate_key() const;

    /**
     * @brief Perform automatic key rotation if configured
     * @return common::VoidResult Success or error code
     */
    common::VoidResult auto_rotate_key_if_needed();

#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    /**
     * @brief Initialize OpenSSL cipher context
     * @return common::VoidResult Success or error code
     */
    common::VoidResult init_cipher_context();

    /**
     * @brief Cleanup OpenSSL cipher context
     */
    void cleanup_cipher_context();
#endif

    std::unique_ptr<base_writer> inner_writer_;
    std::unique_ptr<std::ofstream> output_file_;
    std::filesystem::path output_path_;
    encryption_config config_;
    mutable std::mutex write_mutex_;

    std::atomic<uint64_t> entries_encrypted_{0};
    std::chrono::system_clock::time_point last_key_rotation_;
    std::atomic<bool> is_initialized_{false};

#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    EVP_CIPHER_CTX* cipher_ctx_ = nullptr;
#endif
};

/**
 * @class log_decryptor
 * @brief Utility class for decrypting encrypted log files
 *
 * @details Provides functionality to read and decrypt log files
 * created by encrypted_writer. Supports streaming decryption
 * for large files.
 *
 * @example
 * @code
 * log_decryptor decryptor(key);
 * auto result = decryptor.decrypt_file("secure.log.enc", "decrypted.log");
 * if (result.has_value()) {
 *     std::cout << "Decrypted " << result.value() << " entries\n";
 * }
 * @endcode
 */
class log_decryptor {
public:
    /**
     * @brief Construct decryptor with key
     * @param key Decryption key (copied for safety)
     */
    explicit log_decryptor(const security::secure_key& key);

    /**
     * @brief Destructor - cleans up key material
     */
    ~log_decryptor();

    // Non-copyable and non-movable
    log_decryptor(const log_decryptor&) = delete;
    log_decryptor& operator=(const log_decryptor&) = delete;
    log_decryptor(log_decryptor&&) = delete;
    log_decryptor& operator=(log_decryptor&&) = delete;

    /**
     * @brief Decrypt entire file to output file
     * @param input_path Encrypted log file path
     * @param output_path Decrypted output file path
     * @return Result containing entry count or error
     */
    result<size_t> decrypt_file(
        const std::filesystem::path& input_path,
        const std::filesystem::path& output_path
    );

    /**
     * @brief Decrypt file with callback for each entry
     * @param input_path Encrypted log file path
     * @param callback Function called with each decrypted entry
     * @return Result containing entry count or error
     */
    result<size_t> decrypt_file_streaming(
        const std::filesystem::path& input_path,
        std::function<void(const std::string&)> callback
    );

private:
    security::secure_key key_;

#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    EVP_CIPHER_CTX* cipher_ctx_ = nullptr;
#endif
};

} // namespace kcenon::logger
