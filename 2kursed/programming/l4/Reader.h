#pragma once
#include "Book.h"
#include <string>
#include <vector>

class ReaderTicket {
public:
    ReaderTicket(std::string name) : name_{name} {}

private:
    std::string name_;
};

class Reader {
public:
    Reader(std::string name) :
        books{},
        ticket{new ReaderTicket(name)}
    {}
    ~Reader() {
        delete ticket;
    }
    void read_book(Book* book) {
        books.push_back(book);
    }
private:
    std::vector<Book*> books;
    ReaderTicket* ticket;
};
