> **Language:** **English** | [한국어](CUSTOM_WRITERS_KO.md)

# Creating Custom Writers

This guide explains how to create custom writers for the Logger System to send logs to various destinations.

## Overview

Writers are responsible for the actual output of log messages. The Logger System provides a flexible writer hierarchy that you can extend to create custom output destinations.

## Writer Hierarchy

```
base_writer (abstract)
├── thread_safe_writer (recommended for simple writers)
│   ├── console_writer
│   ├── file_writer
│   │   └── rotating_file_writer
│   └── your_custom_writer (Thread-safety handled automatically!)
├── async_writer (for async wrapper patterns)
├── batch_writer (for batched I/O)
├── critical_writer (for critical log handling)
└── network_writer (for network I/O with custom threading)
```

## Recommended: Using thread_safe_writer (Since v1.3.0)

For most custom writers, inherit from `thread_safe_writer` instead of `base_writer`. This provides:
- **Automatic thread-safety**: Mutex handling is done by the base class
- **Consistent locking**: All writers use the same synchronization strategy
- **Less boilerplate**: No need to manage locks manually
- **Template Method pattern**: Implement `*_impl()` methods, get thread-safety for free

```cpp
#include <kcenon/logger/writers/thread_safe_writer.h>

class my_custom_writer : public kcenon::logger::thread_safe_writer {
public:
    my_custom_writer() : thread_safe_writer() {}

    std::string get_name() const override { return "my_custom"; }

protected:
    // Implement these methods - NO MUTEX NEEDED!
    // The base class handles all synchronization.

    common::VoidResult write_impl(
        logger_system::log_level level,
        const std::string& message,
        const std::string& file,
        int line,
        const std::string& function,
        const std::chrono::system_clock::time_point& timestamp) override
    {
        // Your output logic here - already protected by mutex
        std::cout << format_log_entry(log_entry(level, message, timestamp)) << "\n";
        return common::ok();
    }

    common::VoidResult flush_impl() override {
        // Your flush logic here - already protected by mutex
        std::cout.flush();
        return common::ok();
    }
};
```

### Benefits of thread_safe_writer

1. **No mutex boilerplate**: Derived classes focus only on output logic
2. **Cannot accidentally forget locking**: Public methods are `final`
3. **Deadlock prevention**: Clear contract about when mutex is held
4. **RAII guarantee**: Mutex is always released, even on exceptions

## Base Writer Interface

For advanced use cases (async, batching, custom synchronization), inherit from `base_writer`:

```cpp
class base_writer {
public:
    virtual ~base_writer() = default;

    // Main write method - must be implemented
    virtual common::VoidResult write(
        logger_system::log_level level,
        const std::string& message,
        const std::string& file,
        int line,
        const std::string& function,
        const std::chrono::system_clock::time_point& timestamp) = 0;

    // Flush any buffered data - must be implemented
    virtual common::VoidResult flush() = 0;

    // Optional: color support
    virtual void set_use_color(bool use_color);
    bool use_color() const;

protected:
    // Helper methods available to derived classes
    std::string format_log_entry(const log_entry& entry) const;
};
```

## Simple Examples

### 1. Simple File Writer (Using thread_safe_writer)

A basic file writer using the recommended `thread_safe_writer` base class:

```cpp
#include <kcenon/logger/writers/thread_safe_writer.h>
#include <fstream>

class simple_file_writer : public kcenon::logger::thread_safe_writer {
private:
    std::ofstream file_;
    std::string filename_;

public:
    explicit simple_file_writer(const std::string& filename)
        : filename_(filename) {
        file_.open(filename_, std::ios::app);
        if (!file_.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    }

    ~simple_file_writer() override {
        // flush() is called automatically by base class destructor
        if (file_.is_open()) {
            file_.close();
        }
    }

    std::string get_name() const override { return "simple_file"; }

protected:
    // No mutex needed! thread_safe_writer handles synchronization
    common::VoidResult write_impl(
        logger_system::log_level level,
        const std::string& message,
        const std::string& file,
        int line,
        const std::string& function,
        const std::chrono::system_clock::time_point& timestamp) override
    {
        // Create log entry and format
        log_entry entry = log_entry(level, message, file, line, function, timestamp);
        std::string formatted = format_log_entry(entry);

        file_ << formatted << '\n';

        if (!file_.good()) {
            return make_logger_void_result(
                logger_error_code::file_write_failed,
                "Failed to write to file: " + filename_);
        }
        return common::ok();
    }

    common::VoidResult flush_impl() override {
        file_.flush();
        return common::ok();
    }
};

// Usage
logger->add_writer(std::make_unique<simple_file_writer>("application.log"));
```

