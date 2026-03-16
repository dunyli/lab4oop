#pragma once
#include "Rule.h"
#include "RedLightViolation.h"
class TrafficLightRule : public Rule {
public:
    Violation* check(Vehicle* v) override {
        if (rand() % 100 < 30) return new RedLightViolation(v);
        return nullptr;
    }
};