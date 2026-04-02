// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#pragma once

#include <string>

namespace kcenon::logger {

// Forward declaration
struct log_entry;

/**
 * @interface log_filter_interface
 * @brief Interface for log filters
 * 
 * This interface defines the contract for log filtering.
 * Filters determine whether a log entry should be processed.
 */
class log_filter_interface {
public:
    virtual ~log_filter_interface() = default;
    
    /**
     * @brief Check if a log entry should be processed
     * @param entry The log entry to check
     * @return true if the entry should be logged, false otherwise
     */
    virtual bool should_log(const log_entry& entry) const = 0;
    
    /**
     * @brief Get the name of this filter
     * @return Filter name for identification
     */
    virtual std::string get_name() const = 0;
};

} // namespace kcenon::logger