#ifndef BOOK_H
#define BOOK_H

#include <string>
#include "Author.h"
#include "Cover.h"

class Book {
private:
    std::string title;
    Author* author;
    std::string isbn;
    int publicationYear;
    Cover* cover;

public:
    Book(const std::string& title, Author* author, const std::string& isbn, int year);
    ~Book();
    
    std::string getTitle() const;
    Author* getAuthor() const;
    std::string getISBN() const;
    int getPublicationYear() const;
    Cover* getCover() const;
    
    void displayInfo() const;
};

#endif
