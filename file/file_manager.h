#pragma once

#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unordered_map>
#include <mutex>
#include <sys/stat.h>
#include "block_id.h"
#include "page.h"

namespace MiniDB {
    class FileManager {
        public:
            FileManager(const std::string& db_name, int block_size);
            ~FileManager();
            bool isNew() { return _is_new; };
            void read(const BlockId& block, Page& page);
            void write(BlockId& block, Page& page);
            BlockId append(const std::string& file_name);
            int length(const std::string& file_name);
            int blockSize() { return _block_size; };
        private:
            std::string _db_name;
            int _block_size;
            bool _is_new;
            std::unordered_map<std::string, int> _open_files;
            std::mutex _mutex;
            std::mutex _append_mutex;
            int getFile(const std::string& file_name);
            void pwriteAll(int fd, const char* src, off_t offset, size_t len);
    };
}