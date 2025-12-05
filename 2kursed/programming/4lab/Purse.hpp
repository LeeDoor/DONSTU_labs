#pragma once

#include <vector>
#include <memory>
#include "Currency.hpp"

class Purse {
private:
    std::vector<std::unique_ptr<Currency>> currencies;
public:
    void addCurrency(std::unique_ptr<Currency> currency);
    double getTotalInRubles() const;
    void showAllCurrencies() const;
    void showSummaryByCurrency() const;
    size_t getCurrencyCount() const;
    void removeCurrency(size_t index);
};
