#include <gtest/gtest.h>

#include <string>
using namespace std;

#include <wavefront/Parser.hpp>

TEST(SplitStringSpaceTest, EmptyString) {
    auto split = wavefront::splitStringSpace("");
    ASSERT_EQ(0, split.size());
}

TEST(SplitStringSpaceTest, NoDelimiter) {
    auto split = wavefront::splitStringSpace("foo");
    ASSERT_EQ(1, split.size());
    EXPECT_EQ("foo", split[0]);
}

TEST(SplitStringSpaceTest, SingleDelimiter) {
    auto split = wavefront::splitStringSpace("foo bar");
    ASSERT_EQ(2, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("bar", split[1]);
}

TEST(SplitStringSpaceTest, EmptyElements) {
    auto split = wavefront::splitStringSpace(" ");
    ASSERT_EQ(0, split.size());
}

TEST(SplitStringSpaceTest, StartEmptyElements) {
    auto split = wavefront::splitStringSpace(" bar");
    ASSERT_EQ(1, split.size());
    EXPECT_EQ("bar", split[0]);
}

TEST(SplitStringSpaceTest, EndEmptyElements) {
    auto split = wavefront::splitStringSpace("foo ");
    ASSERT_EQ(1, split.size());
    EXPECT_EQ("foo", split[0]);
}

TEST(SplitStringSpaceTest, MultipleEmptyElements) {
    auto split = wavefront::splitStringSpace("foo  bar");
    ASSERT_EQ(2, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("bar", split[1]);
}

TEST(SplitStringSpaceTest, StartMultipleEmptyElements) {
    auto split = wavefront::splitStringSpace("  bar");
    ASSERT_EQ(1, split.size());
    EXPECT_EQ("bar", split[0]);
}

TEST(SplitStringSpaceTest, EndMultipleEmptyElements) {
    auto split = wavefront::splitStringSpace("foo  ");
    ASSERT_EQ(1, split.size());
    EXPECT_EQ("foo", split[0]);
}

TEST(SplitStringSpaceTest, MaxCount) {
    auto split = wavefront::splitStringSpace("a b c", 1);
    ASSERT_EQ(2, split.size());
    EXPECT_EQ("a", split[0]);
    EXPECT_EQ("b c", split[1]);
}

TEST(SplitStringSpaceTest, MaxCountTrailingWhitespace) {
    auto split = wavefront::splitStringSpace("a   ", 1);
    ASSERT_EQ(1, split.size());
    EXPECT_EQ("a", split[0]);
}
