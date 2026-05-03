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
# Warnings (coverage is handled by testing.cmake)
##################################################
include(warnings)

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

# Test/benchmark dependencies are resolved in dependencies.cmake.
# enable_testing() and add_subdirectory(tests) move to testing.cmake.

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

##################################################
# Examples and tests
##################################################
include(examples)
include(testing)

# Benchmarks (kept in root: depends on FetchBenchmark.cmake helper)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

##################################################
# Installation rules + config-package generation
##################################################
include(install)
