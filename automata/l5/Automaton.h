#pragma once
#include <vector>
#include <set>
#include <string>

typedef std::set<int> StateSet;

struct Transition {
    int from;
    int to;
    std::string sym;
};

class Automaton {
public:
    int n = 0;
    int start = 0;
    std::set<int> finals;
    std::vector<Transition> transitions;
    std::set<std::string> alphabet;
    std::vector<StateSet> eclos;
    std::vector<StateSet> stateSets;

    void computeEclos();
    StateSet move(const StateSet& S, const std::string& sym) const;
    StateSet eclose(const StateSet& S) const;
    Automaton remove_epsilons() const;
    Automaton determine() const;
    void printEpsilonClosures() const;
    void printTransitionTable() const;
    void printDFA() const;
};
