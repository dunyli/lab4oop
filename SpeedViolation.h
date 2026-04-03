#pragma once
#include "Violation.h"
using namespace std;

class SpeedViolation : public Violation {
private:
    int actualSpeed;
    int speedLimit;
public:
    SpeedViolation(Vehicle* v, int speed, int limit, const string& zone, long time)
        : Violation(v, zone, time), actualSpeed(speed), speedLimit(limit) {}

    string getDescription() override {
        return "Превышение скорости: " + to_string(actualSpeed) +
            " > " + to_string(speedLimit) + " км/ч";
    }

    int getFine() override {
        int excess = actualSpeed - speedLimit;
        if (excess <= 20) return 500;
        if (excess <= 40) return 1500;
        return 3000;
    }
};