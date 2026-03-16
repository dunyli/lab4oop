#pragma once
#include "Violation.h"
class BusLaneViolation : public Violation {
public:
    BusLaneViolation(Vehicle* v) : Violation(v) {}
    std::string getDescription() override { return "Bus lane"; }
};