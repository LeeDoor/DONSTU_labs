#include "Author.h"
#include <iostream>

Author::Author(const std::string& name, const std::string& bio)
    : name(name), biography(bio) {}

std::string Author::getName() const {
    return name;
}

std::string Author::getBiography() const {
    return biography;
}

void Author::displayInfo() const {
    std::cout << "Author: " << name << std::endl;
    std::cout << "Biography: " << biography << std::endl;
}
