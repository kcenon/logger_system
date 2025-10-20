# Logger System - Improvement Plan

> **Language:** **English** | [한국어](IMPROVEMENTS_KO.md)

## Current Status

**Version:** 1.0.0
**Last Review:** 2025-01-20
**Overall Score:** 3.0/5

### Strengths
- PIMPL idiom for ABI stability
- Comprehensive writer system
- Good metrics integration
- Flexible DI support

### Critical Issues
- **async_writer unsafe shutdown** - Can cause data loss and resource leaks
- Exception throwing in constructors
- No guaranteed flush on abnormal termination

---

## Critical Issues

### 1. async_writer Unsafe Shutdown

**Location:** `include/kcenon/logger/writers/async_writer.h:87-131`

**Current Issue:**
```cpp
void stop() {
    if (!running_.exchange(false)) {
        return; // Already stopped
    }

    // ... timeout handling ...
    if (status == std::future_status::timeout) {
        // ❌ CRITICAL BUG: Detaches thread without guarantees
        std::cerr << "[async_writer] Warning: Worker thread did not stop within 5 seconds. "
                  << "Detaching thread to prevent deadlock. Some messages may be lost.\n";

        if (worker_thread_.joinable()) {
            worker_thread_.detach();  // ❌ Race condition with flush_remaining()
        }
    }

    // ❌ Races with detached thread!
    try {
        flush_remaining();
    } catch (...) {}
}
```

**Problems:**
1. **Data Race:** `flush_remaining()` accesses `message_queue_` while detached thread may still be running
2. **Resource Leak:** Detached thread continues indefinitely
3. **Log Loss:** Important messages may be silently dropped
4. **Undefined Behavior:** Accessing destroyed objects from detached thread

**Root Causes:**
- Worker thread may block on slow I/O (network, disk)
- No cancellation mechanism for blocking operations
- Arbitrary 5-second timeout

**Proposed Solution:**

```cpp
// Option 1: Two-Phase Shutdown (Recommended)
class async_writer : public base_writer {
public:
    /**
     * @brief Initiate graceful shutdown (non-blocking)
     *
     * Signals worker thread to stop after processing pending messages.
     * Does NOT wait for completion.
     */
    void shutdown_graceful() {
        running_.store(false, std::memory_order_release);

        {
            std::lock_guard lock(queue_mutex_);
            queue_cv_.notify_all();
        }
    }

    /**
     * @brief Wait for shutdown to complete
     *
     * @param timeout Maximum time to wait
     * @return true if shutdown completed, false if timeout
     */
    bool wait_for_shutdown(std::chrono::milliseconds timeout) {
        if (!worker_thread_.joinable()) {
            return true;
        }

        auto future = std::async(std::launch::async, [this] {
            if (worker_thread_.joinable()) {
                worker_thread_.join();
            }
        });

        return future.wait_for(timeout) == std::future_status::ready;
    }

    /**
     * @brief Force shutdown, possibly losing messages
     *
     * Use only when graceful shutdown fails.
     * @return Number of messages lost
     */
    size_t shutdown_forced() {
        // Set poison pill
        force_stop_.store(true, std::memory_order_release);

        {
            std::lock_guard lock(queue_mutex_);
            queue_cv_.notify_all();
        }

        // Wait brief period
        if (worker_thread_.joinable()) {
            auto future = std::async(std::launch::async, [this] {
                worker_thread_.join();
            });

            if (future.wait_for(std::chrono::seconds(1)) != std::future_status::ready) {
                // Still stuck - must detach but document clearly
                std::cerr << "[async_writer] CRITICAL: Worker thread unresponsive. "
                          << "Detaching. This is a bug in wrapped writer.\n";
                worker_thread_.detach();
            }
        }

        // Count remaining messages
        std::lock_guard lock(queue_mutex_);
        size_t lost = message_queue_.size();
        message_queue_ = {};  // Clear
        return lost;
    }

    /**
     * @brief Improved stop with clear semantics
     */
    void stop() {
        if (!running_.exchange(false, std::memory_order_acq_rel)) {
            return;  // Already stopped
        }

        // Phase 1: Signal graceful shutdown
        shutdown_graceful();

        // Phase 2: Wait with reasonable timeout
        bool completed = wait_for_shutdown(std::chrono::seconds(10));

        if (!completed) {
            // Phase 3: Force shutdown
            size_t lost = shutdown_forced();
            if (lost > 0) {
                std::cerr << "[async_writer] ERROR: Lost " << lost
                          << " log messages during forced shutdown\n";
            }
        }

        // Safe to flush now (worker is joined or detached with queue cleared)
        try {
            wrapped_writer_->flush();
        } catch (const std::exception& e) {
            std::cerr << "[async_writer] Error in final flush: " << e.what() << "\n";
        }
    }

    ~async_writer() override {
        stop();  // Destructor calls stop()
    }

private:
    void process_messages() {
        while (true) {
            std::unique_lock lock(queue_mutex_);

            queue_cv_.wait(lock, [this] {
                return !message_queue_.empty() ||
                       !running_.load(std::memory_order_acquire) ||
                       force_stop_.load(std::memory_order_acquire);
            });

            // Check for forced stop
            if (force_stop_.load(std::memory_order_acquire)) {
                return;  // Exit immediately
            }

            // Check for graceful stop
            if (!running_.load(std::memory_order_acquire) && message_queue_.empty()) {
                return;  // Exit after draining queue
            }

            // Process batch
            while (!message_queue_.empty()) {
                auto msg = std::move(message_queue_.front());
                message_queue_.pop();

                lock.unlock();

                try {
                    wrapped_writer_->write(msg.level, msg.message,
                                          msg.file, msg.line,
                                          msg.function, msg.timestamp);
                } catch (const std::exception& e) {
                    // Log to stderr - can't use logger recursively
                    std::cerr << "[async_writer] Write failed: " << e.what() << "\n";
                }

                lock.lock();
            }

            flush_cv_.notify_all();
        }
    }

    std::atomic<bool> force_stop_{false};  // New flag
    // ... rest of members ...
};
```

