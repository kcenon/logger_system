#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

/**
 * @file config_strategy_interface.h
 * @brief Interface for logger configuration strategies (Strategy Pattern)
 * @since 2.0.0
 *
 * @details Defines the contract for configuration strategies that can be
 * applied to logger_config. Enables dynamic, composable configuration
 * based on environment, performance requirements, or deployment context.
 */

#include <memory>
#include <string>
#include <functional>

namespace kcenon::logger {

// Forward declaration
struct logger_config;

/**
 * @class config_strategy_interface
 * @brief Abstract interface for logger configuration strategies
 *
 * @details Configuration strategies allow dynamic modification of logger
 * settings based on various contexts (environment, performance needs, etc.).
 * Multiple strategies can be composed and applied in priority order.
 *
 * @since 2.0.0
 */
class config_strategy_interface {
public:
    virtual ~config_strategy_interface() = default;

    /**
     * @brief Get the strategy name
     * @return Human-readable strategy name
     */
    virtual std::string get_name() const = 0;

    /**
     * @brief Apply this strategy to a logger configuration
     * @param config The configuration to modify
     *
     * @details Modifies the provided configuration according to the
     * strategy's rules. Changes are applied in-place.
     */
    virtual void apply(logger_config& config) const = 0;

    /**
     * @brief Check if this strategy is applicable in the current context
     * @return true if the strategy should be applied
     *
     * @details Override to implement conditional strategy application
     * (e.g., only apply in certain environments).
     */
    virtual bool is_applicable() const { return true; }

    /**
     * @brief Get the strategy priority
     * @return Priority value (higher = applied first)
     *
     * @details When multiple strategies are applied, they are sorted
     * by priority (descending) before application.
     */
    virtual int priority() const { return 0; }
};

/**
 * @brief Factory function type for creating configuration strategies
 */
using config_strategy_factory_fn = std::function<std::unique_ptr<config_strategy_interface>()>;

} // namespace kcenon::logger
