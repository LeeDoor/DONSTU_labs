#include<iostream>
#include "../get_valid_input.hpp"
using namespace std;

bool IsPower2(unsigned N, unsigned pow = 1) {
    if(N > pow) return IsPower2(N, pow * 2);
    if(N == pow) return true;
    return false; 
}

int main() {
    int N;
    do{
        cout << "enter natural N: ";
        N = getValidInput<int>();
    } while(N < 0);
    if(IsPower2(N)) {
        cout << "YES" << endl;
    } else cout << "NO" << endl;
}