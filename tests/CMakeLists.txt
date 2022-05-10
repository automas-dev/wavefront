
enable_testing()

set(TARGET tests)

add_executable(${TARGET}
  parser_tests.cpp
)
target_link_libraries(${TARGET}
  gtest_main
  Wavefront
)

include(GoogleTest)
gtest_discover_tests(${TARGET})
