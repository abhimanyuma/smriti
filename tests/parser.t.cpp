#include <gtest/gtest.h>

#include "parser.h"
#include "resp_value.h"

namespace Smriti {

TEST(ParserTest, UnimplementedType) {
    std::string data{"%0\r\n"};
    auto result = Parser{std::move(data)}.parse();

    EXPECT_FALSE(result.has_value());
}

TEST(ParserTest, SimpleString) {
    std::string data{"+OK\r\n"};
    auto result = Parser{std::move(data)}.parse();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::SimpleString);
    EXPECT_EQ(result->as_string(), "OK");
}


TEST(ParserTest, ErrorString) {
    std::string data{"-ERR We have no value here\r\n"};
    auto result = Parser{std::move(data)}.parse();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Error);
    std::cout << "Result" << result->as_string();
    EXPECT_EQ(result->as_string(), "ERR We have no value here");
}

TEST(ParserTest, PostiveInteger) {
    std::string data{":+34\r\n"};
    auto result = Parser{std::move(data)}.parse();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Integer);
    EXPECT_EQ(result->as_integer(), 34);

}


TEST(ParserTest, NegativeInteger) {
    std::string data{":-34\r\n"};
    auto result = Parser{std::move(data)}.parse();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Integer);
    EXPECT_EQ(result->as_integer(), -34);
}

TEST(ParserTest, ZeroInteger) {
    std::string data{":0\r\n"};
    auto result = Parser{std::move(data)}.parse();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Integer);
    EXPECT_EQ(result->as_integer(), 0);
}

TEST(ParserTest, ImplicitPositiveInteger) {
    std::string data{":34\r\n"};
    auto result = Parser{std::move(data)}.parse();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Integer);
    EXPECT_EQ(result->as_integer(), 34);
}

// TEST(ParserTest, ZeroBulkString) {
//     std::string data{"$0\r\n\r\n"};
//     auto result = Parser{std::move(data)}.parse();

//     EXPECT_TRUE(result.has_value());
//     EXPECT_EQ(result->type(), RespTypeEnum::BulkString);
//     EXPECT_EQ(result->as_string(), "");
// }


// TEST(ParserTest, NullBulkString) {
//     std::string data{"$-1\r\n"};

//     auto result = Parser{std::move(data)}.parse();
//     EXPECT_TRUE(result.has_value());
//     EXPECT_EQ(result->type(), RespTypeEnum::Null);
// }

// TEST(ParserTest, CorrectString) {
//     std::string data{"$20\r\nabcdefghijklmnopqrst\r\n"};

//     auto result = Parser{std::move(data)}.parse();
//     EXPECT_TRUE(result.has_value());
//     EXPECT_EQ(result->type(), RespTypeEnum::BulkString);

//     std::string_view expected_string = "abcdefghijklmnopqrst";
//     EXPECT_EQ(result->as_string(), expected_string);
// }

// TEST(ParserTest, NullArray) {
//     const char* orig_data = "*-1\r\n";
//     size_t length = 5; // Length of "*-1\r\n"

//     char* data = const_cast<char*>(orig_data);
//     auto result = parse(data, length);

//     EXPECT_TRUE(result.has_value());
//     EXPECT_EQ(result->type(), RespTypeEnum::Null);
// }


// TEST(ParserTest, SingleElementArray) {
//     const char* orig_data = "*1\r\n$5\r\nhello\r\n";
//     size_t length = 20; // Length of "*1\r\n$5\r\nhello\r\n"

//     char* data = const_cast<char*>(orig_data);
//     auto result = parse(data, length);

//     EXPECT_TRUE(result.has_value());
//     EXPECT_EQ(result->type(), RespTypeEnum::Array);
//     EXPECT_EQ(result->as_array().size(), 1);
//     EXPECT_EQ(result->as_array()[0].as_string(), "hello");
// }

// TEST(ParserTest, NumArray) {
//     const char* orig_data = "*1\r\n:1\r\n";
//     size_t length = 8; // Length of "*1\r\n:1\r\n"

//     char* data = const_cast<char*>(orig_data);
//     auto result = parse(data, length);

//     EXPECT_TRUE(result.has_value());
//     EXPECT_EQ(result->type(), RespTypeEnum::Array);
//     EXPECT_EQ(result->as_array().size(), 1);
//     EXPECT_EQ(result->as_array()[0].as_integer(), 1);
// }

// TEST(ParserTest, TwoNumArray) {
//     const char* orig_data = "*3\r\n:1\r\n:23\r\n:45\r\n";
//     size_t length = 18; // Length of "*2\r\n:1\r\n:23\r\n:45\r\n"

//     char* data = const_cast<char*>(orig_data);
//     auto result = parse(data, length);

//     EXPECT_TRUE(result.has_value());
//     EXPECT_EQ(result->type(), RespTypeEnum::Array);
//     EXPECT_EQ(result->as_array().size(), 3);
//     EXPECT_EQ(result->as_array()[0].as_integer(), 1);
//     EXPECT_EQ(result->as_array()[2].as_integer(), 45);
// }

// TEST(ParserTest, MultiElementArray) {
//     const char* orig_data = "*4\r\n+HELLO\r\n:23\r\n-Err\r\n$4\r\nBEEF\r\n";

//     char* data = const_cast<char*>(orig_data);
//     auto result = parse(data, length);

//     EXPECT_TRUE(result.has_value());
//     EXPECT_EQ(result->type(), RespTypeEnum::Array);
//     EXPECT_EQ(result->as_array().size(), 4);
//     EXPECT_EQ(result->as_array()[0].as_string(), "HELLO");
//     EXPECT_EQ(result->as_array()[1].as_integer(), 23);
//     EXPECT_EQ(result->as_array()[2].as_string(), "Err");
//     EXPECT_EQ(result->as_array()[3].as_string(), "BEEF");
// }

} // namespace Smriti