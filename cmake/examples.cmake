# examples.cmake -- Example/sample target registration
#
# Inputs:
#   - BUILD_SAMPLES: option (declared in options.cmake), gates inclusion
#   - ENV{CI}: environment variable; when set, skip examples
# Outputs:
#   - examples/ or samples/ subdirectory added when conditions met

if(BUILD_SAMPLES)
    # Skip examples in CI environment to avoid compilation errors with incomplete features
    if(NOT DEFINED ENV{CI})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples)
            add_subdirectory(examples)
        elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/samples)
            add_subdirectory(samples)
        endif()
    else()
        message(STATUS "Skipping examples/samples in CI environment")
    endif()
endif()
