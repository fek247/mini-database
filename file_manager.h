#pragma once

#include <string>

namespace MiniDB {
    class FileManager {
        public:
            FileManager(const std::string& db_name, int block_size) : _db_name(db_name), _block_size(block_size) {};
            bool isNew();
            void read(const Block& block, Page& page);
            void write(Block& block, const Page& page);
            BlockId append(const std::string& file_name);
            int length(const std::string& file_name);
            int blockSize() { return _block_size; };
        private:
            std::string _db_name;
            int _block_size;
    };
}