> **Note**: Compare this with the built-in `file_writer` class which provides
> additional features like buffering and byte counting.

### 2. Rotating File Writer

A more advanced file writer with size-based rotation:

```cpp
class rotating_file_writer : public logger_module::base_writer {
private:
    std::ofstream file_;
    std::mutex mutex_;
    std::string base_filename_;
    size_t max_size_;
    size_t current_size_;
    int file_index_;
    
    void rotate() {
        file_.close();
        
        // Rename current file
        std::string old_name = base_filename_ + "." + std::to_string(file_index_);
        std::rename(base_filename_.c_str(), old_name.c_str());
        
        // Open new file
        file_.open(base_filename_);
        current_size_ = 0;
        file_index_++;
    }
    
public:
    rotating_file_writer(const std::string& filename, size_t max_size)
        : base_filename_(filename)
        , max_size_(max_size)
        , current_size_(0)
        , file_index_(0) {
        file_.open(filename, std::ios::app);
        
        // Get current file size
        file_.seekp(0, std::ios::end);
        current_size_ = file_.tellp();
    }
    
    void write(thread_module::log_level level,
               const std::string& message,
               const std::string& file,
               int line,
               const std::string& function,
               const std::chrono::system_clock::time_point& timestamp) override {
        std::lock_guard<std::mutex> lock(mutex_);
        
        std::string formatted = format_log_entry(level, message, file, 
                                                line, function, timestamp);
        
        // Check if rotation needed
        if (current_size_ + formatted.size() > max_size_) {
            rotate();
        }
        
        file_ << formatted << std::endl;
        current_size_ += formatted.size() + 1; // +1 for newline
    }
    
    void flush() override {
        std::lock_guard<std::mutex> lock(mutex_);
        file_.flush();
    }
};

// Usage: 10MB max file size
logger->add_writer(std::make_unique<rotating_file_writer>("app.log", 10 * 1024 * 1024));
```

### 3. Network Writer

Send logs to a remote server:

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class network_writer : public logger_module::base_writer {
private:
    int socket_fd_;
    struct sockaddr_in server_addr_;
    std::mutex mutex_;
    bool connected_;
    
    void connect() {
        socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd_ < 0) {
            throw std::runtime_error("Failed to create socket");
        }
        
        if (::connect(socket_fd_, (struct sockaddr*)&server_addr_, 
                      sizeof(server_addr_)) < 0) {
            close(socket_fd_);
            throw std::runtime_error("Failed to connect to log server");
        }
        
        connected_ = true;
    }
    
public:
    network_writer(const std::string& host, int port) : connected_(false) {
        memset(&server_addr_, 0, sizeof(server_addr_));
        server_addr_.sin_family = AF_INET;
        server_addr_.sin_port = htons(port);
        
        if (inet_pton(AF_INET, host.c_str(), &server_addr_.sin_addr) <= 0) {
            throw std::runtime_error("Invalid address: " + host);
        }
        
        connect();
    }
    
    ~network_writer() override {
        if (connected_) {
            close(socket_fd_);
        }
    }
    
    void write(thread_module::log_level level,
               const std::string& message,
               const std::string& file,
               int line,
               const std::string& function,
               const std::chrono::system_clock::time_point& timestamp) override {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (!connected_) {
            return; // Silent failure or throw
        }
        
        std::string formatted = format_log_entry(level, message, file, 
                                                line, function, timestamp);
        formatted += "\n";
        
        ssize_t sent = send(socket_fd_, formatted.c_str(), formatted.size(), 0);
        if (sent < 0) {
            // Handle error - maybe reconnect
            connected_ = false;
        }
    }
    
    void flush() override {
        // Network writes are typically unbuffered
    }
};

