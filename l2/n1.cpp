#include <iostream>
#include <ctime>
#include "../get_valid_input.hpp"
using namespace std;
#define ARRAY_SIZE 10

void printArray(int arr[ARRAY_SIZE]) {
    for(int i = 0; i < ARRAY_SIZE; ++i){
        cout << *(arr + i) << " ";
    }
    cout << endl;
}

int main() {
    srand(time(NULL));
    int arr[ARRAY_SIZE];
    for(int i = 0; i < ARRAY_SIZE; ++i){
        *(arr + i) = rand() % 201 - 100;
    }
    cout << "initial array: " << endl;
    printArray(arr);
    cout << "even numbers: " << endl;
    for(int i = 0; i < ARRAY_SIZE; ++i) {
        int el = *(arr + i);
        if(el % 2 == 0) {
            cout << el << " ";
        }
    }
    cout << endl;
    cout << "odd numbers (v protivopolojnom poradke): " << endl;
    for(int i = ARRAY_SIZE - 1; i >= 0; --i){
        int el = *(arr + i);
        if(el % 2 != 0) {
            cout << el << " ";
        }
    }
    cout << endl;
}