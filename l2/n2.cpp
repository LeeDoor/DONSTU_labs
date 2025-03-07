#include <iostream>
#include <ctime>
#include "../get_valid_input.hpp"
#define ARR_SIZE 10

int main () {
    srand(time(NULL));
    double arr[ARR_SIZE];
    for(int i = 0; i < ARR_SIZE; ++i) {
        *(arr + i) = (rand() % 20001 - 10000) / 100.0;
        cout << *(arr + i) << " ";
    }
    cout << endl;
    double C = getValidInput();
    int res = 0;
    for(int i = 0; i < ARR_SIZE; ++i) {
        res += *(arr + i) > C;
    }
    cout << "there are " << res << " elements more than " << C << endl;
} 