cmake_minimum_required(VERSION 3.20)

# C++20 Modules require CMake 3.28+ (gates LOGGER_USE_MODULES in options.cmake)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.28")
    set(LOGGER_CAN_USE_MODULES ON)
else()
    set(LOGGER_CAN_USE_MODULES OFF)
endif()

project(
    logger_system
    VERSION 1.0.0
    DESCRIPTION "High-performance C++20 logging system with asynchronous batching"
    HOMEPAGE_URL "https://github.com/kcenon/logger_system"
    LANGUAGES CXX
)

# Output directories shared across targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

##################################################
# Canonical cmake module orchestration (kcenon-system-layout v1.1)
##################################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(options)        # 1. option() / feature flags / performance tuning
include(compiler)       # 2. C++ standard, platform, sanitizers, feature checks
include(dependencies)   # 3. find_package + FetchContent + thread/common_system
include(warnings)       # 4. logger_add_warnings(), logger_enable_warnings_for_all()
include(targets)        # 5. logger_system library target with explicit sources
include(install)        # 6. install rules + config-package generation
include(testing)        # 7. enable_testing(), tests/, integration_tests/, coverage
include(examples)       # 8. examples/samples subdirectory wiring

# Benchmarks remain inline (relies on cmake/FetchBenchmark.cmake helper)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
