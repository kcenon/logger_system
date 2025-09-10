/**
 * @file logger_v1_compat.h
 * @brief Compatibility wrapper for Logger System v1.x API
 * 
 * This header provides backward compatibility for applications
 * migrating from Logger System v1.x to v2.x. All functions and
 * macros in this file are deprecated and should be replaced
 * with their v2.x equivalents.
 * 
 * @deprecated This entire file is deprecated. Use v2.x API directly.
 * @since 2.0.0
 */

#pragma once

#include "../logger.h"
#include "../config/logger_builder.h"
#include "../writers/console_writer.h"
#include "../writers/file_writer.h"
#include <memory>
#include <stdexcept>
#include <cstdarg>
#include <cstdio>

namespace logger_module {

// Type alias for convenience
using log_level = thread_module::log_level;

namespace v1_compat {

/**
 * @brief Format string with variable arguments (C-style)
 * @deprecated Use structured logging with fields instead
 */
[[deprecated("Use structured logging with fields instead of format strings")]]
inline std::string format_string(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    // Get required buffer size
    va_list args_copy;
    va_copy(args_copy, args);
    int size = vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);
    
    if (size < 0) {
        va_end(args);
        return fmt;
    }
    
    // Format the string
    std::vector<char> buffer(size + 1);
    vsnprintf(buffer.data(), buffer.size(), fmt, args);
    va_end(args);
    
    return std::string(buffer.data(), size);
}

// Legacy logging macros
#define LOG_TRACE(logger, ...) \
    do { \
        if (logger) { \
            logger->log(thread_module::log_level::trace, \
                       logger_module::v1_compat::format_string(__VA_ARGS__)); \
        } \
    } while(0)

#define LOG_DEBUG(logger, ...) \
    do { \
        if (logger) { \
            logger->log(thread_module::log_level::debug, \
                       logger_module::v1_compat::format_string(__VA_ARGS__)); \
        } \
    } while(0)

#define LOG_INFO(logger, ...) \
    do { \
        if (logger) { \
            logger->log(thread_module::log_level::info, \
                       logger_module::v1_compat::format_string(__VA_ARGS__)); \
        } \
    } while(0)

#define LOG_WARNING(logger, ...) \
    do { \
        if (logger) { \
            logger->log(thread_module::log_level::warning, \
                       logger_module::v1_compat::format_string(__VA_ARGS__)); \
        } \
    } while(0)

#define LOG_ERROR(logger, ...) \
    do { \
        if (logger) { \
            logger->log(thread_module::log_level::error, \
                       logger_module::v1_compat::format_string(__VA_ARGS__)); \
        } \
    } while(0)

#define LOG_CRITICAL(logger, ...) \
    do { \
        if (logger) { \
            logger->log(thread_module::log_level::error, \
                       logger_module::v1_compat::format_string(__VA_ARGS__)); \
        } \
    } while(0)

/**
 * @brief Legacy logger creation function
 * @deprecated Use logger_builder instead
 */
[[deprecated("Use logger_builder to create loggers")]]
inline logger* create_logger() {
    static std::unique_ptr<logger> modern_logger;
    auto result = logger_builder()
        .add_writer("console", std::make_unique<console_writer>())
        .build();
    if (result) {
        modern_logger = std::move(result.value());
    }
    return modern_logger.get();
}

/**
 * @brief Legacy logger creation with name
 * @deprecated Use logger_builder with with_name()
 */
[[deprecated("Use logger_builder with add_writer()")]]
inline logger* create_logger(const std::string& name) {
    static std::unique_ptr<logger> modern_logger;
    auto result = logger_builder()
        .add_writer("console", std::make_unique<console_writer>())
        .build();
    if (result) {
        modern_logger = std::move(result.value());
    }
    return modern_logger.get();
}

/**
 * @brief Legacy file logger creation
 * @deprecated Use logger_builder with with_file_writer()
 */
[[deprecated("Use logger_builder with add_writer()")]]
inline logger* create_file_logger(const std::string& filename) {
    static std::unique_ptr<logger> modern_logger;
    auto result = logger_builder()
        .add_writer("file", std::make_unique<file_writer>(filename))
        .build();
    if (result) {
        modern_logger = std::move(result.value());
    }
    return modern_logger.get();
}

/**
 * @brief Legacy writer addition with raw pointer
 * @deprecated Use add_writer with std::unique_ptr
 */
[[deprecated("Use add_writer with std::unique_ptr")]]
inline void add_writer(logger* log, base_writer* writer) {
    if (!log || !writer) {
        throw std::invalid_argument("Null logger or writer");
    }
    
    log->add_writer("writer", std::unique_ptr<base_writer>(writer));
}

/**
 * @brief Legacy console writer addition
 * @deprecated Use logger_builder with with_console_writer()
 */
[[deprecated("Use logger_builder with with_console_writer()")]]
inline void add_console_writer(logger* log) {
    if (!log) {
        throw std::invalid_argument("Null logger");
    }
    
    log->add_writer("console", std::make_unique<console_writer>());
}

/**
 * @brief Legacy file writer addition
 * @deprecated Use logger_builder with with_file_writer()
 */
[[deprecated("Use logger_builder with with_file_writer()")]]
inline void add_file_writer(logger* log, const std::string& filename) {
    if (!log) {
        throw std::invalid_argument("Null logger");
    }
    
    log->add_writer("file", std::make_unique<file_writer>(filename));
}

/**
 * @brief Legacy level setting
 * @deprecated Loggers are immutable in v2. Recreate with logger_builder.
 */
[[deprecated("Loggers are immutable. Recreate with logger_builder.")]]
inline void set_level(logger* log, log_level level) {
    std::cerr << "Warning: set_level is deprecated. "
              << "Logger configuration is immutable in v2.x. "
              << "Please recreate the logger with logger_builder.\n";
}

/**
 * @brief Legacy async mode setting
 * @deprecated Use logger_builder with with_async_mode()
 */
[[deprecated("Use logger_builder with with_async_mode()")]]
inline void set_async(logger* log, bool async) {
    std::cerr << "Warning: set_async is deprecated. "
              << "Logger configuration is immutable in v2.x. "
              << "Please recreate the logger with logger_builder.\n";
}

/**
 * @brief Legacy pattern setting
 * @deprecated Use logger_builder with with_pattern()
 */
[[deprecated("Use logger_builder with with_pattern()")]]
inline void set_pattern(logger* log, const std::string& pattern) {
    std::cerr << "Warning: set_pattern is deprecated. "
              << "Logger configuration is immutable in v2.x. "
              << "Please recreate the logger with logger_builder.\n";
}

/**
 * @brief Legacy logger configuration structure
 * @deprecated Use logger_builder for configuration
 */
struct [[deprecated("Use logger_builder for configuration")]] logger_config_v1 {
    log_level level = log_level::info;
    bool async = false;
    size_t buffer_size = 8192;
    std::string pattern = "[%time%] [%level%] %message%";
    bool colored = true;
    
