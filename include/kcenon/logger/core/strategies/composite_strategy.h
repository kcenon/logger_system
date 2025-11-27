#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

/**
 * @file composite_strategy.h
 * @brief Composite configuration strategy for combining multiple strategies
 * @since 2.0.0
 *
 * @details Allows combining multiple configuration strategies into a single
 * strategy. Strategies are applied in priority order (highest first).
 */

#include "config_strategy_interface.h"
#include "../logger_config.h"
#include <vector>
#include <algorithm>

namespace kcenon::logger {

/**
 * @class composite_strategy
 * @brief Combines multiple configuration strategies
 *
 * @details Stores multiple strategies and applies them in priority order.
 * Strategies with higher priority values are applied first.
 *
 * @since 2.0.0
 */
class composite_strategy : public config_strategy_interface {
public:
    std::string get_name() const override {
        return "composite";
    }

    void apply(logger_config& config) const override {
        // Create sorted copy of strategies by priority (descending)
        std::vector<const config_strategy_interface*> sorted;
        sorted.reserve(strategies_.size());
        for (const auto& strategy : strategies_) {
            sorted.push_back(strategy.get());
        }
        std::sort(sorted.begin(), sorted.end(),
            [](const auto* a, const auto* b) {
                return a->priority() > b->priority();
            });

        // Apply each applicable strategy in order
        for (const auto* strategy : sorted) {
            if (strategy->is_applicable()) {
                strategy->apply(config);
            }
        }
    }

    bool is_applicable() const override {
        // Applicable if any contained strategy is applicable
        return std::any_of(strategies_.begin(), strategies_.end(),
            [](const auto& strategy) {
                return strategy->is_applicable();
            });
    }

    /**
     * @brief Add a strategy to the composite
     * @param strategy The strategy to add
     * @return Reference to this for chaining
     */
    composite_strategy& add(std::unique_ptr<config_strategy_interface> strategy) {
        if (strategy) {
            strategies_.push_back(std::move(strategy));
        }
        return *this;
    }

    /**
     * @brief Add a strategy by constructing it in-place
     * @tparam Strategy The strategy type to construct
     * @tparam Args Constructor argument types
     * @param args Constructor arguments
     * @return Reference to this for chaining
     */
    template<typename Strategy, typename... Args>
    composite_strategy& add(Args&&... args) {
        strategies_.push_back(
            std::make_unique<Strategy>(std::forward<Args>(args)...)
        );
        return *this;
    }

    /**
     * @brief Clear all strategies
     * @return Reference to this for chaining
     */
    composite_strategy& clear() {
        strategies_.clear();
        return *this;
    }

    /**
     * @brief Get the number of strategies
     * @return Number of contained strategies
     */
    std::size_t size() const {
        return strategies_.size();
    }

    /**
     * @brief Check if composite is empty
     * @return true if no strategies are contained
     */
    bool empty() const {
        return strategies_.empty();
    }

private:
    std::vector<std::unique_ptr<config_strategy_interface>> strategies_;
};

} // namespace kcenon::logger
