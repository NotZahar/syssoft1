#include <gtest/gtest.h>

#include "print.cpp"

TEST(Print, print)
{
    Print p("AAA");
    ASSERT_EQ(p.getStr(), "AAA");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}