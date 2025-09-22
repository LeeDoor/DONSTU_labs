#ifndef COUNTRY_H
#define COUNTRY_H

class Country {
private:
    char* name;
    char* governmentForm;
    float area;
    long population;
    static int objectCount;

public:
    Country(const char* n, const char* g, float a, long p);
    Country(const Country& other);
    ~Country();
    
    void setPopulation(long p);
    long getPopulation() const;
    const char* getName() const;
    const char* getGovernmentForm() const;
    float getArea() const;
    
    void displayInfo() const;
    static int getObjectCount();
};

#endif
