#!/bin/bash

# Создаем файл Currency.hpp
cat > Currency.hpp << 'EOF'
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
EOF

# Создаем файл Dollar.hpp
cat > Dollar.hpp << 'EOF'
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
EOF

# Создаем файл Euro.hpp
cat > Euro.hpp << 'EOF'
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
EOF

# Создаем файл Pound.hpp
cat > Pound.hpp << 'EOF'
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
EOF

# Создаем файл Purse.hpp
cat > Purse.hpp << 'EOF'
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
EOF

# Создаем файл Currency.cpp
cat > Currency.cpp << 'EOF'
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
EOF

# Создаем файл Dollar.cpp
cat > Dollar.cpp << 'EOF'
#include "Dollar.hpp"
#include <iostream>

Dollar::Dollar(double amt) : Currency(amt) {}

double Dollar::toRubles() const {
    return amount * 75.5;
}

void Dollar::print() const {
    std::cout << "$" << amount << " (Dollars)";
}

std::string Dollar::getCurrencyCode() const {
    return "USD";
}

int Dollar::toCents() const {
    return static_cast<int>(amount * 100);
}

void Dollar::addCents(int cents) {
    amount += cents / 100.0;
}
EOF

# Создаем файл Euro.cpp
cat > Euro.cpp << 'EOF'
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
EOF

# Создаем файл Pound.cpp
cat > Pound.cpp << 'EOF'
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
EOF

# Создаем файл Purse.cpp
cat > Purse.cpp << 'EOF'
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
EOF

# Создаем файл main.cpp
cat > main.cpp << 'EOF'
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
EOF

# Создаем файл Makefile
cat > Makefile << 'EOF'
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = Currency.cpp Dollar.cpp Euro.cpp Pound.cpp Purse.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = currency_program

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
EOF

# Даем права на выполнение скрипта и Makefile
chmod +x create_files.sh
chmod +x Makefile

echo "Все файлы созданы успешно!"
echo "Для компиляции программы выполните: make"
echo "Для запуска программы выполните: make run"
echo "Для очистки выполните: make clean"
