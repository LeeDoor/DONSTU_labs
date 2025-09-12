#include <iostream>
#include "../get_valid_input.hpp"
#include <cmath>
using namespace std;;

double func(double a, double b) {
    return sqrt(
        a * a + b * b +
        pow(sin(a * b), 2)
    );
}

int main() {
    double x, y, z;
    cout << "enter x: ";
    x = getValidInput<double>();
    cout << "enter y: ";
    y = getValidInput<double>();
    cout << "enter z: ";
    z = getValidInput<double>();
    cout << "initial values: " << endl
         << "x: " << x << endl
         << "y: " << y << endl
         << "z: " << z << endl
         << "result value: " 
         << func(x, y) + func(x, z) + func(y, z)
         << endl;
}