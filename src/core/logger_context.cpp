/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "kcenon/logger/core/logger_context.h"
#include "kcenon/logger/security/signal_manager.h"

namespace kcenon::logger::core {

logger_context::logger_context()
    : signal_context_(std::make_shared<security::signal_manager>())
    , registry_()
{
}

logger_context::logger_context(
    std::shared_ptr<security::signal_manager_interface> signal_mgr
)
    : signal_context_(std::move(signal_mgr))
    , registry_()
{
}

logger_context::~logger_context() = default;

} // namespace kcenon::logger::core
