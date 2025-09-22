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
    static constexpr unsigned POWER2_OF_RAM = 16;
    static constexpr unsigned RAM_SIZE = 1 << POWER2_OF_RAM;
    static constexpr unsigned CHUNK_SIZE = 64;
    static constexpr unsigned CHUNK_ARRAY = (RAM_SIZE / CHUNK_SIZE + 7) / 8;
    static constexpr unsigned LISTS_AMOUNT = (POWER2_OF_RAM << 1) - 1;
    static constexpr unsigned LISTS_ARRAY = LISTS_AMOUNT * sizeof(DataBlock);
    static constexpr unsigned ARRAY_SIZE = LISTS_ARRAY + RAM_SIZE + CHUNK_ARRAY;

    RAMModel();

    /*! throws std::bad_alloc */
    size_t allocate(size_t bytes);
    void free(size_t address);
    void print_diagnostics();

private:
    static unsigned closest_pw2(unsigned n);
    DataBlock* get_data_block_at(unsigned ith);
    void print_bitmap_diagnostics();
    void print_lists_diagnostics();
    DataBlock* get_last_free_block(unsigned block_id);

    std::array<char, ARRAY_SIZE> data_;
};
