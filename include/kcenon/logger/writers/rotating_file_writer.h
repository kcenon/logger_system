#pragma once

#include "base_writer.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <mutex>

namespace kcenon::logger {

struct rotation_config {
    size_t max_file_size{10 * 1024 * 1024};
    size_t max_files{5};
    bool rotate_on_startup{true};
};

class rotating_file_writer : public base_writer {
public:
    rotating_file_writer(const std::filesystem::path& base_path,
                        const rotation_config& config = {})
        : base_path_(base_path), config_(config), current_size_(0) {
        if (config_.rotate_on_startup && std::filesystem::exists(base_path_)) {
            rotate_files();
        }
        open_file();
    }

    ~rotating_file_writer() override { close_file(); }

    void write(const std::string& message) override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!file_.is_open()) open_file();
        if (current_size_ + message.size() > config_.max_file_size) {
            rotate_files();
            open_file();
        }
        file_ << message << std::flush;
        current_size_ += message.size();
    }

    void flush() override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (file_.is_open()) file_.flush();
    }

private:
    void open_file() {
        file_.open(base_path_, std::ios::app);
        if (file_.is_open()) {
            current_size_ = std::filesystem::file_size(base_path_);
        }
    }

    void close_file() {
        if (file_.is_open()) file_.close();
    }

    void rotate_files() {
        close_file();
        auto oldest = get_rotated_path(config_.max_files);
        if (std::filesystem::exists(oldest)) {
            std::filesystem::remove(oldest);
        }
        for (size_t i = config_.max_files - 1; i > 0; --i) {
            auto from = get_rotated_path(i);
            auto to = get_rotated_path(i + 1);
            if (std::filesystem::exists(from)) {
                std::filesystem::rename(from, to);
            }
        }
        if (std::filesystem::exists(base_path_)) {
            std::filesystem::rename(base_path_, get_rotated_path(1));
        }
        current_size_ = 0;
    }

    std::filesystem::path get_rotated_path(size_t index) const {
        return base_path_.string() + "." + std::to_string(index);
    }

    std::filesystem::path base_path_;
    rotation_config config_;
    std::ofstream file_;
    size_t current_size_;
    std::mutex mutex_;
};

} // namespace kcenon::logger
