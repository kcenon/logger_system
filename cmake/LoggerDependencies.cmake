##################################################
# Logger System Dependency Detection Module
#
# This module handles finding and configuring external dependencies.
# Note: thread_system and common_system are now handled by UnifiedDependencies.cmake
# in the main CMakeLists.txt for consistent dependency resolution.
##################################################

# Function to find optional compression libraries
function(logger_find_compression)
    if(NOT LOGGER_USE_COMPRESSION)
        return()
    endif()
    
    # Try to find zlib for compression
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

# Function to find optional encryption libraries
function(logger_find_encryption)
    if(NOT LOGGER_USE_ENCRYPTION)
        return()
    endif()

    # Try to find OpenSSL 3.x first (preferred), then fall back to 1.1.1
    find_package(OpenSSL 3.0 QUIET)
    if(OpenSSL_FOUND)
        message(STATUS "Found OpenSSL 3.x for encryption support: ${OPENSSL_VERSION}")
        set(LOGGER_HAS_ENCRYPTION TRUE PARENT_SCOPE)
        set(LOGGER_OPENSSL_3X TRUE PARENT_SCOPE)
    else()
        # Fall back to OpenSSL 1.1.1
        find_package(OpenSSL 1.1.1 QUIET)
        if(OpenSSL_FOUND)
            message(WARNING "OpenSSL 1.1.1 detected (EOL: September 2023). Consider upgrading to OpenSSL 3.x for continued security support.")
            set(LOGGER_HAS_ENCRYPTION TRUE PARENT_SCOPE)
            set(LOGGER_OPENSSL_3X FALSE PARENT_SCOPE)
        else()
            # Try any OpenSSL version as last resort
            find_package(OpenSSL QUIET)
            if(OpenSSL_FOUND)
                message(WARNING "Found OpenSSL ${OPENSSL_VERSION}. Recommended: OpenSSL 3.x or later.")
                set(LOGGER_HAS_ENCRYPTION TRUE PARENT_SCOPE)
                set(LOGGER_OPENSSL_3X FALSE PARENT_SCOPE)
            else()
                message(WARNING "OpenSSL not found - encryption support disabled")
                set(LOGGER_USE_ENCRYPTION OFF PARENT_SCOPE)
                set(LOGGER_HAS_ENCRYPTION FALSE PARENT_SCOPE)
            endif()
        endif()
    endif()
endfunction()

# Function to find testing dependencies
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

# Function to find benchmark dependencies
function(logger_find_benchmark_dependencies)
    if(NOT BUILD_BENCHMARKS)
        return()
    endif()

    # Try to find Google Benchmark
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

# Main function to find all dependencies
# Note: thread_system and common_system are handled by UnifiedDependencies.cmake
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
