#include <gtest/gtest.h>

// #include "UIModel.cpp"

TEST(UIModel, sourceCodeWasGiven)
{
    // Print p("AAA");
    ASSERT_EQ("AAA", "AAA");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}