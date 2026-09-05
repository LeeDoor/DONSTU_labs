#include "alphabet.hpp"
#include "get_valid_input.hpp"
#include <iostream>

int main() {
    auto alphabet_opt = Alphabet::from_stream(std::cin);
    if (!alphabet_opt) {
        std::cout << "Could not build alphabet from user input\n";
        return 1;
    }
    Alphabet alphabet = *alphabet_opt;
    std::cout << alphabet << std::endl;

    std::cout << "Enter a word (no space separation):> ";
    auto word_opt = word_from_stream(std::cin);
    if(!word_opt) {
        std::cout << "Could not build word from user input\n";
        return 1;
    }
    Word word = *word_opt;
    std::cout << "Word: " << word << "\n";
    std::cout << "Does word match alphabet: " << std::boolalpha << alphabet.does_word_match(word) << "\n";
    if(alphabet.does_word_match(word)) {
        auto number_opt = alphabet.get_number_of_word(word); 
        std::cout << "Word's number in this alphabet: " << *number_opt << "\n";
    }
    std::cout << "Enter number of word:> ";
    Alphabet::Number number = get_valid_input(0);
    Word recovered_word = alphabet.get_word_from_number(number);
    std::cout << "Word from number " << number << ": " << recovered_word << "\n";
}
