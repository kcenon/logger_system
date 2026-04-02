// BSD 3-Clause License
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#include <gtest/gtest.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>

#ifdef BUILD_WITH_COMMON_SYSTEM
    #include <kcenon/common/interfaces/monitoring_interface.h>
#endif

#include <chrono>
#include <memory>
#include <string>
#include <vector>

// Use kcenon::common::interfaces::log_level for ILogger interface
#include <kcenon/common/interfaces/logger_interface.h>
namespace ci = kcenon::common::interfaces;

namespace {

class memory_writer final : public kcenon::logger::base_writer {
public:
    struct entry_record {
        ci::log_level level;
        std::string message;
    };

    // Implement the modern write interface required by base_writer
    kcenon::common::VoidResult write(const kcenon::logger::log_entry& entry) override {
        entry_record rec;
        // Convert to kcenon::common::interfaces::log_level for storage
        rec.level = static_cast<ci::log_level>(static_cast<int>(entry.level));
        rec.message = entry.message.to_string();
        records_.push_back(std::move(rec));
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult flush() override {
        return kcenon::common::ok();
    }

    std::string get_name() const override {
        return "memory_writer";
    }

    // Note: IMonitorable interface removed from base_writer (Phase 4)
    // Use performance_monitor_adapter for monitoring integration if needed

    const std::vector<entry_record>& records() const {
        return records_;
    }

private:
    std::vector<entry_record> records_;
};

} // namespace

TEST(LoggerMinLevelTest, DropsMessagesBelowConfiguredThreshold) {
    kcenon::logger::logger logger(false);
    auto writer = std::make_unique<memory_writer>();
    auto* writer_ptr = writer.get();

    ASSERT_TRUE(logger.add_writer(std::move(writer)).is_ok());

    logger.set_level(ci::log_level::warning);

    EXPECT_FALSE(logger.is_enabled(ci::log_level::info));
    EXPECT_TRUE(logger.is_enabled(ci::log_level::warning));

    logger.log(ci::log_level::info, std::string("info message"));
    EXPECT_TRUE(writer_ptr->records().empty());

    logger.log(ci::log_level::warning, std::string("warning message"));
    ASSERT_EQ(writer_ptr->records().size(), 1U);
    EXPECT_EQ(writer_ptr->records().back().level, ci::log_level::warning);
    EXPECT_EQ(writer_ptr->records().back().message, "warning message");

    logger.log(ci::log_level::error, std::string("error message"));
    ASSERT_EQ(writer_ptr->records().size(), 2U);
    EXPECT_EQ(writer_ptr->records().back().level, ci::log_level::error);
    EXPECT_EQ(writer_ptr->records().back().message, "error message");
}
