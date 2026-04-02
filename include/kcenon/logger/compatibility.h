// BSD 3-Clause License
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

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
