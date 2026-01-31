project(logger_unit)

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

# GTest should already be found by parent CMakeLists.txt
# Just verify it's available
if(NOT TARGET GTest::gtest)
    message(FATAL_ERROR "GTest not found. Please install via vcpkg or enable FetchContent in parent CMakeLists.txt")
endif()
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

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
    message(FATAL_ERROR "GoogleTest targets not available. Ensure logger_find_test_dependencies() ran before configuring logger_unit.")
endif()

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