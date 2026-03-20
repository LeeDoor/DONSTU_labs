#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include "array_utils.hpp"
#include "memory_manager.hpp"
#include "task.hpp"

std::optional<size_t> get_sizet(const std::string& str) {
    if (!str.empty() && str.find_first_not_of("0123456789") == std::string::npos && str[0] != '0') {
        return static_cast<size_t>(std::stoull(str));
    }
    return std::nullopt;
}

size_t get_K() {
    std::string input;
    std::optional<size_t> conv;
    do {
        std::cout << "> ";
        std::getline(std::cin, input);
        conv = get_sizet(input);
    } while (!conv || *conv == 0);
    return *conv;
}

int main() {
    std::cout << "Enter size of first Array:";
    const size_t int_size = get_K();
    std::cout << "Enter size of second Array:";
    const size_t double_size = get_K();

    const size_t segment_size_in_bytes = std::max(sizeof(int) * int_size, sizeof(double) * double_size);

    try {
        MemoryManager manager(segment_size_in_bytes);

        auto int_ptr = manager.new_pointer<int>(int_size);
        auto double_ptr = manager.new_pointer<double>(double_size);

        fill_random(int_ptr);
        fill_random(double_ptr);

        std::cout << "Integer array:\n";
        print_array(int_ptr);
        try {
            std::cout << "Sum between first two positive: " << sum_between_first_two_positive(int_ptr) << '\n';
        } catch (const std::exception& e) {
            std::cout << e.what() << '\n';
        }

        std::cout << "Double array:\n";
        print_array(double_ptr);
        std::cout << "Max absolute element: " << max_abs_element(double_ptr) << '\n';
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }

    return 0;
}
