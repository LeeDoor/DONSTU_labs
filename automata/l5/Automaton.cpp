#include "Automaton.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <tuple>

void Automaton::computeEclos() {
    eclos.assign(n, {});
    for (int i = 0; i < n; ++i) {
        StateSet visited = {i};
        std::vector<int> stack = {i};
        while (!stack.empty()) {
            int cur = stack.back();
            stack.pop_back();
            for (auto& t : transitions)
                if (t.from == cur && t.sym.empty() && !visited.count(t.to)) {
                    visited.insert(t.to);
                    stack.push_back(t.to);
                }
        }
        eclos[i] = visited;
    }
}

StateSet Automaton::move(const StateSet& S, const std::string& sym) const {
    StateSet r;
    for (int s : S)
        for (auto& t : transitions)
            if (t.from == s && t.sym == sym) r.insert(t.to);
    return r;
}

StateSet Automaton::eclose(const StateSet& S) const {
    StateSet r;
    for (int s : S)
        for (int e : eclos[s]) r.insert(e);
    return r;
}

Automaton Automaton::remove_epsilons() const {
    Automaton res;
    res.n = n;
    res.start = start;
    res.finals = finals;
    res.alphabet = alphabet;
    std::set<std::tuple<int,int,std::string>> edges;
    for (int p = 0; p < n; ++p)
        for (int q : eclos[p])
            for (auto& t : transitions)
                if (t.from == q && !t.sym.empty())
                    for (int r : eclos[t.to])
                        edges.insert({p, r, t.sym});
    for (auto& [a, b, s] : edges)
        res.transitions.push_back({a, b, s});
    res.computeEclos();
    return res;
}

Automaton Automaton::determine() const {
    Automaton dfa;
    dfa.alphabet = alphabet;
    dfa.start = 0;
    std::vector<StateSet> subsets;
    std::map<StateSet, int> index;
    auto getIndex = [&](const StateSet& s) {
        auto it = index.find(s);
        if (it != index.end()) return it->second;
        int idx = (int)subsets.size();
        index[s] = idx;
        subsets.push_back(s);
        return idx;
    };
    getIndex(eclose({start}));
    std::vector<std::map<std::string, int>> tr;
    for (size_t i = 0; i < subsets.size(); ++i) {
        while (tr.size() <= i) tr.push_back({});
        for (auto& sym : alphabet) {
            StateSet r = eclose(move(subsets[i], sym));
            if (r.empty()) continue;
            int j = getIndex(r);
            while (tr.size() <= (size_t)j) tr.push_back({});
            tr[i][sym] = j;
        }
    }
    dfa.n = (int)subsets.size();
    dfa.stateSets = subsets;
    for (size_t i = 0; i < subsets.size(); ++i)
        for (int s : subsets[i])
            if (finals.count(s)) { dfa.finals.insert((int)i); break; }
    for (size_t i = 0; i < tr.size(); ++i)
        for (auto& [sym, to] : tr[i])
            dfa.transitions.push_back({(int)i, to, sym});
    return dfa;
}

void Automaton::printEpsilonClosures() const {
    std::cout << "Epsilon closures:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "S" << i << " = ~(q" << i << ") = {";
        bool first = true;
        for (int s : eclos[i]) {
            if (!first) std::cout << ", ";
            std::cout << "q" << s;
            first = false;
        }
        std::cout << "}\n";
    }
    std::cout << "\n";
}

void Automaton::printTransitionTable() const {
    std::vector<std::string> syms(alphabet.begin(), alphabet.end());
    std::vector<std::vector<std::string>> cells(n, std::vector<std::string>(syms.size()));
    for (int i = 0; i < n; ++i)
        for (size_t j = 0; j < syms.size(); ++j) {
            StateSet r = eclose(move(eclos[i], syms[j]));
            std::string cell;
            if (r.empty()) cell = "{}";
            else {
                cell = "{";
                bool first = true;
                for (int s : r) {
                    if (!first) cell += ", ";
                    cell += "S" + std::to_string(s);
                    first = false;
                }
                cell += "}";
            }
            cells[i][j] = cell;
        }
    std::vector<size_t> w(syms.size());
    for (size_t j = 0; j < syms.size(); ++j) {
        w[j] = syms[j].size() + 2;
        for (int i = 0; i < n; ++i) w[j] = std::max(w[j], cells[i][j].size() + 2);
    }
    std::cout << "Transition table:\n";
    std::cout << std::left << std::setw(8) << "S[i]";
    for (size_t j = 0; j < syms.size(); ++j) std::cout << std::left << std::setw(w[j]) << syms[j];
    std::cout << "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << std::left << std::setw(8) << ("S" + std::to_string(i));
        for (size_t j = 0; j < syms.size(); ++j) std::cout << std::left << std::setw(w[j]) << cells[i][j];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Automaton::printDFA() const {
    std::cout << "DFA states:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "P" << i << " = {";
        bool first = true;
        for (int s : stateSets[i]) {
            if (!first) std::cout << ", ";
            std::cout << "S" << s;
            first = false;
        }
        std::cout << "}\n";
    }
    std::cout << "\nInitial state: P0\n";
    std::cout << "Final states:";
    for (int f : finals) std::cout << " P" << f;
    std::cout << "\n\n";
    std::vector<std::string> syms(alphabet.begin(), alphabet.end());
    std::vector<std::vector<std::string>> cells(n, std::vector<std::string>(syms.size(), "-"));
    for (auto& t : transitions)
        for (size_t j = 0; j < syms.size(); ++j)
            if (syms[j] == t.sym) cells[t.from][j] = "P" + std::to_string(t.to);
    std::vector<size_t> w(syms.size());
    for (size_t j = 0; j < syms.size(); ++j) {
        w[j] = syms[j].size() + 2;
        for (int i = 0; i < n; ++i) w[j] = std::max(w[j], cells[i][j].size() + 2);
    }
    std::cout << "DFA transition table:\n";
    std::cout << std::left << std::setw(8) << "P[i]";
    for (size_t j = 0; j < syms.size(); ++j) std::cout << std::left << std::setw(w[j]) << syms[j];
    std::cout << "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << std::left << std::setw(8) << ("P" + std::to_string(i));
        for (size_t j = 0; j < syms.size(); ++j) std::cout << std::left << std::setw(w[j]) << cells[i][j];
        std::cout << "\n";
    }
    std::cout << "\n";
}
