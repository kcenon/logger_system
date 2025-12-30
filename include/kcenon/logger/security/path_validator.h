#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/core/error_codes.h>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace kcenon::logger::security {

/**
 * @class path_validator
 * @brief Validates file paths to prevent security vulnerabilities
 *
 * Security features:
 * - Path traversal prevention (../ attacks)
 * - Symbolic link validation
 * - Filename character restrictions
 * - Base directory enforcement
 */
class path_validator {
public:
    /**
     * @brief Construct with allowed base directory
     * @param allowed_base All paths must be within this directory
     */
    explicit path_validator(std::filesystem::path allowed_base)
        : allowed_base_(std::move(allowed_base)) {

        // Convert to canonical path if it exists
        try {
            if (std::filesystem::exists(allowed_base_)) {
                allowed_base_ = std::filesystem::canonical(allowed_base_);
            } else {
                // Use weakly_canonical for non-existent paths
                allowed_base_ = std::filesystem::weakly_canonical(allowed_base_);
            }
        } catch (const std::filesystem::filesystem_error&) {
            // If canonicalization fails, use as-is
            // Validation will catch issues later
        }
    }

    /**
     * @brief Validate a file path
     * @param path Path to validate
     * @param allow_symlinks Whether symbolic links are allowed (default: false)
     * @param strict_filename Whether to enforce strict filename rules (default: true)
     * @return Success or error describing the security issue
     */
    common::VoidResult validate(
        const std::filesystem::path& path,
        bool allow_symlinks = false,
        bool strict_filename = true
    ) const {
        try {
            // 1. Convert to absolute path and resolve relative components
            std::filesystem::path canonical;
            if (std::filesystem::exists(path)) {
                canonical = std::filesystem::canonical(path);
            } else {
                canonical = std::filesystem::weakly_canonical(path);
            }

            // 2. Check for symbolic links (if not allowed)
            if (!allow_symlinks && std::filesystem::exists(path)) {
                if (std::filesystem::is_symlink(path)) {
                    return make_logger_void_result(
                        logger_error_code::path_traversal_detected,
                        "Symbolic links are not allowed for security reasons"
                    );
                }
            }

            // 3. Verify path is within allowed base directory
            auto [base_end, path_end] = std::mismatch(
                allowed_base_.begin(), allowed_base_.end(),
                canonical.begin(), canonical.end()
            );

            if (base_end != allowed_base_.end()) {
                return make_logger_void_result(
                    logger_error_code::path_traversal_detected,
                    "Path must be within allowed directory: " + allowed_base_.string()
                );
            }

            // 4. Validate filename (if strict mode enabled)
            if (strict_filename && path.has_filename()) {
                auto filename = path.filename().string();

                if (!is_safe_filename(filename)) {
                    return make_logger_void_result(
                        logger_error_code::invalid_filename,
                        "Filename contains invalid or potentially dangerous characters"
                    );
                }
            }

            return common::ok();

        } catch (const std::filesystem::filesystem_error& e) {
            return make_logger_void_result(
                logger_error_code::path_traversal_detected,
                std::string("Path validation failed: ") + e.what()
            );
        }
    }

    /**
     * @brief Get the allowed base directory
     */
    const std::filesystem::path& allowed_base() const {
        return allowed_base_;
    }

    /**
     * @brief Check if a filename is safe (contains only allowed characters)
     * @param name Filename to check
     * @return true if safe, false otherwise
     *
     * Allowed characters:
     * - Alphanumeric (a-z, A-Z, 0-9)
     * - Hyphen (-)
     * - Underscore (_)
     * - Period (.)
     */
    static bool is_safe_filename(const std::string& name) {
        // Empty or special names are not allowed
        if (name.empty() || name == "." || name == "..") {
            return false;
        }

        // Check each character
        for (char c : name) {
            if (!std::isalnum(static_cast<unsigned char>(c)) &&
                c != '-' && c != '_' && c != '.') {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Sanitize a filename by removing/replacing invalid characters
     * @param name Filename to sanitize
     * @param replacement Character to use for invalid characters (default: '_')
     * @return Sanitized filename
     */
    static std::string sanitize_filename(
        const std::string& name,
        char replacement = '_'
    ) {
        // Handle special cases
        if (name.empty()) {
            return "unnamed";
        }
        if (name == ".") {
            return std::string(1, replacement) + name;
        }
        if (name == "..") {
            return std::string(1, replacement) + '.';
        }

        std::string result;
        result.reserve(name.size());

        for (char c : name) {
            if (std::isalnum(static_cast<unsigned char>(c)) ||
                c == '-' || c == '_' || c == '.') {
                result += c;
            } else {
                result += replacement;
            }
        }

        return result;
    }

    /**
     * @brief Create a safe path by joining base and relative path
     * @param base Base directory
     * @param relative Relative path to join
     * @return Validated absolute path or error
     */
    static result<std::filesystem::path> safe_join(
        const std::filesystem::path& base,
        const std::filesystem::path& relative
    ) {
        try {
            // Ensure relative path doesn't contain absolute components
            if (relative.is_absolute()) {
                return result<std::filesystem::path>{
                    logger_error_code::path_traversal_detected,
                    "Cannot join with absolute path"};
            }

            // Join paths
            auto joined = base / relative;

            // Validate the result
            path_validator validator(base);
            auto validation = validator.validate(joined);

            if (validation.is_err()) {
                return result<std::filesystem::path>{
                    get_logger_error_code(validation),
                    get_logger_error_message(validation)};
            }

            return result<std::filesystem::path>(joined);

        } catch (const std::filesystem::filesystem_error& e) {
            return result<std::filesystem::path>{
                logger_error_code::path_traversal_detected,
                std::string("Path join failed: ") + e.what()};
        }
    }

private:
    std::filesystem::path allowed_base_;
};

} // namespace kcenon::logger::security
