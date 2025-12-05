#pragma once

#include "Currency.hpp"

class Euro : public Currency {
public:
    Euro(double amt);
    double toRubles() const override;
    void print() const override;
    std::string getCurrencyCode() const override;
    int toCents() const;
    void exchangeRateAdjustment(double factor);
};
