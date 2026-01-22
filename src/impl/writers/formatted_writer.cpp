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
    : wrapped_(std::move(wrapped)), formatter_(std::move(formatter)) {
    if (!wrapped_) {
        throw std::invalid_argument("formatted_writer: wrapped writer cannot be nullptr");
    }
}

common::VoidResult formatted_writer::write(const log_entry& entry) {
    // If no formatter, pass through all entries unchanged
    if (!formatter_) {
        return wrapped_->write(entry);
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
    return wrapped_->write(formatted_entry);
}

common::VoidResult formatted_writer::flush() {
    return wrapped_->flush();
}

std::string formatted_writer::get_name() const {
    if (formatter_) {
        return "formatted(" + formatter_->get_name() + ")_" + wrapped_->get_name();
    }
    return "formatted_" + wrapped_->get_name();
}

bool formatted_writer::is_healthy() const {
    return wrapped_->is_healthy();
}

const log_formatter_interface* formatted_writer::get_formatter() const {
    return formatter_.get();
}

const log_writer_interface* formatted_writer::get_wrapped_writer() const {
    return wrapped_.get();
}

// Factory function
std::unique_ptr<formatted_writer> make_formatted_writer(
    std::unique_ptr<log_writer_interface> writer,
    std::unique_ptr<log_formatter_interface> formatter) {
    return std::make_unique<formatted_writer>(std::move(writer), std::move(formatter));
}

}  // namespace kcenon::logger
