#include "Dollar.hpp"
#include <iostream>

Dollar::Dollar(double amt) : Currency(amt) {}

double Dollar::toRubles() const {
    return amount * 75.5;
}

void Dollar::print() const {
    std::cout << "$" << amount << " (Dollars)";
}

std::string Dollar::getCurrencyCode() const {
    return "USD";
}

int Dollar::toCents() const {
    return static_cast<int>(amount * 100);
}

void Dollar::addCents(int cents) {
    amount += cents / 100.0;
}