    /**
     * @brief Apply configuration to create logger
     * @deprecated Use logger_builder directly
     */
    [[deprecated("Use logger_builder directly")]]
    std::shared_ptr<logger> create() const {
        auto result = logger_builder()
            .with_min_level(level)
            .with_async(async)
            .with_buffer_size(buffer_size)
            .add_writer("console", std::make_unique<console_writer>())
            .build();
        if (result) {
            return std::move(result.value());
        }
        return nullptr;
    }
};

/**
 * @brief Legacy global logger instance
 * @deprecated Use dependency injection instead of global state
 */
[[deprecated("Use dependency injection instead of global logger")]]
inline logger* get_global_logger() {
    static std::unique_ptr<logger> global;
    if (!global) {
        auto result = logger_builder()
            .add_writer("console", std::make_unique<console_writer>())
            .build();
        if (result) {
            global = std::move(result.value());
        }
    }
    return global.get();
}

/**
 * @brief Legacy initialization function
 * @deprecated Use logger_builder
 */
[[deprecated("Use logger_builder for initialization")]]
inline void init_logging(const logger_config_v1& config = {}) {
    std::cerr << "Warning: init_logging is deprecated. "
              << "Use logger_builder for logger initialization.\n";
}

/**
 * @brief Legacy shutdown function
 * @deprecated Loggers clean up automatically via RAII
 */
[[deprecated("Loggers clean up automatically via RAII")]]
inline void shutdown_logging() {
    std::cerr << "Warning: shutdown_logging is deprecated. "
              << "Loggers clean up automatically when destroyed.\n";
}

} // namespace v1_compat
} // namespace logger_module

// Optional: Import v1 compatibility into global namespace
// Uncomment during migration, remove after completion
// using namespace logger_module::v1_compat;