#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file level_converter.h
 * @brief Utility functions for converting between log level types
 * @author 🍀☀🌕🌥 🌊
 * @since 2.0.0
 *
 * @details This file provides conversion utilities between different log level
 * enumerations used in logger_system and common_system. The conversions ensure
 * seamless interoperability between the legacy logger_system::log_level and
 * the standardized common::interfaces::log_level.
 *
 * @note Both log level enumerations use the same numeric values for equivalent
 * severity levels, making conversion straightforward and zero-overhead.
 */

#include <kcenon/logger/interfaces/logger_types.h>
#include <kcenon/common/interfaces/logger_interface.h>

namespace kcenon::logger {

/**
 * @brief Convert from common::interfaces::log_level to logger_system::log_level
 * @param level The common interfaces log level
 * @return The equivalent logger_system log level
 *
 * @note Both enumerations use identical numeric values, so this is essentially
 * a type-safe cast with no runtime overhead.
 *
 * @deprecated Use common::interfaces::log_level directly instead of converting.
 *             Will be removed in v3.0.0.
 * @since 2.0.0
 */
[[deprecated("Use common::interfaces::log_level directly instead of converting. Will be removed in v3.0.0.")]]
[[nodiscard]] constexpr logger_system::log_level
to_logger_system_level(common::interfaces::log_level level) noexcept {
    switch (level) {
        case common::interfaces::log_level::trace:
            return logger_system::log_level::trace;
        case common::interfaces::log_level::debug:
            return logger_system::log_level::debug;
        case common::interfaces::log_level::info:
            return logger_system::log_level::info;
        case common::interfaces::log_level::warning:
            return logger_system::log_level::warning;
        case common::interfaces::log_level::error:
            return logger_system::log_level::error;
        case common::interfaces::log_level::critical:
            return logger_system::log_level::critical;
        case common::interfaces::log_level::off:
            return logger_system::log_level::off;
        default:
            return logger_system::log_level::info;
    }
}

/**
 * @brief Convert from logger_system::log_level to common::interfaces::log_level
 * @param level The logger_system log level
 * @return The equivalent common interfaces log level
 *
 * @note Both enumerations use identical numeric values, so this is essentially
 * a type-safe cast with no runtime overhead.
 *
 * @note logger_system::log_level has aliases (warn=warning=3, fatal=critical=5),
 * so we use the underlying integer value for conversion to avoid duplicate case errors.
 *
 * @deprecated Use common::interfaces::log_level directly instead of converting.
 *             Will be removed in v3.0.0.
 * @since 2.0.0
 */
[[deprecated("Use common::interfaces::log_level directly instead of converting. Will be removed in v3.0.0.")]]
[[nodiscard]] constexpr common::interfaces::log_level
to_common_level(logger_system::log_level level) noexcept {
    // Use underlying integer to avoid duplicate case value errors
    // (warn and warning both = 3, fatal and critical both = 5)
    const int level_value = static_cast<int>(level);

    switch (level_value) {
        case 0:  // trace
            return common::interfaces::log_level::trace;
        case 1:  // debug
            return common::interfaces::log_level::debug;
        case 2:  // info
            return common::interfaces::log_level::info;
        case 3:  // warn/warning
            return common::interfaces::log_level::warning;
        case 4:  // error
            return common::interfaces::log_level::error;
        case 5:  // fatal/critical
            return common::interfaces::log_level::critical;
        case 6:  // off
            return common::interfaces::log_level::off;
        default:
            return common::interfaces::log_level::info;
    }
}

} // namespace kcenon::logger
