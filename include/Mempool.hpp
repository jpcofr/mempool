#include <cstdlib>
#include <cstdint>
#include <vector>
#include <algorithm>

struct ChunkDescriptor {
  void* address;
  bool is_assigned;

  ChunkDescriptor(void* ptr, bool is_assigned = false) :
    address(ptr), is_assigned(is_assigned) {}
};

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

typedef std::vector<SubPoolDescriptor> MempoolConfig;

class Mempool {
 public:
  // Pool size: 5120. Chunk size: 1024. Chunk amount: 5
  static constexpr std::size_t max_pool_memory_size = 5120 * 1024;

  Mempool(MempoolConfig config, std::size_t alignment);
  ~Mempool();

  void* aligned_alloc(std::size_t size);
  void* free(void* p);

  private:
  MempoolConfig _config;
};
