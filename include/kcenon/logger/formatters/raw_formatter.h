// BSD 3-Clause License
// Copyright (c) 2025, kcenon
// See the LICENSE file in the project root for full license information.

#pragma once

/**
 * @file raw_formatter.h
 * @brief Pass-through formatter that emits the message field unchanged.
 *
 * @details Intended for binary payloads (e.g., encrypted entries) where the
 * default timestamp prefix would corrupt the output. The formatter returns
 * exactly the bytes stored in log_entry.message, with no timestamp, level, or
 * source-location decoration.
 *
 * Use with file_writer when wrapping it behind encrypted_writer:
 * @code
 * auto file = std::make_unique<file_writer>(
 *     "secure.log.enc",
 *     false,
 *     std::make_unique<raw_formatter>());
 * encrypted_writer writer(std::move(file), std::move(config));
 * @endcode
 *
 * @since 4.1.0
 */

#include "../interfaces/log_entry.h"
#include "../interfaces/log_formatter_interface.h"

namespace kcenon::logger {

/**
 * @class raw_formatter
 * @brief Formatter that returns log_entry.message verbatim.
 *
 * Thread-safety: stateless; safe to use concurrently.
 */
class raw_formatter : public log_formatter_interface {
public:
    raw_formatter() = default;
    ~raw_formatter() override = default;

    std::string format(const log_entry& entry) const override {
        return entry.message.to_string();
    }

    std::string get_name() const override {
        return "raw_formatter";
    }
};

} // namespace kcenon::logger
