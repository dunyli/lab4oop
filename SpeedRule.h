#pragma once
#include "Rule.h"
#include "Violation.h"
#include <string>
using namespace std;

class SpeedViolation : public Violation {
public:
    int speed, limit;

    SpeedViolation(Vehicle* v, int s, int l) : Violation(v), speed(s), limit(l) {}

    string getDescription() override {
        return "Speed: " + to_string(speed) + " > " + to_string(limit);
    }
};

class SpeedRule : public Rule {
private:
    int limit;

public:
    SpeedRule(int l) : limit(l) {}

    Violation* check(Vehicle* v) override {
        if (v->speed > limit) {
            return new SpeedViolation(v, v->speed, limit);
        }
        return nullptr;
    }
};