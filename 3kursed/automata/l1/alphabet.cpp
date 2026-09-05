#include "alphabet.hpp"
#include "get_valid_input.hpp"
#include <algorithm>

Alphabet::Alphabet(const Value &characters) : value(characters) {}
Alphabet::Alphabet(Value &&characters) : value(std::move(characters)) {}

std::optional<Alphabet> Alphabet::from_stream(std::istream &is) {
    std::cout << "Enter alphabet size [1; 15] :> ";
    int count = get_valid_input(1, 15);
    Value value;
    std::string buffer;

    std::cout << "Enter alphabet characters (english letters only):> ";
    while (std::getline(std::cin, buffer)) {
        for (char c : buffer) {
            if (auto letter = Letter::char_to_letter(c); letter) {
                if(std::find(value.begin(), value.end(), letter) != value.end()) {
                    std::cout << "WARNING: character '" << c << "' is duplicated; ignoring\n";
                    continue;
                }
                value.push_back(*letter);
                if (value.size() == count) {
                    break;
                }
            }
        }
        if (value.size() == count) {
            break;
        }
        std::cout << ":> ";
    }
    if(value.size() != count) return std::nullopt;
    return value;
}

const Alphabet::Value& Alphabet::get_value() const {
    return value;
}

bool Alphabet::is_letter_in_alphabet(Letter letter) const {
    return std::find(value.begin(), value.end(), letter) != value.end();
}
bool Alphabet::does_word_match(const Word& word_str) const {
    for(auto l : word_str)
        if(!is_letter_in_alphabet(l)) 
            return false;
    return true;
}
constexpr long long ipow(long long base, unsigned int exp) {
    long long res = 1;
    while (exp > 0) {
        if (exp % 2 == 1) res *= base;
        base *= base;
        exp /= 2;
    }
    return res;
}
std::optional<Alphabet::Number> Alphabet::get_number_of_word(const Word& word) const {
    if(!does_word_match(word)) return std::nullopt;
    int k = word.size();
    int n = value.size();
    Number result = 0;
    for(int i = 0; i < word.size(); ++i) {
        Letter letter = word[i];
        int i_k = get_position_of_letter(letter);
        result += i_k * ipow(n, k - i - 1);
    }
    return result;
}
Word Alphabet::get_word_from_number(Number number) const {
    Word result;
    int n = value.size();
    while(number > 0) {
        int remainder = number % n;
        if (remainder == 0) remainder = n;
        result.push_back(value[remainder - 1]);
        number /= n;
        if (remainder == n) number -= 1;
        std::cout << result.back() << ": " << number << "*" << n << " + " << remainder << "\n";
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int Alphabet::get_position_of_letter(Letter letter) const {
    for(int i = 0; i < value.size(); ++i) {
        if(letter == value[i]) return i + 1;
    }
    throw std::logic_error("Invalid use of get_index_of_letter: passed letter not from this alphabet");
}

std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet) {
    os << "{ ";
    auto& value = alphabet.get_value();
    for(auto iter = value.begin(); iter != value.end(); ++iter) {
        os << *iter;
        if(iter != std::prev(value.end())){
            os << ", ";
        }
    }
    os << " }";
    return os;
}
