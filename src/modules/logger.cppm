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
 * @file logger.cppm
 * @brief Primary module interface for kcenon.logger
 * @author kcenon
 * @since 3.0.0
 *
 * @details This file defines the primary module interface for the logger system.
 * It exports all public functionality through module partitions:
 * - kcenon.logger.core: Core logging classes (logger, log_context, log_collector)
 * - kcenon.logger.backends: Backend implementations for integration
 * - kcenon.logger.analysis: Log analysis utilities
 *
 * @note C++20 modules require CMake 3.28+ with CMAKE_CXX_SCAN_FOR_MODULES=ON
 *
 * @example Basic usage:
 * @code
 * import kcenon.logger;
 *
 * int main() {
 *     kcenon::logger::logger log;
 *     log.add_writer(std::make_unique<kcenon::logger::console_writer>());
 *     log.start();
 *     log.log(kcenon::logger::log_level::info, "Hello from modules!");
 *     log.stop();
 *     return 0;
 * }
 * @endcode
 */

export module kcenon.logger;

// Import required dependencies
import kcenon.common;

// Re-export all partitions
export import :core;
export import :backends;
export import :analysis;

/**
 * @brief Module-level documentation namespace
 *
 * The kcenon.logger module provides a high-performance, thread-safe logging
 * system with the following features:
 *
 * - Asynchronous logging with configurable batching
 * - Multiple writer support (console, file, network, rotating file)
 * - Integration with common_system interfaces
 * - Optional thread_system integration for enhanced async performance
 * - Metrics collection and performance monitoring
 * - Filtering and routing capabilities
 * - Signal-safe emergency flush for crash handling
 *
 * Module structure:
 * - kcenon.logger.core: Logger, LogContext, LogCollector, writers
 * - kcenon.logger.backends: Integration backend implementations
 * - kcenon.logger.analysis: Log analysis and statistics utilities
 */
