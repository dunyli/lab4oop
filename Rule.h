#pragma once
#include "Violation.h"
#include "Vehicle.h"
using namespace std;

class Rule {
public:
    virtual ~Rule() {}
    virtual Violation* check(Vehicle* v) = 0;
};