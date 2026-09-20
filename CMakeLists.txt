cmake_minimum_required(VERSION 3.20)

# C++20 Modules require CMake 3.28+ (gates LOGGER_USE_MODULES in options.cmake)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.28")
    set(LOGGER_CAN_USE_MODULES ON)
else()
    set(LOGGER_CAN_USE_MODULES OFF)
endif()

# Read version from the VERSION file (single source of truth, R-29).
# The file contains a single SemVer line, e.g. "1.0.0".
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" LOGGER_VERSION_RAW)
string(STRIP "${LOGGER_VERSION_RAW}" LOGGER_VERSION)

project(
    logger_system
    VERSION ${LOGGER_VERSION}
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
# Resolve project modules explicitly: dependencies also use names such as
# options.cmake and dependencies.cmake in their own CMAKE_MODULE_PATH.
include(cmake/options.cmake)        # 1. option() / feature flags / performance tuning
include(cmake/compiler.cmake)       # 2. C++ standard, platform, sanitizers, feature checks
include(cmake/dependencies.cmake)   # 3. find_package + FetchContent + thread/common_system
include(cmake/warnings.cmake)       # 4. logger_add_warnings(), logger_enable_warnings_for_all()
include(cmake/targets.cmake)        # 5. logger_system library target with explicit sources
include(cmake/install.cmake)        # 6. install rules + config-package generation
include(cmake/testing.cmake)        # 7. enable_testing(), tests/, integration_tests/, coverage
include(cmake/examples.cmake)       # 8. examples/samples subdirectory wiring

# Benchmarks remain inline (relies on cmake/FetchBenchmark.cmake helper)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
