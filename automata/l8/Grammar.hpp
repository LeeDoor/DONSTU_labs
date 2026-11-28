#pragma once

#include <set>
#include <string>
#include <vector>
#include <map>
#include "Automaton.h"

struct Rule {
    std::string lhs;
    std::string rhs;
};

class Grammar {
public:
    Grammar(std::set<char> terminals,
            std::set<char> nonterminals,
            std::vector<Rule> rules,
            char start);

    std::string getType() const;
    bool isRegular() const;
    Automaton toAutomaton() const;
    Automaton toAutomaton(bool verbose) const;

private:
    std::set<char> terminals_;
    std::set<char> nonterminals_;
    std::vector<Rule> rules_;
    char start_;

    bool isType3() const;
    bool isRightLinear(const Rule& rule) const;
    bool allTerminals(const std::string& text) const;
    bool isLeftLinear(const Rule& rule) const;
    std::string type3Name() const;
    bool isType2() const;
    bool isContextFree(const Rule& rule) const;
    bool isType1() const;
    bool isContextDependent(const Rule& rule) const;
    bool step1NeedsNewState(bool verbose) const;
    void step2InitializeAutomaton(Automaton& automaton,
                                  std::map<char, int>& states,
                                  bool needsNewState,
                                  bool verbose) const;
    void step3BuildTransitions(Automaton& automaton,
                               const std::map<char, int>& states,
                               int newState,
                               bool verbose) const;
    void step4SelectFinalStates(Automaton& automaton,
                                const std::map<char, int>& states,
                                bool verbose) const;
    void step5AddEpsilonFinalState(Automaton& automaton,
                                   const std::map<char, int>& states,
                                   bool verbose) const;
};
