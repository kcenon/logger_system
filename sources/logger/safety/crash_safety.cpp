/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "../../interfaces/logger_crash_safety.h"
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>

namespace logger_module {

static logger_crash_safety* g_instance = nullptr;
static std::mutex g_instance_mutex;

logger_crash_safety& logger_crash_safety::instance() {
    std::lock_guard<std::mutex> lock(g_instance_mutex);
    if (!g_instance) {
        g_instance = new logger_crash_safety();
    }
    return *g_instance;
}

logger_crash_safety::~logger_crash_safety() {
    if (backup_thread_running_) {
        stop_backup_thread();
    }
    
    if (emergency_log_fd_ != -1) {
        close(emergency_log_fd_);
        emergency_log_fd_ = -1;
    }
    
    #ifndef _WIN32
    for (int sig : {SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS}) {
        sigaction(sig, &previous_handlers_[sig], nullptr);
    }
    #endif
}

void logger_crash_safety::initialize(logger_crash_safety_level level,
                                    const std::string& emergency_log_path,
                                    uint32_t backup_interval_ms) {
    if (initialized_.exchange(true)) {
        return;
    }
    
    safety_level_ = level;
    emergency_log_path_ = emergency_log_path;
    backup_interval_ms_ = backup_interval_ms;
    
    emergency_log_fd_ = open(emergency_log_path_.c_str(), 
                            O_WRONLY | O_CREAT | O_APPEND, 0644);
    
    if (emergency_log_fd_ == -1) {
        emergency_log_fd_ = STDERR_FILENO;
    }
    
    #ifndef _WIN32
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &logger_crash_safety::signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    for (int sig : {SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS}) {
        sigaction(sig, nullptr, &previous_handlers_[sig]);
        sigaction(sig, &sa, nullptr);
    }
    #endif
    
    if (safety_level_ >= logger_crash_safety_level::standard && auto_backup_enabled_) {
        start_backup_thread();
    }
    
    check_and_recover();
}

void logger_crash_safety::register_logger(const std::string& logger_name,
                                         std::function<void()> flush_callback,
                                         std::function<void(const std::string&)> backup_callback) {
    std::lock_guard<std::mutex> lock(loggers_mutex_);
    
    auto it = std::find_if(loggers_.begin(), loggers_.end(),
                           [&logger_name](const logger_entry& entry) {
                               return entry.name == logger_name;
                           });
    
    if (it != loggers_.end()) {
        it->flush_callback = flush_callback;
        it->backup_callback = backup_callback;
    } else {
        loggers_.push_back({logger_name, flush_callback, backup_callback});
    }
}

void logger_crash_safety::unregister_logger(const std::string& logger_name) {
    std::lock_guard<std::mutex> lock(loggers_mutex_);
    loggers_.erase(std::remove_if(loggers_.begin(), loggers_.end(),
                                  [&logger_name](const logger_entry& entry) {
                                      return entry.name == logger_name;
                                  }),
                   loggers_.end());
}

void logger_crash_safety::emergency_log(const std::string& level, const std::string& message) {
    emergency_log_entry entry;
    entry.timestamp = std::chrono::system_clock::now();
    entry.level = level;
    entry.message = message;
    
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    entry.thread_id = oss.str();
    
    {
        std::lock_guard<std::mutex> lock(emergency_log_mutex_);
        emergency_log_queue_.push(entry);
        
        while (emergency_log_queue_.size() > max_emergency_entries_) {
            emergency_log_queue_.pop();
        }
    }
    
    write_emergency_log_entry(entry);
    total_emergency_logs_++;
}

void logger_crash_safety::set_emergency_log_path(const std::string& path) {
    if (emergency_log_fd_ != -1 && emergency_log_fd_ != STDERR_FILENO) {
        close(emergency_log_fd_);
    }
    
    emergency_log_path_ = path;
    emergency_log_fd_ = open(emergency_log_path_.c_str(), 
                            O_WRONLY | O_CREAT | O_APPEND, 0644);
    
    if (emergency_log_fd_ == -1) {
        emergency_log_fd_ = STDERR_FILENO;
    }
}

void logger_crash_safety::set_auto_backup(bool enable, uint32_t interval_ms) {
    auto_backup_enabled_ = enable;
    backup_interval_ms_ = interval_ms;
    
    if (enable && !backup_thread_running_) {
        start_backup_thread();
    } else if (!enable && backup_thread_running_) {
        stop_backup_thread();
    }
}

void logger_crash_safety::force_flush_all() {
    std::vector<logger_entry> loggers_copy;
    {
        std::lock_guard<std::mutex> lock(loggers_mutex_);
        loggers_copy = loggers_;
    }
    
    for (const auto& logger : loggers_copy) {
        try {
            if (logger.flush_callback) {
                logger.flush_callback();
                successful_flushes_++;
            }
        } catch (...) {
            failed_flushes_++;
        }
    }
}

void logger_crash_safety::force_backup_all() {
    std::vector<logger_entry> loggers_copy;
    {
        std::lock_guard<std::mutex> lock(loggers_mutex_);
        loggers_copy = loggers_;
    }
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream backup_dir;
    backup_dir << "backup_" << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    
    for (const auto& logger : loggers_copy) {
        if (logger.backup_callback) {
            try {
                logger.backup_callback(backup_dir.str());
                backup_count_++;
            } catch (...) {
            }
        }
    }
}

bool logger_crash_safety::is_handling_crash() const {
    return handling_crash_.load();
}

void logger_crash_safety::set_max_emergency_entries(size_t max_entries) {
    max_emergency_entries_ = max_entries;
}

logger_crash_safety::emergency_log_stats logger_crash_safety::get_stats() const {
    emergency_log_stats stats;
    stats.total_emergency_logs = total_emergency_logs_.load();
    stats.successful_flushes = successful_flushes_.load();
    stats.failed_flushes = failed_flushes_.load();
    stats.backup_count = backup_count_.load();
    stats.last_emergency_time = last_emergency_time_;
    return stats;
}

bool logger_crash_safety::check_and_recover() {
    std::string recovery_marker = emergency_log_path_ + ".recovery";
    
    if (std::filesystem::exists(recovery_marker)) {
        emergency_log("WARNING", "Previous crash detected, performing recovery");
        
        std::string backup_path = emergency_log_path_ + ".recovered";
        if (std::filesystem::exists(emergency_log_path_)) {
            std::filesystem::rename(emergency_log_path_, backup_path);
        }
        
        std::filesystem::remove(recovery_marker);
        return true;
    }
    
    std::ofstream marker(recovery_marker);
    marker << "Recovery marker\n";
    
    return false;
}

void logger_crash_safety::signal_handler(int signal) {
    if (g_instance && !g_instance->handling_crash_.exchange(true)) {
        g_instance->handle_logger_crash(signal);
    }
    
    #ifndef _WIN32
    if (g_instance && g_instance->previous_handlers_[signal].sa_handler != SIG_DFL &&
        g_instance->previous_handlers_[signal].sa_handler != SIG_IGN) {
        g_instance->previous_handlers_[signal].sa_handler(signal);
    }
    #endif
    
    _exit(128 + signal);
}

void logger_crash_safety::handle_logger_crash(int signal) {
    last_emergency_time_ = std::chrono::system_clock::now();
    
    const char* signal_name = "UNKNOWN";
    switch (signal) {
        case SIGSEGV: signal_name = "SIGSEGV"; break;
        case SIGABRT: signal_name = "SIGABRT"; break;
        case SIGFPE:  signal_name = "SIGFPE"; break;
        #ifndef _WIN32
        case SIGILL:  signal_name = "SIGILL"; break;
        case SIGBUS:  signal_name = "SIGBUS"; break;
        #endif
    }
    
    signal_safe_emergency_log("FATAL", signal_name);
    
    if (safety_level_ >= logger_crash_safety_level::standard) {
        flush_all_loggers();
    }
    
    if (safety_level_ == logger_crash_safety_level::paranoid) {
        backup_all_loggers();
    }
    
    std::string recovery_marker = emergency_log_path_ + ".recovery";
    int fd = open(recovery_marker.c_str(), O_CREAT | O_WRONLY, 0644);
    if (fd != -1) {
        close(fd);
    }
}

void logger_crash_safety::flush_all_loggers() {
    for (const auto& logger : loggers_) {
        if (logger.flush_callback) {
            try {
                logger.flush_callback();
                successful_flushes_++;
            } catch (...) {
                failed_flushes_++;
            }
        }
    }
}

void logger_crash_safety::backup_all_loggers() {
    for (const auto& logger : loggers_) {
        if (logger.backup_callback) {
            try {
                logger.backup_callback("crash_backup");
                backup_count_++;
            } catch (...) {
            }
        }
    }
}

void logger_crash_safety::write_emergency_log_entry(const emergency_log_entry& entry) {
    auto time_t = std::chrono::system_clock::to_time_t(entry.timestamp);
    
    std::ostringstream oss;
    oss << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
        << "] [" << entry.level << "] "
        << "[TID:" << entry.thread_id << "] ";
    
    if (entry.signal_number != 0) {
        oss << "[SIG:" << entry.signal_number << "] ";
    }
    
    oss << entry.message << "\n";
    
    std::string log_line = oss.str();
    signal_safe_write(log_line.c_str(), log_line.length());
}

void logger_crash_safety::cleanup_old_emergency_logs() {
    
}

void logger_crash_safety::start_backup_thread() {
    if (backup_thread_running_.exchange(true)) {
        return;
    }
    
    backup_thread_ = std::make_unique<std::thread>(&logger_crash_safety::backup_thread_worker, this);
}

void logger_crash_safety::stop_backup_thread() {
    backup_thread_running_ = false;
    
    if (backup_thread_ && backup_thread_->joinable()) {
        backup_thread_->join();
    }
}

void logger_crash_safety::backup_thread_worker() {
    while (backup_thread_running_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(backup_interval_ms_));
        
        if (!backup_thread_running_) {
            break;
        }
        
        if (safety_level_ == logger_crash_safety_level::paranoid) {
            force_backup_all();
        }
    }
}

