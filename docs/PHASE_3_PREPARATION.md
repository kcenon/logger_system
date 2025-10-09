# Phase 3: Error Handling Preparation - logger_system

**Version**: 1.0
**Date**: 2025-10-09
**Status**: Ready for Implementation

---

## Overview

This document outlines the migration path for logger_system to adopt the centralized error handling from common_system Phase 3.

---

## Current State

### Error Handling Status

**Current Approach**:
- Mixed void returns and boolean success indicators
- File operations may throw exceptions
- Inconsistent error reporting across writers

**Example**:
```cpp
// Current: void with potential exceptions
void file_writer::write(const log_entry& entry) {
    // May throw std::ios_base::failure
    file_stream_ << format(entry);
}

// Current: boolean returns
bool logger::initialize(const config& cfg) {
    // Returns false on failure
    return setup_writers(cfg);
}
```

---

## Migration Plan

### Phase 3.1: Import Error Codes

**Action**: Add common_system error code dependency

```cpp
#include <kcenon/common/error/error_codes.h>
#include <kcenon/common/patterns/result.h>

using namespace common;
using namespace common::error;
```

### Phase 3.2: Key API Migrations

#### Priority 1: Logger Lifecycle (High Impact)

```cpp
// Before
bool initialize(const logger_config& config);
void shutdown();

// After
Result<void> initialize(const logger_config& config);
Result<void> shutdown();
```

**Error Codes**:
- `codes::logger_system::file_open_failed`
- `codes::logger_system::file_permission_denied`
- `codes::logger_system::writer_not_initialized`

**Example Implementation**:
```cpp
Result<void> logger::initialize(const logger_config& config) {
    if (initialized_) {
        return error<std::monostate>(
            codes::common::already_exists,
            "Logger already initialized",
            "logger"
        );
    }

    // Setup file writer
    auto writer_result = create_file_writer(config.log_path);
    if (writer_result.is_err()) {
        return error<std::monostate>(
            writer_result.error()
        );
    }

    writers_.push_back(std::move(writer_result.value()));
    initialized_ = true;
    return ok();
}

Result<std::unique_ptr<file_writer>> create_file_writer(
    const std::string& path
) {
    try {
        auto writer = std::make_unique<file_writer>(path);
        return ok(std::move(writer));
    } catch (const std::ios_base::failure& e) {
        return error<std::unique_ptr<file_writer>>(
            codes::logger_system::file_open_failed,
            "Failed to open log file",
            "file_writer",
            e.what()
        );
    } catch (const std::filesystem::filesystem_error& e) {
        return error<std::unique_ptr<file_writer>>(
            codes::logger_system::file_permission_denied,
            "Permission denied",
            "file_writer",
            path
        );
    }
}
```

#### Priority 2: File Operations

```cpp
// Before
void write(const log_entry& entry);
void flush();
bool rotate();

// After
Result<void> write(const log_entry& entry);
Result<void> flush();
Result<void> rotate();
```

**Error Codes**:
- `codes::logger_system::file_write_failed`
- `codes::logger_system::file_rotation_failed`
- `codes::logger_system::writer_stopped`

**Example Implementation**:
```cpp
Result<void> file_writer::write(const log_entry& entry) {
    if (!file_stream_.is_open()) {
        return error<std::monostate>(
            codes::logger_system::writer_not_initialized,
            "Writer not initialized",
            "file_writer"
        );
    }

    if (stopped_) {
        return error<std::monostate>(
            codes::logger_system::writer_stopped,
            "Writer has been stopped",
            "file_writer"
        );
    }

    try {
        file_stream_ << format_entry(entry) << std::endl;

        if (!file_stream_.good()) {
            return error<std::monostate>(
                codes::logger_system::file_write_failed,
                "Failed to write to log file",
                "file_writer",
                std::strerror(errno)
            );
        }

        return ok();
    } catch (const std::exception& e) {
        return error<std::monostate>(
            codes::logger_system::file_write_failed,
            "Write operation failed",
            "file_writer",
            e.what()
        );
    }
}

Result<void> rotating_file_writer::rotate() {
    try {
        close_current_file();

        auto result = open_new_file();
        if (result.is_err()) {
            return result;
        }

        return ok();
    } catch (const std::exception& e) {
        return error<std::monostate>(
            codes::logger_system::file_rotation_failed,
            "Log rotation failed",
            "rotating_file_writer",
            e.what()
        );
    }
}
```

#### Priority 3: Async Writer Operations

