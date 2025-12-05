#pragma once

#include <string>

class Currency {
protected:
    double amount;
public:
    Currency(double amt);
    virtual ~Currency() = default;
    virtual double toRubles() const = 0;
    virtual void print() const;
    virtual std::string getCurrencyCode() const = 0;
    double getAmount() const;
    void setAmount(double amt);
};
