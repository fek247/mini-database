#include "buffer.h"

namespace MiniDB {
    void Buffer::pin(int time) {
        _pin_count++;
        _time_read_in = time;
        _time_unppined = -1;
    }

    void Buffer::unpin(int time) {
        if (_pin_count > 0) {
            _pin_count--;
        }
        if (_pin_count == 0) {
            _time_unppined = time;
        }
    }

    void Buffer::assignToBlock(const BlockId& block) {
        flush();
        _file_mgr.read(block, _buffer_page);
        _assigned_block = block;
        _pin_count = 0;
    }

    void Buffer::flush() {
        if (_is_dirty && _assigned_block) {
            _file_mgr.write(_assigned_block.value(), _buffer_page);
            _is_dirty = false;
        }
    }
}