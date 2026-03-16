#pragma once
#include "Violation.h"
class LineCrossViolation : public Violation {
public:
    LineCrossViolation(Vehicle* v) : Violation(v) {}
    std::string getDescription() override { return "Line cross"; }
};