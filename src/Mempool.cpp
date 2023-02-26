#include "Mempool.hpp"

#include <iostream>

Mempool::Mempool(MempoolConfig config, std::size_t alignment) {
  // config validation
  if (config.empty())
    throw std::invalid_argument("config should have at least one subpool");

  for (auto& sp_descriptor : config) {
    if (sp_descriptor.chunk_amount <= 0) {
      throw std::invalid_argument(
          "There must be at least one chunk per subpool");
    } else if (sp_descriptor.chunk_size == 0) {
      throw std::invalid_argument("Chunk must have a size greater than zero");
    }
  }

  _config = config;
}

Mempool::~Mempool() {}

void* Mempool::aligned_alloc(std::size_t chunk_size) {
  // Protect memory chunk allocation from oversized chunks
  auto largestChunkDescriptor = std::max_element(
      _config.begin(), _config.end(),
      [](const SubPoolDescriptor& a, const SubPoolDescriptor& b) {
        return a.chunk_size < b.chunk_size;
      });

  if (chunk_size > largestChunkDescriptor->chunk_size)
    throw std::invalid_argument(
        "The pool cannot provide a chunk of this size as it is larger than the "
        "preconfigured sizes");

  // Find the subpool with the smallest chunk that fits the requested chunk size
  auto subpool_it = std::min_element(
      std::begin(_config), std::end(_config),
      [chunk_size](const SubPoolDescriptor& a, const SubPoolDescriptor& b) {
        return a.chunk_size < b.chunk_size && a.chunk_size < chunk_size;
      });

  if (subpool_it == std::end(_config))
    throw std::out_of_range(
        "The pool cannot provide a chunk of this size as it is "
        "larger than the preconfigured sizes");

  SubPoolDescriptor subpool = *subpool_it;
}

void* Mempool::free(void* p) {
  // TODO Implement
  return nullptr;
}
