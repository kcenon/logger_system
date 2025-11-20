// BSD 3-Clause License

#pragma once

#include <memory>
#include <string>

#include <kcenon/logger/core/log_context.h>
#include <kcenon/logger/core/logger.h>

namespace kcenon::logger::core {

template<logger_system::log_level MinLevel = logger_system::log_level::info>
class filtered_logger {
public:
    explicit filtered_logger(std::shared_ptr<kcenon::logger::logger> impl)
        : logger_(std::move(impl)) {}

    void log(logger_system::log_level level,
             const std::string& message,
             const log_context& context) const {
        if (!logger_) {
            return;
        }
        if (static_cast<int>(level) < static_cast<int>(MinLevel)) {
            return;
        }
        logger_->log(level, message,
                     std::string(context.file),
                     context.line,
                     std::string(context.function));
    }

    template<logger_system::log_level Level>
    void log(const std::string& message, const log_context& context) const {
        if constexpr (static_cast<int>(Level) >= static_cast<int>(MinLevel)) {
            if (logger_) {
                logger_->log(Level, message,
                              std::string(context.file),
                              context.line,
                              std::string(context.function));
            }
        }
    }

private:
    std::shared_ptr<kcenon::logger::logger> logger_;
};

} // namespace kcenon::logger::core
