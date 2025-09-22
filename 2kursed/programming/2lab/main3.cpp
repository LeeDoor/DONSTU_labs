#include "country3.hpp"
#include <iostream>

int main() {
    std::cout << "Objects at start: " << CountryFriend::getObjectCount() << std::endl;
    
    CountryFriend* country = createCountryFriend("Brazil", "Republic", 8516000, 213000000);
    
    std::cout << "After creating object: " << CountryFriend::getObjectCount() << std::endl;
    
    country->setPopulation(215000000);
    country->displayInfo();

    displayFriendInfo(*country);
    
    destroyCountryFriend(country);
    
    std::cout << "After destroying object: " << CountryFriend::getObjectCount() << std::endl;
    
    return 0;
}
