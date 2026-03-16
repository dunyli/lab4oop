#pragma once
#include <string>
#include "LicensePlate.h"
class Vehicle {
public:
    std::string id;
    int speed;
    int lane;
    LicensePlate* plate;
    Vehicle(std::string i) : id(i), speed(0), lane(0), plate(nullptr) {}
    virtual ~Vehicle() { delete plate; }
    virtual std::string getType() { return "Vehicle"; }
};