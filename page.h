#pragma once

#include <string>
#include <vector>

namespace MiniDB {
    class Page {
        public:
            static constexpr int BYTES_PER_CHAR = 1;

            Page(int block_size) : _byte_buffers(block_size) {};
            void setInt(int offset, int val);
            void setBytes(int offset, const std::vector<std::byte>& val);
            void setString(int offset, const std::string& str);
            int getInt(int offset);
            std::string getString(int offset);
            std::vector<std::byte> getBytes(int offset);
            int maxLength(int strlen);
        private:
            std::vector<std::byte> _byte_buffers;
    };
}