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
