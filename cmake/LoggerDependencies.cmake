##################################################
# Logger System Dependency Detection Module
#
# This module handles finding and configuring external dependencies
##################################################

# Function to find and configure thread_system dependency
function(logger_find_thread_system)
    if(NOT USE_THREAD_SYSTEM OR LOGGER_STANDALONE)
        message(STATUS "Skipping thread_system (standalone mode or disabled)")
        return()
    endif()
    
    # First check if thread_system is available as a sibling directory
    if(EXISTS "${CMAKE_SOURCE_DIR}/../thread_system" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/../thread_system")
        message(STATUS "Found thread_system as sibling directory")
        
        # Add thread_system subdirectory
        add_subdirectory(${CMAKE_SOURCE_DIR}/../thread_system ${CMAKE_BINARY_DIR}/thread_system)

        if(TARGET utilities)
            target_include_directories(utilities PUBLIC
                $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/../thread_system/include>
            )
        endif()
        
        # Set flags to indicate thread_system is available
        set(THREAD_SYSTEM_FOUND TRUE PARENT_SCOPE)
        set(USE_THREAD_SYSTEM TRUE PARENT_SCOPE)
        
        # Add compile definition
        add_compile_definitions(USE_THREAD_SYSTEM)
        
        message(STATUS "Using thread_system for interfaces and utilities")
    else()
        # Try to find installed thread_system
        find_package(ThreadSystem QUIET)
        
        if(ThreadSystem_FOUND)
            message(STATUS "Found installed ThreadSystem: ${ThreadSystem_DIR}")
            set(THREAD_SYSTEM_FOUND TRUE PARENT_SCOPE)
            set(USE_THREAD_SYSTEM TRUE PARENT_SCOPE)
            add_compile_definitions(USE_THREAD_SYSTEM)
        else()
            message(STATUS "thread_system not found - building in standalone mode")
            set(THREAD_SYSTEM_FOUND FALSE PARENT_SCOPE)
            set(USE_THREAD_SYSTEM FALSE PARENT_SCOPE)
            set(LOGGER_STANDALONE TRUE PARENT_SCOPE)
            add_compile_definitions(LOGGER_STANDALONE)
        endif()
    endif()
endfunction()

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
    
    # Try to find OpenSSL for encryption
    find_package(OpenSSL QUIET)
    if(OpenSSL_FOUND)
        message(STATUS "Found OpenSSL for encryption support: ${OPENSSL_VERSION}")
        set(LOGGER_HAS_ENCRYPTION TRUE PARENT_SCOPE)
    else()
        message(WARNING "OpenSSL not found - encryption support disabled")
        set(LOGGER_USE_ENCRYPTION OFF PARENT_SCOPE)
        set(LOGGER_HAS_ENCRYPTION FALSE PARENT_SCOPE)
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

        # Set options before FetchContent_Declare for all platforms
        set(gtest_force_shared_crt ON CACHE BOOL "Use shared CRT when building GoogleTest" FORCE)
        set(BUILD_GMOCK ON CACHE BOOL "Build gmock" FORCE)

        # Windows-specific pthread disable
        if(WIN32 OR MSVC OR MINGW)
            set(gtest_disable_pthreads ON CACHE BOOL "Disable pthread usage in GoogleTest" FORCE)
            set(GTEST_HAS_PTHREAD 0 CACHE INTERNAL "Explicitly disable pthread support in GoogleTest")
            # Add compile definitions to ensure pthread is disabled
            add_compile_definitions(GTEST_HAS_PTHREAD=0)
        endif()

        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
        )

        # Populate first to get the targets
        FetchContent_GetProperties(googletest)
        if(NOT googletest_POPULATED)
            FetchContent_Populate(googletest)

            # Force disable pthread before adding subdirectory on Windows
            if(WIN32 OR MSVC OR MINGW)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTEST_HAS_PTHREAD=0" CACHE STRING "" FORCE)
            endif()

            add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

            # Apply compile definitions to all Google Test targets after they're created
            if(WIN32 OR MSVC OR MINGW)
                if(TARGET gtest)
                    target_compile_definitions(gtest PUBLIC GTEST_HAS_PTHREAD=0)
                endif()
                if(TARGET gtest_main)
                    target_compile_definitions(gtest_main PUBLIC GTEST_HAS_PTHREAD=0)
                endif()
                if(TARGET gmock)
                    target_compile_definitions(gmock PUBLIC GTEST_HAS_PTHREAD=0)
                endif()
                if(TARGET gmock_main)
                    target_compile_definitions(gmock_main PUBLIC GTEST_HAS_PTHREAD=0)
                endif()
            endif()
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
function(logger_find_all_dependencies)
    message(STATUS "========================================")
    message(STATUS "Detecting Logger System Dependencies...")
    
    # Required dependencies
    find_package(Threads REQUIRED)
    
    # Optional dependencies
    logger_find_thread_system()
    logger_find_compression()
    logger_find_encryption()
    logger_find_test_dependencies()
    logger_find_benchmark_dependencies()
    
    message(STATUS "Dependency Detection Complete")
    message(STATUS "========================================")
endfunction()
