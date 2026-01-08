# LoggerCoverage.cmake
# Code coverage configuration for Logger System
#
# This module provides automatic coverage instrumentation for test targets.
# Instead of maintaining a static list, test targets are automatically registered
# using the logger_register_coverage_target() function.

# Coverage options
option(LOGGER_ENABLE_COVERAGE "Enable code coverage reporting" OFF)

# Initialize global property for tracking coverage targets
# This replaces the static LOGGER_COVERAGE_TARGETS list with dynamic registration
define_property(GLOBAL PROPERTY LOGGER_REGISTERED_COVERAGE_TARGETS
    BRIEF_DOCS "List of test targets registered for coverage instrumentation"
    FULL_DOCS "Automatically populated by logger_register_coverage_target(). \
               Used by logger_enable_coverage_for_all() to apply coverage flags."
)
set_property(GLOBAL PROPERTY LOGGER_REGISTERED_COVERAGE_TARGETS "")

# Function to register a test target for coverage instrumentation
# Usage: logger_register_coverage_target(target_name)
# This should be called after add_executable() for each test target
function(logger_register_coverage_target target_name)
    if(NOT TARGET ${target_name})
        message(WARNING "logger_register_coverage_target: Target '${target_name}' does not exist")
        return()
    endif()

    # Append to global list
    set_property(GLOBAL APPEND PROPERTY LOGGER_REGISTERED_COVERAGE_TARGETS ${target_name})

    if(LOGGER_ENABLE_COVERAGE)
        message(STATUS "Registered coverage target: ${target_name}")
    endif()
endfunction()

# Legacy static list for backward compatibility (deprecated)
# New tests should use logger_register_coverage_target() instead
set(LOGGER_COVERAGE_TARGETS_LEGACY
    integration_test
    stress_test
    config_unit
    di_container_test
    crash_safety_test
    monitoring_test
    logger_test
    overflow_policy_test
    health_check_test
)

# Function to add coverage flags to a target
function(logger_add_coverage target)
    if(NOT LOGGER_ENABLE_COVERAGE)
        return()
    endif()
    
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "Code coverage is only supported in Debug builds")
        return()
    endif()
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        # Coverage flags for GCC/Clang
        target_compile_options(${target} PRIVATE
            --coverage
            -fprofile-arcs
            -ftest-coverage
            -O0  # No optimization for accurate coverage
            -g   # Debug symbols
        )

        target_link_options(${target} PRIVATE
            --coverage
            -fprofile-arcs
            -ftest-coverage
        )

        # GCC requires explicit gcov library link when using static libraries
        # with coverage instrumentation. The --coverage flag adds gcov symbols
        # to object files, but the gcov library must be explicitly linked
        # to resolve these symbols in the final executable.
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_link_libraries(${target} PRIVATE gcov)
        endif()

        message(STATUS "Enabled code coverage for ${target}")
        
    elseif(MSVC)
        message(WARNING "Code coverage is not yet supported for MSVC")
    else()
        message(WARNING "Code coverage is not supported for ${CMAKE_CXX_COMPILER_ID}")
    endif()
endfunction()

# Function to setup coverage target
function(logger_setup_coverage_target)
    if(NOT LOGGER_ENABLE_COVERAGE)
        return()
    endif()
    
    # Find required tools
    find_program(GCOV_EXECUTABLE gcov)
    find_program(LCOV_EXECUTABLE lcov)
    find_program(GENHTML_EXECUTABLE genhtml)
    find_program(GCOVR_EXECUTABLE gcovr)
    
    if(GCOVR_EXECUTABLE)
        # Use gcovr for coverage report generation
        add_custom_target(coverage
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
            COMMAND ${GCOVR_EXECUTABLE}
                --root ${CMAKE_SOURCE_DIR}
                --filter ${CMAKE_SOURCE_DIR}/sources/
                --exclude ${CMAKE_SOURCE_DIR}/unittest/
                --exclude ${CMAKE_SOURCE_DIR}/tests/
                --html --html-details
                --output ${CMAKE_BINARY_DIR}/coverage/index.html
                --xml ${CMAKE_BINARY_DIR}/coverage/coverage.xml
                --json ${CMAKE_BINARY_DIR}/coverage/coverage.json
                --print-summary
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating code coverage report with gcovr"
        )
        
        message(STATUS "Coverage target 'coverage' created (using gcovr)")
        
    elseif(LCOV_EXECUTABLE AND GENHTML_EXECUTABLE)
        # Use lcov for coverage report generation
        add_custom_target(coverage
            # Cleanup
            COMMAND ${LCOV_EXECUTABLE} --directory . --zerocounters
            
            # Run tests
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            
            # Capture coverage data
            COMMAND ${LCOV_EXECUTABLE} --directory . --capture --output-file coverage.info
            
            # Remove unwanted files from coverage
            COMMAND ${LCOV_EXECUTABLE} --remove coverage.info 
                '/usr/*' '*/unittest/*' '*/tests/*' '*/build/*' 
                --output-file coverage.cleaned
            
            # Generate HTML report
            COMMAND ${GENHTML_EXECUTABLE} coverage.cleaned 
                --output-directory ${CMAKE_BINARY_DIR}/coverage
                --title "Logger System Coverage Report"
                --show-details --legend
            
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating code coverage report with lcov"
        )
        
        message(STATUS "Coverage target 'coverage' created (using lcov)")
        
    else()
        message(WARNING "Coverage tools not found. Install gcovr or lcov+genhtml for coverage reports")
    endif()
    
    # Add coverage reset target
    add_custom_target(coverage-reset
        COMMAND find ${CMAKE_BINARY_DIR} -name '*.gcda' -delete
        COMMENT "Resetting coverage data"
    )
endfunction()

# Macro to enable coverage for all targets
# This macro should be called AFTER all test subdirectories have been processed
# to ensure all registered targets are covered
macro(logger_enable_coverage_for_all)
    if(LOGGER_ENABLE_COVERAGE)
        # Add coverage to main library
        if(TARGET LoggerSystem)
            logger_add_coverage(LoggerSystem)
        endif()

        # Add coverage to dynamically registered test targets
        if(BUILD_TESTS)
            # Get the list of registered coverage targets
            get_property(_registered_targets GLOBAL PROPERTY LOGGER_REGISTERED_COVERAGE_TARGETS)

            if(_registered_targets)
                message(STATUS "Applying coverage to ${CMAKE_CURRENT_LIST_LINE} registered test targets")
                foreach(_cov_target IN LISTS _registered_targets)
                    if(TARGET ${_cov_target})
                        logger_add_coverage(${_cov_target})
                    else()
                        message(WARNING "Coverage target '${_cov_target}' was registered but does not exist")
                    endif()
                endforeach()
            endif()

            # Also check legacy static list for backward compatibility
            foreach(_cov_target IN LISTS LOGGER_COVERAGE_TARGETS_LEGACY)
                if(TARGET ${_cov_target})
                    # Check if not already in registered list to avoid duplicate processing
                    list(FIND _registered_targets ${_cov_target} _idx)
                    if(_idx EQUAL -1)
                        logger_add_coverage(${_cov_target})
                    endif()
                endif()
            endforeach()
        endif()

        # Setup coverage target
        logger_setup_coverage_target()
    endif()
endmacro()

# Print coverage configuration
if(LOGGER_ENABLE_COVERAGE)
    message(STATUS "========================================")
    message(STATUS "Coverage Configuration:")
    message(STATUS "  Coverage Enabled: YES")
    message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
    message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "========================================")
endif()
