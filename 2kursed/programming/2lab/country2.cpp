#include "country2.hpp"
#include <cstring>
#include <iostream>

int CountryPrivate::objectCount = 0;

CountryPrivate::CountryPrivate(const char* n, const char* g, float a, long p) {
    name = new char[strlen(n) + 1];
    strcpy(name, n);
    
    governmentForm = new char[strlen(g) + 1];
    strcpy(governmentForm, g);
    
    area = a;
    population = p;
    objectCount++;
}

CountryPrivate::~CountryPrivate() {
    delete[] name;
    delete[] governmentForm;
    objectCount--;
}

CountryPrivate* CountryPrivate::createCountry(const char* n, const char* g, float a, long p) {
    return new CountryPrivate(n, g, a, p);
}

void CountryPrivate::destroyCountry(CountryPrivate* country) {
    delete country;
}

void CountryPrivate::setPopulation(long p) {
    population = p;
}

long CountryPrivate::getPopulation() const {
    return population;
}

const char* CountryPrivate::getName() const {
    return name;
}

const char* CountryPrivate::getGovernmentForm() const {
    return governmentForm;
}

float CountryPrivate::getArea() const {
    return area;
}

void CountryPrivate::displayInfo() const {
    std::cout << "Country: " << name << std::endl;
    std::cout << "Government: " << governmentForm << std::endl;
    std::cout << "Area: " << area << " sq km" << std::endl;
    std::cout << "Population: " << population << std::endl;
}

int CountryPrivate::getObjectCount() {
    return objectCount;
}
