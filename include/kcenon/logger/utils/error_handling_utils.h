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
#include <kcenon/common/config/feature_flags.h>
#include <filesystem>
#include <system_error>
#include <chrono>
#include <sstream>
#include <iostream>

#if KCENON_HAS_SOURCE_LOCATION
#  include <source_location>
#endif

// Legacy alias for backward compatibility
#ifndef LOGGER_HAS_SOURCE_LOCATION
#  define LOGGER_HAS_SOURCE_LOCATION KCENON_HAS_SOURCE_LOCATION
#endif

namespace kcenon::logger::utils {

/**
 * @brief Structured error context for debugging
 *
 * Provides additional context information for errors to aid in debugging
 * and troubleshooting. This context can be used to track where and when
 * errors occurred.
 *
 * When C++20 source_location is available, location information is captured
 * automatically. Otherwise, it can be provided manually.
 */
struct error_context {
    logger_error_code code;
    std::string message;
    std::string operation;     // Operation being performed
    std::string source_file;   // Source file where error occurred
    int source_line;           // Line number where error occurred
    std::string function_name; // Function where error occurred
    std::chrono::system_clock::time_point timestamp;

#if KCENON_HAS_SOURCE_LOCATION
    /**
     * @brief Construct error context with automatic source location capture
     * @param error_code The logger error code
     * @param error_message Detailed error message
     * @param op Operation being performed (optional)
     * @param loc Source location (automatically captured)
     */
    error_context(
        logger_error_code error_code,
        std::string error_message,
        std::string op = "",
        std::source_location loc = std::source_location::current()
    ) : code(error_code),
        message(std::move(error_message)),
        operation(std::move(op)),
        source_file(loc.file_name()),
        source_line(static_cast<int>(loc.line())),
        function_name(loc.function_name()),
        timestamp(std::chrono::system_clock::now()) {}
#else
    /**
     * @brief Construct error context with manual location information
     * @param error_code The logger error code
     * @param error_message Detailed error message
     * @param op Operation being performed (optional)
     * @param file Source file name (optional)
     * @param line Source line number (optional)
     */
    error_context(
        logger_error_code error_code,
        std::string error_message,
        std::string op = "",
        std::string file = "",
        int line = 0
    ) : code(error_code),
        message(std::move(error_message)),
        operation(std::move(op)),
        source_file(std::move(file)),
        source_line(line),
        function_name(""),
        timestamp(std::chrono::system_clock::now()) {}
#endif

