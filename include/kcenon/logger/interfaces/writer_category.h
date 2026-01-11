#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
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
 * @file writer_category.h
 * @brief Writer category interfaces and type traits
 * @author 🍀☀🌕🌥 🌊
 * @since 1.4.0
 * @version Phase 2.0 - Writer Hierarchy Clarification
 *
 * @details This file defines interfaces and type traits for categorizing
 * log writers based on their operational characteristics:
 *
 * - **Synchronous Writers**: Blocking I/O operations, immediate output
 * - **Asynchronous Writers**: Non-blocking, queue-based background processing
 * - **Decorator Writers**: Wrap other writers to add functionality
 * - **Composite Writers**: Combine multiple formatters/sinks or writers
 *
 * These categories help users understand writer behavior and make
 * informed decisions about which writer to use for their needs.
 *
 * @note Writers can belong to multiple categories (e.g., a decorator
 * that wraps an async writer is both a decorator and async).
 */

#include <type_traits>
#include <cstdint>

namespace kcenon::logger {

// Forward declarations
class log_writer_interface;
class base_writer;

/**
 * @enum writer_category
 * @brief Categories of log writers based on operational behavior
 *
 * @details Writers are categorized by their primary operational mode.
 * This categorization helps in:
 * - Understanding writer behavior
 * - Selecting appropriate writers for use cases
 * - Documentation and API clarity
 *
 * @since 1.4.0
 */
enum class writer_category : std::uint8_t {
    /** Synchronous, blocking write operations */
    synchronous,
    /** Asynchronous, non-blocking write operations */
    asynchronous,
    /** Wraps another writer to add functionality */
    decorator,
    /** Combines multiple outputs or pipeline stages */
    composite
};

/**
 * @interface sync_writer_tag
 * @brief Tag interface for synchronous writers
 *
 * @details Synchronous writers perform blocking I/O operations.
 * When write() returns, the data has been written to the destination.
 *
 * Characteristics:
 * - Immediate, guaranteed output
 * - Blocking operation (caller waits)
 * - Suitable for low-volume, critical logging
 * - Thread-safe via mutex (thread_safe_writer)
 *
 * Examples: console_writer, file_writer, rotating_file_writer
 *
 * @since 1.4.0
 */
struct sync_writer_tag {
    static constexpr writer_category category = writer_category::synchronous;
};

/**
 * @interface async_writer_tag
 * @brief Tag interface for asynchronous writers
 *
 * @details Asynchronous writers use background processing for I/O.
 * When write() returns, data is queued but may not be written yet.
 *
 * Characteristics:
 * - Non-blocking operation
 * - Higher throughput potential
 * - Requires explicit flush for guaranteed delivery
 * - Suitable for high-volume logging
 *
 * Examples: async_writer, batch_writer, network_writer
 *
 * @since 1.4.0
 */
struct async_writer_tag {
    static constexpr writer_category category = writer_category::asynchronous;
};

/**
 * @interface decorator_writer_tag
 * @brief Tag interface for decorator writers
 *
 * @details Decorator writers wrap another writer to add functionality
 * without modifying the wrapped writer's code (Decorator Pattern).
 *
 * Characteristics:
 * - Wraps an existing writer
 * - Adds or modifies behavior
 * - Transparent to the logging system
 * - Can be stacked (decorator wrapping decorator)
 *
 * Examples: async_writer, batch_writer, critical_writer, encrypted_writer
 *
 * @since 1.4.0
 */
struct decorator_writer_tag {
    static constexpr writer_category category = writer_category::decorator;
};

/**
 * @interface composite_writer_tag
 * @brief Tag interface for composite writers
 *
 * @details Composite writers combine multiple outputs or pipeline stages.
 * They coordinate between different components to produce output.
 *
 * Characteristics:
 * - Combines multiple components
 * - May use Pipeline or Composite pattern
 * - Single entry point, multiple outputs possible
 * - Coordinates formatting and I/O
 *
 * Examples: composite_writer (formatter+sink), hybrid_writer
 *
 * @since 1.4.0
 */
struct composite_writer_tag {
    static constexpr writer_category category = writer_category::composite;
};

// ============================================================================
// Type Traits
// ============================================================================

/**
 * @brief Type trait to check if a writer is synchronous
 * @tparam T Writer type to check
 *
 * @since 1.4.0
 */
template<typename T>
struct is_sync_writer : std::is_base_of<sync_writer_tag, T> {};

template<typename T>
inline constexpr bool is_sync_writer_v = is_sync_writer<T>::value;

/**
 * @brief Type trait to check if a writer is asynchronous
 * @tparam T Writer type to check
 *
 * @since 1.4.0
 */
template<typename T>
struct is_async_writer : std::is_base_of<async_writer_tag, T> {};

template<typename T>
inline constexpr bool is_async_writer_v = is_async_writer<T>::value;

/**
 * @brief Type trait to check if a writer is a decorator
 * @tparam T Writer type to check
 *
 * @since 1.4.0
 */
template<typename T>
struct is_decorator_writer : std::is_base_of<decorator_writer_tag, T> {};

template<typename T>
inline constexpr bool is_decorator_writer_v = is_decorator_writer<T>::value;

/**
 * @brief Type trait to check if a writer is composite
 * @tparam T Writer type to check
 *
 * @since 1.4.0
 */
template<typename T>
struct is_composite_writer : std::is_base_of<composite_writer_tag, T> {};

template<typename T>
inline constexpr bool is_composite_writer_v = is_composite_writer<T>::value;

// ============================================================================
// C++20 Concepts
// ============================================================================

/**
 * @concept SyncWriter
 * @brief Concept for synchronous writers
 *
 * @tparam T Type to check
 *
 * @details Requires the type to be a log writer and tagged as synchronous.
 *
 * @since 1.4.0
 */
template<typename T>
concept SyncWriter = std::is_base_of_v<log_writer_interface, T> &&
                     std::is_base_of_v<sync_writer_tag, T>;

/**
 * @concept AsyncWriter
 * @brief Concept for asynchronous writers
 *
 * @tparam T Type to check
 *
 * @details Requires the type to be a log writer and tagged as asynchronous.
 *
 * @since 1.4.0
 */
template<typename T>
concept AsyncWriter = std::is_base_of_v<log_writer_interface, T> &&
                      std::is_base_of_v<async_writer_tag, T>;

/**
 * @concept DecoratorWriter
 * @brief Concept for decorator writers
 *
 * @tparam T Type to check
 *
 * @details Requires the type to be a log writer and tagged as decorator.
 *
 * @since 1.4.0
 */
template<typename T>
concept DecoratorWriter = std::is_base_of_v<log_writer_interface, T> &&
                          std::is_base_of_v<decorator_writer_tag, T>;

/**
 * @concept CompositeWriter
 * @brief Concept for composite writers
 *
 * @tparam T Type to check
 *
 * @details Requires the type to be a log writer and tagged as composite.
 *
 * @since 1.4.0
 */
template<typename T>
concept CompositeWriter = std::is_base_of_v<log_writer_interface, T> &&
                          std::is_base_of_v<composite_writer_tag, T>;

// ============================================================================
// Runtime Category Query
// ============================================================================

/**
 * @brief Get the primary category of a writer type
 * @tparam T Writer type
 * @return Primary writer category
 *
 * @details Returns the primary category for a writer type.
 * Priority order: composite > decorator > async > sync
 *
 * @since 1.4.0
 */
template<typename T>
constexpr writer_category get_writer_category() {
    if constexpr (is_composite_writer_v<T>) {
        return writer_category::composite;
    } else if constexpr (is_decorator_writer_v<T>) {
        return writer_category::decorator;
    } else if constexpr (is_async_writer_v<T>) {
        return writer_category::asynchronous;
    } else if constexpr (is_sync_writer_v<T>) {
        return writer_category::synchronous;
    } else {
        // Default for untagged writers
        return writer_category::synchronous;
    }
}

/**
 * @brief Get string representation of writer category
 * @param cat Writer category
 * @return Category name as string
 *
 * @since 1.4.0
 */
constexpr const char* to_string(writer_category cat) noexcept {
    switch (cat) {
        case writer_category::synchronous: return "synchronous";
        case writer_category::asynchronous: return "asynchronous";
        case writer_category::decorator: return "decorator";
        case writer_category::composite: return "composite";
    }
    return "unknown";
}

} // namespace kcenon::logger
