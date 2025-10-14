#pragma once
#include "vehicle_registration.hpp"
#include <vector>

class RegistrationDatabase {
private:
    std::vector<VehicleRegistration> registrations;

public:
    RegistrationDatabase();
    void addRegistration(const VehicleRegistration& registration);
    void removeRegistration(const std::string& licenseNumber);
    std::vector<std::string> getOwnersByIssueDate(const std::string& date) const;
    std::vector<std::string> getVehiclesByRegion(const std::string& region) const;
    void displayAll() const;
    
    VehicleRegistration& operator[](size_t index);
    const VehicleRegistration& operator[](size_t index) const;
    size_t size() const;
};