// Usage
logger->add_writer(std::make_unique<network_writer>("192.168.1.100", 5514));
```

### 4. Database Writer

Log to a database (using SQLite as example):

```cpp
#include <sqlite3.h>

class database_writer : public logger_module::base_writer {
private:
    sqlite3* db_;
    sqlite3_stmt* insert_stmt_;
    std::mutex mutex_;
    
public:
    explicit database_writer(const std::string& db_path) {
        if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK) {
            throw std::runtime_error("Failed to open database");
        }
        
        // Create table if not exists
        const char* create_table = R"(
            CREATE TABLE IF NOT EXISTS logs (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                timestamp TEXT NOT NULL,
                level TEXT NOT NULL,
                message TEXT NOT NULL,
                file TEXT,
                line INTEGER,
                function TEXT
            )
        )";
        
        char* err_msg = nullptr;
        if (sqlite3_exec(db_, create_table, nullptr, nullptr, &err_msg) != SQLITE_OK) {
            std::string error = err_msg;
            sqlite3_free(err_msg);
            sqlite3_close(db_);
            throw std::runtime_error("Failed to create table: " + error);
        }
        
        // Prepare insert statement
        const char* insert_sql = R"(
            INSERT INTO logs (timestamp, level, message, file, line, function)
            VALUES (?, ?, ?, ?, ?, ?)
        )";
        
        if (sqlite3_prepare_v2(db_, insert_sql, -1, &insert_stmt_, nullptr) != SQLITE_OK) {
            sqlite3_close(db_);
            throw std::runtime_error("Failed to prepare statement");
        }
    }
    
    ~database_writer() override {
        if (insert_stmt_) {
            sqlite3_finalize(insert_stmt_);
        }
        if (db_) {
            sqlite3_close(db_);
        }
    }
    
    void write(thread_module::log_level level,
               const std::string& message,
               const std::string& file,
               int line,
               const std::string& function,
               const std::chrono::system_clock::time_point& timestamp) override {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Reset statement
        sqlite3_reset(insert_stmt_);
        
        // Format timestamp
        auto time_t = std::chrono::system_clock::to_time_t(timestamp);
        char time_buf[64];
        std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", 
                      std::localtime(&time_t));
        
        // Bind parameters
        sqlite3_bind_text(insert_stmt_, 1, time_buf, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insert_stmt_, 2, level_to_string(level).c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insert_stmt_, 3, message.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insert_stmt_, 4, file.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(insert_stmt_, 5, line);
        sqlite3_bind_text(insert_stmt_, 6, function.c_str(), -1, SQLITE_TRANSIENT);
        
        // Execute
        if (sqlite3_step(insert_stmt_) != SQLITE_DONE) {
            // Handle error
        }
    }
    
    void flush() override {
        // SQLite auto-commits by default
    }
};

// Usage
logger->add_writer(std::make_unique<database_writer>("logs.db"));
```

## Advanced Patterns

### 1. Filtering Writer

A writer that filters logs before outputting:

```cpp
class filtering_writer : public logger_module::base_writer {
private:
    std::unique_ptr<base_writer> inner_writer_;
    std::function<bool(thread_module::log_level, const std::string&)> filter_;
    
public:
    filtering_writer(std::unique_ptr<base_writer> inner,
                    std::function<bool(thread_module::log_level, const std::string&)> filter)
        : inner_writer_(std::move(inner))
        , filter_(std::move(filter)) {}
    
