#include "Pound.hpp"
#include <iostream>

Pound::Pound(double amt) : Currency(amt) {}

double Pound::toRubles() const {
    return amount * 95.2;
}

void Pound::print() const {
    std::cout << "£" << amount << " (Pounds)";
}

std::string Pound::getCurrencyCode() const {
    return "GBP";
}

int Pound::toPence() const {
    return static_cast<int>(amount * 100);
}

void Pound::applyBrexitFee() {
    amount *= 0.99;
}
