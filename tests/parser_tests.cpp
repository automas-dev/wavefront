#include <gtest/gtest.h>

#include <string>
using namespace std;

#include <WavefrontParser.hpp>

TEST(SplitStringTest, EmptyString) {
    auto split = wavefront::splitString("", ',');
    EXPECT_EQ(1, split.size());
    EXPECT_EQ("", split[0]);
}

TEST(SplitStringTest, NoDelimiter) {
    auto split = wavefront::splitString("foo", ',');
    EXPECT_EQ(1, split.size());
    EXPECT_EQ("foo", split[0]);
}

TEST(SplitStringTest, SingleDelimiter) {
    auto split = wavefront::splitString("foo,bar", ',');
    EXPECT_EQ(2, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("bar", split[1]);
}

TEST(SplitStringTest, EmptyElements) {
    auto split = wavefront::splitString(",", ',');
    EXPECT_EQ(2, split.size());
    EXPECT_EQ("", split[0]);
    EXPECT_EQ("", split[1]);
}

TEST(SplitStringTest, StartEmptyElements) {
    auto split = wavefront::splitString(",bar", ',');
    EXPECT_EQ(2, split.size());
    EXPECT_EQ("", split[0]);
    EXPECT_EQ("bar", split[1]);
}

TEST(SplitStringTest, EndEmptyElements) {
    auto split = wavefront::splitString("foo,", ',');
    EXPECT_EQ(2, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("", split[1]);
}

TEST(SplitStringTest, MultipleEmptyElements) {
    auto split = wavefront::splitString("foo,,bar", ',');
    EXPECT_EQ(3, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("", split[1]);
    EXPECT_EQ("bar", split[2]);
}

TEST(SplitStringTest, StartMultipleEmptyElements) {
    auto split = wavefront::splitString(",,bar", ',');
    EXPECT_EQ(3, split.size());
    EXPECT_EQ("", split[0]);
    EXPECT_EQ("", split[1]);
    EXPECT_EQ("bar", split[2]);
}

TEST(SplitStringTest, EndMultipleEmptyElements) {
    auto split = wavefront::splitString("foo,,", ',');
    EXPECT_EQ(3, split.size());
    EXPECT_EQ("foo", split[0]);
    EXPECT_EQ("", split[1]);
    EXPECT_EQ("", split[2]);
}

TEST(SplitStringTest, MaxCount) {
    auto split = wavefront::splitString("a,b,c", ',', 1);
    EXPECT_EQ(2, split.size());
    EXPECT_EQ("a", split[0]);
    EXPECT_EQ("b,c", split[1]);
}
