#pragma once

#include <algorithm>
#include <atomic>
#include <cmath>
#include <thread>
#include <tuple>
#include <vector>

namespace sauerkraut {
/// Return line cache size, using __APPLE__ specific APIs.
size_t cache_line_size();

/// The most basic matrix multiplication routine. Used as the gold to check
/// against, in tests.
template <size_t N>
void serialMultiply(std::vector<int> &mul1, std::vector<int> &mul2,
                    std::vector<int> &res) {
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      for (int k = 0; k < N; ++k)
        res[N * i + j] += mul1[N * i + k] * mul2[N * k + j];
}

/// Multiply serially, but transpose the second matrix beforehand, so that
/// accesses are lined up in the same direction.
template <size_t N>
void transposeMultiply(std::vector<int> &mul1, std::vector<int> &mul2,
                       std::vector<int> &res) {
  std::vector<int> tmp = {0};
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      tmp[N * i + j] = mul2[N * j + i];
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      for (int k = 0; k < N; ++k)
        res[N * i + j] += mul1[N * i + k] * tmp[N * j + k];
}

/// Assumes that you have divided the big matrix up into tiles, and does the
/// multiplication for the little tiles. Takes strides of cache_line_size, with
/// the assumption that starti, startj, and startk will be incremented by that
/// amount in subsequent calls.
template <size_t N>
inline void tiledMultiply(std::vector<int> &mul1, std::vector<int> &mul2,
                          std::vector<int> &res, int starti, int startj,
                          int startk) {
  auto incr = cache_line_size();
  for (int x = starti; x < std::min(starti + incr, N); x++)
    for (int y = startj; y < std::min(startj + incr, N); y++)
      for (int z = startk; z < std::min(startk + incr, N); z++)
        res[N * x + y] += mul1[N * x + z] * mul2[N * z + y];
}

/// The serial tiled matrix multiple algorithm. Divides the matrix into
/// cache_line_size strides, and dispatches to the overloaded function.
template <size_t N>
void tiledMultiply(std::vector<int> &mul1, std::vector<int> &mul2,
                   std::vector<int> &res) {
  auto incr = cache_line_size();
  for (int i = 0; i < N; i += incr)
    for (int j = 0; j < N; j += incr)
      for (int k = 0; k < N; k += incr)
        tiledMultiply<N>(mul1, mul2, res, i, j, k);
}

typedef std::vector<std::tuple<int, int, int>> container;
typedef container::iterator iter;

/// Uses C++11 concurrency features to speed up tiled matrix multiply.
template <size_t N>
void parallelMultiply(std::vector<int> &mul1, std::vector<int> &mul2,
                      std::vector<int> &res) {
  unsigned Nthreads = std::thread::hardware_concurrency();
  container tuples;
  auto incr = cache_line_size();
  for (int i = 0; i < N; i += incr)
    for (int j = 0; j < N; j += incr)
      for (int k = 0; k < N; k += incr)
        tuples.push_back(std::make_tuple(i, j, k));

  // Generally, 8 threads. Each std::thread represents a thread that must be
  // assigned and joined.
  std::vector<std::thread> threads(Nthreads);

  // Every thread gets equal work, take care of leftovers.
  auto partitionSz = tuples.size() % Nthreads ? tuples.size() / (Nthreads - 1)
                                              : tuples.size() / Nthreads;
  // Don't accept zero partition size
  partitionSz = !partitionSz ? 1 : partitionSz;

  auto worker = [&mul1, &mul2, &res](iter begin, iter end) {
    for (auto it = begin; it != end; ++it) {
      auto first = std::get<0>(*it);
      auto second = std::get<1>(*it);
      auto third = std::get<2>(*it);
      tiledMultiply<N>(mul1, mul2, res, first, second, third);
    }
  };

  // We assign each thread using an iterator.
  auto currentThread = threads.begin();

  // We use iter_begin to indicate where in the `tuples` vector the current
  // thread should start from. partitionSz indicates the number of tuples to
  // stuff into the current thread.
  iter iter_begin = tuples.begin();
  iter iter_end = tuples.end();
  for (; iter_begin < iter_end; iter_begin += partitionSz) {
    *currentThread = std::thread(worker, iter_begin, iter_begin + partitionSz);
    ++currentThread;
  }

  // The final thread, possibly leftover from division. `iter_begin` is since
  // loop broke.
  if (iter_begin != iter_end) {
    *currentThread = std::thread(worker, iter_begin, iter_end);
    ++currentThread;
  }

  for (auto &&thread = threads.begin(); thread != currentThread; ++thread)
    thread->join();
}
}
