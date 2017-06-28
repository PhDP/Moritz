add_library(gtest ./gtest/gtest-all.cc)
include_directories(.)
target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})

set(test_src
  run_all.cc
  n-sphere_spec.cc
)

add_executable(wagner_tests ${test_src})

target_link_libraries(wagner_tests
  gtest
  wagner
  ${CMAKE_THREAD_LIBS_INIT}
  ${MATH_LIBS}
)

add_test(
  NAME tests
  COMMAND wagner_tests
)
