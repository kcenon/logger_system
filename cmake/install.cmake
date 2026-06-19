# install.cmake -- Install rules and config-package generation
#
# Inputs:
#   - logger_system: target (defined in targets.cmake)
#   - _LOGGER_HAS_LOCAL_THREAD_SYSTEM: set in targets.cmake when thread_system
#     is a non-IMPORTED local subdirectory target
#   - thread_system_TARGET: set by UnifiedDependencies
#   - LOGGER_USE_*: feature variables (declared in options.cmake), forwarded
#     to logger_system-config.cmake.in via configure_package_config_file
#   - PROJECT_VERSION: set by project()
#   - BUILD_WITH_COMMON_SYSTEM: cache bool (set here unconditionally to ON;
#     UnifiedDependencies also sets it during common_system resolution)
# Outputs:
#   - install() rules for logger_system target, headers, config files
#   - export(EXPORT logger_system-targets ...) when no local thread_system
#   - configure_package_config_file() and write_basic_package_version_file()
#     produce CMake config and version files

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Installation targets list
set(INSTALL_TARGETS)
if(TARGET logger_system)
    list(APPEND INSTALL_TARGETS logger_system)
endif()
# Note: thread_system is an external dependency and should not be installed as part of logger_system

if(INSTALL_TARGETS)
    if(_LOGGER_HAS_LOCAL_THREAD_SYSTEM)
        # Local thread_system targets cannot be exported -- install without EXPORT
        install(TARGETS ${INSTALL_TARGETS}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
    else()
        install(TARGETS ${INSTALL_TARGETS}
            EXPORT logger_system-targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

        export(EXPORT logger_system-targets
            FILE "${CMAKE_CURRENT_BINARY_DIR}/logger_system-targets.cmake"
            NAMESPACE logger_system::
        )
    endif()
endif()

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

if(INSTALL_TARGETS)
    if(_LOGGER_HAS_LOCAL_THREAD_SYSTEM)
        # When thread_system is a local (non-IMPORTED) target (e.g., submodule build),
        # its internal sub-targets (thread_base, etc.) are not in any export set.
        # Skip install(EXPORT) to avoid CMake errors. Consumers building from source
        # already have both logger_system and thread_system available.
        set(_LOGGER_NON_IMPORTED_DEPS "")
        foreach(_ts_candidate ${thread_system_TARGET} ThreadSystem thread_system)
            if(TARGET ${_ts_candidate})
                get_target_property(_imp ${_ts_candidate} IMPORTED)
                if(NOT _imp)
                    list(APPEND _LOGGER_NON_IMPORTED_DEPS "${_ts_candidate}")
                endif()
            endif()
        endforeach()
        message(STATUS
            "logger_system: install(EXPORT) skipped -- non-IMPORTED deps: "
            "${_LOGGER_NON_IMPORTED_DEPS}. "
            "Headers and library are installed. "
            "Subdirectory consumers: link to 'logger_system' target directly.")
    else()
        install(EXPORT logger_system-targets
            FILE logger_system-targets.cmake
            NAMESPACE logger_system::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logger_system
        )
    endif()
endif()

# common_system is a mandatory dependency; ensure the config template
# substitutes @BUILD_WITH_COMMON_SYSTEM@ correctly. UnifiedDependencies sets
# this when common_system is resolved; this line is a fallback default.
set(BUILD_WITH_COMMON_SYSTEM ON CACHE BOOL "Build with common_system integration")

# Ensure all @VAR@ references in the config template resolve to explicit
# values.  The variables originate from option()/set(CACHE) calls in
# options.cmake and this file.  Forwarding them as normal (non-cache)
# variables here guarantees configure_package_config_file() never substitutes
# an empty string, regardless of cache evaluation order.
#
# Feature flags
set(LOGGER_USE_DI                  "${LOGGER_USE_DI}")
set(LOGGER_USE_MONITORING          "${LOGGER_USE_MONITORING}")
set(LOGGER_USE_EXTERNAL_DI         "${LOGGER_USE_EXTERNAL_DI}")
set(LOGGER_FORCE_LIGHTWEIGHT       "${LOGGER_FORCE_LIGHTWEIGHT}")
set(LOGGER_ENABLE_CRASH_HANDLER    "${LOGGER_ENABLE_CRASH_HANDLER}")
set(LOGGER_ENABLE_STRUCTURED_LOGGING "${LOGGER_ENABLE_STRUCTURED_LOGGING}")
set(LOGGER_USE_LOCK_FREE_QUEUE     "${LOGGER_USE_LOCK_FREE_QUEUE}")
set(LOGGER_ENABLE_NETWORK_WRITER   "${LOGGER_ENABLE_NETWORK_WRITER}")
set(LOGGER_ENABLE_FILE_ROTATION    "${LOGGER_ENABLE_FILE_ROTATION}")
set(LOGGER_ENABLE_ASYNC            "${LOGGER_ENABLE_ASYNC}")
set(LOGGER_USE_COMPRESSION         "${LOGGER_USE_COMPRESSION}")
set(LOGGER_USE_ENCRYPTION          "${LOGGER_USE_ENCRYPTION}")
set(LOGGER_USE_THREAD_SYSTEM       "${LOGGER_USE_THREAD_SYSTEM}")
# Performance settings
set(LOGGER_DEFAULT_BUFFER_SIZE     "${LOGGER_DEFAULT_BUFFER_SIZE}")
set(LOGGER_DEFAULT_BATCH_SIZE      "${LOGGER_DEFAULT_BATCH_SIZE}")
set(LOGGER_DEFAULT_QUEUE_SIZE      "${LOGGER_DEFAULT_QUEUE_SIZE}")
set(LOGGER_MAX_WRITERS             "${LOGGER_MAX_WRITERS}")

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/logger_system-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/logger_system-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logger_system
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/logger_system-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install adapter headers (common_system is always required)
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kcenon/logger/adapters/common_logger_adapter.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kcenon/logger/adapters
    COMPONENT Development
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/logger_system-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/logger_system-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logger_system
)
