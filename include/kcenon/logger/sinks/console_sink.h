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

/**
 * @file console_sink.h
 * @brief Console output sink implementation (stdout/stderr)
 * @author 🍀☀🌕🌥 🌊
 * @since 1.3.0
 * @version Sprint 5-6 - Writer Refactoring
 *
 * @details Provides console output functionality following SRP.
 * This sink handles ONLY I/O to stdout/stderr, with no formatting logic.
 * All formatting should be done by a formatter before messages reach this sink.
 */

#include <iostream>
#include <mutex>
#include "../interfaces/log_sink_interface.h"

namespace kcenon::logger {

/**
 * @class console_sink
 * @brief Outputs log messages to console (stdout or stderr)
 *
 * @details Thread-safe console output sink. Accepts pre-formatted messages
 * and writes them to stdout or stderr without any modification.
 *
 * Features:
 * - Thread-safe output (mutex-protected)
 * - Configurable output stream (stdout vs stderr)
 * - Automatic flushing support
 * - Always healthy (console is always available)
 *
 * @note This sink does NOT add timestamps, colors, or any formatting.
 * All formatting must be done by a formatter before calling write_raw().
 *
 * @thread_safety All methods are thread-safe.
 *
 * @example Basic usage:
 * @code
 * auto sink = std::make_unique<console_sink>();
 * sink->write_raw("[2025-01-01 12:00:00] [INFO] Application started\n");
 * sink->flush();
 * @endcode
 *
 * @since 1.3.0
 */
class console_sink : public log_sink_interface {
public:
    /**
     * @brief Construct a console sink
     * @param use_stderr If true, write to stderr; if false, write to stdout
     * @param auto_flush If true, flush after every write
     *
     * @details Creates a console sink that writes to the specified output stream.
     *
     * @note stderr is typically used for error messages, stdout for normal output.
     * Auto-flushing ensures immediate output but may reduce performance.
     *
     * @since 1.3.0
     */
    explicit console_sink(bool use_stderr = false, bool auto_flush = false)
        : use_stderr_(use_stderr)
        , auto_flush_(auto_flush)
    {}

    /**
     * @brief Destructor - ensures all data is flushed
     *
     * @since 1.3.0
     */
    ~console_sink() override {
        flush(); // Ensure all data is written
    }

    /**
     * @brief Write a pre-formatted message to console
     * @param message Pre-formatted message (must include newline if desired)
     * @return result_void Success (console writes rarely fail)
     *
     * @details Writes the message to stdout or stderr without modification.
     * The message should already be fully formatted with timestamps, colors, etc.
     *
     * @note This method is thread-safe.
     *
     * @warning If the message doesn't end with '\n', the next message will
     * continue on the same line. Formatters should include newlines.
     *
     * @since 1.3.0
     */
    result_void write_raw(std::string_view message) override {
        std::lock_guard<std::mutex> lock(mutex_);

        if (use_stderr_) {
            std::cerr << message;
            if (auto_flush_) {
                std::cerr.flush();
            }
        } else {
            std::cout << message;
            if (auto_flush_) {
                std::cout.flush();
            }
        }

        return {}; // Success
    }

    /**
     * @brief Flush buffered output to console
     * @return result_void Success
     *
     * @details Forces any buffered data to be written to the console immediately.
     *
     * @note This method is thread-safe.
     *
     * @since 1.3.0
     */
    result_void flush() override {
        std::lock_guard<std::mutex> lock(mutex_);

        if (use_stderr_) {
            std::cerr.flush();
        } else {
            std::cout.flush();
        }

        return {}; // Success
    }

    /**
     * @brief Check if console is healthy
     * @return true (console is always available)
     *
     * @details Console output is always available, so this always returns true.
     *
     * @note In rare cases (redirected to closed pipe), writes might fail,
     * but the sink itself is still considered healthy.
     *
     * @since 1.3.0
     */
    bool is_healthy() const override {
        return true; // Console is always available
    }

    /**
     * @brief Get the name of this sink
     * @return "console"
     *
     * @since 1.3.0
     */
    std::string get_name() const override {
        return "console";
    }

    /**
     * @brief Get sink configuration information
     * @return String describing current configuration
     *
     * @example "stdout" or "stderr, auto-flush"
     *
     * @since 1.3.0
     */
    std::string get_info() const override {
        std::string info = use_stderr_ ? "stderr" : "stdout";
        if (auto_flush_) {
            info += ", auto-flush";
        }
        return info;
    }

    /**
     * @brief Set whether to use stderr
     * @param use_stderr If true, write to stderr; if false, write to stdout
     *
     * @note This method is thread-safe.
     *
     * @since 1.3.0
     */
    void set_use_stderr(bool use_stderr) {
        std::lock_guard<std::mutex> lock(mutex_);
        use_stderr_ = use_stderr;
    }

    /**
     * @brief Set whether to auto-flush after every write
     * @param auto_flush If true, flush after every write
     *
     * @note This method is thread-safe.
     *
     * @since 1.3.0
     */
    void set_auto_flush(bool auto_flush) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto_flush_ = auto_flush;
    }

private:
    bool use_stderr_;      ///< Write to stderr instead of stdout
    bool auto_flush_;      ///< Flush after every write
    mutable std::mutex mutex_; ///< Protects console output
};

} // namespace kcenon::logger
