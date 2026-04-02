// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#include <kcenon/logger/writers/thread_safe_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>

namespace kcenon::logger {

thread_safe_writer::thread_safe_writer(std::unique_ptr<log_formatter_interface> formatter)
    : base_writer(std::move(formatter)) {
}

common::VoidResult thread_safe_writer::write(const log_entry& entry) {
    std::lock_guard<std::mutex> lock(write_mutex_);
    return write_entry_impl(entry);
}

common::VoidResult thread_safe_writer::flush() {
    std::lock_guard<std::mutex> lock(write_mutex_);
    return flush_impl();
}

} // namespace kcenon::logger
