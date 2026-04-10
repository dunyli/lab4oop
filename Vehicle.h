#pragma once
#include <string>
#include "LicensePlate.h"

using namespace std;

class Vehicle {
public:
    string id;
    int speed;
    int lane;
    LicensePlate* plate;

    Vehicle(string i) : id(i), speed(0), lane(0), plate(nullptr) {}

    virtual ~Vehicle() {
        delete plate;
    }

    virtual string getType() { return "Vehicle"; }
};