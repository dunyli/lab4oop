#pragma once
#include <string>
#include "Vehicle.h"

using namespace std;

class Violation {
public:
    Vehicle* vehicle;

    Violation(Vehicle* v) : vehicle(v) {}
    virtual ~Violation() {}
    virtual string getDescription() { return "Violation"; }
};