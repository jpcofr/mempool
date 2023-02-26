#include <SubPoolDescriptor.hpp>

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
