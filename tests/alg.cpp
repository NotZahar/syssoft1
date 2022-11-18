#include <gtest/gtest.h>

#include "validate.cpp"
#include "translator.cpp"

TEST(Check, isNumber) {
    EXPECT_EQ(syssoft1::Validate::containsNumber("  123   "), true);
    EXPECT_EQ(syssoft1::Validate::containsNumber(" 1 123   "), false);
    EXPECT_EQ(syssoft1::Validate::containsNumber("  1234567899   "), false);
    EXPECT_EQ(syssoft1::Validate::containsNumber("  123456789   "), true);
    EXPECT_EQ(syssoft1::Validate::containsNumber("  0xa123456789   "), false);
    EXPECT_EQ(syssoft1::Validate::containsNumber("0x1ad456f"), true);
    EXPECT_EQ(syssoft1::Validate::containsNumber("  01234567   "), true);
    EXPECT_EQ(syssoft1::Validate::containsNumber(" sd234\"jn98872^^@#$%^& 01234567  . "), false);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}