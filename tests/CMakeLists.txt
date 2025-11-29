enable_testing()
include(GoogleTest)

file(GLOB_RECURSE SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

set(ALL_TEST_TARGETS "")
foreach(TEST_SOURCE ${SOURCE_LIST})
  cmake_path(GET TEST_SOURCE STEM test_name)
  if(${test_name} MATCHES "_tests$")
    list(APPEND ALL_TEST_TARGETS ${test_name})
    message("Generate test ${test_name}")

    add_executable(${test_name} ${TEST_SOURCE} ${HEADER_LIST})
    target_link_libraries(${test_name} PRIVATE gtest gtest_main Wavefront)

    # add_test(
    #   NAME ${test_name}
    #   COMMAND $<TARGET_FILE:${test_name}>
    # )

    gtest_discover_tests(${test_name})
  endif()
endforeach()

