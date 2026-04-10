#pragma once
#include "Violation.h"

class ClonableViolation : public Violation {
public:
    ClonableViolation(Vehicle* v) : Violation(v) {}
    virtual ClonableViolation* clone() = 0;
};