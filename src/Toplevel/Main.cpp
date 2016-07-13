/// Does timing measurements of various matrix multiplication algorithms.

#include <atomic>
#include <ctime> // I don't like std::chrono
#include <iostream>

#include "sauerkraut/Algo.hpp"

using namespace sauerkraut;

static constexpr size_t SZ = 512;

void randomizeInputMatrices(int (&mul1)[SZ][SZ], int (&mul2)[SZ][SZ]) {
  std::srand(std::time(0));
  for (int i = 0; i < SZ; ++i)
    for (int j = 0; j < SZ; ++j) {
      mul1[i][j] = std::rand() % 100;
      mul2[i][j] = std::rand() % 100;
    }
}

int main() {
  int mul1[SZ][SZ] = {0}, mul2[SZ][SZ] = {0}, res[SZ][SZ] = {0};
  randomizeInputMatrices(mul1, mul2);

  std::clock_t begin = std::clock();
  tiledMultiply(mul1, mul2, res);
  std::clock_t end = std::clock();
  double tiledTime = double(end - begin) / CLOCKS_PER_SEC;

  begin = std::clock();
  parallelMultiply(mul1, mul2, res);
  end = std::clock();
  double parallelTime = double(end - begin) / CLOCKS_PER_SEC;

  std::cout << "Tiled multiply took: " << tiledTime << " seconds" << std::endl
            << "Parallel multiply took: " << parallelTime << " seconds"
            << std::endl;
  return 0;
}
