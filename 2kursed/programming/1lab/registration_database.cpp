#include "registration_database.hpp"
#include <iostream>

void RegistrationDatabase::addRegistration(const VehicleRegistration& registration) {
    registrations.push_back(registration);
}

void RegistrationDatabase::displayAll() const {
    std::cout << "All records in database:\n";
    for (const auto& reg : registrations) {
        reg.displayInfo();
    }
}

void RegistrationDatabase::findOwnersByIssueDate(const std::string& date) const {
    std::cout << "Owners with issue date " << date << ":\n";
    bool found = false;
    
    for (const auto& reg : registrations) {
        if (reg.isIssueDateEqual(date)) {
            std::cout << reg.getOwnerName() << "\n";
            found = true;
        }
    }
    
    if (!found) {
        std::cout << "No owners found with specified issue date\n";
    }
}

void RegistrationDatabase::findVehiclesByRegion(const std::string& region) const {
    std::cout << "Vehicles with region " << region << ":\n";
    bool found = false;
    
    for (const auto& reg : registrations) {
        if (reg.isRegionEqual(region)) {
            std::cout << reg.getLicenseNumber() << "\n";
            found = true;
        }
    }
    
    if (!found) {
        std::cout << "No vehicles found with specified region\n";
    }
}

size_t RegistrationDatabase::getCount() const {
    return registrations.size();
}
