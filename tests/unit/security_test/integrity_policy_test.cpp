// BSD 3-Clause License
// Copyright (c) 2025, kcenon
// See the LICENSE file in the project root for full license information.

/**
 * @file integrity_policy_test.cpp
 * @brief Tamper-evident round-trip tests for all writers (Issue #612).
 *
 * Validates that file_writer, rotating_file_writer, and console_writer
 * emit signed records when an integrity_policy is installed, and that
 * the signatures verify against the original record and fail for
 * tampered records.
 */

#include <gtest/gtest.h>

#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/security/integrity_policy.h>
#include <kcenon/logger/security/secure_key_storage.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/writers/rotating_file_writer.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

using namespace kcenon::logger;
using namespace kcenon::logger::security;
using log_level = kcenon::common::interfaces::log_level;

namespace {

std::shared_ptr<hmac_sha256_integrity_policy> make_policy() {
    auto key_result = secure_key_storage::generate_key(32);
    EXPECT_TRUE(key_result.has_value());
    return std::make_shared<hmac_sha256_integrity_policy>(
        std::move(key_result.value()));
}

// Split a signed line of the form:
//   <record> SIGNATURE[<alg>]:<hex>
// Returns {record, signature} on success, {"", ""} on parse failure.
std::pair<std::string, std::string> split_signed_line(const std::string& line) {
    // Find the LAST " SIGNATURE[" marker so colons in record text are fine.
    const std::string marker = " SIGNATURE[";
    auto pos = line.rfind(marker);
    if (pos == std::string::npos) {
        return {"", ""};
    }
    auto colon = line.find("]:", pos);
    if (colon == std::string::npos) {
        return {"", ""};
    }
    std::string record = line.substr(0, pos);
    std::string signature = line.substr(colon + 2);
    return {record, signature};
}

log_entry make_entry(const std::string& msg) {
    return log_entry(log_level::info, msg);
}

} // namespace

// ============================================================================
// Policy primitive tests
// ============================================================================

TEST(IntegrityPolicyTest, HmacRoundTrip) {
    auto policy = make_policy();
    const std::string record = "hello world";
    std::string sig = policy->sign(record);
    EXPECT_FALSE(sig.empty());
    EXPECT_TRUE(policy->verify(record, sig));
    EXPECT_FALSE(policy->verify(record + "x", sig));
    EXPECT_FALSE(policy->verify(record, ""));
}

TEST(IntegrityPolicyTest, TwoPoliciesProduceDifferentSignatures) {
    auto a = make_policy();
    auto b = make_policy();
    const std::string record = "same-record";
    EXPECT_NE(a->sign(record), b->sign(record));
    EXPECT_FALSE(a->verify(record, b->sign(record)));
}

TEST(IntegrityPolicyTest, FormatSignatureSuffixIsNonEmpty) {
    auto policy = make_policy();
    std::string suffix = format_signature_suffix(*policy, "record");
    EXPECT_NE(suffix.find(" SIGNATURE[HMAC-SHA256]:"), std::string::npos);
}

// ============================================================================
// file_writer integrity
// ============================================================================

class FileWriterIntegrityTest : public ::testing::Test {
protected:
    std::filesystem::path temp_dir_;

    void SetUp() override {
        temp_dir_ = std::filesystem::temp_directory_path() /
                    "logger_integrity_file_test";
        std::filesystem::create_directories(temp_dir_);
    }

    void TearDown() override {
        std::error_code ec;
        std::filesystem::remove_all(temp_dir_, ec);
    }
};

TEST_F(FileWriterIntegrityTest, RoundTripSignedLine) {
    auto path = (temp_dir_ / "signed.log").string();
    auto policy = make_policy();

    {
        file_writer writer(path, /*append=*/false);
        writer.set_integrity_policy(policy);
        ASSERT_TRUE(writer.write(make_entry("first")).is_ok());
        ASSERT_TRUE(writer.write(make_entry("second")).is_ok());
    }

    std::ifstream in(path);
    std::string line;
    int verified = 0;
    while (std::getline(in, line)) {
        auto [record, sig] = split_signed_line(line);
        ASSERT_FALSE(record.empty()) << "line missing signature: " << line;
        EXPECT_TRUE(policy->verify(record, sig));
        ++verified;
    }
    EXPECT_EQ(verified, 2);
}

