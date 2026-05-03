cmake_minimum_required(VERSION 3.20)

# C++20 Modules require CMake 3.28+
# Check if we can enable module support
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.28")
    set(LOGGER_CAN_USE_MODULES ON)
else()
    set(LOGGER_CAN_USE_MODULES OFF)
endif()

##################################################
# Project configuration
##################################################
project(
    logger_system
    VERSION 1.0.0
    DESCRIPTION "High-performance C++20 logging system with asynchronous batching"
    HOMEPAGE_URL "https://github.com/kcenon/logger_system"
    LANGUAGES CXX
)

##################################################
# Build options, feature flags, performance tuning
##################################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(options)

##################################################
# C++ standard, compiler checks, sanitizers
##################################################
include(compiler)

##################################################
# Dependencies (find_package + FetchContent)
##################################################
include(dependencies)

##################################################
# Warnings + coverage
##################################################
include(warnings)
include(logger_coverage)

# Perform compiler version check
check_compiler_version()

# Configure platform-specific settings
configure_platform_settings()

# Configure build types
configure_build_types()

# Check required headers
check_required_headers()

# Check C++ features
check_cpp_features()

##################################################
# Output directories
##################################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

##################################################
# Dependency detection + bidirectional guard (Issue #252)
##################################################

# Use our dependency detection module
logger_find_all_dependencies()

# Print feature configuration summary
logger_print_features()

# Skip vcpkg if NO_VCPKG option is set
if(NO_VCPKG)
    message(STATUS "Skipping vcpkg - using system libraries only")
    set(SKIP_VCPKG ON)
endif()

# Perform dependency guard check (declared in dependencies.cmake)
check_bidirectional_dependency_guard()

if(LOGGER_USE_THREAD_SYSTEM)
    unified_find_dependency(thread_system QUIET)
    if(thread_system_FOUND)
        set(THREAD_SYSTEM_FOUND TRUE)
        message(STATUS "Logger System: thread_system integration enabled")
        message(STATUS "  Direction: logger_system → thread_system (using thread_pool for async I/O)")
        # LOGGER_HAS_THREAD_SYSTEM is set per-target below (in thread_system linking section)
    else()
        message(STATUS "Logger System: thread_system not found, using standalone mode")
        set(THREAD_SYSTEM_FOUND FALSE)
    endif()
else()
    message(STATUS "Logger System: Using standalone mode (thread_system integration disabled)")
    set(THREAD_SYSTEM_FOUND FALSE)
endif()

# spdlog (optional for comparison)
find_package(spdlog CONFIG)
if(spdlog_FOUND)
    message(STATUS "Found spdlog library")
endif()

# Google Test (for unit tests)
if (BUILD_TESTS)
    # Test dependencies are handled by LoggerDependencies module
    enable_testing()
endif()

# Google Benchmark (for benchmarks)
# Benchmark dependencies are handled by LoggerDependencies module
# Note: MinGW benchmark disable is handled in the options section (line 58-61)

##################################################
# Source files
##################################################

