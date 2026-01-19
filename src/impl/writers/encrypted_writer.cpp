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

#include <kcenon/logger/writers/encrypted_writer.h>
#include <kcenon/logger/security/audit_logger.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>

#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstring>

namespace kcenon::logger {

// ============================================================================
// encrypted_writer implementation
// ============================================================================

encrypted_writer::encrypted_writer(
    const std::filesystem::path& output_path,
    encryption_config config
) : output_path_(output_path),
    config_(std::move(config)),
    last_key_rotation_(std::chrono::system_clock::now())
{
    // Validate key size (AES-256 requires 32 bytes)
    if (config_.key.size() != 32) {
        throw std::invalid_argument(
            "Invalid key size: expected 32 bytes, got " +
            std::to_string(config_.key.size())
        );
    }

    // Open output file in binary mode
    output_file_ = std::make_unique<std::ofstream>(
        output_path, std::ios::binary | std::ios::trunc
    );
    if (!output_file_ || !output_file_->is_open()) {
        throw std::runtime_error(
            "Failed to open output file: " + output_path.string()
        );
    }

#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    auto init_result = init_cipher_context();
    if (init_result.is_err()) {
        throw std::runtime_error(
            "Failed to initialize cipher context: " +
            get_logger_error_message(init_result)
        );
    }
#endif

    is_initialized_.store(true);

    // Log initialization to audit log
    security::audit_logger::log_audit_event(
        security::audit_logger::audit_event::encryption_key_loaded,
        "encrypted_writer initialized (direct file mode)",
        {{"algorithm", std::to_string(static_cast<int>(config_.algorithm))},
         {"output_path", output_path.string()}}
    );
}

encrypted_writer::encrypted_writer(
    std::unique_ptr<base_writer> inner_writer,
    encryption_config config
) : inner_writer_(std::move(inner_writer)),
    config_(std::move(config)),
    last_key_rotation_(std::chrono::system_clock::now())
{
    if (!inner_writer_) {
        throw std::invalid_argument("inner_writer cannot be null");
    }

    // Validate key size (AES-256 requires 32 bytes)
    if (config_.key.size() != 32) {
        throw std::invalid_argument(
            "Invalid key size: expected 32 bytes, got " +
            std::to_string(config_.key.size())
        );
    }

#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    auto init_result = init_cipher_context();
    if (init_result.is_err()) {
        throw std::runtime_error(
            "Failed to initialize cipher context: " +
            get_logger_error_message(init_result)
        );
    }
#endif

    is_initialized_.store(true);

    // Log initialization to audit log
    security::audit_logger::log_audit_event(
        security::audit_logger::audit_event::encryption_key_loaded,
        "encrypted_writer initialized (decorator mode)",
        {{"algorithm", std::to_string(static_cast<int>(config_.algorithm))},
         {"inner_writer", inner_writer_->get_name()}}
    );
}

encrypted_writer::~encrypted_writer() {
    is_initialized_.store(false);

#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    cleanup_cipher_context();
#endif

    // Flush and close output file if using direct mode
    if (output_file_ && output_file_->is_open()) {
        output_file_->flush();
        output_file_->close();
    }

    // Flush inner writer if using decorator mode
    if (inner_writer_) {
        inner_writer_->flush();
    }
}

common::VoidResult encrypted_writer::write(
    common::interfaces::log_level level,
    const std::string& message,
    const std::string& file,
    int line,
    const std::string& function,
    const std::chrono::system_clock::time_point& timestamp
) {
    if (!is_initialized_.load()) {
        return make_logger_void_result(
            logger_error_code::encryption_failed,
            "encrypted_writer not initialized"
        );
    }

    // Check for automatic key rotation
    auto rotate_result = auto_rotate_key_if_needed();
    if (rotate_result.is_err()) {
        // Log rotation failure but continue with current key
        security::audit_logger::log_audit_event(
            security::audit_logger::audit_event::suspicious_activity,
            "Key rotation failed",
            {{"error", get_logger_error_message(rotate_result)}}
        );
    }

    // Format the log entry first
    std::ostringstream oss;
    auto time_t_val = std::chrono::system_clock::to_time_t(timestamp);
    std::tm tm_val;
#ifdef _WIN32
    localtime_s(&tm_val, &time_t_val);
#else
    localtime_r(&time_t_val, &tm_val);
#endif

    oss << std::put_time(&tm_val, "%Y-%m-%dT%H:%M:%S")
        << " [" << static_cast<int>(level) << "] "
        << message;

    if (!file.empty()) {
        oss << " (" << file << ":" << line << " " << function << ")";
    }

    std::string plaintext = oss.str();

    // Encrypt the formatted log entry
    std::vector<uint8_t> encrypted_data;
    {
        std::lock_guard lock(write_mutex_);
        auto encrypt_result = encrypt_data(plaintext, encrypted_data);
        if (encrypt_result.is_err()) {
            return encrypt_result;
        }

        // Write to direct file output if available
        if (output_file_ && output_file_->is_open()) {
            output_file_->write(
                reinterpret_cast<const char*>(encrypted_data.data()),
                static_cast<std::streamsize>(encrypted_data.size())
            );
            if (!output_file_->good()) {
                return make_logger_void_result(
                    logger_error_code::file_write_failed,
                    "Failed to write encrypted data to file"
                );
            }
            entries_encrypted_.fetch_add(1);
            return common::ok();
        }
    }

    // Fallback to inner writer (decorator mode)
    if (inner_writer_) {
        std::string binary_data(
            reinterpret_cast<const char*>(encrypted_data.data()),
            encrypted_data.size()
        );
        log_entry entry(level, binary_data, timestamp);

        auto write_result = inner_writer_->write(entry);
        if (write_result.is_ok()) {
            entries_encrypted_.fetch_add(1);
        }
        return write_result;
    }

    return make_logger_void_result(
        logger_error_code::writer_not_available,
        "No output target available"
    );
}

common::VoidResult encrypted_writer::flush() {
    std::lock_guard lock(write_mutex_);

    if (output_file_ && output_file_->is_open()) {
        output_file_->flush();
        if (!output_file_->good()) {
            return make_logger_void_result(
                logger_error_code::file_write_failed,
                "Failed to flush encrypted file"
            );
        }
        return common::ok();
    }

    if (inner_writer_) {
        return inner_writer_->flush();
    }

    return make_logger_void_result(
        logger_error_code::writer_not_available,
        "No output target available"
    );
}

std::string encrypted_writer::get_name() const {
    if (!output_path_.empty()) {
        return "encrypted_file(" + output_path_.filename().string() + ")";
    }
    if (inner_writer_) {
        return "encrypted_" + inner_writer_->get_name();
    }
    return "encrypted_writer";
}

bool encrypted_writer::is_healthy() const {
    if (!is_initialized_.load()) {
        return false;
    }

    // Direct file mode
    if (output_file_) {
        return output_file_->is_open() && output_file_->good();
    }

    // Decorator mode
    if (inner_writer_) {
        return inner_writer_->is_healthy();
    }

    return false;
}

common::VoidResult encrypted_writer::rotate_key(security::secure_key new_key) {
    // Validate new key
    if (new_key.size() != 32) {
        return make_logger_void_result(
            logger_error_code::invalid_key_size,
            "New key must be 32 bytes for AES-256"
        );
    }

    std::lock_guard lock(write_mutex_);

    // Flush pending writes with old key
    if (inner_writer_) {
        inner_writer_->flush();
    }

    // Swap keys (old key is securely cleared by secure_key destructor)
    config_.key = std::move(new_key);
    last_key_rotation_ = std::chrono::system_clock::now();

    // Save new key if path is configured
    if (!config_.key_rotation_path.empty()) {
        auto save_result = security::secure_key_storage::save_key(
            config_.key,
            config_.key_rotation_path,
            config_.key_storage_base
        );
        if (save_result.is_err()) {
            return save_result;
        }
    }

    // Log key rotation
    security::audit_logger::log_audit_event(
        security::audit_logger::audit_event::encryption_key_rotated,
        "Encryption key rotated",
        {{"entries_encrypted", std::to_string(entries_encrypted_.load())}}
    );

    return common::ok();
}

uint64_t encrypted_writer::get_entries_encrypted() const {
    return entries_encrypted_.load();
}

std::chrono::system_clock::time_point encrypted_writer::get_last_key_rotation() const {
    return last_key_rotation_;
}

result<std::string> encrypted_writer::decrypt_entry(
    const std::vector<uint8_t>& encrypted_data,
    const security::secure_key& key
) {
#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    // Validate minimum size (header + at least 1 byte of data)
    if (encrypted_data.size() < sizeof(encrypted_log_header)) {
        return result<std::string>(
            logger_error_code::decryption_failed,
            "Encrypted data too small"
        );
    }

    // Parse header
    encrypted_log_header header;
    std::memcpy(&header, encrypted_data.data(), sizeof(header));

    // Validate magic number
    if (header.magic != encrypted_log_header::kMagic) {
        return result<std::string>(
            logger_error_code::decryption_failed,
            "Invalid magic number in encrypted data"
        );
    }

    // Validate version
    if (header.version != encrypted_log_header::kVersion) {
        return result<std::string>(
            logger_error_code::decryption_failed,
            "Unsupported encrypted log version"
        );
    }

    // Validate data length
    size_t expected_size = sizeof(header) + header.encrypted_length;
    if (encrypted_data.size() < expected_size) {
        return result<std::string>(
            logger_error_code::decryption_failed,
            "Encrypted data truncated"
        );
    }

    // Initialize decryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return result<std::string>(
            logger_error_code::decryption_failed,
            "Failed to create cipher context"
        );
    }

    // Select cipher based on algorithm
    const EVP_CIPHER* cipher = nullptr;
    switch (static_cast<encryption_algorithm>(header.algorithm)) {
        case encryption_algorithm::aes_256_gcm:
            cipher = EVP_aes_256_gcm();
            break;
        case encryption_algorithm::aes_256_cbc:
            cipher = EVP_aes_256_cbc();
            break;
        case encryption_algorithm::chacha20_poly1305:
            cipher = EVP_chacha20_poly1305();
            break;
        default:
            EVP_CIPHER_CTX_free(ctx);
            return result<std::string>(
                logger_error_code::decryption_failed,
                "Unknown encryption algorithm"
            );
    }

    // Initialize decryption
    if (EVP_DecryptInit_ex(ctx, cipher, nullptr, nullptr, nullptr) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return result<std::string>(
            logger_error_code::decryption_failed,
            "Failed to initialize decryption"
        );
    }

    // Set IV length for GCM
    if (header.algorithm == static_cast<uint8_t>(encryption_algorithm::aes_256_gcm)) {
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, nullptr) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return result<std::string>(
                logger_error_code::decryption_failed,
                "Failed to set IV length"
            );
        }
    }

    // Set key and IV
    if (EVP_DecryptInit_ex(ctx, nullptr, nullptr,
                          key.data().data(), header.iv.data()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return result<std::string>(
            logger_error_code::decryption_failed,
            "Failed to set key and IV"
        );
    }

    // Set expected tag for GCM
    if (header.algorithm == static_cast<uint8_t>(encryption_algorithm::aes_256_gcm)) {
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16,
                               const_cast<uint8_t*>(header.tag.data())) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return result<std::string>(
                logger_error_code::decryption_failed,
                "Failed to set authentication tag"
            );
        }
    }

    // Decrypt data
    std::vector<uint8_t> plaintext(header.original_length + 16);
    int plaintext_len = 0;

    const uint8_t* ciphertext = encrypted_data.data() + sizeof(header);
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &plaintext_len,
                         ciphertext, header.encrypted_length) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return result<std::string>(
            logger_error_code::decryption_failed,
            "Decryption failed"
        );
    }

    int final_len = 0;
    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + plaintext_len, &final_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return result<std::string>(
            logger_error_code::decryption_failed,
            "Decryption finalization failed (authentication failure)"
        );
    }

    EVP_CIPHER_CTX_free(ctx);

    plaintext_len += final_len;
    return result<std::string>(std::string(
        reinterpret_cast<char*>(plaintext.data()),
        plaintext_len
    ));
