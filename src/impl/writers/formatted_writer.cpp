// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#include <kcenon/logger/writers/formatted_writer.h>

#include <kcenon/logger/interfaces/log_entry.h>

#include <stdexcept>

namespace kcenon::logger {

formatted_writer::formatted_writer(std::unique_ptr<log_writer_interface> wrapped,
                                   std::unique_ptr<log_formatter_interface> formatter)
    : decorator_writer_base(std::move(wrapped), "formatted"), formatter_(std::move(formatter)) {
    // Base class constructor handles nullptr check for wrapped writer
}

common::VoidResult formatted_writer::write(const log_entry& entry) {
    // If no formatter, pass through all entries unchanged
    if (!formatter_) {
        return wrapped().write(entry);
    }

    // Apply formatter to get formatted message
    std::string formatted_message = formatter_->format(entry);

    // Create a new log entry with the formatted message
    log_entry formatted_entry(entry.level, formatted_message, entry.timestamp);

    // Copy optional fields from original entry
    formatted_entry.location = entry.location;
    formatted_entry.thread_id = entry.thread_id;
    formatted_entry.category = entry.category;
    formatted_entry.otel_ctx = entry.otel_ctx;
    formatted_entry.fields = entry.fields;

    // Delegate to wrapped writer
    return wrapped().write(formatted_entry);
}

std::string formatted_writer::get_name() const {
    if (formatter_) {
        return "formatted(" + formatter_->get_name() + ")_" + wrapped().get_name();
    }
    // Use base class default format when no formatter
    return decorator_writer_base::get_name();
}

const log_formatter_interface* formatted_writer::get_formatter() const {
    return formatter_.get();
}

// Factory function
std::unique_ptr<formatted_writer> make_formatted_writer(
    std::unique_ptr<log_writer_interface> writer,
    std::unique_ptr<log_formatter_interface> formatter) {
    return std::make_unique<formatted_writer>(std::move(writer), std::move(formatter));
}

}  // namespace kcenon::logger
