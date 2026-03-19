#pragma once

#include <concepts>
#include <cstddef>
#include <iostream>
#include <random>
#include "memory_pointer.hpp"

template <std::integral T>
void fill_random(MemoryPointer<T>& ptr, T min_value = static_cast<T>(-100), T max_value = static_cast<T>(100)) {
    ptr.reset();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dist(static_cast<long long>(min_value), static_cast<long long>(max_value));

    for (std::size_t i = 0; i < ptr.size(); ++i) {
        ptr.write(static_cast<T>(dist(gen)));
        if (i + 1 < ptr.size()) {
            ptr.next();
        }
    }

    ptr.reset();
}

template <std::floating_point T>
void fill_random(MemoryPointer<T>& ptr, T min_value = static_cast<T>(-100.0), T max_value = static_cast<T>(100.0)) {
    ptr.reset();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dist(min_value, max_value);

    for (std::size_t i = 0; i < ptr.size(); ++i) {
        ptr.write(dist(gen));
        if (i + 1 < ptr.size()) {
            ptr.next();
        }
    }

    ptr.reset();
}

template <typename T>
void print_array(MemoryPointer<T>& ptr) {
    ptr.reset();

    for (std::size_t i = 0; i < ptr.size(); ++i) {
        std::cout << ptr.read();
        if (i + 1 < ptr.size()) {
            std::cout << ' ';
            ptr.next();
        }
    }

    std::cout << '\n';
    ptr.reset();
}
