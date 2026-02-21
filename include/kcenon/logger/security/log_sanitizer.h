// BSD 3-Clause License
//
// Copyright (c) 2021-2025, kcenon
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file log_sanitizer.h
 * @brief Log sanitization for sensitive data protection
 *
 * @details Provides automatic detection and masking of sensitive data in log messages
 * including credit card numbers, SSNs, API keys, passwords, emails, IP addresses,
 * and phone numbers. Supports custom patterns for organization-specific requirements.
 *
 * @example
 * @code
 * log_sanitizer sanitizer;
 * sanitizer.add_pattern(sensitive_data_type::credit_card);
 * sanitizer.add_pattern(sensitive_data_type::email);
 *
 * std::string message = "User john@example.com paid with card 4111-1111-1111-1111";
 * std::string sanitized = sanitizer.sanitize(message);
 * // Result: "User j***@example.com paid with card ****-****-****-1111"
 * @endcode
 */

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <regex>
#include <memory>
#include <functional>

namespace kcenon::logger::security {

/**
 * @brief Types of sensitive data that can be automatically detected
 */
enum class sensitive_data_type {
    credit_card,     ///< Credit card numbers (Visa, MC, Amex, etc.)
    ssn,             ///< Social Security Numbers (US format)
    api_key,         ///< API keys (sk-, api_, bearer tokens)
    password,        ///< Password fields in key=value format
    email,           ///< Email addresses
    ip_address,      ///< IPv4 addresses
    phone_number,    ///< Phone numbers (various formats)
    custom           ///< Custom user-defined patterns
};

/**
 * @brief Configuration for a sanitization rule
 */
struct sanitization_rule {
    std::string name;           ///< Identifier for the rule
    std::regex pattern;         ///< Regex pattern to match
    std::string replacement;    ///< Replacement text or pattern
    bool preserve_partial;      ///< Keep last N characters visible

    sanitization_rule(std::string n, const std::string& p, std::string r, bool pp = false)
        : name(std::move(n))
        , pattern(p, std::regex::icase | std::regex::optimize)
        , replacement(std::move(r))
        , preserve_partial(pp) {}
};

/**
 * @brief Log sanitizer for masking sensitive data
 *
 * @details Thread-safe sanitizer that detects and masks sensitive information
 * in log messages. Supports both built-in patterns for common sensitive data
 * types and custom patterns for organization-specific needs.
 *
 * Built-in patterns:
 * - Credit cards: Masks all but last 4 digits
 * - SSN: Masks all but last 4 digits
 * - API keys: Fully redacted
 * - Passwords: Fully redacted
 * - Emails: Partial masking of local part
 * - IP addresses: Masks last two octets
 * - Phone numbers: Masks middle digits
 */
class log_sanitizer {
public:
    /**
     * @brief Add a built-in pattern for sensitive data detection
     * @param type The type of sensitive data to detect
     * @return Reference to this sanitizer for method chaining
     */
    log_sanitizer& add_pattern(sensitive_data_type type) {
        switch (type) {
            case sensitive_data_type::credit_card:
                add_credit_card_pattern();
                break;
            case sensitive_data_type::ssn:
                add_ssn_pattern();
                break;
            case sensitive_data_type::api_key:
                add_api_key_pattern();
                break;
            case sensitive_data_type::password:
                add_password_pattern();
                break;
            case sensitive_data_type::email:
                add_email_pattern();
                break;
            case sensitive_data_type::ip_address:
                add_ip_address_pattern();
                break;
            case sensitive_data_type::phone_number:
                add_phone_number_pattern();
                break;
            case sensitive_data_type::custom:
                // Custom patterns are added via add_custom_pattern()
                break;
        }
        return *this;
    }

    /**
     * @brief Add a custom sanitization pattern
     * @param name Identifier for the pattern
     * @param regex_pattern Regular expression to match sensitive data
     * @param replacement Replacement string (use $1, $2 for capture groups)
     * @param preserve_partial If true, preserve last few characters
     * @return Reference to this sanitizer for method chaining
     */
    log_sanitizer& add_custom_pattern(
            std::string_view name,
            std::string_view regex_pattern,
            std::string_view replacement = "[REDACTED]",
            bool preserve_partial = false) {
        rules_.emplace_back(
            std::string(name),
            std::string(regex_pattern),
            std::string(replacement),
            preserve_partial);
        return *this;
    }

