# Fuzz harnesses for logger_system
#
# Gated behind BUILD_FUZZERS so the default build is unaffected. libFuzzer is a
# Clang feature, so this target requires a Clang/Clang++ toolchain. Enable with:
#
#   cmake -B build -G Ninja \
#     -DBUILD_FUZZERS=ON \
#     -DCMAKE_C_COMPILER=clang \
#     -DCMAKE_CXX_COMPILER=clang++
#   cmake --build build --target log_entry_fuzzer
#
# Run (example):
#   ./build/fuzz/log_entry_fuzzer -max_total_time=60 fuzz/corpus

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(WARNING
        "BUILD_FUZZERS=ON requires a Clang toolchain for libFuzzer; "
        "skipping fuzz targets (compiler: ${CMAKE_CXX_COMPILER_ID}).")
    return()
endif()

set(FUZZER_FLAGS -fsanitize=fuzzer,address -g -O1 -fno-omit-frame-pointer)

add_executable(log_entry_fuzzer log_entry_fuzzer.cpp)

target_include_directories(log_entry_fuzzer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_compile_features(log_entry_fuzzer PRIVATE cxx_std_20)
target_compile_options(log_entry_fuzzer PRIVATE ${FUZZER_FLAGS})
target_link_options(log_entry_fuzzer PRIVATE ${FUZZER_FLAGS})

message(STATUS "Fuzz targets enabled (libFuzzer + AddressSanitizer)")