# Check for new structure
set(LOGGER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LOGGER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(EXISTS ${LOGGER_INCLUDE_DIR}/kcenon/logger AND EXISTS ${LOGGER_SOURCE_DIR})
    message(STATUS "Logger System: Using new directory structure")

    # Add include directories
    include_directories(${LOGGER_INCLUDE_DIR})

    # Collect source files from new structure
    file(GLOB_RECURSE LOGGER_HEADERS
        ${LOGGER_INCLUDE_DIR}/kcenon/logger/*.h
    )

    file(GLOB_RECURSE LOGGER_SOURCES
        ${LOGGER_SOURCE_DIR}/*.cpp
    )

    ##################################################
    # Optional Module Configuration (Issue #357: SRP)
    ##################################################

    # Filter out server module if disabled
    if(NOT LOGGER_WITH_SERVER)
        message(STATUS "Logger System: Server module disabled (LOGGER_WITH_SERVER=OFF)")
        list(FILTER LOGGER_HEADERS EXCLUDE REGEX ".*/server/.*")
        # Note: server is header-only, no cpp files to exclude
    else()
        message(STATUS "Logger System: Server module enabled")
    endif()

    # Filter out analysis module if disabled
    if(NOT LOGGER_WITH_ANALYSIS)
        message(STATUS "Logger System: Analysis module disabled (LOGGER_WITH_ANALYSIS=OFF)")
        list(FILTER LOGGER_HEADERS EXCLUDE REGEX ".*/analysis/.*")
        # Note: analysis is header-only in include/, but has cppm in modules/
    else()
        message(STATUS "Logger System: Analysis module enabled")
    endif()

    # Check if we have enough sources in new structure
    list(LENGTH LOGGER_SOURCES SOURCE_COUNT)
    if(SOURCE_COUNT GREATER 5)
        # Create library with new structure
        add_library(logger_system ${LOGGER_SOURCES} ${LOGGER_HEADERS})
        set_target_properties(logger_system PROPERTIES EXPORT_NAME logger)
        target_include_directories(logger_system
            PUBLIC
                $<BUILD_INTERFACE:${LOGGER_INCLUDE_DIR}>
                $<INSTALL_INTERFACE:include>
        )

        # DLL export support
        if(BUILD_SHARED_LIBS)
            target_compile_definitions(logger_system PRIVATE LOGGER_SYSTEM_BUILDING)
        else()
            target_compile_definitions(logger_system PUBLIC LOGGER_SYSTEM_STATIC)
        endif()

        # Optional module compile definitions (Issue #357: SRP)
        if(LOGGER_WITH_SERVER)
            target_compile_definitions(logger_system PUBLIC LOGGER_WITH_SERVER=1)
        endif()

        if(LOGGER_WITH_ANALYSIS)
            target_compile_definitions(logger_system PUBLIC LOGGER_WITH_ANALYSIS=1)
        endif()

        # Suppress warnings inherited from parent project
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(logger_system PRIVATE
                -Wno-sign-conversion
                -Wno-conversion
                -Wno-float-equal
                -Wno-double-promotion
                -Wno-shadow
                -Wno-unused-private-field
            )
        elseif(MSVC)
            target_compile_options(logger_system PRIVATE
                /wd4244  # conversion from 'type1' to 'type2', possible loss of data
                /wd4267  # conversion from 'size_t' to 'type', possible loss of data
                /wd4305  # truncation from 'type1' to 'type2'
                /wd4456  # declaration of 'identifier' hides previous local declaration
                /wd4457  # declaration of 'identifier' hides function parameter
                /wd4458  # declaration of 'identifier' hides class member
            )
        endif()

        # common_system integration (always linked if available)
        if(common_system_FOUND)
            message(STATUS "Logger System: common_system found, linking for Result<T> compatibility")

            # Add common_system include path
            if(TARGET kcenon::common_system)
                target_link_libraries(logger_system PUBLIC kcenon::common_system)
            else()
                # Use the found include directory
                target_include_directories(logger_system
                    PUBLIC
                        $<BUILD_INTERFACE:${COMMON_SYSTEM_INCLUDE_DIR}>
                )
            endif()
        else()
            message(STATUS "Logger System: common_system not found, using standalone mode")
        endif()

        # thread_system integration (optional, for advanced features)
        if(THREAD_SYSTEM_FOUND)
            message(STATUS "Logger System: thread_system found, optional backend available")

            # Use the resolved target from UnifiedDependencies
            if(thread_system_TARGET AND TARGET ${thread_system_TARGET})
                get_target_property(_ts_imported ${thread_system_TARGET} IMPORTED)
                if(_ts_imported)
                    # IMPORTED target (vcpkg / find_package): safe to export
                    target_link_libraries(logger_system PUBLIC ${thread_system_TARGET})
                    target_compile_definitions(logger_system PUBLIC LOGGER_HAS_THREAD_SYSTEM)
                else()
                    # Local (submodule) target: cannot be in our export set
                    target_link_libraries(logger_system PRIVATE ${thread_system_TARGET})
                    target_compile_definitions(logger_system PRIVATE LOGGER_HAS_THREAD_SYSTEM)
                    set(_LOGGER_HAS_LOCAL_THREAD_SYSTEM TRUE)
                endif()
            elseif(TARGET ThreadSystem)
                get_target_property(_ts_imported ThreadSystem IMPORTED)
                target_link_libraries(logger_system PUBLIC ThreadSystem)
                target_compile_definitions(logger_system PUBLIC LOGGER_HAS_THREAD_SYSTEM)
                if(NOT _ts_imported)
                    set(_LOGGER_HAS_LOCAL_THREAD_SYSTEM TRUE)
                endif()
            elseif(TARGET thread_system)
                get_target_property(_ts_imported thread_system IMPORTED)
                target_link_libraries(logger_system PUBLIC thread_system)
                target_compile_definitions(logger_system PUBLIC LOGGER_HAS_THREAD_SYSTEM)
                if(NOT _ts_imported)
                    set(_LOGGER_HAS_LOCAL_THREAD_SYSTEM TRUE)
                endif()
            else()
                # Fallback: header-only access when aggregate targets are unavailable.
                target_include_directories(logger_system
                    PUBLIC
                        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../thread_system/include>
                )
                target_compile_definitions(logger_system PRIVATE LOGGER_HAS_THREAD_SYSTEM)
                set(_LOGGER_HAS_LOCAL_THREAD_SYSTEM TRUE)
            endif()
        else()
            message(STATUS "Logger System: Using standalone async implementation (no thread_system)")
        endif()

        # Link OpenSSL if encryption is enabled
        if(LOGGER_USE_ENCRYPTION)
            find_package(OpenSSL QUIET)
            if(OpenSSL_FOUND)
                target_link_libraries(logger_system PUBLIC OpenSSL::SSL OpenSSL::Crypto)
                target_compile_definitions(logger_system PUBLIC LOGGER_HAS_OPENSSL=1)
                message(STATUS "Logger System: OpenSSL linked for encryption support (${OPENSSL_VERSION})")
            else()
                message(WARNING "Logger System: LOGGER_USE_ENCRYPTION=ON but OpenSSL not found, encryption features disabled")
                # Remove encrypted_writer from build since OpenSSL is not available
                get_target_property(CURRENT_SOURCES logger_system SOURCES)
                list(FILTER CURRENT_SOURCES EXCLUDE REGEX ".*encrypted_writer.*")
                set_target_properties(logger_system PROPERTIES SOURCES "${CURRENT_SOURCES}")
            endif()
        else()
            # If encryption is disabled, remove encrypted_writer from build
            get_target_property(CURRENT_SOURCES logger_system SOURCES)
            list(FILTER CURRENT_SOURCES EXCLUDE REGEX ".*encrypted_writer.*")
            set_target_properties(logger_system PROPERTIES SOURCES "${CURRENT_SOURCES}")
            message(STATUS "Logger System: Encryption support disabled (LOGGER_USE_ENCRYPTION=OFF)")
        endif()

        # Link OpenTelemetry if OTLP is enabled
        if(LOGGER_ENABLE_OTLP)
            find_package(opentelemetry-cpp CONFIG QUIET)
            find_package(protobuf CONFIG QUIET)
            find_package(gRPC CONFIG QUIET)
            if(opentelemetry-cpp_FOUND)
                target_link_libraries(logger_system PUBLIC
                    opentelemetry-cpp::logs
                    opentelemetry-cpp::otlp_http_log_record_exporter
                    opentelemetry-cpp::otlp_grpc_log_record_exporter
                )
                target_compile_definitions(logger_system PUBLIC LOGGER_HAS_OTLP=1)
                message(STATUS "Logger System: OpenTelemetry linked for OTLP support")
            else()
                message(WARNING "Logger System: LOGGER_ENABLE_OTLP=ON but opentelemetry-cpp not found")
                message(WARNING "  Install with: vcpkg install kcenon-logger-system[otlp]")
            endif()
        endif()

        set(USE_LEGACY_SOURCES FALSE)

        ##################################################
        # C++20 Modules support (optional, requires CMake 3.28+)
        ##################################################
        if(LOGGER_USE_MODULES AND LOGGER_CAN_USE_MODULES)
            message(STATUS "Logger System: Configuring C++20 module files")

            # Module source files (core modules always included)
            set(LOGGER_MODULE_FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/src/modules/logger.cppm
                ${CMAKE_CURRENT_SOURCE_DIR}/src/modules/core.cppm
                ${CMAKE_CURRENT_SOURCE_DIR}/src/modules/backends.cppm
            )

            # Conditionally include analysis module (Issue #357: SRP)
            if(LOGGER_WITH_ANALYSIS)
                list(APPEND LOGGER_MODULE_FILES
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/modules/analysis.cppm
                )
            endif()

            # Check if module files exist
            set(MODULES_AVAILABLE TRUE)
            foreach(MODULE_FILE ${LOGGER_MODULE_FILES})
                if(NOT EXISTS ${MODULE_FILE})
                    message(WARNING "Module file not found: ${MODULE_FILE}")
                    set(MODULES_AVAILABLE FALSE)
                endif()
            endforeach()

            if(MODULES_AVAILABLE)
                # Add module sources to the library using FILE_SET
                target_sources(logger_system
                    PUBLIC
                        FILE_SET CXX_MODULES
                        FILES ${LOGGER_MODULE_FILES}
                )

                # Define macro to indicate modules are available
                target_compile_definitions(logger_system PUBLIC LOGGER_HAS_MODULES=1)

                message(STATUS "Logger System: C++20 modules configured successfully")
                message(STATUS "  Module files:")
                foreach(MODULE_FILE ${LOGGER_MODULE_FILES})
                    get_filename_component(MODULE_NAME ${MODULE_FILE} NAME)
                    message(STATUS "    - ${MODULE_NAME}")
                endforeach()
            else()
                message(STATUS "Logger System: Some module files missing, modules disabled")
            endif()
        endif()

    else()
        message(FATAL_ERROR "Logger System: New structure incomplete (src/ has fewer than 6 .cpp files)")
    endif()
else()
    message(FATAL_ERROR "Logger System: Required directories missing: include/kcenon/logger and src/")
endif()

if (BUILD_SAMPLES)
    # Skip examples in CI environment to avoid compilation errors with incomplete features
    if(NOT DEFINED ENV{CI})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples)
            add_subdirectory(examples)
        elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/samples)
            add_subdirectory(samples)
        endif()
    else()
        message(STATUS "Skipping examples/samples in CI environment")
    endif()
