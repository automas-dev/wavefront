cmake_minimum_required(VERSION 3.15...3.20)

project(Wavefront
    VERSION 0.2.0
    DESCRIPTION "A Wavefront OBJ and MTL Parser"
    LANGUAGES CXX)

option(WAVEFRONT_BUILD_DOCS "Builds the Wavefront documentation" ON)
option(WAVEFRONT_BUILD_TESTS "Builds the Wavefront tests" ON)

option(WAVEFRONT_TEST_COVERAGE "Generate gcov target for calculating code coverage" ON)
option(WAVEFRONT_TEST_COVERAGE_CAN_FAIL "Tests can fail for coverage" ON)
option(WAVEFRONT_TEST_COVERAGE_DARK "Use dark theme for html output" ON)

# Only if this is the top level project (not included with add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Use -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    if(WAVEFRONT_BUILD_TESTS)
        set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
        if(WAVEFRONT_TEST_COVERAGE AND CMAKE_COMPILER_IS_GNUCXX)
            include(CodeCoverage)
            append_coverage_compiler_flags()
            if(WAVEFRONT_TEST_COVERAGE_CAN_FAIL)
                set(GCOVR_ADDITIONAL_ARGS ${GCOVR_ADDITIONAL_ARGS} --html-high-threshold 100 --html-medium-threshold 90 --fail-under-line 100 --fail-under-branch 100)
            endif()
            if(WAVEFRONT_TEST_COVERAGE_DARK)
                set(GCOVR_ADDITIONAL_ARGS ${GCOVR_ADDITIONAL_ARGS} --html-theme github.dark-green)
            else()
                set(GCOVR_ADDITIONAL_ARGS ${GCOVR_ADDITIONAL_ARGS} --html-theme github.green)
            endif()
        endif()
        # Testing only available for top level projects. It calls enable_testing
        # which must be in the main CMakeLists.
        include(CTest)
    endif()

    if(WAVEFRONT_BUILD_DOCS)
        # Generate documentation using Doxygen
        find_package(Doxygen)

        if(Doxygen_FOUND)
            add_subdirectory(docs)
        else()
            message(STATUS "Doxygen not found, not building docs")
        endif()
    endif()
endif()

# Use for platform independent install dirs, despite being called GNU
include(GNUInstallDirs)

# Create config.h with project version numbers
configure_file(cmake/config.h.in include/config.h)
include_directories(PRIVATE ${CMAKE_BINARY_DIR}/include)

add_subdirectory(external)

add_subdirectory(src)

# Create Targets file
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Generate ConfigVersion file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

# Copy the Config file to the build dir
configure_file(cmake/ProjectConfig.cmake ${PROJECT_NAME}Config.cmake @ONLY)

# Add Config and ConfigVersion files to install
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Add tests
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND WAVEFRONT_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(WAVEFRONT_TEST_COVERAGE AND CMAKE_COMPILER_IS_GNUCXX)
    setup_target_for_coverage_gcovr_html(
        NAME ${PROJECT_NAME}_coverage
        EXECUTABLE ctest
        EXECUTABLE_ARGS --output-on-failure
        DEPENDENCIES ${PROJECT_NAME}
        BASE_DIRECTORY .
        EXCLUDE ${PROJECT_SOURCE_DIR}/tests ${PROJECT_BINARY_DIR}/*
    )
endif()
