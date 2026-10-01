#pragma once
#include <iostream>
#include <limits.h>

template <typename T = int>
T get_valid_input(T from = INT_MIN, T to = INT_MAX) {
    T d;
    char pk;
    do{
        std::cin >> d;
        if(std::cin.fail() || d < from || d > to) {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            std::cout << "enter number again" << std::endl;
            continue;
        }
        pk = std::cin.peek();
        if(pk == ' ' || pk == '\t' || pk == '\n') break;
    } while(true);
    return d;
}
