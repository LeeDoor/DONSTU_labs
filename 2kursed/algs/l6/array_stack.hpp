#pragma once

#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>

template <typename T>
class ArrayStack {
public:
    explicit ArrayStack(std::size_t capacity)
        : data_(capacity == 0 ? nullptr : std::make_unique<T[]>(capacity)),
          size_(0),
          capacity_(capacity) {}

    ArrayStack(const ArrayStack&) = delete;
    ArrayStack& operator=(const ArrayStack&) = delete;
    ArrayStack(ArrayStack&&) noexcept = default;
    ArrayStack& operator=(ArrayStack&&) noexcept = default;

    void push(const T& value) {
        if (size_ == capacity_) {
            throw std::overflow_error("ArrayStack capacity exceeded");
        }
        data_[size_++] = value;
    }

    std::optional<T> pop() {
        if (is_empty()) {
            return std::nullopt;
        }
        return data_[--size_];
    }

    bool is_empty() const {
        return size_ == 0;
    }

    const void* top_address() const {
        if (is_empty()) {
            return nullptr;
        }
        return &data_[size_ - 1];
    }

    void print(std::ostream& out, std::string_view title) const {
        out << title << "\n";
        if (is_empty()) {
            out << "empty\n";
            return;
        }

        bool first = true;
        for (std::size_t index = size_; index > 0; --index) {
            if (!first) {
                out << ' ';
            }
            out << data_[index - 1];
            first = false;
        }
        out << "\n";
    }

private:
    static void print_address(std::ostream& out, const void* address) {
        if (address == nullptr) {
            out << "NIL";
        } else {
            out << address;
        }
    }

    std::unique_ptr<T[]> data_;
    std::size_t size_;
    std::size_t capacity_;
};
