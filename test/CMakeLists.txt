set(mempool_test_src main.cpp tests.cpp)
set(mempool_bench_src benchmarks.cpp)

add_executable(test_bundle ${mempool_test_src})
target_link_libraries(test_bundle PRIVATE gtest gtest_main mempool_lib)

add_executable(bench_bundle ${mempool_bench_src})
target_link_libraries(bench_bundle PRIVATE benchmark::benchmark mempool_lib)
