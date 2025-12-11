#include "lib.hpp"
#include <iostream>
#include <thread>

int main() {
    while(true) {
        std::cout << func() << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
