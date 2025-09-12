#include <iostream>
#include <cmath>
#include <string>
#include "../get_valid_input.hpp"

int main() {
    cout << "enter valid float: ";
    double a = getValidInput();
    cout << "entered " << a << endl;

    double result = a * a * (M_PI - 1);
    cout << "result: " << result << endl;
}