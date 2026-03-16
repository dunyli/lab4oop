#pragma once
#include "Vehicle.h"
class Bus : public Vehicle {
public:
    Bus(std::string i) : Vehicle(i) {}
    std::string getType() override { return "Bus"; }
};