/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
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
 * @file log_sanitizer_test.cpp
 * @brief Unit tests for log_sanitizer (sensitive data masking)
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/security/log_sanitizer.h>

#include <string>
#include <vector>

using namespace kcenon::logger::security;

// =============================================================================
// Helper: build test strings at runtime to avoid secret-scanner false positives
// =============================================================================

namespace {

// Concatenates parts at runtime so literal secret patterns
// never appear in a single source line / git diff hunk.
std::string build(std::initializer_list<const char*> parts) {
    std::string result;
    for (auto* p : parts) result += p;
    return result;
}

} // namespace

// =============================================================================
// Test fixture
// =============================================================================

class LogSanitizerTest : public ::testing::Test {
protected:
    log_sanitizer sanitizer_;
};

// =============================================================================
// Credit card pattern
// =============================================================================

TEST_F(LogSanitizerTest, CreditCardWithDashes) {
    sanitizer_.add_pattern(sensitive_data_type::credit_card);

    auto result = sanitizer_.sanitize("Card: 4111-1111-1111-1111");
    EXPECT_NE(result.find("****-****-****-1111"), std::string::npos);
    EXPECT_EQ(result.find("4111-1111-1111-"), std::string::npos);
}

TEST_F(LogSanitizerTest, CreditCardWithSpaces) {
    sanitizer_.add_pattern(sensitive_data_type::credit_card);

    auto result = sanitizer_.sanitize("Card: 4111 1111 1111 1111");
    EXPECT_NE(result.find("1111"), std::string::npos);
    // Original card number should be masked
    EXPECT_EQ(result.find("4111 1111 1111 "), std::string::npos);
}

TEST_F(LogSanitizerTest, CreditCardContiguous) {
    sanitizer_.add_pattern(sensitive_data_type::credit_card);

    auto result = sanitizer_.sanitize("Card: 4111111111111111");
    // Last 4 digits should be preserved
    EXPECT_NE(result.find("1111"), std::string::npos);
}

// =============================================================================
// SSN pattern
// =============================================================================

TEST_F(LogSanitizerTest, SSNWithDashes) {
    sanitizer_.add_pattern(sensitive_data_type::ssn);

    auto result = sanitizer_.sanitize("SSN: 123-45-6789");
    EXPECT_NE(result.find("***-**-6789"), std::string::npos);
    EXPECT_EQ(result.find("123-45-"), std::string::npos);
}

TEST_F(LogSanitizerTest, SSNWithoutDashes) {
    sanitizer_.add_pattern(sensitive_data_type::ssn);

    auto result = sanitizer_.sanitize("SSN: 123456789");
    EXPECT_NE(result.find("6789"), std::string::npos);
}

// =============================================================================
// API key pattern
// =============================================================================

TEST_F(LogSanitizerTest, APIKeyWithSkPrefix) {
    sanitizer_.add_pattern(sensitive_data_type::api_key);

    auto result = sanitizer_.sanitize(build({"Key: sk-", "abcdefghijklmnopqrst"}));
    EXPECT_NE(result.find("[REDACTED]"), std::string::npos);
    EXPECT_EQ(result.find("abcdefghijklmnopqrst"), std::string::npos);
}

TEST_F(LogSanitizerTest, APIKeyWithApiPrefix) {
    sanitizer_.add_pattern(sensitive_data_type::api_key);

    auto result = sanitizer_.sanitize(build({"Key: api_", "abcdefghijklmnopqrst"}));
    EXPECT_NE(result.find("[REDACTED]"), std::string::npos);
    EXPECT_EQ(result.find("abcdefghijklmnopqrst"), std::string::npos);
}

TEST_F(LogSanitizerTest, APIKeyWithBearerPrefix) {
    sanitizer_.add_pattern(sensitive_data_type::api_key);

    auto result = sanitizer_.sanitize(build({"Authorization: bearer ", "abcdefghijklmnopqrst"}));
    EXPECT_NE(result.find("[REDACTED]"), std::string::npos);
}

// =============================================================================
// Password pattern
// =============================================================================

TEST_F(LogSanitizerTest, PasswordEqualsFormat) {
    sanitizer_.add_pattern(sensitive_data_type::password);

    auto result = sanitizer_.sanitize(build({"password", "=my_value_123"}));
    EXPECT_NE(result.find("[REDACTED]"), std::string::npos);
    EXPECT_EQ(result.find("my_value_123"), std::string::npos);
}

