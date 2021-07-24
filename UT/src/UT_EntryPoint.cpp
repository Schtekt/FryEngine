#include <gtest/gtest.h>

TEST(DUMMYSUITE, DUMMYTEST)
{
    ASSERT_EQ(0,0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}