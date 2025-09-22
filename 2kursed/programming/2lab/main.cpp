#include "teacher.hpp"

int main() {
    std::list<Student> students {
        {"Nikita"}, {"Serega"}, {"Ivan"}, {"OLEG"}
    };
    Teacher teacher;
    
    students.front().add_skips(5);
    students.back().add_skips(3);

    teacher.check_students(students);

}