    void write(thread_module::log_level level,
               const std::string& message,
               const std::string& file,
               int line,
               const std::string& function,
               const std::chrono::system_clock::time_point& timestamp) override {
        if (filter_(level, message)) {
            inner_writer_->write(level, message, file, line, function, timestamp);
        }
    }
    
    void flush() override {
        inner_writer_->flush();
    }
};

// Usage: Only log errors containing "critical"
auto filtered = std::make_unique<filtering_writer>(
    std::make_unique<console_writer>(),
    [](thread_module::log_level level, const std::string& msg) {
        return level >= thread_module::log_level::error && 
               msg.find("critical") != std::string::npos;
    }
);
logger->add_writer(std::move(filtered));
```

### 2. Async Writer Wrapper

The Logger System provides a built-in `async_writer` class that wraps any writer for asynchronous operation. For detailed documentation on async writers, including performance characteristics and usage patterns, see the **[Async Writers Guide](ASYNC_WRITERS.md)**.

**Quick Example:**

```cpp
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/file_writer.h>

// Wrap file writer for async operation
auto file_writer = std::make_unique<kcenon::logger::file_writer>("app.log");
auto async = std::make_unique<kcenon::logger::async_writer>(
    std::move(file_writer),
    10000,  // queue size
    std::chrono::seconds(5)  // flush timeout
);

async->start();
logger->add_writer(std::move(async));
```

For high-throughput scenarios (>100K msg/sec), advanced async implementations are available. See [Async Writers Guide](ASYNC_WRITERS.md) for details.

## Choosing the Right Base Class

| Base Class | Use When |
|------------|----------|
| `thread_safe_writer` | Simple synchronous I/O (file, console, socket) - **recommended** |
| `base_writer` | Custom synchronization, async patterns, or wrapper writers |

### When to Use thread_safe_writer

✅ Simple output destinations (file, console, database)
✅ Standard mutex-based synchronization is sufficient
✅ Want to minimize boilerplate code

### When to Use base_writer Directly

✅ Wrapper patterns (like `async_writer`, `batch_writer`)
✅ Writers with complex internal threading (like `network_writer`)
✅ Custom synchronization requirements (spinlock, RW-lock, lock-free)

## Best Practices

1. **Use thread_safe_writer**: For simple writers, inherit from `thread_safe_writer` to get automatic thread-safety without boilerplate.
2. **Error Handling**: Decide on failure behavior (throw, silent fail, retry/backoff) and expose counters for observability.
3. **Batching**: Prefer batching for I/O heavy writers to reduce syscalls and context switches.
4. **Resource Management**: Use RAII for file handles, sockets, and DB connections; ensure `flush()` is efficient and idempotent.
5. **Configuration**: Make writers configurable (paths, formats, thresholds), and document defaults.
6. **Security**: Avoid writing secrets/PII; consider integrating `log_sanitizer` upstream. If encrypting, use a vetted crypto library rather than demo components.
7. **Windows Networking**: For socket-based writers, guard platform specifics (`#ifdef _WIN32`) and initialize WinSock.

## Testing Custom Writers

```cpp
// Test harness for custom writers
class writer_test {
public:
    static void test_writer(std::unique_ptr<base_writer> writer) {
        // Test basic write
        writer->write(thread_module::log_level::info, 
                     "Test message", 
                     __FILE__, __LINE__, __func__,
                     std::chrono::system_clock::now());
        
        // Test different levels
        for (auto level : {log_level::trace, log_level::debug, 
                          log_level::info, log_level::warning,
                          log_level::error, log_level::critical}) {
            writer->write(level, "Level test", "", 0, "",
                         std::chrono::system_clock::now());
        }
        
        // Test flush
        writer->flush();
        
        // Test concurrent writes
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([&writer, i]() {
                for (int j = 0; j < 100; ++j) {
                    writer->write(log_level::info,
                                 "Thread " + std::to_string(i),
                                 "", 0, "",
                                 std::chrono::system_clock::now());
                }
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        writer->flush();
    }
};
```

---

*Last Updated: 2025-01-11* (Added thread_safe_writer documentation and examples)
