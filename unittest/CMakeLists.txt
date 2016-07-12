include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/gtest/include")

add_executable(UnitTest
  "${CMAKE_CURRENT_SOURCE_DIR}/tHardcode.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tSerialGolding.cpp")

set_target_properties(UnitTest PROPERTIES
  OUTPUT_NAME "unittest")

target_link_libraries(UnitTest
  gtest_main
  Algo)
