#include "GrammarBuilder.hpp"

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <utility>

Grammar GrammarBuilder::readFromCin() {
    std::set<char> terminals;
    std::set<char> nonterminals;
    std::vector<Rule> rules;
    char start = '\0';
    std::string line;

    std::cout << "Terminals (comma-separated): ";
    if (!std::getline(std::cin, line)) {
        throw std::runtime_error("failed to read terminals");
    }
    terminals = parseSymbolSet(line, "terminal");

    std::cout << "Nonterminals (comma-separated): ";
    if (!std::getline(std::cin, line)) {
        throw std::runtime_error("failed to read nonterminals");
    }
    nonterminals = parseSymbolSet(line, "nonterminal");

    std::cout << "Rules (one per line, blank line to finish):\n";
    while (std::getline(std::cin, line)) {
        if (trim(line).empty()) {
            break;
        }
        rules.push_back(parseRule(line));
    }

    std::cout << "Start symbol: ";
    if (!std::getline(std::cin, line)) {
        throw std::runtime_error("failed to read the start symbol");
    }
    line = trim(line);
    if (line.size() != 1) {
        throw std::runtime_error("the start symbol must be exactly one character");
    }
    start = line[0];

    validate(terminals, nonterminals, rules, start);

    return Grammar(
        std::move(terminals),
        std::move(nonterminals),
        std::move(rules),
        start
    );
}

std::set<char> GrammarBuilder::parseSymbolSet(const std::string& line, const std::string& name) {
    std::set<char> result;

    for (const std::string& token : splitByComma(line)) {
        if (token.size() != 1) {
            throw std::runtime_error(name + " symbol must be exactly one character: '" + token + "'");
        }
        result.insert(token[0]);
    }

    return result;
}

std::vector<std::string> GrammarBuilder::splitByComma(const std::string& text) {
    std::vector<std::string> parts;
    if (trim(text).empty()) {
        return parts;
    }

    std::size_t start = 0;
    while (true) {
        std::size_t comma = text.find(',', start);
        std::size_t length = (comma == std::string::npos) ? std::string::npos : comma - start;
        parts.push_back(trim(text.substr(start, length)));

        if (comma == std::string::npos) {
            break;
        }
        start = comma + 1;
    }

    return parts;
}

std::string GrammarBuilder::trim(const std::string& text) {
    std::size_t begin = 0;
    std::size_t end = text.size();

    while (begin < end && std::isspace(static_cast<unsigned char>(text[begin]))) {
        ++begin;
    }
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        --end;
    }

    return text.substr(begin, end - begin);
}

Rule GrammarBuilder::parseRule(const std::string& rawLine) {
    std::string line = trim(rawLine);
    Rule rule;

    std::size_t separator = line.find_first_of(" \t");
    if (separator == std::string::npos) {
        rule.lhs = line;
        rule.rhs = "";
    } else {
        rule.lhs = trim(line.substr(0, separator));
        rule.rhs = trim(line.substr(separator + 1));
    }

    return rule;
}

void GrammarBuilder::validate(const std::set<char>& terminals,
                              const std::set<char>& nonterminals,
                              const std::vector<Rule>& rules,
                              char start) {
    if (nonterminals.empty()) {
        throw std::runtime_error("the set of nonterminals must not be empty");
    }
    if (start == '\0' || nonterminals.count(start) == 0) {
        throw std::runtime_error("the start symbol must belong to the set of nonterminals");
    }

    for (char terminal : terminals) {
        if (nonterminals.count(terminal) != 0) {
            throw std::runtime_error(std::string("symbol '") + terminal + "' belongs to both T and N");
        }
    }

    for (std::size_t i = 0; i < rules.size(); ++i) {
        const Rule& rule = rules[i];
        std::string prefix = "rule " + std::to_string(i + 1) + ": ";

        if (rule.lhs.empty()) {
            throw std::runtime_error(prefix + "the left side must not be empty");
        }

        for (char symbol : rule.lhs) {
            if (terminals.count(symbol) == 0 && nonterminals.count(symbol) == 0) {
                throw std::runtime_error(prefix + "unknown symbol '" + symbol + "' on the left side");
            }
        }
        for (char symbol : rule.rhs) {
            if (terminals.count(symbol) == 0 && nonterminals.count(symbol) == 0) {
                throw std::runtime_error(prefix + "unknown symbol '" + symbol + "' on the right side");
            }
        }
    }
}
