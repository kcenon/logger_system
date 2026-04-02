// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file writer_builder.cpp
 * @brief Implementation of writer_builder
 * @author kcenon
 * @since 4.1.0
 */

#include <kcenon/logger/builders/writer_builder.h>

#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/buffered_writer.h>
#include <kcenon/logger/writers/filtered_writer.h>
#include <kcenon/logger/writers/formatted_writer.h>
#include <kcenon/logger/writers/encrypted_writer.h>

#include <stdexcept>

namespace kcenon::logger {

//============================================================================
// Core Writers
//============================================================================

writer_builder& writer_builder::file(const std::string& filename, bool append) {
    ensure_no_core_writer();
    writer_ = std::make_unique<file_writer>(filename, append);
    return *this;
}

writer_builder& writer_builder::console(bool use_stderr, bool auto_detect_color) {
    ensure_no_core_writer();
    writer_ = std::make_unique<console_writer>(use_stderr, auto_detect_color);
    return *this;
}

writer_builder& writer_builder::custom(std::unique_ptr<log_writer_interface> writer) {
    if (!writer) {
        throw std::invalid_argument("writer_builder::custom: writer cannot be nullptr");
    }
    ensure_no_core_writer();
    writer_ = std::move(writer);
    return *this;
}

//============================================================================
// Decorator Writers
//============================================================================

writer_builder& writer_builder::async(std::size_t queue_size,
                                       std::chrono::seconds flush_timeout) {
    ensure_core_writer();
    writer_ = std::make_unique<async_writer>(std::move(writer_), queue_size, flush_timeout);
    return *this;
}

writer_builder& writer_builder::buffered(std::size_t max_entries,
                                          std::chrono::milliseconds flush_interval) {
    ensure_core_writer();
    writer_ = std::make_unique<buffered_writer>(std::move(writer_), max_entries, flush_interval);
    return *this;
}

writer_builder& writer_builder::encrypted(security::secure_key key) {
    ensure_core_writer();
    encryption_config config(encryption_algorithm::aes_256_gcm, std::move(key));
    writer_ = std::make_unique<encrypted_writer>(std::move(writer_), std::move(config));
    return *this;
}

writer_builder& writer_builder::filtered(std::unique_ptr<log_filter_interface> filter) {
    ensure_core_writer();
    writer_ = std::make_unique<filtered_writer>(std::move(writer_), std::move(filter));
    return *this;
}

writer_builder& writer_builder::formatted(std::unique_ptr<log_formatter_interface> formatter) {
    ensure_core_writer();
    writer_ = std::make_unique<formatted_writer>(std::move(writer_), std::move(formatter));
    return *this;
}

//============================================================================
// Build
//============================================================================

log_writer_ptr writer_builder::build() {
    if (!writer_) {
        throw std::logic_error(
            "writer_builder::build: no core writer has been set. "
            "Call file(), console(), or custom() before build()");
    }
    return std::move(writer_);
}

bool writer_builder::has_core_writer() const noexcept {
    return writer_ != nullptr;
}

void writer_builder::reset() noexcept {
    writer_.reset();
}

//============================================================================
// Private Helpers
//============================================================================

void writer_builder::ensure_core_writer() const {
    if (!writer_) {
        throw std::logic_error(
            "writer_builder: no core writer has been set. "
            "Call file(), console(), or custom() before adding decorators");
    }
}

void writer_builder::ensure_no_core_writer() const {
    if (writer_) {
        throw std::logic_error(
            "writer_builder: a core writer has already been set. "
            "Only one core writer is allowed per builder");
    }
}

}  // namespace kcenon::logger
