#pragma once

#include <cstddef>
#include <cmath>
#include <stdexcept>
#include "memory_pointer.hpp"

inline int sum_between_first_two_positive(MemoryPointer<int>& ptr) {
    ptr.reset();

    bool first_found = false;
    bool second_found = false;
    int sum = 0;

    for (std::size_t i = 0; i < ptr.size(); ++i) {
        const int value = ptr.read();

        if (value > 0) {
            if (!first_found) {
                first_found = true;
            } else {
                second_found = true;
                break;
            }
        } else if (first_found) {
            sum += value;
        }

        if (i + 1 < ptr.size()) {
            ptr.next();
        }
    }

    ptr.reset();

    if (!first_found || !second_found) {
        throw std::runtime_error("Not enough positive elements");
    }

    return sum;
}

inline double max_abs_element(MemoryPointer<double>& ptr) {
    ptr.reset();

    double max_value = ptr.read();
    double max_abs = std::fabs(max_value);

    for (std::size_t i = 1; i < ptr.size(); ++i) {
        ptr.next();
        const double value = ptr.read();
        const double current_abs = std::fabs(value);

        if (current_abs > max_abs) {
            max_abs = current_abs;
            max_value = value;
        }
    }

    ptr.reset();
    return max_value;
}
