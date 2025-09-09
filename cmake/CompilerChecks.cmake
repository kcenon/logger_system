# CompilerChecks.cmake for logger_system
# Based on thread_system's robust compiler checking system

##################################################
# Minimum Compiler Version Requirements
##################################################

# Define minimum compiler versions for C++20 support
set(MIN_GCC_VERSION "10.0")
set(MIN_CLANG_VERSION "11.0")
set(MIN_APPLECLANG_VERSION "12.0")
set(MIN_MSVC_VERSION "19.26")  # Visual Studio 2019 16.6

# Check compiler version
function(check_compiler_version)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${MIN_GCC_VERSION})
            message(FATAL_ERROR "GCC version ${CMAKE_CXX_COMPILER_VERSION} is not supported. Minimum required version is ${MIN_GCC_VERSION}")
        endif()
        message(STATUS "GCC ${CMAKE_CXX_COMPILER_VERSION} - C++20 support verified")
        
        # Special handling for MinGW
        if(MINGW)
            message(STATUS "MinGW detected - applying compatibility settings")
            add_definitions(-D__MINGW32__)
            # MinGW often needs explicit std library linking
            set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lstdc++fs")
        endif()
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${MIN_CLANG_VERSION})
            message(FATAL_ERROR "Clang version ${CMAKE_CXX_COMPILER_VERSION} is not supported. Minimum required version is ${MIN_CLANG_VERSION}")
        endif()
        message(STATUS "Clang ${CMAKE_CXX_COMPILER_VERSION} - C++20 support verified")
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${MIN_APPLECLANG_VERSION})
            message(FATAL_ERROR "Apple Clang version ${CMAKE_CXX_COMPILER_VERSION} is not supported. Minimum required version is ${MIN_APPLECLANG_VERSION}")
        endif()
        message(STATUS "Apple Clang ${CMAKE_CXX_COMPILER_VERSION} - C++20 support verified")
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(MSVC_VERSION LESS 1926)
            message(FATAL_ERROR "MSVC version ${MSVC_VERSION} is not supported. Minimum required version is ${MIN_MSVC_VERSION} (Visual Studio 2019 16.6)")
        endif()
        message(STATUS "MSVC ${MSVC_VERSION} - C++20 support verified")
        
    else()
        message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
        message(WARNING "C++20 support cannot be verified. Build may fail.")
    endif()
endfunction()

##################################################
# Platform Configuration
##################################################
function(configure_platform_settings)
    message(STATUS "========================================")
    message(STATUS "Platform Configuration:")
    message(STATUS "  System: ${CMAKE_SYSTEM_NAME}")
    message(STATUS "  Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
    message(STATUS "========================================")
    
    # Platform-specific configurations
    if(WIN32)
        # Windows-specific settings
        add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
        if(MSVC)
            # MSVC-specific warnings and optimizations
            add_compile_options(/W4 /permissive-)
        endif()
    elseif(APPLE)
        # macOS-specific settings
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-no_warn_duplicate_libraries")
    elseif(UNIX)
        # Linux/Unix-specific settings
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            add_compile_options(-Wall -Wextra -Wpedantic)
        endif()
    endif()
endfunction()

##################################################
# Build Type Configuration
##################################################
function(configure_build_types)
    # Set default build type if not specified
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
        message(STATUS "Build type not specified, defaulting to Release")
    endif()
    
    # Configure build-specific flags
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            add_compile_options(-O3 -DNDEBUG)
            # Enable architecture-specific optimizations on non-CI builds
            if(NOT DEFINED ENV{CI})
                add_compile_options(-march=native)
            endif()
        elseif(MSVC)
            add_compile_options(/O2 /DNDEBUG)
        endif()
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            add_compile_options(-g -O0)
        elseif(MSVC)
            add_compile_options(/Od /Zi)
        endif()
    endif()
endfunction()

##################################################
# Required Headers Check
##################################################
function(check_required_headers)
    include(CheckIncludeFileCXX)
    
    set(REQUIRED_HEADERS
        algorithm
        atomic
        chrono
        condition_variable
        deque
        functional
        future
        iostream
        memory
        mutex
        optional
        string
        thread
        tuple
        utility
        vector
        string_view
        variant
        any
    )
    
    foreach(HEADER ${REQUIRED_HEADERS})
        check_include_file_cxx(${HEADER} HAS_${HEADER})
        if(NOT HAS_${HEADER})
            message(FATAL_ERROR "Required header <${HEADER}> not found")
        endif()
    endforeach()
    
    message(STATUS "All required C++ headers found")
endfunction()

##################################################
# C++20 Feature Checks
##################################################
function(check_cpp_features)
    include(CheckCXXSourceCompiles)
    
    # Check for C++20 concepts
    check_cxx_source_compiles("
        #include <concepts>
        template<typename T>
        concept Incrementable = requires(T t) {
            ++t;
        };
        int main() { return 0; }
    " HAS_CONCEPTS)
    
    if(HAS_CONCEPTS)
        message(STATUS "✅ C++20 concepts support detected")
    else()
        message(STATUS "⚠️ C++20 concepts not fully supported")
    endif()
    
    # Check for std::format
    check_cxx_source_compiles("
        #include <format>
        int main() {
            auto s = std::format(\"Hello, {}!\", \"World\");
            return 0;
        }
    " HAS_STD_FORMAT)
    
    if(HAS_STD_FORMAT)
        message(STATUS "✅ std::format support detected")
        add_compile_definitions(USE_STD_FORMAT=1)
    else()
        message(STATUS "⚠️ std::format not available, using alternative formatting")
    endif()
    
    # Check for std::jthread
    check_cxx_source_compiles("
        #include <thread>
        int main() {
            std::jthread t([](){});
            return 0;
        }
    " HAS_STD_JTHREAD)
    
    if(HAS_STD_JTHREAD)
        message(STATUS "✅ std::jthread support detected")
    else()
        message(STATUS "⚠️ std::jthread not available, using std::thread")
    endif()
endfunction()