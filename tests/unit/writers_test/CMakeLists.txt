project(writers_unit)

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Direct linking for macOS
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add GTest include path (if vcpkg is used)
if(DEFINED CMAKE_TOOLCHAIN_FILE AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../build/vcpkg_installed")
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../build/vcpkg_installed/arm64-osx/include)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../build/vcpkg_installed/x64-osx/include)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../build/vcpkg_installed/x64-linux/include)
    endif()
endif()

# Resolve GoogleTest targets (supports FetchContent and system installs)
set(_LOGGER_GTEST_TARGET "")
set(_LOGGER_GTEST_MAIN_TARGET "")

foreach(_candidate IN ITEMS GTest::gtest GTest::GTest gtest)
    if(NOT _LOGGER_GTEST_TARGET AND TARGET ${_candidate})
        set(_LOGGER_GTEST_TARGET ${_candidate})
    endif()
endforeach()

foreach(_candidate IN ITEMS GTest::gtest_main GTest::Main gtest_main)
    if(NOT _LOGGER_GTEST_MAIN_TARGET AND TARGET ${_candidate})
        set(_LOGGER_GTEST_MAIN_TARGET ${_candidate})
    endif()
endforeach()

if(NOT _LOGGER_GTEST_TARGET OR NOT _LOGGER_GTEST_MAIN_TARGET)
    find_package(GTest QUIET)

    foreach(_candidate IN ITEMS GTest::gtest GTest::GTest gtest)
        if(NOT _LOGGER_GTEST_TARGET AND TARGET ${_candidate})
            set(_LOGGER_GTEST_TARGET ${_candidate})
        endif()
    endforeach()

    foreach(_candidate IN ITEMS GTest::gtest_main GTest::Main gtest_main)
        if(NOT _LOGGER_GTEST_MAIN_TARGET AND TARGET ${_candidate})
            set(_LOGGER_GTEST_MAIN_TARGET ${_candidate})
        endif()
    endforeach()
endif()

if(NOT _LOGGER_GTEST_TARGET AND GTEST_LIBRARIES AND GTEST_INCLUDE_DIRS)
    if(NOT TARGET logger_system_gtest_imported)
        add_library(logger_system_gtest_imported INTERFACE)
    endif()
    target_include_directories(logger_system_gtest_imported INTERFACE ${GTEST_INCLUDE_DIRS})
    target_link_libraries(logger_system_gtest_imported INTERFACE ${GTEST_LIBRARIES})
    set(_LOGGER_GTEST_TARGET logger_system_gtest_imported)
endif()

if(NOT _LOGGER_GTEST_MAIN_TARGET)
    if(GTEST_MAIN_LIBRARIES AND GTEST_INCLUDE_DIRS)
        if(NOT TARGET logger_system_gtest_main_imported)
            add_library(logger_system_gtest_main_imported INTERFACE)
        endif()
        target_include_directories(logger_system_gtest_main_imported INTERFACE ${GTEST_INCLUDE_DIRS})
        target_link_libraries(logger_system_gtest_main_imported INTERFACE ${GTEST_MAIN_LIBRARIES})
        set(_LOGGER_GTEST_MAIN_TARGET logger_system_gtest_main_imported)
    elseif(GTEST_BOTH_LIBRARIES AND GTEST_INCLUDE_DIRS)
        if(NOT TARGET logger_system_gtest_main_imported)
            add_library(logger_system_gtest_main_imported INTERFACE)
        endif()
        target_include_directories(logger_system_gtest_main_imported INTERFACE ${GTEST_INCLUDE_DIRS})
        target_link_libraries(logger_system_gtest_main_imported INTERFACE ${GTEST_BOTH_LIBRARIES})
        set(_LOGGER_GTEST_MAIN_TARGET logger_system_gtest_main_imported)
    endif()
endif()

if(NOT _LOGGER_GTEST_TARGET OR NOT _LOGGER_GTEST_MAIN_TARGET)
    message(FATAL_ERROR "GoogleTest targets not available. Ensure logger_find_test_dependencies() ran before configuring writers_unit.")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    LoggerSystem
    ${_LOGGER_GTEST_TARGET}
    ${_LOGGER_GTEST_MAIN_TARGET}
    Threads::Threads
)

enable_testing()

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

add_custom_target(${PROJECT_NAME}_test ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>
    DEPENDS ${PROJECT_NAME}
)
