/**
 * @file mock_di_container.hpp
 * @brief Mock DI container implementation for testing
 * @date 2025-09-09
 *
 * BSD 3-Clause License
 * Copyright (c) 2025, kcenon
 * All rights reserved.
 */

#pragma once

#include "../../sources/logger/di/di_container_interface.h"
#include "mock_writer.hpp"
#include <unordered_map>
#include <functional>
#include <atomic>

namespace logger_system::testing {

/**
 * @brief Mock DI container for unit testing
 * 
 * Provides controllable dependency injection behavior for testing
 * DI integration and component resolution scenarios.
 */
class mock_di_container : public di_container_interface<base_writer> {
private:
    using factory_func = std::function<std::shared_ptr<base_writer>()>;
    
    mutable std::mutex mutex_;
    std::unordered_map<std::string, factory_func> factories_;
    std::unordered_map<std::string, std::shared_ptr<base_writer>> singletons_;
    mutable std::unordered_map<std::string, size_t> resolution_counts_;
    std::atomic<bool> should_fail_{false};
    std::atomic<bool> use_singletons_{false};
    error_code failure_error_{error_code::component_not_found};

public:
    mock_di_container() = default;
    ~mock_di_container() override = default;

    // di_container_interface implementation
    result<std::shared_ptr<base_writer>> resolve(const std::string& name) override {
        std::lock_guard<std::mutex> lock(mutex_);
        
        resolution_counts_[name]++;

        if (should_fail_.load()) {
            return failure_error_;
        }

        // Check singletons first if enabled
        if (use_singletons_.load()) {
            auto singleton_it = singletons_.find(name);
            if (singleton_it != singletons_.end()) {
                return singleton_it->second;
            }
        }

        // Try factory
        auto factory_it = factories_.find(name);
        if (factory_it == factories_.end()) {
            return error_code::component_not_found;
        }

        auto instance = factory_it->second();
        
        // Store as singleton if enabled
        if (use_singletons_.load()) {
            singletons_[name] = instance;
        }

        return instance;
    }

    result_void register_factory(const std::string& name,
                                std::function<std::shared_ptr<base_writer>()> factory) override {
        if (should_fail_.load()) {
            return failure_error_;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        factories_[name] = factory;
        return {};
    }

    // Mock-specific methods
    result_void register_instance(const std::string& name,
                                 std::shared_ptr<base_writer> instance) {
        if (should_fail_.load()) {
            return failure_error_;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        singletons_[name] = instance;
        return {};
    }

    void set_should_fail(bool fail, error_code error = error_code::component_not_found) {
        should_fail_.store(fail);
        failure_error_ = error;
    }

    void enable_singletons(bool enable = true) {
        use_singletons_.store(enable);
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        factories_.clear();
        singletons_.clear();
        resolution_counts_.clear();
    }

    void reset() {
        clear();
        should_fail_.store(false);
        use_singletons_.store(false);
    }

    // Inspection methods
    size_t get_resolution_count(const std::string& name) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = resolution_counts_.find(name);
        return it != resolution_counts_.end() ? it->second : 0;
    }

    size_t get_total_resolution_count() const {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t total = 0;
        for (const auto& [name, count] : resolution_counts_) {
            total += count;
        }
        return total;
    }

    bool has_factory(const std::string& name) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return factories_.find(name) != factories_.end();
    }

    bool has_singleton(const std::string& name) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return singletons_.find(name) != singletons_.end();
    }

    size_t factory_count() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return factories_.size();
    }

    size_t singleton_count() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return singletons_.size();
    }
};

/**
 * @brief Test helper for setting up mock DI scenarios
 */
class mock_di_scenario {
private:
    std::shared_ptr<mock_di_container> container_;
    std::shared_ptr<mock_writer_factory> writer_factory_;

public:
    mock_di_scenario()
        : container_(std::make_shared<mock_di_container>())
        , writer_factory_(std::make_shared<mock_writer_factory>()) {}

    std::shared_ptr<mock_di_container> get_container() const {
        return container_;
    }

    std::shared_ptr<mock_writer_factory> get_writer_factory() const {
        return writer_factory_;
    }

    void setup_default_writers() {
        // Register common writer types
        container_->register_factory("console", [this]() {
            return writer_factory_->create_writer();
        });

        container_->register_factory("file", [this]() {
            return writer_factory_->create_writer();
        });

        container_->register_factory("async", [this]() {
            return writer_factory_->create_writer();
        });
    }

    void setup_failing_writer(const std::string& name) {
        container_->register_factory(name, [this]() {
            auto writer = writer_factory_->create_writer();
            writer->set_should_fail(true);
            return writer;
        });
    }

    void setup_slow_writer(const std::string& name, std::chrono::milliseconds delay) {
        container_->register_factory(name, [this, delay]() {
            auto writer = writer_factory_->create_writer();
            writer->set_write_delay(delay);
            return writer;
        });
    }

    void reset() {
        container_->reset();
        writer_factory_->reset_all();
    }
};

} // namespace logger_system::testing