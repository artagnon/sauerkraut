/// Uses serialMultiply as the gold to verify operations of algorithms that need
/// to be stressed on large matrices (which can't be hardcoded).

#include "sauerkraut/Algo.hpp"
#include "gtest/gtest.h"

#include <vector>

using namespace sauerkraut;

class SerialGolding : public ::testing::Test {
protected:
  constexpr static int SZ = 512;
  std::vector<int> mul1, mul2, res, gold;

  SerialGolding() : mul1(SZ * SZ), mul2(SZ * SZ), res(SZ * SZ), gold(SZ * SZ) {}

  /// Just fill mul1 and mul2 with whatever.
  void randomizeInputMatrices() {
    std::srand(std::time(0));
    for (int i = 0; i < SZ; ++i)
      for (int j = 0; j < SZ; ++j) {
        mul1[SZ * i + j] = std::rand() % 100;
        mul2[SZ * i + j] = std::rand() % 100;
      }
  }

  void validate() {
    serialMultiply<SZ>(mul1, mul2, gold);
    for (int i = 0; i < SZ; ++i)
      for (int j = 0; j < SZ; ++j)
        ASSERT_EQ(res[SZ * i + j], gold[SZ * i + j])
            << "Match failed for [" << i << "][" << j << "]" << std::endl;
  }
};

TEST_F(SerialGolding, TiledMultiply) {
  randomizeInputMatrices();
  tiledMultiply<SZ>(mul1, mul2, res);
  validate();
}

TEST_F(SerialGolding, ParallelMultiply) {
  randomizeInputMatrices();
  parallelMultiply<SZ>(mul1, mul2, res);
  validate();
}
