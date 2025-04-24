#include "methods2.h"
using namespace std;
int main() {
    string g1text = read_file("G1.txt");
    string s1text = read_file("S1.txt");
    append_file("G1.txt", s1text);
    append_file("S1.txt", g1text);
}
