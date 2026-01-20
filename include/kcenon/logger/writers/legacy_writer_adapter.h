#pragma once

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
 * @file legacy_writer_adapter.h
 * @brief Adapter for legacy writer implementations
 * @since 3.5.0
 *
 * @details This file provides adapter classes for backward compatibility
 * with legacy writer implementations that use the old parameter-based
 * write API instead of the modern log_entry-based API.
 *
 * @note This adapter is provided for migration purposes. New implementations
 * should use the modern write(const log_entry&) API directly.
 */

#include "../interfaces/log_writer_interface.h"
#include "../interfaces/log_entry.h"
#include "../interfaces/writer_category.h"
#include <kcenon/common/patterns/result.h>
#include <memory>
#include <string>
#include <chrono>

namespace kcenon::logger {

/**
 * @interface legacy_writer_interface
 * @brief Interface for legacy writer implementations
 *
 * @details This interface defines the legacy parameter-based write API.
 * Implementations of this interface can be wrapped with legacy_writer_adapter
 * to use them with the modern logging system.
 *
 * @deprecated New implementations should directly implement log_writer_interface
 * using the write(const log_entry&) method.
 *
 * @since 3.5.0
 */
class legacy_writer_interface {
public:
    virtual ~legacy_writer_interface() = default;

    /**
     * @brief Write a log entry using legacy parameter-based API
     * @param level Log severity level
     * @param message Log message text
     * @param file Source file path (empty if not available)
     * @param line Source line number (0 if not available)
     * @param function Function name (empty if not available)
     * @param timestamp Time when the log entry was created
     * @return common::VoidResult Success or error code
     */
    virtual common::VoidResult write(common::interfaces::log_level level,
                                     const std::string& message,
                                     const std::string& file,
                                     int line,
                                     const std::string& function,
                                     const std::chrono::system_clock::time_point& timestamp) = 0;

    /**
     * @brief Flush any buffered data
     * @return common::VoidResult Success or error code
     */
    virtual common::VoidResult flush() = 0;

    /**
     * @brief Get the name of this writer
     * @return Writer name for identification
     */
    virtual std::string get_name() const = 0;

    /**
     * @brief Check if the writer is healthy
     * @return true if the writer is operational
     */
    virtual bool is_healthy() const { return true; }
};

/**
 * @class legacy_writer_adapter
 * @brief Adapter that wraps legacy writers for use with modern API
 *
 * @details This adapter converts the modern write(const log_entry&) calls
 * to the legacy parameter-based API. Use this to integrate existing legacy
 * writer implementations with the modern logging system.
 *
 * Category: Decorator (wraps legacy writer to provide modern API)
 *
 * @example Wrapping a legacy writer:
 * @code
 * class my_legacy_writer : public legacy_writer_interface {
 *     common::VoidResult write(common::interfaces::log_level level,
 *                              const std::string& message, ...) override {
 *         // Legacy implementation
 *         return common::ok();
 *     }
 *     // ... other methods
 * };
 *
 * auto legacy = std::make_unique<my_legacy_writer>();
 * auto modern = std::make_unique<legacy_writer_adapter>(std::move(legacy));
 * logger->add_writer("my_writer", std::move(modern));
 * @endcode
 *
 * @since 3.5.0
 */
class legacy_writer_adapter : public log_writer_interface, public decorator_writer_tag {
public:
    /**
     * @brief Construct adapter with legacy writer
     * @param legacy_writer The legacy writer to wrap
     * @throws std::invalid_argument if legacy_writer is null
     */
    explicit legacy_writer_adapter(std::unique_ptr<legacy_writer_interface> legacy_writer)
        : legacy_writer_(std::move(legacy_writer))
    {
        if (!legacy_writer_) {
            throw std::invalid_argument("Legacy writer cannot be null");
        }
    }

    /**
     * @brief Write using modern API, delegates to legacy writer
     * @param entry The log entry to write
     * @return common::VoidResult Success or error code
     *
     * @details Extracts fields from log_entry and delegates to legacy API.
     * This involves conversion overhead but maintains backward compatibility.
     */
    common::VoidResult write(const log_entry& entry) override {
        // Extract source location if present
        std::string file = entry.location ? entry.location->file.to_string() : "";
        int line = entry.location ? entry.location->line : 0;
        std::string function = entry.location ? entry.location->function.to_string() : "";

        // Convert log_level from logger_system to common::interfaces
        auto level = static_cast<common::interfaces::log_level>(static_cast<int>(entry.level));

        return legacy_writer_->write(level,
                                     entry.message.to_string(),
                                     file,
                                     line,
                                     function,
                                     entry.timestamp);
    }

    /**
     * @brief Flush the legacy writer
     */
    common::VoidResult flush() override {
        return legacy_writer_->flush();
    }

    /**
     * @brief Get writer name with adapter prefix
     */
    std::string get_name() const override {
        return "legacy_adapter_" + legacy_writer_->get_name();
    }

    /**
     * @brief Check if legacy writer is healthy
     */
    bool is_healthy() const override {
        return legacy_writer_->is_healthy();
    }

    /**
     * @brief Get access to the underlying legacy writer
     * @return Non-owning pointer to the legacy writer
     */
    legacy_writer_interface* get_legacy_writer() const {
        return legacy_writer_.get();
    }

private:
    std::unique_ptr<legacy_writer_interface> legacy_writer_;
};

} // namespace kcenon::logger
