/**
 * @file di_container_interface.h
 * @brief Dependency injection container interface
 */

#pragma once

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
    }
};

} // namespace kcenon::logger::di