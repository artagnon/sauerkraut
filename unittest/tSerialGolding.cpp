/// Uses serialMultiply as the gold to verify operations of algorithms that need
/// to be stressed on large matrices (which can't be hardcoded).

#include "sauerkraut/Algo.hpp"
#include "gtest/gtest.h"

using namespace sauerkraut;

TEST(SerialGolding, TiledMultiply) {}
