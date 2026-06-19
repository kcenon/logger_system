// BSD 3-Clause License
// Copyright (c) 2025, kcenon
// See the LICENSE file in the project root for full license information.

/**
 * @file small_string_test.cpp
 * @brief Unit tests for kcenon::logger::small_string
 *
 * Exercises both the small (SSO) and heap storage branches of
 * construction, assignment, append, reserve, and comparison helpers.
 */

#include <gtest/gtest.h>

#include <kcenon/logger/core/small_string.h>

#include <string>
#include <string_view>
#include <utility>

using namespace kcenon::logger;

// Use a compact 16-byte instance so the SSO/heap boundary is easy to
// exercise from tests without allocating enormous payloads.
using sstr16 = small_string<16>;

// =============================================================================
// Construction
// =============================================================================

TEST(SmallStringTest, DefaultConstructIsEmptyAndSmall) {
    sstr16 s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
    EXPECT_EQ(s.length(), 0u);
    EXPECT_TRUE(s.is_small());
    EXPECT_STREQ(s.c_str(), "");
}

TEST(SmallStringTest, ConstructFromCString) {
    sstr16 s("hello");
    EXPECT_EQ(s.size(), 5u);
    EXPECT_TRUE(s.is_small());
    EXPECT_EQ(s.to_string(), "hello");
}

TEST(SmallStringTest, ConstructFromNullCStringYieldsEmpty) {
    const char* p = nullptr;
    sstr16 s(p);
    EXPECT_TRUE(s.empty());
    EXPECT_TRUE(s.is_small());
}

TEST(SmallStringTest, ConstructFromStdString) {
    std::string input("world");
    sstr16 s(input);
    EXPECT_EQ(s.size(), input.size());
    EXPECT_TRUE(s.is_small());
    EXPECT_EQ(s.to_string(), input);
}

TEST(SmallStringTest, ConstructFromStringView) {
    std::string_view sv = "sview";
    sstr16 s(sv);
    EXPECT_EQ(s.size(), sv.size());
    EXPECT_TRUE(s.is_small());
}

TEST(SmallStringTest, ConstructLongStringUsesHeap) {
    // SSO capacity for sstr16 is 15 (16 - 1). 40 chars must spill to heap.
    std::string long_str(40, 'x');
    sstr16 s(long_str);
    EXPECT_EQ(s.size(), long_str.size());
    EXPECT_FALSE(s.is_small());
    EXPECT_EQ(s.to_string(), long_str);
}

// =============================================================================
// Copy / move
// =============================================================================

TEST(SmallStringTest, CopyConstructSmall) {
    sstr16 original("abc");
    sstr16 copy(original);

    EXPECT_EQ(copy.to_string(), "abc");
    EXPECT_TRUE(copy.is_small());
    // Original remains valid and unchanged
    EXPECT_EQ(original.to_string(), "abc");
}

TEST(SmallStringTest, CopyConstructHeap) {
    std::string big(64, 'a');
    sstr16 original(big);
    sstr16 copy(original);

    EXPECT_EQ(copy.size(), big.size());
    EXPECT_FALSE(copy.is_small());
    EXPECT_EQ(copy.to_string(), big);
    EXPECT_EQ(original.to_string(), big);
}

TEST(SmallStringTest, MoveConstructHeapLeavesSourceSmall) {
    std::string big(64, 'b');
    sstr16 source(big);
    sstr16 dest(std::move(source));

    EXPECT_EQ(dest.size(), big.size());
    EXPECT_EQ(dest.to_string(), big);
    // Source should be reset to an empty small-string state
    EXPECT_TRUE(source.is_small());
    EXPECT_TRUE(source.empty());
}

TEST(SmallStringTest, MoveConstructSmall) {
    sstr16 source("xy");
    sstr16 dest(std::move(source));

    EXPECT_EQ(dest.to_string(), "xy");
    EXPECT_TRUE(dest.is_small());
    // Small-string moves are essentially copies, so source state is
    // implementation-defined after move. We only assert on dest.
}

