#pragma once
#include <array>
#include <climits>
#include <cstddef>
#include <cstdint>

class RAMModel {
public:
    struct DataBlock {
        uint16_t address; // specific for POWER2_OF_RAM = 16
        uint8_t taken;
        uint8_t next;
    };
    static constexpr unsigned MEMORY_SIZE = 1024;
    static constexpr unsigned BLOCK_SIZE = 8;
    static constexpr unsigned BITMAP_SIZE = ((MEMORY_SIZE / BLOCK_SIZE + 7) / 8);

    RAMModel();

    /*! throws std::bad_alloc */
    size_t allocate(size_t bytes);
    void free(size_t address);
    void print_diagnostics();

private:
    void set_bit(int position, int value);
    int get_bit(int position);
    int allocate_block(int size);
    void free_block(int address);
    void get_memory_info();

    unsigned char memory[MEMORY_SIZE];
    unsigned char bitmap[BITMAP_SIZE];
};
