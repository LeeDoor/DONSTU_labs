#include <iostream>
#include <limits.h>
using namespace std;

template <typename T = int>
T get_valid_input(T from = INT_MIN, T to = INT_MAX) {
    T d;
    char pk;
    do{
        cin >> d;
        if(cin.fail() || d < from || d > to) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "enter number again" << endl;
            continue;
        }
        pk = cin.peek();
        if(pk == ' ' || pk == '\t' || pk == '\n') break;
    } while(true);
    return d;
}
