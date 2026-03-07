#include "d_array.hpp"

DArray::DArray(size_t size) 
    : data_size_{ size }
    , data_{ new ByteType[data_size_] }
    , taken_size_{ size / (sizeof(Bitmap) * 8) + (size % 8 != 0) }
    , taken_{ new Bitmap[taken_size_] }
{
    for(int i = 0; i < data_size_; ++i) { data_[i] = 0x0; }
    for(int i = 0; i < taken_size_; ++i) { 
        taken_[i] = 0x0;
    }
}
DArray::DArray() : DArray(0) {}
DArray::DArray(const DArray& other)
    : data_size_{ other.data_size_ }
    , data_{ nullptr }
    , taken_size_{ other.taken_size_ }
    , taken_{ nullptr }
{
    ByteType* data = other.data_ ? new ByteType[data_size_] : nullptr;
    Bitmap* taken = other.taken_ ? new Bitmap[taken_size_] : nullptr;
    if(data)
        std::memcpy(data, other.data_, other.data_size_);
    if(taken)
        std::memcpy(taken, other.taken_, other.taken_size_);
    data_ = data;
    taken_ = taken;
}
DArray::DArray(DArray&& other)
    : data_size_{ other.data_size_ }
    , data_{ other.data_ }
    , taken_size_{ other.taken_size_ }
    , taken_{ other.taken_ }
{
    other.data_ = nullptr;
    other.data_size_ = 0;
    other.taken_ = nullptr;
    other.taken_size_ = 0;
}
DArray& DArray::operator=(const DArray& other) {
    if(this == &other) return *this;
    ByteType* data = other.data_ ? new ByteType[other.data_size_] : nullptr;
    Bitmap* taken = other.taken_ ? new Bitmap[other.taken_size_] : nullptr;
    if(data)
        std::memcpy(data, other.data_, other.data_size_);
    if(taken)
        std::memcpy(taken, other.taken_, other.taken_size_);

    delete[] data_; delete[] taken_; 
    data_ = data;
    data_size_ = other.data_size_;
    taken_ = taken;
    taken_size_ = other.taken_size_;
    return *this;
}
DArray& DArray::operator=(DArray&& other) {
    if(this == &other) return *this;
    delete[] data_; delete[] taken_;
    data_ = other.data_;
    data_size_ = other.data_size_;
    taken_ = other.taken_;
    taken_size_ = other.taken_size_;
    other.data_ = nullptr;
    other.data_size_ = 0;
    other.taken_ = nullptr;
    other.taken_size_ = 0;
    return *this;
}
DArray::~DArray() { delete[] data_; delete[] taken_; }

DArray::Iterator DArray::alloc(size_t size) {
    if (size == 0) return create_iterator(nullptr, 0);
    if (size > data_size_) return create_iterator(nullptr, 0);

    size_t bits_needed = size;
    size_t start_pos = 0;
    bool found = false;

    for (size_t i = 0; i <= data_size_ - size; ++i) {
        bool block_free = true;
        for (size_t j = 0; j < size; ++j) {
            size_t bitmap_index = (i + j) / (sizeof(Bitmap) * 8);
            size_t bit_position = (i + j) % (sizeof(Bitmap) * 8);

            if (taken_[bitmap_index] & (1 << bit_position)) {
                block_free = false;
                i = i + j;
                break;
            }
        }

        if (block_free) {
            start_pos = i;
            found = true;
            break;
        }
    }

    if (!found) return create_iterator(nullptr, 0);

    for (size_t j = 0; j < size; ++j) {
        size_t bitmap_index = (start_pos + j) / (sizeof(Bitmap) * 8);
        size_t bit_position = (start_pos + j) % (sizeof(Bitmap) * 8);
        taken_[bitmap_index] |= (1 << bit_position);
    }

    return create_iterator(data_ + start_pos, size);
}

void DArray::free(Iterator& t) {
    if (t.read() == nullptr || t.size() == 0) return;

    ptrdiff_t offset = *t - data_;
    if (offset < 0 || static_cast<size_t>(offset) + t.size() > data_size_) {
        return;
    }

    for (size_t j = 0; j < t.size(); ++j) {
        size_t bitmap_index = (offset + j) / (sizeof(Bitmap) * 8);
        size_t bit_position = (offset + j) % (sizeof(Bitmap) * 8);
        taken_[bitmap_index] &= ~(1 << bit_position);
    }

    t.clear();
}

bool DArray::is_taken(ByteType* addr, size_t size) const {
    if(data_ > addr || data_ + data_size_ < addr) 
        throw std::runtime_error("called is_taken with wrong address");

    ptrdiff_t offset = addr - data_;
    for (size_t j = 0; j < size; ++j) {
        size_t bitmap_index = (offset + j) / (sizeof(Bitmap) * 8);
        size_t bit_position = (offset + j) % (sizeof(Bitmap) * 8);
        if(taken_[bitmap_index] & (1 << bit_position))
            return false;
    }
    return true;
}

DArray::Iterator::Iterator(const Iterator& other)
: data_(other.data_)
, size_(other.size_)
{}

DArray::Iterator::Iterator(Iterator&& other)
: data_(other.data_)
, size_(other.size_)
{
    other.data_ = nullptr;
    other.size_ = 0;
}

DArray::Iterator& DArray::Iterator::operator=(const Iterator& other) {
    if (this != &other) {
        data_ = other.data_;
        size_ = other.size_;
    }
    return *this;
}

DArray::Iterator& DArray::Iterator::operator=(Iterator&& other) {
    if (this != &other) {
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

DArray::Iterator DArray::create_iterator(ByteType* addr, size_t size) const {
    return Iterator(this, addr, size);
}

