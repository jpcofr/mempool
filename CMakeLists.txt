cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
project(challenge_project)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
# Download and configure googletest
FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
)
FetchContent_GetProperties(gtest)
FetchContent_Populate(gtest)
add_subdirectory(${gtest_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/gtest-build)
target_include_directories(gtest INTERFACE $<BUILD_INTERFACE:${gtest_SOURCE_DIR}/googletest/include>)

# Download and configure google benchmark
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1
)
FetchContent_GetProperties(benchmark)
FetchContent_Populate(benchmark)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(${benchmark_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/benchmark-build)
target_include_directories(benchmark INTERFACE $<BUILD_INTERFACE:${benchmark_SOURCE_DIR}/include>)

# Finish configuring every other detail
include_directories(${CMAKE_SOURCE_DIR}/include)

MESSAGE(STATUS "INFO ----> benchmark library: " ${benchmark_BINARY_DIR})
MESSAGE(STATUS "INFO ----> gtest library:     " ${gtest_BINARY_DIR})
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "INFO ----> include directory: '${dir}'")
endforeach()

add_subdirectory(src)
add_subdirectory(test)
