#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>

class Author {
private:
    std::string name;
    std::string biography;

public:
    Author(const std::string& name, const std::string& bio);
    
    std::string getName() const;
    std::string getBiography() const;
    void displayInfo() const;
};

#endif
