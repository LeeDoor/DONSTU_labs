#include "country2.hpp"
#include <iostream>

int main() {
    std::cout << "Objects at start: " << CountryPrivate::getObjectCount() << std::endl;
    
    CountryPrivate* country1 = CountryPrivate::createCountry("Canada", "Monarchy", 9985000, 38000000);
    CountryPrivate* country2 = CountryPrivate::createCountry("Australia", "Monarchy", 7692000, 25600000);
    
    std::cout << "After creating objects: " << CountryPrivate::getObjectCount() << std::endl;
    
    country1->setPopulation(38500000);
    country1->displayInfo();
    country2->displayInfo();
    
    CountryPrivate::destroyCountry(country1);
    CountryPrivate::destroyCountry(country2);
    
    std::cout << "After destroying objects: " << CountryPrivate::getObjectCount() << std::endl;
    
    return 0;
}
