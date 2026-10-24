#include "Automaton.h"
#include "AutomatonReader.h"
#include "Renderer.h"
#include <iostream>

int main() {
    Automaton nfa = AutomatonReader::read(std::cin);

    nfa.printEpsilonClosures();
    nfa.printTransitionTable();
    renderNFA(nfa, "nfa");

    Automaton nfaNoEps = nfa.remove_epsilons();
    renderNFA(nfaNoEps, "nfa_no_eps");

    Automaton dfa = nfa.determine();
    dfa.printDFA();
    renderDFA(dfa, "dfa");

    std::cout << "Graphs written: nfa.png, nfa_no_eps.png, dfa.png\n";
    return 0;
}
