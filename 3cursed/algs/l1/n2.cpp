#include <iostream>
#include <bitset>

int main() {
    constexpr size_t LETTERS = 'X' - 'T' + 1;
    constexpr size_t NUMBERS = '4' - '1' + 1;
    constexpr size_t TOTAL = LETTERS + NUMBERS;

    std::bitset<TOTAL> matches; // all bits are 0
    std::string input;
    std::cout << "enter input text: ";
    while(input.empty())
        std::getline(std::cin, input);
    for(char c : input) {
        if('T' <= c && c <= 'X')
            matches[c - 'T'] = 1;
        if('1' <= c && c <= '4')
            matches[LETTERS + c - '1'] = 1;
    }
    std::cout << "{ ";
    bool isfirst = true; // for colons
    for(int i = 0; i < LETTERS; ++i) {
        if(matches[i]) {
            std::cout << (isfirst ? "" : ", ") << static_cast<char>('T' + i);
            isfirst = false;
        }
    }
    for(int i = 0; i < NUMBERS; ++i) {
        if(matches[i + LETTERS]) {
            std::cout << (isfirst ? "" : ",") << static_cast<char>('1' + i);
            isfirst = false;
        }
    }
    std::cout << " }" << std::endl;
}
