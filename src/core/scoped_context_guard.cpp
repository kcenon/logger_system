/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

#include "kcenon/logger/core/scoped_context_guard.h"
#include "kcenon/logger/core/logger.h"

#include <algorithm>

namespace kcenon::logger {

scoped_context_guard::scoped_context_guard(logger& log)
    : logger_(&log), saved_context_(log.context()) {
    // Save current context state for restoration
}

scoped_context_guard::scoped_context_guard(logger& log,
                                          std::string_view key,
                                          context_value value,
                                          context_category category)
    : scoped_context_guard(log) {
    set(key, value, category);
}

scoped_context_guard::~scoped_context_guard() {
    if (!logger_) {
        return; // Moved-from state
    }

    // Get current context
    auto& ctx = logger_->context();

    // Remove all tracked keys from current context
    for (const auto& key : tracked_keys_) {
        ctx.remove(key);
    }

    // Restore values from saved context
    for (const auto& key : saved_context_.keys()) {
        auto value = saved_context_.get(key);
        auto category = saved_context_.get_category(key);
        if (value && category) {
            ctx.set(key, *value, *category);
        }
    }
}

scoped_context_guard::scoped_context_guard(scoped_context_guard&& other) noexcept
    : logger_(other.logger_),
      saved_context_(std::move(other.saved_context_)),
      tracked_keys_(std::move(other.tracked_keys_)) {
    other.logger_ = nullptr; // Mark other as moved-from
}

scoped_context_guard& scoped_context_guard::operator=(scoped_context_guard&& other) noexcept {
    if (this != &other) {
        // First, restore our current state (like destructor)
        if (logger_) {
            auto& ctx = logger_->context();
            for (const auto& key : tracked_keys_) {
                ctx.remove(key);
            }
            for (const auto& key : saved_context_.keys()) {
                auto value = saved_context_.get(key);
                auto category = saved_context_.get_category(key);
                if (value && category) {
                    ctx.set(key, *value, *category);
                }
            }
        }

        // Transfer ownership from other
        logger_ = other.logger_;
        saved_context_ = std::move(other.saved_context_);
        tracked_keys_ = std::move(other.tracked_keys_);
        other.logger_ = nullptr;
    }
    return *this;
}

scoped_context_guard& scoped_context_guard::set(std::string_view key,
                                                context_value value,
                                                context_category category) {
    if (logger_) {
        logger_->context().set(key, value, category);
        track_key(key);
    }
    return *this;
}

scoped_context_guard& scoped_context_guard::set_trace(
    std::string_view trace_id,
    std::string_view span_id,
    std::optional<std::string_view> parent_span_id) {
    if (logger_) {
        logger_->context().set_trace(trace_id, span_id, parent_span_id);
        track_key("trace_id");
        track_key("span_id");
        if (parent_span_id) {
            track_key("parent_span_id");
        }
    }
    return *this;
}

scoped_context_guard& scoped_context_guard::set_request(
    std::string_view request_id,
    std::optional<std::string_view> correlation_id) {
    if (logger_) {
        logger_->context().set_request(request_id, correlation_id);
        track_key("request_id");
        if (correlation_id) {
            track_key("correlation_id");
        }
    }
    return *this;
}

scoped_context_guard& scoped_context_guard::set_otel(const otlp::otel_context& ctx) {
    if (logger_) {
        logger_->context().set_otel(ctx);
        // Track all OTEL context keys
        if (!ctx.trace_id.empty()) {
            track_key("otel_trace_id");
        }
        if (!ctx.span_id.empty()) {
            track_key("otel_span_id");
        }
        if (!ctx.trace_flags.empty()) {
            track_key("otel_trace_flags");
        }
        if (!ctx.trace_state.empty()) {
            track_key("otel_trace_state");
        }
    }
    return *this;
}

void scoped_context_guard::track_key(std::string_view key) {
    std::string key_str(key);
    // Only add if not already tracked (avoid duplicates)
    if (std::find(tracked_keys_.begin(), tracked_keys_.end(), key_str) == tracked_keys_.end()) {
        tracked_keys_.push_back(std::move(key_str));
    }
}

} // namespace kcenon::logger
