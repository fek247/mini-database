#pragma once

#include <optional>
#include "block_id.h"
#include "page.h"

namespace MiniDB {
    class Buffer {
        public:
            Buffer(const Page& page) : _buffer_page(page) {};
            void assignBlock(const BlockId& block) { _assigned_block = block; };
            bool isPinned() { return _is_pinned; };
            int getTimeReadIn() { return _time_read_in; };
            int getTimeUnpinned() { return _time_unppined; };
            void pinBlock(const BlockId& block, int time);
        private:
            std::optional<BlockId> _assigned_block;
            Page _buffer_page;
            bool _is_pinned;
            int _time_read_in;
            int _time_unppined;
    };
}