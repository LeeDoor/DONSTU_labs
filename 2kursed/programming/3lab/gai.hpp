#pragma once
#include "vehicle_registration.hpp"

#include <string>
class Gai {
public:
    Gai(std::string name) : name_{name}, penalty_size_{500} {}

    void SetPenaltySize(unsigned penalty) { penalty_size_ = penalty; }
    Gai& operator>> (VehicleRegistration& vehicle) {
        std::cout << "Gaishnik " << name_ << " penalized the driver " << vehicle.getOwnerName() << " for " << penalty_size_ << std::endl;
        return *this;
    }

private:
    std::string name_;
    unsigned penalty_size_;
};
