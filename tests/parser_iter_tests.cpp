#include <gtest/gtest.h>

#include <string>
using namespace std;

#include <wavefront/Parser.hpp>
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

TEST(ParserIterTest, Emtpy) {
    istringstream is("");
    auto parser = wavefront::Parser(is);
    EXPECT_FALSE(parser.hasNext());
    auto iter = parser.begin();
    EXPECT_TRUE(iter == Parser::end());
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
