#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cstring>
#include <list>
#include <map>
#include <regex>

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
    void write_bitmap();
    void read_bitmap();
    struct Header {
        uint32_t block_size;
        uint32_t total_blocks;
        uint32_t free_blocks;
        uint32_t bitmap_blocks;
        uint32_t data_start;
    };
    void update_header();
public:
    BlockSpace() : block_size(0), total_blocks(0), free_blocks(0), 
                   bitmap_blocks(0), data_start(0), transaction_active(false) {}
    bool initialize(const std::string& filename, uint32_t blk_size);
    bool open(const std::string& filename);
    std::vector<uint32_t> allocate_blocks(uint32_t count, const std::vector<uint32_t>& preferred = {});
    void free_blocks_at(const std::vector<uint32_t>& blocks);
    std::vector<uint32_t> write_data(const std::vector<uint8_t>& data, const std::vector<uint32_t>& blocks = {});
    void read_data(const std::vector<uint32_t>& blocks, std::vector<uint8_t>& output);
    void get_info(uint32_t& blk_size, uint32_t& total, uint32_t& free, 
                  uint32_t& metadata_size, uint32_t& cache_size);
    void begin_transaction();
    void commit_transaction();
    void rollback_transaction();
};

struct DirEntry {
    std::string name;
    uint32_t first_block;
    uint32_t size;
    bool is_dir;
    DirEntry() : first_block(0), size(0), is_dir(false) {}
    DirEntry(const std::string& n, uint32_t fb, uint32_t sz, bool dir) : 
             name(n), first_block(fb), size(sz), is_dir(dir) {}
};

class FileSystem {
private:
    BlockSpace bs;
    std::string current_path;
    std::unordered_map<uint32_t, DirEntry> inode_table;
    uint32_t next_inode;
    uint32_t root_inode;
    std::map<uint32_t, std::vector<DirEntry>> directory_cache;
    uint32_t allocate_inode();
    void free_inode(uint32_t inode);
    std::vector<uint32_t> read_block_chain(uint32_t first_block, uint32_t block_size);
    void write_block_chain(uint32_t first_block, uint32_t block_size, const std::vector<uint32_t>& blocks);
    uint32_t allocate_block_chain(uint32_t size);
    void free_block_chain(uint32_t first_block);
    std::string normalize_path(const std::string& path);
    std::pair<uint32_t, std::string> resolve_path(const std::string& path);
    void load_directory(uint32_t dir_inode);
    void save_directory(uint32_t dir_inode);
public:
    FileSystem() : next_inode(1), root_inode(0) {}
    bool create(const std::string& fsfile, uint32_t block_size = 4096);
    bool mount(const std::string& fsfile);
    int open(const std::string& filename);
    void close(int fd);
    bool seek(int fd, uint32_t offset);
    uint32_t read(int fd, std::vector<uint8_t>& buffer, uint32_t size);
    uint32_t write(int fd, const std::vector<uint8_t>& buffer, uint32_t size);
    bool remove(const std::string& filename);
    std::vector<std::string> list(const std::string& dir = ".", const std::string& pattern = "*.*");
    bool mkdir(const std::string& dirname);
    bool rmdir(const std::string& dirname);
    bool chdir(const std::string& dir);
    std::string pwd();
    bool import(const std::string& src, const std::string& dst);
};
