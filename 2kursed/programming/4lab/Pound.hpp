#pragma once

#include "Currency.hpp"

class Pound : public Currency {
public:
    Pound(double amt);
    double toRubles() const override;
    void print() const override;
    std::string getCurrencyCode() const override;
    int toPence() const;
    void applyBrexitFee();
};
