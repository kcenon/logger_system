// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file log_entry_pool.h
 * @brief Log entry pool implementation for high-performance memory management
 */

#pragma once

#include <kcenon/logger/interfaces/log_entry.h>
#include <memory>
#include <string>
#include <chrono>

namespace kcenon::logger::memory::log_entry_pool {

/**
 * @brief Pooled log entry structure optimized for reuse
 */
struct pooled_log_entry {
    common::interfaces::log_level level{common::interfaces::log_level::info};
    std::string message;
    std::string file_path;
    int line_number{0};
    std::string function_name;
    std::chrono::system_clock::time_point timestamp;

    /**
     * @brief Reset entry for reuse
     */
    void reset() {
        level = common::interfaces::log_level::info;
        message.clear();
        file_path.clear();
        line_number = 0;
        function_name.clear();
        timestamp = std::chrono::system_clock::now();
    }

    /**
     * @brief Initialize with log data
     */
    void initialize(common::interfaces::log_level lvl,
                   const std::string& msg,
                   const std::string& file,
                   int line,
                   const std::string& func) {
        level = lvl;
        message = msg;
        file_path = file;
        line_number = line;
        function_name = func;
        timestamp = std::chrono::system_clock::now();
    }

    /**
     * @brief Convert to standard log_entry
     */
    log_entry to_log_entry() const {
        log_entry entry(level, message, timestamp);
        if (!file_path.empty() || line_number != 0 || !function_name.empty()) {
            entry.location = source_location(file_path, line_number, function_name);
        }
        return entry;
    }
};

} // namespace kcenon::logger::memory::log_entry_pool