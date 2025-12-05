#include "Euro.hpp"
#include <iostream>

Euro::Euro(double amt) : Currency(amt) {}

double Euro::toRubles() const {
    return amount * 85.3;
}

void Euro::print() const {
    std::cout << "€" << amount << " (Euros)";
}

std::string Euro::getCurrencyCode() const {
    return "EUR";
}

int Euro::toCents() const {
    return static_cast<int>(amount * 100);
}

void Euro::exchangeRateAdjustment(double factor) {
    amount *= factor;
}