TEST_F(FileWriterIntegrityTest, TamperedRecordFailsVerification) {
    auto path = (temp_dir_ / "tampered.log").string();
    auto policy = make_policy();

    {
        file_writer writer(path, /*append=*/false);
        writer.set_integrity_policy(policy);
        ASSERT_TRUE(writer.write(make_entry("payload")).is_ok());
    }

    std::ifstream in(path);
    std::string line;
    ASSERT_TRUE(std::getline(in, line));
    auto [record, sig] = split_signed_line(line);
    ASSERT_FALSE(record.empty());

    // Bit-flip the record: verification must fail.
    std::string tampered = record;
    tampered.back() ^= 0x01;
    EXPECT_FALSE(policy->verify(tampered, sig));
}

TEST_F(FileWriterIntegrityTest, NoPolicyProducesNoSignature) {
    auto path = (temp_dir_ / "plain.log").string();
    {
        file_writer writer(path, /*append=*/false);
        ASSERT_TRUE(writer.write(make_entry("plain")).is_ok());
    }
    std::ifstream in(path);
    std::string line;
    ASSERT_TRUE(std::getline(in, line));
    EXPECT_EQ(line.find(" SIGNATURE["), std::string::npos);
}

// ============================================================================
// rotating_file_writer integrity (delegates to file_writer base)
// ============================================================================

TEST_F(FileWriterIntegrityTest, RotatingWriterRoundTrip) {
    auto path = (temp_dir_ / "rotating.log").string();
    auto policy = make_policy();

    {
        // Large max_size so we don't actually rotate during the test;
        // we only care that the write path applies the signature.
        rotating_file_writer writer(path,
                                    /*max_size=*/10 * 1024 * 1024,
                                    /*max_files=*/3);
        writer.set_integrity_policy(policy);
        ASSERT_TRUE(writer.write(make_entry("rot-a")).is_ok());
        ASSERT_TRUE(writer.write(make_entry("rot-b")).is_ok());
    }

    std::ifstream in(path);
    std::string line;
    int verified = 0;
    while (std::getline(in, line)) {
        auto [record, sig] = split_signed_line(line);
        ASSERT_FALSE(record.empty()) << "line missing signature: " << line;
        EXPECT_TRUE(policy->verify(record, sig));
        ++verified;
    }
    EXPECT_EQ(verified, 2);
}

// ============================================================================
// console_writer integrity (captures stdout/stderr)
// ============================================================================

TEST(ConsoleWriterIntegrityTest, RoundTripOnStdout) {
    auto policy = make_policy();

    console_writer writer(/*use_stderr=*/false,
                          /*auto_detect_color=*/false);
    writer.set_integrity_policy(policy);

    testing::internal::CaptureStdout();
    ASSERT_TRUE(writer.write(make_entry("console-msg")).is_ok());
    std::string captured = testing::internal::GetCapturedStdout();

    ASSERT_FALSE(captured.empty());
    // Strip trailing newline before splitting.
    if (captured.back() == '\n') captured.pop_back();

    auto [record, sig] = split_signed_line(captured);
    ASSERT_FALSE(record.empty()) << "captured: " << captured;
    EXPECT_TRUE(policy->verify(record, sig));
}

TEST(ConsoleWriterIntegrityTest, NoPolicyNoSignature) {
    console_writer writer(/*use_stderr=*/false,
                          /*auto_detect_color=*/false);

    testing::internal::CaptureStdout();
    ASSERT_TRUE(writer.write(make_entry("plain")).is_ok());
    std::string captured = testing::internal::GetCapturedStdout();

    EXPECT_EQ(captured.find(" SIGNATURE["), std::string::npos);
}
