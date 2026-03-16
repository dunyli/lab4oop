#pragma once
#include "Rule.h"
#include "LineCrossViolation.h"
class LineCrossRule : public Rule {
public:
    Violation* check(Vehicle* v) override {
        if (rand() % 100 < 20) return new LineCrossViolation(v);
        return nullptr;
    }
};