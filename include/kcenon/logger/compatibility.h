/**
 * @file compatibility.h
 * @brief Compatibility layer for legacy namespace aliases
 *
 * Provides namespace aliases to maintain backward compatibility
 * with code using the old logger_system namespace.
 */

#pragma once

// Forward declare legacy namespace so that aliasing works without including
// heavier logger headers.
namespace logger_system {}

// Primary alias: expose legacy logger_system as kcenon::logger.
namespace kcenon {
namespace logger = ::logger_system;
} // namespace kcenon

// Additional legacy aliases kept for downstream projects.
namespace logger_module = ::logger_system;
