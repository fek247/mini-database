#include "log_iterator.h"

namespace MiniDB {
    bool LogIterator::hasNext() {
        int len = _log_iterator_page.getInt(0);
        return _curr_offset < len;
    }

    std::optional<std::vector<std::byte>> LogIterator::next() {
        if (!hasNext()) {
            return std::nullopt;
        }

        std::vector<std::byte> bytes = _log_iterator_page.getBytes(_curr_offset);
        _curr_offset += sizeof(int) + bytes.size();
        return bytes;
    }
}