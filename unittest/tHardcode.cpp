/// Hardcodes a 3x3 matrix, and verifies basic sanity of our algorithms by
/// checking each value against a hard-coded gold.

#include "sauerkraut/Algo.hpp"
#include "gtest/gtest.h"

using namespace sauerkraut;

class HardcodeFixture : public ::testing::Test {
protected:
  int mul1[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},
      mul2[3][3] = {{10, 11, 12}, {13, 14, 15}, {16, 17, 18}}, res[3][3] = {0};
  void validate(int res[3][3]) {
    ASSERT_EQ(res[0][0], 84);
    ASSERT_EQ(res[0][1], 90);
    ASSERT_EQ(res[0][2], 96);
    ASSERT_EQ(res[1][0], 201);
    ASSERT_EQ(res[1][1], 216);
    ASSERT_EQ(res[1][2], 231);
    ASSERT_EQ(res[2][0], 318);
    ASSERT_EQ(res[2][1], 342);
    ASSERT_EQ(res[2][2], 366);
  }
};

TEST_F(HardcodeFixture, BasicSerial) {
  serialMultiply(mul1, mul2, res);
  validate(res);
}

TEST_F(HardcodeFixture, BasicTranspose) {
  transposeMultiply(mul1, mul2, res);
  validate(res);
}

TEST_F(HardcodeFixture, TiledMultiply) {
  tiledMultiply(mul1, mul2, res);
  validate(res);
}
