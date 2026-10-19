#pragma once

#include <string>

namespace MiniDB {
    class BlockId {
        public:
            BlockId(const std::string& file_name, int block_number)
                : _file_name(file_name), _block_number(block_number) {};
            std::string fileName() const { return _file_name; }
            int blockNumber() const { return _block_number; }
            bool equals(const BlockId& block);
            std::string toString();
            int hashCode();
        private:
            std::string _file_name;
            int _block_number;
        };
}