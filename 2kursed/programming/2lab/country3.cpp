#include "country3.hpp"
#include <cstring>
#include <iostream>

int CountryFriend::objectCount = 0;

CountryFriend::CountryFriend(const char* n, const char* g, float a, long p) {
    name = new char[strlen(n) + 1];
    strcpy(name, n);
    
    governmentForm = new char[strlen(g) + 1];
    strcpy(governmentForm, g);
    
    area = a;
    population = p;
    objectCount++;
}

CountryFriend::~CountryFriend() {
    delete[] name;
    delete[] governmentForm;
    objectCount--;
}

CountryFriend* createCountryFriend(const char* n, const char* g, float a, long p) {
    return new CountryFriend(n, g, a, p);
}

void destroyCountryFriend(CountryFriend* country) {
    delete country;
}

void CountryFriend::setPopulation(long p) {
    population = p;
}

long CountryFriend::getPopulation() const {
    return population;
}

const char* CountryFriend::getName() const {
    return name;
}

const char* CountryFriend::getGovernmentForm() const {
    return governmentForm;
}

float CountryFriend::getArea() const {
    return area;
}

void CountryFriend::displayInfo() const {
    std::cout << "Country: " << name << std::endl;
    std::cout << "Government: " << governmentForm << std::endl;
    std::cout << "Area: " << area << " sq km" << std::endl;
    std::cout << "Population: " << population << std::endl;
}

int CountryFriend::getObjectCount() {
    return objectCount;
}

void displayFriendInfo(const CountryFriend& country) {
    std::cout << "Friend function access: " << country.name << std::endl;
    std::cout << "Population: " << country.population << std::endl;
}
