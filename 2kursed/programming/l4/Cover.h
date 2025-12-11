#ifndef COVER_H
#define COVER_H

#include <string>

class Cover {
private:
    std::string type;
    std::string design;

public:
    Cover(const std::string& type, const std::string& design);
    
    std::string getType() const;
    std::string getDesign() const;
    void displayInfo() const;
};

#endif
