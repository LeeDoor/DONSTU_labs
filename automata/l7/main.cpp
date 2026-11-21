#include <exception>
#include <iostream>

#include "GrammarBuilder.hpp"

int main() {
    try {
        GrammarBuilder builder;
        const Grammar grammar = builder.readFromCin();
        std::cout << grammar.getType() << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
