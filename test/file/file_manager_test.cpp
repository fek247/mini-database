#include <gtest/gtest.h>
#include "file/file_manager.h"

namespace MiniDB {
    class FileManagerTest : public testing::Test {
        protected:
            std::string _db_name = "db_test";
            int _block_size = 400;
            FileManager _file_manager = FileManager(_db_name, _block_size);
    };

    TEST_F(FileManagerTest, WriteAndRead) {
        Page writePage = Page(_block_size);
        std::string hello_world = "Hello world!";
        std::string to_be_continue = "To be continue";
        writePage.setInt(0, 1234);
        writePage.setString(4, hello_world);
        writePage.setString(4 + writePage.maxLength(hello_world.size()), to_be_continue);
        
        std::string file_name = "test";
        int block_number = 10;
        BlockId writeBlock = BlockId(file_name, block_number);
        _file_manager.write(writeBlock, writePage);

        Page readPage = Page(_block_size);
        _file_manager.read(writeBlock, readPage);

        EXPECT_EQ(readPage.getInt(0), 1234);
        EXPECT_EQ(readPage.getString(4), hello_world);
        EXPECT_EQ(readPage.getString(4 + readPage.maxLength(hello_world.size())), to_be_continue);
    } 
}