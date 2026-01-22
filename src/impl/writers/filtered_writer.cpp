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

#include <kcenon/logger/writers/filtered_writer.h>

#include <kcenon/logger/interfaces/log_entry.h>

#include <stdexcept>

namespace kcenon::logger {

filtered_writer::filtered_writer(std::unique_ptr<log_writer_interface> wrapped,
                                 std::unique_ptr<log_filter_interface> filter)
    : wrapped_(std::move(wrapped)), filter_(std::move(filter)) {
    if (!wrapped_) {
        throw std::invalid_argument("filtered_writer: wrapped writer cannot be nullptr");
    }
}

common::VoidResult filtered_writer::write(const log_entry& entry) {
    // If no filter, pass through all entries
    if (!filter_) {
        return wrapped_->write(entry);
    }

    // Check if entry passes the filter
    if (filter_->should_log(entry)) {
        return wrapped_->write(entry);
    }

    // Entry filtered out - return success (not an error)
    return common::ok();
}

common::VoidResult filtered_writer::flush() {
    return wrapped_->flush();
}

std::string filtered_writer::get_name() const {
    if (filter_) {
        return "filtered(" + filter_->get_name() + ")_" + wrapped_->get_name();
    }
    return "filtered_" + wrapped_->get_name();
}

bool filtered_writer::is_healthy() const {
    return wrapped_->is_healthy();
}

const log_filter_interface* filtered_writer::get_filter() const {
    return filter_.get();
}

const log_writer_interface* filtered_writer::get_wrapped_writer() const {
    return wrapped_.get();
}

// Factory function
std::unique_ptr<filtered_writer> make_filtered_writer(
    std::unique_ptr<log_writer_interface> writer,
    std::unique_ptr<log_filter_interface> filter) {
    return std::make_unique<filtered_writer>(std::move(writer), std::move(filter));
}

}  // namespace kcenon::logger