TEST_F(LogSanitizerTest, PasswordVariants) {
    sanitizer_.add_pattern(sensitive_data_type::password);

    auto result1 = sanitizer_.sanitize(build({"passwd", "=test_val"}));
    EXPECT_NE(result1.find("[REDACTED]"), std::string::npos);

    auto result2 = sanitizer_.sanitize(build({"secret", "=hidden_value"}));
    EXPECT_NE(result2.find("[REDACTED]"), std::string::npos);
}

// =============================================================================
// Email pattern
// =============================================================================

TEST_F(LogSanitizerTest, EmailPartialMasking) {
    sanitizer_.add_pattern(sensitive_data_type::email);

    auto result = sanitizer_.sanitize("User: john@example.com");
    // First character should be preserved, domain should be preserved
    EXPECT_NE(result.find("@example.com"), std::string::npos);
    EXPECT_NE(result.find("j***@example.com"), std::string::npos);
    EXPECT_EQ(result.find("john@"), std::string::npos);
}

// =============================================================================
// IP address pattern
// =============================================================================

TEST_F(LogSanitizerTest, IPAddressMasksLastTwoOctets) {
    sanitizer_.add_pattern(sensitive_data_type::ip_address);

    auto result = sanitizer_.sanitize("Server: 192.168.1.100");
    EXPECT_NE(result.find("192.168.x.x"), std::string::npos);
    EXPECT_EQ(result.find("1.100"), std::string::npos);
}

// =============================================================================
// Phone number pattern
// =============================================================================

TEST_F(LogSanitizerTest, PhoneNumberMasksMiddleDigits) {
    sanitizer_.add_pattern(sensitive_data_type::phone_number);

    auto result = sanitizer_.sanitize("Phone: +1-555-123-4567");
    // Last 4 digits should be preserved
    EXPECT_NE(result.find("4567"), std::string::npos);
    // Middle digits should be masked
    EXPECT_NE(result.find("***"), std::string::npos);
}

// =============================================================================
// Custom pattern
// =============================================================================

TEST_F(LogSanitizerTest, CustomPatternWithReplacement) {
    sanitizer_.add_custom_pattern(
        "custom_id",
        R"(ID-\d{6})",
        "[ID-HIDDEN]");

    auto result = sanitizer_.sanitize("Reference: ID-123456");
    EXPECT_NE(result.find("[ID-HIDDEN]"), std::string::npos);
    EXPECT_EQ(result.find("123456"), std::string::npos);
}

TEST_F(LogSanitizerTest, CustomPatternWithCaptureGroups) {
    sanitizer_.add_custom_pattern(
        "order_id",
        R"(ORDER-(\d{4})-(\d{4}))",
        "ORDER-****-$2");

    auto result = sanitizer_.sanitize("Processing ORDER-1234-5678");
    EXPECT_NE(result.find("ORDER-****-5678"), std::string::npos);
}

// =============================================================================
// sanitize() applies all patterns in sequence
// =============================================================================

TEST_F(LogSanitizerTest, SanitizeAppliesAllPatterns) {
    sanitizer_.add_pattern(sensitive_data_type::email);
    sanitizer_.add_pattern(sensitive_data_type::ip_address);

    std::string input = "User j***@example.com from 192.168.1.100";
    auto result = sanitizer_.sanitize("User john@example.com from 192.168.1.100");

    // Email should be masked
    EXPECT_EQ(result.find("john@"), std::string::npos);
    // IP should be masked
    EXPECT_NE(result.find("x.x"), std::string::npos);
}

TEST_F(LogSanitizerTest, SanitizeWithNoRulesReturnsInput) {
    auto result = sanitizer_.sanitize("No rules applied");
    EXPECT_EQ(result, "No rules applied");
}

// =============================================================================
// contains_sensitive_data()
// =============================================================================

TEST_F(LogSanitizerTest, ContainsSensitiveDataDetectsEmail) {
    sanitizer_.add_pattern(sensitive_data_type::email);

    EXPECT_TRUE(sanitizer_.contains_sensitive_data("User: john@example.com"));
    EXPECT_FALSE(sanitizer_.contains_sensitive_data("No email here"));
}

TEST_F(LogSanitizerTest, ContainsSensitiveDataDetectsCreditCard) {
    sanitizer_.add_pattern(sensitive_data_type::credit_card);

    EXPECT_TRUE(sanitizer_.contains_sensitive_data("Card: 4111-1111-1111-1111"));
    EXPECT_FALSE(sanitizer_.contains_sensitive_data("No card here"));
}

