#include "filesystem.h"
#include <iostream>

int main() {
    FileSystem fs;
    if (!fs.create("test.fs", 4096)) {
        std::cout << "Failed to create filesystem" << std::endl;
        return 1;
    }
    std::cout << "Filesystem created" << std::endl;
    fs.mkdir("/docs");
    fs.mkdir("/data");
    int fd = fs.open("/test.txt");
    if (fd >= 0) {
        std::string text = "Hello, linked block list filesystem!";
        std::vector<uint8_t> data(text.begin(), text.end());
        fs.write(fd, data, data.size());
        fs.close(fd);
        std::cout << "File written" << std::endl;
    }
    auto listing = fs.list("/");
    std::cout << "Root directory contents:" << std::endl;
    for (const auto& item : listing) {
        std::cout << "  " << item << std::endl;
    }
    fd = fs.open("/test.txt");
    if (fd >= 0) {
        std::vector<uint8_t> buffer;
        uint32_t read = fs.read(fd, buffer, 100);
        std::string content(buffer.begin(), buffer.end());
        std::cout << "Read from file: " << content << std::endl;
        fs.close(fd);
    }
    if (fs.import("main.cpp", "/imported.cpp")) {
        std::cout << "File imported successfully" << std::endl;
    }
    listing = fs.list("/");
    std::cout << "After import:" << std::endl;
    for (const auto& item : listing) {
        std::cout << "  " << item << std::endl;
    }
    fs.remove("/test.txt");
    fs.rmdir("/docs");
    std::cout << "Current directory: " << fs.pwd() << std::endl;
    return 0;
}
