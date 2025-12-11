#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include <vector>

namespace FileOps {
    std::vector<std::string> readFile(const std::string& filename);
    bool writeFile(const std::string& filename, const std::vector<std::string>& lines);
    bool appendToFile(const std::string& filename, const std::vector<std::string>& lines);
    bool modifyLine(const std::string& filename, int lineNum, const std::string& newContent);
    bool deleteLine(const std::string& filename, int lineNum);
    bool deleteFile(const std::string& filename);
    std::string getFileInfo(const std::string& filename);
}

#endif
