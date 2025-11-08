/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "kcenon/logger/security/signal_manager.h"

namespace kcenon::logger::security {

// Define static member variable
std::atomic<signal_manager*> signal_manager::current_instance_{nullptr};

} // namespace kcenon::logger::security
