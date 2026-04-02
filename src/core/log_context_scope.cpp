// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

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
            log.context().set(key, v);
        }, value);
    }
}

void log_context_scope::remove_logger_context(const std::string& key) {
    if (logger_ != nullptr) {
        logger_->context().remove(key);
    }
}

} // namespace kcenon::logger
