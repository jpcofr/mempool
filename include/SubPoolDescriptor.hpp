#include <vector>
#include "ChunkDescriptor.hpp"

struct SubPoolDescriptor {
  int32_t chunk_amount;
  std::size_t chunk_size;
  std::vector<ChunkDescriptor>
      chunk_descriptors;

  SubPoolDescriptor(int32_t amount, std::size_t size)
      : chunk_amount(amount), chunk_size(size) {
    for (int32_t i = 0; i < amount; ++i) {
      chunk_descriptors.emplace_back(ChunkDescriptor(nullptr));
    }
  }
};
