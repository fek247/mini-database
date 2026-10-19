#pragma once

#include <string>
#include <cstring>
#include <stdexcept>
#include <mutex>
#include "page.h"
#include "file_manager.h"

namespace MiniDB {
    class LogManager {
        public:
            LogManager(FileManager& file_mgr, const std::string& log_file_name) : 
                _file_mgr(file_mgr), 
                _log_file_name(log_file_name), 
                _log_page(Page(file_mgr.blockSize())),
                _curr_block(file_mgr.append(log_file_name)) {
                    _log_page.setInt(0, 0);
                };
            int append(const std::vector<std::byte>& record);
            void flush(int lsn);
        private:
            int _last_lsn = 0;
            int _last_saved_lsn = 0;
            FileManager& _file_mgr;
            std::string _log_file_name;
            Page _log_page;
            BlockId _curr_block;
            std::mutex _lock_mutex;
            void flushPage();
    };
}