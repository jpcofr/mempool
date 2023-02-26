struct ChunkDescriptor {
  void* _base_ptr;
  bool is_assigned;

  ChunkDescriptor(void* ptr, bool is_assigned = false)
      : _base_ptr(ptr), is_assigned(is_assigned) {}
  // Initialize chunk descriptor
  void initialize_chunk(void* ptr, bool is_assigned = false) {
    ptr = ptr;
  }
};
