#include <iostream>
#include <array>
using namespace std;

int main() {
    std::string input;
    cout << "enter text: " << std::flush;
    getline(cin, input);
    std::array<int, 26> letters;
    letters.fill(0);
    for(char c : input) {
        if(c == '.') break;
        if('a' <= c && c <= 'z') {
            ++letters[c - 'a'];
        }
    }
    for(int i = 0; i < 26; ++i) {
        if(letters[i])
            cout << "'" << static_cast<char>('a' + i) << "': " << letters[i] << " matches.\n";
    }
    return 0;
}
