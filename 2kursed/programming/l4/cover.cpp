#include "Cover.h"
#include <iostream>

Cover::Cover(const std::string& type, const std::string& design)
    : type(type), design(design) {}

std::string Cover::getType() const {
    return type;
}

std::string Cover::getDesign() const {
    return design;
}

void Cover::displayInfo() const {
    std::cout << "Cover Type: " << type << std::endl;
    std::cout << "Cover Design: " << design << std::endl;
}