```cpp
// Before
void async_writer::start();
void async_writer::stop();

// After
Result<void> async_writer::start();
Result<void> async_writer::stop();
```

**Error Codes**:
- `codes::logger_system::async_writer_failed`
- `codes::logger_system::writer_full`

**Example Implementation**:
```cpp
Result<void> async_writer::start() {
    if (running_) {
        return error<std::monostate>(
            codes::common::already_exists,
            "Async writer already running",
            "async_writer"
        );
    }

    try {
        worker_thread_ = std::thread([this]() { process_queue(); });
        running_ = true;
        return ok();
    } catch (const std::system_error& e) {
        return error<std::monostate>(
            codes::logger_system::async_writer_failed,
            "Failed to start async writer",
            "async_writer",
            e.what()
        );
    }
}
```

---

## Migration Checklist

### Code Changes

- [ ] Add common_system error code includes
- [ ] Migrate `logger::initialize()` to Result<void>
- [ ] Migrate `logger::shutdown()` to Result<void>
- [ ] Migrate `file_writer::write()` to Result<void>
- [ ] Migrate `file_writer::rotate()` to Result<void>
- [ ] Migrate `async_writer` lifecycle to Result<void>
- [ ] Update formatter error handling
- [ ] Add error context (file path, errno, etc.)

### Test Updates

- [ ] Update unit tests for Result<T> APIs
- [ ] Add error case tests for each error code
- [ ] Test file permission errors
- [ ] Test disk full scenarios
- [ ] Test rotation failure recovery
- [ ] Verify error message quality

### Documentation

- [ ] Update API reference with Result<T> signatures
- [ ] Document error codes for each function
- [ ] Add error recovery examples
- [ ] Update integration examples
- [ ] Create troubleshooting guide

---

## Example Migrations

### Example 1: Basic Logger Setup

```cpp
// Usage before
logger log;
if (!log.initialize(config)) {
    std::cerr << "Failed to initialize logger" << std::endl;
    return -1;
}

// Usage after
logger log;
auto result = log.initialize(config);
if (result.is_err()) {
    std::cerr << "Logger initialization failed: "
              << result.error().message << std::endl;
    if (result.error().details) {
        std::cerr << "Details: " << *result.error().details << std::endl;
    }
    return -1;
}
```

### Example 2: Monadic Chaining

```cpp
// Chain logger operations
auto result = logger::create(config)
    .and_then([](auto logger) {
        return logger->add_file_writer("/var/log/app.log");
    })
    .and_then([](auto logger) {
        return logger->add_console_writer();
    })
    .map([](auto logger) {
        return logger;  // Ready to use
    })
    .or_else([](const error_info& err) {
        // Fallback to console-only
        log_warn("File logging unavailable: {}", err.message);
        return logger::create_console_only();
    });
```

### Example 3: Error Recovery with Rotation

```cpp
// Attempt rotation with fallback
auto rotate_result = writer->rotate()
    .or_else([&writer](const error_info& err) {
        log_error("Rotation failed: {}", err.message);

        // Try emergency flush
        return writer->flush();
    })
    .or_else([](const error_info& err) {
        log_critical("Cannot write logs: {}", err.message);
        // Alert monitoring system
        alert_ops("Log system failure");
        return ok();  // Continue without logging
    });
```

### Example 4: Async Writer with Error Handling

```cpp
// Create async writer with proper error handling
auto create_async_logger() -> Result<std::shared_ptr<logger>> {
    auto logger = std::make_shared<logger>();

    RETURN_IF_ERROR(logger->initialize(config));

    auto async_writer = std::make_shared<async_writer>(
        std::make_shared<file_writer>("/var/log/app.log")
    );

    RETURN_IF_ERROR(async_writer->start());

    logger->add_writer(async_writer);
    return ok(logger);
}
```

---

## Error Code Mapping

### Logger System Error Codes (-200 to -299)

```cpp
namespace common::error::codes::logger_system {
    // File errors (-200 to -219)
    constexpr int file_open_failed = -200;
    constexpr int file_write_failed = -201;
    constexpr int file_close_failed = -202;
    constexpr int file_rotation_failed = -203;
    constexpr int file_permission_denied = -204;

    // Writer errors (-220 to -239)
    constexpr int writer_not_initialized = -220;
    constexpr int writer_stopped = -221;
    constexpr int writer_full = -222;
    constexpr int async_writer_failed = -223;

    // Format errors (-240 to -259)
    constexpr int invalid_format = -240;
    constexpr int format_error = -241;

    // Filter errors (-260 to -279)
    constexpr int invalid_filter = -260;
    constexpr int filter_rejected = -261;
}
```

