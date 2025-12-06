/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file network_writer.cpp
 * @brief Network writer implementation with jthread compatibility
 * @since 1.3.0 - Refactored to use jthread compatibility layer
 *
 * This implementation uses C++20 std::jthread with std::stop_token for
 * cooperative cancellation where available, with fallback to std::thread
 * for environments without jthread support (e.g., libc++).
 */

#include <kcenon/logger/writers/network_writer.h>
#include <kcenon/logger/utils/error_handling_utils.h>
#include "../async/jthread_compat.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
    typedef SSIZE_T ssize_t;  // Define ssize_t for Windows
    #define close closesocket
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
#endif

#include <cerrno>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

namespace kcenon::logger {

using namespace async;

/**
 * @brief Worker thread for sending buffered logs with jthread compatibility
 *
 * Uses std::jthread with std::stop_token for cooperative cancellation
 * where available, falls back to std::thread with manual stop mechanism.
 */
class network_send_jthread_worker {
public:
    using send_callback = std::function<void()>;

    explicit network_send_jthread_worker(send_callback callback)
        : callback_(std::move(callback))
    {}

    ~network_send_jthread_worker() {
        stop();
    }

    void start() {
        if (running_.exchange(true, std::memory_order_acq_rel)) {
            return;  // Already started
        }

#if LOGGER_HAS_JTHREAD
        auto callback = callback_;
        thread_ = compat_jthread([callback](std::stop_token stop_token) {
            while (!stop_token.stop_requested()) {
                if (callback) {
                    callback();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
#else
        // Create worker thread with manual stop source
        // Note: We use the stop_source created by compat_jthread to ensure
        // request_stop() correctly signals the worker loop
        auto callback = callback_;
        thread_ = compat_jthread([callback](simple_stop_source& stop) {
            while (!stop.stop_requested()) {
                if (callback) {
                    callback();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
#endif
    }

    void stop() {
        if (!running_.exchange(false, std::memory_order_acq_rel)) {
            return;  // Already stopped
        }

        // Request stop and join thread
        thread_.request_stop();
        thread_.join();
    }

    void notify_work() {
        has_work_.store(true, std::memory_order_release);
    }

private:
    send_callback callback_;
    compat_jthread thread_;
    std::atomic<bool> running_{false};
    std::atomic<bool> has_work_{false};
};

/**
 * @brief Worker thread for reconnection attempts with jthread compatibility
 *
 * Uses std::jthread with std::stop_token for cooperative cancellation
 * where available, falls back to std::thread with manual stop mechanism.
 */
class network_reconnect_jthread_worker {
public:
    using reconnect_callback = std::function<void()>;

    explicit network_reconnect_jthread_worker(reconnect_callback callback,
                                              std::chrono::seconds interval)
        : callback_(std::move(callback))
        , reconnect_interval_(interval)
    {}

    ~network_reconnect_jthread_worker() {
        stop();
    }

    void start() {
        if (running_.exchange(true, std::memory_order_acq_rel)) {
            return;  // Already started
        }

#if LOGGER_HAS_JTHREAD
        auto callback = callback_;
        auto interval = reconnect_interval_;
        thread_ = compat_jthread([callback, interval](std::stop_token stop_token) {
            while (!stop_token.stop_requested()) {
                if (callback) {
                    callback();
                }
                // Sleep with stop checking
                for (auto elapsed = std::chrono::milliseconds{0};
                     elapsed < std::chrono::duration_cast<std::chrono::milliseconds>(interval)
                        && !stop_token.stop_requested();
                     elapsed += std::chrono::milliseconds(100)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        });
#else
        // Create worker thread with manual stop source
        // Note: We use the stop_source created by compat_jthread to ensure
        // request_stop() correctly signals the worker loop
        auto callback = callback_;
        auto interval = reconnect_interval_;
        thread_ = compat_jthread([callback, interval](simple_stop_source& stop) {
            while (!stop.stop_requested()) {
                if (callback) {
                    callback();
                }
                // Sleep with stop checking
                for (auto elapsed = std::chrono::milliseconds{0};
                     elapsed < std::chrono::duration_cast<std::chrono::milliseconds>(interval)
                        && !stop.stop_requested();
                     elapsed += std::chrono::milliseconds(100)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        });
#endif
    }

    void stop() {
        if (!running_.exchange(false, std::memory_order_acq_rel)) {
            return;  // Already stopped
        }

        // Request stop and join thread
        thread_.request_stop();
        thread_.join();
    }

private:
    reconnect_callback callback_;
    std::chrono::seconds reconnect_interval_;
    compat_jthread thread_;
    std::atomic<bool> running_{false};
};

network_writer::network_writer(const std::string& host,
                               uint16_t port,
                               protocol_type protocol,
                               size_t buffer_size,
                               std::chrono::seconds reconnect_interval)
    : host_(host)
    , port_(port)
    , protocol_(protocol)
    , buffer_size_(buffer_size)
    , reconnect_interval_(reconnect_interval)
    , socket_fd_(-1) {

#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Failed to initialize Winsock");
    }
#endif

    running_ = true;

    // Create and start send worker
    send_worker_ = std::make_unique<network_send_jthread_worker>(
        [this] { process_buffer(); });
    send_worker_->start();

    // Create and start reconnect worker for TCP
    if (protocol_ == protocol_type::tcp) {
        reconnect_worker_ = std::make_unique<network_reconnect_jthread_worker>(
            [this] { attempt_reconnect(); }, reconnect_interval_);
        reconnect_worker_->start();
    }

    // Initial connection attempt
    connect();
}

network_writer::~network_writer() {
    running_ = false;

    // Stop workers with error handling
    // IMPORTANT: Reset workers after stop to join their threads before accessing any members
    utils::safe_destructor_operation("send_worker_stop", [this]() {
        if (send_worker_) {
            send_worker_->stop();
            send_worker_.reset();
        }
    });

    utils::safe_destructor_operation("reconnect_worker_stop", [this]() {
        if (reconnect_worker_) {
            reconnect_worker_->stop();
            reconnect_worker_.reset();
        }
    });

    // Disconnect with error handling
    utils::safe_destructor_operation("network_disconnect", [this]() {
        disconnect();
    });

#ifdef _WIN32
    // Cleanup Winsock with error handling
    utils::safe_destructor_operation("winsock_cleanup", []() {
        WSACleanup();
    });
#endif
}

result_void network_writer::write(logger_system::log_level level,
                                  const std::string& message,
                                  const std::string& file,
                                  int line,
                                  const std::string& function,
                                  const std::chrono::system_clock::time_point& timestamp) {

    std::lock_guard<std::mutex> lock(buffer_mutex_);

    // Check buffer size
    if (buffer_.size() >= buffer_size_) {
        // Drop oldest message
        buffer_.pop();
        std::lock_guard<std::mutex> stats_lock(stats_mutex_);
        stats_.send_failures++;
        // Note: We still accept the new message after dropping the oldest
    }

    buffer_.push({level, message, file, line, function, timestamp});

    // Notify send worker
    if (send_worker_) {
        send_worker_->notify_work();
    }

    return {}; // Success
}

result_void network_writer::flush() {
    std::unique_lock<std::mutex> lock(buffer_mutex_);
    auto start = std::chrono::steady_clock::now();
    auto timeout = std::chrono::seconds(5); // 5 second timeout

    while (!buffer_.empty()) {
        if (buffer_cv_.wait_for(lock, timeout, [this] { return buffer_.empty() || !running_; })) {
            if (!buffer_.empty() && !running_) {
                return make_logger_error(logger_error_code::flush_timeout,
                                        "Network writer stopped before flush completed");
            }
        } else {
            return make_logger_error(logger_error_code::flush_timeout,
                                    "Network flush timeout");
        }

        if (std::chrono::steady_clock::now() - start > timeout) {
            return make_logger_error(logger_error_code::flush_timeout,
                                    "Network flush exceeded timeout");
        }
    }
    return {}; // Success
}

network_writer::connection_stats network_writer::get_stats() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    return stats_;
}

bool network_writer::connect() {
    if (connected_) {
        return true;
    }

    // Create socket
    int sock_type = (protocol_ == protocol_type::tcp) ? SOCK_STREAM : SOCK_DGRAM;
    socket_fd_ = socket(AF_INET, sock_type, 0);
    if (socket_fd_ < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return false;
    }

    // Resolve hostname
    struct hostent* server = gethostbyname(host_.c_str());
    if (!server) {
        std::cerr << "Failed to resolve host: " << host_ << std::endl;
        close(socket_fd_);
        socket_fd_ = -1;
        return false;
    }

    // Setup server address
    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    std::memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect (TCP only)
    if (protocol_ == protocol_type::tcp) {
        if (::connect(socket_fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Failed to connect to " << host_ << ":" << port_
                     << " - " << strerror(errno) << std::endl;
            close(socket_fd_);
            socket_fd_ = -1;

            std::lock_guard<std::mutex> lock(stats_mutex_);
            stats_.connection_failures++;
            stats_.last_error = std::chrono::system_clock::now();
            return false;
        }
    } else {
        // For UDP, just save the server address
        if (::connect(socket_fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Failed to set UDP destination: " << strerror(errno) << std::endl;
            close(socket_fd_);
            socket_fd_ = -1;
            return false;
        }
    }

    connected_ = true;

    std::lock_guard<std::mutex> lock(stats_mutex_);
    stats_.last_connected = std::chrono::system_clock::now();

    std::cout << "Connected to " << host_ << ":" << port_
              << " via " << (protocol_ == protocol_type::tcp ? "TCP" : "UDP") << std::endl;

    return true;
}

void network_writer::disconnect() {
    if (socket_fd_ >= 0) {
        close(socket_fd_);
        socket_fd_ = -1;
    }
    connected_ = false;
}

bool network_writer::send_data(const std::string& data) {
    if (!connected_ || socket_fd_ < 0) {
        return false;
    }

#ifdef _WIN32
    int sent = ::send(socket_fd_, data.c_str(), static_cast<int>(data.length()), 0);
#else
    ssize_t sent = ::send(socket_fd_, data.c_str(), data.length(), 0);
#endif
    if (sent < 0) {
        if (protocol_ == protocol_type::tcp) {
            // TCP connection lost
            std::cerr << "Send failed: " << strerror(errno) << std::endl;
            disconnect();

            std::lock_guard<std::mutex> lock(stats_mutex_);
            stats_.send_failures++;
            stats_.last_error = std::chrono::system_clock::now();
        }
        return false;
    }

    std::lock_guard<std::mutex> lock(stats_mutex_);
    stats_.messages_sent++;
    stats_.bytes_sent += sent;

    return true;
}

void network_writer::process_buffer() {
    std::unique_lock<std::mutex> lock(buffer_mutex_);

    // Process buffered logs
    while (!buffer_.empty() && running_) {
        auto log = std::move(buffer_.front());
        buffer_.pop();
        lock.unlock();

        // Format and send
        std::string formatted = format_for_network(log);
        send_data(formatted);

        lock.lock();
    }
}

void network_writer::attempt_reconnect() {
    if (!connected_ && running_) {
        std::cout << "Attempting to reconnect to " << host_ << ":" << port_ << std::endl;
        connect();
    }
}

std::string network_writer::format_for_network(const buffered_log& log) {
    // Format as JSON for network transmission
    std::ostringstream oss;
    oss << "{";

    // Timestamp
    auto time_t = std::chrono::system_clock::to_time_t(log.timestamp);
    oss << "\"@timestamp\":\"";
    oss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%SZ") << "\",";

    // Level
    oss << "\"level\":\"" << logger_system::log_level_to_string(log.level) << "\",";

    // Message
    oss << "\"message\":\"" << escape_json(log.message) << "\"";

    // Optional fields
    if (!log.file.empty()) {
        oss << ",\"file\":\"" << escape_json(log.file) << "\"";
        oss << ",\"line\":" << log.line;
    }

    if (!log.function.empty()) {
        oss << ",\"function\":\"" << escape_json(log.function) << "\"";
    }

    // Add hostname
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        oss << ",\"host\":\"" << hostname << "\"";
    }

    oss << "}\n";
    return oss.str();
}

std::string network_writer::escape_json(const std::string& str) const {
    std::string escaped;
    for (char c : str) {
        if (c == '"') escaped += "\\\"";
        else if (c == '\\') escaped += "\\\\";
        else if (c == '\n') escaped += "\\n";
        else if (c == '\r') escaped += "\\r";
        else if (c == '\t') escaped += "\\t";
        else escaped += c;
    }
    return escaped;
}

} // namespace kcenon::logger
