#pragma once
#include "Currency.hpp"
#include <iostream>

class Dodeper {
public:
    virtual void dodep(Currency* dodep) = 0;
};

class Bazovichok : public Dodeper{
public:
    void dodep(Currency* dodep) override {
        std::cout << "+";
        dodep->print();
        std::cout << " Babki EZ\n";
    }
};

class Loshara : public Dodeper {
public:
    void dodep(Currency* dodep) override {
        std::cout << "Opyat obocraly na ";
        dodep->print();
        std::cout << "Dx\n";
    }
};
