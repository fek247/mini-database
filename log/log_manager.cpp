#include "log_manager.h"

namespace MiniDB {
    static constexpr int INT_SIZE = sizeof(int32_t);

    int LogManager::append(const std::vector<std::byte>& record) {
        std::lock_guard<std::mutex> lock(_lock_mutex);
        int curr_len = _log_page.getInt(0);
        int record_size = record.size();
        int byte_needed = record_size + INT_SIZE;

        if (INT_SIZE + byte_needed > _file_mgr.blockSize()) {
            throw std::runtime_error("log record too large");
        }

        int remain = _file_mgr.blockSize() - INT_SIZE - curr_len;
        if (remain < byte_needed) {
            flushPage();
            _log_page.clear();
            _curr_block = _file_mgr.append(_log_file_name);
            _log_page.setInt(INT_SIZE, record_size);
            _log_page.setInt(0, INT_SIZE + record_size);
            memcpy(_log_page.data() + INT_SIZE + INT_SIZE, record.data(), record.size());
        } else {
            _log_page.setInt(0, curr_len + INT_SIZE + record_size);
            _log_page.setInt(INT_SIZE + curr_len, record_size);
            memcpy(_log_page.data() + INT_SIZE + curr_len + INT_SIZE, record.data(), record_size);
        }

        _last_lsn++;

        return _last_lsn;
    }

    // client can call flush so we need to check validate lsn
    void LogManager::flush(int lsn) {
        std::lock_guard<std::mutex> lock(_lock_mutex);
        if (_last_saved_lsn < lsn) {
            flushPage();
        }
    }

    void LogManager::flushPage() {
        _file_mgr.write(_curr_block, _log_page);
        _last_saved_lsn = _last_lsn;
    }

    LogIterator LogManager::iterator() {
        flushPage();
        return LogIterator(_file_mgr, _curr_block);
    }
}