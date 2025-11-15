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

#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>
#include <kcenon/logger/core/small_string.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/utils/error_handling_utils.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

namespace kcenon::logger {

// base_writer implementation

base_writer::base_writer(std::unique_ptr<log_formatter_interface> formatter)
    : formatter_(std::move(formatter)) {
    if (!formatter_) {
        // Default to timestamp_formatter if none provided
        formatter_ = std::make_unique<timestamp_formatter>();
    }

    // Apply color setting from legacy API to formatter
    auto opts = formatter_->get_options();
    opts.use_colors = use_color_;
    formatter_->set_options(opts);
}

log_formatter_interface* base_writer::get_formatter() const {
    return formatter_.get();
}

std::string base_writer::format_log_entry(const log_entry& entry) const {
    if (!formatter_) {
        // Fallback if formatter is somehow null
        return entry.message.to_string();
    }
    return formatter_->format(entry);
}

// Legacy formatting methods (for backward compatibility)

console_writer::console_writer(bool use_stderr, bool auto_detect_color)
    : use_stderr_(use_stderr) {
    if (auto_detect_color) {
        set_use_color(is_color_supported());
    }
}

console_writer::~console_writer() {
    flush();
}

result_void console_writer::write(logger_system::log_level level,
                                  const std::string& message,
                                  const std::string& file,
                                  int line,
                                  const std::string& function,
                                  const std::chrono::system_clock::time_point& timestamp) {
    std::lock_guard<std::mutex> lock(write_mutex_);

    return utils::try_write_operation([&]() -> result_void {
        auto& stream = (use_stderr_ || level <= logger_system::log_level::error)
                       ? std::cerr : std::cout;

        // Create log_entry for new API
        log_entry entry = (!file.empty() || line != 0 || !function.empty())
            ? log_entry(level, message, file, line, function, timestamp)
            : log_entry(level, message, timestamp);

        if (use_color()) {
            // Simple color mapping based on level
            switch (level) {
                case logger_system::log_level::fatal:
                case logger_system::log_level::error:
                    stream << "\033[31m"; // Red
                    break;
                case logger_system::log_level::warning:
                    stream << "\033[33m"; // Yellow
                    break;
                case logger_system::log_level::info:
                    stream << "\033[32m"; // Green
                    break;
                case logger_system::log_level::debug:
                    stream << "\033[36m"; // Cyan
                    break;
                case logger_system::log_level::trace:
                    stream << "\033[37m"; // White
                    break;
                default:
                    break;
            }
        }

        stream << format_log_entry(entry);

        if (use_color()) {
            stream << "\033[0m"; // Reset color
        }

        stream << '\n';

        // Verify stream state
        return utils::check_stream_state(stream, "console write");
    }, logger_error_code::processing_failed);
}

result_void console_writer::flush() {
    std::lock_guard<std::mutex> lock(write_mutex_);
    std::cout.flush();
    std::cerr.flush();
    
    if (std::cout.fail() || std::cerr.fail()) {
        return make_logger_error(logger_error_code::flush_timeout,
                                "Console flush failed");
    }
    
    return {}; // Success
}

void console_writer::set_use_stderr(bool use_stderr) {
    use_stderr_ = use_stderr;
}

bool console_writer::is_color_supported() const {
#ifdef _WIN32
    // Check if running in Windows Terminal or if ANSI is enabled
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return false;
    }
    
    // Check for ENABLE_VIRTUAL_TERMINAL_PROCESSING flag
    return (dwMode & 0x0004) != 0;
#else
    // Check if output is to a terminal and TERM is set
    const char* term = std::getenv("TERM");
    return isatty(STDOUT_FILENO) && term && std::string(term) != "dumb";
#endif
}

} // namespace kcenon::logger