#pragma once
#include "Vehicle.h"
class Truck : public Vehicle {
public:
    Truck(std::string i) : Vehicle(i) {}
    std::string getType() override { return "Truck"; }
};