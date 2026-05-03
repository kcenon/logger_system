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

# Optional thread_system + spdlog (resolution helpers in dependencies.cmake)
logger_resolve_thread_system()
logger_resolve_spdlog()

# Test/benchmark dependencies are resolved in dependencies.cmake.
# enable_testing() and add_subdirectory(tests) move to testing.cmake.

##################################################
# Library target (explicit source list, no file(GLOB))
##################################################
include(targets)

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
