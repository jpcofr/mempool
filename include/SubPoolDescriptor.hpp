#include <vector>

#include "ChunkDescriptor.hpp"

struct SubPoolDescriptor {
  int32_t chunk_amount;
  std::size_t chunk_size;
  std::vector<ChunkDescriptor> chunk_descriptors;

  void* base_ptr;

  SubPoolDescriptor(int32_t amount, std::size_t size, void* base_ptr = nullptr)
      : chunk_amount(amount),
        chunk_size(size),
        base_ptr(base_ptr),
        chunk_descriptors(amount) {}

  void initialize_chunks() {
    auto chunk_base_ptr = reinterpret_cast<std::uintptr_t>(base_ptr);
    uint16_t initialized_chunks = 0;
    for (auto& chunk : chunk_descriptors) {
      // Set the base address
      chunk.base_ptr = reinterpret_cast<void*>(chunk_base_ptr);

      chunk.initialize_chunk(reinterpret_cast<void*>(chunk_base_ptr));
      // Update the initial address of the subpool
      chunk_base_ptr += (initialized_chunks * chunk_size);
    }
  }

  size_t subpool_size() const { return chunk_amount * chunk_size; }
};