**Benefits:**
- ✅ No data races
- ✅ Predictable shutdown
- ✅ Clear semantics (graceful vs forced)
- ✅ Messages not silently lost

**Priority:** P0 (Critical)
**Effort:** 2-3 days (with thorough testing)
**Impact:** Critical (data integrity, stability)

---

### 2. Exception in Constructor

**Location:** `include/kcenon/logger/writers/async_writer.h:44-52`

**Current Issue:**
```cpp
explicit async_writer(std::unique_ptr<base_writer> wrapped_writer,
                     std::size_t queue_size = 10000)
    : wrapped_writer_(std::move(wrapped_writer))
    , max_queue_size_(queue_size)
    , running_(false) {
    if (!wrapped_writer_) {
        throw std::invalid_argument("Wrapped writer cannot be null");  // ❌
    }
}
```

**Problems:**
- Throws exception from constructor
- Leaves object in invalid state if start() called later
- Violates Modern C++ best practices

**Proposed Solution:**

```cpp
// Option 1: Factory function with Result<T>
class async_writer : public base_writer {
public:
    // Constructor becomes private
private:
    async_writer(std::unique_ptr<base_writer> wrapped_writer,
                std::size_t queue_size)
        : wrapped_writer_(std::move(wrapped_writer))
        , max_queue_size_(queue_size)
        , running_(false) {
        // No validation here
    }

public:
    /**
     * @brief Create async_writer with validation
     *
     * @param wrapped_writer Writer to wrap (must not be null)
     * @param queue_size Maximum queue size
     * @return Result containing async_writer or error
     */
    static result<std::unique_ptr<async_writer>>
    create(std::unique_ptr<base_writer> wrapped_writer,
           std::size_t queue_size = 10000) {

        if (!wrapped_writer) {
            return make_logger_error(
                logger_error_code::invalid_argument,
                "Wrapped writer cannot be null"
            );
        }

        if (queue_size == 0) {
            return make_logger_error(
                logger_error_code::invalid_argument,
                "Queue size must be > 0"
            );
        }

        // Use make_unique with private constructor (friend declaration needed)
        auto writer = std::unique_ptr<async_writer>(
            new async_writer(std::move(wrapped_writer), queue_size)
        );

        return result<std::unique_ptr<async_writer>>::ok(std::move(writer));
    }
};

// Usage:
auto result = async_writer::create(std::make_unique<file_writer>("log.txt"));
if (!result) {
    std::cerr << "Failed to create async writer: "
              << result.error().message << "\n";
    return;
}
auto writer = std::move(result.value());
```

**Priority:** P1
**Effort:** 1 day
**Impact:** Medium (API improvement, safety)

---

## High Priority Improvements

### 3. Add Structured Logging Support

**Current Issue:**
Only string-based logging, no structured data.

**Proposed Solution:**

