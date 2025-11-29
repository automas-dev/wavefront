#include <gtest/gtest.h>

#include <string>
using namespace std;

#include <WavefrontParser.hpp>

TEST(TrimStringTest, Both) {
    string s = " a b ";
    wavefront::trimString(s);
    EXPECT_EQ("a b", s);
}

TEST(TrimStringTest, Left) {
    string s = " a b";
    wavefront::trimString(s);
    EXPECT_EQ("a b", s);
}

TEST(TrimStringTest, Right) {
    string s = "a b ";
    wavefront::trimString(s);
    EXPECT_EQ("a b", s);
}

TEST(TrimStringTest, NoTrim) {
    string s = "a b";
    wavefront::trimString(s);
    EXPECT_EQ("a b", s);
}
