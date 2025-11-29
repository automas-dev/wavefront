#include <gtest/gtest.h>

#include <string>
using namespace std;

#include <WavefrontParser.hpp>
using namespace wavefront;

TEST(ParserIterTest, NullIter) {
    EXPECT_EQ(Parser::iterator(), Parser::iterator());
    EXPECT_EQ(Parser::iterator(), Parser::iterator(nullptr));
}

TEST(ParserIterTest, Begin) {
    istringstream is("one\ntwo");
    auto parser = wavefront::Parser(is);
    auto iter = parser.begin();
    auto token = *iter;
    EXPECT_EQ("one", token.key);
    ++iter;
    token = *iter;
    EXPECT_EQ("two", token.key);
    EXPECT_EQ(Parser::end(), ++iter);
}

TEST(ParserIterTest, End) {
    EXPECT_EQ(Parser::iterator(), Parser::end());
}

TEST(ParserIterTest, Equal) {
    EXPECT_TRUE(Parser::end() == Parser::end());
}

TEST(ParserIterTest, NotEqual) {
    EXPECT_FALSE(Parser::end() != Parser::end());
}
