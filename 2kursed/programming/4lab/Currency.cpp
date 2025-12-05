#include "Currency.hpp"
#include <iostream>

Currency::Currency(double amt) : amount(amt) {}

void Currency::print() const {
    std::cout << amount << " " << getCurrencyCode();
}

double Currency::getAmount() const {
    return amount;
}

void Currency::setAmount(double amt) {
    amount = amt;
}
