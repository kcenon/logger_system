# dependencies.cmake -- find_package() and FetchContent declarations for Logger System
#
# Inputs:
#   - LOGGER_USE_COMPRESSION / LOGGER_USE_ENCRYPTION / BUILD_TESTS /
#     BUILD_BENCHMARKS / NO_VCPKG: feature gates (declared in options.cmake)
#   - LOGGER_USE_THREAD_SYSTEM: optional integration gate
#   - UNIFIED_USE_LOCAL / UNIFIED_USE_FETCHCONTENT: dependency mode
# Outputs:
#   - common_system: required, resolved via UnifiedDependencies
#   - thread_system: optional, resolved via UnifiedDependencies when
#     LOGGER_USE_THREAD_SYSTEM=ON
#   - COMMON_SYSTEM_INCLUDE_DIR: backward-compat include path
#   - SKIP_VCPKG: set when NO_VCPKG=ON
#   - logger_find_compression() / logger_find_encryption() /
#     logger_find_test_dependencies() / logger_find_benchmark_dependencies() /
#     logger_find_all_dependencies(): functions
#   - check_bidirectional_dependency_guard(): function (Issue #252)

##################################################
# Unified dependency management
##################################################
include(UnifiedDependencies)

# Default common_system integration to ON; UnifiedDependencies overrides on success.
# This ensures testing.cmake's BUILD_WITH_COMMON_SYSTEM gate is well-defined even
# before unified_find_dependency(common_system REQUIRED) runs.
if(NOT DEFINED BUILD_WITH_COMMON_SYSTEM)
    set(BUILD_WITH_COMMON_SYSTEM ON CACHE BOOL "Build with common_system integration")
endif()

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
# Optional compression library (zlib)
##################################################
function(logger_find_compression)
    if(NOT LOGGER_USE_COMPRESSION)
        return()
    endif()

    find_package(ZLIB QUIET)
    if(ZLIB_FOUND)
        message(STATUS "Found ZLIB for compression support: ${ZLIB_VERSION}")
        set(LOGGER_HAS_COMPRESSION TRUE PARENT_SCOPE)
    else()
        message(WARNING "ZLIB not found - compression support disabled")
        set(LOGGER_USE_COMPRESSION OFF PARENT_SCOPE)
        set(LOGGER_HAS_COMPRESSION FALSE PARENT_SCOPE)
    endif()
endfunction()

##################################################
# Optional encryption library (OpenSSL 3.0+)
##################################################
function(logger_find_encryption)
    if(NOT LOGGER_USE_ENCRYPTION)
        return()
    endif()

    # OpenSSL 3.0+ is required (1.1.1 reached EOL September 2023)
    find_package(OpenSSL 3.0 QUIET)
    if(OpenSSL_FOUND)
        message(STATUS "Found OpenSSL for encryption support: ${OPENSSL_VERSION}")
        set(LOGGER_HAS_ENCRYPTION TRUE PARENT_SCOPE)
    else()
        message(WARNING "OpenSSL 3.0+ not found - encryption support disabled. "
                        "OpenSSL 1.1.x is no longer supported (EOL September 2023).")
        set(LOGGER_USE_ENCRYPTION OFF PARENT_SCOPE)
        set(LOGGER_HAS_ENCRYPTION FALSE PARENT_SCOPE)
    endif()
endfunction()

##################################################
# Test dependencies (GTest with FetchContent fallback)
##################################################
function(logger_find_test_dependencies)
    if(NOT BUILD_TESTS)
        return()
    endif()

    # Try to find GTest
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        message(STATUS "GTest not found, attempting to fetch...")
        include(FetchContent)

        # Set options before FetchContent_Declare so they apply during configuration
        set(gtest_force_shared_crt ON CACHE BOOL "Use shared CRT when building GoogleTest" FORCE)
        set(BUILD_GMOCK ON CACHE BOOL "Build gmock" FORCE)

        if(WIN32 OR MSVC OR MINGW)
            set(gtest_disable_pthreads ON CACHE BOOL "Disable pthread usage in GoogleTest" FORCE)
        endif()

        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
        )

        FetchContent_MakeAvailable(googletest)

        if(WIN32 OR MSVC OR MINGW)
            foreach(logger_gtest_target IN ITEMS gtest gtest_main gmock gmock_main)
                if(TARGET ${logger_gtest_target})
                    target_compile_definitions(${logger_gtest_target} PUBLIC GTEST_HAS_PTHREAD=0)
                endif()
            endforeach()
        endif()
    else()
        message(STATUS "Found GTest: ${GTest_DIR}")
    endif()
endfunction()

