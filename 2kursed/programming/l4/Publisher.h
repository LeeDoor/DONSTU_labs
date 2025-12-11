#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <vector>
#include "Book.h"

class Publisher {
private:
    std::string name;
    std::vector<Book*> books;

public:
    Publisher(const std::string& name);
    
    void addBook(Book* book);
    void displayAllBooks() const;
    std::string getName() const;
};

#endif
