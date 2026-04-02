// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#pragma once

/**
 * @file output_sink_interface.h
 * @brief Interface for log output destinations (Single Responsibility Principle)
 *  kcenon
 * @since 1.3.0
 * @version Sprint 5-6 - Writer Refactoring
 *
 * @details This interface represents the output destination for log messages.
 * It follows the Single Responsibility Principle by handling ONLY the I/O
 * operations, completely separated from formatting concerns.
 *
 * Note: This is different from log_sink_interface which handles async processing.
 * output_sink_interface is for direct I/O operations.
 */

#include <string>
#include <string_view>
#include <memory>
#include <functional>
#include <kcenon/common/patterns/result.h>
#include <kcenon/logger/core/error_codes.h>

namespace kcenon::logger {

/**
 * @class output_sink_interface
 * @brief Abstract interface for log output destinations (I/O only)
 *
 * @details Defines the contract for outputting pre-formatted log messages.
 * This is separate from log_sink_interface which handles async processing.
 *
 * @since 1.3.0
 */
class output_sink_interface {
public:
    virtual ~output_sink_interface() = default;

    /**
     * @brief Write a pre-formatted message to the output destination
     * @param message Pre-formatted log message
     * @return common::VoidResult Success or error code
     */
    virtual common::VoidResult write_raw(std::string_view message) = 0;

    /**
     * @brief Flush any buffered data
     * @return common::VoidResult Success or error code
     */
    virtual common::VoidResult flush() = 0;

    /**
     * @brief Check if the sink is healthy
     * @return true if operational
     */
    virtual bool is_healthy() const = 0;

    /**
     * @brief Get the name of this sink
     * @return Sink identifier
     */
    virtual std::string get_name() const = 0;

    /**
     * @brief Get sink information
     * @return Diagnostic string
     */
    virtual std::string get_info() const {
        return "";
    }
};

using output_sink_factory = std::function<std::unique_ptr<output_sink_interface>()>;

} // namespace kcenon::logger
