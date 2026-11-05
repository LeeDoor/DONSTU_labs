#include "AutomatonReader.h"
#include <sstream>
#include <stdexcept>

// Разбирает число состояний, строку финальных состояний и переходы, проверяя их диапазоны и структуру.
Automaton AutomatonReader::read(std::istream& in) {
    Automaton automaton;
    if (!(in >> automaton.stateCount) || automaton.stateCount <= 0) {
        throw std::runtime_error("The first line must contain a positive number of states.");
    }

    std::string line;
    std::getline(in, line);
    if (!std::getline(in, line)) {
        throw std::runtime_error("The second line must list accepting states.");
    }
    std::istringstream finalLine(line);
    int state;
    while (finalLine >> state) {
        if (state < 0 || state >= automaton.stateCount) {
            throw std::runtime_error("An accepting state is outside the range 0..n-1.");
        }
        automaton.finals.insert(state);
    }
    if (!finalLine.eof()) {
        throw std::runtime_error("Accepting states must be integers.");
    }

    while (std::getline(in, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        std::istringstream row(line);
        Transition transition;
        std::string extra;
        if (!(row >> transition.from >> transition.to >> transition.symbol) ||
            (row >> extra)) {
            throw std::runtime_error(
                "Each transition must have exactly three fields: from to symbol.");
        }
        if (transition.from < 0 || transition.from >= automaton.stateCount ||
            transition.to < 0 || transition.to >= automaton.stateCount) {
            throw std::runtime_error("A transition state is outside the range 0..n-1.");
        }
        if (transition.symbol == "-") {
            throw std::runtime_error("The symbol '-' is reserved for missing transitions.");
        }
        automaton.alphabet.insert(transition.symbol);
        automaton.transitions.push_back(transition);
    }
    return automaton;
}
