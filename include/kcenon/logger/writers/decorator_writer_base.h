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
 * @file decorator_writer_base.h
 * @brief Base class for decorator pattern writers
 * @author kcenon
 * @since 4.0.0
 *
 * @details This file defines decorator_writer_base, a common base class for
 * all decorator pattern implementations in the writer hierarchy. It eliminates
 * code duplication across decorator writers by providing default implementations
 * for wrapped writer management, health status propagation, name delegation,
 * and flush operations.
 *
 * Part of the Decorator pattern refactoring (#356).
 *
 * @example Basic usage:
 * @code
 * // Create a custom decorator by inheriting from decorator_writer_base
 * class my_decorator : public decorator_writer_base {
 * public:
 *     explicit my_decorator(std::unique_ptr<log_writer_interface> wrapped)
 *         : decorator_writer_base(std::move(wrapped), "my_decorator") {}
 *
 *     common::VoidResult write(const log_entry& entry) override {
 *         // Add custom behavior before/after delegation
 *         return wrapped().write(entry);
 *     }
 * };
 * @endcode
 */

#include "../interfaces/log_writer_interface.h"
#include "../interfaces/writer_category.h"

#include <memory>
#include <string>
#include <string_view>

namespace kcenon::logger {

/**
 * @class decorator_writer_base
 * @brief Abstract base class for decorator pattern log writers
 *
 * @details This class provides common functionality for all decorator writers:
 * - Wrapped writer storage and lifecycle management
 * - Default implementations for get_name(), is_healthy(), and flush()
 * - Protected access to wrapped writer for derived classes
 *
 * Derived classes only need to implement write() with their specific behavior.
 *
 * Key features:
 * - Accepts any log_writer_interface implementation (not just base_writer)
 * - Enables unlimited decorator stacking
 * - Thread-safety depends on the wrapped writer
 * - Follows Liskov Substitution Principle
 *
 * Category: Decorator (wraps another writer)
 *
 * @note This is an abstract class; derived classes must implement write().
 *
 * @since 4.0.0
 */
class decorator_writer_base : public log_writer_interface, public decorator_writer_tag {
public:
    /**
     * @brief Construct a decorator writer base
     *
     * @param wrapped The writer to wrap with this decorator
     * @param decorator_name Name prefix for this decorator (e.g., "async", "filtered")
     *
     * @throws std::invalid_argument if wrapped is nullptr
     *
     * @since 4.0.0
     */
    explicit decorator_writer_base(std::unique_ptr<log_writer_interface> wrapped,
                                   std::string_view decorator_name);

    /**
     * @brief Virtual destructor for proper cleanup
     */
    ~decorator_writer_base() override = default;

    // Non-copyable
    decorator_writer_base(const decorator_writer_base&) = delete;
    decorator_writer_base& operator=(const decorator_writer_base&) = delete;

    // Movable
    decorator_writer_base(decorator_writer_base&&) noexcept = default;
    decorator_writer_base& operator=(decorator_writer_base&&) noexcept = default;

    /**
     * @brief Write a log entry (must be implemented by derived classes)
     *
     * @param entry The log entry to write
     * @return common::VoidResult indicating success or failure
     *
     * @since 4.0.0
     */
    common::VoidResult write(const log_entry& entry) override = 0;

    /**
     * @brief Flush the wrapped writer
     *
     * @return common::VoidResult from the wrapped writer's flush operation
     *
     * @details Default implementation delegates to wrapped writer.
     * Override if additional flush behavior is needed.
     *
     * @since 4.0.0
     */
    common::VoidResult flush() override;

    /**
     * @brief Get the name of this writer
     *
     * @return String in format "<decorator_name>_<wrapped_name>"
     *
     * @details Default implementation combines decorator name with wrapped
     * writer's name. Override for custom naming behavior.
     *
     * @since 4.0.0
     */
    std::string get_name() const override;

    /**
     * @brief Check if the writer is healthy
     *
     * @return Health status of the wrapped writer
     *
     * @details Default implementation delegates to wrapped writer.
     * Override to add additional health checks.
     *
     * @since 4.0.0
     */
    bool is_healthy() const override;

    /**
     * @brief Get the wrapped writer (const version)
     *
     * @return Const pointer to the wrapped writer
     *
     * @since 4.0.0
     */
    const log_writer_interface* get_wrapped_writer() const noexcept;

protected:
    /**
     * @brief Access the wrapped writer (non-const)
     *
     * @return Reference to the wrapped writer
     *
     * @note Use this in derived classes to delegate operations
     *
     * @since 4.0.0
     */
    log_writer_interface& wrapped() noexcept;

    /**
     * @brief Access the wrapped writer (const)
     *
     * @return Const reference to the wrapped writer
     *
     * @since 4.0.0
     */
    const log_writer_interface& wrapped() const noexcept;

    /**
     * @brief Get the decorator name
     *
     * @return The decorator name string
     *
     * @since 4.0.0
     */
    const std::string& decorator_name() const noexcept;

private:
    std::unique_ptr<log_writer_interface> wrapped_;
    std::string decorator_name_;
};

}  // namespace kcenon::logger
