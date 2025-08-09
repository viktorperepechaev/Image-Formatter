#include <gtest/gtest.h>

#include "../include/imageformatter/Image.hpp"
#include "../include/imageformatter/DarkenOperation.hpp"

TEST(TrivialTest, Trivial) { EXPECT_TRUE(2 == 2); }

TEST(Image, ValidLoading) {
  EXPECT_NO_THROW(Image("Cute.jpg"));
}

TEST(DarkenOperation, ArgumentValidation) {
  // Valid parameters
  EXPECT_TRUE(DarkenOperation::ValidateArguments({"50"}));
  EXPECT_TRUE(DarkenOperation::ValidateArguments({"0"}));
  EXPECT_TRUE(DarkenOperation::ValidateArguments({"100"}));

  // Invalid parameters
  EXPECT_FALSE(DarkenOperation::ValidateArguments({"150"}));
  EXPECT_FALSE(DarkenOperation::ValidateArguments({"abc"}));
  EXPECT_FALSE(DarkenOperation::ValidateArguments({}));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