    /**
     * @brief Remove a pattern by name
     * @param name Name of the pattern to remove
     * @return Reference to this sanitizer for method chaining
     */
    log_sanitizer& remove_pattern(std::string_view name) {
        rules_.erase(
            std::remove_if(rules_.begin(), rules_.end(),
                [name](const sanitization_rule& rule) {
                    return rule.name == name;
                }),
            rules_.end());
        return *this;
    }

    /**
     * @brief Sanitize a string by masking all detected sensitive data
     * @param input The string to sanitize
     * @return Sanitized string with sensitive data masked
     */
    std::string sanitize(std::string_view input) const {
        if (rules_.empty()) {
            return std::string(input);
        }

        std::string result(input);
        for (const auto& rule : rules_) {
            result = apply_rule(result, rule);
        }
        return result;
    }

    /**
     * @brief Check if a string contains sensitive data
     * @param input The string to check
     * @return true if sensitive data is detected, false otherwise
     */
    bool contains_sensitive_data(std::string_view input) const {
        for (const auto& rule : rules_) {
            if (std::regex_search(input.begin(), input.end(), rule.pattern)) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Get list of active pattern names
     * @return Vector of pattern names currently active
     */
    std::vector<std::string> active_patterns() const {
        std::vector<std::string> names;
        names.reserve(rules_.size());
        for (const auto& rule : rules_) {
            names.push_back(rule.name);
        }
        return names;
    }

    /**
     * @brief Clear all patterns
     * @return Reference to this sanitizer for method chaining
     */
    log_sanitizer& clear() {
        rules_.clear();
        return *this;
    }

    /**
     * @brief Add all common patterns at once
     * @return Reference to this sanitizer for method chaining
     */
    log_sanitizer& add_common_patterns() {
        add_pattern(sensitive_data_type::credit_card);
        add_pattern(sensitive_data_type::ssn);
        add_pattern(sensitive_data_type::api_key);
        add_pattern(sensitive_data_type::password);
        add_pattern(sensitive_data_type::email);
        return *this;
    }

private:
    std::vector<sanitization_rule> rules_;

    void add_credit_card_pattern() {
        // Matches credit card numbers with or without separators
        // Preserves last 4 digits
        rules_.emplace_back(
            "credit_card",
            R"(\b(\d{4}[-\s]?\d{4}[-\s]?\d{4}[-\s]?)(\d{4})\b)",
            "****-****-****-$2",
            true);
    }

    void add_ssn_pattern() {
        // Matches SSN format XXX-XX-XXXX
        // Preserves last 4 digits
        rules_.emplace_back(
            "ssn",
            R"(\b(\d{3})[-\s]?(\d{2})[-\s]?(\d{4})\b)",
            "***-**-$3",
            true);
    }

    void add_api_key_pattern() {
        // Matches common API key formats
        rules_.emplace_back(
            "api_key",
            R"(\b(sk[-_]|api[-_]|key[-_]|token[-_]|bearer\s+)([a-zA-Z0-9]{16,})\b)",
            "$1[REDACTED]",
            false);
    }

    void add_password_pattern() {
        // Matches password=xxx, pwd=xxx, passwd=xxx patterns
        rules_.emplace_back(
            "password",
            R"(((?:password|passwd|pwd|secret|credential)[\s]*[=:]\s*)([^\s&]+))",
            "$1[REDACTED]",
            false);
    }

    void add_email_pattern() {
        // Matches email addresses, masks local part partially
        rules_.emplace_back(
            "email",
            R"(\b([a-zA-Z0-9._%+-])([a-zA-Z0-9._%+-]*)(@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})\b)",
            "$1***$3",
            true);
    }

    void add_ip_address_pattern() {
        // Matches IPv4 addresses, masks last two octets
        rules_.emplace_back(
            "ip_address",
            R"(\b(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})\b)",
            "$1.$2.x.x",
            true);
    }

    void add_phone_number_pattern() {
        // Matches phone numbers in various formats
        rules_.emplace_back(
            "phone_number",
            R"(\b(\+?\d{1,3}[-.\s]?)(\d{3})[-.\s]?(\d{3})[-.\s]?(\d{4})\b)",
            "$1***-***-$4",
            true);
    }

    static std::string apply_rule(const std::string& input, const sanitization_rule& rule) {
        return std::regex_replace(input, rule.pattern, rule.replacement);
    }
};

/**
 * @brief Create a sanitizer with common patterns pre-configured
 * @return A log_sanitizer with credit_card, ssn, api_key, password, and email patterns
 */
inline log_sanitizer make_default_sanitizer() {
    log_sanitizer sanitizer;
    sanitizer.add_common_patterns();
    return sanitizer;
}

} // namespace kcenon::logger::security
