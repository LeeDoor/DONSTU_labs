#include "Purse.hpp"
#include "Dollar.hpp"
#include "Euro.hpp"
#include "Pound.hpp"
#include <iostream>
#include <memory>

int main() {
    Purse myPurse;
    
    myPurse.addCurrency(std::make_unique<Dollar>(100.50));
    myPurse.addCurrency(std::make_unique<Euro>(200.75));
    myPurse.addCurrency(std::make_unique<Pound>(50.25));
    myPurse.addCurrency(std::make_unique<Dollar>(25.0));
    myPurse.addCurrency(std::make_unique<Euro>(75.5));
    
    std::cout << "=== Currency Purse Demo ===" << std::endl;
    std::cout << std::endl;
    
    myPurse.showAllCurrencies();
    std::cout << std::endl;
    
    myPurse.showSummaryByCurrency();
    std::cout << std::endl;
    
    std::cout << "Total in RUB: " << myPurse.getTotalInRubles() << std::endl;
    std::cout << std::endl;
    
    std::cout << "=== Currency Specific Operations ===" << std::endl;
    
    Dollar dollar(50.75);
    std::cout << "Dollar amount: ";
    dollar.print();
    std::cout << std::endl;
    std::cout << "In cents: " << dollar.toCents() << std::endl;
    dollar.addCents(50);
    std::cout << "After adding 50 cents: ";
    dollar.print();
    std::cout << std::endl;
    
    Euro euro(100.0);
    std::cout << "Euro amount: ";
    euro.print();
    std::cout << std::endl;
    euro.exchangeRateAdjustment(1.05);
    std::cout << "After 5% adjustment: ";
    euro.print();
    std::cout << std::endl;
    
    Pound pound(80.0);
    std::cout << "Pound amount: ";
    pound.print();
    std::cout << std::endl;
    pound.applyBrexitFee();
    std::cout << "After Brexit fee: ";
    pound.print();
    std::cout << std::endl;
    
    std::cout << std::endl;
    std::cout << "Total currencies in purse: " << myPurse.getCurrencyCount() << std::endl;
    
    return 0;
}
