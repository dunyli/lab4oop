#pragma once
#include "Vehicle.h"
class PassengerCar : public Vehicle {
public:
    PassengerCar(std::string i) : Vehicle(i) {}
    std::string getType() override { return "PassengerCar"; }
};