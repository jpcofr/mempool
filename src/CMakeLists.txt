set(mempool_exe_src main.cpp Mempool.cpp)

add_library(mempool_lib Mempool.cpp)

add_executable(${PROJECT_NAME}_exe ${mempool_exe_src})
target_link_libraries(${PROJECT_NAME}_exe mempool_lib)