```cpp
// New: structured_log_entry.h
namespace kcenon::logger {

class structured_log_entry {
public:
    structured_log_entry(log_level level, std::string_view message)
        : level_(level), message_(message) {}

    // Add fields
    template<typename T>
    structured_log_entry& field(std::string_view key, const T& value) {
        fields_[std::string(key)] = to_string(value);
        return *this;
    }

    // Add tags
    structured_log_entry& tag(std::string_view tag) {
        tags_.emplace_back(tag);
        return *this;
    }

    // Serialization
    std::string to_json() const {
        std::ostringstream oss;
        oss << "{"
            << R"("level":")" << level_name(level_) << R"(",)"
            << R"("message":")" << escape_json(message_) << R"(",)"
            << R"("timestamp":")" << format_timestamp() << R"(",)";

        if (!fields_.empty()) {
            oss << R"("fields":{)";
            bool first = true;
            for (const auto& [k, v] : fields_) {
                if (!first) oss << ",";
                oss << R"(")" << escape_json(k) << R"(":")"
                    << escape_json(v) << R"(")";
                first = false;
            }
            oss << "},";
        }

        if (!tags_.empty()) {
            oss << R"("tags":[)";
            bool first = true;
            for (const auto& tag : tags_) {
                if (!first) oss << ",";
                oss << R"(")" << escape_json(tag) << R"(")";
                first = false;
            }
            oss << "]";
        }

        oss << "}";
        return oss.str();
    }

private:
    log_level level_;
    std::string message_;
    std::unordered_map<std::string, std::string> fields_;
    std::vector<std::string> tags_;
    std::chrono::system_clock::time_point timestamp_{
        std::chrono::system_clock::now()
    };
};

// Usage:
logger.log_structured(
    structured_log_entry(log_level::info, "User login")
        .field("user_id", 12345)
        .field("ip_address", "192.168.1.1")
        .field("session_id", "abc123")
        .tag("authentication")
        .tag("security")
);

// Output:
// {
//   "level": "info",
//   "message": "User login",
//   "timestamp": "2025-01-20T10:30:45.123Z",
//   "fields": {
//     "user_id": "12345",
//     "ip_address": "192.168.1.1",
//     "session_id": "abc123"
//   },
//   "tags": ["authentication", "security"]
// }

} // namespace kcenon::logger
```

**Priority:** P2
**Effort:** 5-7 days
**Impact:** High (modern logging practices)

---

### 4. Add Log Rotation Support

**Proposed Feature:**

```cpp
struct rotation_policy {
    enum class trigger {
        size,           // Rotate when file exceeds size
        time,           // Rotate at specific intervals
        size_and_time   // Both conditions
    };

    trigger type = trigger::size;

    // Size-based
    size_t max_file_size = 100 * 1024 * 1024;  // 100MB

    // Time-based
    std::chrono::hours rotation_interval{24};
    int rotation_hour = 0;  // 00:00

    // Retention
    size_t max_files = 10;
    std::chrono::hours max_age{7 * 24};  // 7 days

    // Compression
    bool compress_old_files = true;
    std::string compression_format = "gz";  // "gz", "bz2", "zst"
};

class rotating_file_writer : public base_writer {
public:
    rotating_file_writer(const std::string& base_path,
                        const rotation_policy& policy);

    result_void write(...) override {
        // Check if rotation needed
        if (should_rotate()) {
            auto result = rotate();
            if (!result) {
                return result;  // Rotation failed
            }
        }

        // Write to current file
        return current_file_->write(...);
    }

private:
    bool should_rotate() const {
        if (policy_.type == rotation_policy::trigger::size ||
            policy_.type == rotation_policy::trigger::size_and_time) {
            if (current_file_size_ >= policy_.max_file_size) {
                return true;
            }
        }

        if (policy_.type == rotation_policy::trigger::time ||
            policy_.type == rotation_policy::trigger::size_and_time) {
            auto now = std::chrono::system_clock::now();
            auto elapsed = now - last_rotation_time_;
            if (elapsed >= policy_.rotation_interval) {
                return true;
            }
        }

        return false;
    }

    result_void rotate() {
        // 1. Close current file
        current_file_->flush();
        current_file_.reset();

        // 2. Rename current → timestamped
        auto timestamp = format_timestamp(std::chrono::system_clock::now());
        auto rotated_name = base_path_ + "." + timestamp;

        std::filesystem::rename(current_path_, rotated_name);

        // 3. Open new file
        current_file_ = std::make_unique<file_writer>(base_path_);

        // 4. Compress old file (async)
        if (policy_.compress_old_files) {
            compress_async(rotated_name);
        }

        // 5. Clean up old files
        cleanup_old_files();

        last_rotation_time_ = std::chrono::system_clock::now();
        current_file_size_ = 0;

        return result_void{};
    }

    void cleanup_old_files() {
        namespace fs = std::filesystem;

        std::vector<fs::path> log_files;
        for (const auto& entry : fs::directory_iterator(
                 fs::path(base_path_).parent_path())) {
            if (entry.path().filename().string().starts_with(
                    fs::path(base_path_).filename().string())) {
                log_files.push_back(entry.path());
            }
        }

        // Sort by modification time
        std::sort(log_files.begin(), log_files.end(),
            [](const auto& a, const auto& b) {
                return fs::last_write_time(a) > fs::last_write_time(b);
            });

        // Keep only max_files
        for (size_t i = policy_.max_files; i < log_files.size(); ++i) {
            fs::remove(log_files[i]);
        }
    }

    std::string base_path_;
    std::string current_path_;
    std::unique_ptr<file_writer> current_file_;
    rotation_policy policy_;
    std::chrono::system_clock::time_point last_rotation_time_;
    size_t current_file_size_ = 0;
};
```

