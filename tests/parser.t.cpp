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


TEST(ParserTest, ErrorString) {
    const char* data = "-ERR unknown command 'asdf'\r\n";
    size_t length = 29; // Length of "-ERR unknown command 'asdf'\r\n"

    auto result = parse(data, length);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), RespTypeEnum::Error);
    EXPECT_EQ(result->as_string(), "ERR unknown command 'asdf'");
}

} // namespace Smriti