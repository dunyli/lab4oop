#pragma once
#include <string>
#include "Vehicle.h"
class Violation {
public:
    Vehicle* vehicle;
    Violation(Vehicle* v) : vehicle(v) {}
    virtual ~Violation() {}
    virtual std::string getDescription() { return "Violation"; }
};