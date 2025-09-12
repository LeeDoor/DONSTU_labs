#include <ctime>
#include <iostream>
#include <array>
#include <vector>
#include "../get_valid_input.hpp"
using namespace std;

enum Group {
    VPR11,
    VPR12,
    VPR13
};

using Score = unsigned short;
struct Student {
    string name;
    unsigned credit;
    Group group;
    array<Score, 3> scores;
};

void add_new_students(vector<Student>& students);
void print_student_list(const vector<Student>& students);
vector<Student> generate_student_list();
void print_students_with_twos(const vector<Student>& students);
