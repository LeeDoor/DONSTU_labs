#include <iostream>
#include <limits.h>
using namespace std;

bool getValidDouble(double &d) {
    cin >> d;
    if(cin.fail()) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "enter number again" << endl;
        return false;
    }
    char pk = cin.peek();
    if(pk == ' ' || pk == '\t' || pk == '\n') {
        return true;
    }
    return false;
}