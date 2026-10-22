#pragma once

#include <optional>
#include "file/block_id.h"
#include "file/page.h"
#include "file/file_manager.h"

namespace MiniDB {
    class Buffer {
        public:
            Buffer(FileManager& file_mgr, Page page) : _file_mgr(file_mgr), _buffer_page(std::move(page)) {};
            bool isPinned() { return _pin_count > 0; };
            int getTimeReadIn() { return _time_read_in; };
            int getTimeUnpinned() { return _time_unppined; };
            const std::optional<BlockId>& getAssignedBlock() { return _assigned_block; };
            void pin(int time);
            void unpin(int time);
            void assignToBlock(const BlockId& block);
            void setModified() { _is_dirty = true; };
            Page& contents() { return _buffer_page; };
            void flush();
        private:
            FileManager& _file_mgr;
            std::optional<BlockId> _assigned_block;
            Page _buffer_page;
            bool _is_dirty = false;
            int _pin_count = 0;
            int _time_read_in = 0;
            int _time_unppined = -1;
    };
}