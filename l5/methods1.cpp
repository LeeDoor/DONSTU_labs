#include "methods1.h"
#include <iomanip>

void print_table_header() {
    cout << left << setw(20) << "name" 
        << setw(10) << "credit" 
        << setw(8) << "group" 
        << setw(4) << "e1" 
        << setw(4) << "e2"
        << setw(4) << "e3" << endl;

}
void print_group(const Group& group) {
    switch(group) {
        case Group::VPR11:
            cout << left << setw(8) << "VPR11";
            break;
        case Group::VPR12:
           cout << left << setw(8) << "VPR12";
            break;
        case Group::VPR13:
            cout << left << setw(8) << "VPR13";
            break;
    }
}
void print_student(const Student& student) {
    cout << left
        << setw(20) << student.name
        << setw(10) << student.credit;
    print_group(student.group);
    for(const Score& sc : student.scores) {
        cout << left << setw(4) << sc;
    }
    cout << endl;
}
void add_new_students(vector<Student>& students) {
    while(true) {
        cout << "enter the name of the student (-1 to exit): ";
        Student student{};
        cin >> student.name;
        if(student.name == "-1"){
            return;
        }
        int input;
        do {
            cout << "enter credit (should be natural number): ";
            input = getValidInput<int>();
        } while(input <= 0);
        student.credit = input;
        string group_str;
        cout << "enter group name: ";
        bool keep = false;
        do {
            keep = false;
            cin >> group_str;
            if(group_str == "VPR11") {
                student.group = Group::VPR11;
            } else if (group_str == "VPR12") {
                student.group = Group::VPR12;
            } else if (group_str == "VPR13") {
                student.group = Group::VPR13;
            } else {
                keep = true;
                cout << "invalid group. enter VPR11, VPR12 or VPR13: ";
            }
        } while(keep);
        for(int i = 0; i < student.scores.size(); ++i) {
            cout << "enter " << i + 1 << " score: ";
            int score = getValidInput<int>();
            if(score < 0 || score > 5 || score == 1) {
                cout << "invalid score. try again" << endl;
                --i;
                continue;
            }
            student.scores[i] = score;
        }
        students.push_back(student);
        cout << "student added." <<endl;
    }
}
void print_student_list(const vector<Student>& students) {
    print_table_header();
    for(const Student& st : students) {
        print_student(st);
    }
}
vector<Student> generate_student_list() {
    vector<Student> students(10);
    static const array<string, 5> names {
        "Boriska", "Egorka", "Leonid", "Asya", "Katherine"
    };
    for(int i = 0; i < 10; ++i) {
        Student student;
        student.name = names[rand() % 5];
        student.credit = (rand() % 1000000) + 100000;
        student.group = static_cast<Group>(rand() % 3);
        for(Score& sc : student.scores) {
            sc = rand() % 5;
            if(sc == 1) sc = 5;
        }
        students[i] = student;
    }
    return students;
}
void print_students_with_twos(const vector<Student>& students) {
    int counter = 0;
    for(const Student& student : students) {
        bool noobik = false;
        for(const Score& score : student.scores) {
            if(score == 2) {
                noobik = true;
                break;
            }
        }
        if(noobik) {
            if(counter == 0) {
                print_table_header();
            }
            ++counter;
            print_student(student);
        }
    }
    if(counter) {
        cout << "total lohs: " << counter << endl;
    } else {
        cout << "no lohs in the group." << endl;
    }
}

