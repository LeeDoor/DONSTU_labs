#include "Book.h"
#include <iostream>

Book::Book(const std::string& title, Author* author, const std::string& isbn, int year)
    : title(title), author(author), isbn(isbn), publicationYear(year) {
    cover = new Cover("Hardcover", "Blue with gold lettering");
}

Book::~Book() {
    delete cover;
}

std::string Book::getTitle() const {
    return title;
}

Author* Book::getAuthor() const {
    return author;
}

std::string Book::getISBN() const {
    return isbn;
}

int Book::getPublicationYear() const {
    return publicationYear;
}

Cover* Book::getCover() const {
    return cover;
}

void Book::displayInfo() const {
    std::cout << "Title: " << title << std::endl;
    std::cout << "Author: " << author->getName() << std::endl;
    std::cout << "ISBN: " << isbn << std::endl;
    std::cout << "Year: " << publicationYear << std::endl;
    cover->displayInfo();
    std::cout << "------------------------" << std::endl;
}