**Priority:** P2
**Effort:** 4-5 days
**Impact:** High (production requirement)

---

### 5. Add Sampling and Rate Limiting

**Proposed Feature:**

```cpp
class sampling_logger_decorator : public logger_interface {
public:
    sampling_logger_decorator(
        std::shared_ptr<logger_interface> underlying,
        double sample_rate = 0.1  // 10% of messages
    ) : underlying_(underlying), sample_rate_(sample_rate) {}

    void log(log_level level, const std::string& message) override {
        if (should_sample()) {
            underlying_->log(level, message);
        } else {
            dropped_samples_.fetch_add(1, std::memory_order_relaxed);
        }
    }

private:
    bool should_sample() {
        // Thread-safe random sampling
        thread_local std::mt19937 gen(std::random_device{}());
        thread_local std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen) < sample_rate_;
    }

    std::shared_ptr<logger_interface> underlying_;
    double sample_rate_;
    std::atomic<size_t> dropped_samples_{0};
};

class rate_limiting_logger_decorator : public logger_interface {
public:
    rate_limiting_logger_decorator(
        std::shared_ptr<logger_interface> underlying,
        size_t max_messages_per_second = 1000
    ) : underlying_(underlying)
      , max_rate_(max_messages_per_second) {}

    void log(log_level level, const std::string& message) override {
        auto now = std::chrono::steady_clock::now();

        {
            std::lock_guard lock(mutex_);

            // Reset counter if new window
            if (now - window_start_ >= std::chrono::seconds(1)) {
                window_start_ = now;
                window_count_ = 0;
            }

            // Check rate limit
            if (window_count_ >= max_rate_) {
                dropped_messages_.fetch_add(1, std::memory_order_relaxed);
                return;
            }

            window_count_++;
        }

        underlying_->log(level, message);
    }

private:
    std::shared_ptr<logger_interface> underlying_;
    size_t max_rate_;

    std::mutex mutex_;
    std::chrono::steady_clock::time_point window_start_{
        std::chrono::steady_clock::now()
    };
    size_t window_count_ = 0;
    std::atomic<size_t> dropped_messages_{0};
};

// Usage:
auto base_logger = create_logger();

// Sample 10% of debug logs
auto sampled = std::make_shared<sampling_logger_decorator>(
    base_logger, 0.1);

// Rate limit to 1000 msgs/sec
auto rate_limited = std::make_shared<rate_limiting_logger_decorator>(
    sampled, 1000);

rate_limited->log(log_level::debug, "High frequency message");
```

**Priority:** P2
**Effort:** 2-3 days
**Impact:** Medium (performance, cost reduction)

---

## Medium Priority Improvements

### 6. Add Context/MDC (Mapped Diagnostic Context)

**Proposed Feature:**

```cpp
class log_context {
public:
    static log_context& current() {
        thread_local log_context ctx;
        return ctx;
    }

    void set(std::string_view key, std::string_view value) {
        context_[std::string(key)] = std::string(value);
    }

    void remove(std::string_view key) {
        context_.erase(std::string(key));
    }

    std::optional<std::string> get(std::string_view key) const {
        auto it = context_.find(std::string(key));
        if (it != context_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    const auto& all() const { return context_; }

    // RAII helper
    class scoped_context {
    public:
        scoped_context(std::string_view key, std::string_view value)
            : key_(key) {
            log_context::current().set(key_, value);
        }

        ~scoped_context() {
            log_context::current().remove(key_);
        }

    private:
        std::string key_;
    };

private:
    std::unordered_map<std::string, std::string> context_;
};

// Usage:
void handle_request(const Request& req) {
    log_context::scoped_context request_id("request_id", req.id);
    log_context::scoped_context user_id("user_id", req.user);

    logger.info("Processing request");  // Auto includes request_id and user_id

    process_business_logic();

    logger.info("Request completed");
} // Context auto-cleared
```

