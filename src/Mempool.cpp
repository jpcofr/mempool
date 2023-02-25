#include "Mempool.hpp"

#include <iostream>

Mempool::Mempool(MempoolConfig config, std::size_t alignment) {
  // config validation
  if (config.empty())
    throw std::invalid_argument(
        "config should have at least one subpool");

  for (auto& sp_descriptor : config) {
    if (sp_descriptor.chunk_amount <= 0) {
      throw std::invalid_argument("There must be at least one chunk per subpool");
    } else if(sp_descriptor.chunk_size == 0) {
      throw std::invalid_argument("Chunk must have a size greater than zero");
    }
  }
}

Mempool::~Mempool() {
  std::cout << "Mempool resorces were released" << std::endl;
}

void* Mempool::alloc(std::size_t size) {
  // TODO Implement
  return nullptr;
}

void* Mempool::free(void* p) {
  // TODO Implement
  return nullptr;
}
