#include "AutomatonReader.h"
#include <sstream>

Automaton AutomatonReader::read(std::istream& in) {
    Automaton a;
    in >> a.n;
    std::string line;
    std::getline(in, line);
    std::getline(in, line);
    std::istringstream is(line);
    int f;
    while (is >> f) a.finals.insert(f);
    while (std::getline(in, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        std::istringstream ls(line);
        Transition t;
        if (!(ls >> t.from >> t.to)) continue;
        if (ls >> t.sym) a.alphabet.insert(t.sym);
        else t.sym = "";
        a.transitions.push_back(t);
    }
    a.computeEclos();
    return a;
}
