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

/**
 * [ ] The mempool pre-allocates a continuous memory range that it will manage throughout its lifecycle.
 *    [ ] Add OS system calls to allocate memory in Mempool constructor
 *    [ ] Add OS system calls to free memory in Mempool constructor
 *    [ ] Identify corner cases
 *    [ ] Add test for corner cases
 * [ ] It is constructed from multiple, user-defined sub-pools with fixed memory chunk sizes.
 * [ ] The request to allocate memory within a mempool should result in an allocation of the
 *         chunk within the sub-pool that has the smallest possible chunk that will fit the
 *         requested allocation size.
 *    [ ] List corner cases to test
 *    [ ] Add test for corner cases
 *    [ ] Replace mocks in tests with gtest mocks.
 ** Requirements
 * [ ] Chunk sizes should be multiples of the provided alignment.
  *   [ ] Identify corner cases
 *    [ ] Add test for corner cases
 * [ ] The mempool has limited memory assigned when created should have an internal limit of the maximum amount of memory that it can allocate in total during creation.
*  [x] Allocation of memory chunks should be protected against attempts to allocate a size bigger than the biggest chunk.
 *    [ ] Identify more corner cases
 *    [ ] Replace mocks in tests with gtest mocks.
*  [ ] Allocation of memory chunks should be protected against attempts to allocate more chunks that the given sub-pool can hold.
 *    [ ] Identify corner cases
 *    [ ] Test corner cases
 *    [ ] Replace mocks in tests with gtest mocks.
 * [ ] The creation of a mempool with sub-pool configuration where the sum of all chunks would be bigger than the predefined limit is disallowed.
 * [ ] Allocations & deallocations of memory chunks within the mempool should be thread safe.
 *    [ ] Refactor data structures
 *    [ ] Allow mutual exclusion through locks or other synchronization primitives
 * [ ] The mempool should operate only on the pre-allocated memory that was allocated during the mempool creation, therefore an attempt to allocate a chunk within the sub-pool that is fully allocated shouldn't cause the mempool to resize, but should be treated as an erroneous situation.
 *    [ ] Test corner case: "every chunk in the chosen subpool is already allocated"
 * [ ] Deallocation of memory chunks should be protected against attempts to deallocate a pointer that wasn't allocated within the mempool.
 *    [ ] free(void* p) shall throw an exception when the chunk freed is locked by mempool clients.
 *    [ ] mempool shall throw an exception if a chunk cannot be freed because it is being read at least one chunk owner.
 * [ ] Allocation of the main memory range should be aligned to the provided alignment.
 *    [ ] List corner cases to test
 *    [ ] Add test for corner cases
 * [ ] Upon destruction of the mempool, the mempool should print the statistics of usage:
 *      [ ] peak usage of the chunks within each sub-pool,
 *          [ ] Add access statistics by chunk
 *      [ ] amount of unfreed chunks within each sub-pool.
 *         [ ] Implement an option to enable full thread safety to allow destructing chunks that are still in use.
 * [ ] Upon destruction of the mempool, the mempool should deallocate its underlying memory range.
 *    [ ] Test corner case: "Chunks in use disallow destruction if are used by clients (only "when writing" and later "while being read and written" )"
 *    [ ] List other corner cases to test
 *    [ ] Add test for corner cases
 * [ ] Time complexity of allocation & deallocation should be linear or better
 *    [ ] Replace vectors<> for a more efficient data structure (a tree? a hash table?).
 *      [ ] Decide if boost/GLib/std or other library has datastructures with sublinear time complexity applicable for this problem.
 *      [ ] Update CMakeLists.txt if necessary.
*/

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
