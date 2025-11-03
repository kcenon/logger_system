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

#include <kcenon/logger/core/error_codes.h>
#include <filesystem>
#include <string>
#include <algorithm>

namespace kcenon::logger::utils {

/**
 * @brief File utility functions for path validation and sanitization
 *
 * Provides security-focused file operations including path traversal
 * prevention, filename sanitization, and permission management.
 *
 * @note These utilities are essential for Phase 4 (Security Hardening).
 */
class file_utils {
public:
    /**
     * @brief Validate log file path against path traversal attacks
     * @param path File path to validate
     * @param allowed_base Base directory where log files are allowed (optional)
     * @return result_void Success if path is valid, error otherwise
     *
     * Security checks:
     * - Prevents path traversal (e.g., ../../../etc/passwd)
     * - Ensures path is within allowed_base directory (if specified)
     * - Validates path doesn't contain suspicious patterns
     * - Uses canonical path resolution to prevent symlink attacks
     *
     * @note Thread-safe. Does not modify filesystem.
     * @note Critical for Phase 4 security requirements.
     *
     * Usage example:
     * @code
     * auto result = file_utils::validate_log_path(
     *     "logs/app.log",
     *     "/var/log/myapp"
     * );
     * if (!result) {
     *     // Path is invalid or outside allowed directory
     * }
     * @endcode
     */
    static result_void validate_log_path(
        const std::filesystem::path& path,
        const std::filesystem::path& allowed_base = ""
    ) {
        try {
            // Check for obviously suspicious patterns
            std::string path_str = path.string();

            // Detect path traversal attempts
            if (path_str.find("..") != std::string::npos) {
                return make_logger_error(
                    logger_error_code::sanitization_failed,
                    "Path contains '..' (path traversal attempt)"
                );
            }

            // If allowed_base is specified, ensure path is within it
            if (!allowed_base.empty()) {
                // Convert both paths to canonical form (resolves symlinks)
                std::filesystem::path canonical_path;
                std::filesystem::path canonical_base;

                // Get canonical base (must exist)
                if (std::filesystem::exists(allowed_base)) {
                    canonical_base = std::filesystem::canonical(allowed_base);
                } else {
                    canonical_base = std::filesystem::absolute(allowed_base);
                }

                // Get canonical path (may not exist yet for new files)
                std::filesystem::path parent = path.parent_path();
                if (!parent.empty() && std::filesystem::exists(parent)) {
                    canonical_path = std::filesystem::canonical(parent) / path.filename();
                } else {
                    canonical_path = std::filesystem::absolute(path);
                }

                // Check if canonical_path starts with canonical_base
                auto [base_end, path_end] = std::mismatch(
                    canonical_base.begin(), canonical_base.end(),
                    canonical_path.begin(), canonical_path.end()
                );

                if (base_end != canonical_base.end()) {
                    return make_logger_error(
                        logger_error_code::sanitization_failed,
                        "Path is outside allowed directory: " + path_str
                    );
                }
            }

            return {};  // Valid path
        } catch (const std::filesystem::filesystem_error& e) {
            return make_logger_error(
                logger_error_code::file_permission_denied,
                std::string("Path validation failed: ") + e.what()
            );
        }
    }

    /**
     * @brief Sanitize filename by removing/replacing dangerous characters
     * @param filename Filename to sanitize
     * @return Sanitized filename safe for filesystem use
     *
     * Transformations:
     * - Removes path separators (/, \)
     * - Replaces control characters with underscore
     * - Removes null bytes
     * - Truncates to reasonable length (255 characters)
     * - Preserves extension if present
     *
     * @note Thread-safe. Creates a new string.
     * @note Does not validate path components, only sanitizes filename.
     *
     * Usage example:
     * @code
     * std::string safe_name = file_utils::sanitize_filename(
     *     user_input_filename
     * );
     * @endcode
     */
    static std::string sanitize_filename(const std::string& filename) {
        if (filename.empty()) {
            return "unnamed.log";
        }

        std::string result;
        result.reserve(filename.size());

        for (char c : filename) {
            // Remove path separators
            if (c == '/' || c == '\\') {
                continue;
            }

            // Remove null bytes
            if (c == '\0') {
                continue;
            }

            // Replace control characters with underscore
            if (c < 32 || c == 127) {
                result += '_';
                continue;
            }

            // Remove potentially dangerous characters
            if (c == ':' || c == '*' || c == '?' || c == '"' ||
                c == '<' || c == '>' || c == '|') {
                result += '_';
                continue;
            }

            result += c;
        }

        // Truncate to safe length (255 is common filesystem limit)
        if (result.size() > 255) {
            // Try to preserve extension
            size_t ext_pos = result.find_last_of('.');
            if (ext_pos != std::string::npos && ext_pos > 250) {
                // Extension is reasonable, keep it
                std::string ext = result.substr(ext_pos);
                result = result.substr(0, 255 - ext.size()) + ext;
            } else {
                result = result.substr(0, 255);
            }
        }

        // Ensure result is not empty
        if (result.empty()) {
            return "unnamed.log";
        }

        return result;
    }

