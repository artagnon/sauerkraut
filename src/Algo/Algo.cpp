#include <stddef.h>
#include <sys/sysctl.h>

#include "sauerkraut/Algo.hpp"

// __APPLE__'ism
size_t cache_line_size() {
  size_t line_size = 0;
  size_t sizeof_line_size = sizeof(line_size);
  sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, 0, 0);
  return line_size;
}

void clsMultiply(int **mul1, int **mul2, int **res, int N) {
  auto SM = cache_line_size() / sizeof(int);
  int *rres = nullptr, *rmul1 = nullptr, *rmul2 = nullptr;
  int i2 = 0, j2 = 0, k2 = 0;
  for (int i = 0; i < N; i += SM)
    for (int j = 0; j < N; j += SM)
      for (int k = 0; k < N; k += SM)
        for (i2 = 0, rres = &res[i][j], rmul1 = &mul1[i][k]; i2 < SM;
             ++i2, rres += N, rmul1 += N)
          for (k2 = 0, rmul2 = &mul2[k][j]; k2 < SM; ++k2, rmul2 += N)
            for (j2 = 0; j2 < SM; ++j2)
              rres[j2] += rmul1[k2] * rmul2[j2];
}

void transposeMultiply(int **mul1, int **mul2, int **res, int N) {
  int tmp[N][N];
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      tmp[i][j] = mul2[j][i];
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      for (int k = 0; k < N; ++k)
        res[i][j] += mul1[i][k] * tmp[j][k];
}

void serialMultiply(int **mul1, int **mul2, int **res, int N) {
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      for (int k = 0; k < N; ++k)
        res[i][j] += mul1[i][k] + mul2[k][j];
}
