#pragma once
#include "Vehicle.h"
#include <string>
#include <ctime>
using namespace std;

class Violation {
public:
    Vehicle* vehicle;
    string zoneName;
    long timestamp;

    Violation(Vehicle* v, const string& zone, long time)
        : vehicle(v), zoneName(zone), timestamp(time) {}

    virtual ~Violation() {}
    virtual string getDescription() { return "Violation"; }
    virtual int getFine() { return 0; }
};