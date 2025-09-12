#include <iostream>
#include <cmath>
#include <ctime>
#include "../get_valid_input.hpp"

double GetMaxAbs(const double* arr, int n) {
    double maxabs = fabs(arr[0]);
    for(int i = 1; i < n; ++i) {
        maxabs = max(maxabs, fabs(arr[i]));
    }
    cout << "max abs value: " << maxabs << endl;
}
double GetSumAfterZero(const double* arr, int n) {
    int i = 0;
    while (i < n && arr[i] != 0) ++i;
    double sum = 0;
    for(++i;  i < n; ++i) {
        sum += fabs(arr[i]);
    }
    cout << "sum of absolute numbers after 0: " << sum << endl;
}

void ShiftZeros(double *arr, int n) {
    int i = 0;
    for(int j = 0; j < n; ++j) {
        if (arr[j]) {
            arr[i++] = arr[j];
        }
    }
    for(; i < n; ++i) arr[i] = 0;
}

int main() {
    srand(time(NULL));
    cout << "enter N: ";
    int n = getValidInput<int>();
    double *arr = new double[n];

    cout << "initial array: ";
    for(int i = 0; i < n; ++i){
        if(rand() % 3 != 0) 
            arr[i] = (rand() % 2000 - 1000) / 10.0;
        else arr[i] = 0;
        cout << arr[i] << " ";
    }
    cout << endl;
    GetMaxAbs(arr, n);
    GetSumAfterZero(arr, n);
    ShiftZeros(arr, n);
    cout << "shifted zeros: " << endl;
    for(int i = 0; i < n; ++i){
        cout << arr[i] << " ";
    }
    cout << endl;
}