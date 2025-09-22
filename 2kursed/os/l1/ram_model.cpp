#include "ram_model.hpp"
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define MEMORY_SIZE 1024
#define MIN_BLOCK_SIZE 64  // Минимальный размер блока для битовой карты

// Глобальный массив памяти (1024 байта + дополнительное пространство для битовой карты)
uint8_t memory[MEMORY_SIZE + (MEMORY_SIZE / MIN_BLOCK_SIZE + 7) / 8];

// Структура для хранения информации о блоке в двоичном разбиении
typedef struct block_info {
    uint16_t size;         // Размер блока
    uint16_t is_free;      // Свободен ли блок (1 - свободен, 0 - занят)
    uint16_t process_id;   // ID процесса (0 для свободных блоков)
    struct block_info* next; // Указатель на следующий блок в списке
} block_info;

// Функции для работы с битовой картой
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

// Функции для двоичного разбиения
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
    // Инициализация памяти нулями
    memset(memory, 0, sizeof(memory));
    
    // Инициализация битовой карты (все блоки свободны)
    uint16_t bitmap_size = (MEMORY_SIZE / MIN_BLOCK_SIZE + 7) / 8;
    for (uint16_t i = 0; i < bitmap_size; i++) {
        memory[MEMORY_SIZE + i] = 0; // Все биты установлены в 0 (свободно)
    }
    
    // Инициализация первого блока для двоичного разбиения
    block_info* first_block = (block_info*)memory;
    first_block->size = MEMORY_SIZE - sizeof(block_info);
    first_block->is_free = 1;
    first_block->process_id = 0;
    first_block->next = NULL;
}

// Функция выделения памяти с использованием двоичного разбиения
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
            // Найден подходящий блок
            if (current->size >= block_size) {
                // Делим блок пока не достигнем нужного размера
                while (current->size >= block_size * 2 && 
                       block_size * 2 <= MEMORY_SIZE - current_addr - sizeof(block_info)) {
                    // Создаем новый блок-близнец
                    block_info* buddy = (block_info*)((uint8_t*)current + block_size);
                    buddy->size = current->size - block_size - sizeof(block_info);
                    buddy->is_free = 1;
                    buddy->process_id = 0;
                    buddy->next = current->next;
                    
                    current->size = block_size - sizeof(block_info);
                    current->next = buddy;
                    
                    block_size *= 2;
                }
                
                // Выделяем блок
                current->is_free = 0;
                current->process_id = process_id;
                
                // Обновляем битовую карту
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

// Функция освобождения памяти
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
    
    // Освобождаем блок
    block->is_free = 1;
    uint16_t process_id = block->process_id;
    block->process_id = 0;
    
    // Обновляем битовую карту
    uint16_t start_block = header_addr / MIN_BLOCK_SIZE;
    uint16_t end_block = (header_addr + sizeof(block_info) + block->size - 1) / MIN_BLOCK_SIZE;
    
    for (uint16_t i = start_block; i <= end_block; i++) {
        set_bit(i, 0);
    }
    
    // Попытка объединения с ближайшими блоками
    block_info* current = (block_info*)memory;
    while (current != NULL) {
        if (current->is_free && current->next != NULL && current->next->is_free) {
            // Проверяем, являются ли блоки близнецами
            uint16_t current_addr = (uint16_t)((uint8_t*)current - memory);
            uint16_t buddy_addr = current_addr + current->size + sizeof(block_info);
            
            block_info* buddy = (block_info*)(memory + buddy_addr);
            
            if ((uint8_t*)buddy == (uint8_t*)current->next && buddy->is_free) {
                // Объединяем блоки
                current->size += buddy->size + sizeof(block_info);
                current->next = buddy->next;
                
                // Очищаем информацию о buddy
                memset(buddy, 0, sizeof(block_info));
            }
        }
        current = current->next;
    }
    
    printf("Блок по адресу %u освобожден (процесс %u)\n", address, process_id);
}

// Функция для получения информации о памяти
void get_memory_info() {
    printf("\n=== ИНФОРМАЦИЯ О ПАМЯТИ ===\n");
    
    // Информация о блоках двоичного разбиения
    printf("Двоичное разбиение:\n");
    block_info* current = (block_info*)memory;
    uint16_t total_used = 0;
    uint16_t total_free = 0;
    uint16_t used_blocks = 0;
    uint16_t free_blocks = 0;
    
    while (current != NULL) {
        uint16_t addr = (uint16_t)((uint8_t*)current - memory);
        if (current->is_free) {
            printf("  Свободный блок: адрес=%u, размер=%u байт\n", 
                   addr + sizeof(block_info), current->size);
            total_free += current->size;
            free_blocks++;
        } else {
            printf("  Занятый блок: адрес=%u, размер=%u байт, процесс=%u\n", 
                   addr + sizeof(block_info), current->size, current->process_id);
            total_used += current->size;
            used_blocks++;
        }
        current = current->next;
    }
    
    printf("\nСтатистика двоичного разбиения:\n");
    printf("  Занятых блоков: %u\n", used_blocks);
    printf("  Свободных блоков: %u\n", free_blocks);
    printf("  Всего занято: %u байт\n", total_used);
    printf("  Всего свободно: %u байт\n", total_free);
    printf("  Общий размер: %u байт\n", MEMORY_SIZE);
    
    // Информация о битовой карте
    printf("\nБитовая карта (блоки по %d байт):\n", MIN_BLOCK_SIZE);
    uint16_t bitmap_blocks = MEMORY_SIZE / MIN_BLOCK_SIZE;
    uint16_t bitmap_used = 0;
    uint16_t bitmap_free = 0;
    
    printf("  Состояние: ");
    for (uint16_t i = 0; i < bitmap_blocks; i++) {
        if (get_bit(i)) {
            printf("1");
            bitmap_used++;
        } else {
            printf("0");
            bitmap_free++;
        }
    }
    printf("\n");
    
    printf("\nСтатистика битовой карты:\n");
    printf("  Занято блоков: %u\n", bitmap_used);
    printf("  Свободно блоков: %u\n", bitmap_free);
    printf("  Занято памяти: %u байт\n", bitmap_used * MIN_BLOCK_SIZE);
    printf("  Свободно памяти: %u байт\n", bitmap_free * MIN_BLOCK_SIZE);
    printf("==============================\n\n");
}

