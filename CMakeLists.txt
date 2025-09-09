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
set(CMAKE_CXX_STANDARD 20)
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
option(USE_LOCKFREE "Use lock-free implementation" ON)
option(USE_THREAD_SYSTEM "Use external thread_system if available" ON)
option(LOGGER_STANDALONE "Build in standalone mode without thread_system" OFF)
option(NO_VCPKG "Skip vcpkg and use system libraries only" OFF)

##################################################
# Include Custom CMake Modules
##################################################

# Add custom cmake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include feature configuration module
include(LoggerFeatures)

# Include dependency detection module
include(LoggerDependencies)

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

# Skip vcpkg if NO_VCPKG option is set or if LOGGER_STANDALONE is ON
if(NO_VCPKG OR LOGGER_STANDALONE)
    message(STATUS "Skipping vcpkg - using system libraries only")
    set(SKIP_VCPKG ON)
endif()

# Try to find thread_system (only if not in standalone mode)
if(USE_THREAD_SYSTEM AND NOT LOGGER_STANDALONE)
    # First try to find it as a sibling directory
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../thread_system/CMakeLists.txt")
        message(STATUS "Found thread_system as sibling directory")
        # Build thread_system as submodule to avoid vcpkg issues
        set(BUILD_THREADSYSTEM_AS_SUBMODULE ON CACHE BOOL "Build ThreadSystem as submodule" FORCE)
        add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../thread_system" "${CMAKE_BINARY_DIR}/thread_system")
        set(THREAD_SYSTEM_FOUND TRUE)
    else()
        # Try to find it as an installed package
        find_package(thread_system CONFIG QUIET)
        if(thread_system_FOUND)
            message(STATUS "Found thread_system package")
            set(THREAD_SYSTEM_FOUND TRUE)
        endif()
    endif()
    
    if(THREAD_SYSTEM_FOUND)
        message(STATUS "Using thread_system for interfaces and utilities")
        add_compile_definitions(USE_THREAD_SYSTEM)
        
        # Create a unified thread_system target that includes all components
        if(NOT TARGET thread_system)
            add_library(thread_system INTERFACE)
            target_link_libraries(thread_system INTERFACE 
                utilities 
                interfaces 
                thread_base 
                thread_pool 
                typed_thread_pool 
                lockfree
            )
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
    # Try to find GTest from vcpkg first
    find_package(GTest CONFIG)
    if(NOT GTest_FOUND)
        # Fallback to FetchContent if not found
        include(FetchContent)
        
        # Disable pthread requirement on Windows
        if(WIN32)
            set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
            set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
        endif()
        
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
        )
        FetchContent_MakeAvailable(googletest)
    endif()
    enable_testing()
endif()

# Google Benchmark (for benchmarks)
if (BUILD_BENCHMARKS)
    # Try to find benchmark from vcpkg first
    find_package(benchmark CONFIG)
    if(NOT benchmark_FOUND)
        # Fallback to FetchContent if not found
        include(FetchContent)
        FetchContent_Declare(
            benchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG v1.8.3
        )
        set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(benchmark)
    endif()
endif()

##################################################
# Source files
##################################################
add_subdirectory(sources)

if (BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

if (BUILD_TESTS)
    add_subdirectory(unittest)
    add_subdirectory(tests)
endif()

if (BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

##################################################
# Installation
##################################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Installation targets list
set(INSTALL_TARGETS logger)
if(TARGET thread_system)
    list(APPEND INSTALL_TARGETS thread_system)
endif()

install(TARGETS ${INSTALL_TARGETS}
    EXPORT LoggerSystemTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY sources/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/logger_system
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(EXPORT LoggerSystemTargets
    FILE LoggerSystemTargets.cmake
    NAMESPACE LoggerSystem::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LoggerSystem
)

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

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerSystemConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/LoggerSystemConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LoggerSystem
)
