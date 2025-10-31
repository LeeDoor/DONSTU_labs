#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cstring>

class BlockSpace {
private:
    std::fstream file;
    uint32_t block_size;
    uint32_t total_blocks;
    uint32_t free_blocks;
    std::vector<bool> bitmap;
    uint32_t bitmap_blocks;
    uint32_t data_start;
    bool transaction_active;
    std::unordered_map<uint32_t, std::vector<uint8_t>> transaction_cache;

    void write_bitmap() {
        file.seekp(sizeof(Header));
        for (uint32_t i = 0; i < bitmap_blocks; i++) {
            std::vector<uint8_t> block(block_size);
            uint32_t bits_in_block = block_size * 8;
            uint32_t start_bit = i * bits_in_block;
            uint32_t end_bit = std::min(start_bit + bits_in_block, total_blocks);
            
            for (uint32_t j = start_bit; j < end_bit; j++) {
                uint32_t byte_index = (j - start_bit) / 8;
                uint32_t bit_index = (j - start_bit) % 8;
                if (bitmap[j]) {
                    block[byte_index] |= (1 << bit_index);
                }
            }
            file.write(reinterpret_cast<char*>(block.data()), block_size);
        }
    }

    void read_bitmap() {
        file.seekg(sizeof(Header));
        bitmap.assign(total_blocks, false);
        
        for (uint32_t i = 0; i < bitmap_blocks; i++) {
            std::vector<uint8_t> block(block_size);
            file.read(reinterpret_cast<char*>(block.data()), block_size);
            
            uint32_t bits_in_block = block_size * 8;
            uint32_t start_bit = i * bits_in_block;
            uint32_t end_bit = std::min(start_bit + bits_in_block, total_blocks);
            
            for (uint32_t j = start_bit; j < end_bit; j++) {
                uint32_t byte_index = (j - start_bit) / 8;
                uint32_t bit_index = (j - start_bit) % 8;
                bitmap[j] = (block[byte_index] >> bit_index) & 1;
            }
        }
        
        free_blocks = 0;
        for (bool free : bitmap) {
            if (!free) free_blocks++;
        }
    }

    struct Header {
        uint32_t block_size;
        uint32_t total_blocks;
        uint32_t free_blocks;
        uint32_t bitmap_blocks;
        uint32_t data_start;
    };

public:
    BlockSpace() : block_size(0), total_blocks(0), free_blocks(0), 
                   bitmap_blocks(0), data_start(0), transaction_active(false) {}

    bool initialize(const std::string& filename, uint32_t blk_size) {
        if (blk_size < 1024 || blk_size > 65536 || (blk_size & (blk_size - 1)) != 0) {
            std::cout << "Error: Block size must be power of 2 between 1KB and 64KB" << std::endl;
            return false;
        }

        file.open(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cout << "Error: Cannot create file" << std::endl;
            return false;
        }

        block_size = blk_size;
        total_blocks = (1ULL << 16) - 1;
        
        uint64_t bitmap_size_bits = total_blocks;
        uint64_t bitmap_size_bytes = (bitmap_size_bits + 7) / 8;
        bitmap_blocks = (bitmap_size_bytes + block_size - 1) / block_size;
        
        data_start = sizeof(Header) + bitmap_blocks * block_size;
        
        bitmap.assign(total_blocks, false);
        free_blocks = total_blocks;

        Header header;
        header.block_size = block_size;
        header.total_blocks = total_blocks;
        header.free_blocks = free_blocks;
        header.bitmap_blocks = bitmap_blocks;
        header.data_start = data_start;

        file.write(reinterpret_cast<char*>(&header), sizeof(Header));
        write_bitmap();

        std::vector<uint8_t> empty_block(block_size, 0);
        for (uint32_t i = 0; i < total_blocks; i++) {
            uint64_t offset = data_start + static_cast<uint64_t>(i) * block_size;
            file.seekp(offset);
            file.write(reinterpret_cast<char*>(empty_block.data()), block_size);
        }

        return true;
    }

    bool open(const std::string& filename) {
        file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (!file.is_open()) {
            std::cout << "Error: Cannot open file" << std::endl;
            return false;
        }

        Header header;
        file.read(reinterpret_cast<char*>(&header), sizeof(Header));
        
        block_size = header.block_size;
        total_blocks = header.total_blocks;
        free_blocks = header.free_blocks;
        bitmap_blocks = header.bitmap_blocks;
        data_start = header.data_start;

        read_bitmap();
        return true;
    }

    std::vector<uint32_t> allocate_blocks(uint32_t count, const std::vector<uint32_t>& preferred = {}) {
        std::vector<uint32_t> allocated;
        
        if (count > free_blocks) {
            std::cout << "Error: Not enough free blocks" << std::endl;
            return allocated;
        }

        for (uint32_t block : preferred) {
            if (block < total_blocks && !bitmap[block] && allocated.size() < count) {
                bitmap[block] = true;
                allocated.push_back(block);
            }
        }

        for (uint32_t i = 0; i < total_blocks && allocated.size() < count; i++) {
            if (!bitmap[i]) {
                bitmap[i] = true;
                allocated.push_back(i);
            }
        }

        free_blocks -= allocated.size();
        
        if (!transaction_active) {
            write_bitmap();
            update_header();
        }
        
        return allocated;
    }
    std::vector<uint32_t> buffer;
    void cut_file(const std::vector<uint32_t>& blocks) {
        buffer.clear();
        buffer = blocks;
    }
    void paste_file(const std::vector<uint32_t>& destination) {
        if(buffer.empty()) {
            std::cout << "not cut blocks yet\n";
            return;
        }
        if(buffer.size() > destination.size()) {
            std::cout << "Destination is too small\n";
            return;
        }

        std::vector<uint8_t> readed_data;
        read_data(buffer, readed_data);
        write_data(readed_data, buffer);
        free_blocks_at(buffer);
        buffer.clear();
    }
    void free_blocks_at(const std::vector<uint32_t>& blocks) {
        for (uint32_t block : blocks) {
            if (block < total_blocks && bitmap[block]) {
                bitmap[block] = false;
                free_blocks++;
            }
        }
        
        if (!transaction_active) {
            write_bitmap();
            update_header();
        }
    }

