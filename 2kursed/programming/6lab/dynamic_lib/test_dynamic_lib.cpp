#include "file_manager.h"
#include <iostream>

int main() {
    std::cout << "=== Testing dynamic library ===" << std::endl;
    
    FileOps::FileManager fm;
    
    std::cout << "\n1. Opening file:" << std::endl;
    fm.openFile("test_manager.txt", 2);
    
    std::cout << "\n2. Writing to file:" << std::endl;
    std::vector<std::string> data = {
        "Manager line 1",
        "Manager line 2",
        "Manager line 3"
    };
    fm.write(data);
    
    std::cout << "\n3. Reading file:" << std::endl;
    auto content = fm.read();
    for (size_t i = 0; i < content.size(); ++i) {
        std::cout << "Line " << i + 1 << ": " << content[i] << std::endl;
    }
    
    std::cout << "\n4. Locking file:" << std::endl;
    fm.lock();
    
    std::cout << "\n5. Trying to write while locked:" << std::endl;
    fm.write({"Test line"});
    
    std::cout << "\n6. Unlocking file:" << std::endl;
    fm.unlock();
    
    std::cout << "\n7. Modifying line 2:" << std::endl;
    fm.modify(2, "MODIFIED LINE 2");
    
    std::cout << "\n8. Appending data:" << std::endl;
    fm.append({"Appended line 1", "Appended line 2"});
    
    std::cout << "\n9. File info:" << std::endl;
    std::cout << fm.getInfo() << std::endl;
    
    std::cout << "\n10. Removing line 1:" << std::endl;
    fm.removeLine(1);
    
    std::cout << "\n11. Final content:" << std::endl;
    content = fm.read();
    for (size_t i = 0; i < content.size(); ++i) {
        std::cout << "Line " << i + 1 << ": " << content[i] << std::endl;
    }
    
    std::cout << "\n12. Deleting file:" << std::endl;
    fm.removeFile();
    
    return 0;
}
