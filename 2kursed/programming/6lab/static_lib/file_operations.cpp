#include "file_operations.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <ctime>
#include <cstdio>

namespace FileOps {
    std::vector<std::string> readFile(const std::string& filename) {
        std::vector<std::string> lines;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return lines;
        }
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
        return lines;
    }
    
    bool writeFile(const std::string& filename, const std::vector<std::string>& lines) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error creating file: " << filename << std::endl;
            return false;
        }
        for (const auto& line : lines) {
            file << line << std::endl;
        }
        file.close();
        return true;
    }
    
    bool appendToFile(const std::string& filename, const std::vector<std::string>& lines) {
        std::ofstream file(filename, std::ios_base::app);
        if (!file.is_open()) {
            std::cerr << "Error opening file for append: " << filename << std::endl;
            return false;
        }
        for (const auto& line : lines) {
            file << line << std::endl;
        }
        file.close();
        return true;
    }
    
    bool modifyLine(const std::string& filename, int lineNum, const std::string& newContent) {
        if (lineNum < 1) {
            std::cerr << "Line number must be >= 1" << std::endl;
            return false;
        }
        auto lines = readFile(filename);
        if (lines.empty() && lineNum > 1) {
            std::cerr << "File is empty or doesn't exist" << std::endl;
            return false;
        }
        if (static_cast<size_t>(lineNum) > lines.size()) {
            std::cerr << "Line " << lineNum << " doesn't exist (total lines: " << lines.size() << ")" << std::endl;
            return false;
        }
        lines[lineNum - 1] = newContent;
        return writeFile(filename, lines);
    }
    
    bool deleteLine(const std::string& filename, int lineNum) {
        if (lineNum < 1) {
            std::cerr << "Line number must be >= 1" << std::endl;
            return false;
        }
        auto lines = readFile(filename);
        if (lines.empty()) {
            std::cerr << "File is empty or doesn't exist" << std::endl;
            return false;
        }
        if (static_cast<size_t>(lineNum) > lines.size()) {
            std::cerr << "Line " << lineNum << " doesn't exist (total lines: " << lines.size() << ")" << std::endl;
            return false;
        }
        lines.erase(lines.begin() + (lineNum - 1));
        return writeFile(filename, lines);
    }
    
    bool deleteFile(const std::string& filename) {
        if (std::remove(filename.c_str()) != 0) {
            std::cerr << "Error deleting file: " << filename << std::endl;
            return false;
        }
        return true;
    }
    
    std::string getFileInfo(const std::string& filename) {
        struct stat fileInfo;
        std::stringstream info;
        if (stat(filename.c_str(), &fileInfo) != 0) {
            return "File doesn't exist or inaccessible";
        }
        info << "File: " << filename << std::endl;
        info << "Size: " << fileInfo.st_size << " bytes" << std::endl;
        char timeStr[100];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&fileInfo.st_mtime));
        info << "Last modified: " << timeStr << std::endl;
        info << "Permissions: ";
        info << ((fileInfo.st_mode & S_IRUSR) ? "r" : "-");
        info << ((fileInfo.st_mode & S_IWUSR) ? "w" : "-");
        info << ((fileInfo.st_mode & S_IXUSR) ? "x" : "-");
        info << ((fileInfo.st_mode & S_IRGRP) ? "r" : "-");
        info << ((fileInfo.st_mode & S_IWGRP) ? "w" : "-");
        info << ((fileInfo.st_mode & S_IXGRP) ? "x" : "-");
        info << ((fileInfo.st_mode & S_IROTH) ? "r" : "-");
        info << ((fileInfo.st_mode & S_IWOTH) ? "w" : "-");
        info << ((fileInfo.st_mode & S_IXOTH) ? "x" : "-");
        return info.str();
    }
}
