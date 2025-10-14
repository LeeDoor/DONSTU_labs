#pragma once
#include <string>
#include <iostream>

class VehicleRegistration {
private:
    std::string licenseNumber;
    std::string ownerName;
    std::string passportData;
    std::string issueDate;
    std::string expiryDate;
    std::string region;

public:
    VehicleRegistration();
    VehicleRegistration(const std::string& num, const std::string& name, 
                       const std::string& passport, const std::string& issue, 
                       const std::string& expiry, const std::string& reg);
    VehicleRegistration(const VehicleRegistration& other);
    
    std::string getLicenseNumber() const;
    std::string getOwnerName() const;
    std::string getPassportData() const;
    std::string getIssueDate() const;
    std::string getExpiryDate() const;
    std::string getRegion() const;
    
    void setLicenseNumber(const std::string& num);
    void setOwnerName(const std::string& name);
    void setPassportData(const std::string& passport);
    void setIssueDate(const std::string& issue);
    void setExpiryDate(const std::string& expiry);
    void setRegion(const std::string& reg);
    
    bool isValid() const;
    int daysUntilExpiry() const;
    
    VehicleRegistration& operator=(const VehicleRegistration& other);
    bool operator==(const VehicleRegistration& other) const;
    bool operator!=(const VehicleRegistration& other) const;
    bool operator<(const VehicleRegistration& other) const;
    VehicleRegistration operator+(int days) const;
    VehicleRegistration operator+(const std::string& newRegion) const;
    
    friend std::ostream& operator<<(std::ostream& os, const VehicleRegistration& reg);
};

