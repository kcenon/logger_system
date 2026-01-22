/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
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

#include <kcenon/logger/writers/formatted_writer.h>

#include <kcenon/logger/interfaces/log_entry.h>

#include <stdexcept>

namespace kcenon::logger {

formatted_writer::formatted_writer(std::unique_ptr<log_writer_interface> wrapped,
                                   std::unique_ptr<log_formatter_interface> formatter)
    : decorator_writer_base(std::move(wrapped), "formatted"), formatter_(std::move(formatter)) {
    // Base class constructor handles nullptr check for wrapped writer
}

common::VoidResult formatted_writer::write(const log_entry& entry) {
    // If no formatter, pass through all entries unchanged
    if (!formatter_) {
        return wrapped().write(entry);
    }

    // Apply formatter to get formatted message
    std::string formatted_message = formatter_->format(entry);

    // Create a new log entry with the formatted message
    log_entry formatted_entry(entry.level, formatted_message, entry.timestamp);

    // Copy optional fields from original entry
    formatted_entry.location = entry.location;
    formatted_entry.thread_id = entry.thread_id;
    formatted_entry.category = entry.category;
    formatted_entry.otel_ctx = entry.otel_ctx;
    formatted_entry.fields = entry.fields;

    // Delegate to wrapped writer
    return wrapped().write(formatted_entry);
}

std::string formatted_writer::get_name() const {
    if (formatter_) {
        return "formatted(" + formatter_->get_name() + ")_" + wrapped().get_name();
    }
    // Use base class default format when no formatter
    return decorator_writer_base::get_name();
}

const log_formatter_interface* formatted_writer::get_formatter() const {
    return formatter_.get();
}

// Factory function
std::unique_ptr<formatted_writer> make_formatted_writer(
    std::unique_ptr<log_writer_interface> writer,
    std::unique_ptr<log_formatter_interface> formatter) {
    return std::make_unique<formatted_writer>(std::move(writer), std::move(formatter));
}

}  // namespace kcenon::logger
