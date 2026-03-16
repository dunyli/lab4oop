#pragma once
#include "Violation.h"
class RedLightViolation : public Violation {
public:
    RedLightViolation(Vehicle* v) : Violation(v) {}
    std::string getDescription() override { return "Red light"; }
};