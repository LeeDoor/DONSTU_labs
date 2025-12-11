#include "static_lib/file_operations.h"
#include "dynamic_lib/file_manager.h"
#include <iostream>

int main() {
    std::cout << "=== File System Operations Demo ===" << std::endl;
    
    std::cout << "\n--- Direct static library usage ---" << std::endl;
    std::vector<std::string> content = {"Main test line 1", "Main test line 2"};
    FileOps::writeFile("main_test.txt", content);
    
    auto readContent = FileOps::readFile("main_test.txt");
    for (const auto& line : readContent) {
        std::cout << line << std::endl;
    }
    
    std::cout << "\n--- FileManager usage ---" << std::endl;
    FileOps::FileManager manager("managed_file.txt");
    
    std::vector<std::string> managerData = {
        "Managed data line 1",
        "Managed data line 2",
        "Managed data line 3"
    };
    
    manager.write(managerData);
    
    auto managerContent = manager.read();
    std::cout << "File content:" << std::endl;
    for (size_t i = 0; i < managerContent.size(); ++i) {
        std::cout << i+1 << ": " << managerContent[i] << std::endl;
    }
    
    std::cout << "\nFile information:" << std::endl;
    std::cout << manager.getInfo() << std::endl;
    
    manager.modify(2, "Updated line 2");
    
    std::cout << "\nAfter modification:" << std::endl;
    managerContent = manager.read();
    for (size_t i = 0; i < managerContent.size(); ++i) {
        std::cout << i+1 << ": " << managerContent[i] << std::endl;
    }
    
    manager.removeFile();
    FileOps::deleteFile("main_test.txt");
    
    std::cout << "\nAll files cleaned up" << std::endl;
    
    return 0;
}