endif()

if (BUILD_TESTS)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
        add_subdirectory(tests)
    else()
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/unittest)
            add_subdirectory(unittest)
        endif()
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
            add_subdirectory(tests)
        endif()
    endif()

    # Enable sanitizers for test targets if requested
    logger_enable_sanitizers_for_tests()

    # Enable warnings for all targets
    logger_enable_warnings_for_all()
endif()

if (BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# Integration tests
if (LOGGER_BUILD_INTEGRATION_TESTS AND BUILD_TESTS AND BUILD_WITH_COMMON_SYSTEM)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/integration_tests AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/integration_tests/CMakeLists.txt)
        message(STATUS "Adding integration_tests subdirectory")
        add_subdirectory(integration_tests)
    endif()
elseif (LOGGER_BUILD_INTEGRATION_TESTS AND BUILD_TESTS AND NOT BUILD_WITH_COMMON_SYSTEM)
    message(STATUS "Skipping integration_tests because BUILD_WITH_COMMON_SYSTEM=OFF")
endif()

# Enable coverage for all registered test targets
# IMPORTANT: This must be called AFTER all test subdirectories (tests/, integration_tests/)
# have been processed, so that all test targets are registered for coverage.
if (BUILD_TESTS)
    logger_enable_coverage_for_all()
endif()

##################################################
# Installation
##################################################
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
        # Local thread_system targets cannot be exported — install without EXPORT
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
            "logger_system: install(EXPORT) skipped — non-IMPORTED deps: "
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
# substitutes @BUILD_WITH_COMMON_SYSTEM@ correctly.
set(BUILD_WITH_COMMON_SYSTEM ON CACHE BOOL "Build with common_system integration")

# Ensure all @VAR@ references in the config template resolve to explicit
# values.  The variables originate from option()/set(CACHE) calls in
# logger_features.cmake and this file.  Forwarding them as normal (non-cache)
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
