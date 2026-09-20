from pathlib import Path
import os
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]


class CMakeModuleIsolationTests(unittest.TestCase):
    def configure(self, source, build, *flags, env=None):
        result = subprocess.run(
            ["cmake", "-S", str(source), "-B", str(build), *flags],
            text=True, capture_output=True, env=env, timeout=90,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_dependency_loads_its_own_modules(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            common = root / "common_system"
            modules = common / "cmake"
            modules.mkdir(parents=True)
            (common / "include").mkdir()
            (common / "CMakeLists.txt").write_text('''
cmake_minimum_required(VERSION 3.20)
project(common_system LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(options)
include(dependencies)
''')
            (modules / "options.cmake").write_text('''
set(COMMON_OPTIONS_LOADED TRUE)
''')
            (modules / "dependencies.cmake").write_text('''
if(NOT COMMON_OPTIONS_LOADED)
    message(FATAL_ERROR "Dependency options were shadowed by the parent")
endif()
add_library(common_system INTERFACE IMPORTED GLOBAL)
add_library(kcenon::common_system ALIAS common_system)
set_target_properties(common_system PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")
''')
            env = os.environ.copy()
            env["common_system_ROOT"] = str(common)
            self.configure(
                ROOT, root / "build", "-DUNIFIED_USE_LOCAL=ON",
                "-DLOGGER_BUILD_TESTS=OFF", "-DLOGGER_BUILD_SAMPLES=OFF",
                "-DLOGGER_BUILD_INTEGRATION_TESTS=OFF", "-DBUILD_BENCHMARKS=OFF",
                "-DLOGGER_USE_ENCRYPTION=OFF", env=env,
            )

    def test_dependency_modes_preserve_requested_logger_tests(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            helper = (ROOT / "cmake/UnifiedDependencies.cmake").as_posix()
            (root / "CMakeLists.txt").write_text('''
cmake_minimum_required(VERSION 3.20)
project(dependency_options NONE)
include("''' + helper + '''")
unified_setup_dependency_mode()
foreach(flag BUILD_TESTS BUILD_INTEGRATION_TESTS BUILD_TESTING
             LOGGER_BUILD_TESTS LOGGER_BUILD_INTEGRATION_TESTS)
    if(NOT ${flag})
        message(FATAL_ERROR "Dependency setup disabled caller option ${flag}")
    endif()
endforeach()
''')
            for mode in ("LOCAL", "FETCHCONTENT"):
                with self.subTest(mode=mode):
                    self.configure(
                        root, root / mode, f"-DUNIFIED_USE_{mode}=ON",
                        "-DBUILD_TESTS=ON", "-DBUILD_INTEGRATION_TESTS=ON",
                        "-DBUILD_TESTING=ON", "-DLOGGER_BUILD_TESTS=ON",
                        "-DLOGGER_BUILD_INTEGRATION_TESTS=ON",
                    )


if __name__ == "__main__":
    unittest.main()
