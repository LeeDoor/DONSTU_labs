#include "vehicle_registration.hpp"
#include <ctime>

VehicleRegistration::VehicleRegistration() 
    : licenseNumber(""), ownerName(""), passportData(""), 
      issueDate(""), expiryDate(""), region("") {}

VehicleRegistration::VehicleRegistration(const std::string& num, const std::string& name, 
                       const std::string& passport, const std::string& issue, 
                       const std::string& expiry, const std::string& reg)
    : licenseNumber(num), ownerName(name), passportData(passport), 
      issueDate(issue), expiryDate(expiry), region(reg) {}

VehicleRegistration::VehicleRegistration(const VehicleRegistration& other)
    : licenseNumber(other.licenseNumber), ownerName(other.ownerName),
      passportData(other.passportData), issueDate(other.issueDate),
      expiryDate(other.expiryDate), region(other.region) {}

std::string VehicleRegistration::getLicenseNumber() const { return licenseNumber; }
std::string VehicleRegistration::getOwnerName() const { return ownerName; }
std::string VehicleRegistration::getPassportData() const { return passportData; }
std::string VehicleRegistration::getIssueDate() const { return issueDate; }
std::string VehicleRegistration::getExpiryDate() const { return expiryDate; }
std::string VehicleRegistration::getRegion() const { return region; }

void VehicleRegistration::setLicenseNumber(const std::string& num) { licenseNumber = num; }
void VehicleRegistration::setOwnerName(const std::string& name) { ownerName = name; }
void VehicleRegistration::setPassportData(const std::string& passport) { passportData = passport; }
void VehicleRegistration::setIssueDate(const std::string& issue) { issueDate = issue; }
void VehicleRegistration::setExpiryDate(const std::string& expiry) { expiryDate = expiry; }
void VehicleRegistration::setRegion(const std::string& reg) { region = reg; }

bool VehicleRegistration::isValid() const {
    return !licenseNumber.empty() && !ownerName.empty() && !expiryDate.empty();
}

int VehicleRegistration::daysUntilExpiry() const {
    return 365;
}

VehicleRegistration& VehicleRegistration::operator=(const VehicleRegistration& other) {
    if (this != &other) {
        licenseNumber = other.licenseNumber;
        ownerName = other.ownerName;
        passportData = other.passportData;
        issueDate = other.issueDate;
        expiryDate = other.expiryDate;
        region = other.region;
    }
    return *this;
}

bool VehicleRegistration::operator==(const VehicleRegistration& other) const {
    return licenseNumber == other.licenseNumber && passportData == other.passportData;
}

bool VehicleRegistration::operator!=(const VehicleRegistration& other) const {
    return !(*this == other);
}

bool VehicleRegistration::operator<(const VehicleRegistration& other) const {
    return expiryDate < other.expiryDate;
}

VehicleRegistration VehicleRegistration::operator+(int days) const {
    VehicleRegistration result = *this;
    return result;
}

VehicleRegistration VehicleRegistration::operator+(const std::string& newRegion) const {
    VehicleRegistration result = *this;
    result.region = newRegion;
    return result;
}

std::ostream& operator<<(std::ostream& os, const VehicleRegistration& reg) {
    os << "Number: " << reg.licenseNumber 
       << ", Owner: " << reg.ownerName 
       << ", Region: " << reg.region
       << ", issue date: " << reg.issueDate
       << ", expiry date: " << reg.expiryDate;
    return os;
}
