#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "ram_model.hpp"

#define MEMORY_SIZE 1024
#define MIN_BLOCK_SIZE 64  

uint8_t memory[MEMORY_SIZE + (MEMORY_SIZE / MIN_BLOCK_SIZE + 7) / 8];

typedef struct block_info {
    uint16_t size;         
    uint16_t is_free;     
    uint16_t process_id; 
    struct block_info* next; 
} block_info;

void set_bit(uint16_t position, int value) {
    uint16_t byte_index = position / 8;
    uint16_t bit_index = position % 8;
    
    if (value) {
        memory[MEMORY_SIZE + byte_index] |= (1 << bit_index);
    } else {
        memory[MEMORY_SIZE + byte_index] &= ~(1 << bit_index);
    }
}

int get_bit(uint16_t position) {
    uint16_t byte_index = position / 8;
    uint16_t bit_index = position % 8;
    return (memory[MEMORY_SIZE + byte_index] >> bit_index) & 1;
}

uint16_t find_nearest_power_of_two(uint16_t size) {
    uint16_t power = 1;
    while (power < size) {
        power <<= 1;
    }
    return power > MEMORY_SIZE ? MEMORY_SIZE : power;
}

block_info* get_block_header(uint16_t address) {
    return (block_info*)&memory[address];
}

void initialize_memory() {
    memset(memory, 0, sizeof(memory));
    
    uint16_t bitmap_size = (MEMORY_SIZE / MIN_BLOCK_SIZE + 7) / 8;
    for (uint16_t i = 0; i < bitmap_size; i++) {
        memory[MEMORY_SIZE + i] = 0; 
    }
    
    block_info* first_block = (block_info*)memory;
    first_block->size = MEMORY_SIZE - sizeof(block_info);
    first_block->is_free = 1;
    first_block->process_id = 0;
    first_block->next = NULL;
}

uint16_t allocate_binary_buddy(uint16_t size, uint16_t process_id) {
    if (size == 0 || size > MEMORY_SIZE - sizeof(block_info)) {
        printf("Ошибка: недопустимый размер %u\n", size);
        return 0;
    }
    
    uint16_t required_size = size + sizeof(block_info);
    uint16_t block_size = find_nearest_power_of_two(required_size);
    
    block_info* current = (block_info*)memory;
    block_info* prev = NULL;
    
    while (current != NULL) {
        uint16_t current_addr = (uint16_t)((uint8_t*)current - memory);
        
        if (current->is_free && current->size >= size) {
            if (current->size >= block_size) {
                while (current->size >= block_size * 2 && 
                       block_size * 2 <= MEMORY_SIZE - current_addr - sizeof(block_info)) {
                    block_info* buddy = (block_info*)((uint8_t*)current + block_size);
                    buddy->size = current->size - block_size - sizeof(block_info);
                    buddy->is_free = 1;
                    buddy->process_id = 0;
                    buddy->next = current->next;
                    
                    current->size = block_size - sizeof(block_info);
                    current->next = buddy;
                    
                    block_size *= 2;
                }
                
                current->is_free = 0;
                current->process_id = process_id;
                
                uint16_t start_block = current_addr / MIN_BLOCK_SIZE;
                uint16_t end_block = (current_addr + sizeof(block_info) + size - 1) / MIN_BLOCK_SIZE;
                
                for (uint16_t i = start_block; i <= end_block; i++) {
                    set_bit(i, 1);
                }
                
                return current_addr + sizeof(block_info);
            }
        }
        
        prev = current;
        current = current->next;
    }
    
    printf("Ошибка: недостаточно памяти для выделения %u байт\n", size);
    return 0;
}

void free_binary_buddy(uint16_t address) {
    if (address < sizeof(block_info) || address >= MEMORY_SIZE) {
        printf("Ошибка: недопустимый адрес %u\n", address);
        return;
    }
    
    uint16_t header_addr = address - sizeof(block_info);
    block_info* block = get_block_header(header_addr);
    
    if (block->is_free) {
        printf("Ошибка: блок по адресу %u уже свободен\n", address);
        return;
    }
    
    block->is_free = 1;
    uint16_t process_id = block->process_id;
    block->process_id = 0;
    
    uint16_t start_block = header_addr / MIN_BLOCK_SIZE;
    uint16_t end_block = (header_addr + sizeof(block_info) + block->size - 1) / MIN_BLOCK_SIZE;
    
    for (uint16_t i = start_block; i <= end_block; i++) {
        set_bit(i, 0);
    }
    
    block_info* current = (block_info*)memory;
    while (current != NULL) {
        if (current->is_free && current->next != NULL && current->next->is_free) {
            uint16_t current_addr = (uint16_t)((uint8_t*)current - memory);
            uint16_t buddy_addr = current_addr + current->size + sizeof(block_info);
            
            block_info* buddy = (block_info*)(memory + buddy_addr);
            
            if ((uint8_t*)buddy == (uint8_t*)current->next && buddy->is_free) {
                current->size += buddy->size + sizeof(block_info);
                current->next = buddy->next;
                
                memset(buddy, 0, sizeof(block_info));
            }
        }
        current = current->next;
    }
    
    printf("Блок по адресу %u освобожден (процесс %u)\n", address, process_id);
}

void get_memory_info() {
    printf("Buddy system allocation lists:\n");
    block_info* current = (block_info*)memory;
    uint16_t total_used = 0;
    uint16_t total_free = 0;
    uint16_t used_blocks = 0;
    uint16_t free_blocks = 0;
    
    while (current != NULL) {
        uint16_t addr = (uint16_t)((uint8_t*)current - memory);
        if (current->is_free) {
            printf("Free block address=%u size=%u\n", 
                   addr + sizeof(block_info), current->size);
            total_free += current->size;
            free_blocks++;
        } else {
            printf("Free block address=%u size=%u\n", 
                   addr + sizeof(block_info), current->size, current->process_id);
            total_used += current->size;
            used_blocks++;
        }
        current = current->next;
    }
    
    printf("Total free memory: %u\n", total_free);
    printf("Total allocated memory: %u\n", total_used);
}
RAMModel::RAMModel() {
    initialize_memory();
}
size_t RAMModel::allocate(size_t bytes) {
    static int id = 0;
    return allocate_binary_buddy(bytes, id++);
}
void RAMModel::free(size_t address) {
    free_binary_buddy(address);
}
void RAMModel::print_diagnostics() {
    get_memory_info(); 
}
