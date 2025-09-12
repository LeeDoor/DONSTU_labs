#include "methods2.h"
using namespace std;
string read_file(string filename) {
    ifstream g1(filename);
    stringstream ss;
    ss << g1.rdbuf();
    return ss.str();
}
void append_file(string filename, string& text) {
    ofstream f(filename, ios::app);
    f << text;
}
