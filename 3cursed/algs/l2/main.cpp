#include <cmath>
#include <filesystem>
#include <optional>
#include <iostream>
#include <string>
#include <fstream>

std::optional<size_t> get_sizet(const std::string& str) {
    if(str.empty() || str.find_first_not_of("0123456789") == std::string::npos && str[0] != '0')
        return std::stoi(str);
    return std::nullopt;
}

size_t get_K() {
    std::string input;
    std::optional<int> conv;
    do {
        std::cout << "Enter positive integer K: ";
        std::getline(std::cin, input);
        conv = get_sizet(input);
    } while (!conv || conv == 0);
    return static_cast<size_t>(*conv);
}

std::shared_ptr<std::fstream> get_valid_filename() {
    std::string input;
    std::error_code ec;
    do { 
        std::cout << "enter filename: ";
        std::getline(std::cin, input);
        std::shared_ptr<std::fstream> fs = 
            std::make_shared<std::fstream>(input, std::ios::in | std::ios::out);
        if(!fs->is_open()) {
            std::cout << "file could not be opened. Try again." << std::endl;
            continue;
        } 
        if(std::filesystem::is_empty(input, ec)) {
            std::cout << "Input file is empty. Try Again." << std::endl;
            continue;
        }
        if(ec) {
            std::cout << "Failed to gather file size info. Restart with root." << std::endl;
            continue;
        }
        return fs;
    } while(true);
}

std::shared_ptr<std::fstream> create_file(const std::string& fn) {
    std::shared_ptr<std::fstream> fs = 
        std::make_shared<std::fstream>(fn, std::ios::in | std::ios::out | std::ios::trunc);
    if(!fs->is_open()) {
        return nullptr;
    }
    return fs;
}

void fill_first_file(size_t K, std::shared_ptr<std::fstream> ifs, std::shared_ptr<std::fstream> ofs) {
    std::string input;
    do {
        std::getline(*ifs, input);
        if(input.size() > K) input.resize(K);
        *ofs << input << std::endl;

    } while (!ifs->eof());
}

void fill_second_file(size_t K, std::shared_ptr<std::fstream> ifs, std::shared_ptr<std::fstream> ofs) {
    std::string input;
    do {
        std::getline(*ifs, input);
        if(input.size() < K) 
            *ofs << '*';
        else 
            *ofs << input[K - 1];
    } while (!ifs->eof());
}

int main() {
    size_t K = get_K();
    auto fs = get_valid_filename();
    auto fso1 = create_file("output_1.txt"),
         fso2 = create_file("output_2.txt"); 
    fill_first_file(K, fs, fso1);
    fs->clear();
    fs->seekg(0, std::ios::beg);
    fill_second_file(K, fs, fso2);
}
