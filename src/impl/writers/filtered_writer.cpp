// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#include <kcenon/logger/writers/filtered_writer.h>

#include <kcenon/logger/interfaces/log_entry.h>

#include <stdexcept>

namespace kcenon::logger {

filtered_writer::filtered_writer(std::unique_ptr<log_writer_interface> wrapped,
                                 std::unique_ptr<log_filter_interface> filter)
    : decorator_writer_base(std::move(wrapped), "filtered"), filter_(std::move(filter)) {
    // Base class constructor handles nullptr check for wrapped writer
}

common::VoidResult filtered_writer::write(const log_entry& entry) {
    // If no filter, pass through all entries
    if (!filter_) {
        return wrapped().write(entry);
    }

    // Check if entry passes the filter
    if (filter_->should_log(entry)) {
        return wrapped().write(entry);
    }

    // Entry filtered out - return success (not an error)
    return common::ok();
}

std::string filtered_writer::get_name() const {
    if (filter_) {
        return "filtered(" + filter_->get_name() + ")_" + wrapped().get_name();
    }
    // Use base class default format when no filter
    return decorator_writer_base::get_name();
}

const log_filter_interface* filtered_writer::get_filter() const {
    return filter_.get();
}

// Factory function
std::unique_ptr<filtered_writer> make_filtered_writer(
    std::unique_ptr<log_writer_interface> writer,
    std::unique_ptr<log_filter_interface> filter) {
    return std::make_unique<filtered_writer>(std::move(writer), std::move(filter));
}

}  // namespace kcenon::logger