TEST(SmallStringTest, CopyAssignmentReplacesContent) {
    sstr16 a("one");
    sstr16 b("two");
    a = b;
    EXPECT_EQ(a.to_string(), "two");
}

TEST(SmallStringTest, CopyAssignmentSelfIsNoop) {
    sstr16 a("keep");
    // Route through a reference to avoid -Wself-assign-overloaded warnings
    // while still exercising the same-object branch in operator=.
    sstr16& alias = a;
    a = alias;
    EXPECT_EQ(a.to_string(), "keep");
}

TEST(SmallStringTest, MoveAssignmentSmallToHeap) {
    sstr16 small_val("tiny");
    sstr16 heap_val(std::string(40, 'Z'));
    small_val = std::move(heap_val);

    EXPECT_FALSE(small_val.is_small());
    EXPECT_EQ(small_val.size(), 40u);
    EXPECT_TRUE(heap_val.is_small());
    EXPECT_TRUE(heap_val.empty());
}

TEST(SmallStringTest, MoveAssignmentHeapToSmall) {
    sstr16 heap_val(std::string(32, 'Q'));
    sstr16 small_val("hi");
    heap_val = std::move(small_val);

    EXPECT_TRUE(heap_val.is_small());
    EXPECT_EQ(heap_val.to_string(), "hi");
}

TEST(SmallStringTest, MoveAssignmentSelfIsNoop) {
    sstr16 a("keep");
    // Use a reference so compilers don't emit -Wself-move while still
    // exercising the same-object branch in operator=.
    sstr16& alias = a;
    a = std::move(alias);
    EXPECT_EQ(a.to_string(), "keep");
}

// =============================================================================
// Assign / clear
// =============================================================================

TEST(SmallStringTest, AssignThenReassignSmall) {
    sstr16 s;
    s.assign("ab", 2);
    EXPECT_EQ(s.to_string(), "ab");
    s.assign("cdef", 4);
    EXPECT_EQ(s.to_string(), "cdef");
    EXPECT_TRUE(s.is_small());
}

TEST(SmallStringTest, AssignTransitionsSmallToHeap) {
    sstr16 s("abc");
    std::string big(40, 'g');
    s.assign(big.data(), big.size());
    EXPECT_EQ(s.size(), big.size());
    EXPECT_FALSE(s.is_small());
    EXPECT_EQ(s.to_string(), big);
}

TEST(SmallStringTest, AssignShrinksBackToSmall) {
    sstr16 s(std::string(40, 'g'));
    s.assign("x", 1);
    EXPECT_EQ(s.to_string(), "x");
    EXPECT_TRUE(s.is_small());
}

TEST(SmallStringTest, ClearResetsSize) {
    sstr16 s("content");
    s.clear();
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
    EXPECT_STREQ(s.c_str(), "");
}

TEST(SmallStringTest, ClearOnHeapKeepsCapacity) {
    sstr16 s(std::string(40, 'h'));
    ASSERT_FALSE(s.is_small());
    s.clear();
    EXPECT_EQ(s.size(), 0u);
    EXPECT_FALSE(s.is_small()); // heap buffer retained
}

// =============================================================================
// Reserve / capacity
// =============================================================================

TEST(SmallStringTest, ReserveWithinSsoKeepsSmall) {
    sstr16 s("a");
    s.reserve(8); // SSO capacity is 15
    EXPECT_TRUE(s.is_small());
    EXPECT_GE(s.capacity(), 8u);
}

TEST(SmallStringTest, ReserveBeyondSsoMovesToHeap) {
    sstr16 s("abc");
    s.reserve(64);
    EXPECT_FALSE(s.is_small());
    EXPECT_GE(s.capacity(), 64u);
    EXPECT_EQ(s.to_string(), "abc");
}

TEST(SmallStringTest, ReserveNoShrink) {
    sstr16 s(std::string(40, 'q'));
    size_t before = s.capacity();
    s.reserve(1);
    EXPECT_GE(s.capacity(), before);
}

