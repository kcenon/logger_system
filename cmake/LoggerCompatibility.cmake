# LoggerCompatibility.cmake
# C++20 feature detection and compatibility layer (std::format required)

##################################################
# Feature Detection Functions
##################################################

# Test for std::format availability (C++20 required)
function(check_std_format)
    include(CheckCXXSourceCompiles)

    set(CMAKE_REQUIRED_FLAGS "-std=c++20")
    check_cxx_source_compiles("
        #include <format>
        int main() {
            auto s = std::format(\"Hello {}\", \"World\");
            return 0;
        }
    " HAS_STD_FORMAT)

    if(HAS_STD_FORMAT)
        message(STATUS "✅ std::format is available")
        add_definitions(-DLOGGER_HAS_STD_FORMAT)
        add_definitions(-DLOGGER_USE_STD_FORMAT)
        set(LOGGER_FORMAT_BACKEND "std::format" PARENT_SCOPE)
    else()
        message(FATAL_ERROR "std::format is required. Please use a C++20 compliant compiler:\n"
            "  - GCC 13+\n"
            "  - Clang 14+\n"
            "  - MSVC 19.29+ (Visual Studio 2019 16.10+)\n"
            "  - Apple Clang 15+")
    endif()
endfunction()

# Test for concepts availability
function(check_concepts)
    include(CheckCXXSourceCompiles)
    
    set(CMAKE_REQUIRED_FLAGS "-std=c++20")
    check_cxx_source_compiles("
        #include <concepts>
        template<typename T>
        concept Addable = requires(T a, T b) {
            a + b;
        };
        template<Addable T>
        T add(T a, T b) { return a + b; }
        int main() {
            return add(1, 2);
        }
    " HAS_CONCEPTS)
    
    if(HAS_CONCEPTS)
        message(STATUS "C++20 concepts are available")
        add_definitions(-DLOGGER_HAS_CONCEPTS)
    else()
        message(STATUS "C++20 concepts not available - using SFINAE fallback")
    endif()
endfunction()

# Test for std::span availability
function(check_std_span)
    include(CheckCXXSourceCompiles)
    
    set(CMAKE_REQUIRED_FLAGS "-std=c++20")
    check_cxx_source_compiles("
        #include <span>
        int main() {
            int arr[] = {1, 2, 3};
            std::span<int> s(arr);
            return s.size();
        }
    " HAS_STD_SPAN)
    
    if(HAS_STD_SPAN)
        message(STATUS "std::span is available")
        add_definitions(-DLOGGER_HAS_STD_SPAN)
    else()
        message(STATUS "std::span not available - using pointer/size pairs")
    endif()
endfunction()

# Test for designated initializers
function(check_designated_initializers)
    include(CheckCXXSourceCompiles)
    
    set(CMAKE_REQUIRED_FLAGS "-std=c++20")
    check_cxx_source_compiles("
        struct Point { int x, y; };
        int main() {
            Point p{.x = 1, .y = 2};
            return p.x + p.y;
        }
    " HAS_DESIGNATED_INITIALIZERS)
    
    if(HAS_DESIGNATED_INITIALIZERS)
        message(STATUS "Designated initializers are available")
        add_definitions(-DLOGGER_HAS_DESIGNATED_INITIALIZERS)
    else()
        message(STATUS "Designated initializers not available - using traditional initialization")
    endif()
endfunction()

##################################################
# Main Compatibility Check Function
##################################################

function(configure_logger_compatibility)
    message(STATUS "========================================")
    message(STATUS "Logger System - Feature Detection:")
    message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")

    if(CMAKE_CXX_STANDARD LESS 20)
        message(FATAL_ERROR "C++20 is required. Please set CMAKE_CXX_STANDARD to 20 or higher.")
    endif()

    message(STATUS "  Mode: C++20 Required")
    add_definitions(-DLOGGER_CPP20_MODE)

    # Test each C++20 feature
    check_std_format()
    check_concepts()
    check_std_span()
    check_designated_initializers()

    message(STATUS "========================================")
endfunction()

##################################################
# External Dependency Management
##################################################

# Setup formatting library for target (std::format only)
function(setup_formatting_library TARGET_NAME)
    message(STATUS "========================================")
    message(STATUS "Setting up formatting library for ${TARGET_NAME}")

    if(DEFINED LOGGER_FORMAT_BACKEND AND LOGGER_FORMAT_BACKEND STREQUAL "std::format")
        message(STATUS "  ✅ Using std::format (C++20 native)")
        target_compile_definitions(${TARGET_NAME} PRIVATE LOGGER_USE_STD_FORMAT)
    else()
        message(FATAL_ERROR "std::format is required. LOGGER_FORMAT_BACKEND must be 'std::format'.")
    endif()

    message(STATUS "========================================")
endfunction()