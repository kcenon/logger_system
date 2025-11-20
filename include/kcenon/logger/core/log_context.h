// BSD 3-Clause License

#pragma once

#include <chrono>
#include <string_view>
#include <thread>

namespace kcenon::logger::core {

struct log_context {
    std::string_view file{"unknown"};
    int line{0};
    std::string_view function{"unknown"};
    std::thread::id thread_id{std::this_thread::get_id()};
    std::chrono::time_point<std::chrono::system_clock> timestamp{
        std::chrono::system_clock::now()};
};

inline log_context make_log_context(std::string_view file,
                                    int line,
                                    std::string_view function) {
    log_context ctx;
    ctx.file = file;
    ctx.line = line;
    ctx.function = function;
    ctx.thread_id = std::this_thread::get_id();
    ctx.timestamp = std::chrono::system_clock::now();
    return ctx;
}

} // namespace kcenon::logger::core
