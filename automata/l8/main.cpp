#include <exception>
#include <iostream>
#include "GrammarBuilder.hpp"
#include "Renderer.h"

int main() {
    try {
        GrammarBuilder builder;
        Grammar grammar = builder.readFromCin();
        std::cout << "Grammar type: " << grammar.getType() << '\n';
        if (!grammar.isRegular()) {
            std::cout << "The grammar is not regular; automaton construction skipped.\n";
            return 0;
        }
        Automaton nfa = grammar.toAutomaton(true);
        std::cout << "Regular grammar accepted. NFA: " << nfa.n << " states, "
                  << nfa.transitions.size() << " transitions.\n";
        renderNFA(nfa, "grammar_nfa");
        Automaton dfa = nfa.determine(true);
        dfa.printDFA();
        renderDFA(dfa, "grammar_dfa");
        std::cout << "Images written: grammar_nfa.png, grammar_dfa.png\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
