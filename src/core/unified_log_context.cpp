/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

#include "kcenon/logger/core/unified_log_context.h"

#include <algorithm>

namespace kcenon::logger {

unified_log_context::unified_log_context(const unified_log_context& other) {
    std::shared_lock lock(other.mutex_);
    data_ = other.data_;
}

unified_log_context::unified_log_context(unified_log_context&& other) noexcept {
    std::unique_lock lock(other.mutex_);
    data_ = std::move(other.data_);
}

unified_log_context& unified_log_context::operator=(const unified_log_context& other) {
    if (this != &other) {
        std::scoped_lock lock(mutex_, other.mutex_);
        data_ = other.data_;
    }
    return *this;
}

unified_log_context& unified_log_context::operator=(unified_log_context&& other) noexcept {
    if (this != &other) {
        std::scoped_lock lock(mutex_, other.mutex_);
        data_ = std::move(other.data_);
    }
    return *this;
}

// =========================================================================
// Setters
// =========================================================================

unified_log_context& unified_log_context::set(std::string_view key,
                                              context_value value,
                                              context_category category) {
    std::unique_lock lock(mutex_);
    data_[std::string(key)] = entry{std::move(value), category};
    return *this;
}

unified_log_context& unified_log_context::set_trace(
    std::string_view trace_id,
    std::string_view span_id,
    std::optional<std::string_view> parent_span_id) {
    std::unique_lock lock(mutex_);
    data_["trace_id"] = entry{std::string(trace_id), context_category::trace};
    data_["span_id"] = entry{std::string(span_id), context_category::trace};
    if (parent_span_id) {
        data_["parent_span_id"] = entry{std::string(*parent_span_id), context_category::trace};
    }
    return *this;
}

unified_log_context& unified_log_context::set_request(
    std::string_view request_id,
    std::optional<std::string_view> correlation_id) {
    std::unique_lock lock(mutex_);
    data_["request_id"] = entry{std::string(request_id), context_category::request};
    if (correlation_id) {
        data_["correlation_id"] = entry{std::string(*correlation_id), context_category::request};
    }
    return *this;
}

unified_log_context& unified_log_context::set_otel(const otlp::otel_context& ctx) {
    std::unique_lock lock(mutex_);
    if (!ctx.trace_id.empty()) {
        data_["otel_trace_id"] = entry{ctx.trace_id, context_category::otel};
    }
    if (!ctx.span_id.empty()) {
        data_["otel_span_id"] = entry{ctx.span_id, context_category::otel};
    }
    if (!ctx.trace_flags.empty()) {
        data_["otel_trace_flags"] = entry{ctx.trace_flags, context_category::otel};
    }
    if (!ctx.trace_state.empty()) {
        data_["otel_trace_state"] = entry{ctx.trace_state, context_category::otel};
    }
    return *this;
}

// =========================================================================
// Getters
// =========================================================================

std::optional<context_value> unified_log_context::get(std::string_view key) const {
    std::shared_lock lock(mutex_);
    auto it = data_.find(std::string(key));
    if (it != data_.end()) {
        return it->second.value;
    }
    return std::nullopt;
}

std::string unified_log_context::get_string(std::string_view key,
                                            std::string_view default_value) const {
    auto value = get(key);
    if (!value) {
        return std::string(default_value);
    }
    if (auto* str = std::get_if<std::string>(&*value)) {
        return *str;
    }
    return std::string(default_value);
}

std::optional<context_category> unified_log_context::get_category(std::string_view key) const {
    std::shared_lock lock(mutex_);
    auto it = data_.find(std::string(key));
    if (it != data_.end()) {
        return it->second.category;
    }
    return std::nullopt;
}

// =========================================================================
// Query methods
// =========================================================================

bool unified_log_context::has(std::string_view key) const {
    std::shared_lock lock(mutex_);
    return data_.find(std::string(key)) != data_.end();
}

bool unified_log_context::empty() const {
    std::shared_lock lock(mutex_);
    return data_.empty();
}

size_t unified_log_context::size() const {
    std::shared_lock lock(mutex_);
    return data_.size();
}

std::vector<std::string> unified_log_context::keys() const {
    std::shared_lock lock(mutex_);
    std::vector<std::string> result;
    result.reserve(data_.size());
    for (const auto& [key, _] : data_) {
        result.push_back(key);
    }
    return result;
}

std::vector<std::string> unified_log_context::keys(context_category category) const {
    std::shared_lock lock(mutex_);
    std::vector<std::string> result;
    for (const auto& [key, entry] : data_) {
        if (entry.category == category) {
            result.push_back(key);
        }
    }
    return result;
}

// =========================================================================
// Removal
// =========================================================================

void unified_log_context::remove(std::string_view key) {
    std::unique_lock lock(mutex_);
    data_.erase(std::string(key));
}

void unified_log_context::clear() {
    std::unique_lock lock(mutex_);
    data_.clear();
}

void unified_log_context::clear(context_category category) {
    std::unique_lock lock(mutex_);
    for (auto it = data_.begin(); it != data_.end();) {
        if (it->second.category == category) {
            it = data_.erase(it);
        } else {
            ++it;
        }
    }
}

// =========================================================================
// Export
// =========================================================================

log_fields unified_log_context::to_fields() const {
    std::shared_lock lock(mutex_);
    log_fields result;
    for (const auto& [key, entry] : data_) {
        std::visit(
            [&result, &key](const auto& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, std::monostate>) {
                    // Skip null values
                } else if constexpr (std::is_same_v<T, bool>) {
                    result[key] = val;
                } else if constexpr (std::is_same_v<T, int64_t>) {
                    result[key] = val;
                } else if constexpr (std::is_same_v<T, double>) {
                    result[key] = val;
                } else if constexpr (std::is_same_v<T, std::string>) {
                    result[key] = val;
                }
            },
            entry.value);
    }
    return result;
}

unified_log_context& unified_log_context::merge(const unified_log_context& other, bool overwrite) {
    std::scoped_lock lock(mutex_, other.mutex_);
    for (const auto& [key, entry] : other.data_) {
        if (overwrite || data_.find(key) == data_.end()) {
            data_[key] = entry;
        }
    }
    return *this;
}

}  // namespace kcenon::logger