#else
    return result<std::string>(
        logger_error_code::decryption_failed,
        "OpenSSL not available - decryption disabled"
    );
#endif
}

common::VoidResult encrypted_writer::encrypt_data(
    const std::string& plaintext,
    std::vector<uint8_t>& output
) {
#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    // Generate IV
    uint8_t iv[16];
    auto iv_result = generate_iv(iv);
    if (iv_result.is_err()) {
        return iv_result;
    }

    // Prepare header
    encrypted_log_header header;
    header.algorithm = static_cast<uint8_t>(config_.algorithm);
    header.original_length = static_cast<uint32_t>(plaintext.size());
    std::memcpy(header.iv.data(), iv, encrypted_log_header::kIvSize);

    // Initialize encryption
    if (EVP_EncryptInit_ex(cipher_ctx_, nullptr, nullptr,
                          config_.key.data().data(), iv) != 1) {
        return make_logger_void_result(
            logger_error_code::encryption_failed,
            "Failed to initialize encryption"
        );
    }

    // Allocate output buffer (header + ciphertext + potential padding)
    size_t max_ciphertext_len = plaintext.size() + EVP_MAX_BLOCK_LENGTH;
    output.resize(sizeof(header) + max_ciphertext_len);

    // Encrypt
    int ciphertext_len = 0;
    if (EVP_EncryptUpdate(cipher_ctx_,
                         output.data() + sizeof(header),
                         &ciphertext_len,
                         reinterpret_cast<const uint8_t*>(plaintext.data()),
                         static_cast<int>(plaintext.size())) != 1) {
        return make_logger_void_result(
            logger_error_code::encryption_failed,
            "Encryption update failed"
        );
    }

    int final_len = 0;
    if (EVP_EncryptFinal_ex(cipher_ctx_,
                           output.data() + sizeof(header) + ciphertext_len,
                           &final_len) != 1) {
        return make_logger_void_result(
            logger_error_code::encryption_failed,
            "Encryption finalization failed"
        );
    }

    ciphertext_len += final_len;
    header.encrypted_length = static_cast<uint32_t>(ciphertext_len);

    // Get authentication tag for GCM
    if (config_.algorithm == encryption_algorithm::aes_256_gcm) {
        if (EVP_CIPHER_CTX_ctrl(cipher_ctx_, EVP_CTRL_GCM_GET_TAG,
                               encrypted_log_header::kTagSize, header.tag.data()) != 1) {
            return make_logger_void_result(
                logger_error_code::encryption_failed,
                "Failed to get authentication tag"
            );
        }
    }

    // Copy header to output
    std::memcpy(output.data(), &header, sizeof(header));

    // Resize to actual size
    output.resize(sizeof(header) + ciphertext_len);

    return common::ok();
#else
    return make_logger_void_result(
        logger_error_code::encryption_failed,
        "OpenSSL not available - encryption disabled"
    );
#endif
}

