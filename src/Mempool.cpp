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

  // Allocate physical memory
  try {
    _base_ptr = std::malloc(max_pool_memory_size);
    if (_base_ptr == nullptr) {
      throw std::bad_alloc();
    }
  } catch (std::bad_alloc& e) {
    std::cerr << "Allocation failed: " << e.what() << std::endl;
    // Perform error handling or cleanup as required.
  }

  // Initialize subpools
  auto subpool_base_ptr = reinterpret_cast<std::uintptr_t>(_base_ptr);
  uint16_t initialized_subpools = 0;
  for (auto& subpool : _config) {
    // Set the base address and initialize chunks
    subpool._base_ptr = reinterpret_cast<void*>(subpool_base_ptr);
    subpool.initialize_chunks();
    initialized_subpools++;
    // Update the initial address of the subpool
    subpool_base_ptr += initialized_subpools * subpool.subpool_size();
  }
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

void* Mempool::free(void* chunk_ptr) {
  // Releases the Mempool memory segment back to the OS
  if (_base_ptr != nullptr) {
    std::free(_base_ptr);
  } else {
    // Handle the case where _memory_segment is null
  }
  return nullptr;
}
