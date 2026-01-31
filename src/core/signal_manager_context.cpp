/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "kcenon/logger/core/signal_manager_context.h"

namespace kcenon::logger::core {

signal_manager_context::signal_manager_context(
    std::shared_ptr<security::signal_manager_interface> signal_mgr
)
    : signal_manager_(std::move(signal_mgr))
{
}

auto signal_manager_context::get_signal_manager() const
    -> std::shared_ptr<security::signal_manager_interface>
{
    std::lock_guard lock(mutex_);
    return signal_manager_;
}

void signal_manager_context::set_signal_manager(
    std::shared_ptr<security::signal_manager_interface> signal_mgr
)
{
    std::lock_guard lock(mutex_);
    signal_manager_ = std::move(signal_mgr);
}

bool signal_manager_context::is_initialized() const
{
    std::lock_guard lock(mutex_);
    return signal_manager_ != nullptr;
}

} // namespace kcenon::logger::core
