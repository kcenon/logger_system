/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>
#include <kcenon/logger/writers/encrypted_writer.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/security/secure_key_storage.h>

#include <filesystem>
#include <fstream>
#include <thread>
#include <vector>

namespace kcenon::logger::test {

class EncryptedWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create temporary directory for test files
        test_dir_ = std::filesystem::temp_directory_path() / "encrypted_writer_test";
        std::filesystem::create_directories(test_dir_);

        // Generate test key
        auto key_result = security::secure_key_storage::generate_key(32);
        ASSERT_TRUE(key_result.has_value()) << "Failed to generate test key";
        test_key_ = std::make_unique<security::secure_key>(std::move(key_result.value()));
    }

    void TearDown() override {
        // Cleanup test directory
        std::error_code ec;
        std::filesystem::remove_all(test_dir_, ec);
    }

    std::filesystem::path test_dir_;
    std::unique_ptr<security::secure_key> test_key_;
};

#ifdef LOGGER_HAS_OPENSSL_CRYPTO

TEST_F(EncryptedWriterTest, ConstructWithValidConfig) {
    auto log_path = test_dir_ / "test.log.enc";

    // Create key copy for config
    security::secure_key key_copy(test_key_->data());

    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_copy)
    );

    auto inner_writer = std::make_unique<file_writer>(log_path.string());

    EXPECT_NO_THROW({
        encrypted_writer writer(std::move(inner_writer), std::move(config));
        EXPECT_TRUE(writer.is_healthy());
        EXPECT_EQ(writer.get_name(), "encrypted_file");
    });
}

TEST_F(EncryptedWriterTest, ThrowsOnNullInnerWriter) {
    security::secure_key key_copy(test_key_->data());
    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_copy)
    );

    EXPECT_THROW(
        encrypted_writer(nullptr, std::move(config)),
        std::invalid_argument
    );
}

TEST_F(EncryptedWriterTest, ThrowsOnInvalidKeySize) {
    auto log_path = test_dir_ / "test.log.enc";

    // Create key with wrong size (16 bytes instead of 32)
    security::secure_key invalid_key(16);
    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(invalid_key)
    );

    auto inner_writer = std::make_unique<file_writer>(log_path.string());

    EXPECT_THROW(
        encrypted_writer(std::move(inner_writer), std::move(config)),
        std::invalid_argument
    );
}

TEST_F(EncryptedWriterTest, WriteAndDecryptSingleEntry) {
    auto log_path = test_dir_ / "single.log.enc";

    // Create writer using decorator pattern (wrapping file_writer)
    security::secure_key key_copy(test_key_->data());
    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_copy)
    );

    {
        encrypted_writer writer(
            std::make_unique<file_writer>(log_path.string()),
            std::move(config)
        );

        log_entry entry(kcenon::common::interfaces::log_level::info,
                        "Test encrypted message",
                        "test.cpp",
                        42,
                        "TestFunction",
                        std::chrono::system_clock::now());
        auto result = writer.write(entry);

        EXPECT_TRUE(result.is_ok()) << "Write failed: " << get_logger_error_message(result);
        EXPECT_EQ(writer.get_entries_encrypted(), 1);

        writer.flush();
    }

    // Verify file was created and is not plaintext
    ASSERT_TRUE(std::filesystem::exists(log_path));
    std::ifstream file(log_path, std::ios::binary);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    // Check magic number in encrypted file
    EXPECT_GE(content.size(), sizeof(encrypted_log_header));
    encrypted_log_header header;
    std::memcpy(&header, content.data(), sizeof(header));
    EXPECT_EQ(header.magic, encrypted_log_header::kMagic);
    EXPECT_EQ(header.version, encrypted_log_header::kVersion);

    // Verify content is not plaintext
    EXPECT_EQ(content.find("Test encrypted message"), std::string::npos);

    // Decrypt and verify
    std::vector<uint8_t> encrypted_data(content.begin(), content.end());
    auto decrypt_result = encrypted_writer::decrypt_entry(encrypted_data, *test_key_);

    ASSERT_TRUE(decrypt_result.has_value())
        << "Decryption failed: " << decrypt_result.error_message();

    std::string decrypted = decrypt_result.value();
    EXPECT_NE(decrypted.find("Test encrypted message"), std::string::npos);
    EXPECT_NE(decrypted.find("test.cpp"), std::string::npos);
}

TEST_F(EncryptedWriterTest, WriteMultipleEntries) {
    auto log_path = test_dir_ / "multiple.log.enc";
    constexpr int num_entries = 100;

    security::secure_key key_copy(test_key_->data());
    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_copy)
    );

    {
        encrypted_writer writer(
            std::make_unique<file_writer>(log_path.string()),
            std::move(config)
        );

        for (int i = 0; i < num_entries; ++i) {
            log_entry entry(kcenon::common::interfaces::log_level::debug,
                           "Entry number " + std::to_string(i),
                           std::chrono::system_clock::now());
            auto result = writer.write(entry);
            EXPECT_TRUE(result.is_ok());
        }

        EXPECT_EQ(writer.get_entries_encrypted(), num_entries);
        writer.flush();
    }

    // Decrypt using log_decryptor
    auto decrypted_path = test_dir_ / "decrypted.log";
    log_decryptor decryptor(*test_key_);

    auto result = decryptor.decrypt_file(log_path, decrypted_path);
    ASSERT_TRUE(result.has_value())
        << "File decryption failed: " << result.error_message();

    EXPECT_EQ(result.value(), num_entries);

    // Verify decrypted content
    std::ifstream decrypted_file(decrypted_path);
    std::string line;
    int count = 0;
    while (std::getline(decrypted_file, line)) {
        if (!line.empty()) {
            EXPECT_NE(line.find("Entry number"), std::string::npos);
            ++count;
        }
    }
    EXPECT_EQ(count, num_entries);
}

