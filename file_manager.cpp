#include <file_manager.h>

namespace MiniDB {
    FileManager::FileManager(const std::string& db_name, int block_size) {
        _db_name = db_name;
        _block_size = block_size;
        // std::filesystem::path dir_path = db_name;
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

    void FileManager::read(const BlockId& block, Page& page) {
        int fd = getFile(block.fileName());
        if (fd == -1) {
            std::cerr << "Failed to get file" << std::endl;
            return;
        }

        char* dst = reinterpret_cast<char*>(page.data());
        int offset = static_cast<off_t>(_block_size) * block.blockNumber();
        int total = 0;
        while (total < _block_size) {
            ssize_t n = pread(fd, dst + total, _block_size - total, offset + total);

            if (n == -1) {
                if (errno == EINTR) {
                    continue;
                }

                std::cerr << "Error when read block" << std::endl;
                return;
            }

            if (n == 0) {
                break;
            }
            total += n;
        }

        close(fd);
    }

    int FileManager::getFile(const std::string& file_name) {
        std::string path = _db_name + "/" + file_name;
        int fd = open(path.c_str(), O_RDWR | O_CREAT | O_CLOEXEC, S_IRUSR | S_IWUSR);

        if (fd == -1) {
            std::cerr << "Error opening/creating " << path << ": " << strerror(errno) << std::endl;
            return -1;
        }

        return fd;
    }
}