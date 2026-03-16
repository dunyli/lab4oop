#pragma once
#include "Violation.h"
class SpeedViolation : public Violation {
public:
    int speed, limit;
    SpeedViolation(Vehicle* v, int s, int l) : Violation(v), speed(s), limit(l) {}
    std::string getDescription() override {
        return "Speed: " + std::to_string(speed) + " > " + std::to_string(limit);
    }
};