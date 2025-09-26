#ifndef KCENON_LOGGER_COMPATIBILITY_H
#define KCENON_LOGGER_COMPATIBILITY_H

// Forward declare legacy namespace so that aliasing works without including
// heavier logger headers.
namespace logger_system {}

// Primary alias: expose legacy logger_system as kcenon::logger.
namespace kcenon {
namespace logger = ::logger_system;
} // namespace kcenon

// Additional legacy aliases kept for downstream projects.
namespace logger_module = ::logger_system;

#endif // KCENON_LOGGER_COMPATIBILITY_H
