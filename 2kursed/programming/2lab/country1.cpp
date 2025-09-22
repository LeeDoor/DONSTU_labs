#include "country1.hpp"
#include <cstring>
#include <iostream>

int Country::objectCount = 0;

Country::Country(const char* n, const char* g, float a, long p) {
    name = new char[strlen(n) + 1];
    strcpy(name, n);
    
    governmentForm = new char[strlen(g) + 1];
    strcpy(governmentForm, g);
    
    area = a;
    population = p;
    objectCount++;
}

Country::Country(const Country& other) {
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
    
    governmentForm = new char[strlen(other.governmentForm) + 1];
    strcpy(governmentForm, other.governmentForm);
    
    area = other.area;
    population = other.population;
    objectCount++;
}

Country::~Country() {
    delete[] name;
    delete[] governmentForm;
    objectCount--;
}

void Country::setPopulation(long p) {
    population = p;
}

long Country::getPopulation() const {
    return population;
}

const char* Country::getName() const {
    return name;
}

const char* Country::getGovernmentForm() const {
    return governmentForm;
}

float Country::getArea() const {
    return area;
}

void Country::displayInfo() const {
    std::cout << "Country: " << name << std::endl;
    std::cout << "Government: " << governmentForm << std::endl;
    std::cout << "Area: " << area << " sq km" << std::endl;
    std::cout << "Population: " << population << std::endl;
}

int Country::getObjectCount() {
    return objectCount;
}
