#include "registration_database.hpp"
#include <iostream>

int main() {
    RegistrationDatabase database;

    VehicleRegistration reg1("A123BC77", "Ivanov Ivan Ivanovich", "4510 123456", "2023-01-15", "2028-01-15", "Moscow");
    VehicleRegistration reg2("B456DE78", "Petrov Petr Petrovich", "4511 654321", "2023-01-15", "2028-01-15", "Saint Petersburg");
    VehicleRegistration reg3("M789FG99", "Sidorov Alexey Vladimirovich", "4512 789012", "2023-02-20", "2028-02-20", "Moscow");

    database.addRegistration(reg1);
    database.addRegistration(reg2);
    database.addRegistration(reg3);

    database.displayAll();

    std::cout << "\n";
    database.findOwnersByIssueDate("2023-01-15");

    std::cout << "\n";
    database.findVehiclesByRegion("Moscow");

    return 0;
}
