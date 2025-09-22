
#include <string>
class Student {
public:
    Student(std::string name_)
    : name{name_}
    , skips{0}
{}
    int getSkips() const {
        return skips;
    }
    std::string getName() const {
        return name;
    }
    void add_skips(unsigned to_add) {
        skips += to_add;
    }
private:
    std::string name;
    int skips;
};
