#include <iostream>
#include "../get_valid_input.hpp"
using namespace std;

void ShiftLeft3(double& a, double& b, double& c) {
    double buff = a;
    a = b;
    b = c;
    c = buff;
}

void getInput(double& a, double& b, double& c) {
    cout << "enter a: ";
    a = getValidInput<double>();
    cout << "enter b: ";
    b = getValidInput<double>();
    cout << "enter c: ";
    c = getValidInput<double>();
}

void task() {
    double a, b, c;
    getInput(a, b, c);
    cout << "initial set: " << endl;
    cout << "( " << a << ", " << b << ", " << c << ")" << endl;
    ShiftLeft3(a, b, c);
    cout << "result set: " << endl;
    cout << "( " << a << ", " << b << ", " << c << ")" << endl;
}

int main() {
    task(); task();
}