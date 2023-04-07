set(mempool_test_src main.cpp tests.cpp)
set(mempool_bench_src benchmarks.cpp)

add_executable(${PROJECT_NAME}_test ${mempool_test_src})
target_link_libraries(${PROJECT_NAME}_test PRIVATE gtest gtest_main mempool_lib)

add_executable(${PROJECT_NAME}_bench ${mempool_bench_src})
target_link_libraries(${PROJECT_NAME}_bench PRIVATE benchmark::benchmark mempool_lib)