TEST_F(EncryptedWriterTest, KeyRotation) {
    auto log_path = test_dir_ / "rotation.log.enc";

    security::secure_key key_copy(test_key_->data());
    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_copy)
    );

    encrypted_writer writer(
        std::make_unique<file_writer>(log_path.string()),
        std::move(config)
    );

    // Write with first key
    {
        log_entry entry(kcenon::common::interfaces::log_level::info,
                       "Before rotation",
                       std::chrono::system_clock::now());
        writer.write(entry);
    }

    auto first_rotation_time = writer.get_last_key_rotation();

    // Generate new key and rotate
    auto new_key_result = security::secure_key_storage::generate_key(32);
    ASSERT_TRUE(new_key_result.has_value());

    // Small delay to ensure timestamp difference
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    auto rotate_result = writer.rotate_key(std::move(new_key_result.value()));
    EXPECT_TRUE(rotate_result.is_ok())
        << "Key rotation failed: " << get_logger_error_message(rotate_result);

    EXPECT_GT(writer.get_last_key_rotation(), first_rotation_time);

    // Write with new key
    {
        log_entry entry(kcenon::common::interfaces::log_level::info,
                       "After rotation",
                       std::chrono::system_clock::now());
        writer.write(entry);
    }

    EXPECT_EQ(writer.get_entries_encrypted(), 2);
}

TEST_F(EncryptedWriterTest, InvalidKeyRotation) {
    auto log_path = test_dir_ / "invalid_rotation.log.enc";

    security::secure_key key_copy(test_key_->data());
    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_copy)
    );

    encrypted_writer writer(
        std::make_unique<file_writer>(log_path.string()),
        std::move(config)
    );

    // Try to rotate with invalid key size
    security::secure_key invalid_key(16);  // Should be 32
    auto result = writer.rotate_key(std::move(invalid_key));

    EXPECT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result), logger_error_code::invalid_key_size);
}

TEST_F(EncryptedWriterTest, DecryptWithWrongKey) {
    auto log_path = test_dir_ / "wrong_key.log.enc";

    security::secure_key key_copy(test_key_->data());
    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_copy)
    );

    {
        encrypted_writer writer(
            std::make_unique<file_writer>(log_path.string()),
            std::move(config)
        );

        log_entry entry(kcenon::common::interfaces::log_level::info,
                       "Secret message",
                       std::chrono::system_clock::now());
        writer.write(entry);
        writer.flush();
    }

    // Try to decrypt with different key
    auto wrong_key_result = security::secure_key_storage::generate_key(32);
    ASSERT_TRUE(wrong_key_result.has_value());

    std::ifstream file(log_path, std::ios::binary);
    std::vector<uint8_t> encrypted_data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    auto decrypt_result = encrypted_writer::decrypt_entry(
        encrypted_data,
        wrong_key_result.value()
    );

    EXPECT_FALSE(decrypt_result.has_value());
    EXPECT_EQ(decrypt_result.error_code(), logger_error_code::decryption_failed);
}

TEST_F(EncryptedWriterTest, ThreadSafety) {
    auto log_path = test_dir_ / "threadsafe.log.enc";

    security::secure_key key_copy(test_key_->data());
    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_copy)
    );

    // Create writer with decorator pattern
    auto writer = std::make_unique<encrypted_writer>(
        std::make_unique<file_writer>(log_path.string()),
        std::move(config)
    );
    auto* writer_ptr = writer.get();

    constexpr int num_threads = 4;
    constexpr int entries_per_thread = 50;

    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([writer_ptr, t]() {
            for (int i = 0; i < entries_per_thread; ++i) {
                log_entry entry(kcenon::common::interfaces::log_level::info,
                               "Thread " + std::to_string(t) + " Entry " + std::to_string(i),
                               std::chrono::system_clock::now());
                auto result = writer_ptr->write(entry);
                EXPECT_TRUE(result.is_ok());
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(writer_ptr->get_entries_encrypted(), num_threads * entries_per_thread);
}

TEST_F(EncryptedWriterTest, DecryptCorruptedData) {
    // Create minimal corrupted data
    std::vector<uint8_t> corrupted_data = {0x00, 0x01, 0x02, 0x03};

    auto result = encrypted_writer::decrypt_entry(corrupted_data, *test_key_);
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error_code(), logger_error_code::decryption_failed);
}

TEST_F(EncryptedWriterTest, DecryptInvalidMagic) {
    encrypted_log_header header;
    header.magic = 0x12345678;  // Invalid magic
    header.encrypted_length = 16;

    std::vector<uint8_t> bad_data(sizeof(header) + 16);
    std::memcpy(bad_data.data(), &header, sizeof(header));

    auto result = encrypted_writer::decrypt_entry(bad_data, *test_key_);
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error_code(), logger_error_code::decryption_failed);
}

#else // !LOGGER_HAS_OPENSSL_CRYPTO

TEST_F(EncryptedWriterTest, OpenSSLNotAvailable) {
    // When OpenSSL is not available, encryption should fail gracefully
    GTEST_SKIP() << "OpenSSL not available, skipping encryption tests";
}

#endif // LOGGER_HAS_OPENSSL_CRYPTO

} // namespace kcenon::logger::test

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
