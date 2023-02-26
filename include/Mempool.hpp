#include <SubPoolDescriptor.hpp>

typedef std::vector<SubPoolDescriptor> MempoolConfig;

class Mempool {
 public:
  // Pool size: 5120. Chunk size: 1024. Chunk amount: 5
  static constexpr std::size_t chunk_size = 1024;
  static constexpr std::size_t chunk_amount = 1024;
  static constexpr std::size_t max_pool_memory_size = chunk_size * chunk_amount;
  static constexpr std::size_t total_subpools =
      max_pool_memory_size / (chunk_size * chunk_amount);

  Mempool(MempoolConfig config, std::size_t alignment);
  ~Mempool();

  void* aligned_alloc(std::size_t size);
  void* free(void* chunk_ptr);

 private:
  MempoolConfig config;
  void* _base_ptr;
};
