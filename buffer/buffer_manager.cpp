#include "buffer_manager.h"

namespace MiniDB {
    int BufferManager::pin(const BlockId& block) {
        int min_time_readin_index = -1;
        int min_time_readin = -1;
        for (int i = 0; i < _buffer_pool.size(); i++) {
            if (!_buffer_pool[i].isPinned()) {
                if (min_time_readin == -1) {
                    min_time_readin = _buffer_pool[i].getTimeReadIn();
                    min_time_readin_index = i;
                } else {
                    min_time_readin = std::min(min_time_readin, _buffer_pool[i].getTimeReadIn());
                    min_time_readin_index = _buffer_pool[i].getTimeReadIn() < min_time_readin ? i : min_time_readin_index;
                }
            }
        }

        if (min_time_readin_index == -1) {
            throw std::runtime_error("Don't have any unpinned buffer in buffer poll. Need queue this request! Implement later");
        }

        _curr_time++;
        _buffer_pool[min_time_readin_index].pinBlock(block, _curr_time);

        return min_time_readin_index;
    }
}