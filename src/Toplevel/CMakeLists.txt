include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(Toplevel
  "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp")

set_target_properties(Toplevel PROPERTIES
  OUTPUT_NAME "sauerkraut")

target_link_libraries(Toplevel
  Algo)
