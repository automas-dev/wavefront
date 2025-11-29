#include <gtest/gtest.h>

#include <string>
using namespace std;

#include <WavefrontParser.hpp>
#include <sstream>

TEST(StreamTest, DoesItEnd) {
    std::stringstream s("a");
    EXPECT_TRUE(s);
    EXPECT_EQ('a', s.get());
    EXPECT_TRUE(s);
    EXPECT_EQ(EOF, s.peek());
    EXPECT_EQ(EOF, s.get());
    EXPECT_FALSE(s);
}

TEST(SplitStringTest, EmptyString) {
    auto split = wavefront::splitString("", ',');
    ASSERT_EQ(1, split.size());
    EXPECT_EQ("", split[0]);
}

TEST(SplitStringTest, NoDelimiter) {
    auto split = wavefront::splitString("foo", ',');
    ASSERT_EQ(1, split.size());
    EXPECT_EQ("foo", split[0]);
}

TEST(SplitStringTest, SingleDelimiter) {
    auto split = wavefront::splitString("foo,bar", ',');
    ASSERT_EQ(2, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("bar", split[1]);
}

TEST(SplitStringTest, EmptyElements) {
    auto split = wavefront::splitString(",", ',');
    ASSERT_EQ(2, split.size());
    EXPECT_EQ("", split[0]);
    EXPECT_EQ("", split[1]);
}

TEST(SplitStringTest, StartEmptyElements) {
    auto split = wavefront::splitString(",bar", ',');
    ASSERT_EQ(2, split.size());
    EXPECT_EQ("", split[0]);
    EXPECT_EQ("bar", split[1]);
}

TEST(SplitStringTest, EndEmptyElements) {
    auto split = wavefront::splitString("foo,", ',');
    ASSERT_EQ(2, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("", split[1]);
}

TEST(SplitStringTest, MultipleEmptyElements) {
    auto split = wavefront::splitString("foo,,bar", ',');
    ASSERT_EQ(3, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("", split[1]);
    EXPECT_EQ("bar", split[2]);
}

TEST(SplitStringTest, StartMultipleEmptyElements) {
    auto split = wavefront::splitString(",,bar", ',');
    ASSERT_EQ(3, split.size());
    EXPECT_EQ("", split[0]);
    EXPECT_EQ("", split[1]);
    EXPECT_EQ("bar", split[2]);
}

TEST(SplitStringTest, EndMultipleEmptyElements) {
    auto split = wavefront::splitString("foo,,", ',');
    ASSERT_EQ(3, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("", split[1]);
    EXPECT_EQ("", split[2]);
}

TEST(SplitStringTest, MaxCount) {
    auto split = wavefront::splitString("a,b,c", ',', 1);
    ASSERT_EQ(2, split.size());
    EXPECT_EQ("a", split[0]);
    EXPECT_EQ("b,c", split[1]);
}
