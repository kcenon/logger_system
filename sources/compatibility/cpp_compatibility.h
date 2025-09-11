#pragma once

// C++17/C++20 compatibility layer for logger_system
// This header provides feature detection and fallback mechanisms

#include <type_traits>
#include <functional>
#include <memory>

##################################################
// C++20 Feature Detection and Fallbacks
##################################################

// std::format compatibility
#ifdef LOGGER_HAS_STD_FORMAT
    #include <format>
    namespace logger_compat {
        using std::format;
        using std::format_string;
        using std::make_format_args;
        using std::vformat;
    }
    #define LOGGER_FORMAT_AVAILABLE 1
#elif defined(LOGGER_USE_FMT)
    #include <fmt/format.h>
    #include <fmt/args.h>
    namespace logger_compat {
        using fmt::format;
        template<typename... Args>
        using format_string = fmt::format_string<Args...>;
        using fmt::make_format_args;
        using fmt::vformat;
    }
    #define LOGGER_FORMAT_AVAILABLE 1
#else
    #include <sstream>
    #include <string>
    namespace logger_compat {
        // Basic string formatting fallback
        template<typename... Args>
        std::string format(const std::string& fmt, Args&&... args) {
            // Simple fallback - just concatenate with spaces
            std::ostringstream oss;
            oss << fmt;
            ((oss << " " << args), ...);  // C++17 fold expression
            return oss.str();
        }
        
        template<typename T>
        using format_string = const T&;
    }
    #define LOGGER_FORMAT_AVAILABLE 0
#endif

// std::span compatibility
#ifdef LOGGER_HAS_STD_SPAN
    #include <span>
    namespace logger_compat {
        template<typename T, std::size_t Extent = std::dynamic_extent>
        using span = std::span<T, Extent>;
    }
    #define LOGGER_SPAN_AVAILABLE 1
#else
    #include <iterator>
    namespace logger_compat {
        // Simple span-like class for C++17
        template<typename T, std::size_t Extent = SIZE_MAX>
        class span {
        public:
            using element_type = T;
            using value_type = std::remove_cv_t<T>;
            using size_type = std::size_t;
            using pointer = T*;
            using const_pointer = const T*;
            using reference = T&;
            using const_reference = const T&;
            using iterator = T*;
            using const_iterator = const T*;
            
            constexpr span() noexcept : data_(nullptr), size_(0) {}
            constexpr span(pointer ptr, size_type count) : data_(ptr), size_(count) {}
            
            template<std::size_t N>
            constexpr span(element_type (&arr)[N]) noexcept : data_(arr), size_(N) {}
            
            template<typename Container>
            constexpr span(Container& cont) : data_(cont.data()), size_(cont.size()) {}
            
            constexpr iterator begin() const noexcept { return data_; }
            constexpr iterator end() const noexcept { return data_ + size_; }
            constexpr const_iterator cbegin() const noexcept { return data_; }
            constexpr const_iterator cend() const noexcept { return data_ + size_; }
            
            constexpr reference operator[](size_type idx) const { return data_[idx]; }
            constexpr reference front() const { return data_[0]; }
            constexpr reference back() const { return data_[size_ - 1]; }
            constexpr pointer data() const noexcept { return data_; }
            
            constexpr size_type size() const noexcept { return size_; }
            constexpr size_type size_bytes() const noexcept { return size_ * sizeof(element_type); }
            constexpr bool empty() const noexcept { return size_ == 0; }
            
        private:
            pointer data_;
            size_type size_;
        };
    }
    #define LOGGER_SPAN_AVAILABLE 0
#endif

// Concepts compatibility
#ifdef LOGGER_HAS_CONCEPTS
    #include <concepts>
    namespace logger_compat {
        template<typename T>
        concept Stringable = requires(T t) {
            std::to_string(t);
        } || std::convertible_to<T, std::string>;
        
        template<typename T>
        concept Loggable = requires(T t) {
            { t.to_string() } -> std::convertible_to<std::string>;
        } || Stringable<T>;
    }
    #define LOGGER_CONCEPTS_AVAILABLE 1
#else
    #include <string>
    namespace logger_compat {
        // SFINAE-based concept emulation for C++17
        template<typename T, typename = void>
        struct is_stringable : std::false_type {};
        
        template<typename T>
        struct is_stringable<T, std::void_t<decltype(std::to_string(std::declval<T>()))>>
            : std::true_type {};
            
        template<>
        struct is_stringable<std::string> : std::true_type {};
        
        template<>
        struct is_stringable<const char*> : std::true_type {};
        
        template<typename T, typename = void>
        struct is_loggable : std::false_type {};
        
        template<typename T>
        struct is_loggable<T, std::void_t<decltype(std::declval<T>().to_string())>>
            : std::true_type {};
            
        template<typename T>
        struct is_loggable<T, std::enable_if_t<is_stringable<T>::value && 
                                               !std::is_same_v<decltype(std::declval<T>().to_string()), void>>>
            : std::true_type {};
        
        template<typename T>
        inline constexpr bool Stringable = is_stringable<T>::value;
        
        template<typename T>
        inline constexpr bool Loggable = is_loggable<T>::value || is_stringable<T>::value;
    }
    #define LOGGER_CONCEPTS_AVAILABLE 0
#endif

##################################################
// Utility Macros for Feature-Conditional Code
##################################################

#ifdef LOGGER_CPP17_MODE
    #define LOGGER_IF_CPP20(code) 
    #define LOGGER_IF_CPP17(code) code
    #define LOGGER_CONSTEXPR_CPP20 
    #define LOGGER_CONSTEVAL_CPP20 
#else
    #define LOGGER_IF_CPP20(code) code
    #define LOGGER_IF_CPP17(code)
    #define LOGGER_CONSTEXPR_CPP20 constexpr
    #define LOGGER_CONSTEVAL_CPP20 consteval
#endif

// Template parameter constraint macros
#ifdef LOGGER_HAS_CONCEPTS
    #define LOGGER_REQUIRES(condition) requires condition
    #define LOGGER_CONCEPT_CHECK(concept_name, type) concept_name<type>
#else
    #define LOGGER_REQUIRES(condition)
    #define LOGGER_CONCEPT_CHECK(concept_name, type) std::enable_if_t<logger_compat::concept_name<type>>* = nullptr
#endif

##################################################
// Performance Optimization Compatibility
##################################################

// Branch prediction hints
#if defined(__GNUC__) || defined(__clang__)
    #define LOGGER_LIKELY(x)   __builtin_expect(!!(x), 1)
    #define LOGGER_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define LOGGER_LIKELY(x)   (x)
    #define LOGGER_UNLIKELY(x) (x)
#endif

// Force inline
#if defined(_MSC_VER)
    #define LOGGER_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define LOGGER_FORCE_INLINE __attribute__((always_inline)) inline
#else
    #define LOGGER_FORCE_INLINE inline
#endif

// No discard compatibility
#ifdef __has_cpp_attribute
    #if __has_cpp_attribute(nodiscard) >= 201603L
        #define LOGGER_NODISCARD [[nodiscard]]
    #else
        #define LOGGER_NODISCARD
    #endif
#else
    #define LOGGER_NODISCARD
#endif