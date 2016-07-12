include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(UnitTest
  "${CMAKE_CURRENT_SOURCE_DIR}/tAll.cpp")

set_target_properties(UnitTest PROPERTIES
  OUTPUT_NAME "unittest"
  LINKER_LANGUAGE CXX)

target_link_libraries(UnitTest
  gtest_main
  Src)
