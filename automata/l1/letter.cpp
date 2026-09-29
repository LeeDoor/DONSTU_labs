#include "letter.hpp"
#include <stdexcept>

bool Letter::operator==(const Letter& other) const {
    return get_value() == other.get_value();
}
bool Letter::operator<(const Letter& other) const {
    return get_value() < other.get_value();
}

std::optional<Letter> Letter::char_to_letter(char c) {
    bool letter = is_letter(c);
    if (letter) {
        return Letter{c};
    } else {
        return std::nullopt;
    }
}

bool Letter::is_letter(char c) {
    return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z';
}
Letter::Letter(char letter) : value{letter} {
    if (!is_letter(letter))
        throw std::logic_error("Constructing letter with non-letter character");
}

Letter::Value Letter::get_value() const {
    return value;
}

std::ostream& operator<<(std::ostream& os, const Letter& letter) {
    os << letter.get_value();
    return os;
}
