# Logger library
add_library(logger)

target_sources(logger
    PRIVATE
        logger/logger.cpp
        logger/core/log_collector.cpp
        logger/writers/console_writer.cpp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger/logger.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger/writers/base_writer.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger/writers/console_writer.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/logger/core/log_collector.h>
)

target_include_directories(logger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/logger_system>
)

# Link with system libraries
target_link_libraries(logger
    PUBLIC
        Threads::Threads
)

# Set properties
set_target_properties(logger PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

# Compiler-specific options
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(logger PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(logger PRIVATE /W4)
endif()