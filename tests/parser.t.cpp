#include <gtest/gtest.h>

#include "parser.h"
#include "resp_value.h"

namespace Smriti {

TEST(ParserTest, UnimplementedType) {
    const char* data = "*0\r\n";
    size_t length = 4; // Length of "*0\r\n"

    auto result = parse(data, length);

    EXPECT_FALSE(result.has_value());
}

TEST(ParserTest, SimpleString) {
    const char* data = "+OK\r\n";
    size_t length = 5; // Length of "*0\r\n"

    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::SimpleString);
    EXPECT_EQ(result->as_string(), "OK");
}

TEST(ParserTest, SimpleStringWrongLength) {
    const char* data = "+OK\r\n";
    size_t length = 3; // Length of "*0\r\n"

    auto result = parse(data, length);

    EXPECT_FALSE(result.has_value());
}

TEST(ParserTest, ErrorString) {
    const char* data = "-ERR unknown command 'asdf'\r\n";
    size_t length = 29; // Length of "-ERR unknown command 'asdf'\r\n"

    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Error);
    EXPECT_EQ(result->as_string(), "ERR unknown command 'asdf'");
}

TEST(ParserTest, PostiveInteger) {
    const char* data = ":+34\r\n";
    size_t length = 6; // Length of ":+34\r\n"
    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Integer);
    EXPECT_EQ(result->as_integer(), 34);
}

TEST(ParserTest, WrongLengthPostiveInteger) {
    const char* data = ":+3423231121232\r\n";
    size_t length = 5; // Length of ":+34\r\n"
    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Integer);
    // The parse will stop when length is exhausted
    EXPECT_EQ(result->as_integer(), 342);
}

TEST(ParserTest, NegativeInteger) {
    const char* data = ":-34\r\n";
    size_t length = 6; // Length of ":-34\r\n"
    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Integer);
    EXPECT_EQ(result->as_integer(), -34);
}

TEST(ParserTest, ZeroInteger) {
    const char* data = ":0\r\n";
    size_t length = 4; // Length of ":0\r\n"
    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Integer);
    EXPECT_EQ(result->as_integer(), 0);
}

TEST(ParserTest, ImplicitPositiveInteger) {
    const char* data = ":34\r\n";
    size_t length = 5; // Length of ":34\r\n"
    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Integer);
    EXPECT_EQ(result->as_integer(), 34);
}

TEST(ParserTest, ZeroBulkString) {
    const char* data = "$0\r\n\r\n";
    size_t length = 6; // Length of "$0\r\n\r\n"
    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::BulkString);
    EXPECT_EQ(result->as_string(), "");
}


TEST(ParserTest, NullBulkString) {
    const char* data = "$-1\r\n";
    size_t length = 5; // Length of "$-1\r\n"
    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Null);
}

TEST(ParserTest, CorrectString) {
    const char* data = "$20\r\nabcdefghijklmnopqrst\r\n";
    size_t length = 27; // Length of "$20\r\nabcdefghijklmnopqrst\r\n"
    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::BulkString);

    std::string_view expected_string = "abcdefghijklmnopqrst";
    EXPECT_EQ(result->as_string(), expected_string);
}

TEST(ParserTest, MemoryIncorrectString) {
    const char* data = "$25\r\nabcdefghijklmnopqrst\r\n";
    size_t length = 27; // Length of "$20\r\nabcdefghijklmnopqrst\r\n"
    auto result = parse(data, length);

    EXPECT_FALSE(result.has_value());
}

} // namespace Smriti