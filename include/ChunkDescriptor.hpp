struct ChunkDescriptor {
  void* address;
  bool is_assigned;

  ChunkDescriptor(void* ptr, bool is_assigned = false) :
    address(ptr), is_assigned(is_assigned) {}
};
