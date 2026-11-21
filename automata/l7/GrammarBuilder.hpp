#pragma once

#include "Grammar.hpp"

class GrammarBuilder {
public:
    Grammar readFromCin();

private:
    static std::set<char> parseSymbolSet(const std::string& line, const std::string& name);
    static std::vector<std::string> splitByComma(const std::string& text);
    static std::string trim(const std::string& text);
    static Rule parseRule(const std::string& rawLine);
    static void validate(const std::set<char>& terminals,
                         const std::set<char>& nonterminals,
                         const std::vector<Rule>& rules,
                         char start);
};
