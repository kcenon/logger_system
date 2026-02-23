// BSD 3-Clause License
// Copyright (c) 2025, kcenon
// See the LICENSE file in the project root for full license information.

/**
 * @file di_container_test.cpp
 * @brief Unit tests validating logger_system integration with common_system's
 *        service_container for dependency injection.
 *
 * These tests verify that logger types (base_writer, log_writer_interface)
 * can be registered and resolved through the unified service_container,
 * replacing the legacy lightweight_di_container.
 */

#include <gtest/gtest.h>

#include <kcenon/common/di/service_container.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/log_writer_interface.h>
#include <kcenon/logger/writers/base_writer.h>

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

using namespace kcenon::common::di;
using namespace kcenon::logger;

// Mock writer for testing service_container with logger types
class mock_writer : public base_writer {
private:
    std::string name_;
    static std::atomic<int> instance_count_;

public:
    explicit mock_writer(const std::string& name = "mock") : name_(name)
    {
        ++instance_count_;
    }

    ~mock_writer() override { --instance_count_; }

    kcenon::common::VoidResult write(const log_entry& /*entry*/) override
    {
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult flush() override { return kcenon::common::ok(); }

    bool is_healthy() const override { return true; }

    std::string get_name() const override { return name_; }

    static int get_instance_count() { return instance_count_.load(); }

    static void reset_instance_count() { instance_count_.store(0); }
};

std::atomic<int> mock_writer::instance_count_{0};

// Second mock for multi-type registration tests
class another_mock_writer : public base_writer {
public:
    kcenon::common::VoidResult write(const log_entry& /*entry*/) override
    {
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult flush() override { return kcenon::common::ok(); }
    bool is_healthy() const override { return true; }
    std::string get_name() const override { return "another"; }
};

// Test fixture
class di_container_test : public ::testing::Test {
protected:
    service_container container_;

    void SetUp() override { mock_writer::reset_instance_count(); }

    void TearDown() override
    {
        container_.clear();
        mock_writer::reset_instance_count();
    }
};

// --- Registration Tests ---

TEST_F(di_container_test, RegisterSimpleFactory_Singleton_Succeeds)
{
    auto result = container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>("singleton"); },
        service_lifetime::singleton);

    EXPECT_TRUE(result.is_ok());
    EXPECT_TRUE(container_.is_registered<base_writer>());
}

TEST_F(di_container_test, RegisterSimpleFactory_Transient_Succeeds)
{
    auto result = container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>("transient"); },
        service_lifetime::transient);

    EXPECT_TRUE(result.is_ok());
    EXPECT_TRUE(container_.is_registered<base_writer>());
}

TEST_F(di_container_test, RegisterInstance_Succeeds)
{
    auto instance = std::make_shared<mock_writer>("singleton_instance");

    auto result = container_.register_instance<base_writer>(instance);

    EXPECT_TRUE(result.is_ok());
    EXPECT_TRUE(container_.is_registered<base_writer>());
}

TEST_F(di_container_test, RegisterFactory_WithContainerAccess_Succeeds)
{
    auto result = container_.register_factory<base_writer>(
        [](IServiceContainer&) { return std::make_shared<mock_writer>("factory"); },
        service_lifetime::transient);

    EXPECT_TRUE(result.is_ok());
    EXPECT_TRUE(container_.is_registered<base_writer>());
}

TEST_F(di_container_test, RegisterDuplicate_Fails)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>(); },
        service_lifetime::singleton);

    auto result = container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>(); },
        service_lifetime::singleton);

    EXPECT_TRUE(result.is_err());
}

// --- Resolution Tests ---

TEST_F(di_container_test, Resolve_Singleton_ReturnsSameInstance)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>("singleton"); },
        service_lifetime::singleton);

    auto result1 = container_.resolve<base_writer>();
    auto result2 = container_.resolve<base_writer>();

    ASSERT_TRUE(result1.is_ok());
    ASSERT_TRUE(result2.is_ok());
    EXPECT_EQ(result1.value(), result2.value());
}

TEST_F(di_container_test, Resolve_Transient_ReturnsDifferentInstances)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>("transient"); },
        service_lifetime::transient);

    auto result1 = container_.resolve<base_writer>();
    auto result2 = container_.resolve<base_writer>();

    ASSERT_TRUE(result1.is_ok());
    ASSERT_TRUE(result2.is_ok());
    EXPECT_NE(result1.value(), result2.value());
}

TEST_F(di_container_test, Resolve_Instance_ReturnsSameObject)
{
    auto instance = std::make_shared<mock_writer>("registered");
    container_.register_instance<base_writer>(instance);

    auto result = container_.resolve<base_writer>();

    ASSERT_TRUE(result.is_ok());
    EXPECT_EQ(result.value(), instance);

    auto writer = std::dynamic_pointer_cast<mock_writer>(result.value());
    ASSERT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "registered");
}

