#include "Grammar.hpp"

#include <algorithm>
#include <utility>
#include <map>
#include <stdexcept>
#include <iostream>

Grammar::Grammar(std::set<char> terminals,
                 std::set<char> nonterminals,
                 std::vector<Rule> rules,
                 char start)
    : terminals_(std::move(terminals)),
      nonterminals_(std::move(nonterminals)),
      rules_(std::move(rules)),
      start_(start) {}

std::string Grammar::getType() const {
    if (isType3()) {
        return type3Name();
    }
    if (isType2()) {
        return "Type 2 (context-free)";
    }
    if (isType1()) {
        return "Type 1 (context-dependent)";
    }
    return "Type 0 (unrestricted)";
}

bool Grammar::isRegular() const { return isType3(); }

Automaton Grammar::toAutomaton() const { return toAutomaton(false); }

Automaton Grammar::toAutomaton(bool verbose) const {
    if (!isRegular()) throw std::runtime_error("grammar is not regular");
    if (!std::all_of(rules_.begin(), rules_.end(), [this](const Rule& r) {
            if (r.rhs.empty()) return r.lhs.size() == 1 && r.lhs[0] == start_;
            if (r.rhs.size() == 1) return terminals_.count(r.rhs[0]) != 0;
            return r.rhs.size() == 2 && terminals_.count(r.rhs[0]) != 0
                && nonterminals_.count(r.rhs[1]) != 0;
        })) {
        throw std::runtime_error("this construction requires rules A -> aB, A -> a, or S -> epsilon");
    }
    Automaton a;
    std::map<char, int> state;
    bool needsNewState = step1NeedsNewState(verbose);
    step2InitializeAutomaton(a, state, needsNewState, verbose);
    const int newState = needsNewState ? a.n - 1 : -1;
    step3BuildTransitions(a, state, newState, verbose);
    step4SelectFinalStates(a, state, verbose);
    step5AddEpsilonFinalState(a, state, verbose);
    a.computeEclos();
    return a;
}

bool Grammar::step1NeedsNewState(bool verbose) const {
    if (verbose) std::cout << "\nStep 1. Augment the grammar:\n";
    bool needsNewState = false;
    bool foundTerminalRule = false;
    for (const Rule& terminalRule : rules_) {
        if (terminalRule.rhs.size() != 1 || !terminals_.count(terminalRule.rhs[0])) continue;
        foundTerminalRule = true;
        bool matching = std::any_of(rules_.begin(), rules_.end(), [&](const Rule& rule) {
            return rule.lhs == terminalRule.lhs && rule.rhs.size() == 2
                && rule.rhs[0] == terminalRule.rhs[0] && nonterminals_.count(rule.rhs[1]);
        });
        if (verbose) {
            std::cout << "  Rule " << terminalRule.lhs << " -> " << terminalRule.rhs
                      << (matching ? " has a matching A -> aB rule; no new rule is added.\n"
                                   : " has no matching transition; add " + terminalRule.lhs + " -> "
                                     + terminalRule.rhs + "N'.\n");
        }
        needsNewState |= !matching;
    }
    if (verbose && !foundTerminalRule)
        std::cout << "  No terminal-only productions require augmentation.\n";
    return needsNewState;
}

void Grammar::step2InitializeAutomaton(Automaton& automaton,
                                       std::map<char, int>& states,
                                       bool needsNewState,
                                       bool verbose) const {
    for (char symbol : nonterminals_) states[symbol] = static_cast<int>(states.size());
    automaton.n = static_cast<int>(states.size());
    if (needsNewState) ++automaton.n;
    automaton.start = states.at(start_);
    for (char terminal : terminals_) automaton.alphabet.insert(std::string(1, terminal));
    if (!verbose) return;
    std::cout << "Step 2. Define the automaton sets:\n  q0 = " << start_ << "\n  Q = {";
    bool first = true;
    for (const auto& [symbol, id] : states) {
        (void)id;
        if (!first) std::cout << ", ";
        std::cout << symbol;
        first = false;
    }
    if (needsNewState) { if (!first) std::cout << ", "; std::cout << "N'"; }
    std::cout << "}\n  E = {";
    first = true;
    for (char symbol : terminals_) {
        if (!first) std::cout << ", ";
        std::cout << symbol;
        first = false;
    }
    std::cout << "}\n";
}

