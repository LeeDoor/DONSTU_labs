#include <iostream>
#include "../get_valid_input.hpp"
#include "ram_model.hpp"

void print_header() {
    std::cout 
        << "====================" << std::endl
        << "RAM SIZE: "   << RAMModel::RAM_SIZE   << std::endl
        << "CHUNK SIZE: " << RAMModel::CHUNK_ARRAY << std::endl
        << "ARRAY SIZE: " << RAMModel::ARRAY_SIZE << std::endl
        << "====================" << std::endl
    ;
}
void print_menu() {
    std::cout 
        << "1) allocate memory" << std::endl
        << "2) free memory" << std::endl
        << "3) show memory allocation info" << std::endl
        << "0) exit" << std::endl
        << "Enter number of action> "
    ;
}

RAMModel ram;

bool apply_menu() {
    print_menu();
    int choice = get_valid_input(0, 3);
    switch(choice) {
        case 1: {
            std::cout << "Enter the size of allocating chunk [1; " << RAMModel::RAM_SIZE << "]> ";
            int bytes = get_valid_input(1, static_cast<int>(RAMModel::RAM_SIZE));
            try {
                size_t address = ram.allocate(bytes);
                std::cout << "Allocated " << bytes << " bytes at " << address << std::endl;
            } catch(const std::bad_alloc& ex) {
                std::cout << "Insufficient memory to perform an operation. Skip" << std::endl;
            }
            break;
        }
        case 2: {
            std::cout << "Enter the address of a chunk to free [0; " << RAMModel::RAM_SIZE << "]> ";
            int address = get_valid_input(1, static_cast<int>(RAMModel::RAM_SIZE));
            ram.free(address);
            std::cout << "Freed at " << address << std::endl;
            break;
        }
        case 3: {
            ram.print_diagnostics();
            break;
        }
        case 0: return false;
    }
    return true;
}

int main() {
    print_header();
    while(apply_menu());
}
