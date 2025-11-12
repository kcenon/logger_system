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

#pragma once

/**
 * @file monitoring_integration_detector.h
 * @brief Automatically toggles monitoring_system integration macros when headers are available.
 */

#if !defined(LOGGER_MONITORING_INTEGRATION_DETECTED)
#define LOGGER_MONITORING_INTEGRATION_DETECTED

#if !defined(USE_MONITORING_SYSTEM) && !defined(LOGGER_STANDALONE_MODE)
#  if __has_include(<kcenon/monitoring/interfaces/monitoring_interface.h>)
#    define USE_MONITORING_SYSTEM 1
#  endif
#endif

#if !defined(ENABLE_MONITORING_INTEGRATION) && defined(USE_MONITORING_SYSTEM)
#  if __has_include(<kcenon/monitoring/interfaces/monitoring_interface.h>)
#    define ENABLE_MONITORING_INTEGRATION 1
#  endif
#endif

#endif // LOGGER_MONITORING_INTEGRATION_DETECTED
