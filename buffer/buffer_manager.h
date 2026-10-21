#pragma once

#include "buffer.h"
#include "file_manager.h"
#include <vector>
#include <math.h>
#include <stdexcept>

namespace MiniDB {
    class BufferManager {
        public:
            BufferManager(FileManager& file_mgr, int size) : _file_mgr(file_mgr) {
                _buffer_pool.reserve(size);
                for (int i = 0; i < size; i++) {
                    _buffer_pool.push_back(Buffer(_file_mgr, Page(file_mgr.blockSize())));
                }
            };
            ~BufferManager();
            Buffer& pin(const BlockId& block);
            void unpin(Buffer& buffer);
            void flushAll();
        private:
            FileManager& _file_mgr;
            std::vector<Buffer> _buffer_pool;
            int _curr_time = 0;
    };
}