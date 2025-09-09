#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <string>
#include <chrono>
#include <optional>
#include "../core/small_string.h"

#ifdef USE_THREAD_SYSTEM
    #include <interfaces/logger_interface.h>
#else
    #include "../logger_interface.h"
#endif

namespace logger_module {

/**
 * @struct source_location
 * @brief Source code location information
 */
struct source_location {
    small_string_256 file;  // File paths can be long
    int line;
    small_string_128 function;  // Function names are typically shorter
    
    source_location(const std::string& f = "", int l = 0, const std::string& func = "")
        : file(f), line(l), function(func) {}
    
    source_location(const char* f = "", int l = 0, const char* func = "")
        : file(f), line(l), function(func) {}
};

/**
 * @struct log_entry
 * @brief Represents a single log entry
 * 
 * This structure contains all the information for a log message.
 * Designed to be move-only for performance.
 */
struct log_entry {
    // Required fields
    thread_module::log_level level;
    small_string_256 message;  // Using SSO for messages
    std::chrono::system_clock::time_point timestamp;
    
    // Optional fields
    std::optional<source_location> location;
    std::optional<small_string_64> thread_id;  // Thread IDs are typically small
    std::optional<small_string_128> category;  // Categories are usually short
    
    // Constructor for basic entry
    log_entry(thread_module::log_level lvl, 
              const std::string& msg,
              std::chrono::system_clock::time_point ts = std::chrono::system_clock::now())
        : level(lvl), message(msg), timestamp(ts) {}
    
    // Constructor with source location
    log_entry(thread_module::log_level lvl,
              const std::string& msg,
              const std::string& file,
              int line,
              const std::string& function,
              std::chrono::system_clock::time_point ts = std::chrono::system_clock::now())
        : level(lvl), message(msg), timestamp(ts),
          location(source_location{file, line, function}) {}
    
    // Move constructor and assignment
    log_entry(log_entry&&) noexcept = default;
    log_entry& operator=(log_entry&&) noexcept = default;
    
    // Delete copy operations for performance
    log_entry(const log_entry&) = delete;
    log_entry& operator=(const log_entry&) = delete;
};

} // namespace logger_module