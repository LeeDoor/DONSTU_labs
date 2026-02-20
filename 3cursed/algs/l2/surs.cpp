#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>

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

std::list<std::string> get_longest_lines(std::shared_ptr<std::fstream> ifs) {
    std::string line;
    std::list<std::string> result;
    do {
        std::getline(*ifs, line);
        if(!result.empty() && line.size() > result.begin()->size()) {
            result.clear();
        }
        if(result.empty() || result.begin()->size() == line.size()) {
            result.push_back(std::move(line));
        }
    } while(!ifs->eof());
    return result;
}

int main() {
    auto ifs = get_valid_filename();
    auto surnames = get_longest_lines(ifs);
    std::cout << "Longest surnames: " << std::endl;
    std::for_each(surnames.begin(), surnames.end(), 
            [](const std::string& str) {
                std::cout << str << std::endl;
            });
}
