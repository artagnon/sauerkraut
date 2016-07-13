include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(Timing
  "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp")

set_target_properties(Timing PROPERTIES
  OUTPUT_NAME "timing")

target_link_libraries(Timing
  Algo)
