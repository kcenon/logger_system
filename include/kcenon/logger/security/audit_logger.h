#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/security/secure_key_storage.h>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>

// OpenSSL for HMAC (optional, fallback to simple hash)
#if __has_include(<openssl/hmac.h>)
#include <openssl/hmac.h>
#include <openssl/evp.h>
#define HAS_OPENSSL_HMAC 1
#endif

namespace kcenon::logger::security {

/**
 * @class audit_logger
 * @brief Secure audit logging with tamper detection
 *
 * Features:
 * - JSON-formatted audit entries
 * - HMAC signatures for tamper detection
 * - Thread-safe logging
 * - Configurable audit file location
 */
class audit_logger {
public:
    /**
     * @enum audit_event
     * @brief Types of security-relevant events to audit
     */
    enum class audit_event {
        logger_started,
        logger_stopped,
        writer_added,
        writer_removed,
        encryption_key_loaded,
        encryption_key_rotated,
        encryption_key_generated,
        permission_denied,
        path_traversal_attempt,
        insecure_permissions_detected,
        suspicious_activity,
        configuration_changed,
        authentication_success,
        authentication_failure,
        file_access_denied,
        file_access_granted
    };

    /**
     * @brief Initialize audit logger
     * @param audit_file Path to audit log file
     * @param hmac_key HMAC key for tamper detection (optional)
     */
    static void initialize(
        const std::filesystem::path& audit_file,
        const secure_key* hmac_key = nullptr
    ) {
        std::lock_guard lock(get_mutex());

        get_audit_file() = audit_file;

        if (hmac_key) {
            // Create a copy of the key data (secure_key is move-only)
            get_hmac_key().reset(new secure_key(hmac_key->data()));
        }

        // Create audit file directory if needed
        auto parent = audit_file.parent_path();
        if (!parent.empty() && !std::filesystem::exists(parent)) {
            std::filesystem::create_directories(parent);
        }

        // Set secure permissions on audit file
        if (std::filesystem::exists(audit_file)) {
            std::filesystem::permissions(
                audit_file,
                std::filesystem::perms::owner_read |
                std::filesystem::perms::owner_write,
                std::filesystem::perm_options::replace
            );
        }
    }

    /**
     * @brief Log an audit event
     * @param event Event type
     * @param details Event details
     * @param metadata Additional metadata (key-value pairs)
     */
    static void log_audit_event(
        audit_event event,
        const std::string& details,
        const std::map<std::string, std::string>& metadata = {}
    ) {
        std::lock_guard lock(get_mutex());

        // Skip if not initialized
        if (get_audit_file().empty()) {
            return;
        }

        // 1. Get current timestamp
        auto now = std::chrono::system_clock::now();

        // 2. Build JSON entry
        std::ostringstream oss;
        oss << "{"
            << "\"timestamp\":\"" << format_iso8601(now) << "\","
            << "\"event\":\"" << event_to_string(event) << "\","
            << "\"details\":\"" << escape_json(details) << "\"";

        // 3. Add metadata if present
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

        std::string json_entry = oss.str();

        // 4. Calculate HMAC signature (if key is available)
        std::string signature;
        if (get_hmac_key()) {
            signature = calculate_hmac(json_entry, *get_hmac_key());
        }

        // 5. Write to audit file
        write_to_audit_log(json_entry, signature);
    }

    /**
     * @brief Verify integrity of an audit log entry
     * @param json_entry JSON audit entry
     * @param signature HMAC signature
     * @return true if signature is valid, false otherwise
     */
    static bool verify_entry(
        const std::string& json_entry,
        const std::string& signature
    ) {
        if (!get_hmac_key() || signature.empty()) {
            return false;
        }

        std::string expected = calculate_hmac(json_entry, *get_hmac_key());
        return expected == signature;
    }

private:
    /**
     * @brief Format timestamp as ISO8601 string
     */
    static std::string format_iso8601(
        const std::chrono::system_clock::time_point& tp
    ) {
        auto time_t_value = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_value;
#ifdef _WIN32
        localtime_s(&tm_value, &time_t_value);
#else
        localtime_r(&time_t_value, &tm_value);
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm_value, "%Y-%m-%dT%H:%M:%S");
        return oss.str();
    }

