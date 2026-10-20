#pragma once

#include "buffer.h"
#include "file_manager.h"
#include <vector>
#include <math.h>
#include <stdexcept>

namespace MiniDB {
    class BufferManager {
        public:
            BufferManager(FileManager& file_mgr, int size) : _file_mgr(file_mgr), _buffer_pool(size) {
                for (int i = 0; i < size; i++) {
                    _buffer_pool[i] = Buffer(Page(file_mgr.blockSize()));
                }
            };
            int pin(const BlockId& block);
        private:
            FileManager& _file_mgr;
            std::vector<Buffer> _buffer_pool;
            int _curr_time;
    };
}