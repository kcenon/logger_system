#include <gtest/gtest.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/base_writer.h>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace {

class memory_writer final : public kcenon::logger::base_writer {
public:
    struct entry_record {
        logger_system::log_level level;
        std::string message;
    };

    kcenon::logger::result_void write(logger_system::log_level level,
                                      const std::string& message,
                                      const std::string& file,
                                      int line,
                                      const std::string& function,
                                      const std::chrono::system_clock::time_point& timestamp) override {
        (void)file;
        (void)line;
        (void)function;
        (void)timestamp;
        records_.push_back({level, message});
        return {};
    }

    kcenon::logger::result_void flush() override {
        return {};
    }

    std::string get_name() const override {
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

    logger.set_min_level(logger_system::log_level::warn);

    EXPECT_FALSE(logger.is_enabled(logger_system::log_level::info));
    EXPECT_TRUE(logger.is_enabled(logger_system::log_level::warn));

    logger.log(logger_system::log_level::info, "info message");
    EXPECT_TRUE(writer_ptr->records().empty());

    logger.log(logger_system::log_level::warn, "warn message");
    ASSERT_EQ(writer_ptr->records().size(), 1U);
    EXPECT_EQ(writer_ptr->records().back().level, logger_system::log_level::warn);
    EXPECT_EQ(writer_ptr->records().back().message, "warn message");

    logger.log(logger_system::log_level::error, "error message");
    ASSERT_EQ(writer_ptr->records().size(), 2U);
    EXPECT_EQ(writer_ptr->records().back().level, logger_system::log_level::error);
    EXPECT_EQ(writer_ptr->records().back().message, "error message");
}
