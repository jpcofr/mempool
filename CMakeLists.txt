cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
project(challenge_project)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


include(ExternalProject)
set(EXTERNAL_DEPENDENCIES ${CMAKE_CURRENT_BINARY_DIR}/external)
## Download and configure googletest
ExternalProject_Add(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
    PREFIX ${EXTERNAL_DEPENDENCIES}/gtest
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

# Get gtest ready for linking and including
ExternalProject_Get_Property(gtest SOURCE_DIR BINARY_DIR)
set(GTEST_LIB ${BINARY_DIR}/lib/libgtest.a)
include_directories(${SOURCE_DIR}/googletest/include)

## Download and configure google benchmark
ExternalProject_Add(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1
    PREFIX ${EXTERNAL_DEPENDENCIES}/benchmark
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    CMAKE_ARGS -DBENCHMARK_ENABLE_TESTING=OFF
)
# Get benchmark ready for linking and including
ExternalProject_Get_Property(benchmark SOURCE_DIR BINARY_DIR)
set(BENCHMARK_LIB ${BINARY_DIR}/src/libbenchmark.a)
include_directories(${SOURCE_DIR}/include)

## Finish configuring every other detail
include_directories(${CMAKE_SOURCE_DIR}/include)

MESSAGE(STATUS "INFO ----> benchmark library: " ${BENCHMARK_LIB})
MESSAGE(STATUS "INFO ----> gtest library:     " ${GTEST_LIB})
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "INFO ----> include directory: '${dir}'")
endforeach()

add_subdirectory(src)
add_subdirectory(test)
