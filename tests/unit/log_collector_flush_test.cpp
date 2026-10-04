// BSD 3-Clause License
// Copyright (c) 2026, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#include <gtest/gtest.h>
#include <kcenon/logger/core/log_collector.h>
#include <kcenon/logger/writers/base_writer.h>

#include <atomic>
#include <chrono>
#include <future>
#include <memory>
#include <utility>

namespace {

class GatedCollectorWriter : public kcenon::logger::base_writer {
public:
    explicit GatedCollectorWriter(std::shared_future<void> release)
        : release_(std::move(release)) {}

    kcenon::common::VoidResult write(const kcenon::logger::log_entry&) override {
        write_started.set_value();
        release_.wait();
        write_finished.store(true);
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult flush() override {
        flushed_before_write_finished.store(!write_finished.load());
        return kcenon::common::ok();
    }

    std::string get_name() const override { return "gated_collector"; }

    std::promise<void> write_started;
    std::atomic<bool> write_finished{false};
    std::atomic<bool> flushed_before_write_finished{false};

private:
    std::shared_future<void> release_;
};

TEST(LogCollectorFlushTest, WaitsForDequeuedWriteBeforeFlushingWriters) {
    using namespace std::chrono_literals;
    std::promise<void> release;
    auto writer = std::make_shared<GatedCollectorWriter>(release.get_future().share());
    auto write_started = writer->write_started.get_future();
    kcenon::logger::log_collector collector(16);
    collector.add_writer(writer);
    collector.start();
    collector.enqueue(kcenon::common::interfaces::log_level::error,
                      "Error with location", __FILE__, __LINE__, __func__,
                      std::chrono::system_clock::now());

    // At this point the queue is empty, but its message has not been written.
    if (write_started.wait_for(5s) != std::future_status::ready) {
        release.set_value();
        collector.stop();
        FAIL() << "The collector did not start writing";
    }

    std::promise<void> flush_started;
    auto started = flush_started.get_future();
    auto flushed = std::async(std::launch::async, [&] {
        flush_started.set_value();
        collector.flush();
    });
    started.wait();
    EXPECT_EQ(flushed.wait_for(100ms), std::future_status::timeout);

    // Always unblock the writer, including when the assertion above fails.
    release.set_value();
    EXPECT_EQ(flushed.wait_for(5s), std::future_status::ready);
    flushed.get();
    EXPECT_TRUE(writer->write_finished.load());
    EXPECT_FALSE(writer->flushed_before_write_finished.load());
    collector.stop();
}

}  // namespace
