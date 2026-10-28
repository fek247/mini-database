#include <gtest/gtest.h>
#include "log/log_manager.h"
#include "log/log_iterator.h"
#include "file/file_manager.h"

namespace MiniDB {
    class LogManagerTest : public testing::Test {
        protected:
            std::string _log_file_name = "log";
            std::string _db_name = "db_test";
            int _block_size = 400;
            FileManager _file_manager = FileManager(_db_name, _block_size);
            LogManager _log_manager = LogManager(_file_manager, _log_file_name);
    };

    TEST_F(LogManagerTest, Append) {
        std::string hello = "Hello world!";
        std::vector<std::byte> bytes(hello.size());
        std::memcpy(bytes.data(), hello.data(), hello.size());
        int last_lsn = _log_manager.append(bytes);

        LogIterator log_iter = _log_manager.iterator();
        EXPECT_TRUE(log_iter.hasNext());
        std::optional<std::vector<std::byte>> first_record = log_iter.next();
        ASSERT_TRUE(first_record.has_value());
        EXPECT_EQ(first_record.value(), bytes);
    }
}
