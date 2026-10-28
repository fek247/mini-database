#include <gtest/gtest.h>
#include "file/block_id.h"

namespace MiniDB {
    class BlockIdTest : public testing::Test {
        protected:
            int _block_number = 10;
            std::string _file_name = "test.txt";
            BlockId block = BlockId(_file_name, _block_number);
    };

    TEST_F(BlockIdTest, GetFileName) {
        EXPECT_EQ(block.fileName(), _file_name);
    }

    TEST_F(BlockIdTest, GetBlockNumber) {
        EXPECT_EQ(block.blockNumber(), _block_number);
    }

    TEST_F(BlockIdTest, Equal) {
        BlockId sameBlock = BlockId("test.txt", _block_number);
        EXPECT_TRUE(block.equals(sameBlock));

        BlockId differenceFileName = BlockId("another.txt", _block_number);
        EXPECT_FALSE(block.equals(differenceFileName));

        BlockId differenceBlockNumber = BlockId("test.txt", _block_number + 1);
        EXPECT_FALSE(block.equals(differenceBlockNumber));
    }
}