common::VoidResult encrypted_writer::generate_iv(uint8_t* iv) {
#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    if (RAND_bytes(iv, 16) != 1) {
        return make_logger_void_result(
            logger_error_code::encryption_failed,
            "Failed to generate random IV"
        );
    }
    return common::ok();
#else
    return make_logger_void_result(
        logger_error_code::encryption_failed,
        "OpenSSL not available"
    );
#endif
}

bool encrypted_writer::should_rotate_key() const {
    if (!config_.key_rotation_interval.has_value()) {
        return false;
    }

    auto now = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::hours>(
        now - last_key_rotation_
    );

    return elapsed >= config_.key_rotation_interval.value();
}

common::VoidResult encrypted_writer::auto_rotate_key_if_needed() {
    if (!should_rotate_key()) {
        return common::ok();
    }

    // Generate new key
    auto key_result = security::secure_key_storage::generate_key(32);
    if (!key_result.has_value()) {
        return make_logger_void_result(
            key_result.error_code(),
            key_result.error_message()
        );
    }

    return rotate_key(std::move(key_result.value()));
}

#ifdef LOGGER_HAS_OPENSSL_CRYPTO
common::VoidResult encrypted_writer::init_cipher_context() {
    cipher_ctx_ = EVP_CIPHER_CTX_new();
    if (!cipher_ctx_) {
        return make_logger_void_result(
            logger_error_code::encryption_failed,
            "Failed to create cipher context"
        );
    }

    // Select cipher based on algorithm
    const EVP_CIPHER* cipher = nullptr;
    switch (config_.algorithm) {
        case encryption_algorithm::aes_256_gcm:
            cipher = EVP_aes_256_gcm();
            break;
        case encryption_algorithm::aes_256_cbc:
            cipher = EVP_aes_256_cbc();
            break;
        case encryption_algorithm::chacha20_poly1305:
            cipher = EVP_chacha20_poly1305();
            break;
        default:
            EVP_CIPHER_CTX_free(cipher_ctx_);
            cipher_ctx_ = nullptr;
            return make_logger_void_result(
                logger_error_code::encryption_failed,
                "Unknown encryption algorithm"
            );
    }

    // Initialize cipher
    if (EVP_EncryptInit_ex(cipher_ctx_, cipher, nullptr, nullptr, nullptr) != 1) {
        EVP_CIPHER_CTX_free(cipher_ctx_);
        cipher_ctx_ = nullptr;
        return make_logger_void_result(
            logger_error_code::encryption_failed,
            "Failed to initialize cipher"
        );
    }

    // Set IV length for GCM
    if (config_.algorithm == encryption_algorithm::aes_256_gcm) {
        if (EVP_CIPHER_CTX_ctrl(cipher_ctx_, EVP_CTRL_GCM_SET_IVLEN, 16, nullptr) != 1) {
            EVP_CIPHER_CTX_free(cipher_ctx_);
            cipher_ctx_ = nullptr;
            return make_logger_void_result(
                logger_error_code::encryption_failed,
                "Failed to set IV length"
            );
        }
    }

    return common::ok();
}

