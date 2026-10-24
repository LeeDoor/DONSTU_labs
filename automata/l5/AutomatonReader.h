#pragma once
#include "Automaton.h"
#include <istream>

class AutomatonReader {
public:
    static Automaton read(std::istream& in);
};