**Priority:** P3
**Effort:** 2-3 days
**Impact:** Medium (debugging, tracing)

---

## Testing Requirements

### Critical Tests

1. **Async Writer Shutdown:**
   ```cpp
   TEST(AsyncWriter, GracefulShutdownCompletes) {
       auto writer = async_writer::create(
           std::make_unique<mock_writer>()).value();

       writer->start();

       // Enqueue many messages
       for (int i = 0; i < 10000; ++i) {
           writer->write(log_level::info, std::to_string(i), "", 0, "", {});
       }

       // Shutdown should complete without timeout
       writer->shutdown_graceful();
       EXPECT_TRUE(writer->wait_for_shutdown(std::chrono::seconds(5)));
   }

   TEST(AsyncWriter, ForcedShutdownWithSlowWriter) {
       auto slow_writer = std::make_unique<mock_writer>();
       slow_writer->set_write_delay(std::chrono::seconds(1));  // Very slow

       auto writer = async_writer::create(std::move(slow_writer)).value();
       writer->start();

       // Enqueue messages
       for (int i = 0; i < 100; ++i) {
           writer->write(log_level::info, std::to_string(i), "", 0, "", {});
       }

       // Force shutdown should handle gracefully
       size_t lost = writer->shutdown_forced();
       EXPECT_GT(lost, 0);  // Some messages will be lost
   }
   ```

2. **No Exceptions in Constructors:**
   ```cpp
   TEST(AsyncWriter, FactoryRejectsNullWriter) {
       auto result = async_writer::create(nullptr);
       EXPECT_TRUE(result.is_err());
       EXPECT_EQ(result.error().code, logger_error_code::invalid_argument);
   }
   ```

3. **Log Rotation:**
   ```cpp
   TEST(RotatingFileWriter, RotatesAtSizeLimit) {
       rotation_policy policy;
       policy.type = rotation_policy::trigger::size;
       policy.max_file_size = 1024;  // 1KB

       rotating_file_writer writer("test.log", policy);

       // Write until rotation
       for (int i = 0; i < 1000; ++i) {
           writer.write(log_level::info, std::string(100, 'x'), "", 0, "", {});
       }

       // Check files exist
       EXPECT_TRUE(std::filesystem::exists("test.log"));
       EXPECT_TRUE(std::filesystem::exists("test.log.2025-01-20-10-30-45"));
   }
   ```

---

## Implementation Roadmap

| Phase | Tasks | Duration | Priority |
|-------|-------|----------|----------|
| Phase 1 | Fix async_writer shutdown | 2-3 days | P0 |
| Phase 2 | Remove constructor exceptions | 1 day | P1 |
| Phase 3 | Structured logging | 5-7 days | P2 |
| Phase 4 | Log rotation | 4-5 days | P2 |
| Phase 5 | Sampling & rate limiting | 2-3 days | P2 |
| Phase 6 | MDC/Context support | 2-3 days | P3 |

**Total Effort:** 16-23 days

---

## Breaking Changes

### Version 2.0.0 (Proposed)

**Breaking Changes:**
1. `async_writer` constructor becomes private, use factory
2. `stop()` behavior changes (now three-phase)
3. New shutdown methods: `shutdown_graceful()`, `wait_for_shutdown()`, `shutdown_forced()`

**Migration Guide:**
```cpp
// Old (1.x)
auto writer = std::make_unique<async_writer>(
    std::make_unique<file_writer>("log.txt"));

// New (2.0)
auto result = async_writer::create(
    std::make_unique<file_writer>("log.txt"));
if (!result) {
    // Handle error
}
auto writer = std::move(result.value());
```

---

## References

- [Structured Logging Best Practices](https://stackify.com/what-is-structured-logging-and-why-developers-need-it/)
- [Log Rotation Strategies](https://betterstack.com/community/guides/logging/log-rotation/)
- [spdlog - Fast C++ Logging](https://github.com/gabime/spdlog)
- [Log4j2 Architecture](https://logging.apache.org/log4j/2.x/manual/architecture.html)
