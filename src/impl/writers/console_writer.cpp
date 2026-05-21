// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>
#include <kcenon/logger/core/small_string.h>
#include <kcenon/logger/security/integrity_policy.h>
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

console_writer::console_writer(bool use_stderr,
                               bool auto_detect_color,
                               std::unique_ptr<log_formatter_interface> formatter)
    : use_stderr_(use_stderr)
    , formatter_(formatter ? std::move(formatter) : std::make_unique<timestamp_formatter>()) {
    if (auto_detect_color) {
        set_use_color(is_color_supported());
    } else {
        // Apply color setting to formatter
        auto opts = formatter_->get_options();
        opts.use_colors = use_color_;
        formatter_->set_options(opts);
    }
}

console_writer::~console_writer() {
    flush();
}

common::VoidResult console_writer::write(const log_entry& entry) {
    std::lock_guard<std::mutex> lock(mutex_);
    return utils::try_write_operation([&]() -> common::VoidResult {
        // Convert logger_system::log_level to common::interfaces::log_level for comparison
        auto level = static_cast<common::interfaces::log_level>(static_cast<int>(entry.level));

        // Route error/fatal to stderr; everything less severe to stdout.
        // log_level values are ordered by severity (trace=0 ... fatal=5),
        // so >= error selects the high-severity bucket.
        auto& stream = (use_stderr_ || level >= common::interfaces::log_level::error)
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

        std::string formatted = format_entry(entry);

        // Append tamper-evident signature (Issue #612) when configured.
        // Color reset is emitted first so the signature is never colorized
        // and stays machine-readable by downstream collectors.
        stream << formatted;

        if (use_color()) {
            stream << "\033[0m"; // Reset color
        }

        if (integrity_policy_) {
            stream << security::format_signature_suffix(
                *integrity_policy_, formatted);
        }

        stream << '\n';

        // Verify stream state
        return utils::check_stream_state(stream, "console write");
    }, logger_error_code::processing_failed);
}

common::VoidResult console_writer::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    return utils::try_write_operation([&]() -> common::VoidResult {
        std::cout.flush();
        std::cerr.flush();

        if (std::cout.fail() || std::cerr.fail()) {
            return make_logger_void_result(logger_error_code::flush_timeout, "Console flush failed");
        }

        return common::ok();
    }, logger_error_code::flush_timeout);
}

bool console_writer::is_healthy() const {
    return std::cout.good() && std::cerr.good();
}

void console_writer::set_use_stderr(bool use_stderr) {
    std::lock_guard<std::mutex> lock(mutex_);
    use_stderr_ = use_stderr;
}

void console_writer::set_use_color(bool use_color) {
    use_color_ = use_color;
    if (formatter_) {
        auto opts = formatter_->get_options();
        opts.use_colors = use_color_;
        formatter_->set_options(opts);
    }
}

bool console_writer::use_color() const {
    return use_color_;
}

void console_writer::set_integrity_policy(
    std::shared_ptr<security::integrity_policy> policy) {
    std::lock_guard<std::mutex> lock(mutex_);
    integrity_policy_ = std::move(policy);
}

std::string console_writer::format_entry(const log_entry& entry) const {
    if (!formatter_) {
        // Fallback if formatter is somehow null
        return entry.message.to_string();
    }
    return formatter_->format(entry);
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