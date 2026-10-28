#include <gtest/gtest.h>
#include <limits.h>
#include "file/page.h"

namespace MiniDB {
    class FileTest : public testing::Test {
        protected:
            int _block_size = 400;
            Page page = Page(_block_size);
};

    TEST_F(FileTest, GetAndSetInt) {
        page.setInt(100, 200);
        page.setInt(104, 1234);
        EXPECT_EQ(page.getInt(100), 200);
        EXPECT_EQ(page.getInt(104), 1234);
    }

    TEST_F(FileTest, GetAndSetString) {
        page.setString(100, "Hello World!");
        EXPECT_EQ(page.getString(100), "Hello World!");
    }

    TEST_F(FileTest, GetAndSetByte) {
        std::vector<std::byte> bytes = { std::byte{0x01}, std::byte{0x02}, std::byte{0xFF} };
        page.setBytes(100, bytes);
        EXPECT_EQ(page.getBytes(100), bytes);
    }

    TEST_F(FileTest, Clear) {
        page.setInt(100, 1234);
        page.setString(104, "Hi everyone!");
        page.clear();
        EXPECT_EQ(page.getInt(100), 0);
        EXPECT_EQ(page.getInt(104), 0);
        EXPECT_EQ(page.getString(104), "");
    }

    TEST_F(FileTest, MaxLength) {
        std::string str = "Hello";
        EXPECT_EQ(page.maxLength(str.size()), 9);

        page.setString(0, str);
        int next = page.maxLength(str.size());
        page.setInt(next, 420);

        EXPECT_EQ(page.getInt(next), 420);
        EXPECT_EQ(page.getString(0), str);
    }

    TEST_F(FileTest, IntAtBoundary) {
        page.setInt(0, -1);
        page.setInt(_block_size - sizeof(int), INT_MAX);

        EXPECT_EQ(page.getInt(0), -1);
        EXPECT_EQ(page.getInt(_block_size - sizeof(int)), INT_MAX);
    }

    TEST_F(FileTest, StringAtBoundary) {
        std::string begin = "Hello!";
        page.setString(0, begin);

        std::string end = "end of year!";
        int max_length = page.maxLength(end.size());
        page.setString(_block_size - max_length, end);
        
        EXPECT_EQ(page.getString(0), begin);
        EXPECT_EQ(page.getString(_block_size - max_length), end);
    }

    TEST_F(FileTest, EmptyString) {
        page.setString(0, "");

        EXPECT_EQ(page.getInt(0), 0);
        EXPECT_EQ(page.getString(0), "");
    }
}
