# Log filter unit tests (level, regex, composite, function, field, category filters)
add_executable(logger_filters_test filters_test.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_filters_test PRIVATE logger_system GTest::gtest_main)
else()
    target_link_libraries(logger_filters_test PRIVATE logger_system gtest_main)
endif()

add_test(NAME logger_filters_test COMMAND logger_filters_test)
set_target_properties(logger_filters_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
