#include "methods2.h"
#include <filesystem>
using namespace std;
int main() {
    if(!filesystem::exists("G1.txt") || !filesystem::exists("S1.txt")) {
        cout << "failov nesushestivuit!" << endl;
        return -1;
    }
    string g1text = read_file("G1.txt");
    string s1text = read_file("S1.txt");
    append_file("G1.txt", s1text);
    append_file("S1.txt", g1text);
    cout << "appended successfully." << endl;
}
