#pragma once
#include "Vehicle.h"

class Bus : public Vehicle {
public:
    Bus(string i) : Vehicle(i) {}
    string getType() override { return "Bus"; }
};