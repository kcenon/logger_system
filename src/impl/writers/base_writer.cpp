// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file base_writer.cpp
 * @brief Implementation of base_writer class
 *  kcenon
 * @since 1.0.0
 * @since 1.3.0 Separated from console_writer.cpp for proper code organization
 */

#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>

namespace kcenon::logger {

base_writer::base_writer(std::unique_ptr<log_formatter_interface> formatter)
    : formatter_(std::move(formatter)) {
    if (!formatter_) {
        // Default to timestamp_formatter if none provided
        formatter_ = std::make_unique<timestamp_formatter>();
    }

    // Apply color setting from legacy API to formatter
    auto opts = formatter_->get_options();
    opts.use_colors = use_color_;
    formatter_->set_options(opts);
}

log_formatter_interface* base_writer::get_formatter() const {
    return formatter_.get();
}

std::string base_writer::format_log_entry(const log_entry& entry) const {
    if (!formatter_) {
        // Fallback if formatter is somehow null
        return entry.message.to_string();
    }
    return formatter_->format(entry);
}

} // namespace kcenon::logger
