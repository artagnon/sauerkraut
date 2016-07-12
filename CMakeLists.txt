cmake_minimum_required(VERSION 2.6)
project(Sauerkraut)

set(CMAKE_CXX_FLAGS "-Werror -g -O0 -std=c++14")
set(CMAKE_BUILD_TYPE Debug)
set(UNITTEST_EXE "${PROJECT_BINARY_DIR}/unittest/unittest")

include_directories("${PROJECT_SOURCE_DIR}/include")
add_subdirectory(src/Algo)
add_subdirectory(src/Toplevel)
add_subdirectory(gtest)
add_subdirectory(unittest)

add_custom_target(check "${UNITTEST_EXE}"
  COMMENT "Running unittests ..."
  DEPENDS Toplevel
  DEPENDS UnitTest
  USES_TERMINAL)
