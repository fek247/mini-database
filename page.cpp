#include <page.h>

namespace MiniDB {
    void Page::setInt(int offset, int val) {
        memcpy(_byte_buffers.data() + offset, &val, sizeof(val));
    }

    void Page::setString(int offset, const std::string& str) {
        memcpy(_byte_buffers.data() + offset, &str, sizeof(str));
    }

    void Page::setBytes(int offset, const std::vector<std::byte>& bytes) {
        memcpy(_byte_buffers.data() + offset, &bytes, sizeof(bytes));
    }

    int Page::getInt(int offset) {
        int res;
        memcpy(&res, _byte_buffers.data() + offset, sizeof(res));
        return res;
    }

    std::string Page::getString(int offset) {
        int size;
        memcpy(&size, _byte_buffers.data() + offset, sizeof(size));
        const char* start = reinterpret_cast<const char*>(_byte_buffers.data() + offset + sizeof(size));
        return std::string(start, size);
    }

    std::vector<std::byte> Page::getBytes(int offset) {
        int size;
        memcpy(&size, _byte_buffers.data() + offset, sizeof(size));
        auto first = _byte_buffers.begin() + offset + sizeof(size);
        return std::vector<std::byte>(first, first + size);
    }
}