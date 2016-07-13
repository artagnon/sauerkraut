/// Uses serialMultiply as the gold to verify operations of algorithms that need
/// to be stressed on large matrices (which can't be hardcoded).

#include "sauerkraut/Algo.hpp"
#include "gtest/gtest.h"

using namespace sauerkraut;

class SerialGolding : public ::testing::Test {
protected:
  constexpr static int SZ = 512;
  int mul1[SZ][SZ] = {0}, mul2[SZ][SZ] = {0}, res[SZ][SZ] = {0},
      gold[SZ][SZ] = {0};
  void randomizeInputMatrices() {
    std::srand(std::time(0));
    for (int i = 0; i < SZ; ++i)
      for (int j = 0; j < SZ; ++j) {
        mul1[i][j] = std::rand() % 100;
        mul2[i][j] = std::rand() % 100;
      }
  }
  void validate() {
    serialMultiply(mul1, mul2, gold);
    for (int i = 0; i < SZ; ++i)
      for (int j = 0; j < SZ; ++j)
        ASSERT_EQ(res[i][j], gold[i][j]);
  }
};

TEST_F(SerialGolding, TiledMultiply) {
  randomizeInputMatrices();
  tiledMultiply(mul1, mul2, res);
  validate();
}

TEST_F(SerialGolding, DISABLED_ParallelMultiply) {
  randomizeInputMatrices();
  parallelMultiply(mul1, mul2, res);
  validate();
}
