#include "vehicle_registration.hpp"
#include <iostream>

VehicleRegistration::VehicleRegistration() = default;

VehicleRegistration::VehicleRegistration(std::string number, std::string name, std::string passport, 
                       std::string issue, std::string expire, std::string region)
    : licenseNumber(std::move(number)), ownerName(std::move(name)), 
      passportData(std::move(passport)), issueDate(std::move(issue)), 
      expirationDate(std::move(expire)), registrationRegion(std::move(region)) {}

VehicleRegistration::VehicleRegistration(const VehicleRegistration& other)
    : licenseNumber(other.licenseNumber), ownerName(other.ownerName),
      passportData(other.passportData), issueDate(other.issueDate),
      expirationDate(other.expirationDate), registrationRegion(other.registrationRegion) {}

std::string VehicleRegistration::getLicenseNumber() const { return licenseNumber; }
std::string VehicleRegistration::getOwnerName() const { return ownerName; }
std::string VehicleRegistration::getPassportData() const { return passportData; }
std::string VehicleRegistration::getIssueDate() const { return issueDate; }
std::string VehicleRegistration::getExpirationDate() const { return expirationDate; }
std::string VehicleRegistration::getRegistrationRegion() const { return registrationRegion; }

void VehicleRegistration::setLicenseNumber(const std::string& number) { licenseNumber = number; }
void VehicleRegistration::setOwnerName(const std::string& name) { ownerName = name; }
void VehicleRegistration::setPassportData(const std::string& passport) { passportData = passport; }
void VehicleRegistration::setIssueDate(const std::string& issue) { issueDate = issue; }
void VehicleRegistration::setExpirationDate(const std::string& expire) { expirationDate = expire; }
void VehicleRegistration::setRegistrationRegion(const std::string& region) { registrationRegion = region; }

void VehicleRegistration::displayInfo() const {
    std::cout << "License Number: " << licenseNumber << "\n";
    std::cout << "Owner: " << ownerName << "\n";
    std::cout << "Passport: " << passportData << "\n";
    std::cout << "Issue Date: " << issueDate << "\n";
    std::cout << "Expiration Date: " << expirationDate << "\n";
    std::cout << "Region: " << registrationRegion << "\n";
    std::cout << "--------------------------\n";
}

bool VehicleRegistration::isIssueDateEqual(const std::string& date) const {
    return issueDate == date;
}

bool VehicleRegistration::isRegionEqual(const std::string& region) const {
    return registrationRegion == region;
}
