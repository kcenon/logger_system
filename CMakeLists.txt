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

message(STATUS "Logger System: Using C++20 mode - std::format enabled")
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
# Dependency Management (Phase 5: Modern CMake FetchContent)
##################################################

# Use FetchContent for dependency management (CMake 3.14+)
include(FetchContent)

# common_system integration (required for Phase 2.2+)
message(STATUS "=== Dependency Management (FetchContent) ===")

# Try to find common_system package first (for local development)
find_package(common_system CONFIG QUIET)

if(NOT common_system_FOUND)
        # Check for common_system in multiple locations with comprehensive search
        # Priority order: Environment variable -> CI/CD workspace -> sibling directories
        set(_LOGGER_COMMON_PATHS
            # Environment variable (HIGHEST PRIORITY)
            "$ENV{COMMON_SYSTEM_ROOT}/include"
            # GitHub Actions: various checkout patterns
            "$ENV{GITHUB_WORKSPACE}/common_system/include"                      # Standard GITHUB_WORKSPACE
            "${CMAKE_CURRENT_SOURCE_DIR}/../common_system/include"              # Sibling (sanitizers.yml pattern)
            "${CMAKE_CURRENT_SOURCE_DIR}/../../common_system/include"           # Parent directory
            "${CMAKE_SOURCE_DIR}/../common_system/include"                      # Sibling to source root
        )

        message(STATUS "Searching for common_system in the following locations:")
        foreach(_path ${_LOGGER_COMMON_PATHS})
            message(STATUS "  Checking: ${_path}")
            if(EXISTS "${_path}/kcenon/common/patterns/result.h")
                message(STATUS "  ✓ Found common_system at: ${_path}")
                set(COMMON_SYSTEM_INCLUDE_DIR "${_path}")
                set(common_system_FOUND TRUE)
                break()
            else()
                message(STATUS "  ✗ Not found")
            endif()
        endforeach()

        if(NOT common_system_FOUND)
            # Fallback to FetchContent (Phase 5: Modern dependency management)
            message(STATUS "  ⚠ Local common_system not found, using FetchContent")
            message(STATUS "  Fetching common_system from GitHub...")

            FetchContent_Declare(
                common_system
                GIT_REPOSITORY https://github.com/kcenon/common_system.git
                GIT_TAG        main  # TODO: Pin to specific tag/commit for reproducibility
                GIT_SHALLOW    TRUE  # Faster checkout
                GIT_PROGRESS   TRUE  # Show download progress
            )

            # Make common_system available
            FetchContent_MakeAvailable(common_system)

            # After FetchContent, set the include directory
            if(TARGET common_system OR EXISTS "${common_system_SOURCE_DIR}/include/kcenon/common/patterns/result.h")
                message(STATUS "  ✓ FetchContent: common_system available at ${common_system_SOURCE_DIR}")
                set(COMMON_SYSTEM_INCLUDE_DIR "${common_system_SOURCE_DIR}/include")
                set(common_system_FOUND TRUE)
            else()
                message(FATAL_ERROR "FetchContent failed to provide common_system.\n"
                    "This should not happen. Please check:\n"
                    "  1. Internet connectivity\n"
                    "  2. GitHub repository access\n"
                    "  3. CMake version >= 3.14")
            endif()
        endif()
else()
    message(STATUS "Found common_system package")
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
        # Priority: Environment variable, then sibling directory
        set(_THREAD_SYSTEM_SEARCH_PATHS
            "$ENV{THREAD_SYSTEM_ROOT}"                                   # Environment variable (HIGHEST PRIORITY)
            "${CMAKE_CURRENT_SOURCE_DIR}/../thread_system"               # Sibling directory
        )

        set(_THREAD_SYSTEM_PATH "")
        foreach(_path ${_THREAD_SYSTEM_SEARCH_PATHS})
            if(EXISTS "${_path}/CMakeLists.txt")
                message(STATUS "Found thread_system at: ${_path}")
                set(_THREAD_SYSTEM_PATH "${_path}")
                break()
            endif()
        endforeach()

        if(_THREAD_SYSTEM_PATH)
            message(STATUS "Using thread_system from: ${_THREAD_SYSTEM_PATH}")
            # Build thread_system as submodule to avoid vcpkg issues
            # Force the cache variable to be set before processing subdirectory
            set(BUILD_THREADSYSTEM_AS_SUBMODULE ON CACHE BOOL "Build ThreadSystem as submodule" FORCE)
            set(BUILD_TESTS OFF CACHE BOOL "Disable tests for thread_system" FORCE)
            set(BUILD_SAMPLES OFF CACHE BOOL "Disable samples for thread_system" FORCE)
            # Set THREAD_SYSTEM_ROOT before adding subdirectory for utilities to find main include
            set(THREAD_SYSTEM_ROOT "${_THREAD_SYSTEM_PATH}" CACHE PATH "Thread system root directory" FORCE)
            add_subdirectory("${_THREAD_SYSTEM_PATH}" "${CMAKE_BINARY_DIR}/thread_system_build")

            # Fix utilities include path for CI environment
            if(TARGET utilities)
                target_include_directories(utilities PUBLIC
                    $<BUILD_INTERFACE:${_THREAD_SYSTEM_PATH}/include>
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

        # thread_system integration (always linked if available)
        if(THREAD_SYSTEM_FOUND)
            message(STATUS "Logger System: thread_system found, backend available for use")
            target_include_directories(LoggerSystem
                PUBLIC
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../thread_system/include>
            )
            target_link_libraries(LoggerSystem PRIVATE thread_base interfaces utilities)
        else()
            message(STATUS "Logger System: thread_system not found, only standalone backend available")
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
