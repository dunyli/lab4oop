#pragma once
#include "Violation.h"

class Rule {
public:
    virtual ~Rule() {}
    virtual Violation* check(Vehicle* v) { return nullptr; }
};