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

# C++20 is REQUIRED (no fallback to earlier standards)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Verify that C++20 is actually being used
if(CMAKE_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "Logger System requires C++20 or later. Current standard: C++${CMAKE_CXX_STANDARD}")
endif()

message(STATUS "Logger System: Using C++20 standard")
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
option(LOGGER_ENABLE_COVERAGE "Enable code coverage reporting" OFF)

# Map LOGGER_BUILD_* to BUILD_* for compatibility with CI/CD
if(DEFINED LOGGER_BUILD_SAMPLES)
    set(BUILD_SAMPLES ${LOGGER_BUILD_SAMPLES} CACHE BOOL "Build sample programs" FORCE)
endif()
if(DEFINED LOGGER_BUILD_TESTS)
    set(BUILD_TESTS ${LOGGER_BUILD_TESTS} CACHE BOOL "Build unit tests" FORCE)
endif()

# Respect global BUILD_INTEGRATION_TESTS flag if set
if(DEFINED BUILD_INTEGRATION_TESTS)
    if(BUILD_INTEGRATION_TESTS)
        set(_LOGGER_BUILD_IT_VALUE ON)
    else()
        set(_LOGGER_BUILD_IT_VALUE OFF)
    endif()
    set(LOGGER_BUILD_INTEGRATION_TESTS ${_LOGGER_BUILD_IT_VALUE} CACHE BOOL "Build integration tests" FORCE)
endif()
option(NO_VCPKG "Skip vcpkg and use system libraries only" OFF)

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

##################################################
# Dependency Management (Unified CMake Module)
##################################################

# Add cmake modules path (required before including UnifiedDependencies)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include unified dependency management module
include(UnifiedDependencies)

# Setup dependency resolution mode
# Supports: UNIFIED_USE_LOCAL, UNIFIED_USE_FETCHCONTENT, find_package with fallback
unified_setup_dependency_mode()

message(STATUS "=== Dependency Management (UnifiedDependencies) ===")

# Find common_system (required dependency)
unified_find_dependency(common_system REQUIRED)

# Set COMMON_SYSTEM_INCLUDE_DIR for backward compatibility
if(common_system_FOUND AND common_system_SOURCE_DIR)
    set(COMMON_SYSTEM_INCLUDE_DIR "${common_system_SOURCE_DIR}/include")
elseif(COMMON_SYSTEM_INCLUDE_DIR)
    # Already set by UnifiedDependencies
else()
    # Try to get from target
    if(TARGET ${common_system_TARGET})
        get_target_property(_cs_inc ${common_system_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
        if(_cs_inc)
            list(GET _cs_inc 0 COMMON_SYSTEM_INCLUDE_DIR)
        endif()
    endif()
endif()

##################################################
# Include Custom CMake Modules
##################################################

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

# Check C++ features
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

# Skip vcpkg if NO_VCPKG option is set
if(NO_VCPKG)
    message(STATUS "Skipping vcpkg - using system libraries only")
    set(SKIP_VCPKG ON)
endif()

# Find thread_system (required dependency)
unified_find_dependency(thread_system REQUIRED)

set(THREAD_SYSTEM_FOUND TRUE)
message(STATUS "Using thread_system for interfaces and utilities")
add_compile_definitions(USE_THREAD_SYSTEM)

# Create a unified thread_system target if needed
if(NOT TARGET thread_system AND NOT TARGET ThreadSystem)
    add_library(thread_system INTERFACE)
    if(TARGET utilities AND TARGET interfaces AND TARGET thread_base)
        target_link_libraries(thread_system INTERFACE
            utilities
            interfaces
            thread_base
        )
    endif()
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

        # Suppress warnings inherited from parent project
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(LoggerSystem PRIVATE
                -Wno-sign-conversion
                -Wno-conversion
                -Wno-float-equal
                -Wno-double-promotion
                -Wno-shadow
                -Wno-unused-private-field
            )
        elseif(MSVC)
            target_compile_options(LoggerSystem PRIVATE
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
                target_link_libraries(LoggerSystem PUBLIC kcenon::common_system)
            else()
                # Use the found include directory
                target_include_directories(LoggerSystem
                    PUBLIC
                        $<BUILD_INTERFACE:${COMMON_SYSTEM_INCLUDE_DIR}>
                )
            endif()
        else()
            message(STATUS "Logger System: common_system not found, using standalone mode")
        endif()

        # thread_system integration (required dependency)
        message(STATUS "Logger System: thread_system found, backend available for use")
        if(TARGET ThreadSystem)
            target_link_libraries(LoggerSystem PRIVATE ThreadSystem)
        elseif(TARGET thread_system)
            target_link_libraries(LoggerSystem PRIVATE thread_system)
        else()
            target_include_directories(LoggerSystem
                PUBLIC
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../thread_system/include>
            )
            target_link_libraries(LoggerSystem PRIVATE thread_base interfaces utilities)
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
if (LOGGER_BUILD_INTEGRATION_TESTS AND BUILD_TESTS AND BUILD_WITH_COMMON_SYSTEM)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/integration_tests AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/integration_tests/CMakeLists.txt)
        message(STATUS "Adding integration_tests subdirectory")
        add_subdirectory(integration_tests)
    endif()
elseif (LOGGER_BUILD_INTEGRATION_TESTS AND BUILD_TESTS AND NOT BUILD_WITH_COMMON_SYSTEM)
    message(STATUS "Skipping integration_tests because BUILD_WITH_COMMON_SYSTEM=OFF")
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
