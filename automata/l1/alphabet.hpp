#pragma once
#include "letter.hpp"
#include "word.hpp"
#include <vector>
#include <istream>

class Alphabet {
public:
    using Value = std::vector<Letter>;
    using Number = unsigned long;

    Alphabet(const Value &characters);
    Alphabet(Value &&characters);

    static std::optional<Alphabet> from_stream(std::istream &is);

    const Value& get_value() const;
    
    bool is_letter_in_alphabet(Letter letter) const;
    bool does_word_match(const Word& word) const;
    std::optional<Alphabet::Number> get_number_of_word(const Word& word) const;
    Word get_word_from_number(Number number) const;

private:
    int get_position_of_letter(Letter letter) const;

    Value value;
};

std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet);
