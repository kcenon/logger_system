// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

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
