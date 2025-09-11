# LoggerCompatibility.cmake
# C++17/C++20 feature detection and compatibility layer

##################################################
# Feature Detection Functions
##################################################

# Test for std::format availability
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
        message(STATUS "std::format is available")
        add_definitions(-DLOGGER_HAS_STD_FORMAT)
    else()
        message(STATUS "std::format not available - using fallback formatting")
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
    
    if(CMAKE_CXX_STANDARD EQUAL 17)
        message(STATUS "  Mode: C++17 Compatibility")
        add_definitions(-DLOGGER_CPP17_MODE)
        
        # In C++17 mode, disable all C++20 features
        message(STATUS "  - std::format: DISABLED (using fmt library)")
        message(STATUS "  - concepts: DISABLED (using SFINAE)")
        message(STATUS "  - std::span: DISABLED (using pointer/size)")
        message(STATUS "  - designated initializers: DISABLED")
        
    else()
        message(STATUS "  Mode: C++20 Enhanced")
        add_definitions(-DLOGGER_CPP20_MODE)
        
        # Test each C++20 feature
        check_std_format()
        check_concepts()
        check_std_span()
        check_designated_initializers()
    endif()
    
    message(STATUS "========================================")
endfunction()

##################################################
# External Dependency Management
##################################################

function(setup_formatting_library)
    if(CMAKE_CXX_STANDARD EQUAL 17 OR NOT HAS_STD_FORMAT)
        # Use fmt library as fallback
        find_package(fmt QUIET)
        if(fmt_FOUND)
            message(STATUS "Using external fmt library for formatting")
            add_definitions(-DLOGGER_USE_FMT)
        else()
            message(STATUS "fmt library not found - using basic string formatting")
            add_definitions(-DLOGGER_USE_BASIC_FORMAT)
        endif()
    else()
        message(STATUS "Using std::format for formatting")
        add_definitions(-DLOGGER_USE_STD_FORMAT)
    endif()
endfunction()