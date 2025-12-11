#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

namespace FileOps {
    class FileManager {
    private:
        std::string currentFile;
        bool isLocked;
        int accessMode;
        
    public:
        FileManager();
        explicit FileManager(const std::string& filename);
        ~FileManager();
        
        bool openFile(const std::string& filename, int mode = 2);
        void closeFile();
        std::vector<std::string> read();
        bool write(const std::vector<std::string>& lines);
        bool append(const std::vector<std::string>& lines);
        bool modify(int lineNum, const std::string& newContent);
        bool removeLine(int lineNum);
        bool removeFile();
        bool lock();
        void unlock();
        bool isFileLocked() const;
        std::string getInfo() const;
        std::string getCurrentFilename() const;
        bool isOpen() const;
        bool canWrite() const;
        bool canRead() const;
    };
}

#endif
