#pragma once
#include "Vehicle.h"

class PassengerCar : public Vehicle {
public:
    PassengerCar(string i) : Vehicle(i) {}
    string getType() override { return "PassengerCar"; }
};