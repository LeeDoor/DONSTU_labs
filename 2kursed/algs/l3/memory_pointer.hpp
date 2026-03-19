#pragma once
#include "memory_manager.hpp"
#include <cstddef>
#include <stdexcept>

class MemoryManager;

template <typename T>
class MemoryPointer {
public:
    MemoryPointer() = default;

    MemoryPointer(MemoryManager* manager, std::size_t segment_index, std::size_t element_count)
        : manager_(manager), segment_index_(segment_index), element_count_(element_count), position_(0), valid_(true) {}

    MemoryPointer(const MemoryPointer&) = delete;
    MemoryPointer& operator=(const MemoryPointer&) = delete;

    MemoryPointer(MemoryPointer&& other) noexcept {
        move_from(static_cast<MemoryPointer&&>(other));
    }

    MemoryPointer& operator=(MemoryPointer&& other) noexcept {
        if (this != &other) {
            release();
            move_from(static_cast<MemoryPointer&&>(other));
        }
        return *this;
    }

    ~MemoryPointer() {
        release();
    }

    T read() const {
        ensure_valid();
        return manager_->read<T>(segment_index_, position_);
    }

    void write(const T& value) {
        ensure_valid();
        manager_->template write<T>(segment_index_, position_, value);
    }

    bool next() {
        ensure_valid();
        if (position_ + 1 >= element_count_) {
            return false;
        }
        ++position_;
        return true;
    }

    bool prev() {
        ensure_valid();
        if (position_ == 0) {
            return false;
        }
        --position_;
        return true;
    }

    void reset() {
        ensure_valid();
        position_ = 0;
    }

    std::size_t size() const {
        ensure_valid();
        return element_count_;
    }

private:
    void ensure_valid() const {
        if (!valid_) {
            throw std::runtime_error("Invalid pointer");
        }
    }

    void release() {
        if (valid_ && manager_ != nullptr) {
            manager_->free_segment(segment_index_);
        }

        manager_ = nullptr;
        segment_index_ = 0;
        element_count_ = 0;
        position_ = 0;
        valid_ = false;
    }

    void move_from(MemoryPointer&& other) noexcept {
        manager_ = other.manager_;
        segment_index_ = other.segment_index_;
        element_count_ = other.element_count_;
        position_ = other.position_;
        valid_ = other.valid_;

        other.manager_ = nullptr;
        other.segment_index_ = 0;
        other.element_count_ = 0;
        other.position_ = 0;
        other.valid_ = false;
    }

    MemoryManager* manager_ = nullptr;
    std::size_t segment_index_ = 0;
    std::size_t element_count_ = 0;
    std::size_t position_ = 0;
    bool valid_ = false;
};