void encrypted_writer::cleanup_cipher_context() {
    if (cipher_ctx_) {
        EVP_CIPHER_CTX_free(cipher_ctx_);
        cipher_ctx_ = nullptr;
    }
}
#endif

// ============================================================================
// log_decryptor implementation
// ============================================================================

log_decryptor::log_decryptor(const security::secure_key& key)
    : key_(key.data())  // Copy key data
{
#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    cipher_ctx_ = EVP_CIPHER_CTX_new();
#endif
}

log_decryptor::~log_decryptor() {
#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    if (cipher_ctx_) {
        EVP_CIPHER_CTX_free(cipher_ctx_);
        cipher_ctx_ = nullptr;
    }
#endif
}

result<size_t> log_decryptor::decrypt_file(
    const std::filesystem::path& input_path,
    const std::filesystem::path& output_path
) {
    std::ofstream output(output_path, std::ios::binary | std::ios::trunc);
    if (!output) {
        return result<size_t>(
            logger_error_code::file_open_failed,
            "Failed to open output file: " + output_path.string()
        );
    }

    size_t entry_count = 0;
    auto callback = [&output, &entry_count](const std::string& decrypted) {
        output << decrypted << "\n";
        ++entry_count;
    };

    auto stream_result = decrypt_file_streaming(input_path, callback);
    if (!stream_result.has_value()) {
        return stream_result;
    }

    return result<size_t>(entry_count);
}