void Grammar::step3BuildTransitions(Automaton& automaton,
                                    const std::map<char, int>& states,
                                    int newState,
                                    bool verbose) const {
    if (verbose) std::cout << "Step 3. Convert productions to transitions:\n";
    for (const Rule& rule : rules_) {
        if (rule.rhs.size() != 2) continue;
        const std::string symbol(1, rule.rhs[0]);
        automaton.transitions.push_back({states.at(rule.lhs[0]), states.at(rule.rhs[1]), symbol});
        if (verbose) std::cout << "  " << rule.lhs << " -> " << rule.rhs << "  =>  δ("
                               << rule.lhs[0] << ", " << rule.rhs[0] << ") = " << rule.rhs[1] << '\n';
    }
    for (const Rule& rule : rules_) {
        if (rule.rhs.size() != 1 || !terminals_.count(rule.rhs[0])) continue;
        bool matching = std::any_of(rules_.begin(), rules_.end(), [&](const Rule& candidate) {
            return candidate.lhs == rule.lhs && candidate.rhs.size() == 2
                && candidate.rhs[0] == rule.rhs[0] && nonterminals_.count(candidate.rhs[1]);
        });
        if (matching) continue;
        const std::string symbol(1, rule.rhs[0]);
        automaton.transitions.push_back({states.at(rule.lhs[0]), newState, symbol});
        if (verbose) std::cout << "  " << rule.lhs << " -> " << rule.rhs << "N'  =>  δ("
                               << rule.lhs[0] << ", " << rule.rhs[0] << ") = N'\n";
    }
}

void Grammar::step4SelectFinalStates(Automaton& automaton,
                                     const std::map<char, int>& states,
                                     bool verbose) const {
    if (verbose) std::cout << "Step 4. Select final states from matching terminal productions:\n";
    for (const Rule& transition : rules_) {
        if (transition.rhs.size() != 2) continue;
        bool hasTerminalProduction = std::any_of(rules_.begin(), rules_.end(), [&](const Rule& rule) {
            return rule.lhs == transition.lhs && rule.rhs.size() == 1
                && rule.rhs[0] == transition.rhs[0];
        });
        if (!hasTerminalProduction) continue;
        bool inserted = automaton.finals.insert(states.at(transition.rhs[1])).second;
        if (verbose && inserted)
            std::cout << "  " << transition.lhs << " -> " << transition.rhs << " and "
                      << transition.lhs << " -> " << transition.rhs[0]
                      << " => final state " << transition.rhs[1] << '\n';
    }
}

void Grammar::step5AddEpsilonFinalState(Automaton& automaton,
                                        const std::map<char, int>& states,
                                        bool verbose) const {
    if (verbose) std::cout << "Step 5. Check for S -> ε:\n";
    bool hasEpsilonStartRule = std::any_of(rules_.begin(), rules_.end(), [this](const Rule& rule) {
        return rule.lhs.size() == 1 && rule.lhs[0] == start_ && rule.rhs.empty();
    });
    if (hasEpsilonStartRule) {
        automaton.finals.insert(states.at(start_));
        if (verbose) std::cout << "  S -> ε found; " << start_ << " is final.\n";
    } else if (verbose) {
        std::cout << "  No S -> ε production.\n";
    }
    // N' is final because every added A -> aN' stands for the original A -> a.
    if (automaton.n > static_cast<int>(nonterminals_.size())) {
        automaton.finals.insert(automaton.n - 1);
        if (verbose) std::cout << "  N' is final because it was added for a terminal-only production.\n";
    }
    if (verbose) {
        std::cout << "  Final states F = {";
        bool first = true;
        for (int finalState : automaton.finals) {
            if (!first) std::cout << ", ";
            auto it = std::find_if(states.begin(), states.end(), [finalState](const auto& item) {
                return item.second == finalState;
            });
            std::cout << (it == states.end() ? "N'" : std::string(1, it->first));
            first = false;
        }
        std::cout << "}\n";
    }
}

