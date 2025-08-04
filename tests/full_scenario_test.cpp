#include <gtest/gtest.h>

#include "../include/imageformatter/Image.hpp"

TEST(TrivialTest, Trivial) { EXPECT_TRUE(2 == 2); }

TEST(ImageTest, ValidLoading) {
  EXPECT_NO_THROW(Image("Cute.jpg"));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
