#pragma once

#include <set>
#include <string>
#include <vector>

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
};
