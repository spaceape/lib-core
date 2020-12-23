set(NAME test)
cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(
  -fPIC -DPIC
)

set(common_srcs
)

set(common_libs
  core test
)

add_executable(test-ios test-ios.cpp ${common_srcs})
target_link_libraries(test-ios ${common_libs})
