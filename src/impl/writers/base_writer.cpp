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
 * @file base_writer.cpp
 * @brief Implementation of base_writer class
 * @author 🍀☀🌕🌥 🌊
 * @since 1.0.0
 * @since 1.3.0 Separated from console_writer.cpp for proper code organization
 */

#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>

namespace kcenon::logger {

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

} // namespace kcenon::logger
