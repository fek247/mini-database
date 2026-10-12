#include <block_id.h>
#include <functional>

namespace MiniDB {
    bool BlockId::equals(const BlockId& block) {
        return block.fileName() == _file_name && block.blockNumber() == _block_number;
    }

    std::string BlockId::toString() {
        return "[file " + _file_name + ", block " + _block_number + "]";
    }

    int BlockId::hashCode() {
        return std::hash<std::string>{}(toString());
    }
}