bool Grammar::isType3() const {
    bool rightLinear = std::all_of(
        rules_.begin(),
        rules_.end(),
        [this](const Rule& rule) { return isRightLinear(rule); }
    );
    if (rightLinear) {
        return true;
    }

    return std::all_of(
        rules_.begin(),
        rules_.end(),
        [this](const Rule& rule) { return isLeftLinear(rule); }
    );
}

bool Grammar::isRightLinear(const Rule& rule) const {
    if (rule.lhs.size() != 1 || nonterminals_.count(rule.lhs[0]) == 0) {
        return false;
    }
    if (rule.rhs.empty() || allTerminals(rule.rhs)) {
        return true;
    }

    char last = rule.rhs.back();
    if (nonterminals_.count(last) == 0) {
        return false;
    }

    return allTerminals(rule.rhs.substr(0, rule.rhs.size() - 1));
}

bool Grammar::allTerminals(const std::string& text) const {
    for (char symbol : text) {
        if (terminals_.count(symbol) == 0) {
            return false;
        }
    }
    return true;
}

bool Grammar::isLeftLinear(const Rule& rule) const {
    if (rule.lhs.size() != 1 || nonterminals_.count(rule.lhs[0]) == 0) {
        return false;
    }
    if (rule.rhs.empty() || allTerminals(rule.rhs)) {
        return true;
    }

    char first = rule.rhs.front();
    if (nonterminals_.count(first) == 0) {
        return false;
    }

    return allTerminals(rule.rhs.substr(1));
}

std::string Grammar::type3Name() const {
    bool rightLinear = std::all_of(
        rules_.begin(),
        rules_.end(),
        [this](const Rule& rule) { return isRightLinear(rule); }
    );
    bool leftLinear = std::all_of(
        rules_.begin(),
        rules_.end(),
        [this](const Rule& rule) { return isLeftLinear(rule); }
    );

    if (rightLinear && leftLinear) {
        return "Type 3 (right-linear and left-linear)";
    }
    if (rightLinear) {
        return "Type 3 (right-linear)";
    }
    if (leftLinear) {
        return "Type 3 (left-linear)";
    }
    return "";
}

bool Grammar::isType2() const {
    return std::all_of(
        rules_.begin(),
        rules_.end(),
        [this](const Rule& rule) { return isContextFree(rule); }
    );
}

bool Grammar::isContextFree(const Rule& rule) const {
    return rule.lhs.size() == 1 && nonterminals_.count(rule.lhs[0]) != 0;
}

bool Grammar::isType1() const {
    return std::all_of(
        rules_.begin(),
        rules_.end(),
        [this](const Rule& rule) { return isContextDependent(rule); }
    );
}

bool Grammar::isContextDependent(const Rule& rule) const {
    if (rule.lhs.empty() || rule.rhs.empty()) {
        return false;
    }

    for (std::size_t index = 0; index < rule.lhs.size(); ++index) {
        if (nonterminals_.count(rule.lhs[index]) == 0) {
            continue;
        }

        std::string leftContext = rule.lhs.substr(0, index);
        std::string rightContext = rule.lhs.substr(index + 1);

        if (rule.rhs.size() < leftContext.size() + rightContext.size() + 1) {
            continue;
        }
        if (rule.rhs.compare(0, leftContext.size(), leftContext) != 0) {
            continue;
        }
        if (!rightContext.empty()) {
            std::size_t suffixStart = rule.rhs.size() - rightContext.size();
            if (rule.rhs.compare(suffixStart, rightContext.size(), rightContext) != 0) {
                continue;
            }
        }

        return true;
    }

    return false;
}
