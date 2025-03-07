#include <iostream>
#include <limits.h>
using namespace std;

template <typename T = double>
T getValidInput() {
    T d;
    char pk;
    do{
        cin >> d;
        if(cin.fail()) {
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