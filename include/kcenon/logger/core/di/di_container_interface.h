// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file di_container_interface.h
 * @brief Dependency injection container interface
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace kcenon::logger::di {

/**
 * @brief Interface for dependency injection container
 */
class di_container_interface {
public:
    virtual ~di_container_interface() = default;

    /**
     * @brief Register a service with the container using type erasure
     */
    virtual void register_service(std::type_index type, std::shared_ptr<void> service) = 0;

    /**
     * @brief Get a service from the container using type erasure
     */
    virtual std::shared_ptr<void> get_service(std::type_index type) = 0;

    /**
     * @brief Check if a service is registered using type erasure
     */
    virtual bool has_service(std::type_index type) const = 0;

    /**
     * @brief Clear all registered services
     */
    virtual void clear() = 0;

    /**
     * @brief Register a factory function for creating services
     */
    virtual void register_factory(const std::string& name, std::function<std::shared_ptr<void>()> factory) = 0;

    /**
     * @brief Get a service by name using factory
     */
    virtual std::shared_ptr<void> get_service(const std::string& name) = 0;

    /**
     * @brief Template wrappers for type-safe access
     */
    template<typename T>
    void register_service(std::shared_ptr<T> service) {
        register_service(std::type_index(typeid(T)), service);
    }

    template<typename T>
    std::shared_ptr<T> get_service() {
        auto result = get_service(std::type_index(typeid(T)));
        return std::static_pointer_cast<T>(result);
    }

    template<typename T>
    bool has_service() const {
        return has_service(std::type_index(typeid(T)));
    }
};

/**
 * @brief Basic DI container implementation
 */
class basic_di_container : public di_container_interface {
private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> services_;
    std::unordered_map<std::string, std::function<std::shared_ptr<void>()>> factories_;

public:
    void register_service(std::type_index type, std::shared_ptr<void> service) override {
        services_[type] = service;
    }

    std::shared_ptr<void> get_service(std::type_index type) override {
        auto it = services_.find(type);
        if (it != services_.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool has_service(std::type_index type) const override {
        return services_.find(type) != services_.end();
    }

    void clear() override {
        services_.clear();
        factories_.clear();
    }

    void register_factory(const std::string& name, std::function<std::shared_ptr<void>()> factory) override {
        factories_[name] = factory;
    }

    std::shared_ptr<void> get_service(const std::string& name) override {
        auto it = factories_.find(name);
        if (it != factories_.end()) {
            return it->second();
        }
        return nullptr;
    }
};

} // namespace kcenon::logger::di