set(temp_src main.cpp Mempool.cpp)

add_library(mempool_lib Mempool.cpp)

add_executable(exe_bundle ${temp_src})
target_link_libraries(exe_bundle mempool_lib)