### Error Messages

| Code | Message | When to Use |
|------|---------|-------------|
| file_open_failed | "Failed to open log file" | File open error |
| file_write_failed | "Failed to write to log file" | Write I/O error |
| file_rotation_failed | "Log rotation failed" | Rotation error |
| file_permission_denied | "Permission denied" | Access denied |
| writer_not_initialized | "Writer not initialized" | Use before init |
| writer_stopped | "Writer has been stopped" | Use after stop |
| async_writer_failed | "Async writer failed" | Thread creation error |
| writer_full | "Writer queue full" | Queue capacity reached |

---

## Testing Strategy

### Unit Tests

```cpp
TEST(LoggerPhase3, InitializeWithInvalidPath) {
    logger log;
    logger_config config;
    config.log_path = "/invalid/path/app.log";

    auto result = log.initialize(config);

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(
        codes::logger_system::file_open_failed,
        result.error().code
    );
}

TEST(LoggerPhase3, WriteAfterShutdown) {
    logger log;
    log.initialize(config);
    log.shutdown();

    log_entry entry{"test", log_level::info};
    auto result = log.write(entry);

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(
        codes::logger_system::writer_stopped,
        result.error().code
    );
}

TEST(LoggerPhase3, RotationFailureRecovery) {
    rotating_file_writer writer("/var/log/app.log");

    // Simulate disk full
    fill_disk();

    auto result = writer.rotate()
        .or_else([&](const error_info& err) {
            // Cleanup old logs
            cleanup_old_logs();
            // Retry
            return writer.rotate();
        });

    EXPECT_TRUE(result.is_ok());
}
```

### Integration Tests

```cpp
TEST(LoggerPhase3, HighVolumeWithErrors) {
    auto logger_result = create_logger_with_async_writer();
    ASSERT_TRUE(logger_result.is_ok());

    auto logger = logger_result.value();

    // Write many entries
    for (int i = 0; i < 100000; ++i) {
        auto result = logger->log(log_level::info, "Message " + std::to_string(i));
        if (result.is_err()) {
            // Handle error without stopping
            handle_log_error(result.error());
        }
    }

    // Verify no data loss
    verify_log_integrity();
}
```

---

## Performance Impact

### Expected Overhead

- **Result<T> size**: +24 bytes per return (variant overhead)
- **Success path**: ~0-1ns (inline optimization)
- **Error path**: ~2-3ns (error_info construction)

### Current Performance (Baseline)

- **Synchronous write**: 4.34M msgs/sec
- **Async write**: 8.2M msgs/sec
- **File rotation**: <1ms

### Expected After Migration

- **Synchronous write**: 4.3M msgs/sec (-1%)
- **Async write**: 8.1M msgs/sec (-1%)
- **File rotation**: <1ms (no change)

**Conclusion**: Negligible performance impact (<2%)

---

## Implementation Timeline

### Week 1: Core APIs
- Day 1-2: Migrate logger lifecycle (initialize, shutdown)
- Day 3-4: Migrate file_writer operations
- Day 5: Update tests

### Week 2: Advanced Features
- Day 1-2: Migrate rotating_file_writer
- Day 3-4: Migrate async_writer
- Day 5: Integration tests

### Week 3: Finalization
- Day 1-2: Migrate remaining writers (console, network)
- Day 3: Performance validation
- Day 4-5: Documentation and code review

---

## Backwards Compatibility

### Strategy: Dual API (Transitional)

Provide both old and new APIs during transition:

```cpp
class logger {
public:
    // New API (Result<T>)
    Result<void> initialize(const logger_config& config);

    // Legacy API (deprecated)
    [[deprecated("Use Result<void> initialize() instead")]]
    bool initialize_legacy(const logger_config& config) {
        auto result = initialize(config);
        return result.is_ok();
    }
};
```

**Timeline**:
- Phase 3.1: Add new Result<T> APIs
- Phase 3.2: Deprecate old APIs
- Phase 4: Remove deprecated APIs (after 6 months)

---

## References

- [common_system Error Codes](../../common_system/include/kcenon/common/error/error_codes.h)
- [Error Handling Guidelines](../../common_system/docs/ERROR_HANDLING.md)
- [Result<T> Implementation](../../common_system/include/kcenon/common/patterns/result.h)
- [logger_system RAII Guidelines](RAII_GUIDELINES.md)

---

**Document Status**: Phase 3 Preparation Complete
**Next Action**: Begin implementation or await approval
**Maintainer**: logger_system team
