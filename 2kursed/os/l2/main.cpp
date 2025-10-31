#include <iostream>
#include <cstring>

unsigned char visits[32];
unsigned char RAM[1024];
unsigned char DISK[1024];

void init() {
    memset(visits, 0, 32);
    memset(RAM, 0, 1024);
    memset(DISK, 0, 1024);
}

int get_page_number(int address) {
    return address / 32;
}

int get_offset(int address) {
    return address % 32;
}

int find_page_in_ram(int page) {
    for (int i = 0; i < 32; i++) {
        if (RAM[i * 32] == page) {
            return i;
        }
    }
    return -1;
}

int find_free_ram_slot() {
    for (int i = 0; i < 32; i++) {
        if (RAM[i * 32] == 0) {
            return i;
        }
    }
    return -1;
}

void update_nfu_counter(int ram_slot) {
    int counter_addr = ram_slot * 32 + 1;
    unsigned char counter = RAM[counter_addr];
    counter = (counter >> 1) | (RAM[ram_slot * 32 + 2] << 7);
    RAM[counter_addr] = counter;
    RAM[ram_slot * 32 + 2] = 0;
}

int find_nfu_victim() {
    int victim = 0;
    /*unsigned char min_counter = 255;
    
    for (int i = 0; i < 32; i++) {
        if (RAM[i * 32] != 0) {
            unsigned char counter = RAM[i * 32 + 1];
            if (counter < min_counter) {
                min_counter = counter;
                victim = i;
            }
        }
    }*/
    for(int i = 1; i < 32; ++i) {
        if(visits[victim] > visits[i]) victim = i;
    }
    return victim;
}

void swap_page(int victim_ram_slot, int new_page) {
    if (victim_ram_slot == -1) return;
    
    int old_page = RAM[victim_ram_slot * 32];
    if (old_page != 0) {
        memcpy(&DISK[old_page * 32], &RAM[victim_ram_slot * 32 + 3], 29);
    }
    
    RAM[victim_ram_slot * 32] = new_page;
    RAM[victim_ram_slot * 32 + 1] = 0;
    RAM[victim_ram_slot * 32 + 2] = 0;
    memcpy(&RAM[victim_ram_slot * 32 + 3], &DISK[new_page * 32], 29);
}

void handle_page_fault(int page) {
    int free_slot = find_free_ram_slot();
    if (free_slot != -1) {
        swap_page(free_slot, page);
        std::cout << "Page " << page << " loaded into RAM at slot " << free_slot << std::endl;
    } else {
        int victim = find_nfu_victim();
        if (victim != -1) {
            std::cout << "Evicting page " << RAM[victim * 32] << " from slot " << victim << std::endl;
            swap_page(victim, page);
            std::cout << "Page " << page << " loaded into RAM at slot " << victim << std::endl;
        }
    }
}

void read_memory(int address) {
    int page = get_page_number(address);
    int offset = get_offset(address);
    
    int ram_slot = find_page_in_ram(page);
    if (ram_slot == -1) {
        std::cout << "Page fault on read!" << std::endl;
        handle_page_fault(page);
        ram_slot = find_page_in_ram(page);
    }
    
    if (ram_slot != -1) {
        ++visits[page];
        RAM[ram_slot * 32 + 2] = 1;
        unsigned char value = RAM[ram_slot * 32 + 3 + offset];
        std::cout << "Value at address " << address << ": " << (int)value << std::endl;
    }
}

void write_memory(int address, unsigned char value) {
    int page = get_page_number(address);
    int offset = get_offset(address);
    
    int ram_slot = find_page_in_ram(page);
    if (ram_slot == -1) {
        std::cout << "Page fault on write!" << std::endl;
        handle_page_fault(page);
        ram_slot = find_page_in_ram(page);
    }
    
    if (ram_slot != -1) {
        ++visits[page];
        RAM[ram_slot * 32 + 2] = 1;
        RAM[ram_slot * 32 + 3 + offset] = value;
        std::cout << "Written value " << (int)value << " at address " << address << std::endl;
    }
}

void display_memory_map() {
    std::cout << "Memory page map:" << std::endl;
    std::cout << "RAM:" << std::endl;
    for (int i = 0; i < 32; i++) {
        if (RAM[i * 32] != 0) {
            std::cout << "Slot " << i << ": Page " << (int)RAM[i * 32] << std::endl;
        }
    }
    
    std::cout << "External storage:" << std::endl;
    for (int i = 1; i < 32; i++) {
        bool in_ram = false;
        for (int j = 0; j < 32; j++) {
            if (RAM[j * 32] == i) {
                in_ram = true;
                break;
            }
        }
        if (!in_ram) {
            std::cout << "Page " << i << std::endl;
        }
    }
}

void timer_interrupt() {
    for (int i = 0; i < 32; i++) {
        if (RAM[i * 32] != 0) {
            update_nfu_counter(i);
        }
    }
}

int main() {
    init();
    
    while (true) {
        std::cout << "\nMenu:\n";
        std::cout << "1. Read memory cell\n";
        std::cout << "2. Write to memory cell\n";
        std::cout << "3. Display memory map\n";
        std::cout << "4. Timer interrupt\n";
        std::cout << "5. Exit\n";
        std::cout << "Choose action: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                int address;
                std::cout << "Enter address (0-1023): ";
                std::cin >> address;
                if (address >= 0 && address < 1024) {
                    read_memory(address);
                } else {
                    std::cout << "Invalid address!" << std::endl;
                }
                break;
            }
            case 2: {
                int address;
                unsigned int value;
                std::cout << "Enter address (0-1023): ";
                std::cin >> address;
                std::cout << "Enter value (0-255): ";
                std::cin >> value;
                if (address >= 0 && address < 1024) {
                    write_memory(address, value);
                } else {
                    std::cout << "Invalid address!" << std::endl;
                }
                break;
            }
            case 3:
                display_memory_map();
                break;
            case 4:
                timer_interrupt();
                std::cout << "Timer interrupt processed" << std::endl;
                break;
            case 5:
                return 0;
            default:
                std::cout << "Invalid choice!" << std::endl;
                break;
        }
    }
    
    return 0;
}