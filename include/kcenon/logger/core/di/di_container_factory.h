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
 * @file di_container_factory.h
 * @brief Factory for creating DI container instances
 */

#pragma once

#include <kcenon/logger/core/di/di_container_interface.h>
#include <memory>

namespace kcenon::logger::di {

/**
 * @brief Factory class for creating DI container instances
 */
class di_container_factory {
public:
    /**
     * @brief Container type enumeration
     */
    enum class container_type {
        automatic,
        basic,
        advanced
    };
    /**
     * @brief Create a default DI container instance
     * @return Shared pointer to DI container interface
     */
    static std::shared_ptr<di_container_interface> create_default() {
        return std::make_shared<basic_di_container>();
    }

    /**
     * @brief Create a DI container with specific configuration
     * @param config Configuration parameters for the container
     * @return Shared pointer to DI container interface
     */
    template<typename ConfigType>
    static std::shared_ptr<di_container_interface> create_with_config(const ConfigType& config) {
        (void)config; // Suppress unused parameter warning
        return create_default();
    }

    /**
     * @brief Get the global DI container instance
     * @return Reference to the global DI container
     */
    static di_container_interface& get_global_container() {
        static basic_di_container global_instance;
        return global_instance;
    }
};

} // namespace kcenon::logger::di