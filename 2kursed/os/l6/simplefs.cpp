#include "filesystem.h"

uint32_t FileSystem::allocate_inode() {
    uint32_t inode = next_inode++;
    inode_table[inode] = DirEntry();
    return inode;
}
void FileSystem::free_inode(uint32_t inode) {
    inode_table.erase(inode);
}
std::vector<uint32_t> FileSystem::read_block_chain(uint32_t first_block) {
    std::vector<uint32_t> chain;
    uint32_t current = first_block;
    while (current != 0) {
        chain.push_back(current);
        std::vector<uint8_t> block_data;
        bs.read_data({current}, block_data);
        if (block_data.size() >= sizeof(uint32_t)) {
            memcpy(&current, block_data.data() + block_size - sizeof(uint32_t), sizeof(uint32_t));
        } else current = 0;
    }
    return chain;
}
void FileSystem::write_block_chain(uint32_t first_block, const std::vector<uint32_t>& blocks) {
    for (size_t i = 0; i < blocks.size(); i++) {
        uint32_t next_block = (i + 1 < blocks.size()) ? blocks[i + 1] : 0;
        std::vector<uint8_t> block_data(block_size, 0);
        if (i == 0 && first_block != blocks[0]) {
            std::cerr << "Error: Chain mismatch" << std::endl;
            return;
        }
        memcpy(block_data.data() + block_size - sizeof(uint32_t), &next_block, sizeof(uint32_t));
        bs.write_data(block_data, {blocks[i]});
    }
}
uint32_t FileSystem::allocate_block_chain(uint32_t size) {
    std::vector<uint32_t> blocks = bs.allocate_blocks(size);
    if (blocks.empty()) return 0;
    write_block_chain(blocks[0], blocks);
    return blocks[0];
}
void FileSystem::free_block_chain(uint32_t first_block) {
    std::vector<uint32_t> blocks = read_block_chain(first_block);
    bs.free_blocks_at(blocks);
}
std::string FileSystem::normalize_path(const std::string& path) {
    if (path.empty()) return "/";
    std::string result;
    if (path[0] != '/') result = current_path + "/" + path;
    else result = path;
    std::vector<std::string> parts;
    size_t start = 0;
    while (start < result.size()) {
        size_t end = result.find('/', start);
        if (end == std::string::npos) end = result.size();
        std::string part = result.substr(start, end - start);
        if (!part.empty() && part != ".") {
            if (part == "..") { if (!parts.empty()) parts.pop_back(); }
            else parts.push_back(part);
        }
        start = end + 1;
    }
    result = "/";
    for (size_t i = 0; i < parts.size(); i++) {
        if (i > 0) result += "/";
        result += parts[i];
    }
    return result;
}
std::pair<uint32_t, std::string> FileSystem::resolve_path(const std::string& path) {
    std::string norm = normalize_path(path);
    if (norm == "/") return {root_inode, ""};
    size_t pos = norm.find_last_of('/');
    std::string dir_path = norm.substr(0, pos);
    std::string name = norm.substr(pos + 1);
    uint32_t dir_inode = root_inode;
    if (!dir_path.empty() && dir_path != "/") {
        std::vector<std::string> parts;
        size_t start = 1;
        while (start < dir_path.size()) {
            size_t end = dir_path.find('/', start);
            if (end == std::string::npos) end = dir_path.size();
            parts.push_back(dir_path.substr(start, end - start));
            start = end + 1;
        }
        dir_inode = root_inode;
        for (const auto& part : parts) {
            load_directory(dir_inode);
            bool found = false;
            for (const auto& entry : directory_cache[dir_inode]) {
                if (entry.name == part && entry.is_dir) {
                    dir_inode = entry.first_block;
                    found = true;
                    break;
                }
            }
            if (!found) return {0, ""};
        }
    }
    return {dir_inode, name};
}
void FileSystem::load_directory(uint32_t dir_inode) {
    if (directory_cache.find(dir_inode) != directory_cache.end()) return;
    std::vector<uint8_t> dir_data;
    DirEntry& dir_entry = inode_table[dir_inode];
    std::vector<uint32_t> blocks = read_block_chain(dir_entry.first_block);
    for (uint32_t block : blocks) {
        std::vector<uint8_t> block_data;
        bs.read_data({block}, block_data);
        for (size_t i = 0; i < block_data.size(); i += sizeof(uint32_t) + 256 + sizeof(uint32_t) + sizeof(uint32_t) + 1) {
            if (i + sizeof(uint32_t) + 256 + sizeof(uint32_t) + sizeof(uint32_t) + 1 > block_data.size()) break;
            uint32_t inode_num;
            memcpy(&inode_num, block_data.data() + i, sizeof(uint32_t));
            if (inode_num == 0) break;
            char name_buf[257];
            memcpy(name_buf, block_data.data() + i + sizeof(uint32_t), 256);
            name_buf[256] = '\0';
            uint32_t first_block, size;
            uint8_t is_dir;
            memcpy(&first_block, block_data.data() + i + sizeof(uint32_t) + 256, sizeof(uint32_t));
            memcpy(&size, block_data.data() + i + sizeof(uint32_t) + 256 + sizeof(uint32_t), sizeof(uint32_t));
            memcpy(&is_dir, block_data.data() + i + sizeof(uint32_t) + 256 + sizeof(uint32_t) * 2, 1);
            directory_cache[dir_inode].push_back(DirEntry(name_buf, first_block, size, is_dir != 0));
        }
    }
}
void FileSystem::save_directory(uint32_t dir_inode) {
    if (directory_cache.find(dir_inode) == directory_cache.end()) return;
    DirEntry& dir_entry = inode_table[dir_inode];
    std::vector<uint8_t> dir_data;
    for (const auto& entry : directory_cache[dir_inode]) {
        uint32_t inode_num = 0;
        for (const auto& [inum, e] : inode_table) {
            if (e.name == entry.name && e.first_block == entry.first_block && e.size == entry.size && e.is_dir == entry.is_dir) {
                inode_num = inum;
                break;
            }
        }
        dir_data.resize(dir_data.size() + sizeof(uint32_t) + 256 + sizeof(uint32_t) + sizeof(uint32_t) + 1);
        size_t offset = dir_data.size() - (sizeof(uint32_t) + 256 + sizeof(uint32_t) + sizeof(uint32_t) + 1);
        memcpy(dir_data.data() + offset, &inode_num, sizeof(uint32_t));
        memcpy(dir_data.data() + offset + sizeof(uint32_t), entry.name.c_str(), entry.name.size());
        memcpy(dir_data.data() + offset + sizeof(uint32_t) + 256, &entry.first_block, sizeof(uint32_t));
        memcpy(dir_data.data() + offset + sizeof(uint32_t) + 256 + sizeof(uint32_t), &entry.size, sizeof(uint32_t));
        uint8_t is_dir = entry.is_dir ? 1 : 0;
        memcpy(dir_data.data() + offset + sizeof(uint32_t) + 256 + sizeof(uint32_t) * 2, &is_dir, 1);
    }
    uint32_t needed_blocks = (dir_data.size() + bs.block_size - 1) / bs.block_size;
    std::vector<uint32_t> old_blocks = read_block_chain(dir_entry.first_block);
    if (old_blocks.size() != needed_blocks) {
        free_block_chain(dir_entry.first_block);
        dir_entry.first_block = allocate_block_chain(needed_blocks);
    }
    std::vector<uint32_t> blocks = read_block_chain(dir_entry.first_block);
    for (size_t i = 0; i < blocks.size(); i++) {
        size_t start = i * bs.block_size;
        size_t end = std::min(start + bs.block_size, dir_data.size());
        std::vector<uint8_t> block_data(bs.block_size, 0);
        if (start < dir_data.size()) {
            memcpy(block_data.data(), dir_data.data() + start, end - start);
        }
        uint32_t next_block = (i + 1 < blocks.size()) ? blocks[i + 1] : 0;
        memcpy(block_data.data() + bs.block_size - sizeof(uint32_t), &next_block, sizeof(uint32_t));
        bs.write_data(block_data, {blocks[i]});
    }
}
bool FileSystem::create(const std::string& fsfile, uint32_t block_size) {
    if (!bs.initialize(fsfile, block_size)) return false;
    root_inode = allocate_inode();
    inode_table[root_inode] = DirEntry("/", allocate_block_chain(1), 0, true);
    current_path = "/";
    directory_cache[root_inode].clear();
    save_directory(root_inode);
    return true;
}
bool FileSystem::mount(const std::string& fsfile) {
    if (!bs.open(fsfile)) return false;
    root_inode = allocate_inode();
    inode_table[root_inode] = DirEntry("/", 1, 0, true);
    current_path = "/";
    load_directory(root_inode);
    return true;
}
int FileSystem::open(const std::string& filename) {
    auto [dir_inode, name] = resolve_path(filename);
    if (dir_inode == 0 || name.empty()) return -1;
    load_directory(dir_inode);
    for (const auto& entry : directory_cache[dir_inode]) {
        if (entry.name == name && !entry.is_dir) {
            return entry.first_block;
        }
    }
    uint32_t inode = allocate_inode();
    uint32_t first_block = allocate_block_chain(1);
    inode_table[inode] = DirEntry(name, first_block, 0, false);
    directory_cache[dir_inode].push_back(DirEntry(name, first_block, 0, false));
    save_directory(dir_inode);
    return first_block;
}
void FileSystem::close(int fd) {
}
bool FileSystem::seek(int fd, uint32_t offset) {
    return true;
}
uint32_t FileSystem::read(int fd, std::vector<uint8_t>& buffer, uint32_t size) {
    std::vector<uint32_t> blocks = read_block_chain(fd);
    if (blocks.empty()) return 0;
    std::vector<uint8_t> all_data;
    bs.read_data(blocks, all_data);
    uint32_t read_size = std::min(size, (uint32_t)all_data.size());
    buffer.assign(all_data.begin(), all_data.begin() + read_size);
    return read_size;
}
uint32_t FileSystem::write(int fd, const std::vector<uint8_t>& buffer, uint32_t size) {
    std::vector<uint32_t> blocks = read_block_chain(fd);
    uint32_t needed_blocks = (size + bs.block_size - 1) / bs.block_size;
    if (blocks.size() != needed_blocks) {
        free_block_chain(fd);
        uint32_t new_first = allocate_block_chain(needed_blocks);
        for (auto& [inode, entry] : inode_table) {
            if (entry.first_block == fd) {
                entry.first_block = new_first;
                entry.size = size;
                break;
            }
        }
        fd = new_first;
        blocks = read_block_chain(fd);
    }
    for (size_t i = 0; i < blocks.size(); i++) {
        size_t start = i * bs.block_size;
        size_t end = std::min(start + bs.block_size, (size_t)size);
        std::vector<uint8_t> block_data(bs.block_size, 0);
        if (start < buffer.size()) {
            memcpy(block_data.data(), buffer.data() + start, end - start);
        }
        uint32_t next_block = (i + 1 < blocks.size()) ? blocks[i + 1] : 0;
        memcpy(block_data.data() + bs.block_size - sizeof(uint32_t), &next_block, sizeof(uint32_t));
        bs.write_data(block_data, {blocks[i]});
    }
    return size;
}
bool FileSystem::remove(const std::string& filename) {
    auto [dir_inode, name] = resolve_path(filename);
    if (dir_inode == 0 || name.empty()) return false;
    load_directory(dir_inode);
    for (auto it = directory_cache[dir_inode].begin(); it != directory_cache[dir_inode].end(); ++it) {
        if (it->name == name && !it->is_dir) {
            free_block_chain(it->first_block);
            for (auto& [inode, entry] : inode_table) {
                if (entry.first_block == it->first_block) {
                    free_inode(inode);
                    break;
                }
            }
            directory_cache[dir_inode].erase(it);
            save_directory(dir_inode);
            return true;
        }
    }
    return false;
}
std::vector<std::string> FileSystem::list(const std::string& dir, const std::string& pattern) {
    auto [dir_inode, name] = resolve_path(dir);
    if (dir_inode == 0) return {};
    load_directory(dir_inode);
    std::vector<std::string> result;
    std::regex pat(pattern == "*.*" ? ".*" : pattern);
    for (const auto& entry : directory_cache[dir_inode]) {
        if (std::regex_match(entry.name, pat)) {
            result.push_back(entry.name + (entry.is_dir ? "/" : ""));
        }
    }
    return result;
}
bool FileSystem::mkdir(const std::string& dirname) {
    auto [dir_inode, name] = resolve_path(dirname);
    if (dir_inode == 0 || name.empty()) return false;
    load_directory(dir_inode);
    for (const auto& entry : directory_cache[dir_inode]) {
        if (entry.name == name) return false;
    }
    uint32_t inode = allocate_inode();
    uint32_t first_block = allocate_block_chain(1);
    inode_table[inode] = DirEntry(name, first_block, 0, true);
    directory_cache[dir_inode].push_back(DirEntry(name, first_block, 0, true));
    save_directory(dir_inode);
    directory_cache[first_block].clear();
    save_directory(first_block);
    return true;
}
bool FileSystem::rmdir(const std::string& dirname) {
    auto [dir_inode, name] = resolve_path(dirname);
    if (dir_inode == 0 || name.empty()) return false;
    load_directory(dir_inode);
    for (auto it = directory_cache[dir_inode].begin(); it != directory_cache[dir_inode].end(); ++it) {
        if (it->name == name && it->is_dir) {
            load_directory(it->first_block);
            if (!directory_cache[it->first_block].empty()) return false;
            free_block_chain(it->first_block);
            for (auto& [inode, entry] : inode_table) {
                if (entry.first_block == it->first_block) {
                    free_inode(inode);
                    break;
                }
            }
            directory_cache[dir_inode].erase(it);
            save_directory(dir_inode);
            return true;
        }
    }
    return false;
}
bool FileSystem::chdir(const std::string& dir) {
    auto [dir_inode, name] = resolve_path(dir);
    if (dir_inode == 0) return false;
    load_directory(dir_inode);
    for (const auto& entry : directory_cache[dir_inode]) {
        if (entry.name == name && entry.is_dir) {
            current_path = normalize_path(dir);
            return true;
        }
    }
    return false;
}
std::string FileSystem::pwd() {
    return current_path;
}
bool FileSystem::import(const std::string& src, const std::string& dst) {
    std::ifstream src_file(src, std::ios::binary);
    if (!src_file.is_open()) return false;
    src_file.seekg(0, std::ios::end);
    uint32_t size = src_file.tellg();
    src_file.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(size);
    src_file.read(reinterpret_cast<char*>(data.data()), size);
    int fd = open(dst);
    if (fd < 0) return false;
    write(fd, data, size);
    close(fd);
    return true;
}
