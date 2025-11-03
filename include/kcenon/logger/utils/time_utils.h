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

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace kcenon::logger::utils {

/**
 * @brief Time utility functions for timestamp formatting
 *
 * Provides thread-safe timestamp formatting functions in various formats
 * commonly used in logging systems.
 */
class time_utils {
public:
    /**
     * @brief Format timestamp to human-readable format (YYYY-MM-DD HH:MM:SS.mmm)
     * @param tp Time point to format
     * @return Formatted timestamp string with millisecond precision
     *
     * Output format: "2025-11-03 14:30:15.123"
     *
     * @note Thread-safe. Uses platform-specific thread-safe time conversion.
     */
    static std::string format_timestamp(
        const std::chrono::system_clock::time_point& tp
    ) {
        auto time_t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_buf{};

#ifdef _WIN32
        localtime_s(&tm_buf, &time_t);  // Windows thread-safe version
#else
        localtime_r(&time_t, &tm_buf);  // POSIX thread-safe version
#endif

        // Format base timestamp
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_buf);

        // Add milliseconds
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()
        ) % 1000;

        std::ostringstream oss;
        oss << buffer << "."
            << std::setfill('0') << std::setw(3) << ms.count();

        return oss.str();
    }

    /**
     * @brief Format timestamp to ISO 8601 / RFC 3339 format with UTC timezone
     * @param tp Time point to format
     * @return ISO 8601 formatted timestamp string
     *
     * Output format: "2025-11-03T14:30:15.123Z"
     *
     * @note Thread-safe. Always outputs in UTC (Z timezone indicator).
     * @note Compatible with JSON parsers and log aggregation systems (ELK, Splunk, etc.)
     */
    static std::string format_iso8601(
        const std::chrono::system_clock::time_point& tp
    ) {
        auto time_t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_buf{};

#ifdef _WIN32
        gmtime_s(&tm_buf, &time_t);  // Windows thread-safe version (UTC)
#else
        gmtime_r(&time_t, &tm_buf);  // POSIX thread-safe version (UTC)
#endif

        // Format base timestamp (ISO 8601)
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &tm_buf);

        // Add milliseconds
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()
        ) % 1000;

        std::ostringstream oss;
        oss << buffer << "."
            << std::setfill('0') << std::setw(3) << ms.count()
            << "Z";  // UTC timezone indicator

        return oss.str();
    }

    /**
     * @brief Format timestamp to compact format (YYYYMMDDHHMMSSmmm)
     * @param tp Time point to format
     * @return Compact timestamp string without separators
     *
     * Output format: "20251103143015123"
     *
     * @note Useful for filename generation and sorting.
     * @note Thread-safe.
     */
    static std::string format_compact(
        const std::chrono::system_clock::time_point& tp
    ) {
        auto time_t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_buf{};

#ifdef _WIN32
        localtime_s(&tm_buf, &time_t);
#else
        localtime_r(&time_t, &tm_buf);
#endif

        // Format compact timestamp
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", &tm_buf);

        // Add milliseconds
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()
        ) % 1000;

        std::ostringstream oss;
        oss << buffer
            << std::setfill('0') << std::setw(3) << ms.count();

        return oss.str();
    }

    /**
     * @brief Format timestamp for file rotation (YYYYMMDD or YYYYMMDD_HH)
     * @param tp Time point to format
     * @param include_hour Whether to include hour in the format
     * @return Formatted timestamp suitable for file naming
     *
     * Output formats:
     * - Without hour: "20251103"
     * - With hour: "20251103_14"
     *
     * @note Useful for rotating file writers.
     * @note Thread-safe.
     */
    static std::string format_for_rotation(
        const std::chrono::system_clock::time_point& tp,
        bool include_hour = false
    ) {
        auto time_t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_buf{};

#ifdef _WIN32
        localtime_s(&tm_buf, &time_t);
#else
        localtime_r(&time_t, &tm_buf);
#endif

        char buffer[32];
        if (include_hour) {
            std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H", &tm_buf);
        } else {
            std::strftime(buffer, sizeof(buffer), "%Y%m%d", &tm_buf);
        }

        return std::string(buffer);
    }

    /**
     * @brief Get current system time as time_point
     * @return Current time_point
     *
     * @note Convenience function for getting current time.
     */
    static std::chrono::system_clock::time_point now() {
        return std::chrono::system_clock::now();
    }
};

} // namespace kcenon::logger::utils
