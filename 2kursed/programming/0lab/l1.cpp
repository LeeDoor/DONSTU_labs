#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "get_valid_input.hpp"

struct Employee {
    std::string first_name;
    std::string last_name;
    int age;
    std::string position;
    double salary;
    int truancy;
};

const std::string filename = "empbuf.txt";

std::vector<Employee> read_from_file() {
    std::vector<Employee> employees;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Could not open file for reading. Starting with empty list\n";
        return employees;
    }
    Employee emp;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        if (iss >> emp.first_name >> emp.last_name >> emp.age >> emp.position >> emp.salary >> emp.truancy) {
            employees.push_back(emp);
        }
    }
    return employees;
}

void write_to_file(const std::vector<Employee>& emps) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file for writing\n";
        return;
    }
    for (const auto& emp : emps) {
        file << emp.first_name << " " << emp.last_name << " " 
             << emp.age << " " << emp.position << " " 
             << emp.salary << " " << emp.truancy << "\n";
    }
    file.close();
    std::cout << "Data successfully written to file\n";
}

void show_employees(const std::vector<Employee>& emps) {
    if (emps.empty()) {
        std::cout << "No employees in the system.\n";
        return;
    }
    std::cout << "\n=== Employee List ===\n";
    for (size_t i = 0; i < emps.size(); ++i) {
        const auto& emp = emps[i];
        std::cout << i + 1 << ") " << emp.first_name << " " << emp.last_name
                  << ", Age: " << emp.age
                  << ", Position: " << emp.position
                  << ", Salary: $" << std::fixed << std::setprecision(2) << emp.salary 
                  << ", Truancies: "<< emp.truancy << "\n";
    }
    std::cout << "=====================\n\n";
}

Employee get_emp_from_cin() {
    Employee emp;
    
    std::cout << "Enter first name: ";
    std::cin >> emp.first_name;
    
    std::cout << "Enter last name: ";
    std::cin >> emp.last_name;
    
    std::cout << "Enter age: ";
    emp.age = getValidInput();
    
    std::cout << "Enter position: ";
    std::cin.ignore(); 
    std::getline(std::cin, emp.position);
    
    std::cout << "Enter salary: ";
    emp.salary = getValidInput<double>();
    
    emp.truancy = 0;
    return emp;
}

void add_employee(std::vector<Employee>& emps, const Employee& emp) {
    emps.push_back(emp);
    std::cout << "Employee added successfully.\n";
}

void remove_employee(std::vector<Employee>& emps, int index) {
    index -= 1;
    if (index < 0 || index >= static_cast<int>(emps.size())) {
        std::cout << "Invalid employee number.\n";
        return;
    }
    
    const auto& emp = emps[index];
    std::cout << "Removing employee: " << emp.first_name << " " << emp.last_name << "\n";
    emps.erase(emps.begin() + index);
    std::cout << "Employee removed successfully.\n";
}

void add_truancy(std::vector<Employee>& emps, int employee_id) {
    employee_id -= 1;
    if (employee_id < 0 || employee_id >= static_cast<int>(emps.size())) {
        std::cout << "Invalid employee ID.\n";
        return;
    }
    Employee& emp = emps[employee_id];
    ++emp.truancy;
    std::cout << "Truancy added to " << emp.first_name << " " << emp.last_name 
              << ". Total truancies: " << emp.truancy<< "\n";
    if (emp.truancy > 2) {
        std::cout << "WARNING: " << emp.first_name << " " << emp.last_name 
                  << " has " << emp.truancy << " truancies. Firing...\n";
        std::string fired_name = emp.first_name + " " + emp.last_name;
        emps.erase(emps.begin() + employee_id);
        std::cout << fired_name << " has been fired.\n";
    }
}

int main() {
    bool running = true;
    std::vector<Employee> emps = read_from_file();
    
    while (running) {
        std::cout 
            << "\nEmployee Manager\n"
            "1) Show list\n"
            "2) Hire Employee\n"
            "3) Fire Employee\n"
            "4) Write to file\n"
            "5) Set truancy\n"
            "0) exit\n"
            "Enter your choice [0-4]> ";
        
        int choice = -1;
        choice = getValidInput();
        
        switch(choice) {
            case 0:
                running = false;
                break;
            case 1:
                show_employees(emps);
                break;
            case 2:
                add_employee(emps, get_emp_from_cin());
                break;
            case 3:
                std::cout << "Enter employee number to remove: ";
                remove_employee(emps, getValidInput());
                break;
            case 4:
                write_to_file(emps);
                break;  
            case 5:
                std::cout << "Select an employee who skipped the workday>";
                add_truancy(emps, getValidInput());
        }
    }
}
