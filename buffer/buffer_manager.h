#pragma once

#include "buffer.h"
#include "file/file_manager.h"
#include <vector>
#include <math.h>
#include <stdexcept>
#include <condition_variable>

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
            std::vector<BlockId> _wait_queue;
            std::mutex _wait_lock;
            std::vector<size_t> _unpin_indexs; 
            std::condition_variable _condition_var;
            int _curr_time = 0;
    };
}