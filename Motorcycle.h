#pragma once
#include "Vehicle.h"
class Motorcycle : public Vehicle {
public:
    Motorcycle(std::string i) : Vehicle(i) {}
    std::string getType() override { return "Motorcycle"; }
};