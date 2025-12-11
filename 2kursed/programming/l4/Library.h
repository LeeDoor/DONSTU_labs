#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "Book.h"

class Library {
private:
    std::vector<Book*> books;

public:
    void addBook(Book* book);
    void displayAllBooks() const;
    void searchByAuthor(const std::string& authorName) const;
    void searchByYear(int year) const;
};

#endif
