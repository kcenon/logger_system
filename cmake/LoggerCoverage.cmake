# LoggerCoverage.cmake
# Code coverage configuration for Logger System

# Coverage options
option(LOGGER_ENABLE_COVERAGE "Enable code coverage reporting" OFF)
set(LOGGER_COVERAGE_TARGETS
    basic_integration_test
    thread_system_integration_test
    monitoring_integration_test
    version_compatibility_test
    logger_min_level_threshold_test
    logger_thread_safety_test
    integration_tests
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
macro(logger_enable_coverage_for_all)
    if(LOGGER_ENABLE_COVERAGE)
        # Add coverage to main library
        if(TARGET LoggerSystem)
            logger_add_coverage(LoggerSystem)
        endif()

        # Add coverage to test executables
        if(BUILD_TESTS)
            foreach(_cov_target IN LISTS LOGGER_COVERAGE_TARGETS)
                if(TARGET ${_cov_target})
                    logger_add_coverage(${_cov_target})
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
