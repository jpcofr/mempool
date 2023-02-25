#include <iostream>

#include "Mempool.hpp"

int main() {
  MempoolConfig config;
  std::size_t alignment = 16;
  Mempool mempool(config, alignment);

  std::cout << "Called mempool constructor" << std::endl;
  return 0;
}
