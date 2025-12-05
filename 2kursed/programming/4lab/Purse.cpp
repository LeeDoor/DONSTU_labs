#include "Purse.hpp"
#include "Dollar.hpp"
#include "Euro.hpp"
#include "Pound.hpp"
#include <iostream>
#include <iomanip>
#include <map>

void Purse::addCurrency(std::unique_ptr<Currency> currency) {
    currencies.push_back(std::move(currency));
}

double Purse::getTotalInRubles() const {
    double total = 0.0;
    for (const auto& currency : currencies) {
        total += currency->toRubles();
    }
    return total;
}

void Purse::showAllCurrencies() const {
    std::cout << "All currencies in purse:" << std::endl;
    for (size_t i = 0; i < currencies.size(); ++i) {
        std::cout << i + 1 << ". ";
        currencies[i]->print();
        std::cout << " = " << std::fixed << std::setprecision(2) 
                  << currencies[i]->toRubles() << " RUB" << std::endl;
    }
}

void Purse::showSummaryByCurrency() const {
    std::map<std::string, double> summary;
    std::map<std::string, int> count;
    
    for (const auto& currency : currencies) {
        std::string code = currency->getCurrencyCode();
        summary[code] += currency->getAmount();
        count[code]++;
    }
    
    std::cout << "Summary by currency:" << std::endl;
    for (const auto& [code, amount] : summary) {
        std::cout << code << ": " << amount << " units ("
                  << count[code] << " entries)" << std::endl;
    }
}

size_t Purse::getCurrencyCount() const {
    return currencies.size();
}

void Purse::removeCurrency(size_t index) {
    if (index < currencies.size()) {
        currencies.erase(currencies.begin() + index);
    }
}
