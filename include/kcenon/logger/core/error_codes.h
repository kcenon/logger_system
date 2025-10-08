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
#include <stdexcept>
#include <utility>
#include <optional>

#if __has_include(<kcenon/common/patterns/result.h>)
#include <kcenon/common/patterns/result.h>
#define KCENON_COMMON_RESULT_AVAILABLE 1
#elif __has_include(<common/patterns/result.h>)
#include <common/patterns/result.h>
#define KCENON_COMMON_RESULT_AVAILABLE 1
#ifndef KCENON_COMMON_RESULT_FALLBACK_DEFINED
#define KCENON_COMMON_RESULT_FALLBACK_DEFINED
namespace kcenon {
namespace common {
using common::error_info;
template<typename T>
using Result = common::Result<T>;
using VoidResult = common::VoidResult;
using common::ok;
using common::error;
using common::is_ok;
using common::is_error;
using common::get_value;
using common::get_error;
namespace error_codes {
using namespace common::error_codes;
} // namespace error_codes
} // namespace common
} // namespace kcenon
#endif // KCENON_COMMON_RESULT_FALLBACK_DEFINED
#else
#ifndef KCENON_COMMON_RESULT_FALLBACK_DEFINED
#define KCENON_COMMON_RESULT_FALLBACK_DEFINED
namespace kcenon {
namespace common {

struct error_info {
    int code;
    std::string message;
    std::string category;
};

inline error_info make_error_info(int code_value, std::string message_value, std::string category_value) {
    return error_info{code_value, std::move(message_value), std::move(category_value)};
}

template<typename T>
class Result {
public:
    Result() = default;

    explicit Result(T value)
        : value_(std::move(value)) {}

    explicit Result(error_info info)
        : error_(std::move(info)) {}

    bool has_value() const { return value_.has_value(); }
    bool has_error() const { return error_.has_value(); }

    T& value() {
        if (!value_) {
            throw std::logic_error("Result does not contain a value");
        }
        return *value_;
    }

    const T& value() const {
        if (!value_) {
            throw std::logic_error("Result does not contain a value");
        }
        return *value_;
    }

    error_info& error() {
        if (!error_) {
            throw std::logic_error("Result does not contain an error");
        }
        return *error_;
    }

    const error_info& error() const {
        if (!error_) {
            throw std::logic_error("Result does not contain an error");
        }
        return *error_;
    }

private:
    std::optional<T> value_;
    std::optional<error_info> error_;
};

class VoidResult {
public:
    VoidResult() = default;

    explicit VoidResult(error_info info)
        : error_(std::move(info)) {}

    bool has_value() const { return !error_.has_value(); }
    bool has_error() const { return error_.has_value(); }

    error_info& error() {
        if (!error_) {
            throw std::logic_error("VoidResult does not contain an error");
        }
        return *error_;
    }

    const error_info& error() const {
        if (!error_) {
            throw std::logic_error("VoidResult does not contain an error");
        }
        return *error_;
    }

private:
    std::optional<error_info> error_;
};

template<typename T>
Result<T> ok(T value) {
    return Result<T>(std::move(value));
}

inline VoidResult ok() {
    return VoidResult{};
}

template<typename T>
Result<T> error(error_info info) {
    return Result<T>(std::move(info));
}

inline VoidResult error(error_info info) {
    return VoidResult(std::move(info));
}

template<typename T>
bool is_ok(const Result<T>& result) {
    return result.has_value();
}

template<typename T>
bool is_error(const Result<T>& result) {
    return result.has_error();
}

inline bool is_ok(const VoidResult& result) {
    return !result.has_error();
}

inline bool is_error(const VoidResult& result) {
    return result.has_error();
}

template<typename T>
T& get_value(Result<T>& result) {
    return result.value();
}

template<typename T>
const T& get_value(const Result<T>& result) {
    return result.value();
}

template<typename T>
error_info& get_error(Result<T>& result) {
    return result.error();
}

template<typename T>
const error_info& get_error(const Result<T>& result) {
    return result.error();
}

inline const error_info& get_error(const VoidResult& result) {
    return result.error();
}

namespace error_codes {
enum class generic_error {
    none = 0
};
} // namespace error_codes

} // namespace common
} // namespace kcenon
#endif // KCENON_COMMON_RESULT_FALLBACK_DEFINED
#endif

namespace kcenon::logger {

/**
 * @enum logger_error_code
 * @brief Error codes specific to the logger system
 * 
 * These error codes extend the thread_module error codes with
 * logger-specific error conditions.
 */
enum class logger_error_code {
    // General errors (0-999)
    success = 0,
    unknown_error = 1,
    not_implemented = 2,
    invalid_argument = 3,
    
    // Writer errors (1000-1099)
    writer_not_found = 1000,
    writer_initialization_failed = 1001,
    writer_already_exists = 1002,
    writer_not_healthy = 1003,
    
    // File errors (1100-1199)
    file_open_failed = 1100,
    file_write_failed = 1101,
    file_rotation_failed = 1102,
    file_permission_denied = 1103,
    
    // Network errors (1200-1299)
    network_connection_failed = 1200,
    network_send_failed = 1201,
    network_timeout = 1202,
    
    // Buffer/Queue errors (1300-1399)
    buffer_overflow = 1300,
    queue_full = 1301,
    queue_stopped = 1302,
    
    // Configuration errors (1400-1499)
    invalid_configuration = 1400,
    configuration_missing = 1401,
    configuration_conflict = 1402,
    
    // Metrics errors (1500-1599)
    metrics_collection_failed = 1500,
    metrics_not_available = 1501,
    
    // Processing errors (1600-1699)
    flush_timeout = 1600,
    processing_failed = 1601,
    filter_error = 1602,
    formatter_error = 1603,
    
