#include <iostream>
#include <array>
using namespace std;

int main() {
    std::string input;
    cout << "enter text: " << flush;
    getline(cin, input);
    array<int, 26> letters;
    letters.fill(0);
    for(char c : input) {
        if(c == '.') break;
        if('a' <= c && c <= 'z') {
            ++letters[c - 'a'];
        }
    }

    int counter = 0;
    for(int i = 0; i < 26; ++i) {
        if(letters[i] >= 2) {
            cout << "'" << static_cast<char>('a' + i) << "': " << letters[i] << " matches.\n";
            ++counter;
        }
    }
    cout << "Elements in mnojestvo: " << counter << endl;
    return 0;
}
