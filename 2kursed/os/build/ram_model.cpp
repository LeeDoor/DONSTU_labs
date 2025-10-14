#include "ram_model.hpp"
#include <cstring>
#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>

#define MEMORY_SIZE 1024
#define BLOCK_SIZE 8
#define BITMAP_SIZE ((MEMORY_SIZE / BLOCK_SIZE + 7) / 8)

unsigned char memory[MEMORY_SIZE];
unsigned char bitmap[BITMAP_SIZE];
std::map<int, std::vector<std::pair<int, int>>> buddy_allocated_blocks;

void RAMModel::set_bit(int position, int value) {
    int byte_index = position / 8;
    int bit_index = position % 8;
    
    if (value) {
        bitmap[byte_index] |= (1 << bit_index);
    } else {
        bitmap[byte_index] &= ~(1 << bit_index);
    }
}

int RAMModel::get_bit(int position) {
    int byte_index = position / 8;
    int bit_index = position % 8;
    return (bitmap[byte_index] >> bit_index) & 1;
}

int RAMModel::allocate_block(int size) {
    int blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int consecutive_free = 0;
    int start_block = -1;
    
    for (int i = 0; i < MEMORY_SIZE / BLOCK_SIZE; i++) {
        if (get_bit(i) == 0) {
            if (consecutive_free == 0) {
                start_block = i;
            }
            consecutive_free++;
            
            if (consecutive_free >= blocks_needed) {
                for (int j = start_block; j < start_block + blocks_needed; j++) {
                    set_bit(j, 1);
                }
                
                int actual_size = blocks_needed * BLOCK_SIZE;
                int buddy_size = 1;
                while (buddy_size * 2 <= actual_size) {
                    buddy_size *= 2;
                }
                
                buddy_allocated_blocks[buddy_size].push_back({start_block * BLOCK_SIZE, actual_size});
                return start_block * BLOCK_SIZE;
            }
        } else {
            consecutive_free = 0;
            start_block = -1;
        }
    }
    
    return -1;
}

void RAMModel::free_block(int address) {
    int start_block = address / BLOCK_SIZE;
    
    if (address < 0 || address >= MEMORY_SIZE || address % BLOCK_SIZE != 0) {
        std::cout << "Error: Invalid address" << std::endl;
        return;
    }
    
    if (get_bit(start_block) == 0) {
        std::cout << "Error: Block is already free" << std::endl;
        return;
    }
    
    for (auto& [size, blocks] : buddy_allocated_blocks) {
        for (auto it = blocks.begin(); it != blocks.end(); ++it) {
            if (it->first == address) {
                blocks.erase(it);
                if (blocks.empty()) {
                    buddy_allocated_blocks.erase(size);
                }
                break;
            }
        }
    }
    
    int i = start_block;
    while (i < MEMORY_SIZE / BLOCK_SIZE && get_bit(i) == 1) {
        set_bit(i, 0);
        i++;
    }
}

void RAMModel::get_memory_info() {
    std::vector<std::pair<int, int>> free_blocks;
    std::vector<std::pair<int, int>> allocated_blocks;
    
    int i = 0;
    while (i < MEMORY_SIZE / BLOCK_SIZE) {
        if (get_bit(i) == 0) {
            int start = i * BLOCK_SIZE;
            int size = 0;
            while (i < MEMORY_SIZE / BLOCK_SIZE && get_bit(i) == 0) {
                size += BLOCK_SIZE;
                i++;
            }
            free_blocks.push_back({start, size});
        } else {
            int start = i * BLOCK_SIZE;
            int size = 0;
            while (i < MEMORY_SIZE / BLOCK_SIZE && get_bit(i) == 1) {
                size += BLOCK_SIZE;
                i++;
            }
            allocated_blocks.push_back({start, size});
        }
    }
    
    int total_free = 0;
    int total_allocated = 0;
    
    std::cout << "Free blocks: " << free_blocks.size() << std::endl;
    for (const auto& block : free_blocks) {
        std::cout << "Start: " << block.first << ", Size: " << block.second << std::endl;
        total_free += block.second;
    }
    
    std::cout << "Allocated blocks: " << allocated_blocks.size() << std::endl;
    for (const auto& block : allocated_blocks) {
        std::cout << "Start: " << block.first << ", Size: " << block.second << std::endl;
        total_allocated += block.second;
    }
    
    std::cout << "Buddy system allocation lists:" << std::endl;
    for (const auto& [size, blocks] : buddy_allocated_blocks) {
        std::cout << "Size " << size << " blocks: ";
        for (const auto& block : blocks) {
            std::cout << "[" << block.first << ", " << block.second << "] ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "Total free memory: " << total_free << std::endl;
    std::cout << "Total allocated memory: " << total_allocated << std::endl;
}
RAMModel::RAMModel() {
    std::memset(memory, 0, MEMORY_SIZE);
    std::memset(bitmap, 0, BITMAP_SIZE);
}

size_t RAMModel::allocate(size_t bytes) {
    return allocate_block(bytes);
}
void RAMModel::free(size_t address) {
    free_block(address);
}
void RAMModel::print_diagnostics() {
    get_memory_info();
}
