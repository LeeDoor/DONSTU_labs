#include "student.hpp"
#include <iostream>
#include <list>

class Teacher {
public:
    void check_students(std::list<Student>& students) {
        for(auto iter = students.begin(); iter != students.end(); ++iter) {
            auto student = *iter;
            if(student.getSkips() > 2) {
                std::cout << "removing student " << student.getName() << std::endl;
                ++iter;
                students.erase(std::prev(iter));
                --iter;
            }
        }
    }
};
