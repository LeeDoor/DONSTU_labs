#include <iostream>
#include <string>
#include "../get_valid_input.hpp"

int main() {
    double mn = DBL_MAX;
    int idx = 0;
    for(int i = 1; i <= 4; ++i){
        double cur = getValidInput();
        if(mn > cur) {
            mn = cur;
            idx = i;
        }
    }
    cout << "minimum number is at " << idx << ". it was " << mn << endl;
}