    std::vector<uint32_t> write_data(const std::vector<uint8_t>& data, const std::vector<uint32_t>& blocks = {}) {
        std::vector<uint32_t> target_blocks = blocks;
        
        if (target_blocks.empty()) {
            uint32_t blocks_needed = (data.size() + block_size - 1) / block_size;
            target_blocks = allocate_blocks(blocks_needed);
        }
        
        if (target_blocks.size() * block_size < data.size()) {
            std::cout << "Error: Not enough blocks for data" << std::endl;
            return {};
        }

        for (size_t i = 0; i < target_blocks.size(); i++) {
            uint32_t block = target_blocks[i];
            uint64_t offset = data_start + static_cast<uint64_t>(block) * block_size;
            
            const uint8_t* block_data = data.data() + i * block_size;
            size_t block_data_size = std::min(block_size, static_cast<uint32_t>(data.size() - i * block_size));
            
            if (transaction_active) {
                std::vector<uint8_t> cached_data(block_data, block_data + block_data_size);
                transaction_cache[block] = cached_data;
            } else {
                file.seekp(offset);
                file.write(reinterpret_cast<const char*>(block_data), block_data_size);
            }
        }
        
        return target_blocks;
    }

    void read_data(const std::vector<uint32_t>& blocks, std::vector<uint8_t>& output) {
        output.clear();
        output.reserve(blocks.size() * block_size);
        
        for (uint32_t block : blocks) {
            if (block >= total_blocks || !bitmap[block]) {
                std::cout << "Error: Invalid block number" << std::endl;
                return;
            }
            
            std::vector<uint8_t> block_data(block_size);
            uint64_t offset = data_start + static_cast<uint64_t>(block) * block_size;
            
            if (transaction_active && transaction_cache.find(block) != transaction_cache.end()) {
                block_data = transaction_cache[block];
            } else {
                file.seekg(offset);
                file.read(reinterpret_cast<char*>(block_data.data()), block_size);
            }
            
            output.insert(output.end(), block_data.begin(), block_data.end());
        }
    }

    void get_info(uint32_t& blk_size, uint32_t& total, uint32_t& free, 
                  uint32_t& metadata_size, uint32_t& cache_size) {
        blk_size = block_size;
        total = total_blocks;
        free = free_blocks;
        metadata_size = data_start;
        cache_size = transaction_cache.size() * block_size;
    }

    void begin_transaction() {
        transaction_active = true;
        transaction_cache.clear();
    }

    void commit_transaction() {
        if (!transaction_active) return;
        
        write_bitmap();
        update_header();
        
        for (const auto& [block, data] : transaction_cache) {
            uint64_t offset = data_start + static_cast<uint64_t>(block) * block_size;
            file.seekp(offset);
            file.write(reinterpret_cast<const char*>(data.data()), data.size());
        }
        
        transaction_cache.clear();
        transaction_active = false;
    }

    void rollback_transaction() {
        if (!transaction_active) return;
        
        read_bitmap();
        transaction_cache.clear();
        transaction_active = false;
    }

private:
    void update_header() {
        Header header;
        header.block_size = block_size;
        header.total_blocks = total_blocks;
        header.free_blocks = free_blocks;
        header.bitmap_blocks = bitmap_blocks;
        header.data_start = data_start;
        
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&header), sizeof(Header));
    }
};

int main() {
    BlockSpace bs;
    
    if (!bs.initialize("test_blocks.bin", 4096)) {
        return 1;
    }
    
    std::cout << "Block space initialized successfully" << std::endl;
    
    uint32_t blk_size, total, free, metadata, cache;
    bs.get_info(blk_size, total, free, metadata, cache);
    
    std::cout << "Block size: " << blk_size << std::endl;
    std::cout << "Total blocks: " << total << std::endl;
    std::cout << "Free blocks: " << free << std::endl;
    std::cout << "Metadata size: " << metadata << std::endl;
    
    std::string test_data = "Hello, this is a test data for block space implementation!";
    std::vector<uint8_t> data(test_data.begin(), test_data.end());
    
    bs.begin_transaction();
    
    auto blocks = bs.write_data(data);
    std::cout << "Data written to blocks: ";
    for (auto block : blocks) {
        std::cout << block << "'th, ";
    }
    std::cout << std::endl;
    
    bs.get_info(blk_size, total, free, metadata, cache);
    std::cout << "Free blocks after allocation: " << free << std::endl;
    std::cout << "Cache size during transaction: " << cache << std::endl;
    
    std::vector<uint8_t> read_data;
    bs.read_data(blocks, read_data);
    
    std::string read_string(read_data.begin(), read_data.end());
    std::cout << "Read data: " << read_string << std::endl;
    
    bs.commit_transaction();
    
    bs.free_blocks_at(blocks);
    bs.get_info(blk_size, total, free, metadata, cache);
    std::cout << "Free blocks after freeing: " << free << std::endl;


    bs.cut_file({0, 1, 2});
    bs.paste_file({1, 2, 3});
    
    return 0;
}
