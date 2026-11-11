#include "buffer_manager.h"

namespace MiniDB {
    Buffer BufferManager::pin(const BlockId& block) {
        long long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto buffer = tryToPin(block);
        while (!buffer.has_value() && !waitingTooLong(now)) {
            std::unique_lock<std::mutex> lock(_unpin_lock);
            _condition_var.wait(lock, [this] {
                return _has_unpin;
            });
            if (auto r = tryToPin(block)) {
                buffer.emplace(std::move(*r));
            }
        }

        if (!buffer.has_value()) {
            throw std::runtime_error("Buffer abort exception");
        }
        _curr_time++;

        return buffer.value();
    }

    void BufferManager::unpin(Buffer& buffer) {
        _curr_time++;
        buffer.unpin(_curr_time);
        if (!buffer.isPinned()) {
            {
                std::lock_guard<std::mutex> lock(_unpin_lock);
                _has_unpin = true;
            }
            _condition_var.notify_all();
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

    std::optional<Buffer> BufferManager::tryToPin(const BlockId& block) {
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
            return std::nullopt;
        }

        if (!exist) {
            _buffer_pool[target_index].assignToBlock(block);
        }
        _buffer_pool[target_index].pin(_curr_time);

        return _buffer_pool[target_index]; 
    }

    bool BufferManager::waitingTooLong(long long timestamp) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timestamp > MAX_TIME;
    }
}