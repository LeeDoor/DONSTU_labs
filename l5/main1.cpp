#include "methods1.h"

int main() {
    srand(time(NULL));
    int action;
    cout << "Welcome to MEGA VEDOMOST VPR" << endl;
    vector<Student> students;
    bool working = true;
    while (working){
        cout << "enter your action: " << endl << 
                "1) add new students" << endl << 
                "2) print student list" << endl << 
                "3) generate student list" << endl <<
                "4) print students with twos" << endl <<
                "[-1)] exit" << endl;
        action = getValidInput<int>();
        switch(action) {
            case 1:
                add_new_students(students);
                break;
            case 2:
                print_student_list(students);
                break;
            case 3:
                students = generate_student_list();
                break;
            case 4:
                print_students_with_twos(students);
                break;
            default:
                working = false;
                break;
        }
    };
}
