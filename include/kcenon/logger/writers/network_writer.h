#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "base_writer.h"
#include "../interfaces/log_entry.h"
#include "../interfaces/writer_category.h"
#include <queue>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <mutex>
#include <cstdint>

namespace kcenon::logger {

// Forward declarations for worker threads (std::jthread-based)
class network_send_jthread_worker;
class network_reconnect_jthread_worker;

/**
 * @class network_writer
 * @brief Sends logs over network (TCP/UDP)
 *
 * Category: Asynchronous (non-blocking network I/O with background threads)
 *
 * @since 1.4.0 Added async_writer_tag for category classification
 */
class network_writer : public base_writer, public async_writer_tag {
public:
    enum class protocol_type {
        tcp,
        udp
    };
    
    /**
     * @brief Constructor
     * @param host Remote host address
     * @param port Remote port
     * @param protocol Network protocol (TCP/UDP)
     * @param buffer_size Internal buffer size
     * @param reconnect_interval Reconnection interval in seconds
     */
    network_writer(const std::string& host,
                   uint16_t port,
                   protocol_type protocol = protocol_type::tcp,
                   size_t buffer_size = 8192,
                   std::chrono::seconds reconnect_interval = std::chrono::seconds(5));
    
    /**
     * @brief Destructor
     */
    ~network_writer() override;
    
    /**
     * @brief Write log entry
     * @param entry The log entry to write
     * @return common::VoidResult indicating success or error
     * @since 3.5.0 Changed to use log_entry directly
     */
    common::VoidResult write(const log_entry& entry) override;

    /**
     * @brief Flush pending logs
     */
    common::VoidResult flush() override;
    
    /**
     * @brief Get writer name
     */
    std::string get_name() const override { return "network"; }
    
    /**
     * @brief Check if connected
     */
    bool is_connected() const { return connected_.load(); }
    
    /**
     * @brief Get connection statistics
     */
    struct connection_stats {
        uint64_t messages_sent;
        uint64_t bytes_sent;
        uint64_t connection_failures;
        uint64_t send_failures;
        std::chrono::system_clock::time_point last_connected;
        std::chrono::system_clock::time_point last_error;
    };
    
    connection_stats get_stats() const;
    
private:
    
    // Network operations
    bool connect();
    void disconnect();
    bool send_data(const std::string& data);
    void process_buffer();
    void attempt_reconnect();
    
    // Format log for network transmission
    std::string format_for_network(const log_entry& entry);
    
private:
    std::string host_;
    uint16_t port_;
    protocol_type protocol_;
    size_t buffer_size_;
    std::chrono::seconds reconnect_interval_;
    
    // Socket handling
    int socket_fd_;
    std::atomic<bool> connected_{false};
    std::atomic<bool> running_{false};
    
    // Buffering
    std::queue<log_entry> buffer_;
    mutable std::mutex buffer_mutex_;
    std::condition_variable buffer_cv_;
    
    // Worker threads (using std::jthread)
    std::unique_ptr<network_send_jthread_worker> send_worker_;
    std::unique_ptr<network_reconnect_jthread_worker> reconnect_worker_;
    
    // Statistics
    mutable std::mutex stats_mutex_;
    connection_stats stats_{};
    
    // Helper functions
    std::string escape_json(const std::string& str) const;
};

} // namespace kcenon::logger