result<size_t> log_decryptor::decrypt_file_streaming(
    const std::filesystem::path& input_path,
    std::function<void(const std::string&)> callback
) {
#ifdef LOGGER_HAS_OPENSSL_CRYPTO
    std::ifstream input(input_path, std::ios::binary);
    if (!input) {
        return result<size_t>(
            logger_error_code::file_open_failed,
            "Failed to open input file: " + input_path.string()
        );
    }

    size_t entry_count = 0;
    std::vector<uint8_t> buffer;

    while (input.good()) {
        // Read header
        encrypted_log_header header;
        input.read(reinterpret_cast<char*>(&header), sizeof(header));

        if (input.gcount() == 0) {
            break;  // End of file
        }

        if (input.gcount() != sizeof(header)) {
            return result<size_t>(
                logger_error_code::file_read_failed,
                "Incomplete header at entry " + std::to_string(entry_count)
            );
        }

        // Validate header
        if (header.magic != encrypted_log_header::kMagic) {
            return result<size_t>(
                logger_error_code::decryption_failed,
                "Invalid magic number at entry " + std::to_string(entry_count)
            );
        }

        // Read encrypted data
        buffer.resize(sizeof(header) + header.encrypted_length);
        std::memcpy(buffer.data(), &header, sizeof(header));
        input.read(
            reinterpret_cast<char*>(buffer.data() + sizeof(header)),
            header.encrypted_length
        );

        if (static_cast<size_t>(input.gcount()) != header.encrypted_length) {
            return result<size_t>(
                logger_error_code::file_read_failed,
                "Incomplete data at entry " + std::to_string(entry_count)
            );
        }

        // Decrypt entry
        auto decrypt_result = encrypted_writer::decrypt_entry(buffer, key_);
        if (!decrypt_result.has_value()) {
            return result<size_t>(
                decrypt_result.error_code(),
                "Decryption failed at entry " + std::to_string(entry_count) +
                ": " + decrypt_result.error_message()
            );
        }

        callback(decrypt_result.value());
        ++entry_count;
    }

    return result<size_t>(entry_count);
#else
    return result<size_t>(
        logger_error_code::decryption_failed,
        "OpenSSL not available - decryption disabled"
    );
#endif
}

} // namespace kcenon::logger
