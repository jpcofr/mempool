#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "Mempool.hpp"

class SolutionTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(SolutionTest, configuration_has_entries) {
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

TEST_F(SolutionTest, chunk_amount_and_size_are_consistent) {
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