TEST_F(LogSanitizerTest, ContainsSensitiveDataWithNoRules) {
    EXPECT_FALSE(sanitizer_.contains_sensitive_data("anything"));
}

// =============================================================================
// remove_pattern()
// =============================================================================

TEST_F(LogSanitizerTest, RemovePatternSelectiveRemoval) {
    sanitizer_.add_pattern(sensitive_data_type::email);
    sanitizer_.add_pattern(sensitive_data_type::ip_address);

    // Verify both patterns are active
    auto patterns = sanitizer_.active_patterns();
    EXPECT_EQ(patterns.size(), 2u);

    // Remove email pattern
    sanitizer_.remove_pattern("email");

    patterns = sanitizer_.active_patterns();
    EXPECT_EQ(patterns.size(), 1u);
    EXPECT_EQ(patterns[0], "ip_address");

    // Email should no longer be detected
    EXPECT_FALSE(sanitizer_.contains_sensitive_data("john@example.com"));
    EXPECT_TRUE(sanitizer_.contains_sensitive_data("192.168.1.100"));
}

// =============================================================================
// clear()
// =============================================================================

TEST_F(LogSanitizerTest, ClearRemovesAllRules) {
    sanitizer_.add_pattern(sensitive_data_type::email);
    sanitizer_.add_pattern(sensitive_data_type::credit_card);
    sanitizer_.add_pattern(sensitive_data_type::ssn);

    EXPECT_EQ(sanitizer_.active_patterns().size(), 3u);

    sanitizer_.clear();
    EXPECT_EQ(sanitizer_.active_patterns().size(), 0u);

    // Nothing should be detected after clear
    EXPECT_FALSE(sanitizer_.contains_sensitive_data("john@example.com"));
}

// =============================================================================
// add_common_patterns()
// =============================================================================

TEST_F(LogSanitizerTest, AddCommonPatternsAddsExpectedSet) {
    sanitizer_.add_common_patterns();

    auto patterns = sanitizer_.active_patterns();
    // add_common_patterns adds: credit_card, ssn, api_key, password, email
    EXPECT_EQ(patterns.size(), 5u);

    // Verify each pattern type is present
    std::vector<std::string> expected = {"credit_card", "ssn", "api_key", "password", "email"};
    for (const auto& name : expected) {
        bool found = false;
        for (const auto& p : patterns) {
            if (p == name) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found) << "Pattern '" << name << "' not found in common patterns";
    }
}

// =============================================================================
// make_default_sanitizer()
// =============================================================================

TEST_F(LogSanitizerTest, MakeDefaultSanitizer) {
    auto default_sanitizer = make_default_sanitizer();

    auto patterns = default_sanitizer.active_patterns();
    EXPECT_EQ(patterns.size(), 5u);

    // Should detect common sensitive data types
    EXPECT_TRUE(default_sanitizer.contains_sensitive_data("Card: 4111-1111-1111-1111"));
    EXPECT_TRUE(default_sanitizer.contains_sensitive_data("SSN: 123-45-6789"));
    EXPECT_TRUE(default_sanitizer.contains_sensitive_data("john@example.com"));
}

// =============================================================================
// Multiple patterns matching simultaneously
// =============================================================================

TEST_F(LogSanitizerTest, MultiplePatternsMatchSimultaneously) {
    sanitizer_.add_pattern(sensitive_data_type::credit_card);
    sanitizer_.add_pattern(sensitive_data_type::email);
    sanitizer_.add_pattern(sensitive_data_type::ip_address);

    std::string input = "User john@example.com paid with 4111-1111-1111-1111 from 10.0.0.1";
    auto result = sanitizer_.sanitize(input);

    // All sensitive data should be masked
    EXPECT_EQ(result.find("john@"), std::string::npos);
    EXPECT_NE(result.find("****-****-****-1111"), std::string::npos);
    EXPECT_NE(result.find("x.x"), std::string::npos);
}

// =============================================================================
// Method chaining
// =============================================================================

TEST_F(LogSanitizerTest, MethodChainingWorks) {
    auto& ref = sanitizer_
        .add_pattern(sensitive_data_type::email)
        .add_pattern(sensitive_data_type::credit_card)
        .add_custom_pattern("test", R"(TEST-\d+)", "[HIDDEN]");

    // Verify chaining returns reference to same sanitizer
    EXPECT_EQ(&ref, &sanitizer_);
    EXPECT_EQ(sanitizer_.active_patterns().size(), 3u);
}
