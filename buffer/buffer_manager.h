#pragma once

#include "buffer.h"
#include "file/file_manager.h"
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
            Buffer pin(const BlockId& block);
            void unpin(Buffer& buffer);
            void flushAll();
        private:
            FileManager& _file_mgr;
            std::vector<Buffer> _buffer_pool;
            std::mutex _unpin_lock;
            std::condition_variable _condition_var;
            bool _has_unpin = false;
            int _curr_time = 0;
            std::optional<Buffer> tryToPin(const BlockId& block);
            bool waitingTooLong(long long timestamp);
            const long MAX_TIME = 10000;
    };
}