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
 * @file composite_writer.h
 * @brief Composite writer combining formatter and sink (Pipeline Pattern)
 * @author 🍀☀🌕🌥 🌊
 * @since 1.3.0
 * @version Sprint 5-6 - Writer Refactoring
 *
 * @details Implements the Pipeline pattern by composing a formatter and sink.
 * This follows the Single Responsibility Principle by delegating:
 * - Formatting to log_formatter_interface
 * - I/O to log_sink_interface
 *
 * The writer simply coordinates the pipeline: entry -> format -> write
 */

#include <memory>
#include "../interfaces/log_writer_interface.h"
#include "../interfaces/log_formatter_interface.h"
#include "../interfaces/log_sink_interface.h"

namespace kcenon::logger {

/**
 * @class composite_writer
 * @brief Coordinates formatting and output through a pipeline
 *
 * @details Implements the Composite and Pipeline patterns to create a
 * flexible logging architecture. This writer:
 * 1. Accepts a log_entry
 * 2. Passes it to the formatter (format stage)
 * 3. Passes the formatted string to the sink (output stage)
 *
 * Benefits:
 * - Single Responsibility: Each component has one job
 * - Open/Closed: Can add new formatters/sinks without modifying this class
 * - Dependency Injection: Components are injected via constructor
 * - Testability: Each component can be tested independently
 * - Flexibility: Mix and match any formatter with any sink
 *
 * @example Creating a composite writer:
 * @code
 * // Create formatter
 * auto formatter = std::make_unique<json_formatter>();
 *
 * // Create sink
 * auto sink = std::make_unique<file_sink>("/var/log/app.log");
 *
 * // Compose them into a writer
 * auto writer = std::make_unique<composite_writer>(
 *     std::move(formatter),
 *     std::move(sink)
 * );
 *
 * // Use the writer
 * log_entry entry = create_log_entry(...);
 * writer->write(entry);
 * @endcode
 *
 * @note This class is thread-safe if the underlying formatter and sink
 * are thread-safe (which they should be).
 *
 * @since 1.3.0
 */
class composite_writer : public log_writer_interface {
public:
    /**
     * @brief Construct a composite writer
     * @param formatter Formatter to use for converting log_entry to string
     * @param sink Sink to use for outputting formatted messages
     *
     * @details Creates a writer that pipelines log entries through the
     * provided formatter and sink.
     *
     * @pre formatter must not be nullptr
     * @pre sink must not be nullptr
     *
     * @throws std::invalid_argument if formatter or sink is nullptr
     *
     * @since 1.3.0
     */
    composite_writer(std::unique_ptr<log_formatter_interface> formatter,
                    std::unique_ptr<log_sink_interface> sink)
        : formatter_(std::move(formatter))
        , sink_(std::move(sink))
    {
        if (!formatter_) {
            throw std::invalid_argument("Formatter cannot be null");
        }
        if (!sink_) {
            throw std::invalid_argument("Sink cannot be null");
        }
    }

    /**
     * @brief Destructor
     *
     * @details Ensures the sink is flushed before destruction.
     *
     * @since 1.3.0
     */
    ~composite_writer() override {
        flush(); // Ensure all data is written
    }

    /**
     * @brief Write a log entry through the pipeline
     * @param entry The log entry to write
     * @return result_void Success or error code
     *
     * @details Pipeline stages:
     * 1. Format: entry -> formatter->format(entry) -> formatted_string
     * 2. Output: formatted_string -> sink->write_raw(formatted_string)
     *
     * @note Thread-safety depends on formatter and sink implementations.
     *
     * @since 1.3.0
     */
    result_void write(const log_entry& entry) override {
        // Stage 1: Format the log entry
        std::string formatted = formatter_->format(entry);

        // Stage 2: Write to sink
        return sink_->write_raw(formatted);
    }

    /**
     * @brief Flush the sink
     * @return result_void Success or error code
     *
     * @details Delegates flushing to the sink.
     *
     * @since 1.3.0
     */
    result_void flush() override {
        return sink_->flush();
    }

    /**
     * @brief Get the name of this writer
     * @return Composite name: "formatter_name+sink_name"
     *
     * @example "json+file", "timestamp+console"
     *
     * @since 1.3.0
     */
    std::string get_name() const override {
        return formatter_->get_name() + "+" + sink_->get_name();
    }

    /**
     * @brief Check if the writer is healthy
     * @return true if the sink is healthy
     *
     * @details A composite writer is healthy if its sink is healthy.
     * Formatters are assumed to always be healthy.
     *
     * @since 1.3.0
     */
    bool is_healthy() const override {
        return sink_->is_healthy();
    }

    /**
     * @brief Get the formatter
     * @return Non-owning pointer to the formatter
     *
     * @note For inspection/configuration purposes only.
     * Do not delete the returned pointer.
     *
     * @since 1.3.0
     */
    log_formatter_interface* get_formatter() const {
        return formatter_.get();
    }

    /**
     * @brief Get the sink
     * @return Non-owning pointer to the sink
     *
     * @note For inspection/configuration purposes only.
     * Do not delete the returned pointer.
     *
     * @since 1.3.0
     */
    log_sink_interface* get_sink() const {
        return sink_.get();
    }

private:
    std::unique_ptr<log_formatter_interface> formatter_; ///< Formatting stage
    std::unique_ptr<log_sink_interface> sink_;          ///< Output stage
};

/**
 * @brief Factory function to create a composite writer
 * @param formatter Formatter to use
 * @param sink Sink to use
 * @return Unique pointer to the created writer
 *
 * @details Convenience function for creating composite writers with
 * type deduction.
 *
 * @example
 * @code
 * auto writer = make_composite_writer(
 *     std::make_unique<json_formatter>(),
 *     std::make_unique<console_sink>()
 * );
 * @endcode
 *
 * @since 1.3.0
 */
inline std::unique_ptr<composite_writer> make_composite_writer(
    std::unique_ptr<log_formatter_interface> formatter,
    std::unique_ptr<log_sink_interface> sink)
{
    return std::make_unique<composite_writer>(
        std::move(formatter),
        std::move(sink)
    );
}

} // namespace kcenon::logger
