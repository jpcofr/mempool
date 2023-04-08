cmake_minimum_required(VERSION 3.25)
project(mempool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Configuring ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# Configure doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT
    ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile)
    set(src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
    set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the documentation")
endif(DOXYGEN_FOUND)

## Configure testing
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Download and configure googletest
FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
)
FetchContent_GetProperties(gtest)
if(NOT gtest_POPULATED)
  FetchContent_Populate(gtest)
  add_subdirectory(${gtest_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/gtest-build)
endif()

# Download and configure google benchmark
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1
)
FetchContent_GetProperties(benchmark)
if(NOT benchmark_POPULATED)
  FetchContent_Populate(benchmark)
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  add_subdirectory(${benchmark_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/benchmark-build)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)
