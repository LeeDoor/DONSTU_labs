#include "registration_database.hpp"
#include <algorithm>

RegistrationDatabase::RegistrationDatabase() {}

void RegistrationDatabase::addRegistration(const VehicleRegistration& registration) {
    registrations.push_back(registration);
}

void RegistrationDatabase::removeRegistration(const std::string& licenseNumber) {
    registrations.erase(
        std::remove_if(registrations.begin(), registrations.end(),
            [&](const VehicleRegistration& reg) {
                return reg.getLicenseNumber() == licenseNumber;
            }),
        registrations.end()
    );
}

std::vector<std::string> RegistrationDatabase::getOwnersByIssueDate(const std::string& date) const {
    std::vector<std::string> owners;
    for (const auto& reg : registrations) {
        if (reg.getIssueDate() == date) {
            owners.push_back(reg.getOwnerName());
        }
    }
    return owners;
}

std::vector<std::string> RegistrationDatabase::getVehiclesByRegion(const std::string& region) const {
    std::vector<std::string> vehicles;
    for (const auto& reg : registrations) {
        if (reg.getRegion() == region) {
            vehicles.push_back(reg.getLicenseNumber());
        }
    }
    return vehicles;
}

void RegistrationDatabase::displayAll() const {
    for (const auto& reg : registrations) {
        std::cout << reg << std::endl;
    }
}

VehicleRegistration& RegistrationDatabase::operator[](size_t index) {
    return registrations[index];
}

const VehicleRegistration& RegistrationDatabase::operator[](size_t index) const {
    return registrations[index];
}

size_t RegistrationDatabase::size() const {
    return registrations.size();
}
