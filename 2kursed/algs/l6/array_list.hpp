#pragma once

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>

class ArrayList {
public:
    explicit ArrayList(std::size_t capacity)
        : data_(capacity == 0 ? nullptr : std::make_unique<int[]>(capacity)),
          size_(0),
          capacity_(capacity) {}

    void push_back(int value) {
        if (size_ == capacity_) {
            throw std::overflow_error("ArrayList capacity exceeded");
        }
        data_[size_++] = value;
    }

    bool is_empty() const {
        return size_ == 0;
    }

    void move_max_to_front() {
        if (size_ < 2) {
            return;
        }

        std::size_t max_index = 0;
        for (std::size_t index = 1; index < size_; ++index) {
            if (data_[index] > data_[max_index]) {
                max_index = index;
            }
        }

        int max_value = data_[max_index];
        for (std::size_t index = max_index; index > 0; --index) {
            data_[index] = data_[index - 1];
        }
        data_[0] = max_value;
    }

    void print(std::ostream& out, std::string_view title) const {
        out << title << "\n";
        if (is_empty()) {
            out << "empty\n";
            return;
        }

        bool first = true;
        for (std::size_t index = 0; index < size_; ++index) {
            if (!first) {
                out << ' ';
            }
            out << data_[index];
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

    std::unique_ptr<int[]> data_;
    std::size_t size_;
    std::size_t capacity_;
};
