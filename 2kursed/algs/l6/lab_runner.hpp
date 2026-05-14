#pragma once

#include "array_list.hpp"
#include "array_stack.hpp"
#include "linked_list.hpp"
#include "linked_stack.hpp"
#include "stack_concepts.hpp"

#include <cstddef>
#include <iostream>
#include <random>

class LabRunner {
public:
    void run() {
        run_stack_task();
        run_list_task();
    }

private:
    template <StackType<int> Stack>
    void split_by_parity(Stack& source, Stack& even, Stack& odd) {
        while (auto value = source.pop()) {
            if (*value % 2 == 0) {
                even.push(*value);
            } else {
                odd.push(*value);
            }
        }
    }

    void run_stack_task() {
        constexpr std::size_t capacity = 8;

        LinkedStack<int> linked_source(capacity);
        LinkedStack<int> linked_even(capacity);
        LinkedStack<int> linked_odd(capacity);

        ArrayStack<int> array_source(capacity);
        ArrayStack<int> array_even(capacity);
        ArrayStack<int> array_odd(capacity);

        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::uniform_int_distribution<int> distribution(-20, 20);

        for (std::size_t index = 0; index < capacity; ++index) {
            int value = distribution(generator);
            linked_source.push(value);
            array_source.push(value);
        }

        std::cout << "Task 1\n";
        linked_source.print(std::cout, "Linked source:");
        array_source.print(std::cout, "Array source:");

        split_by_parity(linked_source, linked_even, linked_odd);
        split_by_parity(array_source, array_even, array_odd);

        linked_even.print(std::cout, "Linked even:");
        linked_odd.print(std::cout, "Linked odd:");
        array_even.print(std::cout, "Array even:");
        array_odd.print(std::cout, "Array odd:");

        std::cout << "Linked even top: ";
        print_address(linked_even.top_address());
        std::cout << "\nLinked odd top: ";
        print_address(linked_odd.top_address());
        std::cout << "\nArray even top: ";
        print_address(array_even.top_address());
        std::cout << "\nArray odd top: ";
        print_address(array_odd.top_address());
        std::cout << "\n";
    }

    void run_list_task() {
        constexpr std::size_t capacity = 8;

        LinkedList linked;
        ArrayList array(capacity);
        fill_lists(linked, array, capacity);

        std::cout << "\nTask 2\n";
        linked.print(std::cout, "Linked source:");
        array.print(std::cout, "Array source:");

        linked.move_max_to_front();
        array.move_max_to_front();

        linked.print(std::cout, "Linked result:");
        array.print(std::cout, "Array result:");
    }

    void fill_lists(LinkedList& linked, ArrayList& array, std::size_t count) {
        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::uniform_int_distribution<int> distribution(-20, 20);

        for (std::size_t index = 0; index < count; ++index) {
            int value = distribution(generator);
            linked.push_back(value);
            array.push_back(value);
        }
    }

    void print_address(const void* address) {
        if (address == nullptr) {
            std::cout << "NIL";
        } else {
            std::cout << address;
        }
    }
};
