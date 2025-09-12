#include "ram_model.hpp"
#include <iostream>

RAMModel::RAMModel() {
    data_.fill(0);
}

/*! throws std::bad_alloc */
size_t RAMModel::allocate(size_t bytes) {
    return 0;
}
void RAMModel::free(size_t address) {
    std::cout << "Freeing " << address << "\n";
}
void RAMModel::print_diagnostics() {
    for(size_t i = 0; i < CHUNK_ARRAY; ++i) {
        char chunk = data_.at(i);
        for(size_t b = 0; b < 8; ++b) {
            bool taken = chunk & (1 << b);
            std::cout << "block #" << i * 8 + b << " is " << (taken ? "taken" : "free") << std::endl;
        }
    }
}
