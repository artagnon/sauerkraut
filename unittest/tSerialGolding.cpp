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

  /// Just fill mul1 and mul2 with whatever.
  void randomizeInputMatrices() {
    std::srand(std::time(0));
    for (int i = 0; i < SZ; ++i)
      for (int j = 0; j < SZ; ++j) {
        mul1[i][j] = std::rand() % 100;
        mul2[i][j] = std::rand() % 100;
      }
  }

  /// Common version.
  void validate() {
    serialMultiply(mul1, mul2, gold);
    for (int i = 0; i < SZ; ++i)
      for (int j = 0; j < SZ; ++j)
        ASSERT_EQ(res[i][j], gold[i][j]) << "Match failed for [" << i << "]["
                                         << j << "]" << std::endl;
  }

  /// Version for Parallel Matrix Multiply.
  void validate(std::atomic<int> (&res)[SZ][SZ]) {
    serialMultiply(mul1, mul2, gold);
    for (int i = 0; i < SZ; ++i)
      for (int j = 0; j < SZ; ++j)
        ASSERT_EQ(res[i][j], gold[i][j]) << "Match failed for [" << i << "]["
                                         << j << "]" << std::endl;
  }
};

TEST_F(SerialGolding, TiledMultiply) {
  randomizeInputMatrices();
  tiledMultiply(mul1, mul2, res);
  validate();
}

TEST_F(SerialGolding, ParallelMultiply) {
  randomizeInputMatrices();
  std::atomic<int> res[SZ][SZ] = {};
  parallelMultiply(mul1, mul2, res);
  validate(res);
}
