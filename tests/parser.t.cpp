#include <gtest/gtest.h>

#include "../src/parser.h"

TEST(ParserTest, NullResponse) {
    const char* data = "*0\r\n";
    size_t length = 4; // Length of "*0\r\n"

    Smriti::RespValue response = Smriti::parse(data, length);

    EXPECT_EQ(response.type(), Smriti::RespTypeEnum::Null);
    EXPECT_TRUE(response.is_null());
}