#include "file_manager.h"

namespace MiniDB {
    FileManager::FileManager(const std::string& db_name, int block_size) {
        _db_name = db_name;
        _block_size = block_size;
        try {
            if (std::filesystem::create_directories(db_name) == 0) {
                _is_new = true;
            } else {
                _is_new = false;
            }
        } catch (const std::filesystem::filesystem_error& err) {
            std::cerr << "Filesystem error: " << err.what() << std::endl;
        }

        for (const auto& entry : std::filesystem::directory_iterator(db_name)) {
            std::string file_name = entry.path().filename().string();
            
            if (entry.is_regular_file() && file_name.rfind("temp", 0) == 0) {
                if (std::filesystem::remove(entry.path()) != 0) {
                    std::cout << "Error when delete temp file" << entry.path().string() << " in database: " << db_name << std::endl; 
                }
            }
        }
    }

    FileManager::~FileManager() {
        for (const auto& [file_name, fd] : _open_files) {
            close(fd);
        }
    }

    void FileManager::read(const BlockId& block, Page& page) {
        int fd = getFile(block.fileName());
        if (fd == -1) {
            throw std::runtime_error("Failed to get file descriptor");
        }

        auto dst = reinterpret_cast<char*>(page.data());
        auto offset = static_cast<off_t>(_block_size) * block.blockNumber();
        size_t total = 0;
        while (total < _block_size) {
            ssize_t n = pread(fd, dst + total, _block_size - total, offset + total);

            if (n == -1) {
                if (errno == EINTR) {
                    continue;
                }

                throw std::system_error(errno, std::generic_category(), "pread");  
            }

            if (n == 0) {
                std::memset(dst + total, 0, _block_size - total);
                break;
            }
            total += static_cast<size_t>(n);
        }
    }

    void FileManager::write(BlockId& block, Page& page) {
        int fd = getFile(block.fileName());
        if (fd == -1) {
            throw std::runtime_error("Failed to get file descriptor");
        }

        auto src = reinterpret_cast<const char*>(page.data());
        auto offset = static_cast<off_t>(block.blockNumber()) * _block_size;
        pwriteAll(fd, src, offset, _block_size);
    }

    BlockId FileManager::append(const std::string& file_name) {
        int fd = getFile(file_name);
        if (fd == -1) {
            throw std::runtime_error("Failed to get file descriptor");
        }

        std::lock_guard<std::mutex> lock(_append_mutex);
        int new_block_id = length(file_name);

        std::vector<char> zeros(_block_size, 0);
        off_t offset = static_cast<off_t>(new_block_id) * _block_size;
        pwriteAll(fd, zeros.data(), offset, _block_size);

        return BlockId(file_name, new_block_id);
    }

    int FileManager::length(const std::string& file_name) {
        int fd = getFile(file_name);
        if (fd == -1) {
            throw std::runtime_error("Failed to get file descriptor");
        }

        struct stat st;

        if (fstat(fd, &st) == -1) {
            throw std::system_error(errno, std::generic_category(), "fstat");
        }

        return static_cast<int>(st.st_size / _block_size);
    }

    int FileManager::getFile(const std::string& file_name) {
        std::lock_guard<std::mutex> lock(_mutex);

        auto it = _open_files.find(file_name);
        if (it != _open_files.end()) {
            return it->second;
        }

        std::string path = _db_name + "/" + file_name;
        int fd = open(path.c_str(), O_RDWR | O_CREAT | O_CLOEXEC, S_IRUSR | S_IWUSR);

        if (fd == -1) {
            throw std::system_error(errno, std::generic_category(), "open " + path);
        }

        _open_files[file_name] = fd;
        return fd;
    }

    void FileManager::pwriteAll(int fd, const char* src, off_t offset, size_t len) {
        size_t total = 0;

        while (total < len) {
            ssize_t n = pwrite(fd, src + total, len - total, offset + total);
            if (n == -1) {
                if (errno == EINTR) {
                    continue;
                }

                throw std::system_error(errno, std::generic_category(), "pwrite");         
            }

            if (n == 0) {
                throw std::runtime_error("pwrite wrote 0 bytes");
            }
            total += static_cast<size_t>(n);
        }
    }
}