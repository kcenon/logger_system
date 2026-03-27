# kcenon-logger-system portfile
# High-performance C++20 async logging library with 4.34M msg/sec throughput

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO kcenon/logger_system
    REF "v${VERSION}"
    SHA512 90884ec4210e6ebeb534aed2eb9928cfc2a80d0ab7220f9ed9be63d32bcfc6154e6bd2960c7d11cb196c3dcb345650c6e182f8edc44867b4bcf0efeb1dfed0f2
    HEAD_REF main
)

# Feature-based options
vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        encryption    LOGGER_USE_ENCRYPTION
        otlp          LOGGER_ENABLE_OTLP
        thread-system LOGGER_USE_THREAD_SYSTEM
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_TESTS=OFF
        -DBUILD_BENCHMARKS=OFF
        -DBUILD_SAMPLES=OFF
        -DLOGGER_BUILD_INTEGRATION_TESTS=OFF
        -DLOGGER_ENABLE_COVERAGE=OFF
        -DFETCHCONTENT_FULLY_DISCONNECTED=ON
        ${FEATURE_OPTIONS}
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME LoggerSystem
    CONFIG_PATH lib/cmake/LoggerSystem
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
