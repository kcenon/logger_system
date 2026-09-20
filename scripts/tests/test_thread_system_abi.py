from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]


class ThreadSystemAbiTests(unittest.TestCase):
    def run_command(self, *args):
        result = subprocess.run(args, text=True, capture_output=True, timeout=90)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_source_library_and_consumer_agree_on_public_layout(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            dependency = root / "dependency"
            dependency.mkdir()
            (dependency / "abi.h").write_text('''
#include <cstddef>
struct thread_state {
#ifdef USE_STD_JTHREAD
    char stop_state[64];
#endif
    char state;
};
std::size_t library_size();
''')
            (dependency / "abi.cpp").write_text('''
#include "abi.h"
std::size_t library_size() { return sizeof(thread_state); }
''')
            (dependency / "CMakeLists.txt").write_text('''
if(ENABLE_JTHREAD)
    add_compile_definitions(USE_STD_JTHREAD)
endif()
add_library(thread_core STATIC abi.cpp)
target_include_directories(thread_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(ThreadSystem::Core ALIAS thread_core)
''')
            (root / "consumer.cpp").write_text('''
#include "abi.h"
int main() { return library_size() == sizeof(thread_state) ? 0 : 1; }
''')
            helper = (ROOT / "cmake/thread_system_compat.cmake").as_posix()
            (root / "CMakeLists.txt").write_text('''
cmake_minimum_required(VERSION 3.20)
project(thread_abi LANGUAGES CXX)
add_subdirectory(dependency)
include("''' + helper + '''")
logger_propagate_thread_system_abi(ThreadSystem::Core)
add_executable(consumer consumer.cpp)
target_link_libraries(consumer PRIVATE ThreadSystem::Core)
enable_testing()
add_test(NAME consumer_layout COMMAND consumer)
''')
            for enabled in ("ON", "OFF"):
                with self.subTest(jthread=enabled):
                    build = root / enabled
                    self.run_command("cmake", "-S", str(root), "-B", str(build),
                                     f"-DENABLE_JTHREAD={enabled}")
                    self.run_command("cmake", "--build", str(build))
                    self.run_command("ctest", "--test-dir", str(build),
                                     "--output-on-failure")


if __name__ == "__main__":
    unittest.main()
