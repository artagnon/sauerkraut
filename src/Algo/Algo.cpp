#include <cstddef>
#include <sys/sysctl.h>

#include "sauerkraut/Algo.hpp"

namespace sauerkraut {
// __APPLE__'ism
size_t cache_line_size() {
  size_t line_size = 0;
  size_t sizeof_line_size = sizeof(line_size);
  sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, 0, 0);
  return line_size;
}
}
