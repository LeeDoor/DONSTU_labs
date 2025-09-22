#ifndef COUNTRYPRIVATE_H
#define COUNTRYPRIVATE_H

class CountryPrivate {
private:
    char* name;
    char* governmentForm;
    float area;
    long population;
    static int objectCount;

    CountryPrivate(const char* n, const char* g, float a, long p);
    ~CountryPrivate();

public:
    static CountryPrivate* createCountry(const char* n, const char* g, float a, long p);
    static void destroyCountry(CountryPrivate* country);
    
    void setPopulation(long p);
    long getPopulation() const;
    const char* getName() const;
    const char* getGovernmentForm() const;
    float getArea() const;
    
    void displayInfo() const;
    static int getObjectCount();
};

#endif