    // Security errors (1700-1799)
    encryption_failed = 1700,
    decryption_failed = 1701,
    authentication_failed = 1702,
    sanitization_failed = 1703,
    
    // DI Container errors (1800-1899)
    di_not_available = 1800,
    component_not_found = 1801,
    registration_failed = 1802,
    creation_failed = 1803,
    operation_failed = 1804,

    // Writer errors (1900-1999)
    writer_not_available = 1900,
    writer_configuration_error = 1901,
    writer_operation_failed = 1902
};

/**
 * @brief Convert logger_error_code to string representation
 * @param code The error code to convert
 * @return Human-readable error description
 */
inline std::string logger_error_to_string(logger_error_code code) {
    switch (code) {
        case logger_error_code::success:
            return "Success";
        case logger_error_code::unknown_error:
            return "Unknown error";
        case logger_error_code::not_implemented:
            return "Not implemented";
        case logger_error_code::invalid_argument:
            return "Invalid argument";
            
        // Writer errors
        case logger_error_code::writer_not_found:
            return "Writer not found";
        case logger_error_code::writer_initialization_failed:
            return "Writer initialization failed";
        case logger_error_code::writer_already_exists:
            return "Writer already exists";
        case logger_error_code::writer_not_healthy:
            return "Writer not healthy";
            
        // File errors
        case logger_error_code::file_open_failed:
            return "Failed to open file";
        case logger_error_code::file_write_failed:
            return "Failed to write to file";
        case logger_error_code::file_rotation_failed:
            return "File rotation failed";
        case logger_error_code::file_permission_denied:
            return "File permission denied";
            
        // Network errors
        case logger_error_code::network_connection_failed:
            return "Network connection failed";
        case logger_error_code::network_send_failed:
            return "Network send failed";
        case logger_error_code::network_timeout:
            return "Network timeout";
            
        // Buffer/Queue errors
        case logger_error_code::buffer_overflow:
            return "Buffer overflow";
        case logger_error_code::queue_full:
            return "Queue is full";
        case logger_error_code::queue_stopped:
            return "Queue is stopped";
            
        // Configuration errors
        case logger_error_code::invalid_configuration:
            return "Invalid configuration";
        case logger_error_code::configuration_missing:
            return "Configuration missing";
        case logger_error_code::configuration_conflict:
            return "Configuration conflict";
            
        // Metrics errors
        case logger_error_code::metrics_collection_failed:
            return "Metrics collection failed";
        case logger_error_code::metrics_not_available:
            return "Metrics not available";
            
        // Processing errors
        case logger_error_code::flush_timeout:
            return "Flush timeout";
        case logger_error_code::processing_failed:
            return "Processing failed";
        case logger_error_code::filter_error:
            return "Filter error";
        case logger_error_code::formatter_error:
            return "Formatter error";
            
        // Security errors
        case logger_error_code::encryption_failed:
            return "Encryption failed";
        case logger_error_code::decryption_failed:
            return "Decryption failed";
        case logger_error_code::authentication_failed:
            return "Authentication failed";
        case logger_error_code::sanitization_failed:
            return "Sanitization failed";
            
        // DI Container errors
        case logger_error_code::di_not_available:
            return "DI container not available";
        case logger_error_code::component_not_found:
            return "Component not found in DI container";
        case logger_error_code::registration_failed:
            return "Failed to register component in DI container";
        case logger_error_code::creation_failed:
            return "Failed to create component from factory";
        case logger_error_code::operation_failed:
            return "DI container operation failed";
            
        default:
            return "Unknown logger error code";
    }
}

// Result wrapper built on top of common_system Result pattern
template<typename T>
class result {
public:
    result(T value)
        : value_(common::ok<T>(std::move(value))) {}

    result(const T& value)
        : value_(common::ok<T>(value)) {}

    result(logger_error_code code, const std::string& msg = "")
        : value_(common::error_info(
              static_cast<int>(code),
              msg.empty() ? logger_error_to_string(code) : msg,
              "logger_system")) {}

    // Static factory method to avoid constructor ambiguity
    static result ok_value(const T& value) {
        result res(logger_error_code::success);
        // Directly assign common::Result value
        res.value_ = common::ok<T>(value);
        return res;
    }

    bool has_value() const { return common::is_ok(value_); }
    explicit operator bool() const { return has_value(); }

    T& value() { return common::get_value(value_); }
    const T& value() const { return common::get_value(value_); }

    logger_error_code error_code() const {
        return static_cast<logger_error_code>(common::get_error(value_).code);
    }

    const std::string& error_message() const {
        return common::get_error(value_).message;
    }

    const common::Result<T>& raw() const { return value_; }

private:
    common::Result<T> value_;
};

class result_void {
public:
    result_void() = default;

    explicit result_void(logger_error_code code, const std::string& msg = "")
        : value_(common::error_info(
              static_cast<int>(code),
              msg.empty() ? logger_error_to_string(code) : msg,
              "logger_system")) {}

    static result_void success() { return result_void(); }

    static result_void error(logger_error_code code, const std::string& msg = "") {
        return result_void(code, msg);
    }

    bool has_error() const { return common::is_error(value_); }
    explicit operator bool() const { return !has_error(); }

    logger_error_code error_code() const {
        return static_cast<logger_error_code>(common::get_error(value_).code);
    }

    const std::string& error_message() const {
        return common::get_error(value_).message;
    }

    const common::VoidResult& raw() const { return value_; }

private:
    common::VoidResult value_{};
};

inline result_void make_logger_error(logger_error_code code, const std::string& message = "") {
    return result_void{code, message};
}

template<typename T>
inline result<T> make_logger_error(logger_error_code code, const std::string& message = "") {
    return result<T>{code, message};
}

using error_code = logger_error_code;

} // namespace kcenon::logger
