// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file signal_manager_test.cpp
 * @brief Unit tests for signal_manager (registration, handler lifecycle, emergency flush)
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/security/signal_manager.h>

#include <csignal>

using namespace kcenon::logger::security;

// =============================================================================
// Mock critical_logger_interface for testing
// =============================================================================

class mock_critical_logger : public critical_logger_interface {
public:
    explicit mock_critical_logger(int fd = -1, const char* buf = nullptr, size_t buf_size = 0)
        : fd_(fd), buffer_(buf), buffer_size_(buf_size) {}

    int get_emergency_fd() const override { return fd_; }
    const char* get_emergency_buffer() const override { return buffer_; }
    size_t get_emergency_buffer_size() const override { return buffer_size_; }

private:
    int fd_;
    const char* buffer_;
    size_t buffer_size_;
};

// =============================================================================
// Test fixture
// =============================================================================

class SignalManagerTest : public ::testing::Test {
protected:
    void TearDown() override {
        // Ensure handlers are uninstalled after each test to avoid
        // interfering with other tests or the test runner
    }
};

// =============================================================================
// Construction
// =============================================================================

TEST_F(SignalManagerTest, DefaultConstruction) {
    signal_manager mgr;

    EXPECT_FALSE(mgr.are_handlers_installed());
    EXPECT_EQ(mgr.logger_count(), 0u);
}

// =============================================================================
// Registration
// =============================================================================

TEST_F(SignalManagerTest, RegisterSingleLogger) {
    signal_manager mgr;
    mock_critical_logger logger;

    mgr.register_logger(&logger);

    EXPECT_EQ(mgr.logger_count(), 1u);
    EXPECT_TRUE(mgr.are_handlers_installed());

    mgr.unregister_logger(&logger);
}

TEST_F(SignalManagerTest, RegisterMultipleLoggers) {
    signal_manager mgr;
    mock_critical_logger logger1;
    mock_critical_logger logger2;
    mock_critical_logger logger3;

    mgr.register_logger(&logger1);
    mgr.register_logger(&logger2);
    mgr.register_logger(&logger3);

    EXPECT_EQ(mgr.logger_count(), 3u);
    EXPECT_TRUE(mgr.are_handlers_installed());

    mgr.unregister_logger(&logger1);
    mgr.unregister_logger(&logger2);
    mgr.unregister_logger(&logger3);
}

TEST_F(SignalManagerTest, RegisterSameLoggerTwiceIsDeduplicated) {
    signal_manager mgr;
    mock_critical_logger logger;

    // std::set ignores duplicate inserts
    mgr.register_logger(&logger);
    mgr.register_logger(&logger);

    EXPECT_EQ(mgr.logger_count(), 1u);

    mgr.unregister_logger(&logger);
}

// =============================================================================
// Handler installation / uninstallation
// =============================================================================

TEST_F(SignalManagerTest, HandlersInstalledOnFirstRegistration) {
    signal_manager mgr;
    mock_critical_logger logger;

    EXPECT_FALSE(mgr.are_handlers_installed());

    mgr.register_logger(&logger);
    EXPECT_TRUE(mgr.are_handlers_installed());

    mgr.unregister_logger(&logger);
}

TEST_F(SignalManagerTest, HandlersRemovedOnLastUnregistration) {
    signal_manager mgr;
    mock_critical_logger logger1;
    mock_critical_logger logger2;

    mgr.register_logger(&logger1);
    mgr.register_logger(&logger2);
    EXPECT_TRUE(mgr.are_handlers_installed());

    // Remove first logger - handlers should remain
    mgr.unregister_logger(&logger1);
    EXPECT_TRUE(mgr.are_handlers_installed());
    EXPECT_EQ(mgr.logger_count(), 1u);

    // Remove last logger - handlers should be removed
    mgr.unregister_logger(&logger2);
    EXPECT_FALSE(mgr.are_handlers_installed());
    EXPECT_EQ(mgr.logger_count(), 0u);
}

TEST_F(SignalManagerTest, HandlersReinstalledOnReRegistration) {
    signal_manager mgr;
    mock_critical_logger logger;

    // Register, then unregister
    mgr.register_logger(&logger);
    EXPECT_TRUE(mgr.are_handlers_installed());

    mgr.unregister_logger(&logger);
    EXPECT_FALSE(mgr.are_handlers_installed());

    // Re-register should reinstall handlers
    mgr.register_logger(&logger);
    EXPECT_TRUE(mgr.are_handlers_installed());

    mgr.unregister_logger(&logger);
}

// =============================================================================
// Unregister edge cases
// =============================================================================

TEST_F(SignalManagerTest, UnregisterNonRegisteredLoggerIsNoOp) {
    signal_manager mgr;
    mock_critical_logger logger;

    // Unregistering a logger that was never registered should not crash
    mgr.unregister_logger(&logger);
    EXPECT_EQ(mgr.logger_count(), 0u);
    EXPECT_FALSE(mgr.are_handlers_installed());
}

TEST_F(SignalManagerTest, UnregisterDoesNotAffectOtherLoggers) {
    signal_manager mgr;
    mock_critical_logger logger1;
    mock_critical_logger logger2;

    mgr.register_logger(&logger1);
    mgr.register_logger(&logger2);
    EXPECT_EQ(mgr.logger_count(), 2u);

    mgr.unregister_logger(&logger1);
    EXPECT_EQ(mgr.logger_count(), 1u);
    EXPECT_TRUE(mgr.are_handlers_installed());

    mgr.unregister_logger(&logger2);
}

// =============================================================================
// Signal handler safety (SIGTERM - the safest to test)
// =============================================================================

TEST_F(SignalManagerTest, OriginalHandlerRestoredAfterUnregister) {
    // Save the current SIGTERM handler
    auto original_handler = std::signal(SIGTERM, SIG_DFL);
    // Restore it immediately
    std::signal(SIGTERM, original_handler);

    signal_manager mgr;
    mock_critical_logger logger;

    mgr.register_logger(&logger);
    // At this point, signal_manager has installed its own handler

    mgr.unregister_logger(&logger);
    // After unregister, original handler should be restored

    // Verify the handler was restored by checking it's not signal_manager's
    auto current_handler = std::signal(SIGTERM, SIG_DFL);
    std::signal(SIGTERM, current_handler); // Restore what we just read
    EXPECT_EQ(current_handler, original_handler);
}

// =============================================================================
// detail::safe_write / safe_fsync wrappers
// =============================================================================

TEST_F(SignalManagerTest, SafeWriteToInvalidFdReturnsError) {
    // Writing to an invalid fd should return -1
    ssize_t result = detail::safe_write(-1, "test", 4);
    EXPECT_EQ(result, -1);
}

TEST_F(SignalManagerTest, SafeFsyncToInvalidFdReturnsError) {
    // fsync on invalid fd should return -1
    int result = detail::safe_fsync(-1);
    EXPECT_EQ(result, -1);
}
