// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file log_sink_interface.h
 * @brief Interface for log sinks that receive formatted output.
 *
 */

#pragma once

#include <kcenon/common/patterns/result.h>
#include <kcenon/logger/core/error_codes.h>

namespace kcenon::logger {

// Forward declaration
struct log_entry;

/**
 * @interface log_sink_interface
 * @brief Interface for log sinks
 * 
 * This interface defines the contract for log processing sinks.
 * Sinks are responsible for processing log entries (e.g., async queuing).
 */
class log_sink_interface {
public:
    virtual ~log_sink_interface() = default;
    
    /**
     * @brief Process a log entry
     * @param entry The log entry to process
     * @return common::VoidResult indicating success or failure
     */
    virtual common::VoidResult process(const log_entry& entry) = 0;
    
    /**
     * @brief Check if this sink supports async processing
     * @return true if async processing is supported
     */
    virtual bool supports_async() const = 0;
};

} // namespace kcenon::logger