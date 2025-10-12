cmake_minimum_required(VERSION 3.16)

##################################################
# Project configuration
##################################################
project(
    LoggerSystem
    VERSION 1.0.0
    DESCRIPTION "High-performance C++20 logging system with asynchronous batching"
    HOMEPAGE_URL "https://github.com/kcenon/logger_system"
    LANGUAGES CXX
)

##################################################
# C++ standard requirements
##################################################

# C++20 is required (unified across all systems)
set(CMAKE_CXX_STANDARD 20)
message(STATUS "Logger System: Using C++20 mode - Enhanced features enabled")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generate compile_commands.json for tooling

# Output compiler information for debugging purposes
message(STATUS "========================================")
message(STATUS "Logger System - Compiler Configuration:")
message(STATUS "  ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Path: ${CMAKE_CXX_COMPILER}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "========================================")

##################################################
# Global options
##################################################
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(BUILD_SAMPLES "Build sample programs" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(LOGGER_BUILD_INTEGRATION_TESTS "Build integration tests" ON)

# Map LOGGER_BUILD_* to BUILD_* for compatibility with CI/CD
if(DEFINED LOGGER_BUILD_SAMPLES)
    set(BUILD_SAMPLES ${LOGGER_BUILD_SAMPLES} CACHE BOOL "Build sample programs" FORCE)
endif()
if(DEFINED LOGGER_BUILD_TESTS)
    set(BUILD_TESTS ${LOGGER_BUILD_TESTS} CACHE BOOL "Build unit tests" FORCE)
endif()
option(USE_LOCKFREE "Use lock-free implementation" ON)
option(USE_THREAD_SYSTEM "Use external thread_system if available" ON)
option(LOGGER_STANDALONE "Build in standalone mode without thread_system" OFF)
option(NO_VCPKG "Skip vcpkg and use system libraries only" OFF)
option(BUILD_WITH_COMMON_SYSTEM "Build with common_system integration (optional)" ON)

# LOGGER_STANDALONE_MODE takes precedence - force disable common_system and thread_system
if(LOGGER_STANDALONE_MODE)
    message(STATUS "LOGGER_STANDALONE_MODE is ON - forcing BUILD_WITH_COMMON_SYSTEM=OFF and USE_THREAD_SYSTEM=OFF")
    set(BUILD_WITH_COMMON_SYSTEM OFF CACHE BOOL "Disabled by LOGGER_STANDALONE_MODE" FORCE)
    set(USE_THREAD_SYSTEM OFF CACHE BOOL "Disabled by LOGGER_STANDALONE_MODE" FORCE)
endif()

# Disable tests and benchmarks for MinGW builds due to compatibility issues
if(MINGW)
    if(BUILD_TESTS)
        message(STATUS "Disabling tests for MinGW build due to compatibility issues")
        set(BUILD_TESTS OFF CACHE BOOL "Tests disabled for MinGW" FORCE)
    endif()
    if(BUILD_BENCHMARKS)
        message(STATUS "Disabling benchmarks for MinGW build due to compatibility issues")
        set(BUILD_BENCHMARKS OFF CACHE BOOL "Benchmarks disabled for MinGW" FORCE)
    endif()
endif()

# common_system integration (required for Phase 2.2+)
if(BUILD_WITH_COMMON_SYSTEM)
    # Try to find common_system package first
    find_package(common_system CONFIG QUIET)

    if(NOT common_system_FOUND)
        # Check for common_system in multiple locations
        # Priority order: CI/CD workspace -> sibling directory -> parent directory
        set(_LOGGER_COMMON_PATHS
            "${CMAKE_CURRENT_SOURCE_DIR}/common_system/include"          # CI/CD: workspace/common_system
            "$ENV{GITHUB_WORKSPACE}/common_system/include"               # CI/CD: explicit GITHUB_WORKSPACE
            "${CMAKE_CURRENT_SOURCE_DIR}/../common_system/include"       # Local: sibling directory
            "${CMAKE_CURRENT_SOURCE_DIR}/../../common_system/include"    # Local: parent directory
        )

        foreach(_path ${_LOGGER_COMMON_PATHS})
            if(EXISTS "${_path}/kcenon/common/patterns/result.h")
                message(STATUS "Found common_system at: ${_path}")
                set(COMMON_SYSTEM_INCLUDE_DIR "${_path}")
                set(common_system_FOUND TRUE)
                break()
            endif()
        endforeach()

        if(NOT common_system_FOUND)
            message(FATAL_ERROR "common_system is required but was not found.\n"
                "Please ensure common_system is available in one of the following locations:\n"
                "  - ${CMAKE_CURRENT_SOURCE_DIR}/../common_system\n"
                "  - ${CMAKE_CURRENT_SOURCE_DIR}/../../common_system\n"
                "  - ${CMAKE_CURRENT_SOURCE_DIR}/common_system\n"
                "  - $ENV{GITHUB_WORKSPACE}/common_system\n"
                "Or set BUILD_WITH_COMMON_SYSTEM=OFF to build in standalone mode (not recommended for Phase 2.2+).")
        endif()
    else()
        message(STATUS "Found common_system package")
    endif()
else()
    message(STATUS "Building without common_system integration (BUILD_WITH_COMMON_SYSTEM=OFF)")
    message(WARNING "Standalone mode is not fully supported in Phase 2.2+")
endif()

##################################################
# Include Custom CMake Modules
##################################################

# Add custom cmake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include feature configuration module
include(LoggerFeatures)

# Include dependency detection module
include(LoggerDependencies)

# Include sanitizer configuration module
include(LoggerSanitizers)

# Include warning configuration module
include(LoggerWarnings)

# Include coverage configuration module
include(LoggerCoverage)

# Include compiler checks module (following thread_system pattern)
include(CompilerChecks)

# Perform compiler version check
check_compiler_version()

# Configure platform-specific settings
configure_platform_settings()

# Configure build types
configure_build_types()

# Check required headers
check_required_headers()

# Check C++20 features
check_cpp_features()

##################################################
# Output directories
##################################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

##################################################
# Dependencies (modular approach)
##################################################

# Use our dependency detection module
logger_find_all_dependencies()

# Print feature configuration summary
logger_print_features()

# Skip vcpkg if NO_VCPKG option is set or if LOGGER_STANDALONE_MODE is ON
if(NO_VCPKG OR LOGGER_STANDALONE_MODE)
    message(STATUS "Skipping vcpkg - using system libraries only")
    set(SKIP_VCPKG ON)
endif()

# Try to find thread_system (only if not in standalone mode)
if(USE_THREAD_SYSTEM AND NOT LOGGER_STANDALONE_MODE)
    # Check if thread_system components are already added
    if(NOT TARGET interfaces)
        # First try to find it as a sibling directory
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../thread_system/CMakeLists.txt")
            message(STATUS "Found thread_system as sibling directory")
            # Build thread_system as submodule to avoid vcpkg issues
            # Force the cache variable to be set before processing subdirectory
            set(BUILD_THREADSYSTEM_AS_SUBMODULE ON CACHE BOOL "Build ThreadSystem as submodule" FORCE)
            set(BUILD_TESTS OFF CACHE BOOL "Disable tests for thread_system" FORCE)
            set(BUILD_SAMPLES OFF CACHE BOOL "Disable samples for thread_system" FORCE)
            # Set THREAD_SYSTEM_ROOT before adding subdirectory for utilities to find main include
            set(THREAD_SYSTEM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../thread_system" CACHE PATH "Thread system root directory" FORCE)
            add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../thread_system" "${CMAKE_BINARY_DIR}/thread_system_build")

            # Fix utilities include path for CI environment
            if(TARGET utilities)
                target_include_directories(utilities PUBLIC
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../thread_system/include>
                )
            endif()

            set(THREAD_SYSTEM_FOUND TRUE)
        else()
            # Try to find it as an installed package
            find_package(thread_system CONFIG QUIET)
            if(thread_system_FOUND)
                message(STATUS "Found thread_system package")
                set(THREAD_SYSTEM_FOUND TRUE)
            endif()
        endif()
    else()
        message(STATUS "thread_system components already included")
        set(THREAD_SYSTEM_FOUND TRUE)
    endif()

    if(THREAD_SYSTEM_FOUND)
        message(STATUS "Using thread_system for interfaces and utilities")
        add_compile_definitions(USE_THREAD_SYSTEM)

        # Create a unified thread_system target that includes all components
        if(NOT TARGET thread_system)
            add_library(thread_system INTERFACE)
            if(TARGET utilities AND TARGET interfaces AND TARGET thread_base)
                target_link_libraries(thread_system INTERFACE
                    utilities
                    interfaces
                    thread_base
                    thread_pool
                    typed_thread_pool
                    lockfree
                )
            endif()
        endif()
    else()
        message(STATUS "thread_system not found, using standalone mode")
        add_compile_definitions(LOGGER_STANDALONE)
    endif()
else()
    message(STATUS "Building in standalone mode")
    add_compile_definitions(LOGGER_STANDALONE)
endif()

# fmt library (optional but recommended)
find_package(fmt CONFIG)
if(fmt_FOUND)
    message(STATUS "Found fmt library")
    add_compile_definitions(USE_FMT)
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

    # Check if we have enough sources in new structure
    list(LENGTH LOGGER_SOURCES SOURCE_COUNT)
    if(SOURCE_COUNT GREATER 5)
        # Create library with new structure
        add_library(LoggerSystem ${LOGGER_SOURCES} ${LOGGER_HEADERS})
        target_include_directories(LoggerSystem
            PUBLIC
                $<BUILD_INTERFACE:${LOGGER_INCLUDE_DIR}>
                $<INSTALL_INTERFACE:include>
        )

        # common_system integration (conditional)
        if(BUILD_WITH_COMMON_SYSTEM AND common_system_FOUND)
            message(STATUS "Logger System: Using common_system interfaces")

            # Add common_system include path
            if(TARGET kcenon::common_system)
                target_link_libraries(LoggerSystem PUBLIC kcenon::common_system)
            else()
                # Use the found include directory
                target_include_directories(LoggerSystem
                    PUBLIC
                        $<BUILD_INTERFACE:${COMMON_SYSTEM_INCLUDE_DIR}>
                )
            endif()

            target_compile_definitions(LoggerSystem
                PUBLIC
                    LOGGER_BUILD_WITH_COMMON_SYSTEM
                    BUILD_WITH_COMMON_SYSTEM
                    LOGGER_USING_COMMON_INTERFACES
            )
        else()
            message(STATUS "Logger System: Building without common_system")
            # No common_system compile definitions
        endif()

        # Add thread_system integration if available and not explicitly disabled
        option(LOGGER_STANDALONE_MODE "Build logger_system in standalone mode without thread_system integration" OFF)

        if(THREAD_SYSTEM_FOUND AND NOT LOGGER_STANDALONE_MODE)
            message(STATUS "Logger System: Using thread_system integration mode")
            target_include_directories(LoggerSystem
                PUBLIC
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../thread_system/include>
            )
            target_link_libraries(LoggerSystem PRIVATE thread_base interfaces utilities)
            target_compile_definitions(LoggerSystem PUBLIC USE_THREAD_SYSTEM_INTEGRATION)
        else()
            message(STATUS "Logger System: Using standalone mode")
            target_compile_definitions(LoggerSystem PUBLIC LOGGER_STANDALONE_MODE)
        endif()

        set(USE_LEGACY_SOURCES FALSE)
    else()
        message(STATUS "Logger System: New structure incomplete, using legacy sources")
        set(USE_LEGACY_SOURCES TRUE)
    endif()
else()
    message(STATUS "Logger System: Using legacy directory structure")
    set(USE_LEGACY_SOURCES TRUE)
endif()

# Use legacy sources if needed
if(USE_LEGACY_SOURCES)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sources)
        add_subdirectory(sources)
    endif()
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
    
    # Enable coverage for all targets
    logger_enable_coverage_for_all()
endif()

if (BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# Integration tests
if (LOGGER_BUILD_INTEGRATION_TESTS AND BUILD_TESTS)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/integration_tests AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/integration_tests/CMakeLists.txt)
        message(STATUS "Adding integration_tests subdirectory")
        add_subdirectory(integration_tests)
    endif()
endif()

##################################################
# Installation
##################################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Installation targets list
set(INSTALL_TARGETS)
if(TARGET LoggerSystem)
    list(APPEND INSTALL_TARGETS LoggerSystem)
endif()
# Note: thread_system is an external dependency and should not be installed as part of LoggerSystem

if(INSTALL_TARGETS)
    install(TARGETS ${INSTALL_TARGETS}
        EXPORT LoggerSystemTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

install(DIRECTORY sources/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/logger_system
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Temporarily disabled to avoid export dependency issues
# if(INSTALL_TARGETS)
#     install(EXPORT LoggerSystemTargets
#         FILE LoggerSystemTargets.cmake
#         NAMESPACE LoggerSystem::
#         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LoggerSystem
#     )
# endif()

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/LoggerSystemConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerSystemConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LoggerSystem
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerSystemConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install adapter headers if BUILD_WITH_COMMON_SYSTEM is enabled
if(BUILD_WITH_COMMON_SYSTEM)
    install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/kcenon/logger/adapters/common_logger_adapter.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kcenon/logger/adapters
        COMPONENT Development
    )
endif()

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerSystemConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerSystemConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LoggerSystem
)
