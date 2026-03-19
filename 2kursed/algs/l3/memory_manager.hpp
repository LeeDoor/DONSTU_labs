#pragma once

#include <cstddef>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <type_traits>

template<typename T> class MemoryPointer;

template <typename T>
concept MemoryStorable = std::is_trivially_copyable_v<T>;

class MemoryManager {
public:
    explicit MemoryManager(std::size_t segment_size_in_bytes);

    template <MemoryStorable T>
    MemoryPointer<T> new_pointer(std::size_t element_count) {
        if (element_count == 0) {
            throw std::invalid_argument("Size must be positive");
        }

        const std::size_t required_bytes = sizeof(T) * element_count;
        if (required_bytes > segment_size_in_bytes_) {
            throw std::runtime_error("Not enough memory");
        }

        for (std::size_t i = 0; i < 2; ++i) {
            if (!segments_[i].occupied) {
                segments_[i].occupied = true;
                return MemoryPointer<T>(this, i, element_count);
            }
        }

        throw std::runtime_error("No free segment");
    }

private:
    template <typename T>
    friend class MemoryPointer;

    struct Segment {
        std::unique_ptr<std::byte[]> data;
        bool occupied = false;
    };

    template <MemoryStorable T>
    T read(std::size_t segment_index, std::size_t position) const {
        const std::size_t offset = position * sizeof(T);
        if (offset + sizeof(T) > segment_size_in_bytes_) {
            throw std::out_of_range("Read out of range");
        }

        T value{};
        std::memcpy(&value, segments_[segment_index].data.get() + offset, sizeof(T));
        return value;
    }

    template <MemoryStorable T>
    void write(std::size_t segment_index, std::size_t position, const T& value) {
        const std::size_t offset = position * sizeof(T);
        if (offset + sizeof(T) > segment_size_in_bytes_) {
            throw std::out_of_range("Write out of range");
        }

        std::memcpy(segments_[segment_index].data.get() + offset, &value, sizeof(T));
    }

    void free_segment(std::size_t segment_index);

    Segment segments_[2];
    std::size_t segment_size_in_bytes_ = 0;
};