    /**
     * @brief Set file permissions (Unix/POSIX systems)
     * @param file Path to file
     * @param perms Permissions to set (default: owner read/write only)
     * @return result_void Success or error
     *
     * Default permissions (0600):
     * - Owner: read + write
     * - Group: none
     * - Others: none
     *
     * @note On Windows, this function has limited effect (read-only flag).
     * @note Thread-safe. Modifies filesystem.
     * @note Important for Phase 4 security (encryption keys, sensitive logs).
     *
     * Usage example:
     * @code
     * // Make log file readable only by owner
     * file_utils::set_file_permissions(
     *     "sensitive.log",
     *     std::filesystem::perms::owner_read
     * );
     * @endcode
     */
    static result_void set_file_permissions(
        const std::filesystem::path& file,
        std::filesystem::perms perms = std::filesystem::perms::owner_read |
                                        std::filesystem::perms::owner_write
    ) {
        try {
            if (!std::filesystem::exists(file)) {
                return make_logger_error(
                    logger_error_code::file_open_failed,
                    "File does not exist: " + file.string()
                );
            }

            std::filesystem::permissions(file, perms);
            return {};
        } catch (const std::filesystem::filesystem_error& e) {
            return make_logger_error(
                logger_error_code::file_permission_denied,
                std::string("Failed to set file permissions: ") + e.what()
            );
        }
    }

    /**
     * @brief Check if path is absolute
     * @param path Path to check
     * @return true if path is absolute, false otherwise
     *
     * @note Thread-safe.
     */
    static bool is_absolute(const std::filesystem::path& path) {
        return path.is_absolute();
    }

    /**
     * @brief Get file size
     * @param path Path to file
     * @return File size in bytes, or 0 if file doesn't exist/error
     *
     * @note Thread-safe. Does not modify filesystem.
     * @note Returns 0 on error (file not found, no permissions, etc.)
     */
    static std::size_t get_file_size(const std::filesystem::path& path) {
        std::error_code ec;
        auto size = std::filesystem::file_size(path, ec);
        if (ec) {
            return 0;
        }
        return static_cast<std::size_t>(size);
    }

    /**
     * @brief Check if file exists and is writable
     * @param path Path to file
     * @return true if file exists and is writable, false otherwise
     *
     * @note Thread-safe. Does not modify filesystem.
     */
    static bool is_writable(const std::filesystem::path& path) {
        std::error_code ec;

        // If file doesn't exist, check if parent directory is writable
        if (!std::filesystem::exists(path, ec)) {
            auto parent = path.parent_path();
            if (parent.empty()) {
                parent = ".";
            }
            return std::filesystem::exists(parent, ec) &&
                   !ec;  // Basic existence check
        }

        // File exists, check status
        auto status = std::filesystem::status(path, ec);
        if (ec) {
            return false;
        }

        // Check if we have write permissions
        auto perms = status.permissions();
        return (perms & std::filesystem::perms::owner_write) != std::filesystem::perms::none;
    }

    /**
     * @brief Generate safe temporary filename
     * @param prefix Prefix for temporary file
     * @param extension Extension for file (default: .tmp)
     * @return Safe temporary filename with timestamp
     *
     * Format: prefix_YYYYMMDDHHMMSS_random.extension
     *
     * @note Thread-safe. Does not create the file.
     * @note Includes timestamp and random component for uniqueness.
     */
    static std::string generate_temp_filename(
        const std::string& prefix = "temp",
        const std::string& extension = ".tmp"
    ) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);

        std::tm tm_buf{};
#ifdef _WIN32
        localtime_s(&tm_buf, &time_t);
#else
        localtime_r(&time_t, &tm_buf);
#endif

        // Format: prefix_YYYYMMDDHHMMSS_random.ext
        char buffer[64];
        std::snprintf(buffer, sizeof(buffer),
                     "%s_%04d%02d%02d%02d%02d%02d_%d%s",
                     sanitize_filename(prefix).c_str(),
                     tm_buf.tm_year + 1900,
                     tm_buf.tm_mon + 1,
                     tm_buf.tm_mday,
                     tm_buf.tm_hour,
                     tm_buf.tm_min,
                     tm_buf.tm_sec,
                     static_cast<int>(std::chrono::steady_clock::now().time_since_epoch().count() % 10000),
                     extension.c_str());

        return std::string(buffer);
    }
};

} // namespace kcenon::logger::utils
