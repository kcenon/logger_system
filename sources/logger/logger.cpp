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

#include "logger.h"
#include "core/log_collector.h"
#include "writers/base_writer.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include <mutex>

namespace logger_module {

// Implementation class
class logger::impl {
public:
    impl(bool async, std::size_t buffer_size)
        : async_(async)
        , buffer_size_(buffer_size)
        , min_level_(thread_module::log_level::trace)
        , running_(false) {
        if (async_) {
            collector_ = std::make_unique<log_collector>(buffer_size_);
        }
    }
    
    ~impl() {
        stop();
    }
    
    void log(thread_module::log_level level, const std::string& message,
             const std::string& file, int line, const std::string& function) {
        if (level > min_level_.load(std::memory_order_acquire)) {
            return;
        }
        
        auto timestamp = std::chrono::system_clock::now();
        
        if (async_ && collector_) {
            // Async logging through collector
            collector_->enqueue(level, message, file, line, function, timestamp);
        } else {
            // Sync logging directly to writers
            std::lock_guard<std::mutex> lock(writers_mutex_);
            for (auto& writer : writers_) {
                writer->write(level, message, file, line, function, timestamp);
            }
        }
    }
    
    void flush() {
        if (async_ && collector_) {
            collector_->flush();
        }
        
        std::lock_guard<std::mutex> lock(writers_mutex_);
        for (auto& writer : writers_) {
            writer->flush();
        }
    }
    
    void add_writer(std::unique_ptr<base_writer> writer) {
        std::lock_guard<std::mutex> lock(writers_mutex_);
        if (async_ && collector_) {
            collector_->add_writer(writer.get());
        }
        writers_.push_back(std::move(writer));
    }
    
    void clear_writers() {
        std::lock_guard<std::mutex> lock(writers_mutex_);
        if (async_ && collector_) {
            collector_->clear_writers();
        }
        writers_.clear();
    }
    
    void start() {
        if (async_ && collector_ && !running_.exchange(true)) {
            collector_->start();
        }
    }
    
    void stop() {
        if (running_.exchange(false)) {
            if (async_ && collector_) {
                collector_->stop();
            }
            flush();
        }
    }
    
    bool is_running() const {
        return running_.load();
    }
    
    void set_min_level(thread_module::log_level level) {
        min_level_.store(level, std::memory_order_release);
    }
    
    thread_module::log_level get_min_level() const {
        return min_level_.load(std::memory_order_acquire);
    }
    
private:
    bool async_;
    std::size_t buffer_size_;
    std::atomic<thread_module::log_level> min_level_;
    std::atomic<bool> running_;
    std::unique_ptr<log_collector> collector_;
    std::vector<std::unique_ptr<base_writer>> writers_;
    mutable std::mutex writers_mutex_;
};

// Logger implementation
logger::logger(bool async, std::size_t buffer_size)
    : pimpl_(std::make_unique<impl>(async, buffer_size)) {
}

logger::~logger() = default;

void logger::log(thread_module::log_level level, const std::string& message) {
    pimpl_->log(level, message, "", 0, "");
}

void logger::log(thread_module::log_level level, const std::string& message,
                 const std::string& file, int line, const std::string& function) {
    pimpl_->log(level, message, file, line, function);
}

bool logger::is_enabled(thread_module::log_level level) const {
    return level <= pimpl_->get_min_level();
}

void logger::flush() {
    pimpl_->flush();
}

void logger::add_writer(std::unique_ptr<base_writer> writer) {
    pimpl_->add_writer(std::move(writer));
}

void logger::clear_writers() {
    pimpl_->clear_writers();
}

void logger::set_min_level(thread_module::log_level level) {
    pimpl_->set_min_level(level);
}

thread_module::log_level logger::get_min_level() const {
    return pimpl_->get_min_level();
}

void logger::start() {
    pimpl_->start();
}

void logger::stop() {
    pimpl_->stop();
}

bool logger::is_running() const {
    return pimpl_->is_running();
}

} // namespace logger_module