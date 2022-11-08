#include <gtest/gtest.h>

#include "print.hpp"

TEST(Print, print)
{
    Print p("Hitler");
    ASSERT_EQ(p.getStr(), "Hitler");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}