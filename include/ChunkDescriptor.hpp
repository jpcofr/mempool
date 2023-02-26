#include <shared_mutex>

struct ChunkDescriptor {
  void* base_ptr;
  bool is_assigned;
  std::shared_mutex _shared_mutex;

  ChunkDescriptor(void* base_ptr = nullptr, bool is_assigned = false)
      : base_ptr(base_ptr), is_assigned(is_assigned) {}

  // Copy constructor
  ChunkDescriptor(const ChunkDescriptor& other)
      : base_ptr(other.base_ptr), is_assigned(other.is_assigned) {}

  // Copy assignment operator
  ChunkDescriptor& operator=(const ChunkDescriptor& other) {
    this->base_ptr = other.base_ptr;
    this->is_assigned = other.is_assigned;

    return *this;
  }

  // Move constructor
  ChunkDescriptor(ChunkDescriptor&& other) noexcept
      : base_ptr(std::move(other.base_ptr)),
        is_assigned(std::move(other.is_assigned)),
        _shared_mutex() {}

  // Move assignment operator
  ChunkDescriptor& operator=(ChunkDescriptor&& other) noexcept {
    base_ptr = std::move(other.base_ptr);
    is_assigned = std::move(other.is_assigned);

    return *this;
  }

  // Initialize chunk descriptor
  void initialize_chunk(void* base_ptr, bool is_assigned = false) {
    std::unique_lock<std::shared_mutex> lock(_shared_mutex);
    base_ptr = base_ptr;
    this->is_assigned = is_assigned;
  }
};