    /**
     * @brief Convert error context to a formatted string
     * @return Formatted error message with context
     */
    std::string to_string() const {
        std::ostringstream oss;
        oss << "[" << logger_error_to_string(code) << "]";
        if (!message.empty()) {
            oss << " " << message;
        }
        if (!operation.empty()) {
            oss << " (during: " << operation << ")";
        }
        if (!source_file.empty()) {
            oss << " at " << source_file;
            if (source_line > 0) {
                oss << ":" << source_line;
            }
        }
        if (!function_name.empty()) {
            oss << " in " << function_name << "()";
        }
        return oss.str();
    }
};

/**
 * @brief Log error to stderr with context
 *
 * Used primarily in destructors where throwing exceptions is not allowed.
 * Provides diagnostic information without propagating errors.
 *
 * @param context Error context to log
 */
inline void log_error_context(const error_context& context) {
    std::cerr << "[logger_system] Error: " << context.to_string() << std::endl;
}

/**
 * @brief Error handling helper for write operations
 *
 * This function wraps write operations with comprehensive exception handling,
 * mapping different exception types to appropriate logger error codes.
 *
 * Usage example:
 * @code
 * result_void write(const log_entry& entry) {
 *     return try_write_operation([&]() -> result_void {
 *         // Perform write operation
 *         file_stream_ << formatted << '\n';
 *         return {};
 *     });
 * }
 * @endcode
 *
 * @tparam F Callable type that returns result_void
 * @param operation The operation to execute
 * @param default_error_code Error code to use for unexpected exceptions (default: file_write_failed)
 * @return result_void Success or error result
 */
template<typename F>
result_void try_write_operation(
    F&& operation,
    logger_error_code default_error_code = logger_error_code::file_write_failed
) {
    try {
        return operation();
    }
    catch (const std::filesystem::filesystem_error& e) {
        // Filesystem-specific errors (permission denied, disk full, etc.)
        return make_logger_error(
            logger_error_code::file_permission_denied,
            std::string("Filesystem error: ") + e.what()
        );
    }
    catch (const std::ios_base::failure& e) {
        // I/O operation failures (stream errors, write errors, etc.)
        return make_logger_error(
            logger_error_code::file_write_failed,
            std::string("I/O error: ") + e.what()
        );
    }
    catch (const std::system_error& e) {
        // System-level errors
        return make_logger_error(
            default_error_code,
            std::string("System error: ") + e.what()
        );
    }
    catch (const std::bad_alloc& e) {
        // Memory allocation failures
        return make_logger_error(
            logger_error_code::buffer_overflow,
            std::string("Memory allocation failed: ") + e.what()
        );
    }
    catch (const std::exception& e) {
        // Generic exception catch-all
        return make_logger_error(
            default_error_code,
            std::string("Unexpected error: ") + e.what()
        );
    }
    catch (...) {
        // Non-standard exception
        return make_logger_error(
            default_error_code,
            "Unknown error (non-standard exception)"
        );
    }
}

/**
 * @brief Error handling helper for file open operations
 *
 * Specialized version of try_write_operation for file open operations.
 * Uses file_open_failed as the default error code.
 *
 * @tparam F Callable type that returns result_void
 * @param operation The operation to execute
 * @return result_void Success or error result
 */
template<typename F>
result_void try_open_operation(F&& operation) {
    return try_write_operation(
        std::forward<F>(operation),
        logger_error_code::file_open_failed
    );
}

/**
 * @brief Error handling helper for network operations
 *
 * Specialized version of try_write_operation for network operations.
 * Uses network_send_failed as the default error code.
 *
 * @tparam F Callable type that returns result_void
 * @param operation The operation to execute
 * @return result_void Success or error result
 */
template<typename F>
result_void try_network_operation(F&& operation) {
    return try_write_operation(
        std::forward<F>(operation),
        logger_error_code::network_send_failed
    );
}

/**
 * @brief Error handling helper for encryption operations
 *
 * Specialized version of try_write_operation for encryption operations.
 * Uses encryption_failed as the default error code.
 *
 * @tparam F Callable type that returns result_void
 * @param operation The operation to execute
 * @return result_void Success or error result
 */
template<typename F>
result_void try_encryption_operation(F&& operation) {
    return try_write_operation(
        std::forward<F>(operation),
        logger_error_code::encryption_failed
    );
}

/**
 * @brief Condition verification helper
 *
 * Checks a boolean condition and returns an error if the condition is false.
 * This helper reduces boilerplate code for precondition checks.
 *
 * Usage example:
 * @code
 * auto check = check_condition(
 *     file_stream_.is_open(),
 *     logger_error_code::file_write_failed,
 *     "File stream is not open"
 * );
 * if (!check) return check;
 * @endcode
 *
 * @param condition The condition to check
 * @param error_code Error code to return if condition is false
 * @param message Error message
 * @return result_void Success if condition is true, error otherwise
 */
inline result_void check_condition(
    bool condition,
    logger_error_code error_code,
    const std::string& message
) {
    if (!condition) {
        return make_logger_error(error_code, message);
    }
    return {};
}

/**
 * @brief Stream state verification helper
 *
 * Checks if a stream is in a good state and returns an appropriate error if not.
 * This is a specialized version of check_condition for stream operations.
 *
 * @tparam Stream Stream type (e.g., std::ofstream, std::ifstream)
 * @param stream The stream to check
 * @param operation_name Name of the operation for error message (e.g., "write", "open")
 * @return result_void Success if stream is good, error otherwise
 */
template<typename Stream>
inline result_void check_stream_state(
    const Stream& stream,
    const std::string& operation_name = "operation"
) {
    if (!stream.good()) {
        if (stream.eof()) {
            return make_logger_error(
                logger_error_code::file_write_failed,
                "Stream error: Unexpected end of file during " + operation_name
            );
        }
        if (stream.fail()) {
            return make_logger_error(
                logger_error_code::file_write_failed,
                "Stream error: Logical error during " + operation_name
            );
        }
        if (stream.bad()) {
            return make_logger_error(
                logger_error_code::file_write_failed,
                "Stream error: Read/write error during " + operation_name
            );
        }
        return make_logger_error(
            logger_error_code::file_write_failed,
            "Stream is in an error state after " + operation_name
        );
    }
    return {};
}

/**
 * @brief File existence verification helper
 *
 * Checks if a file exists at the given path.
 *
 * @param path The file path to check
 * @return result_void Success if file exists, error otherwise
 */
inline result_void check_file_exists(const std::filesystem::path& path) {
    try {
        if (!std::filesystem::exists(path)) {
            return make_logger_error(
                logger_error_code::file_open_failed,
                "File does not exist: " + path.string()
            );
        }
        return {};
    } catch (const std::filesystem::filesystem_error& e) {
        return make_logger_error(
            logger_error_code::file_permission_denied,
            std::string("Cannot access file: ") + e.what()
        );
    }
}

/**
 * @brief Directory creation helper
 *
 * Creates a directory if it doesn't exist, with proper error handling.
 *
 * @param dir The directory path to create
 * @return result_void Success if directory exists or was created, error otherwise
 */
inline result_void ensure_directory_exists(const std::filesystem::path& dir) {
    if (dir.empty()) {
        return {}; // Empty path is valid (current directory)
    }

    return try_open_operation([&]() -> result_void {
        if (!std::filesystem::exists(dir)) {
            if (!std::filesystem::create_directories(dir)) {
                return make_logger_error(
                    logger_error_code::file_permission_denied,
                    "Failed to create directory: " + dir.string()
                );
            }
        }
        return {};
    });
}

/**
 * @brief Safe operation execution for destructors
 *
 * Executes an operation in a destructor-safe manner. Exceptions are caught
 * and logged to stderr instead of being propagated, preventing std::terminate.
 *
 * This function should be used in destructors where throwing exceptions
 * would be unsafe.
 *
 * Usage example:
 * @code
 * ~my_writer() {
 *     safe_destructor_operation("flush", [this]() {
 *         flush();
 *     });
 * }
 * @endcode
 *
 * @tparam F Callable type
 * @param operation_name Name of the operation for logging
 * @param operation The operation to execute
 */
template<typename F>
inline void safe_destructor_operation(
    const std::string& operation_name,
    F&& operation
) noexcept {
    try {
        operation();
    }
    catch (const std::exception& e) {
        error_context ctx(
            logger_error_code::destructor_cleanup_failed,
            e.what(),
            operation_name
        );
        log_error_context(ctx);
    }
    catch (...) {
        error_context ctx(
            logger_error_code::destructor_cleanup_failed,
            "Unknown exception",
            operation_name
        );
        log_error_context(ctx);
    }
}

/**
 * @brief Safe operation with result for destructors
 *
 * Similar to safe_destructor_operation but for operations that return result_void.
 * Logs both exceptions and result errors.
 *
 * @tparam F Callable type that returns result_void
 * @param operation_name Name of the operation for logging
 * @param operation The operation to execute
 */
template<typename F>
inline void safe_destructor_result_operation(
    const std::string& operation_name,
    F&& operation
) noexcept {
    try {
        auto result = operation();
        if (!result) {
            error_context ctx(
                result.error_code(),
                result.error_message(),
                operation_name
            );
            log_error_context(ctx);
        }
    }
    catch (const std::exception& e) {
        error_context ctx(
            logger_error_code::destructor_cleanup_failed,
            e.what(),
            operation_name
        );
        log_error_context(ctx);
    }
    catch (...) {
        error_context ctx(
            logger_error_code::destructor_cleanup_failed,
            "Unknown exception",
            operation_name
        );
        log_error_context(ctx);
    }
}

} // namespace kcenon::logger::utils
