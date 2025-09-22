#pragma once
#include <string>

class VehicleRegistration {
private:
    std::string licenseNumber;
    std::string ownerName;
    std::string passportData;
    std::string issueDate;
    std::string expirationDate;
    std::string registrationRegion;

public:
    VehicleRegistration();
    VehicleRegistration(std::string number, std::string name, std::string passport, 
                       std::string issue, std::string expire, std::string region);
    VehicleRegistration(const VehicleRegistration& other);

    std::string getLicenseNumber() const;
    std::string getOwnerName() const;
    std::string getPassportData() const;
    std::string getIssueDate() const;
    std::string getExpirationDate() const;
    std::string getRegistrationRegion() const;

    void setLicenseNumber(const std::string& number);
    void setOwnerName(const std::string& name);
    void setPassportData(const std::string& passport);
    void setIssueDate(const std::string& issue);
    void setExpirationDate(const std::string& expire);
    void setRegistrationRegion(const std::string& region);

    void displayInfo() const;
    bool isIssueDateEqual(const std::string& date) const;
    bool isRegionEqual(const std::string& region) const;
};
