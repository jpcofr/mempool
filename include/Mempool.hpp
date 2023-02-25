#include <cstdlib>
#include <cstdint>
#include <vector>

struct SubPoolDescriptor {
  int32_t chunk_amount;
  std::size_t chunk_size;
  SubPoolDescriptor(int32_t amount, std::size_t size)
      : chunk_amount(amount), chunk_size(size) {}
};

typedef std::vector<SubPoolDescriptor> MempoolConfig;

/**
 * [ ] The mempool pre-allocates a continuous memory range that it will manage throughout its lifecycle.
 * [ ] It is constructed from multiple, user-defined sub-pools with fixed memory chunk sizes.
 * [ ] The request to allocate a memory within a mempool should result in an allocation of the
 *         chunk within the sub-pool that has the smallest possible chunk that will fit the
 *         requested allocation size.
 *
 ** Requirements
 * [ ] Chunk sizes should be multiples of the provided alignment.
 * [ ] The mempool has limited memory assigned when created should have an internal limit of the maximum amount of memory that it can allocate in total during creation.
 * [ ] Allocation of memory chunks should be protected against attempts to allocate a size bigger than the biggest chunk.
 * [ ] Allocation of memory chunks should be protected against attempts to allocate more chunks that the given sub-pool can hold.
 * [ ] The creation of a mempool with sub-pool configuration where the sum of all chunks would be bigger than the predefined limit is disallowed.
 * [ ] Allocations & deallocations of memory chunks within the mempool should be thread safe.
 * [ ] The mempool should operate only on the pre-allocated memory that was allocated during the mempool creation, therefore an attempt to allocate a chunk within the sub-pool that is fully allocated shouldn't cause the mempool to resize, but should be treated as an erroneous situation.
 * [ ] Deallocation of memory chunks should be protected against attempts to deallocate a pointer that wasn't allocated within the mempool.
 * [ ] Allocation of the main memory range should be aligned to the provided alignment.
 * [ ] Upon destruction of the mempool, the mempool should print the statistics of usage:
 *      - peak usage of the chunks within each sub-pool,
 *      - amount of unfreed chunks within each sub-pool.
 *      - Upon destruction of the mempool, the mempool should deallocate its underlying memory range.
 * [ ] Time complexity of allocation & deallocation should be linear or better
*/

class Mempool {
 public:
  static constexpr std::size_t max_pool_memory_size = 2048 * 1024;

  Mempool(MempoolConfig config, std::size_t alignment);
  ~Mempool();

  void* alloc(std::size_t size);
  void* free(void* p);
};
