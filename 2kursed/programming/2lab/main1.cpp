#include "country1.hpp"
#include <iostream>

int main() {
    std::cout << "Objects at start: " << Country::getObjectCount() << std::endl;
    
    Country countries[3] = {
        Country("USA", "Republic", 9834000, 331000000),
        Country("Germany", "Federal Republic", 357022, 83200000),
        Country("Japan", "Constitutional Monarchy", 377975, 125000000)
    };
    
    std::cout << "After static array: " << Country::getObjectCount() << std::endl;
    
    Country* dynamicCountry = new Country("France", "Republic", 643801, 68000000);
    std::cout << "After dynamic object: " << Country::getObjectCount() << std::endl;
    
    countries[0].setPopulation(332000000);
    countries[0].displayInfo();
    
    delete dynamicCountry;
    std::cout << "After deleting dynamic object: " << Country::getObjectCount() << std::endl;
    
    return 0;
}