void logger_crash_safety::signal_safe_write(const char* data, size_t length) {
    if (emergency_log_fd_ != -1) {
        size_t written = 0;
        while (written < length) {
            ssize_t result = write(emergency_log_fd_, data + written, length - written);
            if (result > 0) {
                written += result;
            } else if (result == -1 && errno != EINTR) {
                break;
            }
        }
    }
}

void logger_crash_safety::signal_safe_emergency_log(const char* level, const char* message) {
    char buffer[512];
    
    time_t now = time(nullptr);
    struct tm* tm_info = localtime(&now);
    
    int len = snprintf(buffer, sizeof(buffer),
                      "[%04d-%02d-%02d %02d:%02d:%02d] [%s] %s\n",
                      tm_info->tm_year + 1900,
                      tm_info->tm_mon + 1,
                      tm_info->tm_mday,
                      tm_info->tm_hour,
                      tm_info->tm_min,
                      tm_info->tm_sec,
                      level,
                      message);
    
    if (len > 0 && static_cast<size_t>(len) < sizeof(buffer)) {
        signal_safe_write(buffer, len);
    }
}

bool log_file_recovery::is_corrupted(const std::string& file_path) {
    if (!std::filesystem::exists(file_path)) {
        return false;
    }
    
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file) {
        return true;
    }
    
    std::streamsize size = file.tellg();
    if (size == 0) {
        return false;
    }
    
    file.seekg(-1, std::ios::end);
    char last_char;
    file.get(last_char);
    
    return last_char != '\n';
}

