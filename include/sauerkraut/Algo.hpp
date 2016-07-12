#pragma once

#include <algorithm>

namespace sauerkraut {
/// Return line cache size, using __APPLE__ specific APIs.
size_t cache_line_size();

/// Multiply serially, but transpose the second matrix beforehand, so that
/// accesses are lined up in the same direction.
template <size_t N>
void transposeMultiply(int mul1[N][N], int mul2[N][N], int res[N][N]) {
  int tmp[N][N];
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      tmp[i][j] = mul2[j][i];
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      for (int k = 0; k < N; ++k)
        res[i][j] += mul1[i][k] * tmp[j][k];
}

/// The most basic matrix multiplication routine. Used as the gold to check
/// against, in tests.
template <size_t N>
void serialMultiply(int mul1[N][N], int mul2[N][N], int res[N][N]) {
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      for (int k = 0; k < N; ++k)
        res[i][j] += mul1[i][k] * mul2[k][j];
}

/// Assumes that you have divided the big matrix up into tiles, and does the
/// multiplication for the little tiles. Takes strides of cache_line_size, with
/// the assumption that starti, startj, and startk will be incremented by that
/// amount in subsequent calls.
template <size_t N>
inline void tiledMultiply(int mul1[N][N], int mul2[N][N], int res[N][N],
                          int starti, int startj, int startk) {
  auto incr = cache_line_size();
  for (int x = starti; x < std::min(starti + incr, N); x++)
    for (int y = startj; y < std::min(startj + incr, N); y++)
      for (int z = startk; z < std::min(startk + incr, N); z++)
        res[x][y] += mul1[x][z] * mul2[z][y];
}

/// The serial tiled matrix multiple algorithm. Divides the matrix into
/// cache_line_size strides, and dispatches to the overloaded function.
template <size_t N>
void tiledMultiply(int mul1[N][N], int mul2[N][N], int res[N][N]) {
  auto incr = cache_line_size();
  for (int i = 0; i < N; i += incr)
    for (int j = 0; j < N; j += incr)
      for (int k = 0; k < N; k += incr)
        tiledMultiply(mul1, mul2, res, i, j, k);
}
}
