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

#include <string>
#include <sstream>
#include <iomanip>

// Always use logger_system interface (Phase 3-5)
#include <kcenon/logger/interfaces/logger_interface.h>

namespace kcenon::logger::utils {

/**
 * @brief String utility functions for log formatting and conversion
 *
 * Provides common string manipulation functions used across formatters
 * and writers, including escaping, conversion, and extraction utilities.
 */
class string_utils {
public:
    /**
     * @brief Convert log level to human-readable string
     * @param level Log level to convert
     * @return String representation of log level
     *
     * Output format: "CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE", "OFF"
     *
     * @note Thread-safe and stateless.
     */
    static std::string level_to_string(logger_system::log_level level) {
        switch (level) {
            case logger_system::log_level::fatal:   return "CRITICAL";
            case logger_system::log_level::error:   return "ERROR";
            case logger_system::log_level::warn:    return "WARNING";
            case logger_system::log_level::info:    return "INFO";
            case logger_system::log_level::debug:   return "DEBUG";
            case logger_system::log_level::trace:   return "TRACE";
            case logger_system::log_level::off:     return "OFF";
        }
        return "UNKNOWN";
    }

    /**
     * @brief Convert log level to ANSI color code
     * @param level Log level to convert
     * @param use_colors Whether to return colors (if false, returns empty string)
     * @return ANSI escape sequence for the log level color
     *
     * Color mapping:
     * - CRITICAL/FATAL: Bright Magenta
     * - ERROR: Bright Red
     * - WARNING: Bright Yellow
     * - INFO: Bright Green
     * - DEBUG: Bright Cyan
     * - TRACE: White
     *
     * @note Use with "\033[0m" to reset color after output.
     * @note Thread-safe and stateless.
     */
    static std::string level_to_color(logger_system::log_level level, bool use_colors = true) {
        if (!use_colors) {
            return "";
        }

        switch (level) {
            case logger_system::log_level::fatal:   return "\033[1;35m"; // Bright Magenta
            case logger_system::log_level::error:   return "\033[1;31m"; // Bright Red
            case logger_system::log_level::warn:    return "\033[1;33m"; // Bright Yellow
            case logger_system::log_level::info:    return "\033[1;32m"; // Bright Green
            case logger_system::log_level::debug:   return "\033[1;36m"; // Bright Cyan
            case logger_system::log_level::trace:   return "\033[37m";   // White
            case logger_system::log_level::off:     return "\033[90m";   // Dark Gray
        }
        return "";
    }

    /**
     * @brief ANSI color reset sequence
     * @return ANSI escape sequence to reset colors
     *
     * @note Use after colored output to reset terminal colors.
     */
    static const char* color_reset() {
        return "\033[0m";
    }

    /**
     * @brief Escape special characters for JSON
     * @param str String to escape
     * @return JSON-escaped string safe for inclusion in JSON documents
     *
     * Escaped characters:
     * - " → \"
     * - \ → \\
     * - / → \/
     * - \b, \f, \n, \r, \t → respective escape sequences
     * - Control characters (0x00-0x1F) → \uXXXX
     *
     * @note Thread-safe. Creates a new string with escaped content.
     * @note Compatible with JSON parsers and log aggregation systems.
     */
    static std::string escape_json(const std::string& str) {
        std::ostringstream oss;
        for (char c : str) {
            switch (c) {
                case '"':  oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '/':  oss << "\\/"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;
                default:
                    // Handle control characters
                    if (c >= 0 && c < 0x20) {
                        oss << "\\u"
                            << std::hex << std::setw(4) << std::setfill('0')
                            << static_cast<int>(c);
                    } else {
                        oss << c;
                    }
                    break;
            }
        }
        return oss.str();
    }

    /**
     * @brief Escape special characters for XML
     * @param str String to escape
     * @return XML-escaped string safe for inclusion in XML documents
     *
     * Escaped characters:
     * - & → &amp;
     * - < → &lt;
     * - > → &gt;
     * - " → &quot;
     * - ' → &apos;
     *
     * @note Thread-safe. Creates a new string with escaped content.
     */
    static std::string escape_xml(const std::string& str) {
        std::ostringstream oss;
        for (char c : str) {
            switch (c) {
                case '&':  oss << "&amp;"; break;
                case '<':  oss << "&lt;"; break;
                case '>':  oss << "&gt;"; break;
                case '"':  oss << "&quot;"; break;
                case '\'': oss << "&apos;"; break;
                default:   oss << c; break;
            }
        }
        return oss.str();
    }

    /**
     * @brief Extract filename from full file path
     * @param file_path Full path to file (may include directories)
     * @return Just the filename without directory components
     *
     * Examples:
     * - "/path/to/file.cpp" → "file.cpp"
     * - "C:\\path\\to\\file.cpp" → "file.cpp"
     * - "file.cpp" → "file.cpp"
     *
     * @note Works with both Unix (/) and Windows (\) path separators.
     * @note Thread-safe.
     */
    static std::string extract_filename(const std::string& file_path) {
        if (file_path.empty()) {
            return file_path;
        }

        size_t pos = file_path.find_last_of("/\\");
        if (pos != std::string::npos) {
            return file_path.substr(pos + 1);
        }

        return file_path;
    }

    /**
     * @brief Trim whitespace from both ends of string
     * @param str String to trim
     * @return Trimmed string
     *
     * @note Removes spaces, tabs, newlines, and other whitespace characters.
     * @note Thread-safe. Creates a new string.
     */
    static std::string trim(const std::string& str) {
        const char* whitespace = " \t\n\r\f\v";

        size_t start = str.find_first_not_of(whitespace);
        if (start == std::string::npos) {
            return "";  // String is all whitespace
        }

        size_t end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }

    /**
     * @brief Convert string to lowercase
     * @param str String to convert
     * @return Lowercase version of string
     *
     * @note Only converts ASCII characters (A-Z → a-z).
     * @note Thread-safe. Creates a new string.
     */
    static std::string to_lower(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            if (c >= 'A' && c <= 'Z') {
                c = c + ('a' - 'A');
            }
        }
        return result;
    }

    /**
     * @brief Convert string to uppercase
     * @param str String to convert
     * @return Uppercase version of string
     *
     * @note Only converts ASCII characters (a-z → A-Z).
     * @note Thread-safe. Creates a new string.
     */
    static std::string to_upper(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            if (c >= 'a' && c <= 'z') {
                c = c - ('a' - 'A');
            }
        }
        return result;
    }

    /**
     * @brief Replace all occurrences of a substring
     * @param str Source string
     * @param from Substring to replace
     * @param to Replacement substring
     * @return String with all replacements made
     *
     * @note If 'from' is empty, returns original string.
     * @note Thread-safe. Creates a new string.
     */
    static std::string replace_all(
        const std::string& str,
        const std::string& from,
        const std::string& to
    ) {
        if (from.empty()) {
            return str;
        }

        std::string result = str;
        size_t pos = 0;

        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }

        return result;
    }
};

} // namespace kcenon::logger::utils
