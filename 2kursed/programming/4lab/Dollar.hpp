#pragma once
#include "Currency.hpp"

class Dollar : public Currency {
public:
    Dollar(double amt);
    double toRubles() const override;
    void print() const override;
    std::string getCurrencyCode() const override;
    int toCents() const;
    void addCents(int cents);
};
