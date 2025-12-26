/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
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

/**
 * @file executor_integration.cpp
 * @brief Implementation of IExecutor-based integration
 * @author kcenon
 * @since 1.5.0
 */

#include <kcenon/logger/integration/executor_integration.h>

#if LOGGER_HAS_IEXECUTOR

namespace kcenon::logger::integration {

// Static member initialization
std::atomic<executor_type> executor_integration::current_type_{executor_type::none};
std::shared_ptr<common::interfaces::IExecutor> executor_integration::executor_{nullptr};
std::mutex executor_integration::executor_mutex_{};

void executor_integration::enable(std::shared_ptr<common::interfaces::IExecutor> executor) {
    std::lock_guard<std::mutex> lock(executor_mutex_);

    if (executor) {
        executor_ = std::move(executor);
        current_type_.store(executor_type::external, std::memory_order_release);
    } else if (!executor_) {
        // Create default standalone executor if none exists
        executor_ = create_default_executor();
        if (executor_) {
            current_type_.store(executor_type::standalone, std::memory_order_release);
        }
    } else {
        // Re-enable existing executor
        if (auto* standalone = dynamic_cast<standalone_executor*>(executor_.get())) {
            current_type_.store(executor_type::standalone, std::memory_order_release);
        } else {
            current_type_.store(executor_type::external, std::memory_order_release);
        }
    }
}

void executor_integration::disable() {
    std::lock_guard<std::mutex> lock(executor_mutex_);

    current_type_.store(executor_type::none, std::memory_order_release);
    // Note: We don't shut down the executor here - let it be managed by its owner
    // Just release our reference
    executor_.reset();
}

bool executor_integration::is_enabled() noexcept {
    return current_type_.load(std::memory_order_acquire) != executor_type::none;
}

executor_type executor_integration::get_executor_type() noexcept {
    return current_type_.load(std::memory_order_acquire);
}

void executor_integration::set_executor(std::shared_ptr<common::interfaces::IExecutor> executor) {
    std::lock_guard<std::mutex> lock(executor_mutex_);

    if (executor) {
        executor_ = std::move(executor);
        if (auto* standalone = dynamic_cast<standalone_executor*>(executor_.get())) {
            current_type_.store(executor_type::standalone, std::memory_order_release);
        } else {
            current_type_.store(executor_type::external, std::memory_order_release);
        }
    } else {
        executor_.reset();
        current_type_.store(executor_type::none, std::memory_order_release);
    }
}

std::shared_ptr<common::interfaces::IExecutor> executor_integration::get_executor() noexcept {
    std::lock_guard<std::mutex> lock(executor_mutex_);
    return executor_;
}

bool executor_integration::submit_task(std::function<void()> task) {
    if (!is_enabled()) {
        return false;
    }

    std::shared_ptr<common::interfaces::IExecutor> executor;
    {
        std::lock_guard<std::mutex> lock(executor_mutex_);
        executor = executor_;
    }

    if (!executor || !executor->is_running()) {
        return false;
    }

    auto job = std::make_unique<function_job>(std::move(task));
    auto result = executor->execute(std::move(job));

    return result.is_ok();
}

bool executor_integration::submit_task_delayed(
    std::function<void()> task,
    std::chrono::milliseconds delay_ms) {

    if (!is_enabled()) {
        return false;
    }

    std::shared_ptr<common::interfaces::IExecutor> executor;
    {
        std::lock_guard<std::mutex> lock(executor_mutex_);
        executor = executor_;
    }

    if (!executor || !executor->is_running()) {
        return false;
    }

    auto job = std::make_unique<function_job>(std::move(task));
    auto result = executor->execute_delayed(std::move(job), delay_ms);

    return result.is_ok();
}

std::string executor_integration::get_executor_name() noexcept {
    switch (current_type_.load(std::memory_order_acquire)) {
        case executor_type::standalone:
            return "standalone";
        case executor_type::external:
            return "external";
        case executor_type::none:
        default:
            return "none";
    }
}

size_t executor_integration::pending_tasks() noexcept {
    std::lock_guard<std::mutex> lock(executor_mutex_);
    if (executor_) {
        return executor_->pending_tasks();
    }
    return 0;
}

size_t executor_integration::worker_count() noexcept {
    std::lock_guard<std::mutex> lock(executor_mutex_);
    if (executor_) {
        return executor_->worker_count();
    }
    return 0;
}

std::shared_ptr<common::interfaces::IExecutor> executor_integration::create_default_executor() {
    return standalone_executor_factory::create(8192, "logger_async_executor");
}

} // namespace kcenon::logger::integration

#endif // LOGGER_HAS_IEXECUTOR
