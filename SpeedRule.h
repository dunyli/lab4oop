#pragma once
#include "Rule.h"
#include "SpeedViolation.h"
class SpeedRule : public Rule {
public:
    int limit;
    SpeedRule(int l) : limit(l) {}
    Violation* check(Vehicle* v) override {
        if (v->speed > limit) return new SpeedViolation(v, v->speed, limit);
        return nullptr;
    }
};