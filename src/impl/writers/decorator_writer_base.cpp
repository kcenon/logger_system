// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file decorator_writer_base.cpp
 * @brief Implementation of decorator_writer_base
 * @author kcenon
 * @since 4.0.0
 */

#include <kcenon/logger/writers/decorator_writer_base.h>

#include <stdexcept>

namespace kcenon::logger {

decorator_writer_base::decorator_writer_base(std::unique_ptr<log_writer_interface> wrapped,
                                             std::string_view decorator_name)
    : wrapped_(std::move(wrapped)), decorator_name_(decorator_name) {
    if (!wrapped_) {
        throw std::invalid_argument("decorator_writer_base: wrapped writer cannot be nullptr");
    }
}

common::VoidResult decorator_writer_base::flush() {
    return wrapped_->flush();
}

std::string decorator_writer_base::get_name() const {
    return decorator_name_ + "_" + wrapped_->get_name();
}

bool decorator_writer_base::is_healthy() const {
    return wrapped_->is_healthy();
}

const log_writer_interface* decorator_writer_base::get_wrapped_writer() const noexcept {
    return wrapped_.get();
}

log_writer_interface& decorator_writer_base::wrapped() noexcept {
    return *wrapped_;
}

const log_writer_interface& decorator_writer_base::wrapped() const noexcept {
    return *wrapped_;
}

const std::string& decorator_writer_base::decorator_name() const noexcept {
    return decorator_name_;
}

}  // namespace kcenon::logger
