#include "ram_model.hpp"
#include <iostream>

unsigned RAMModel::closest_pw2(unsigned n) {
    if (n == 0) return 1;
    if (n == 1) return 1;

    unsigned int power = 1;
    while ((1 << power) < n) {
        ++power;
    }
    return power;
}

RAMModel::RAMModel() {
    data_.fill(0);
    get_data_block_at(1)->address = LISTS_ARRAY;
    get_data_block_at(1)->taken = 1;
    get_data_block_at(1)->next = 2;
    get_data_block_at(2)->address = LISTS_ARRAY + RAM_SIZE / 2;
    get_data_block_at(1)->taken = 1;
    get_data_block_at(2)->next = 2;
}

/*! throws std::bad_alloc */
size_t RAMModel::allocate(size_t bytes) {
    unsigned chunk_pw2 = closest_pw2(bytes);
    while((1 << chunk_pw2) < CHUNK_SIZE) 
        ++chunk_pw2;
    if(chunk_pw2 > POWER2_OF_RAM) throw std::bad_alloc();
    unsigned block_list_id = (1 << (POWER2_OF_RAM - chunk_pw2)) - 1;
    DataBlock* closest;
    do get_last_free_block(block_list_id++);
    while(!closest->taken);
    std::cout << "selected block: " << closest->address << std::endl;
    return 0;
}

RAMModel::DataBlock* RAMModel::get_last_free_block(unsigned block_id) {
    DataBlock* block = get_data_block_at(block_id);
    while(block->taken) {
        auto next = get_data_block_at(block->next);
        if(block == next) return block;
        block = next;
    }
    return block;
}

void RAMModel::free(size_t address) {
    std::cout << "Freeing " << address << "\n";
}
RAMModel::DataBlock* RAMModel::get_data_block_at(unsigned ith) {
    if(ith > POWER2_OF_RAM - 8) throw std::bad_alloc();
    return reinterpret_cast<DataBlock*>(data_.data() + sizeof(DataBlock) * ith);
}

void RAMModel::print_diagnostics() {
    print_bitmap_diagnostics();
    print_lists_diagnostics();
}
void RAMModel::print_bitmap_diagnostics() {
    constexpr auto start = LISTS_ARRAY + RAM_SIZE;
    constexpr auto end = start + CHUNK_ARRAY;

    std::cout << "Bitmap statistics:" << std::endl;
    for(size_t i = start; i < end; ++i) {
        char chunk = data_.at(i);
        for(size_t b = 0; b < 8; ++b) {
            bool taken = chunk & (1 << b);
            std::cout << static_cast<int>(taken);
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}

void RAMModel::print_lists_diagnostics() {
    constexpr auto start = 0;
    constexpr auto end = start + LISTS_ARRAY;

    unsigned step = 0;
    for(unsigned cur_block_size = POWER2_OF_RAM; (1 << cur_block_size) >= CHUNK_SIZE; --cur_block_size) {
        DataBlock* cur = get_data_block_at(step);
        while(cur->next != cur->address) {
            std::cout << (1 << cur_block_size) << "B At: " << cur->address << std::endl;
            auto next = get_data_block_at(cur->next);
            if(cur == next) break;
            cur = next;
        }
        step = step * 2 + 1;
    }
}

