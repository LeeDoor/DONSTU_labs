#pragma once
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>

class DArray {
public:
    using ByteType = char;
    using Bitmap = unsigned;

    class Iterator {
    public:
        friend DArray;

        Iterator(const Iterator& other);
        Iterator(Iterator&& other);
        Iterator& operator=(const Iterator& other);
        Iterator& operator=(Iterator&& other);
        ~Iterator() {
            if(data_ != nullptr && owner_array_->is_taken(data_, size_)) {
                std::cerr << "Called DArray::~Iterator before clearing it" << std::flush;
#ifndef NDEBUG
                std::exit(1);
#endif
            }
        }

        ByteType* operator*() { return data_; }
        ByteType* operator->() { return data_; }
        ByteType* read() { return data_; }

        size_t size() const { return size_; }

        void write(const ByteType* new_data, size_t size) {
            if(size > size_) 
                throw std::runtime_error("too big size index at DArray::Iterator::write");
            std::memcpy(data_, new_data, size);
        }
        void clear() { data_ = nullptr; size_ = 0; }

    protected:
        Iterator(const DArray* owner, ByteType* data, size_t size)
        : owner_array_{ owner }
        , data_{ data }
        , size_{ size }
        {}

        const DArray* owner_array_;
        ByteType* data_;
        size_t size_;
    };

    DArray(size_t size);
    DArray();
    DArray(const DArray& other);
    DArray(DArray&& other);
    DArray& operator=(const DArray& other);
    DArray& operator=(DArray&& other);
    ~DArray();

    Iterator alloc(size_t size);
    void free(Iterator& t);
    bool is_taken(ByteType* addr, size_t size) const;

protected:
    DArray::Iterator create_iterator(ByteType* addr, size_t size) const;

    size_t data_size_;
    ByteType* data_;
    size_t taken_size_;
    Bitmap* taken_;
};
