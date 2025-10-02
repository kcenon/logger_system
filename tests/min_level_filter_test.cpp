#include <gtest/gtest.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/common/interfaces/monitoring_interface.h>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

// Use thread_system log_level when integrated
#ifdef USE_THREAD_SYSTEM_INTEGRATION
    #include <kcenon/thread/interfaces/logger_interface.h>
    using log_level = kcenon::thread::log_level;
#else
    #include <kcenon/logger/interfaces/logger_types.h>
    using log_level = logger_system::log_level;
#endif

namespace {

class memory_writer final : public kcenon::logger::base_writer {
public:
    struct entry_record {
        logger_system::log_level level;
        std::string message;
    };

    // Implement the legacy write interface required by base_writer
    kcenon::logger::result_void write(logger_system::log_level level,
                                      const std::string& message,
                                      const std::string& file,
                                      int line,
                                      const std::string& function,
                                      const std::chrono::system_clock::time_point& timestamp) override {
        entry_record rec;
        rec.level = level;
        rec.message = message;
        records_.push_back(std::move(rec));
        return {};
    }

    kcenon::logger::result_void flush() override {
        return {};
    }

    std::string get_name() const override {
        return "memory_writer";
    }

    // IMonitorable interface implementation
    common::Result<common::interfaces::metrics_snapshot>
    get_monitoring_data() override {
        common::interfaces::metrics_snapshot snapshot;
        snapshot.source_id = "memory_writer";
        snapshot.capture_time = std::chrono::system_clock::now();
        return snapshot;
    }

    common::Result<common::interfaces::health_check_result>
    health_check() override {
        common::interfaces::health_check_result result;
        result.timestamp = std::chrono::system_clock::now();
        result.status = common::interfaces::health_status::healthy;
        result.message = "Memory writer operational";
        return result;
    }

    std::string get_component_name() const override {
        return "memory_writer";
    }

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

    ASSERT_TRUE(logger.add_writer(std::move(writer)));

    logger.set_min_level(log_level::warning);

    EXPECT_FALSE(logger.is_enabled(log_level::info));
    EXPECT_TRUE(logger.is_enabled(log_level::warning));

    logger.log(log_level::info, "info message");
    EXPECT_TRUE(writer_ptr->records().empty());

    logger.log(log_level::warning, "warning message");
    ASSERT_EQ(writer_ptr->records().size(), 1U);
    EXPECT_EQ(writer_ptr->records().back().level, logger_system::log_level::warning);
    EXPECT_EQ(writer_ptr->records().back().message, "warning message");

    logger.log(log_level::error, "error message");
    ASSERT_EQ(writer_ptr->records().size(), 2U);
    EXPECT_EQ(writer_ptr->records().back().level, logger_system::log_level::error);
    EXPECT_EQ(writer_ptr->records().back().message, "error message");
}
