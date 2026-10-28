#include <gtest/gtest.h>
#include "file/file_manager.h"

namespace MiniDB {
    class FileManagerTest : public testing::Test {
        protected:
            std::string _db_name = "db_test";
            std::string _file_name = "test";
            int _block_size = 400;
            FileManager _file_manager = FileManager(_db_name, _block_size);
            Page _write_page = Page(_block_size);
            Page _read_page = Page(_block_size);
    };

    TEST_F(FileManagerTest, WriteAndRead) {
        std::string hello_world = "Hello world!";
        std::string to_be_continue = "To be continue";
        _write_page.setInt(0, 1234);
        _write_page.setString(4, hello_world);
        _write_page.setString(4 + _write_page.maxLength(hello_world.size()), to_be_continue);

        int block_number = 0;
        BlockId write_block = BlockId(_file_name, block_number);
        _file_manager.write(write_block, _write_page);

        _file_manager.read(write_block, _read_page);

        EXPECT_EQ(_read_page.getInt(0), 1234);
        EXPECT_EQ(_read_page.getString(4), hello_world);
        EXPECT_EQ(_read_page.getString(4 + _read_page.maxLength(hello_world.size())), to_be_continue);

        _write_page.clear();
        _write_page.setInt(0, 12);
        _write_page.setInt(4, 4321);
        _write_page.setString(8, "test");
        BlockId first_write_block = BlockId(_file_name, 0);
        _file_manager.write(first_write_block, _write_page);

        _read_page.clear();
        _file_manager.read(first_write_block, _read_page);

        EXPECT_EQ(_read_page.getInt(0), 12);
        EXPECT_EQ(_read_page.getInt(4), 4321);
        EXPECT_EQ(_read_page.getString(8), "test");
    }

    TEST_F(FileManagerTest, Append) {
        int old_length = _file_manager.length(_file_name);
        BlockId append_block = _file_manager.append(_file_name);

        EXPECT_EQ(append_block.fileName(), _file_name);
        EXPECT_GE(append_block.blockNumber(), 0);
        EXPECT_EQ(append_block.blockNumber(), old_length);
    }

    TEST_F(FileManagerTest, Length) {
        std::string new_file_name = "new_file_test";
        int length = _file_manager.length(new_file_name);

        _file_manager.append(new_file_name);

        EXPECT_EQ(_file_manager.length(new_file_name), length + 1);
    }

    TEST_F(FileManagerTest, BlockSize) {
        EXPECT_EQ(_file_manager.blockSize(), _block_size);
    }
}