set(TARGET tests)

enable_testing()

add_executable(${TARGET}
  parser_tests.cpp
)
target_link_libraries(${TARGET} PRIVATE gtest gtest_main Wavefront)

add_test(
  NAME ${TARGET}
  COMMAND $<TARGET_FILE:${TARGET}>
)

include(GoogleTest)
gtest_discover_tests(${TARGET})
