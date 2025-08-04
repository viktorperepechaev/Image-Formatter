#include <gtest/gtest.h>

TEST(TrivialTest, Trivial) { EXPECT_TRUE(2 == 2); }

// Точка входа для GTest
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
