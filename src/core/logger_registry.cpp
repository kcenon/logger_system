/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "kcenon/logger/core/logger_registry.h"
#include <algorithm>

namespace kcenon::logger::core {

void logger_registry::register_logger(security::critical_logger_interface* log)
{
    if (!log) {
        return;
    }

    std::unique_lock lock(mutex_);

    // Check if already registered
    auto it = std::find(loggers_.begin(), loggers_.end(), log);
    if (it == loggers_.end()) {
        loggers_.push_back(log);
    }
}

void logger_registry::unregister_logger(security::critical_logger_interface* log)
{
    if (!log) {
        return;
    }

    std::unique_lock lock(mutex_);

    // Remove all instances (should be only one, but use remove to be safe)
    loggers_.erase(
        std::remove(loggers_.begin(), loggers_.end(), log),
        loggers_.end()
    );
}

auto logger_registry::get_registered_loggers() const
    -> std::vector<security::critical_logger_interface*>
{
    std::shared_lock lock(mutex_);
    return loggers_;
}

bool logger_registry::empty() const
{
    std::shared_lock lock(mutex_);
    return loggers_.empty();
}

size_t logger_registry::size() const
{
    std::shared_lock lock(mutex_);
    return loggers_.size();
}

} // namespace kcenon::logger::core
