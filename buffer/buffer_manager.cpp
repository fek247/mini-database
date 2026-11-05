#include "buffer_manager.h"

namespace MiniDB {
    Buffer& BufferManager::pin(const BlockId& block) {
        int target_index = -1;
        int min_time_readin = -1;
        bool exist = false;
        for (size_t i = 0; i < _buffer_pool.size(); i++) {
            auto assign_block = _buffer_pool[i].getAssignedBlock();
            if (assign_block && assign_block.value().equals(block)) {
                target_index = i;
                exist = true;
                break;
            }

            if (!_buffer_pool[i].isPinned()) {
                if (min_time_readin == -1) {
                    min_time_readin = _buffer_pool[i].getTimeReadIn();
                    target_index = i;
                } else {
                    target_index = _buffer_pool[i].getTimeReadIn() < min_time_readin ? i : target_index;
                    min_time_readin = std::min(min_time_readin, _buffer_pool[i].getTimeReadIn());
                }
            }
        }

        if (target_index == -1) {
            std::unique_lock lock(_wait_lock);
            _condition_var.wait(lock, [this] {
                return !_unpin_indexs.empty();
            });
            target_index = _unpin_indexs.back();
            _unpin_indexs.pop_back();
            lock.unlock();
        }

        _curr_time++;
        if (!exist) {
            _buffer_pool[target_index].assignToBlock(block);
        }
        _buffer_pool[target_index].pin(_curr_time);

        return _buffer_pool[target_index];
    }

    void BufferManager::unpin(Buffer& buffer) {
        _curr_time++;
        buffer.unpin(_curr_time);
        if (!buffer.isPinned()) {
             
        }
    }

    void BufferManager::flushAll() {
        for (auto& buffer : _buffer_pool) {
            buffer.flush();
        }
    }

    BufferManager::~BufferManager() {
        flushAll();
    }
}