// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file monitoring_interface.h
 * @brief Interface for logger monitoring and health checks
 *
 * DEPRECATED: This file is deprecated and will be removed in version 3.0.0.
 * Please use <kcenon/common/interfaces/monitoring_interface.h> instead.
 *
 * Migration guide:
 * - Replace kcenon::logger::monitoring::monitoring_interface with common::interfaces::IMonitor
 * - Replace kcenon::logger::monitoring::health_status with common::interfaces::health_status
 * - Replace kcenon::logger::monitoring::monitoring_data with common::interfaces::metrics_snapshot
 * - For backward compatibility, include "monitoring_interface_transition.h" temporarily
 */

#pragma once

// Deprecation warning
#if defined(__GNUC__) || defined(__clang__)
#warning "logger/core/monitoring/monitoring_interface.h is deprecated. Use <kcenon/common/interfaces/monitoring_interface.h> instead."
#elif defined(_MSC_VER)
#pragma message("Warning: logger/core/monitoring/monitoring_interface.h is deprecated. Use <kcenon/common/interfaces/monitoring_interface.h> instead.")
#endif

// Forward to transition header for compatibility
#include "monitoring_interface_transition.h"

// NOTE: All definitions have been moved to monitoring_interface_transition.h
// This file is kept for backward compatibility and will be removed in v3.0.0