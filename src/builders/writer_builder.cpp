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

/**
 * @file writer_builder.cpp
 * @brief Implementation of writer_builder fluent API
 * @author kcenon
 * @since 4.0.0
 */

#include <kcenon/logger/builders/writer_builder.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/network_writer.h>
#include <kcenon/logger/writers/rotating_file_writer.h>
#include <kcenon/logger/writers/otlp_writer.h>
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/buffered_writer.h>
#include <kcenon/logger/writers/encrypted_writer.h>
#include <kcenon/logger/writers/filtered_writer.h>
#include <kcenon/logger/interfaces/log_filter_interface.h>

#include <stdexcept>

namespace kcenon::logger {

//========================================================================
// Core Writers
//========================================================================

writer_builder& writer_builder::file(const std::string& path,
                                     bool append) {
    writer_ = std::make_unique<file_writer>(path, append);
    return *this;
}

writer_builder& writer_builder::rotating_file(const std::string& path,
                                              std::size_t max_size,
                                              std::size_t max_files,
                                              std::size_t check_interval) {
    writer_ = std::make_unique<rotating_file_writer>(
        path, max_size, max_files, check_interval);
    return *this;
}

writer_builder& writer_builder::rotating_file(const std::string& path,
                                              rotation_type type,
                                              std::size_t max_files,
                                              std::size_t check_interval) {
    // Time-based constructor: rotating_file_writer(path, type, max_files)
    // Note: check_interval parameter is ignored to avoid ambiguity with combined constructor
    // The default check_interval (100) will be used
    (void)check_interval;  // Suppress unused parameter warning
    writer_ = std::make_unique<rotating_file_writer>(path, type, max_files);
    return *this;
}

writer_builder& writer_builder::console(bool use_stderr,
                                       bool auto_detect_color) {
    writer_ = std::make_unique<console_writer>(use_stderr, auto_detect_color);
    return *this;
}

writer_builder& writer_builder::network(const std::string& host,
                                       uint16_t port,
                                       network_writer::protocol_type protocol,
                                       std::size_t buffer_size,
                                       std::chrono::seconds reconnect_interval) {
    writer_ = std::make_unique<network_writer>(
        host, port, protocol, buffer_size, reconnect_interval);
    return *this;
}

writer_builder& writer_builder::otlp(const otlp_writer::config& config) {
    writer_ = std::make_unique<otlp_writer>(config);
    return *this;
}

//========================================================================
// Decorators
//========================================================================

writer_builder& writer_builder::async(std::size_t queue_size,
                                     std::chrono::seconds flush_timeout) {
    ensure_core_writer();
    writer_ = std::make_unique<async_writer>(
        std::move(writer_), queue_size, flush_timeout);
    return *this;
}

writer_builder& writer_builder::buffered(std::size_t max_entries,
                                        std::chrono::milliseconds flush_interval) {
    ensure_core_writer();
    writer_ = std::make_unique<buffered_writer>(
        std::move(writer_), max_entries, flush_interval);
    return *this;
}

writer_builder& writer_builder::encrypted(encryption_config config) {
    ensure_core_writer();
    writer_ = std::make_unique<encrypted_writer>(
        std::move(writer_), std::move(config));
    return *this;
}

writer_builder& writer_builder::thread_safe() {
    ensure_core_writer();
    // TODO: Implement thread_safe_writer decorator once available
    // For now, this is a placeholder
    // writer_ = std::make_unique<thread_safe_writer>(std::move(writer_));
    return *this;
}

writer_builder& writer_builder::filtered(std::unique_ptr<log_filter_interface> filter) {
    ensure_core_writer();
    writer_ = std::make_unique<filtered_writer>(
        std::move(writer_), std::move(filter));
    return *this;
}

//========================================================================
// Build
//========================================================================

log_writer_ptr writer_builder::build() {
    ensure_core_writer();
    return std::move(writer_);
}

//========================================================================
// Private Methods
//========================================================================

void writer_builder::ensure_core_writer() const {
    if (!writer_) {
        throw std::logic_error(
            "writer_builder: No core writer configured. "
            "Call file(), console(), network(), or otlp() before adding decorators or calling build().");
    }
}

} // namespace kcenon::logger
