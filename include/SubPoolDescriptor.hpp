#include <vector>

#include "ChunkDescriptor.hpp"

struct SubPoolDescriptor {
  int32_t chunk_amount;
  std::size_t chunk_size;
  std::vector<ChunkDescriptor> chunk_descriptors;

  void* _base_ptr;

  SubPoolDescriptor(int32_t amount, std::size_t size, void* segment = nullptr)
      : chunk_amount(amount), chunk_size(size), _base_ptr(segment) {
    for (int32_t i = 0; i < amount; ++i) {
      chunk_descriptors.emplace_back(ChunkDescriptor(nullptr));
    }
  }

  void initialize_chunks() {
    auto chunk_base_ptr = reinterpret_cast<std::uintptr_t>(_base_ptr);
    uint16_t initialized_chunks = 0;
    for (auto& chunk : chunk_descriptors) {
      // Set the base address
      chunk._base_ptr = reinterpret_cast<void*>(chunk_base_ptr);

      chunk.initialize_chunk(reinterpret_cast<void*>(chunk_base_ptr));
      // Update the initial address of the subpool
      chunk_base_ptr += (initialized_chunks * chunk_size);
    }
  }

  size_t subpool_size() const { return chunk_amount * chunk_size; }
};
