#include "buffer.h"

namespace MiniDB {
    void Buffer::pinBlock(const BlockId& block, int time) {
        _is_pinned = true;
        _time_read_in = time;
        _time_unppined = -1;
        _assigned_block = block;
    }
}