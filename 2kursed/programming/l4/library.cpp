#include "Library.h"
#include <iostream>

void Library::addBook(Book* book) {
    books.push_back(book);
}

void Library::displayAllBooks() const {
    std::cout << "=== ALL BOOKS IN LIBRARY ===" << std::endl;
    for (const auto& book : books) {
        book->displayInfo();
    }
}

void Library::searchByAuthor(const std::string& authorName) const {
    std::cout << "=== BOOKS BY AUTHOR: " << authorName << " ===" << std::endl;
    bool found = false;
    for (const auto& book : books) {
        if (book->getAuthor()->getName() == authorName) {
            book->displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No books found by author: " << authorName << std::endl;
    }
}

void Library::searchByYear(int year) const {
    std::cout << "=== BOOKS FROM YEAR: " << year << " ===" << std::endl;
    bool found = false;
    for (const auto& book : books) {
        if (book->getPublicationYear() == year) {
            book->displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No books found from year: " << year << std::endl;
    }
}
