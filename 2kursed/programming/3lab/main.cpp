#include <iostream>
#include "gai.hpp"
#include "vehicle_registration.hpp"
#include "registration_database.hpp"

int main() {
    VehicleRegistration r;
    r.setOwnerName("Leonid");
    Gai gai("POLKOVNIK");
    gai.SetPenaltySize(228);
    gai >> r;
    return 0;
}
