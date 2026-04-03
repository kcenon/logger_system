# Utils unit tests (string_utils, time_utils, file_utils)
add_executable(logger_string_utils_test string_utils_test.cpp)
add_executable(logger_time_utils_test time_utils_test.cpp)
add_executable(logger_file_utils_test file_utils_test.cpp)

foreach(_target IN ITEMS logger_string_utils_test logger_time_utils_test logger_file_utils_test)
    if(TARGET GTest::gtest_main)
        target_link_libraries(${_target} PRIVATE logger_system GTest::gtest_main)
    else()
        target_link_libraries(${_target} PRIVATE logger_system gtest_main)
    endif()

    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endforeach()

add_test(NAME logger_string_utils_test COMMAND logger_string_utils_test)
add_test(NAME logger_time_utils_test COMMAND logger_time_utils_test)
add_test(NAME logger_file_utils_test COMMAND logger_file_utils_test)
