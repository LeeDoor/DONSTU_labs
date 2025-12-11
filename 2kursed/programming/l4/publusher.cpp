#include "Publisher.h"
#include <iostream>

Publisher::Publisher(const std::string& name) : name(name) {}

void Publisher::addBook(Book* book) {
    books.push_back(book);
}

void Publisher::displayAllBooks() const {
    std::cout << "Publisher: " << name << std::endl;
    std::cout << "Books published:" << std::endl;
    for (const auto& book : books) {
        book->displayInfo();
    }
}

std::string Publisher::getName() const {
    return name;
}
