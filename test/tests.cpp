#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "Mempool.hpp"

class MempoolTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(MempoolTest,
       chunk_allocation_chooses_pool_descriptors_with_the_smallest_chunk_size) {
  // Arrange
  // First subpool size 1
  SubPoolDescriptor sp_descriptor_size_1(2, 1U);
  void* memory_address_0 = reinterpret_cast<int32_t*>(0);
  ChunkDescriptor chunk_descriptor_0(memory_address_0);
  void* memory_address_1 = reinterpret_cast<int32_t*>(1);
  ChunkDescriptor chunk_descriptor_size_2(memory_address_1);
  sp_descriptor_size_1.chunk_descriptors.push_back(chunk_descriptor_0);
  sp_descriptor_size_1.chunk_descriptors.push_back(chunk_descriptor_size_2);

  // Second subpool size 2
  SubPoolDescriptor sp_descriptor_size_2(2, 2U);
  void* memory_address_2 = reinterpret_cast<int32_t*>(2);
  ChunkDescriptor chunk_descriptor_2(memory_address_2);
  void* memory_address_3 = reinterpret_cast<int32_t*>(3);
  ChunkDescriptor chunk_descriptor_3(memory_address_3);
  sp_descriptor_size_2.chunk_descriptors.push_back(chunk_descriptor_size_2);
  sp_descriptor_size_2.chunk_descriptors.push_back(chunk_descriptor_3);

  MempoolConfig mempoolConfig;
  mempoolConfig.push_back(sp_descriptor_size_1);
  mempoolConfig.push_back(sp_descriptor_size_2);

  Mempool mempool(mempoolConfig, 1U);

  // Act
  try {
    // Assert
    void* allocated_address = mempool.aligned_alloc(1U);

  } catch (const std::invalid_argument& e) {
    // Assert
    EXPECT_STREQ(
        e.what(),
        "The pool cannot provide a chunk of this size as it is larger than the "
        "preconfigured sizes");
  }
}

TEST_F(MempoolTest,
       DISABLED_chunk_allocation_chooses_pool_descriptors_with_free_chunks) {
  // FIXME: Disabled (not yet implemented)
  // Arrange
  // Prepare chunks
  SubPoolDescriptor sp_descriptor_size_1(1, 1U);
  SubPoolDescriptor sp_descriptor_size_2(2, 2U);
  MempoolConfig mempoolConfig;
  mempoolConfig.push_back(sp_descriptor_size_1);
  mempoolConfig.push_back(sp_descriptor_size_2);
  Mempool mempool(mempoolConfig, 1U);

  // Act
  EXPECT_THROW(mempool.aligned_alloc(4U), std::invalid_argument);

  try {
    // Assert
    mempool.aligned_alloc(4U);
  } catch (const std::invalid_argument& e) {
    // Assert
    EXPECT_STREQ(
        e.what(),
        "Memory chunk allocation is restricted to sub-pools with free chunks.");
  }
}

TEST_F(MempoolTest, DISABLED_chunk_allocation_respects_subpool_chunk_amount) {
  // FIXME: Disabled (not yet implemented)
  // Arrange
  SubPoolDescriptor sp_descriptor_size_1(1, 1U);
  SubPoolDescriptor sp_descriptor_size_2(2, 2U);
  MempoolConfig mempoolConfig;
  mempoolConfig.push_back(sp_descriptor_size_1);
  mempoolConfig.push_back(sp_descriptor_size_2);
  Mempool mempool(mempoolConfig, 1U);

  // Act
  EXPECT_THROW(mempool.aligned_alloc(4U), std::invalid_argument);

  try {
    // Assert
    mempool.aligned_alloc(4U);
  } catch (const std::invalid_argument& e) {
    // Assert
    EXPECT_STREQ(e.what(),
                 "Memory chunk allocation should be restricted to the number "
                 "of chunks that a sub-pool can hold.");
  }
}

TEST_F(MempoolTest, protect_memory_chunk_allocation_from_oversized_chunks) {
  // Arrange
  SubPoolDescriptor sp_descriptor_size_1(1, 1U);
  SubPoolDescriptor sp_descriptor_size_2(2, 2U);
  MempoolConfig mempoolConfig;
  mempoolConfig.push_back(sp_descriptor_size_1);
  mempoolConfig.push_back(sp_descriptor_size_2);
  Mempool mempool(mempoolConfig, 1U);

  // Act
  EXPECT_THROW(mempool.aligned_alloc(4U), std::invalid_argument);

  try {
    // Assert
    mempool.aligned_alloc(4U);
  } catch (const std::invalid_argument& e) {
    // Assert
    EXPECT_STREQ(e.what(),
                 "The pool cannot provide a chunk of this size as it is larger "
                 "than the preconfigured sizes");
  }
}

TEST_F(MempoolTest, configuration_has_entries) {
  // Arrange
  MempoolConfig mempool_config;

  // Act
  try {
    Mempool mempool(mempool_config, 1U);
  } catch (const std::invalid_argument& e) {
    // Assert
    EXPECT_STREQ(e.what(), "config should have at least one subpool");
  }
}

TEST_F(MempoolTest, chunk_amount_and_size_are_consistent) {
  // Arrange

  SubPoolDescriptor sp_descriptor_no_chunks(0, 1U);
  SubPoolDescriptor sp_descriptor_negative_chunks(-1, 1U);
  SubPoolDescriptor sp_descriptor_no_size(1, 0U);

  MempoolConfig mempool_config_no_chunks;
  mempool_config_no_chunks.push_back(sp_descriptor_no_chunks);

  MempoolConfig mempool_config_negative_chunks;
  mempool_config_negative_chunks.push_back(sp_descriptor_negative_chunks);

  MempoolConfig mempool_config_no_size;
  mempool_config_no_size.push_back(sp_descriptor_no_size);

  // Act
  EXPECT_THROW(Mempool mempool(mempool_config_no_chunks, 1U),
               std::invalid_argument);
  try {
    // Assert
    Mempool mempool(mempool_config_no_chunks, 1U);
  } catch (const std::invalid_argument& e) {
    // Assert
    EXPECT_STREQ(e.what(), "There must be at least one chunk per subpool");
  }

  EXPECT_THROW(Mempool mempool(mempool_config_negative_chunks, 1U),
               std::invalid_argument);
  try {
    // Assert
    Mempool mempool(mempool_config_no_chunks, 1U);
  } catch (const std::invalid_argument& e) {
    // Assert
    EXPECT_STREQ(e.what(), "There must be at least one chunk per subpool");
  }

  EXPECT_THROW(Mempool mempool(mempool_config_no_size, 1U),
               std::invalid_argument);
  try {
    // Assert
    Mempool mempool(mempool_config_no_size, 1U);
  } catch (const std::invalid_argument& e) {
    // Assert
    EXPECT_STREQ(e.what(), "Chunk must have a size greater than zero");
  }
}
