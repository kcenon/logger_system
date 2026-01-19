// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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

    // Implement the legacy write interface required by base_writer
    // Note: base_writer::write uses kcenon::common::interfaces::log_level for backward compatibility
    kcenon::common::VoidResult write(kcenon::common::interfaces::log_level level,
                                     const std::string& message,
                                     [[maybe_unused]] const std::string& file,
                                     [[maybe_unused]] int line,
                                     [[maybe_unused]] const std::string& function,
                                     [[maybe_unused]] const std::chrono::system_clock::time_point& timestamp) override {
        entry_record rec;
        // Convert to kcenon::common::interfaces::log_level for storage
        rec.level = static_cast<ci::log_level>(static_cast<int>(level));
        rec.message = message;
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
