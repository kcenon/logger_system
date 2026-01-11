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

#include <kcenon/logger/core/log_context_scope.h>
#include <kcenon/logger/core/logger.h>

namespace kcenon::logger {

log_context_scope::log_context_scope(logger& log, const log_fields& fields)
    : previous_context_(log_context_storage::get()),
      had_previous_(log_context_storage::has_context()),
      logger_(&log) {
    for (const auto& [key, value] : fields) {
        // Track which keys we're adding
        if (previous_context_.find(key) == previous_context_.end()) {
            added_keys_.push_back(key);
        }
        log_context_storage::set(key, value);

        // Also set on logger for non-thread-local context
        std::visit([&log, &key](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::string>) {
                log.set_context(key, v);
            } else if constexpr (std::is_same_v<T, int64_t>) {
                log.set_context(key, v);
            } else if constexpr (std::is_same_v<T, double>) {
                log.set_context(key, v);
            } else if constexpr (std::is_same_v<T, bool>) {
                log.set_context(key, v);
            }
        }, value);
    }
}

void log_context_scope::remove_logger_context(const std::string& key) {
    if (logger_ != nullptr) {
        logger_->remove_context(key);
    }
}

} // namespace kcenon::logger
