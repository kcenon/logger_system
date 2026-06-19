// BSD 3-Clause License
// Copyright (c) 2025, kcenon
// See the LICENSE file in the project root for full license information.

/**
 * @file integrity_policy.h
 * @brief Tamper-evident log signing policies for writers.
 *
 * Issue #612 (ISO/IEC 27001 A.12.4.2 / A.12.4.3 - Protection of log information).
 *
 * Provides an abstract integrity_policy interface and a concrete
 * hmac_sha256_integrity_policy implementation, reusing the same HMAC
 * approach as audit_logger. The policy produces a per-record signature
 * that writers append to their output so consumers can verify the log
 * has not been tampered with after emission.
 *
 * The policy is deliberately minimal:
 *  - sign(record) returns a hex-encoded signature string
 *  - verify(record, signature) returns true only if signatures match
 *  - name() returns a short identifier used in signature prefixes
 */

#pragma once

#include <kcenon/logger/security/secure_key_storage.h>

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

// OpenSSL 3.x+ for HMAC via EVP_MAC API (optional, fallback to portable hash)
#if __has_include(<openssl/hmac.h>)
#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/params.h>
#ifndef LOGGER_INTEGRITY_HAS_OPENSSL
#define LOGGER_INTEGRITY_HAS_OPENSSL 1
#endif
#endif

namespace kcenon::logger::security {

/**
 * @class integrity_policy
 * @brief Abstract interface for log integrity signing.
 *
 * Implementations must be thread-safe: sign() and verify() may be invoked
 * concurrently from writer threads.
 */
class integrity_policy {
public:
    virtual ~integrity_policy() = default;

    /**
     * @brief Produce a signature for @p record.
     * @param record The serialized log record to sign.
     * @return Hex-encoded signature, or empty string on failure.
     */
    virtual std::string sign(const std::string& record) const = 0;

    /**
     * @brief Verify that @p signature matches @p record.
     * @param record The original log record.
     * @param signature The signature previously emitted by sign().
     * @return true if the signature is valid, false otherwise.
     */
    virtual bool verify(const std::string& record,
                        const std::string& signature) const = 0;

    /**
     * @brief Short identifier used as a prefix in serialized signatures
     *        (e.g. "HMAC-SHA256"). Implementations must return a stable
     *        non-empty ASCII string.
     */
    virtual std::string name() const = 0;
};

/**
 * @class hmac_sha256_integrity_policy
 * @brief HMAC-SHA256 integrity policy (ISO/IEC 27001 A.12.4.2 default).
 *
 * Uses OpenSSL's EVP_MAC API when available; otherwise falls back to a
 * portable, non-cryptographic hash. The fallback is functionally correct
 * (sign/verify round-trip) but MUST NOT be relied on for tamper detection
 * in production. Build with OpenSSL 3.x to get real HMAC-SHA256.
 *
 * Thread-safety: instances are immutable after construction; sign() and
 * verify() may be called concurrently.
 */
class hmac_sha256_integrity_policy final : public integrity_policy {
public:
    /**
     * @brief Construct from a secure_key.
     * @param key HMAC key (moved in; policy takes ownership).
     *
     * The key is stored in a secure_key to guarantee zeroization on
     * destruction (see secure_key_storage.h).
     */
    explicit hmac_sha256_integrity_policy(secure_key key)
        : key_(std::make_shared<secure_key>(std::move(key))) {
    }

    std::string sign(const std::string& record) const override {
        return compute_hmac(record, *key_);
    }

    bool verify(const std::string& record,
                const std::string& signature) const override {
        if (signature.empty()) {
            return false;
        }
        const std::string expected = compute_hmac(record, *key_);
        if (expected.empty() || expected.size() != signature.size()) {
            return false;
        }
        // Constant-time compare to avoid signature-timing leaks.
        unsigned char diff = 0;
        for (size_t i = 0; i < expected.size(); ++i) {
            diff |= static_cast<unsigned char>(expected[i])
                  ^ static_cast<unsigned char>(signature[i]);
        }
        return diff == 0;
    }

    std::string name() const override {
        return "HMAC-SHA256";
    }

private:
    static std::string compute_hmac(const std::string& message,
                                    const secure_key& key) {
#ifdef LOGGER_INTEGRITY_HAS_OPENSSL
        unsigned char digest[EVP_MAX_MD_SIZE];
        size_t digest_len = 0;

        EVP_MAC* mac = EVP_MAC_fetch(nullptr, "HMAC", nullptr);
        if (!mac) {
            return std::string();
        }

        EVP_MAC_CTX* ctx = EVP_MAC_CTX_new(mac);
        if (!ctx) {
            EVP_MAC_free(mac);
            return std::string();
        }

        OSSL_PARAM params[] = {
            OSSL_PARAM_construct_utf8_string(
                OSSL_MAC_PARAM_DIGEST,
                const_cast<char*>("SHA256"),
                0),
            OSSL_PARAM_END
        };

        if (EVP_MAC_init(ctx, key.data().data(), key.size(), params) != 1) {
            EVP_MAC_CTX_free(ctx);
            EVP_MAC_free(mac);
            return std::string();
        }

        if (EVP_MAC_update(
                ctx,
                reinterpret_cast<const unsigned char*>(message.data()),
                message.size()) != 1) {
            EVP_MAC_CTX_free(ctx);
            EVP_MAC_free(mac);
            return std::string();
        }

        if (EVP_MAC_final(ctx, digest, &digest_len, sizeof(digest)) != 1) {
            EVP_MAC_CTX_free(ctx);
            EVP_MAC_free(mac);
            return std::string();
        }

        EVP_MAC_CTX_free(ctx);
        EVP_MAC_free(mac);

        std::ostringstream hex;
        hex << std::hex << std::setfill('0');
        for (size_t i = 0; i < digest_len; ++i) {
            hex << std::setw(2) << static_cast<int>(digest[i]);
        }
        return hex.str();
#else
        // Portable fallback. NOT cryptographically secure; intended only
        // to keep sign/verify round-trip working on builds without OpenSSL.
        std::size_t hash = 0xcbf29ce484222325ULL;
        for (size_t i = 0; i < message.size(); ++i) {
            hash ^= static_cast<unsigned char>(message[i]);
            hash *= 0x100000001b3ULL;
            if (i < key.size()) {
                hash ^= key.data()[i];
            }
        }
        std::ostringstream oss;
        oss << std::hex << std::setfill('0') << std::setw(16) << hash;
        return oss.str();
#endif
    }

    // shared_ptr so the policy can be copied cheaply across decorator chains
    // while still ensuring the key is zeroized when the last owner releases.
    std::shared_ptr<secure_key> key_;
};

/**
 * @brief Format a signature line suitable for appending to a text log record.
 *
 * Format: " SIGNATURE[<policy-name>]:<hex>"
 *
 * The leading space makes the signature clearly separable from the preceding
 * record text while keeping the line self-descriptive for the verify tool.
 */
inline std::string format_signature_suffix(const integrity_policy& policy,
                                           const std::string& record) {
    std::string sig = policy.sign(record);
    if (sig.empty()) {
        return std::string();
    }
    std::string out;
    out.reserve(14 + policy.name().size() + sig.size());
    out.append(" SIGNATURE[");
    out.append(policy.name());
    out.append("]:");
    out.append(sig);
    return out;
}

} // namespace kcenon::logger::security
