#include <iostream>
#include "Author.h"
#include "Book.h"
#include "Publisher.h"
#include "Library.h"

int main() {
    Author author1("J.K. Rowling", "British author best known for the Harry Potter series");
    Author author2("George Orwell", "English novelist and essayist");
    Author author3("J.R.R. Tolkien", "English writer and philologist");
    
    Book book1("Harry Potter and the Philosopher's Stone", &author1, "978-0439708180", 1997);
    Book book2("Harry Potter and the Chamber of Secrets", &author1, "978-0439064873", 1998);
    Book book3("1984", &author2, "978-0451524935", 1949);
    Book book4("Animal Farm", &author2, "978-0451526342", 1945);
    Book book5("The Hobbit", &author3, "978-0547928227", 1937);
    Book book6("The Lord of the Rings", &author3, "978-0544003415", 1954);
    
    Publisher publisher1("Bloomsbury Publishing");
    publisher1.addBook(&book1);
    publisher1.addBook(&book2);
    
    Publisher publisher2("Secker & Warburg");
    publisher2.addBook(&book3);
    publisher2.addBook(&book4);
    
    Publisher publisher3("George Allen & Unwin");
    publisher3.addBook(&book5);
    publisher3.addBook(&book6);
    
    Library library;
    library.addBook(&book1);
    library.addBook(&book2);
    library.addBook(&book3);
    library.addBook(&book4);
    library.addBook(&book5);
    library.addBook(&book6);
    
    std::cout << "=== ONLINE BOOK CATALOG DEMONSTRATION ===" << std::endl;
    std::cout << std::endl;
    
    library.displayAllBooks();
    std::cout << std::endl;
    
    library.searchByAuthor("J.K. Rowling");
    std::cout << std::endl;
    
    library.searchByYear(1949);
    std::cout << std::endl;
    
    library.searchByAuthor("Stephen King");
    std::cout << std::endl;
    
    library.searchByYear(2000);
    std::cout << std::endl;
    
    std::cout << "=== PUBLISHER INFORMATION ===" << std::endl;
    publisher1.displayAllBooks();
    std::cout << std::endl;
    publisher2.displayAllBooks();
    std::cout << std::endl;
    publisher3.displayAllBooks();
    
    return 0;
}
