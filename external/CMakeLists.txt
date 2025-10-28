include(${CMAKE_CURRENT_LIST_DIR}/glm.cmake)

if(WAVEFRONT_BUILD_TESTS)
    include(${CMAKE_CURRENT_LIST_DIR}/gtest.cmake)
endif()
