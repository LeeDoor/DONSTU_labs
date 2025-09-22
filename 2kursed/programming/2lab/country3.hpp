#ifndef COUNTRYFRIEND_H
#define COUNTRYFRIEND_H

class CountryFriend {
private:
    char* name;
    char* governmentForm;
    float area;
    long population;
    static int objectCount;

    CountryFriend(const char* n, const char* g, float a, long p);
    ~CountryFriend();

    friend CountryFriend* createCountryFriend(const char* n, const char* g, float a, long p);
    friend void destroyCountryFriend(CountryFriend* country);
    friend void displayFriendInfo(const CountryFriend& country);

public:
    void setPopulation(long p);
    long getPopulation() const;
    const char* getName() const;
    const char* getGovernmentForm() const;
    float getArea() const;
    
    void displayInfo() const;
    static int getObjectCount();
};

CountryFriend* createCountryFriend(const char* n, const char* g, float a, long p);
void destroyCountryFriend(CountryFriend* country);
void displayFriendInfo(const CountryFriend& country);

#endif
