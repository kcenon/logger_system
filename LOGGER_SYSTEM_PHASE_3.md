# Phase 3: Code Quality Improvement

**Phase:** 3/5
**Priority:** 🟡 Medium
**Estimated Duration:** 4-6 weeks
**Objective:** Improve maintainability and extensibility
**Status:** ⬜ Not Started
**Dependencies:** Phase 1-2 completion recommended

---

## Table of Contents

1. [Overview](#overview)
2. [Work Items](#work-items)
3. [Detailed Improvements](#detailed-improvements)
4. [Migration Guide](#migration-guide)
5. [Validation Criteria](#validation-criteria)

---

## Overview

### Purpose
Phase 3 improves long-term maintainability through **code deduplication and architecture simplification**.

### Key Improvements
1. **Formatter Separation:** Integrate formatting logic using strategy pattern
2. **Remove Conditional Compilation:** Convert to runtime polymorphism
3. **Unify Error Handling:** Eliminate try-catch duplication
4. **Remove Code Duplication:** Extract common utilities

### Expected Benefits
- ✅ 50% reduction in code duplication
- ✅ 75% reduction in test combinations
- ✅ 50% reduction in time to add new writers
- ✅ Reduced build time

---

## Work Items

### Task 3.1: Formatter Separation
- [ ] 3.1.1 Design log_formatter_interface
- [ ] 3.1.2 Extract existing formatting logic
- [ ] 3.1.3 Implement timestamp_formatter
- [ ] 3.1.4 Implement json_formatter
- [ ] 3.1.5 Implement xml_formatter
- [ ] 3.1.6 Support custom_formatter
- [ ] 3.1.7 Integrate with base_writer
- [ ] 3.1.8 Write unit tests

### Task 3.2: Remove Conditional Compilation
- [ ] 3.2.1 Design integration_backend interface
- [ ] 3.2.2 Implement thread_system_backend
- [ ] 3.2.3 Implement standalone_backend
- [ ] 3.2.4 Integrate with logger
- [ ] 3.2.5 Simplify tests
- [ ] 3.2.6 Update build scripts

### Task 3.3: Unify Error Handling
- [ ] 3.3.1 Write try_operation helper function
- [ ] 3.3.2 Apply to all writers
- [ ] 3.3.3 Verify error code consistency
- [ ] 3.3.4 Update unit tests

### Task 3.4: Extract Common Utilities
- [ ] 3.4.1 file_utils (path validation, etc.)
- [ ] 3.4.2 time_utils (timestamp formatting)
- [ ] 3.4.3 string_utils (level conversion, etc.)
- [ ] 3.4.4 Write tests

---

## Detailed Improvements

### Improvement 3.1: Formatter Separation

#### Current Structure (Problem)
Every writer implements formatting logic individually

```cpp
// base_writer.h (388 lines)
class base_writer {
protected:
    std::string format_log_entry(const log_entry& entry) {
        // Duplicated formatting logic
        auto timestamp = format_timestamp(entry.timestamp);
        auto level_str = level_to_string(entry.level);
        auto color = level_to_color(entry.level);
        // ...
        return formatted;
    }

    std::string level_to_string(log_level level) { /*...*/ }
    std::string level_to_color(log_level level) { /*...*/ }
    std::string format_timestamp(...) { /*...*/ }
};
```

**Problems:**
- Identical code repeated in all writers
- Multiple locations must be modified when changing format
- Difficult to add new formats

#### Improved Structure: Strategy Pattern

```cpp
// log_formatter_interface.h

namespace kcenon::logger {

struct format_options {
    bool include_timestamp = true;
    bool include_thread_id = true;
    bool include_source_location = false;
    bool use_colors = false;
};

class log_formatter_interface {
public:
    virtual ~log_formatter_interface() = default;

    virtual std::string format(const log_entry& entry) const = 0;

    virtual void set_options(const format_options& opts) {
        options_ = opts;
    }

protected:
    format_options options_;
};

} // namespace
```

#### Concrete Implementation

```cpp
// timestamp_formatter.h - Default formatter

class timestamp_formatter : public log_formatter_interface {
public:
    std::string format(const log_entry& entry) const override {
        std::ostringstream oss;

        // Timestamp
        if (options_.include_timestamp) {
            oss << "[" << format_timestamp(entry.timestamp) << "] ";
        }

        // Level (with color)
        if (options_.use_colors) {
            oss << level_to_color(entry.level);
        }
        oss << "[" << level_to_string(entry.level) << "] ";
        if (options_.use_colors) {
            oss << "\033[0m";  // reset
        }

        // Thread ID
        if (options_.include_thread_id) {
            oss << "[thread:" << entry.thread_id << "] ";
        }

        // Message
        oss << entry.message;

        // Source location
        if (options_.include_source_location && !entry.file_path.empty()) {
            oss << " [" << entry.file_path << ":" << entry.line_number
                << " in " << entry.function_name << "()]";
        }

        return oss.str();
    }

private:
    static std::string format_timestamp(
        const std::chrono::system_clock::time_point& tp) {
        auto time_t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm;
        localtime_r(&time_t, &tm);

        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);

        // Add milliseconds
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()
        ) % 1000;

        return std::string(buffer) + "." +
               std::to_string(ms.count()).substr(0, 3);
    }

    static std::string level_to_string(log_level level) {
        switch (level) {
            case log_level::trace:    return "TRACE";
            case log_level::debug:    return "DEBUG";
            case log_level::info:     return "INFO";
            case log_level::warning:  return "WARN";
            case log_level::error:    return "ERROR";
            case log_level::fatal:    return "FATAL";
            default:                  return "UNKNOWN";
        }
    }

    static std::string level_to_color(log_level level) {
        switch (level) {
            case log_level::trace:    return "\033[37m";   // white
            case log_level::debug:    return "\033[36m";   // cyan
            case log_level::info:     return "\033[32m";   // green
            case log_level::warning:  return "\033[33m";   // yellow
            case log_level::error:    return "\033[31m";   // red
            case log_level::fatal:    return "\033[1;31m"; // bold red
            default:                  return "";
        }
    }
};
```

```cpp
// json_formatter.h

class json_formatter : public log_formatter_interface {
public:
    std::string format(const log_entry& entry) const override {
        // nlohmann/json is recommended (simple implementation shown here)
        std::ostringstream oss;

        oss << "{"
            << "\"timestamp\":\"" << format_iso8601(entry.timestamp) << "\","
            << "\"level\":\"" << level_to_string(entry.level) << "\","
            << "\"thread_id\":" << entry.thread_id << ","
            << "\"message\":\"" << escape_json(entry.message) << "\"";

        if (options_.include_source_location) {
            oss << ",\"file\":\"" << escape_json(entry.file_path) << "\""
                << ",\"line\":" << entry.line_number
                << ",\"function\":\"" << escape_json(entry.function_name) << "\"";
        }

        oss << "}";

        return oss.str();
    }

private:
    static std::string escape_json(const std::string& str) {
        std::ostringstream oss;
        for (char c : str) {
            switch (c) {
                case '"':  oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\n': oss << "\\n"; break;
                case '\t': oss << "\\t"; break;
                default:   oss << c; break;
            }
        }
        return oss.str();
    }
};
```

#### base_writer Integration

```cpp
// base_writer.h (improved)

class base_writer : public log_writer_interface {
public:
    explicit base_writer(std::unique_ptr<log_formatter_interface> formatter = nullptr)
        : formatter_(std::move(formatter)) {
        if (!formatter_) {
            // Default formatter
            formatter_ = std::make_unique<timestamp_formatter>();
        }
    }

    // Set formatter
    void set_formatter(std::unique_ptr<log_formatter_interface> formatter) {
        formatter_ = std::move(formatter);
    }

    result_void write(const log_entry& entry) override {
        try {
            auto formatted = formatter_->format(entry);
            return write_formatted(formatted, entry);
        } catch (const std::exception& e) {
            return make_logger_error(
                logger_error_code::format_failed,
                e.what()
            );
        }
    }

protected:
    // Derived classes only need to implement this
    virtual result_void write_formatted(
        const std::string& formatted,
        const log_entry& entry
    ) = 0;

private:
    std::unique_ptr<log_formatter_interface> formatter_;
};
```

**Benefits:**
- base_writer size: 388 lines → ~150 lines (60% reduction)
- Code duplication eliminated
- Easy to add new formatters

---

### Improvement 3.2: Remove Conditional Compilation

#### Current Structure (Problem)

```cpp
// thread_integration_detector.h

#ifdef USE_THREAD_SYSTEM_INTEGRATION
    #include <kcenon/thread/interfaces/logger_interface.h>
    using log_level = kcenon::thread::log_level;
#else
    #include <kcenon/logger/interfaces/logger_interface.h>
    using log_level = logger_system::log_level;
#endif

// Conversion functions also conditional
#ifdef USE_THREAD_SYSTEM_INTEGRATION
logger_system::log_level convert_log_level(kcenon::thread::log_level level) {
    // ...
}
#endif
```

**Problems:**
- Test combination explosion (2^4 = 16 combinations)
- Increased build time
- Difficult debugging
- Reduced code readability

#### Improved Structure: Runtime Polymorphism

```cpp
// integration_backend.h

namespace kcenon::logger {

enum class log_level_backend {
    trace, debug, info, warning, error, fatal
};

class integration_backend {
public:
    virtual ~integration_backend() = default;

    // Level conversion
    virtual log_level_backend normalize_level(int external_level) const = 0;

    // Metrics reporting
    virtual void report_metrics(const metrics_snapshot& metrics) const {}

    // Backend name
    virtual std::string get_backend_name() const = 0;
};

} // namespace
```

#### Concrete Implementation

```cpp
// thread_system_backend.h

#ifdef USE_THREAD_SYSTEM_INTEGRATION

class thread_system_backend : public integration_backend {
public:
    log_level_backend normalize_level(int external_level) const override {
        // thread::log_level is descending (critical=0)
        auto level = static_cast<kcenon::thread::log_level>(external_level);

        switch (level) {
            case kcenon::thread::log_level::critical:
                return log_level_backend::fatal;
            case kcenon::thread::log_level::error:
                return log_level_backend::error;
            case kcenon::thread::log_level::warn:
                return log_level_backend::warning;
            case kcenon::thread::log_level::info:
                return log_level_backend::info;
            case kcenon::thread::log_level::debug:
                return log_level_backend::debug;
            case kcenon::thread::log_level::trace:
                return log_level_backend::trace;
            default:
                return log_level_backend::info;
        }
    }

    std::string get_backend_name() const override {
        return "thread_system";
    }
};

#endif
```

```cpp
// standalone_backend.h

class standalone_backend : public integration_backend {
public:
    log_level_backend normalize_level(int external_level) const override {
        // Direct mapping (same enum)
        return static_cast<log_level_backend>(external_level);
    }

    std::string get_backend_name() const override {
        return "standalone";
    }
};
```

#### logger Integration

```cpp
// logger.h (improved)

class logger {
public:
    explicit logger(std::unique_ptr<integration_backend> backend = nullptr)
        : backend_(std::move(backend)) {
        if (!backend_) {
            backend_ = std::make_unique<standalone_backend>();
        }
    }

    void log(int external_level, const std::string& message) {
        // Convert level via backend
        auto normalized = backend_->normalize_level(external_level);

        log_entry entry;
        entry.level = normalized;
        entry.message = message;
        // ...

        enqueue(entry);
    }

private:
    std::unique_ptr<integration_backend> backend_;
};
```

#### logger_builder Integration

```cpp
// logger_builder.h

class logger_builder {
public:
    logger_builder& with_thread_system_backend() {
#ifdef USE_THREAD_SYSTEM_INTEGRATION
        backend_ = std::make_unique<thread_system_backend>();
#else
        throw std::runtime_error("thread_system not available");
#endif
        return *this;
    }

    logger_builder& with_standalone_backend() {
        backend_ = std::make_unique<standalone_backend>();
        return *this;
    }

    result<std::unique_ptr<logger>> build() {
        if (!backend_) {
            // Auto-detect
#ifdef USE_THREAD_SYSTEM_INTEGRATION
            backend_ = std::make_unique<thread_system_backend>();
#else
            backend_ = std::make_unique<standalone_backend>();
#endif
        }

        return std::make_unique<logger>(std::move(backend_));
    }

private:
    std::unique_ptr<integration_backend> backend_;
};
```

**Benefits:**
- 90% reduction in conditional compilation
- Test combinations: 16 → 4 (75% reduction)
- Runtime backend switching possible
- Improved code readability

---

### Improvement 3.3: Unify Error Handling

#### Current Structure (Problem)
Same try-catch pattern repeated in all writers

```cpp
result_void file_writer::write(...) {
    try {
        if (!file_stream_.is_open()) {
            return make_logger_error(...);
        }
        // Work
        if (condition) {
            return make_logger_error(...);
        }
        return {};
    } catch (const std::exception& e) {
        return make_logger_error(..., e.what());
    }
}
```

#### Improvement: try_operation Helper

```cpp
// error_handling_utils.h

namespace kcenon::logger::utils {

// Error handling helper
template<typename F>
result_void try_write_operation(
    F&& operation,
    logger_error_code default_error_code = logger_error_code::write_failed
) {
    try {
        return operation();
    } catch (const std::filesystem::filesystem_error& e) {
        return make_logger_error(
            logger_error_code::file_system_error,
            std::string("Filesystem error: ") + e.what()
        );
    } catch (const std::ios_base::failure& e) {
        return make_logger_error(
            logger_error_code::io_error,
            std::string("I/O error: ") + e.what()
        );
    } catch (const std::exception& e) {
        return make_logger_error(
            default_error_code,
            std::string("Unexpected error: ") + e.what()
        );
    }
}

// Condition verification helper
inline result_void check_condition(
    bool condition,
    logger_error_code error_code,
    const std::string& message
) {
    if (!condition) {
        return make_logger_error(error_code, message);
    }
    return {};
}

} // namespace
```

#### Application Example

```cpp
// file_writer.cpp (improved)

result_void file_writer::write(const log_entry& entry) {
    return utils::try_write_operation([&]() -> result_void {
        // Condition verification
        auto check = utils::check_condition(
            file_stream_.is_open(),
            logger_error_code::file_not_open,
            "File stream is not open"
        );
        if (!check) return check;

        // Actual work
        auto formatted = formatter_->format(entry);
        file_stream_ << formatted << '\n';

        // Result verification
        return utils::check_condition(
            !file_stream_.fail(),
            logger_error_code::write_failed,
            "Failed to write to file"
        );
    });
}
```

**Benefits:**
- try-catch duplication eliminated
- Improved error message consistency
- Enhanced code readability

---

### Improvement 3.4: Extract Common Utilities

```cpp
// file_utils.h

namespace kcenon::logger::utils {

class file_utils {
public:
    // Path validation
    static result_void validate_log_path(
        const std::filesystem::path& path,
        const std::filesystem::path& allowed_base
    );

    // Create directory
    static result_void ensure_directory_exists(
        const std::filesystem::path& dir
    );

    // Set file permissions
    static result_void set_file_permissions(
        const std::filesystem::path& file,
        std::filesystem::perms perms = std::filesystem::perms::owner_read |
                                        std::filesystem::perms::owner_write
    );

    // Generate safe filename
    static std::string sanitize_filename(const std::string& filename);
};

} // namespace
```

---

## Migration Guide

### Existing Code Compatibility

**Phase 3 aims to maintain backward compatibility.**

#### User Code Remains Unchanged

```cpp
// Existing code (continues to work)
auto logger = logger_builder()
    .add_writer("file", std::make_unique<file_writer>("app.log"))
    .build()
    .value();

logger->info("Hello, World!");
```

#### Utilize New Features (Optional)

```cpp
// Use custom formatter
auto json_logger = logger_builder()
    .add_writer("file",
        std::make_unique<file_writer>(
            "app.log",
            std::make_unique<json_formatter>()
        )
    )
    .build()
    .value();

// Explicit backend specification
auto thread_logger = logger_builder()
    .with_thread_system_backend()
    .add_writer("console", std::make_unique<console_writer>())
    .build()
    .value();
```

---

## Validation Criteria

| Validation Item | Criteria | Measurement Method |
|----------|------|----------|
| **Code Duplication** | 50% reduction | Static analysis |
| **Test Combinations** | 75% reduction | CI/CD build count |
| **Build Time** | No regression | Benchmark |
| **Backward Compatibility** | 100% | Run existing examples |
| **All Tests** | 100% pass | CTest |

---

## Work Checklist

### Pre-Work Preparation
- [ ] Confirm Phase 1-2 completion
- [ ] Create Git branch (`feature/phase3-code-quality`)

### During Work
- [ ] Task 3.1: Formatter separation
- [ ] Task 3.2: Remove conditional compilation
- [ ] Task 3.3: Unify error handling
- [ ] Task 3.4: Extract common utilities

### Post-Work Validation
- [ ] Confirm code duplication reduction
- [ ] Confirm test combination reduction
- [ ] Verify backward compatibility
- [ ] Complete code review

### Deployment
- [ ] Create Git tag (`v1.2.0`)
- [ ] Write Release Notes
- [ ] Prepare for Phase 4

---

## Next Steps

After Phase 3 completion:
- **Phase 4:** Security hardening (key management, path validation)
- **Documentation:** [LOGGER_SYSTEM_PHASE_4.md](./LOGGER_SYSTEM_PHASE_4.md)

---

**Start Date:** TBD
**Assignee:** TBD
**Reviewer:** TBD
