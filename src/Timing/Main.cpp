/// Does timing measurements of various matrix multiplication algorithms.

#include <atomic>
#include <ctime> // I don't like std::chrono
#include <iostream>
#include <vector>

#include "sauerkraut/Algo.hpp"

using namespace sauerkraut;

class Helper {
public:
  static constexpr size_t SZ = 2048;
  std::vector<int> mul1, mul2, res;

  Helper() : mul1(SZ * SZ), mul2(SZ * SZ), res(SZ * SZ) {}

  /// Random-initializes mul1 and mul2.
  void randomizeInputMatrices() {
    std::srand(std::time(0));
    for (int i = 0; i < SZ; ++i)
      for (int j = 0; j < SZ; ++j) {
        mul1[SZ * i + j] = std::rand() % 100;
        mul2[SZ * i + j] = std::rand() % 100;
      }
  }

  /// Does tiledMultiply without regard for the result.
  void tiledMultiply() { ::tiledMultiply<SZ>(mul1, mul2, res); }

  /// Does parallelMultiply without regard for the result.
  void parallelMultiply() { ::parallelMultiply<SZ>(mul1, mul2, res); }
};

int main() {
  Helper helper;
  helper.randomizeInputMatrices();

  std::clock_t begin = std::clock();
  helper.tiledMultiply();
  std::clock_t end = std::clock();
  double tiledTime = double(end - begin) / CLOCKS_PER_SEC;

  begin = std::clock();
  helper.parallelMultiply();
  end = std::clock();
  double parallelTime = double(end - begin) / CLOCKS_PER_SEC;

  std::cout << "Tiled multiply took: " << tiledTime << " seconds" << std::endl
            << "Parallel multiply took: " << parallelTime << " seconds"
            << std::endl;
  return 0;
}
