#include <gtest/gtest.h>

#include <string>
using namespace std;

#include <wavefront/Parser.hpp>

TEST(ParserTest, TwoLines) {
    istringstream is("one\ntwo");
    auto parser = wavefront::Parser(is);
    wavefront::Parser::Token token;
    parser.read(token);
    EXPECT_EQ("one", token.key);
    parser.read(token);
    EXPECT_EQ("two", token.key);
    EXPECT_FALSE(parser.hasNext());
}

TEST(ParserTest, Space) {
    istringstream is("one two");
    auto parser = wavefront::Parser(is);
    wavefront::Parser::Token token;
    parser.read(token);
    EXPECT_EQ("one", token.key);
    auto p = token.params();
    ASSERT_EQ(1, p.size());
    EXPECT_EQ("two", p[0]);
    EXPECT_FALSE(parser.hasNext());
}

TEST(ParserTest, MultipleSpaces) {
    istringstream is("one  two");
    auto parser = wavefront::Parser(is);
    wavefront::Parser::Token token;
    parser.read(token);
    EXPECT_EQ("one", token.key);
    auto p = token.params();
    ASSERT_EQ(1, p.size());
    EXPECT_EQ("two", p[0]);
    EXPECT_FALSE(parser.hasNext());
}

TEST(ParserTest, TrailingSpace) {
    istringstream is("one two ");
    auto parser = wavefront::Parser(is);
    wavefront::Parser::Token token;
    parser.read(token);
    EXPECT_EQ("one", token.key);
    auto p = token.params();
    ASSERT_EQ(1, p.size());
    EXPECT_EQ("two", p[0]);
    EXPECT_FALSE(parser.hasNext());
}

TEST(ParserTest, Comment) {
    istringstream is("#one");
    auto parser = wavefront::Parser(is);
    wavefront::Parser::Token token;
    parser.read(token);
    EXPECT_EQ("", token.key);
    EXPECT_FALSE(parser.hasNext());
}

TEST(ParserTest, TrailingComment) {
    istringstream is("one # two");
    auto parser = wavefront::Parser(is);
    wavefront::Parser::Token token;
    parser.read(token);
    EXPECT_EQ("one", token.key);
    EXPECT_FALSE(parser.hasNext());
}

TEST(ParserTest, HasNext) {
    istringstream is("one\ntwo");
    auto parser = wavefront::Parser(is);
    wavefront::Parser::Token token;
    ASSERT_TRUE(parser.hasNext());
    parser.read(token);
    EXPECT_EQ("one", token.key);
    ASSERT_TRUE(parser.hasNext());
    parser.read(token);
    EXPECT_EQ("two", token.key);
    EXPECT_FALSE(parser.hasNext());
}

TEST(ParserTest, EOFHasNext) {
    istringstream is("");
    is.get();
    ASSERT_FALSE(is);
    auto parser = wavefront::Parser(is);
    ASSERT_FALSE(parser.hasNext());
}

TEST(ParserTest, EmptyStream) {
    istringstream is("");
    auto parser = wavefront::Parser(is);
    EXPECT_EQ(parser.begin(), parser.end());
}
