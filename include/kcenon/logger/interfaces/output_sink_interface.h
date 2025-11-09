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
 * @file output_sink_interface.h
 * @brief Interface for log output destinations (Single Responsibility Principle)
 * @author 🍀☀🌕🌥 🌊
 * @since 1.3.0
 * @version Sprint 5-6 - Writer Refactoring
 *
 * @details This interface represents the output destination for log messages.
 * It follows the Single Responsibility Principle by handling ONLY the I/O
 * operations, completely separated from formatting concerns.
 *
 * Note: This is different from log_sink_interface which handles async processing.
 * output_sink_interface is for direct I/O operations.
 */

#include <string>
#include <string_view>
#include <memory>
#include <functional>
#include <kcenon/logger/core/error_codes.h>

namespace kcenon::logger {

/**
 * @class output_sink_interface
 * @brief Abstract interface for log output destinations (I/O only)
 *
 * @details Defines the contract for outputting pre-formatted log messages.
 * This is separate from log_sink_interface which handles async processing.
 *
 * @since 1.3.0
 */
class output_sink_interface {
public:
    virtual ~output_sink_interface() = default;

    /**
     * @brief Write a pre-formatted message to the output destination
     * @param message Pre-formatted log message
     * @return result_void Success or error code
     */
    virtual result_void write_raw(std::string_view message) = 0;

    /**
     * @brief Flush any buffered data
     * @return result_void Success or error code
     */
    virtual result_void flush() = 0;

    /**
     * @brief Check if the sink is healthy
     * @return true if operational
     */
    virtual bool is_healthy() const = 0;

    /**
     * @brief Get the name of this sink
     * @return Sink identifier
     */
    virtual std::string get_name() const = 0;

    /**
     * @brief Get sink information
     * @return Diagnostic string
     */
    virtual std::string get_info() const {
        return "";
    }
};

using output_sink_factory = std::function<std::unique_ptr<output_sink_interface>()>;

} // namespace kcenon::logger