// =============================================================================
// Append
// =============================================================================

TEST(SmallStringTest, AppendStaysInSso) {
    sstr16 s("ab");
    s.append("cd", 2);
    EXPECT_EQ(s.to_string(), "abcd");
    EXPECT_TRUE(s.is_small());
}

TEST(SmallStringTest, AppendPromotesToHeap) {
    sstr16 s("abc");
    std::string tail(40, 't');
    s.append(tail.data(), tail.size());
    EXPECT_FALSE(s.is_small());
    EXPECT_EQ(s.size(), 3u + tail.size());
    EXPECT_EQ(s.to_string(), std::string("abc") + tail);
}

TEST(SmallStringTest, AppendStdStringOverload) {
    sstr16 s("hi");
    s.append(std::string("!"));
    EXPECT_EQ(s.to_string(), "hi!");
}

TEST(SmallStringTest, AppendCStringOperator) {
    sstr16 s("a");
    s += "bc";
    EXPECT_EQ(s.to_string(), "abc");
}

TEST(SmallStringTest, AppendStdStringOperator) {
    sstr16 s("a");
    s += std::string("b");
    EXPECT_EQ(s.to_string(), "ab");
}

TEST(SmallStringTest, AppendOnHeapExtendsHeap) {
    sstr16 s(std::string(40, 'a'));
    ASSERT_FALSE(s.is_small());
    std::string tail(20, 'b');
    s.append(tail.data(), tail.size());
    EXPECT_EQ(s.size(), 60u);
    EXPECT_EQ(s.to_string().substr(0, 40), std::string(40, 'a'));
    EXPECT_EQ(s.to_string().substr(40), std::string(20, 'b'));
}

// =============================================================================
// Conversions and comparison
// =============================================================================

TEST(SmallStringTest, StringViewConversion) {
    sstr16 s("view");
    std::string_view sv = s;
    EXPECT_EQ(sv.size(), 4u);
    EXPECT_EQ(sv, std::string_view("view"));
}

TEST(SmallStringTest, EqualityWithSameContent) {
    sstr16 a("same");
    sstr16 b("same");
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(SmallStringTest, EqualityBetweenSmallAndHeap) {
    std::string big(40, 'r');
    sstr16 a(big);
    sstr16 b(big);
    EXPECT_TRUE(a == b);
}

TEST(SmallStringTest, InequalityDifferentSizes) {
    sstr16 a("abc");
    sstr16 b("abcd");
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(SmallStringTest, EqualityWithStdString) {
    sstr16 s("match");
    EXPECT_TRUE(s == std::string("match"));
    EXPECT_FALSE(s == std::string("other"));
}

// =============================================================================
// Memory statistics
// =============================================================================

TEST(SmallStringTest, MemoryStatsSmall) {
    sstr16 s("short");
    auto stats = s.get_memory_stats();
    EXPECT_EQ(stats.string_size, 5u);
    EXPECT_TRUE(stats.is_small);
    EXPECT_EQ(stats.heap_bytes_used, 0u);
    EXPECT_GT(stats.total_bytes, 0u);
}

TEST(SmallStringTest, MemoryStatsHeap) {
    sstr16 s(std::string(40, 'k'));
    auto stats = s.get_memory_stats();
    EXPECT_EQ(stats.string_size, 40u);
    EXPECT_FALSE(stats.is_small);
    EXPECT_GT(stats.heap_bytes_used, 0u);
    EXPECT_GT(stats.total_bytes, stats.heap_bytes_used);
}

// =============================================================================
// Public aliases
// =============================================================================

TEST(SmallStringTest, Alias64Works) {
    small_string_64 s("payload");
    EXPECT_EQ(s.to_string(), "payload");
    EXPECT_TRUE(s.is_small());
}

TEST(SmallStringTest, Alias128Works) {
    small_string_128 s("network");
    EXPECT_EQ(s.to_string(), "network");
    EXPECT_TRUE(s.is_small());
}
