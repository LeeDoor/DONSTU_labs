#include "Grammar.hpp"

#include <algorithm>
#include <utility>

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
