#include "word.hpp"

std::optional<Word> word_from_stream(std::istream& is) {
    std::string str;
    is >> str;
    Word word;
    word.reserve(str.size());
    for(char c : str) {
        auto letter_opt = Letter::char_to_letter(c);
        if(!letter_opt) {
            return std::nullopt;
        } 
        word.push_back(*letter_opt);
    }
    return word;
}

std::ostream& operator<<(std::ostream& os, const Word& word) {
    if(word.empty()) {
        os << "\"e\"";
        return os;
    }
    os << "\"";
    for(auto& l : word) {
        os << l;
    }
    os << "\"";
    return os;
}
