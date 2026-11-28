#include "Renderer.h"
#include <fstream>
#include <cstdlib>

static void writeDot(const std::string& name, const std::string& dot) {
    std::ofstream(name + ".dot") << dot;
    system(("dot -Tpng " + name + ".dot -o " + name + ".png").c_str());
}

void renderNFA(const Automaton& a, const std::string& name) {
    std::string d = "digraph NFA{rankdir=LR;node[shape=circle];start[shape=point];start->q0;";
    for (int f : a.finals) d += "q" + std::to_string(f) + "[shape=doublecircle];";
    for (auto& t : a.transitions)
        d += "q" + std::to_string(t.from) + "->q" + std::to_string(t.to)
           + "[label=\"" + (t.sym.empty() ? "ε" : t.sym) + "\"];";
    writeDot(name, d + "}");
}

void renderDFA(const Automaton& dfa, const std::string& name) {
    std::string d = "digraph DFA{rankdir=LR;node[shape=circle];start[shape=point];start->P0;";
    for (int f : dfa.finals) d += "P" + std::to_string(f) + "[shape=doublecircle];";
    for (auto& t : dfa.transitions)
        d += "P" + std::to_string(t.from) + "->P" + std::to_string(t.to)
           + "[label=\"" + t.sym + "\"];";
    writeDot(name, d + "}");
}
