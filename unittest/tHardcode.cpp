/// Hardcodes a 3x3 matrix, and verifies basic sanity of our algorithms by
/// checking each value against a hard-coded gold.

#include "sauerkraut/Algo.hpp"
#include "gtest/gtest.h"

#include <vector>

using namespace sauerkraut;

class HardcodeFixture : public ::testing::Test {
protected:
  std::vector<int> mul1 = {1, 2, 3, 4, 5, 6, 7, 8, 9},
                   mul2 = {10, 11, 12, 13, 14, 15, 16, 17, 18}, res;

  HardcodeFixture() : res(9) {}

  /// This data was obtained by running the serial version.
  void validate() {
    ASSERT_EQ(res[0], 84);
    ASSERT_EQ(res[1], 90);
    ASSERT_EQ(res[2], 96);
    ASSERT_EQ(res[3], 201);
    ASSERT_EQ(res[4], 216);
    ASSERT_EQ(res[5], 231);
    ASSERT_EQ(res[6], 318);
    ASSERT_EQ(res[7], 342);
    ASSERT_EQ(res[8], 366);
  }
};

TEST_F(HardcodeFixture, BasicSerial) {
  serialMultiply<3>(mul1, mul2, res);
  validate();
}

TEST_F(HardcodeFixture, BasicTranspose) {
  transposeMultiply<3>(mul1, mul2, res);
  validate();
}

TEST_F(HardcodeFixture, TiledMultiply) {
  tiledMultiply<3>(mul1, mul2, res);
  validate();
}