##################################################
# Benchmark dependencies (Google Benchmark with FetchContent fallback)
##################################################
function(logger_find_benchmark_dependencies)
    if(NOT BUILD_BENCHMARKS)
        return()
    endif()

    find_package(benchmark QUIET)
    if(NOT benchmark_FOUND)
        message(STATUS "Google Benchmark not found, attempting to fetch...")
        include(FetchContent)

        # Disable benchmark tests for all platforms
        set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)

        # Disable features that might require pthread on Windows
        if(WIN32 OR MSVC OR MINGW)
            set(BENCHMARK_ENABLE_LTO OFF CACHE BOOL "Disable LTO" FORCE)
            set(BENCHMARK_USE_LIBCXX OFF CACHE BOOL "Disable libcxx" FORCE)
            set(BENCHMARK_ENABLE_EXCEPTIONS OFF CACHE BOOL "Disable exceptions" FORCE)
            set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable install" FORCE)
        endif()

        FetchContent_Declare(
            googlebenchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG v1.8.3
        )

        # Use the same pattern as GoogleTest for consistency
        FetchContent_GetProperties(googlebenchmark)
        if(NOT googlebenchmark_POPULATED)
            FetchContent_Populate(googlebenchmark)
            add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR} EXCLUDE_FROM_ALL)
        endif()
    else()
        message(STATUS "Found Google Benchmark: ${benchmark_DIR}")
    endif()
endfunction()

##################################################
# Aggregate dependency detection entry point
##################################################
function(logger_find_all_dependencies)
    message(STATUS "========================================")
    message(STATUS "Detecting Logger System Dependencies...")

    # Required dependencies
    find_package(Threads REQUIRED)

    # Optional dependencies (thread_system handled by UnifiedDependencies)
    logger_find_compression()
    logger_find_encryption()
    logger_find_test_dependencies()
    logger_find_benchmark_dependencies()

    message(STATUS "Dependency Detection Complete")
    message(STATUS "========================================")
endfunction()

##################################################
# Bidirectional Dependency Guard (Issue #252 — Resolved)
##################################################
# The bidirectional dependency risk between logger_system and thread_system has been
# resolved. thread_system's BUILD_WITH_LOGGER_SYSTEM is deprecated (thread_system#336)
# and scheduled for removal in v0.5.0.0.
#
# This guard remains as a safety check during the transition period.
# Direction: logger_system -> thread_system (optional, unidirectional)
# Removed:   thread_system -> logger_system (deprecated, uses ILogger DI instead)
#
# See: docs/DEPENDENCY_ARCHITECTURE.md for the resolved dependency graph
# See: docs/integration/THREAD_SYSTEM.md for integration options
function(check_bidirectional_dependency_guard)
    # Check if we're enabling logger -> thread direction
    if(LOGGER_USE_THREAD_SYSTEM)
        # Check if thread_system still has deprecated reverse dependency (thread -> logger)
        if(DEFINED BUILD_WITH_LOGGER_SYSTEM AND BUILD_WITH_LOGGER_SYSTEM)
            message(WARNING
                "=================================================================\n"
                "DEPRECATED CONFIGURATION DETECTED (Issue #252)\n"
                "=================================================================\n"
                "  logger_system -> thread_system (LOGGER_USE_THREAD_SYSTEM=ON)\n"
                "  thread_system -> logger_system (BUILD_WITH_LOGGER_SYSTEM=ON)\n"
                "\n"
                "BUILD_WITH_LOGGER_SYSTEM is deprecated in thread_system (see\n"
                "thread_system#336) and scheduled for removal in v0.5.0.0.\n"
                "thread_system now uses ILogger dependency injection instead.\n"
                "\n"
                "ACTION:\n"
                "  Set BUILD_WITH_LOGGER_SYSTEM=OFF and use ILogger DI approach.\n"
                "  See: docs/DEPENDENCY_ARCHITECTURE.md\n"
                "=================================================================")
        endif()

        # Additional check: Inspect thread_system target for deprecated reverse dependency
        if(TARGET thread_system::thread_system)
            get_target_property(THREAD_DEFS thread_system::thread_system
                               INTERFACE_COMPILE_DEFINITIONS)
            if(THREAD_DEFS)
                string(FIND "${THREAD_DEFS}" "BUILD_WITH_LOGGER_SYSTEM" _bidep_pos)
                if(NOT _bidep_pos EQUAL -1)
                    message(WARNING
                        "Deprecated configuration detected: thread_system target has "
                        "BUILD_WITH_LOGGER_SYSTEM in compile definitions. "
                        "This is deprecated (thread_system#336) and will be removed in v0.5.0.0.")
                endif()
            endif()
        endif()
    endif()
endfunction()