    /**
     * @brief Escape JSON special characters
     */
    static std::string escape_json(const std::string& str) {
        std::ostringstream oss;
        for (char c : str) {
            switch (c) {
                case '"':  oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b";  break;
                case '\f': oss << "\\f";  break;
                case '\n': oss << "\\n";  break;
                case '\r': oss << "\\r";  break;
                case '\t': oss << "\\t";  break;
                default:
                    if (c < 32) {
                        oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                    } else {
                        oss << c;
                    }
                    break;
            }
        }
        return oss.str();
    }

    /**
     * @brief Convert audit event to string
     */
    static std::string event_to_string(audit_event event) {
        switch (event) {
            case audit_event::logger_started:
                return "logger_started";
            case audit_event::logger_stopped:
                return "logger_stopped";
            case audit_event::writer_added:
                return "writer_added";
            case audit_event::writer_removed:
                return "writer_removed";
            case audit_event::encryption_key_loaded:
                return "encryption_key_loaded";
            case audit_event::encryption_key_rotated:
                return "encryption_key_rotated";
            case audit_event::encryption_key_generated:
                return "encryption_key_generated";
            case audit_event::permission_denied:
                return "permission_denied";
            case audit_event::path_traversal_attempt:
                return "path_traversal_attempt";
            case audit_event::insecure_permissions_detected:
                return "insecure_permissions_detected";
            case audit_event::suspicious_activity:
                return "suspicious_activity";
            case audit_event::configuration_changed:
                return "configuration_changed";
            case audit_event::authentication_success:
                return "authentication_success";
            case audit_event::authentication_failure:
                return "authentication_failure";
            case audit_event::file_access_denied:
                return "file_access_denied";
            case audit_event::file_access_granted:
                return "file_access_granted";
            default:
                return "unknown_event";
        }
    }

    /**
     * @brief Calculate HMAC signature
     * @param message Message to sign
     * @param key HMAC key
     * @return Hex-encoded signature
     */
    static std::string calculate_hmac(
        const std::string& message,
        const secure_key& key
    ) {
#ifdef HAS_OPENSSL_HMAC
        // Use OpenSSL HMAC-SHA256
        unsigned char digest[EVP_MAX_MD_SIZE];
        unsigned int digest_len = 0;

        HMAC(EVP_sha256(),
             key.data().data(), key.size(),
             reinterpret_cast<const unsigned char*>(message.data()), message.size(),
             digest, &digest_len);

        // Convert to hex string
        std::ostringstream hex;
        hex << std::hex << std::setfill('0');
        for (unsigned int i = 0; i < digest_len; ++i) {
            hex << std::setw(2) << static_cast<int>(digest[i]);
        }
        return hex.str();
#else
        // Fallback: simple hash (NOT cryptographically secure)
        // In production without OpenSSL, use a proper HMAC library
        size_t hash = 0;
        for (size_t i = 0; i < message.size(); ++i) {
            hash = hash * 31 + message[i];
            if (i < key.size()) {
                hash ^= key.data()[i];
            }
        }

        std::ostringstream oss;
        oss << std::hex << std::setfill('0') << std::setw(16) << hash;
        return oss.str();
#endif
    }

    /**
     * @brief Write entry to audit log file
     * @param json_entry JSON audit entry
     * @param signature HMAC signature (optional)
     */
    static void write_to_audit_log(
        const std::string& json_entry,
        const std::string& signature
    ) {
        std::ofstream file(get_audit_file(), std::ios::app);
        if (!file) {
            // Cannot log audit event - this is a critical failure
            // In production, consider alternative logging (syslog, etc.)
            return;
        }

        // Write JSON entry
        file << json_entry;

        // Write signature if present
        if (!signature.empty()) {
            file << "\nSIGNATURE:" << signature;
        }

        file << "\n";
        file.flush();
    }

    // Singleton storage for configuration
    static std::mutex& get_mutex() {
        static std::mutex mutex;
        return mutex;
    }

    static std::filesystem::path& get_audit_file() {
        static std::filesystem::path path;
        return path;
    }

    static std::unique_ptr<secure_key>& get_hmac_key() {
        static std::unique_ptr<secure_key> key;
        return key;
    }
};

} // namespace kcenon::logger::security
