#pragma once
#include <array>
#include <cstddef>

class RAMModel {
public:
    static constexpr unsigned RAM_SIZE = 1024;
    static constexpr unsigned CHUNK_SIZE = 64;
    static constexpr unsigned CHUNK_ARRAY = (RAM_SIZE / CHUNK_SIZE + 7) / 8;
    static constexpr unsigned ARRAY_SIZE = RAM_SIZE + CHUNK_ARRAY;

    RAMModel();

    /*! throws std::bad_alloc */
    size_t allocate(size_t bytes);
    void free(size_t address);
    void print_diagnostics();

private:
    std::array<char, ARRAY_SIZE> data_;
};
