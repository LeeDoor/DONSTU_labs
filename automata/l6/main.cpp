#include "Automaton.h"
#include "AutomatonReader.h"
#include "Renderer.h"
#include <iostream>
#include <stdexcept>

// Загружает входной автомат, минимизирует его, сохраняет обе схемы и печатает результат.
int main() {
    try {
        const Automaton automaton = AutomatonReader::read(std::cin);
        const Automaton minimized = automaton.minimize();

        Renderer::render(automaton, "automaton");
        Renderer::render(minimized, "minimized");
        minimized.print(std::cout);

        std::cerr << "Images written: automaton.png, minimized.png\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
