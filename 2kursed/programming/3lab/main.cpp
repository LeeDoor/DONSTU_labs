#include <iostream>
#include "vehicle_registration.hpp"
#include "registration_database.hpp"

int main() {
    RegistrationDatabase db;
    
    VehicleRegistration reg1("A123BC", "Ivanov Ivan Ivanovich", "4500 123456", "2023-01-15", "2028-01-15", "77");
    VehicleRegistration reg2("B456DE", "Petrov Petr Petrovich", "4500 654321", "2023-01-15", "2027-06-20", "50");
    VehicleRegistration reg3("C789FG", "Sidorov Alexey Viktorovich", "4600 111111", "2023-03-10", "2028-03-10", "77");
    
    db.addRegistration(reg1);
    db.addRegistration(reg2);
    db.addRegistration(reg3);
    
    std::cout << "All registrations:" << std::endl;
    db.displayAll();
    std::cout << std::endl;
    
    std::cout << "Owners with issue date 2023-01-15:" << std::endl;
    auto owners = db.getOwnersByIssueDate("2023-01-15");
    for (const auto& owner : owners) {
        std::cout << owner << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "Vehicles in region 77:" << std::endl;
    auto vehicles = db.getVehiclesByRegion("77");
    for (const auto& vehicle : vehicles) {
        std::cout << vehicle << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "Indexing operation:" << std::endl;
    std::cout << "First record: " << db[0] << std::endl;
    std::cout << std::endl;
    
    std::cout << "Assignment operation:" << std::endl;
    VehicleRegistration reg4 = reg1;
    std::cout << "Copied record: " << reg4 << std::endl;
    std::cout << std::endl;
    
    std::cout << "Comparison operations:" << std::endl;
    std::cout << "reg1 == reg4: " << (reg1 == reg4) << std::endl;
    std::cout << "reg1 != reg2: " << (reg1 != reg2) << std::endl;
    std::cout << "reg1 < reg2: " << (reg1 < reg2) << std::endl;
    std::cout << std::endl;
    
    std::cout << "Arithmetic operations:" << std::endl;
    VehicleRegistration reg5 = reg1 + "99";
    std::cout << "Region changed: " << reg5 << std::endl;
    std::cout << std::endl;
    
    std::cout << "Using setters and getters:" << std::endl;
    VehicleRegistration reg6;
    reg6.setLicenseNumber("X999YY");
    reg6.setOwnerName("Smith John");
    reg6.setRegion("01");
    std::cout << "New registration: " << reg6.getLicenseNumber() << ", " 
              << reg6.getOwnerName() << ", " << reg6.getRegion() << std::endl;
    
    return 0;
}
