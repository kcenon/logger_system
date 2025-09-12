#pragma once

// C++17/C++20 compatibility layer for logger_system
// This header provides feature detection and fallback mechanisms

#include <type_traits>
#include <functional>
#include <memory>

##################################################
// Runtime C++20 Feature Detection and Fallbacks
##################################################

// Feature detection macros - check availability at compile time
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    #define LOGGER_HAS_STD_FORMAT_BUILTIN 1
#else
    #define LOGGER_HAS_STD_FORMAT_BUILTIN 0
#endif

#if defined(__cpp_concepts) && __cpp_concepts >= 201907L
    #define LOGGER_HAS_CONCEPTS_BUILTIN 1
#else
    #define LOGGER_HAS_CONCEPTS_BUILTIN 0
#endif

#if defined(__cpp_lib_span) && __cpp_lib_span >= 202002L
    #define LOGGER_HAS_SPAN_BUILTIN 1
#else
    #define LOGGER_HAS_SPAN_BUILTIN 0
#endif

// std::format compatibility - use C++20 if available, fallback to fmt or basic
#if LOGGER_HAS_STD_FORMAT_BUILTIN && !defined(LOGGER_FORCE_CPP17_FORMAT)
    #include <format>
    namespace logger_compat {
        using std::format;
        using std::format_string;
        using std::make_format_args;
        using std::vformat;
    }
    #define LOGGER_FORMAT_AVAILABLE 1
    #define LOGGER_USING_STD_FORMAT 1
#elif defined(USE_FMT) || defined(LOGGER_USE_FMT)
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
    #define LOGGER_USING_FMT 1
#else
    #include <sstream>
    #include <string>
    namespace logger_compat {
        // Basic string formatting fallback
        template<typename... Args>
        std::string format(const std::string& fmt, Args&&... args) {
            std::ostringstream oss;
            oss << fmt;
            ((oss << " " << args), ...);  // C++17 fold expression
            return oss.str();
        }
        
        template<typename T>
        using format_string = const T&;
        
        template<typename... Args>
        auto make_format_args(Args&&... args) {
            return std::make_tuple(std::forward<Args>(args)...);
        }
        
        template<typename... Args>
        std::string vformat(const std::string& fmt, const std::tuple<Args...>& args) {
            return std::apply([&fmt](auto&&... a) { return format(fmt, a...); }, args);
        }
    }
    #define LOGGER_FORMAT_AVAILABLE 0
    #define LOGGER_USING_BASIC_FORMAT 1
#endif

// std::span compatibility - use C++20 if available, fallback to custom implementation
#if LOGGER_HAS_SPAN_BUILTIN && !defined(LOGGER_FORCE_CPP17_SPAN)
    #include <span>
    namespace logger_compat {
        template<typename T, std::size_t Extent = std::dynamic_extent>
        using span = std::span<T, Extent>;
    }
    #define LOGGER_SPAN_AVAILABLE 1
    #define LOGGER_USING_STD_SPAN 1
#else
    #include <iterator>
    namespace logger_compat {
        // Simple span-like class for C++17 fallback
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
    #define LOGGER_USING_CUSTOM_SPAN 1
#endif

// Concepts compatibility - use C++20 if available, fallback to SFINAE
#if LOGGER_HAS_CONCEPTS_BUILTIN && !defined(LOGGER_FORCE_CPP17_CONCEPTS)
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
        
        template<typename T>
        concept Formattable = requires(T t) {
            std::formatter<T>{};
        };
    }
    #define LOGGER_CONCEPTS_AVAILABLE 1
    #define LOGGER_USING_STD_CONCEPTS 1
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
        
        template<typename T, typename = void>
        struct is_formattable : std::false_type {};
        
        template<typename T>
        struct is_formattable<T, std::void_t<decltype(std::formatter<T>{})>>
            : std::true_type {};
        
        template<typename T>
        inline constexpr bool Stringable = is_stringable<T>::value;
        
        template<typename T>
        inline constexpr bool Loggable = is_loggable<T>::value || is_stringable<T>::value;
        
        template<typename T>
        inline constexpr bool Formattable = is_formattable<T>::value;
    }
    #define LOGGER_CONCEPTS_AVAILABLE 0
    #define LOGGER_USING_SFINAE_CONCEPTS 1
#endif

##################################################
// Feature-Specific Utility Macros
##################################################

// Feature availability checks
#define LOGGER_HAS_FEATURE(feature_name) LOGGER_HAS_##feature_name##_BUILTIN

// Conditional compilation based on individual features
#define LOGGER_IF_FORMAT(code) \
    do { if constexpr(LOGGER_HAS_STD_FORMAT_BUILTIN) { code } } while(0)

#define LOGGER_IF_SPAN(code) \
    do { if constexpr(LOGGER_HAS_SPAN_BUILTIN) { code } } while(0)

#define LOGGER_IF_CONCEPTS(code) \
    do { if constexpr(LOGGER_HAS_CONCEPTS_BUILTIN) { code } } while(0)

// Legacy mode macros (backward compatibility)
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

// Template parameter constraint macros - use C++20 concepts if available
#if LOGGER_HAS_CONCEPTS_BUILTIN && !defined(LOGGER_FORCE_CPP17_CONCEPTS)
    #define LOGGER_REQUIRES(condition) requires condition
    #define LOGGER_CONCEPT_CHECK(concept_name, type) concept_name<type>
    #define LOGGER_ENABLE_IF_CONCEPT(concept_name, type)
#else
    #define LOGGER_REQUIRES(condition)
    #define LOGGER_CONCEPT_CHECK(concept_name, type)
    #define LOGGER_ENABLE_IF_CONCEPT(concept_name, type) std::enable_if_t<logger_compat::concept_name<type>>* = nullptr
#endif

// Format string macros - adapt based on available implementation
#if LOGGER_HAS_STD_FORMAT_BUILTIN && !defined(LOGGER_FORCE_CPP17_FORMAT)
    #define LOGGER_FORMAT(fmt, ...) logger_compat::format(fmt, __VA_ARGS__)
    #define LOGGER_FORMAT_STRING(str) logger_compat::format_string<decltype(__VA_ARGS__)...>{str}
#else
    #define LOGGER_FORMAT(fmt, ...) logger_compat::format(fmt, __VA_ARGS__)
    #define LOGGER_FORMAT_STRING(str) str
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