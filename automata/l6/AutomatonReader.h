#pragma once

#include "Automaton.h"
#include <istream>

class AutomatonReader {
public:
    // Читает автомат из потока в формате лабораторной и сообщает об ошибках формата исключением.
    static Automaton read(std::istream& in);
};
