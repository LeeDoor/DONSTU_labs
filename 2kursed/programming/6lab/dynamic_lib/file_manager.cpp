#include "file_manager.h"
#include "../static_lib/file_operations.h"
#include <iostream>
#include <fstream>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

namespace FileOps {
    FileManager::FileManager() : currentFile(""), isLocked(false), accessMode(0) {}
    
    FileManager::FileManager(const std::string& filename) 
        : currentFile(filename), isLocked(false), accessMode(2) {}
    
    FileManager::~FileManager() {
        if (isLocked) unlock();
        closeFile();
    }
    
    bool FileManager::openFile(const std::string& filename, int mode) {
        if (isLocked) unlock();
        currentFile = filename;
        accessMode = mode;
        
        if (mode == 0 || mode == 2) {
            std::ifstream testFile(filename);
            if (!testFile.good()) {
                std::cerr << "Error: file doesn't exist " << filename << std::endl;
                currentFile = "";
                return false;
            }
            testFile.close();
        }
        
        std::cout << "File opened: " << filename 
                  << " mode: " << (mode == 0 ? "read" : (mode == 1 ? "write" : "read/write")) 
                  << std::endl;
        return true;
    }
    
    void FileManager::closeFile() {
        if (isLocked) unlock();
        currentFile = "";
        accessMode = 0;
    }
    
    std::vector<std::string> FileManager::read() {
        if (!canRead()) {
            std::cerr << "Error: no read permission" << std::endl;
            return {};
        }
        if (currentFile.empty()) {
            std::cerr << "Error: no file opened" << std::endl;
            return {};
        }
        return FileOps::readFile(currentFile);
    }
    
    bool FileManager::write(const std::vector<std::string>& lines) {
        if (!canWrite()) {
            std::cerr << "Error: no write permission" << std::endl;
            return false;
        }
        if (currentFile.empty()) {
            std::cerr << "Error: no file opened" << std::endl;
            return false;
        }
        if (isLocked) {
            std::cerr << "Error: file is locked" << std::endl;
            return false;
        }
        return FileOps::writeFile(currentFile, lines);
    }
    
    bool FileManager::append(const std::vector<std::string>& lines) {
        if (!canWrite()) {
            std::cerr << "Error: no write permission" << std::endl;
            return false;
        }
        if (currentFile.empty()) {
            std::cerr << "Error: no file opened" << std::endl;
            return false;
        }
        if (isLocked) {
            std::cerr << "Error: file is locked" << std::endl;
            return false;
        }
        return FileOps::appendToFile(currentFile, lines);
    }
    
    bool FileManager::modify(int lineNum, const std::string& newContent) {
        if (!canWrite()) {
            std::cerr << "Error: no write permission" << std::endl;
            return false;
        }
        if (currentFile.empty()) {
            std::cerr << "Error: no file opened" << std::endl;
            return false;
        }
        if (isLocked) {
            std::cerr << "Error: file is locked" << std::endl;
            return false;
        }
        return FileOps::modifyLine(currentFile, lineNum, newContent);
    }
    
    bool FileManager::removeLine(int lineNum) {
        if (!canWrite()) {
            std::cerr << "Error: no write permission" << std::endl;
            return false;
        }
        if (currentFile.empty()) {
            std::cerr << "Error: no file opened" << std::endl;
            return false;
        }
        if (isLocked) {
            std::cerr << "Error: file is locked" << std::endl;
            return false;
        }
        return FileOps::deleteLine(currentFile, lineNum);
    }
    
    bool FileManager::removeFile() {
        if (currentFile.empty()) {
            std::cerr << "Error: no file opened" << std::endl;
            return false;
        }
        if (isLocked) {
            std::cerr << "Error: file is locked" << std::endl;
            return false;
        }
        bool result = FileOps::deleteFile(currentFile);
        if (result) closeFile();
        return result;
    }
    
    bool FileManager::lock() {
        if (currentFile.empty()) {
            std::cerr << "Error: no file opened" << std::endl;
            return false;
        }
        int fd = open(currentFile.c_str(), O_RDWR);
        if (fd == -1) {
            std::cerr << "Error opening file for locking" << std::endl;
            return false;
        }
        if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
            close(fd);
            std::cerr << "Error locking file" << std::endl;
            return false;
        }
        isLocked = true;
        std::cout << "File locked: " << currentFile << std::endl;
        close(fd);
        return true;
    }
    
    void FileManager::unlock() {
        if (!isLocked) return;
        int fd = open(currentFile.c_str(), O_RDWR);
        if (fd != -1) {
            flock(fd, LOCK_UN);
            close(fd);
        }
        isLocked = false;
        std::cout << "File unlocked: " << currentFile << std::endl;
    }
    
    bool FileManager::isFileLocked() const { return isLocked; }
    
    std::string FileManager::getInfo() const {
        if (currentFile.empty()) return "No file opened";
        return FileOps::getFileInfo(currentFile);
    }
    
    std::string FileManager::getCurrentFilename() const { return currentFile; }
    
    bool FileManager::isOpen() const { return !currentFile.empty(); }
    
    bool FileManager::canWrite() const { return accessMode == 1 || accessMode == 2; }
    
    bool FileManager::canRead() const { return accessMode == 0 || accessMode == 2; }
}
