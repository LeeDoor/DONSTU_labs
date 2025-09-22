#pragma once
#include "vehicle_registration.hpp"
#include <vector>

class RegistrationDatabase {
private:
    std::vector<VehicleRegistration> registrations;

public:
    void addRegistration(const VehicleRegistration& registration);
    void displayAll() const;
    void findOwnersByIssueDate(const std::string& date) const;
    void findVehiclesByRegion(const std::string& region) const;
    size_t getCount() const;
};
