#include "memory_manager.hpp"

MemoryManager::MemoryManager(std::size_t segment_size_in_bytes)
    : segment_size_in_bytes_(segment_size_in_bytes) {
    if (segment_size_in_bytes_ == 0) {
        throw std::invalid_argument("Segment size must be positive");
    }

    segments_[0].data = std::make_unique<std::byte[]>(segment_size_in_bytes_);
    segments_[0].occupied = false;

    segments_[1].data = std::make_unique<std::byte[]>(segment_size_in_bytes_);
    segments_[1].occupied = false;
}

void MemoryManager::free_segment(std::size_t segment_index) {
    if (segment_index < 2) {
        segments_[segment_index].occupied = false;
    }
}
