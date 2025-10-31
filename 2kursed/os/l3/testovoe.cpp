#include <cstring>
#include <iostream>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
int main() {
    std::string directory = "superdir";
    if(!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cout << "no such directory\n";
        return 1;
    }
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry.status())) {
            std::cout << entry.path().filename() << std::endl;
            std::ifstream file(entry.path());
            std::string line;
            while (std::getline(file, line)) {
                if(std::strcmp(line.c_str(), "stop process\n")) {
                    std::cout << "met '" << line << "' in file\n";
                    file.close();
                    return 1;
                }
            }
            file.close();
        }
    }

}
