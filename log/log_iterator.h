#pragma once

#include <optional>
#include "file/file_manager.h"
#include "file/block_id.h"
#include "file/page.h"

namespace MiniDB {
    class LogIterator {
        public:
            LogIterator(FileManager& file_manager, const BlockId& block_id) :  _file_manager(_file_manager), _block_id(block_id), _log_iterator_page(Page(file_manager.blockSize())) {
                file_manager.read(_block_id, _log_iterator_page);
            };
            bool hasNext();
            std::optional<std::vector<std::byte>> next();
        private:
            FileManager& _file_manager;
            BlockId _block_id;
            Page _log_iterator_page;
            int _curr_offset = 0;
    };
}