TEST_F(di_container_test, Resolve_Unregistered_Fails)
{
    auto result = container_.resolve<base_writer>();

    EXPECT_TRUE(result.is_err());
}

TEST_F(di_container_test, ResolveOrNull_Unregistered_ReturnsNull)
{
    auto ptr = container_.resolve_or_null<base_writer>();

    EXPECT_EQ(ptr, nullptr);
}

TEST_F(di_container_test, ResolveOrNull_Registered_ReturnsInstance)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>(); },
        service_lifetime::singleton);

    auto ptr = container_.resolve_or_null<base_writer>();

    EXPECT_NE(ptr, nullptr);
}

// --- Factory Resolution Tests ---

TEST_F(di_container_test, Resolve_FactoryWithContainer_CreatesInstance)
{
    container_.register_factory<base_writer>(
        [](IServiceContainer&) { return std::make_shared<mock_writer>("from_factory"); },
        service_lifetime::transient);

    auto result = container_.resolve<base_writer>();

    ASSERT_TRUE(result.is_ok());
    auto writer = std::dynamic_pointer_cast<mock_writer>(result.value());
    ASSERT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "from_factory");
}

TEST_F(di_container_test, Resolve_SimpleFactory_CreatesInstance)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>("simple"); },
        service_lifetime::transient);

    auto result = container_.resolve<base_writer>();

    ASSERT_TRUE(result.is_ok());
    auto writer = std::dynamic_pointer_cast<mock_writer>(result.value());
    ASSERT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "simple");
}

// --- Clear and Unregister Tests ---

TEST_F(di_container_test, Clear_RemovesAllRegistrations)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>(); },
        service_lifetime::singleton);

    EXPECT_TRUE(container_.is_registered<base_writer>());

    container_.clear();

    EXPECT_FALSE(container_.is_registered<base_writer>());
}

TEST_F(di_container_test, Unregister_RemovesSpecificType)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>(); },
        service_lifetime::singleton);

    EXPECT_TRUE(container_.is_registered<base_writer>());

    auto result = container_.unregister<base_writer>();
    EXPECT_TRUE(result.is_ok());
    EXPECT_FALSE(container_.is_registered<base_writer>());
}

// --- Thread Safety Tests ---

TEST_F(di_container_test, ConcurrentResolve_Singleton_ThreadSafe)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>("singleton"); },
        service_lifetime::singleton);

    const int num_threads = 8;
    const int ops_per_thread = 100;
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<base_writer>> results(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(
            [this, &results, i, ops_per_thread]()
            {
                for (int j = 0; j < ops_per_thread; ++j) {
                    auto result = container_.resolve<base_writer>();
                    EXPECT_TRUE(result.is_ok());
                    if (j == 0) {
                        results[i] = result.value();
                    }
                }
            });
    }

    for (auto& t : threads) {
        t.join();
    }

    // All threads should get the same singleton instance
    for (int i = 1; i < num_threads; ++i) {
        EXPECT_EQ(results[0], results[i]);
    }
}

TEST_F(di_container_test, ConcurrentResolve_Transient_ThreadSafe)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>("transient"); },
        service_lifetime::transient);

    const int num_threads = 8;
    const int ops_per_thread = 50;
    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(
            [this, &success_count, ops_per_thread]()
            {
                for (int j = 0; j < ops_per_thread; ++j) {
                    auto result = container_.resolve<base_writer>();
                    if (result.is_ok()) {
                        ++success_count;
                    }
                }
            });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(success_count.load(), num_threads * ops_per_thread);
}

// --- Service Descriptor Tests ---

TEST_F(di_container_test, RegisteredServices_ReturnsDescriptors)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>(); },
        service_lifetime::singleton);

    auto services = container_.registered_services();

    EXPECT_GE(services.size(), 1u);
}

// --- Freeze Tests ---

TEST_F(di_container_test, Freeze_PreventsNewRegistrations)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>(); },
        service_lifetime::singleton);
    container_.freeze();

    EXPECT_TRUE(container_.is_frozen());

    auto result = container_.register_simple_factory<log_writer_interface>(
        []() { return std::make_shared<another_mock_writer>(); },
        service_lifetime::singleton);

    EXPECT_TRUE(result.is_err());
}

TEST_F(di_container_test, Freeze_AllowsResolution)
{
    container_.register_simple_factory<base_writer>(
        []() { return std::make_shared<mock_writer>(); },
        service_lifetime::singleton);
    container_.freeze();

    auto result = container_.resolve<base_writer>();
    EXPECT_TRUE(result.is_ok());
}
