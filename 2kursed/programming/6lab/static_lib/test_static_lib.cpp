#include "file_operations.h"
#include <iostream>

int main() {
    const std::string testFile = "test_file.txt";
    
    std::cout << "=== Testing static library ===" << std::endl;
    
    std::cout << "\n1. Writing to file:" << std::endl;
    std::vector<std::string> initialContent = {
        "Line 1: Hello world!",
        "Line 2: This is test file.",
        "Line 3: Created for library testing."
    };
    
    FileOps::writeFile(testFile, initialContent);
    
    std::cout << "\n2. Reading file:" << std::endl;
    auto content = FileOps::readFile(testFile);
    for (size_t i = 0; i < content.size(); ++i) {
        std::cout << "Line " << i + 1 << ": " << content[i] << std::endl;
    }
    
    std::cout << "\n3. Appending to file:" << std::endl;
    std::vector<std::string> additionalContent = {
        "Line 4: Added line.",
        "Line 5: Another added line."
    };
    FileOps::appendToFile(testFile, additionalContent);
    
    std::cout << "\n4. Modifying line:" << std::endl;
    FileOps::modifyLine(testFile, 2, "Line 2: MODIFIED LINE!");
    
    std::cout << "\n5. Deleting line:" << std::endl;
    FileOps::deleteLine(testFile, 3);
    
    std::cout << "\n6. File info:" << std::endl;
    std::cout << FileOps::getFileInfo(testFile) << std::endl;
    
    std::cout << "\n7. Reading modified file:" << std::endl;
    content = FileOps::readFile(testFile);
    for (size_t i = 0; i < content.size(); ++i) {
        std::cout << "Line " << i + 1 << ": " << content[i] << std::endl;
    }
    
    std::cout << "\n8. Deleting file:" << std::endl;
    FileOps::deleteFile(testFile);
    
    std::cout << "\n9. Info after deletion:" << std::endl;
    std::cout << FileOps::getFileInfo(testFile) << std::endl;
    
    return 0;
}