bool log_file_recovery::recover_file(const std::string& file_path, const std::string& recovery_path) {
    if (!std::filesystem::exists(file_path)) {
        return false;
    }
    
    if (!is_corrupted(file_path)) {
        return true;
    }
    
    std::ifstream source(file_path, std::ios::binary);
    std::ofstream dest(recovery_path, std::ios::binary);
    
    if (!source || !dest) {
        return false;
    }
    
    std::string line;
    while (std::getline(source, line)) {
        if (!line.empty()) {
            dest << line << '\n';
        }
    }
    
    return dest.good();
}

bool log_file_recovery::create_backup_with_checksum(const std::string& source_path,
                                                   const std::string& backup_path) {
    try {
        std::filesystem::copy_file(source_path, backup_path,
                                   std::filesystem::copy_options::overwrite_existing);
        
        std::string checksum = calculate_checksum(backup_path);
        std::string checksum_path = backup_path + ".sha256";
        
        return write_checksum(checksum, checksum_path);
    } catch (...) {
        return false;
    }
}

bool log_file_recovery::verify_integrity(const std::string& file_path, const std::string& checksum_path) {
    std::string calculated = calculate_checksum(file_path);
    std::string stored = read_checksum(checksum_path);
    
    return !calculated.empty() && calculated == stored;
}

std::string log_file_recovery::calculate_checksum(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        return "";
    }
    
    uint32_t hash = 0;
    char buffer[4096];
    
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        for (std::streamsize i = 0; i < file.gcount(); ++i) {
            hash = hash * 31 + static_cast<unsigned char>(buffer[i]);
        }
    }
    
    std::ostringstream oss;
    oss << std::hex << hash;
    return oss.str();
}

bool log_file_recovery::write_checksum(const std::string& checksum, const std::string& checksum_path) {
    std::ofstream file(checksum_path);
    if (!file) {
        return false;
    }
    
    file << checksum;
    return file.good();
}

std::string log_file_recovery::read_checksum(const std::string& checksum_path) {
    std::ifstream file(checksum_path);
    if (!file) {
        return "";
    }
    
    std::string checksum;
    file >> checksum;
    return checksum;
}

void async_logger_crash_safety::configure_async_safety(const std::string& /* logger_name */,
                                                      uint32_t /* flush_timeout_ms */,
                                                      bool /* emergency_sync_mode */) {
    
}

void async_logger_crash_safety::set_overflow_handler(const std::string& /* logger_name */,
                                                    std::function<void(size_t dropped_messages)> /* overflow_callback */) {
    
}

void async_logger_crash_safety::handle_async_crash(const std::string& /* logger_name */) {
    
}

} // namespace logger_module