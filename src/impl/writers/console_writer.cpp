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

console_writer::console_writer(bool use_stderr, bool auto_detect_color)
    : use_stderr_(use_stderr) {
    if (auto_detect_color) {
        set_use_color(is_color_supported());
    }
}

console_writer::~console_writer() {
    flush();
}

common::VoidResult console_writer::write_entry_impl(const log_entry& entry) {
    // Note: Mutex is already held by thread_safe_writer::write()
    return utils::try_write_operation([&]() -> common::VoidResult {
        // Convert logger_system::log_level to common::interfaces::log_level for comparison
        auto level = static_cast<common::interfaces::log_level>(static_cast<int>(entry.level));

        auto& stream = (use_stderr_ || level <= common::interfaces::log_level::error)
                       ? std::cerr : std::cout;

        if (use_color()) {
            // Simple color mapping based on level
            switch (level) {
                case common::interfaces::log_level::fatal:
                case common::interfaces::log_level::error:
                    stream << "\033[31m"; // Red
                    break;
                case common::interfaces::log_level::warning:
                    stream << "\033[33m"; // Yellow
                    break;
                case common::interfaces::log_level::info:
                    stream << "\033[32m"; // Green
                    break;
                case common::interfaces::log_level::debug:
                    stream << "\033[36m"; // Cyan
                    break;
                case common::interfaces::log_level::trace:
                    stream << "\033[37m"; // White
                    break;
                default:
                    break;
            }
        }

        // Use format_log_entry which preserves all structured fields
        stream << format_log_entry(entry);

        if (use_color()) {
            stream << "\033[0m"; // Reset color
        }

        stream << '\n';

        // Verify stream state
        return utils::check_stream_state(stream, "console write");
    }, logger_error_code::processing_failed);
}

common::VoidResult console_writer::write_impl(common::interfaces::log_level level,
                                              const std::string& message,
                                              const std::string& file,
                                              int line,
                                              const std::string& function,
                                              const std::chrono::system_clock::time_point& timestamp) {
    // Legacy API: create a log_entry and delegate to write_entry_impl
    log_entry entry = (!file.empty() || line != 0 || !function.empty())
        ? log_entry(level, message, file, line, function, timestamp)
        : log_entry(level, message, timestamp);

    return write_entry_impl(entry);
}

common::VoidResult console_writer::flush_impl() {
    // Note: Mutex is already held by thread_safe_writer::flush()
    return utils::try_write_operation([&]() -> common::VoidResult {
        std::cout.flush();
        std::cerr.flush();

        if (std::cout.fail() || std::cerr.fail()) {
            return make_logger_void_result(logger_error_code::flush_timeout, "Console flush failed");
        }

        return common::ok();
    }, logger_error_code::flush_timeout);
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