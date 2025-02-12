#include <iostream>
#include <cmath>
#include <string>
#include "../get_valid_double.hpp"

int main() {
    double a;
    cout << "enter valid float: ";
    while(!getValidDouble(a));
    cout << "entered " << a << endl;

    double result = a * a * (M_PI - 1);
    cout << "result: " << result << endl;
}